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
    __u64 timestamp;
    __u64 duration;
    __u64 transmited_bytes;
    __u64 flags[6];
    __u64 scan;
}flow_value;

typedef struct pkt_data {
    __be32 src;
    __be32 dst;
	__u16 port16[2];
	__u16 l3_proto;
	__u16 l4_proto;
	__u16 data_len;
	__u16 pkt_len;
	__u32 seq;
    bool flags[6];
}pkt_data;


typedef struct tree_node {
    int feature;
    int value;
    int left;
    int right;
} tree_node;


BPF_TABLE("lru_hash", flow_key, flow_value, flow_table, 100);
BPF_TABLE("hash", int, tree_node, tree_nodes, N_NODES);
BPF_TABLE("hash", int, int, tree_roots, N_TREES);

static __always_inline bool parse_udp(pkt_data *pkt, void *data, void *data_end,__u64 *offset){

	struct udphdr *udp_header = data + *offset;
    *offset += sizeof(struct tcphdr);
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
	pkt->seq = tcp_header->seq;
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
    else{
        return false; // for ipv6
    }
    
    return true;
}

static __always_inline bool parse_l4data(pkt_data *pkt,void *data, void *data_end, __u64 *offset){

    if(pkt->l4_proto == IPPROTO_TCP){
        if(!parse_tcp(pkt,data,data_end,offset))
            return false;
    }
    else if(pkt->l4_proto == IPPROTO_UDP){
        if(!parse_tcp(pkt,data,data_end,offset))
            return false;
    }
    else{
        return false; // neither TCP or UDP
    }

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

static __always_inline int predict_MAP(__u64 duration,__u16 protocol, __u16 dst_port,__u64 packet_counter, __u64 transmited_bytes, int current_flags){
    int features[6] = {duration,protocol,dst_port,packet_counter,transmited_bytes,current_flags};
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
                        if (features[node->feature]*10 < node->value) {
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
                        else
                            continue;
                    }
                }
                else
                   continue;
            }
        }
        else
            continue;    
    }

    int most_voted_class = -1;
    int most_voted_votes = 0;
    for (int i=0; i<2; i++) {

        if (votes[i] > most_voted_votes) {
            most_voted_class = i;
            most_voted_votes = votes[i];
        }
    }
    return most_voted_class;
}

static __always_inline void flow_table_add(pkt_data *pkt){
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
            else if(!pkt->flags[i] && value->flags[i] != 2)
                value->flags[i] = 1;
            current_flags = current_flags + value->flags[i];
            current_flags = current_flags * 10;
        }
        if(MODEL_MODE)
            value->scan = predict_MAP(value->duration,key.protocol,key.dst_port,value->packet_counter,value->transmited_bytes,current_flags);
        else
            value->scan = predict_C(value->duration,key.protocol,key.dst_port,value->packet_counter,value->transmited_bytes,current_flags);

    } 
    else {
        flow_value new = {};
        new.packet_counter = 1;
        new.transmited_bytes = pkt->pkt_len;
        new.timestamp = bpf_ktime_get_ns();
        new.duration = 0;
        int current_flags = 0;
        for(int i = 0; i < sizeof(new.flags)/sizeof(__u64); i++){
            if(pkt->flags[i])
                new.flags[i] = 2;
            else if(!pkt->flags[i] && new.flags[i] != 2)
                new.flags[i] = 1;
            current_flags = current_flags + new.flags[i];
            current_flags = current_flags * 10;
        }
        if(MODEL_MODE)
            new.scan = predict_MAP(new.duration,key.protocol,key.dst_port,new.packet_counter,new.transmited_bytes,current_flags);
        else
            new.scan = predict_C(new.duration,key.protocol,key.dst_port,new.packet_counter,new.transmited_bytes,current_flags);

        flow_table.update(&key, &new);
        
    }
}

int ebpf_main(struct xdp_md *ctx) {

    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;
    pkt_data pkt = {};

    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;

    flow_table_add(&pkt);

    return XDP_PASS;
}
