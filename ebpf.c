#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/if_packet.h>
#include <linux/ipv6.h>
#include "rf_model.h"

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
    __u64 scan; // 0 scan, 1 normal
    __u64 scan_counter; // counts how many times has it been classified as a scan
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
    int src_ip;
    int dst_ip;
    int dst_port;
    int timestamp; // optional, adjust timestamp in userspace, used for stored flows
} event;

typedef struct ps_index {
    __u32 dst_ip;
    __u16 dst_port;
    __u16 src_port;
    __u64 timestamp;    
} ps_index;

typedef struct ps_value {
    ps_index scans[PS_THRESHOLD];
    int index_counter;
    int ps_type; // 0 vertical, 1 horizontal, 2 block
} ps_value;

BPF_TABLE("lru_hash", flow_key, flow_value, flow_table, 1000);
BPF_TABLE("hash", int, tree_node, tree_nodes, N_NODES);
BPF_TABLE("hash", int, int, tree_roots, N_TREES);
BPF_TABLE("lru_hash", __u32, ps_value, portscan_table, 1000);
BPF_PERF_OUTPUT(output);

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

static __always_inline int predict_MAP(__u64 duration,__u16 protocol,__u64 packet_counter, __u64 transmited_bytes, int current_flags){

    int features[5] = {duration*10,protocol*10,packet_counter*10,transmited_bytes*10,current_flags*10};
    int votes [2] = {0,0}; // the size should be the same as the number of classes

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

static __always_inline void flow_table_add(pkt_data *pkt, flow_key *fk, flow_value *fv){

    flow_key key = {};

    key.src_ip = pkt->src;
    key.dst_ip = pkt->dst;
    key.src_port = pkt->port16[0];
    key.dst_port = pkt->port16[1];
    key.protocol = pkt->l4_proto;

    char flags_to_string[6] = {"UAPRSF"};

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

        if(MODEL_MODE) // MODEL_MODE 1 = via Maps
            value->scan = predict_MAP(value->duration,key.protocol,value->packet_counter,value->transmited_bytes,current_flags);
        else
            value->scan = predict_C(value->duration,key.protocol,value->packet_counter,value->transmited_bytes,current_flags);

         if(!value->scan)
            value->scan_counter += 1;
     
        *fv = *value;
    } 
    else {

        flow_value new = {};
        new.packet_counter = 1;
        new.transmited_bytes = pkt->pkt_len;
        new.timestamp = bpf_ktime_get_ns();
        new.duration = 0;
        new.scan_counter = 0;
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

        if(MODEL_MODE) // MODEL_MODE 1 = via Maps
            new.scan = predict_MAP(new.duration,key.protocol,new.packet_counter,new.transmited_bytes,current_flags);
        else
            new.scan = predict_C(new.duration,key.protocol,new.packet_counter,new.transmited_bytes,current_flags);
        
        if(!new.scan)
            new.scan_counter += 1;

        *fv = new;

        flow_table.update(&key, &new);
    }

    *fk = key;
}

static __always_inline int ps_table_add(flow_key fk, flow_value fv, ps_value *psv){
    /*
    returns 0 if a scan was received but did not reach the treshhold
    returns 1 if threshold as been hit and all stored scans need to be printed
    returns 2 if threshold had already been reached and its new scan
    */
    __u32 key = fk.src_ip;
    ps_value *value = portscan_table.lookup(&key);
    ps_index current_ps = {fk.dst_ip,fk.dst_port,fk.src_port,bpf_ktime_get_boot_ns()};

    if(value){ 
        // is this flow already stored ? if so we update it, also update type of portscan
        bool found = false;
        int ps_type = 0; // vertical
        for(int i = 0; i < sizeof(value->scans)/sizeof(ps_index); i++){
            if(value->scans[i].dst_ip == fk.dst_ip && value->scans[i].dst_port == fk.dst_port && value->scans[i].src_port == fk.src_port){
                value->scans[i] = current_ps;
            }
            if(value->scans[0].dst_ip != value->scans[i].dst_ip || fk.dst_ip != value->scans[i].dst_ip){
                ps_type = 1; // horizontal
            }
            if( (value->scans[0].dst_port != value->scans[i].dst_port || fk.dst_port != value->scans[i].dst_port) && ps_type == 1){
                ps_type = 2; // block
            }
            if(i+1==value->index_counter)
                break;
        }
        value->ps_type = ps_type;

        // threshold already achieved and its not a already existing flow
        if(value->index_counter == PS_THRESHOLD)
            return 2;
        // new flow, add it
        if(value->index_counter < sizeof(value->scans)/sizeof(ps_index) && value->index_counter >= 0){
            value->scans[value->index_counter] = current_ps;
            value->index_counter += 1;
            // did we reach the limit when adding this new flow ?
            if(value->index_counter == PS_THRESHOLD){
                *psv = *value;
                return 1;
            }
        }
    }
    else{
        ps_value new = {};
        new.scans[0] = current_ps;
        new.index_counter += 1;
        portscan_table.update(&key, &new);
    }

    return 0;
}

static __always_inline void ps_table_remove(flow_key fk){
    
    __u32 key = fk.src_ip;
    ps_value *value = portscan_table.lookup(&key);
    if(value){
        value->index_counter -= 1;

        if(value->index_counter == 0){
            portscan_table.delete(&key);
            return;
        }

        bool found = false;
        for(int i = 0; i < sizeof(value->scans)/sizeof(ps_index); i++){
            if(value->scans[i].dst_ip == fk.dst_ip && value->scans[i].dst_port == fk.dst_port && value->scans[i].src_port == fk.src_port){
                found = true;
                if(found && i+1 < sizeof(value->scans)/sizeof(ps_index))
                    value->scans[i] = value->scans[i+1] ;
            }
        }
    }
}

static __always_inline void event_output(struct xdp_md *ctx, flow_key fk, flow_value fv, ps_value psv,int event_type){

    event e = {psv.ps_type,fk.src_ip,fk.dst_ip,0};

    if(event_type == 1){
        for(int i = 0; i < sizeof(psv.scans)/sizeof(ps_index); i++){
            e.dst_port= psv.scans[i].dst_port;
            e.timestamp = (bpf_ktime_get_boot_ns() - psv.scans[i].timestamp)/1000000000; // convert to s to be handled in int, perf cannot receive __u64
            output.perf_submit(ctx, &e, sizeof(event));
        }
        return;
    }

    if(event_type == 2){
        e.dst_port = fk.dst_port;
    }

    output.perf_submit(ctx, &e, sizeof(event));
}

int ebpf_main(struct xdp_md *ctx) {
    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;

    pkt_data pkt = {};

    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;
    
    flow_key fk = {};
    flow_value fv = {};

    flow_table_add(&pkt,&fk,&fv);

    if(!fv.scan){
        ps_value psv = {};
        int event_type = ps_table_add(fk,fv,&psv);
        if(event_type!=0)
            event_output(ctx,fk,fv,psv,event_type);
    }

    else if(fv.scan_counter*100/fv.packet_counter <= 5)
        ps_table_remove(fk);

    return XDP_PASS;
}
