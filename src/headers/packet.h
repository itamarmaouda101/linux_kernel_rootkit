#include <linux/skbuff.h>
#define IP

struct packet_info ={
    unsigned int src_ip,
    unsigned int dst_ip,
    struct tcphdr *tcp_header,
    unsigned int src_port,
    unsigned int dst_port

};
int need_to_hide(struct sk_buff *skb)
{
    skb->protocol
    if (skb->protocol == htons(ETH_P_IP))
    {
        if()
    }
}

