#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/if_packet.h>
#include "ebpfids_rf.h"

#define TABLE_SIZE 100

typedef struct flow_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u16 protocol;
    __u16 padding;
}flow_key;

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

typedef struct table_state{
    int count;
    int ring_buffer;
} table_state;

BPF_TABLE("hash", flow_key, int, flow_table, TABLE_SIZE);
BPF_TABLE("array", int, table_state, counter, 1);

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

static __always_inline int calculate_flags(pkt_data *pkt){
    int current_flags = 0;
    __u64 flags[6];
    for(int i = 0; i < sizeof(flags)/sizeof(__u64); i++){
            if(pkt->flags[i])
                flags[i] = 2;
            else
                flags[i] = 1;
            current_flags = current_flags + flags[i];
            if(i!=sizeof(flags)/sizeof(__u64)-1)
                current_flags = current_flags * 10;
        }
    return current_flags;
}

int ebpf_main(struct xdp_md *ctx) {

    // ctx->rx_queue_index = 0 indicates that either the packet was not processed, is begnin or belongs to active flow
    // ctx->rx_queue_index = 1 the packet is a portscan
    // ctx->rx_queue_index = 2 the flow table is full or perf output overflow, and classifications from offload are not viable until restored

    int counter_key = 0;
    table_state* ts = counter.lookup(&counter_key);
    if(ts){
        if(ts->count == TABLE_SIZE){
            ctx->rx_queue_index = 2;
            return XDP_PASS;
        }
    }

    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;
    ctx->rx_queue_index = 0;
    pkt_data pkt = {};

    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;
    
    flow_key key = {pkt.src,pkt.dst,pkt.port16[0],pkt.port16[1],pkt.l4_proto,0};
    int* value = flow_table.lookup(&key);
    if(value)
       return XDP_PASS;
    else if(ts){
        if(ts->ring_buffer == 1){
            ctx->rx_queue_index = 2;
            return XDP_PASS;
        }
    }
    
    int current_flags = calculate_flags(&pkt);
    int rf_pred = predict_C(0,pkt.l4_proto,1,pkt.pkt_len,current_flags);
    
    if(rf_pred != 0)
        ctx->rx_queue_index = 1;  

    return XDP_PASS;
}
