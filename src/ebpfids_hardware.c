#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/if_packet.h>
#include "ebpfids_rf.h"

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

typedef struct meta_data{
    pkt_data pkt;
    int rf_pred;
} meta_data;

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

int ebpf_main(struct xdp_md *ctx) {

    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;

    pkt_data pkt = {};

    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;
    
    int current_flags = 0;
    __u64 flags[6];
    for(int i = 0; i < sizeof(flags)/sizeof(__u64); i++){
            if(pkt.flags[i])
                flags[i] = 2;
            else
                flags[i] = 1;
            current_flags = current_flags + flags[i];
            if(i!=sizeof(flags)/sizeof(__u64)-1)
                current_flags = current_flags * 10;
        }
    
    int rf_pred = predict_C(0,pkt.l4_proto,1,pkt.pkt_len,current_flags);

    if (bpf_xdp_adjust_head(ctx, 0 - (int)sizeof(meta_data)))
		return XDP_PASS;

    data_end = (void*)(long)ctx->data_end;
    data = (void*)(long)ctx->data;

    __u64 offset = sizeof(meta_data);

    if (data + offset > data_end)
        return XDP_PASS;

    meta_data *md = data;
	md->pkt = pkt;
    md->rf_pred = rf_pred;

    return XDP_PASS;
}
