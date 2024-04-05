#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/if_packet.h>
#include "ebpfids_rf.h"

typedef struct flow_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u16 protocol;
}flow_key;

typedef struct flow_value {
    __u64 packet_counter;
    __u64 timestamp; // not a feature but used to calculate
    __u64 duration;
    __u64 transmited_bytes;
    __u64 flags[6];
    // end of features
    __u64 scan; // 0 normal, [1-4] scan
    __u64 scan_counter; // counts how many times has it been classified as a scan
    __u64 suspicious; // 0 normal ; 1 change from scan to normal; 2 no longer suspicous , used to trigger deletion of portscan_table
}flow_value;

typedef struct pkt_data {
    __be32 src;
    __be32 dst;
    __u16 port16[2];
    __u16 l3_proto;
    __u16 l4_proto;
    __u16 data_len;
    __u16 pkt_len;
    bool flags[6];
}pkt_data;

typedef struct tree_node {
    int feature;
    int value;
    int left;
    int right;
} tree_node;

typedef struct event {
    int type;
    int ps_method;
    int ps_type;
    __u32 src_ip;
    __u32 dst_ip;
    __u16 dst_port;
    __u16 src_port;
    __u16 protocol;
    __u16 padding;
} event;

typedef struct ps_value {
    __u32 dst_ip; // used to calculate ps_type
    __u16 dst_port; // used to calculate ps_type
    __u64 timestamp; // used to compare to PS_DELAY
    int ps_counter;
    int ps_method; // 0 vertical, 1 horizontal, 2 block
    int ps_type; // type of scan, syn, ack, etc
} ps_value;

typedef struct ring_buffer {
    int flag;
    int lost;
} ring_buffer;

typedef struct meta_data{
    pkt_data pkt;
    int rf_pred;
} meta_data;

typedef struct tail_ctx{
    flow_key fk;
    flow_value fv;
} tail_ctx;

BPF_TABLE("lru_hash", flow_key, flow_value, flow_table, 500);
BPF_TABLE("lru_hash", __u32, ps_value, portscan_table, 50);

BPF_TABLE("array", int, tree_node, tree_nodes, N_NODES);
BPF_TABLE("array", int, int, tree_roots, N_TREES);

BPF_TABLE("array", int, ring_buffer, event_table, 1);
BPF_PERF_OUTPUT(output);

BPF_TABLE("array", int, tail_ctx, tail_table, 1);
BPF_PROG_ARRAY(tail, 1);

static __always_inline bool parse_udp(pkt_data *pkt, void *data, void *data_end,__u64 *offset){

    struct udphdr *udp_header = data + *offset;
    *offset += sizeof(struct udphdr);
    if (data + *offset > data_end)
        return false;
    pkt->port16[0] = udp_header->source;
    pkt->port16[1] = udp_header->dest;

    return true;
}

static __always_inline bool parse_tcp(pkt_data *pkt, void *data, void *data_end,__u64 *offset){

    struct tcphdr *tcp_header = data + *offset;
    *offset += sizeof(struct tcphdr);
    if (data + *offset > data_end)
        return false;

    pkt->port16[0] = tcp_header->source;
    pkt->port16[1] = tcp_header->dest;
    pkt->flags[0] = tcp_header->urg;
    pkt->flags[1] = tcp_header->ack;
    pkt->flags[2] = tcp_header->psh;
    pkt->flags[3] = tcp_header->rst;
    pkt->flags[4] = tcp_header->syn;
    pkt->flags[5] = tcp_header->fin;

	return true;
}

static __always_inline bool parse_ipv4(pkt_data *pkt, void *data, void *data_end,__u64 *offset){

    struct iphdr *ipv4_header = data + *offset;
    *offset += sizeof(struct iphdr);
    if (data + *offset > data_end)
        return false;

    pkt->src = ipv4_header->saddr;
    pkt->dst = ipv4_header->daddr;
    pkt->l4_proto = ipv4_header->protocol;

	return true;
}

static __always_inline bool parse_ethernet_header(pkt_data *pkt,void *data, void *data_end, __u64 *offset){
    struct ethhdr *ethernet_header = data;
    *offset = sizeof(struct ethhdr);
    if (data + *offset > data_end) 
        return false;

    int ethernet_protocol = ethernet_header->h_proto;
    pkt->l3_proto = bpf_htons(ethernet_protocol);

    return true;
}

static __always_inline bool parse_ip_header(pkt_data *pkt,void *data, void *data_end, __u64 *offset){

    if(pkt->l3_proto == ETH_P_IP){
        if(!parse_ipv4(pkt,data,data_end,offset))
            return false;
    }
    else
        return false; // for ipv6

    return true;
}

static __always_inline bool parse_l4data(pkt_data *pkt,void *data, void *data_end, __u64 *offset){

    if(pkt->l4_proto == IPPROTO_TCP){
        if(!parse_tcp(pkt,data,data_end,offset))
            return false;
    }
    else if(pkt->l4_proto == IPPROTO_UDP){
        if(!parse_udp(pkt,data,data_end,offset))
            return false;
    }
    else
        return false; // neither TCP or UDP

    return true;
}

