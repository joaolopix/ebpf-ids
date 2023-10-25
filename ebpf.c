#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/types.h>
#include <linux/if_packet.h>
#include <linux/ipv6.h>


struct flow_key {
    __u32 src_ip;
    __u32 dst_ip;
    __u16 src_port;
    __u16 dst_port;
    __u16 protocol;
};

struct flow_value {
    __u64 count;
    __u64 timestamp;
};

struct pkt_data {
	union {
		__be32 src;
		__be32 srcv6[4];
	};
	union {
		__be32 dst;
		__be32 dstv6[4];
	};
	__u16 port16[2];
	__u16 l3_proto;
	__u16 l4_proto;
	__u16 data_len;
	__u16 pkt_len;
	__u32 seq;
};

BPF_TABLE("lru_hash", struct flow_key, struct flow_value, flow_table, 30);

static __always_inline bool parse_udp(void *data, __u64 *offset, void *data_end,struct pkt_data *pkt){

	struct udphdr *udp_header = data + *offset;
    *offset += sizeof(struct tcphdr);
	if (data + *offset > data_end)
        return false;

	pkt->port16[0] = udp_header->source;
	pkt->port16[1] = udp_header->dest;
	return true;
}

static __always_inline bool parse_tcp(void *data, __u64 *offset, void *data_end,struct pkt_data *pkt){

	struct tcphdr *tcp_header = data + *offset;
    *offset += sizeof(struct tcphdr);
	if (data + *offset > data_end)
        return false;

	pkt->port16[0] = tcp_header->source;
	pkt->port16[1] = tcp_header->dest;
	pkt->seq = tcp_header->seq;

	return true;
}

static __always_inline bool parse_ipv4(void *data, __u64 *offset, void *data_end, struct pkt_data *pkt){

	struct iphdr *ipv4_header = data + *offset;
	*offset += sizeof(struct iphdr);
	if (data + *offset > data_end)
            return false;

	pkt->src = ipv4_header->saddr;
	pkt->dst = ipv4_header->daddr;
	pkt->l4_proto = ipv4_header->protocol;

	return true;
}

static __always_inline bool parse_ethernet_header(struct pkt_data *pkt,void *data, void *data_end, __u64 *offset){
    struct ethhdr *ethernet_header = data;
    *offset = sizeof(struct ethhdr);
    if (data + *offset > data_end) 
        return false;

	int ethernet_protocol = ethernet_header->h_proto;
    pkt->l3_proto = bpf_htons(ethernet_protocol);
    return true;

}

static __always_inline bool parse_ip_header(struct pkt_data *pkt,void *data, void *data_end, __u64 *offset){

    if(pkt->l3_proto == ETH_P_IP){
        
        if(!parse_ipv4(data,offset,data_end,pkt))
            return false;
    }

    return true;
}

static __always_inline bool parse_l4data(struct pkt_data *pkt,void *data, void *data_end, __u64 *offset){

    if(pkt->l4_proto == IPPROTO_TCP){
        if(!parse_tcp(data,offset,data_end,pkt))
            return false;
    }
    else if(pkt->l4_proto == IPPROTO_UDP){
        if(!parse_tcp(data,offset,data_end,pkt))
            return false;
    }

    return true;
}

static __always_inline bool packet_parser(struct pkt_data *pkt,void *data, void *data_end){

    __u64 offset;

    if(!parse_ethernet_header(pkt,data,data_end,&offset))
            return false;
    
    if(!parse_ip_header(pkt,data,data_end,&offset))
            return false;
    
    if(!parse_l4data(pkt,data,data_end,&offset))
            return false;
    
    return true;
}


int flow_counter(struct xdp_md *ctx) {

    struct flow_key key = {};

    void *data = (void *)(long)ctx->data; 
    void *data_end = (void *)(long)ctx->data_end;
    struct pkt_data pkt = {};
   
    if(!packet_parser(&pkt,data,data_end))
        return XDP_PASS;

    key.src_ip = pkt.src;
    key.src_port = pkt.port16[0];
    key.dst_ip = pkt.dst;
    key.dst_port = pkt.port16[1];
    key.protocol = pkt.l4_proto;

    struct flow_value *value = flow_table.lookup(&key);
    if (value) {
        value->count++;
        value->timestamp = bpf_ktime_get_ns();
    } else {
        struct flow_value new = {};
        new.count = 1;
        new.timestamp = bpf_ktime_get_ns();
        flow_table.update(&key, &new);
    }

    return XDP_PASS;
}
