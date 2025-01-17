#include <linux/netlink.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <libnftnl/chain.h>
#include <libmnl/libmnl.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/netfilter/nf_tables.h>
#include <linux/netfilter.h>
#include <linux/types.h>

#define NFT_MSG_GETTRAVNODES 25
#define NFT_MSG_RESETCHAIN 26

struct req_ret {
    struct nlmsghdr nlh;
    struct nfgenmsg genmsg;
    struct nlattr attr;
    char data[0];
};
struct get_ret {
    struct nlmsghdr nlh;
    struct nfgenmsg genmsg;
    struct nlattr attr1;
    unsigned int traversed_nodes;
    struct nlattr attr2;
    unsigned int swaps;
    struct nlattr attr3;
    unsigned int exprs;
};


int main(int argc, char **argv){
    struct mnl_socket *nl;
    char buf [MNL_SOCKET_BUFFER_SIZE];
    struct nlmsghdr *nlh;
    struct nftnl_chain *t = NULL;
    int ret;
    int family;

    if(argc != 4){
        printf("Usage: sudo ./mrf_ctrl <get/reset> <TABLE NAME> <CHAIN NAME>\n");
        return -1;
    }
    if(getuid()){
        printf("execute with sudo...\n");
        return -2;
    }
    family = NFPROTO_IPV4;
    t = nftnl_chain_alloc();
    if(!strncmp(argv[1], "get", 3))
        nlh = nftnl_chain_nlmsg_build_hdr(buf, NFT_MSG_GETTRAVNODES, family, 0, 0);
    else if(!strncmp(argv[1], "reset", 5)) {
        nlh = nftnl_chain_nlmsg_build_hdr(buf, NFT_MSG_RESETCHAIN, family, 0, 0);
    }else {
        printf("Unknown request %s\nShould be \"get\" or \"reset\"", argv[1]);
        return -3;
    }

    nftnl_chain_set_str(t, NFTNL_CHAIN_TABLE, argv[2]);
    nftnl_chain_set_str(t, NFTNL_CHAIN_NAME, argv[3]);
    nftnl_chain_nlmsg_build_payload(nlh, t);

    nl = mnl_socket_open(NETLINK_NETFILTER);
    mnl_socket_bind(nl, 0, MNL_SOCKET_AUTOPID);
    mnl_socket_sendto(nl, nlh, nlh->nlmsg_len);
    ret  = mnl_socket_recvfrom(nl, buf, sizeof(buf));
    if(ret > 0){
        //printf("len: %u type: %hu, flags: %hu, nlmsg_seq: %u, portid: %u\n", ret->nlh.nlmsg_len, ret->nlh.nlmsg_type, ret->nlh.nlmsg_flags, ret->nlh.nlmsg_seq, ret->nlh.nlmsg_pid);
        //printf("family %u, version %d, res_id %d\n", ret->genmsg.nfgen_family, ret->genmsg.version, ret->genmsg.res_id);
        struct req_ret *ret = (struct req_ret *)buf;
        struct get_ret *trav_swaps = (struct get_ret *)buf;
        switch(ret->nlh.nlmsg_type & 0x00ff) {
            case NFT_MSG_RESETCHAIN:
                //printf("attr_len %hu attr_type %hu %s\n", ret->attr.nla_len, ret->attr.nla_type, ret->data);
                printf("%s\n", ret->data);
                break;
            case NFT_MSG_GETTRAVNODES:
                if(trav_swaps->attr1.nla_type == MNL_TYPE_U32){
                    printf("%u\n", ntohl(trav_swaps->traversed_nodes));
                }
                if(trav_swaps->attr2.nla_type == MNL_TYPE_U32){
                    printf("%u\n", ntohl(trav_swaps->swaps));
                }
                if(trav_swaps->attr3.nla_type == MNL_TYPE_U32){
                    printf("%u\n", ntohl(trav_swaps->exprs));
                }
                //     printf("attr_len: %hu attr_type: %hu traversed nodes: %d\n", ret->attr.nla_len, ret->attr.nla_type, ntohl(*(int *)ret->data));
                break;
            default:
                printf("Unknown answer...\n");
                return -4;
        }

    }
    mnl_socket_close(nl);
}