static __always_inline bool packet_parser(pkt_data *pkt,void *data, void *data_end){

    __u64 offset;

    if(!parse_ethernet_header(pkt,data,data_end,&offset))
            return false;
    
    if(!parse_ip_header(pkt,data,data_end,&offset))
            return false;
    
    if(!parse_l4data(pkt,data,data_end,&offset))
            return false;

    pkt->pkt_len = data_end - data;
    pkt->data_len = data_end - data - offset;
    
    return true;
}

static __always_inline int parse_meta_data(struct xdp_md *ctx,pkt_data *pkt,void *data, void *data_end, __u64* rf_pred){

    if(ctx->rx_queue_index==0)
        return false;

     meta_data *md;
    __u64 offset = sizeof(meta_data);
    if (data + offset > data_end)
        return false;

    md = data;
    *pkt = md->pkt;
    *rf_pred = md->rf_pred;

    if(*rf_pred < 0 || *rf_pred > 4)
        return false;
    
    if (bpf_xdp_adjust_head(ctx, (int)sizeof(meta_data)))
		return false;
    return true;

}

static __always_inline int predict_MAP(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags){

    int features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};
    int votes [5] = {0}; // the size should be the same as the number of classes

    int j = 0;
    for (int i = 0; i < N_TREES; i++) {
        int* current_root = tree_roots.lookup(&j);
        j++;
        if(current_root){
            int current_node = *current_root;
            for (int d = 0; d <= MAX_TREE_DEPTH; d++){
                tree_node* node = tree_nodes.lookup(&current_node);
                if(node){
                    if(node->feature < sizeof(features)/sizeof(int) && node->feature >= 0){
                        if (features[node->feature] < node->value) {
                            current_node = node->left;
                        } 
                        else {
                            current_node = node->right;
                        }
                    }
                    else{
                        if(node->value < sizeof(votes)/sizeof(int) && node->value >= 0){
                            votes[node->value] += 1;
                            break;
                        }
                    }
                }
            }
        } 
    }

    int most_voted_class = -1;
    int most_voted_votes = 0;
    for (int i=0; i<sizeof(votes)/sizeof(int); i++) {

        if (votes[i] > most_voted_votes) {
            most_voted_class = i;
            most_voted_votes = votes[i];
        }
    }
    return most_voted_class;
}

static __always_inline int predict_sub_hierachy(flow_key key,int current_flags,int rf_pred){
    if(rf_pred == 0){
        return 0; // normal;
    }
    else{
        if(key.protocol == IPPROTO_UDP){
            return 1; // udp scan
        }
        else if (key.protocol == IPPROTO_TCP){
            if (current_flags == 121111){
                return 2;
            }
            else if (current_flags == 111121){
                return 3;
            }
            else if (current_flags == 121112){
                return 4;
            }
        }
        return 5;
    }

}

static __always_inline void flow_table_add(pkt_data *pkt, flow_key *fk, flow_value *fv, __u64 rf_pred){

    flow_key key = {};

    key.src_ip = pkt->src;
    key.dst_ip = pkt->dst;
    key.src_port = pkt->port16[0];
    key.dst_port = pkt->port16[1];
    key.protocol = pkt->l4_proto;

    flow_value *value = flow_table.lookup(&key);
    
    if (value) {
        value->packet_counter++;
        value->transmited_bytes += pkt->pkt_len;
        value->duration += bpf_ktime_get_ns()-value->timestamp;
        value->timestamp = bpf_ktime_get_ns();
        int current_flags = 0;
        for(int i = 0; i < sizeof(value->flags)/sizeof(__u64); i++){
            if(pkt->flags[i])
                value->flags[i] = 2;
            current_flags = current_flags + value->flags[i];
            if(i!=sizeof(value->flags)/sizeof(__u64)-1)
                current_flags = current_flags * 10;
        }

        __u64 prev_pred = value->scan;

        if(MODEL_MODE) // MODEL_MODE 1 = via ebpf Maps
            value->scan = predict_MAP(value->duration,key.protocol,value->packet_counter,value->transmited_bytes,current_flags);
        else
            value->scan = predict_C(value->duration,key.protocol,value->packet_counter,value->transmited_bytes,current_flags);

        if(prev_pred != 0 && value->scan != prev_pred) // flow was classifed as a scan previously and it changed to normal
            value->suspicious = 1;

        if(value->scan != 0)
            value->scan_counter += 1;

        if(value->suspicious == 2)
            value->suspicious = 0; // remove trigger
        if(value->scan_counter*100/value->packet_counter <= 5 && value->suspicious == 1) // no longer suspicious
            value->suspicious = 2; // trigger deletion event from portscan_table
        
        *fv = *value;
    } 
    else {

        flow_value new = {};
        new.packet_counter = 1;
        new.transmited_bytes = pkt->pkt_len;
        new.timestamp = bpf_ktime_get_ns();
        new.duration = 0;
        new.scan_counter = 0;
        new.suspicious = 0;
        int current_flags = 0;
        for(int i = 0; i < sizeof(new.flags)/sizeof(__u64); i++){
            if(pkt->flags[i]==1)
                new.flags[i] = 2;
            else
                new.flags[i] = 1;
            current_flags = current_flags + new.flags[i];
            if(i!=sizeof(new.flags)/sizeof(__u64)-1)
                current_flags = current_flags * 10;
        }
        if(OFFLOAD_MODE)
            new.scan = predict_sub_hierachy(key,current_flags,rf_pred);
        else{
            if(MODEL_MODE) // MODEL_MODE 1 = via Maps
                new.scan = predict_MAP(new.duration,key.protocol,new.packet_counter,new.transmited_bytes,current_flags);
            else
                new.scan = predict_C(new.duration,key.protocol,new.packet_counter,new.transmited_bytes,current_flags);
        }

        if(new.scan != 0)
            new.scan_counter += 1;

        *fv = new;

        flow_table.update(&key, &new);
    }

    *fk = key;
}

static __always_inline int ps_table_add(flow_key fk, flow_value fv, ps_value *psv){
    
    //returns 0 if a scan was received but did not reach the treshhold
    //returns 1 if threshold as been hit 
    //returns -2 if existing flows were deleted and we must restart count from this one

    __u32 key = fk.src_ip;
    ps_value *value = portscan_table.lookup(&key);

    if(value){ 

        if(fv.timestamp - value->timestamp > PS_DELAY){
            value->dst_ip = fk.dst_ip;
            value->dst_port = fk.dst_port;
            value->timestamp = fv.timestamp;
            value->ps_counter = 1;
            value->ps_method = 0;
            value->ps_type = fv.scan;
            *psv = *value;
            return -2;
        }
  
        if(value->dst_ip != fk.dst_ip)
            value->ps_method = 1; // horizontal
        if(value->ps_method == 1 && value->dst_port != fk.dst_port)
            value->ps_method = 2; //block

        value->dst_ip = fk.dst_ip;
        value->dst_port = fk.dst_port;
        value->timestamp = fv.timestamp;
        value->ps_counter += 1;
        value->ps_type = fv.scan;
        *psv = *value;
        if(value->ps_counter >= PS_THRESHOLD){
            return 1;
        }

    }
    else{
        ps_value new = {};
        new.dst_ip = fk.dst_ip;
        new.dst_port = fk.dst_port;
        new.timestamp = fv.timestamp;
        new.ps_counter = 1;
        new.ps_method = 0;
        new.ps_type = fv.scan;
        portscan_table.update(&key, &new);
        *psv = new;
        if(new.ps_counter >= PS_THRESHOLD){
            return 1;
        }
    }
    return 0;
}

static __always_inline void ps_table_remove(flow_key fk){
    
    __u32 key = fk.src_ip;
    ps_value *value = portscan_table.lookup(&key);
    if(value){
        value->ps_counter -= 1;
        if(value->ps_counter == 0)
            portscan_table.delete(&key);
    }
}

static __always_inline bool perf_output_capable(){

    int key = 0;
    ring_buffer *value = event_table.lookup(&key);
    if(value){
        if(value->flag == 1)
            return true;
        else
            value->lost += 1;
    }
    return false;
}

static __always_inline void event_output(struct xdp_md *ctx, flow_key fk, ps_value psv,int event_type){

    // event_type = 0 flow classifed as a scan
    // event_type = 1 portscan alert
    // event_type = -1 remove stored flow
    // event_type = -2 delete existing flows and restart

    if(perf_output_capable()){
        event e = {event_type,psv.ps_method,psv.ps_type,fk.src_ip,fk.dst_ip,fk.dst_port,fk.src_port,fk.protocol,0};
        output.perf_submit(ctx, &e, sizeof(event));
    }

}

int ebpf_main_tail(struct xdp_md *ctx){

    int key = 0;
    tail_ctx *value = tail_table.lookup(&key);
    if(value){
        flow_key fk = value->fk;
        flow_value fv = value->fv;

        if(fv.scan != 0){
        ps_value psv = {};
        int event_type = ps_table_add(fk,fv,&psv);
        event_output(ctx,fk,psv,event_type);
        if(event_type == 1 && DETECTION_MODE)
            return XDP_DROP;
        }

        else if(fv.suspicious == 2){
            ps_table_remove(fk);
            event_output(ctx,fk,(ps_value){},-1);
        }

        if(OFFLOAD_MODE && fv.packet_counter==2){
            event_output(ctx,fk,(ps_value){},-3);
        }
    }

    return XDP_PASS;
}

int ebpf_main(struct xdp_md *ctx) {

    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;

    pkt_data pkt = {};
    __u64 rf_pred = ctx->rx_queue_index;
    
    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;
 
    flow_key fk = {};
    flow_value fv = {};

    flow_table_add(&pkt,&fk,&fv,rf_pred);

    int key = 0;
    tail_ctx new = {fk,fv};
    tail_table.update(&key, &new);
    tail.call(ctx,0);

    return XDP_PASS;
}
