/******************************************************************************
* NORTi Network System Internal Definitions                                   *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Mar/1999                                                                 *
* 15/Mar/1999 'seq0' is added to T_TCP_CEP.                                   *
* 21/Mar/1999 ICMP APIs are added.                                            *
* 01/May/1999 'time' is added to T_TCP_CEP.                                   *
* 22/May/1999 tcpip_cretical is added.                                        *
* 22/Jul/1999 added about IPB                                                 *
* 21/Sep/1999 added ARP retry count                                           *
* 19/Jan/2000 added options in T_UDP_CEP.                                     *
* 19/Jan/2000 added T_IP_MREQ                                                 *
* 26/Jan/2000 added T_MGRP_CB                                                 *
* 12/May/2000 added data in T_UDP_RCV                                         *
* 17/Apr/2002 added cwnd and ssth to T_TCP_CEP                                *
* 23/Apr/2002 modified SNMP function macro                                    *
* 31/Jan/2002 added ip_reassembling function                                  *
* 23/Mar/2002 added T_TCP_RETRANS structure to T_TCP_CEP                      *
* 13/May/2002 added keep alive timer parameters                               *
* 03/Jul/2002 added sgetpf, smax variable to T_TCP_CEP                        *
* 28/Aug/2002 added T_IPF for IP reassembly                                   *
* 14/Apr/2003 Modified ARP table for static entries                           *
******************************************************************************/

#ifndef NONETS_H
#define NONETS_H
#ifdef __cplusplus
extern "C" {
#endif

/* Resource ID auto assign */
#define AUTO_ID 0

/* TCP Callback Function Type */

typedef ER (*TCP_CALLBACK)(ID, FN, VP);

/* Header for Internal Control */
/* Note: This struct is aligned with T_IPF struct used */
/*       for IP reassembly, any changes in this struct */
/*       should be reflect in T_IPF struct also        */
typedef struct t_ctl_header {
    VP cep;                 /* Pointer to Communication End Point Control Block */
    H tskid;                /* Task ID to Wake Up */
    H mpfid;                /* Memory Pool ID to Release This Packet */
    H fncd;                 /* Function or Event Code for Callback */
    H arprty;               /* ARP Retry count */
    INT parblk;             /* Pointer to Parameter Block for Callback */
    TCP_CALLBACK callback;  /* Function Pointer to Callback */
    UW dummy1;              /* required for align with T_IPF */
    UW dummy2;              /* required for align with T_IPF */
#define REASS 1             /* Reassembled packet */
    UB rcheck;              /* Reassembly check */
    UB hedflg;              /* IP header setup flag */
    UB *dummy3;             /* required for align with T_IPF */
} T_CTL_HEADER;

#define ARP_RETRY 3         /* ARP retry count */

/* Ethernet Header */

typedef struct t_eth_header {
    UH len;                 /* Received Ethernet Frame Length (and filler) */
    UB da[6];               /* Destination MAC Address */
    UB sa[6];               /* Source MAC Address */
    UH etype;               /* Ethernet Type */
} T_ETH_HEADER;

#define ETH_HEADER_SZ    (sizeof (T_ETH_HEADER) - 2)

#define ETYPE_IP    0x0800  /* Ethernet Type of IP Protocol */
#define ETYPE_ARP   0x0806  /* Ethernet Type of ARP Protocol */

/* Ethernet Frame */

typedef struct t_eth {
    struct t_eth *next;     /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    UB data[ETHERNET_MTU];  /* Data */
} T_ETH;

/* ARP Packet */

typedef struct t_arp {
    struct t_arp *next;     /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    UH htype;               /* Hardware Address Type (1) */
    UH ptype;               /* Protocol Address Type (ETYPE_IP) */
    UB hlen;                /* Hardware Address Length (6) */
    UB plen;                /* Protocol Address Length (4) */
    UH ope;                 /* Operation (ARP_REQ or ARP_RES) */
    UB sha[6];              /* Source Hardware Address */
    UB spa[4];              /* Source Protocol Address */
    UB tha[6];              /* Target Hardware Address */
    UB tpa[4];              /* Target Protocol Address */
    UB pad[18];
} T_ARP;

#define ARP_REQ     1       /* ARP Request */
#define ARP_RES     2       /* ARP Response */

#define ARP_PACKET_SZ  60

#define IP_ARP_TIMER 100    /* IP_ARP_TIMER interval 100 msec */

/* IP Header */

typedef struct t_ip_header {
    UB ver;                 /* Version, Internal Header Length */
    UB tos;                 /* Type of Service */
    UH tl;                  /* Total Length */
    UH id;                  /* Identification */
    UH fo;                  /* Flags, Fragment Offset */
    UB ttl;                 /* Time to Live */
    UB prot;                /* Protocol Number */
    UH hc;                  /* Header Checksum */
    UW sa;                  /* Source IP Address */
    UW da;                  /* Destination IP Address */
} T_IP_HEADER;

#define IP_VER       0x0F   /* IP version */
#define IP_HEADER_SZ 20     /* sizeof (T_IP_HEADER) */
#define PROT_ICMP    1
#define PROT_IGMP    2
#define PROT_TCP     6
#define PROT_UDP     17
#define IP_DF        0x4000 /* IP Dont Fragment Flag */
#define IP_MF        0x2000 /* IP More Fragment Flag */
#define IP_OFFSET    0x1FFF /* Mask for fragment offset */

#define PATH_MTU (ETHERNET_MTU - IP_HEADER_SZ)

/* IP Packet */

typedef struct t_ip {
    struct t_ip *next;      /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    T_IP_HEADER ip;         /* IP Header (without option data) */
    B data[1];              /* Data (Variable Size) */
} T_IP;

/* IP fragment header */

typedef struct t_ipf {
    struct t_ip         *nt;                /* pointer to next pakcet - not used    */
    VP                  cep;                /* Control cep structure                */
    T_IP                *prevf;             /* pointer to previous fragment         */
    T_IP                *nextf;             /* pointer to next fragment             */
    INT                 dummy0;             /* required for align with T_IP         */
    UW                  offset;             /* offset of fragment in IP datagram    */
    UW                  end;                /* last byte of data in datagram        */
    UW                  len;                /* length of the fragment               */
    UB                  rcheck;             /* To check for reasssembly packet      */
    UB                  hedf;               /* Control header flag                  */
    UB                  *ptr;               /* pointer to fragment data             */
    B data[1];
}T_IPF;

/* Structure of IP hash queue - header of reassembly buffer */

struct iphash_que {
    struct iphash_que   *next;              /* Pointer to next reassembly buffer            */
    T_IP                *fragments;         /* Pointer to reassembly buffer first fragment  */
    T_IP                *frag2;             /* Pointer to reassembly buffer second fragment */
    UW                  len;                /* Total length of original datagram            */
    W                   rtime;              /* Time for the reassembling process            */
    UH                  ihlen;              /* Length of the IP header                      */
    HNO                 ID_timer;           /* Timer Handler ID                             */
    struct iphash_que   **pprev;            /* Pointer to previous reassembly buffer        */
    T_IP_HEADER         *iph;               /* Pointer to IP header of the reassembly buffer*/
    UB                  data[1];
};

#define IPF_MREQ_BLK    3                   /* Min memory blocks required for IP reassembly  */
#define IPF_UMEM_BLK    9                   /* IP reassembly upper threshold value           */

/* ICMP Header */

typedef struct t_icmp_header {
    UB type;                /* ICMP Type */
    UB code;                /* ICMP Code */
    UH cs;                  /* Checksum */
    UH id;                  /* Identifier */
    UH seq;                 /* Sequence Number */
} T_ICMP_HEADER;

/* ICMP Message */

typedef struct t_icmp_msg {
    UB type;                /* ICMP Type */
    UB code;                /* ICMP Code */
    UH cs;                  /* Checksum */
    UH id;                  /* Identifier */
    UH seq;                 /* Sequence Number */
    UB data[IP_HEADER_SZ+8];/* Option Data (Variable Size) */
} T_ICMP_MSG;

#define TYPE_ECHO_REP    0
#define TYPE_ECHO_REQ    8

/* UDP Header */

typedef struct t_udp_header {
    UH sp;                  /* Source Port Number */
    UH dp;                  /* Destination Port Number */
    UH len;                 /* Packet Length */
    UH cs;                  /* Checksum */
} T_UDP_HEADER;

#define UDP_HEADER_SZ   (sizeof (T_UDP_HEADER))

/* UDP Packet */

typedef struct t_udp_pkt {
    struct t_udp_pkt *next; /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    T_IP_HEADER ip;         /* IP Header (without option data) */
    T_UDP_HEADER udp;       /* UDP Header */
    UB *data;               /* Pointer to UDP Data */
} T_UDP_PKT;

/* TCP Header */

typedef struct t_tcp_header {
    UH sp;                  /* Source Port Number */
    UH dp;                  /* Destination Port Number */
    UW seq;                 /* Sequence Number */
    UW ack;                 /* Acknowledgment Number */
    UH flag;                /* Header Length (Data Offset), Flags */
    UH win;                 /* Window Size */
    UH cs;                  /* Checksum */
    UH urgp;                /* Urgent Pointer */
                            /* Option */
} T_TCP_HEADER;

#define TCP_MTU (ETHERNET_MTU - IP_HEADER_SZ - TCP_HEADER_SZ)

#define TCP_HEADER_SZ   (sizeof (T_TCP_HEADER)) /* 20 */
#define FIN_FLAG    0x01    /* Fin Flag */
#define SYN_FLAG    0x02    /* Synchronize Flag */
#define RST_FLAG    0x04    /* Reset Flag */
#define PSH_FLAG    0x08    /* Push Flag */
#define ACK_FLAG    0x10    /* Acknowledgement Flag */
#define URG_FLAG    0x20    /* Urgent Flag */

#define DACK_TIME wait(200) /* Delayed ACK Time */

/* TCP Packet */

typedef struct t_tcp_pkt {
    struct t_tcp_pkt *next; /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    T_IP_HEADER ip;         /* IP Header (without option data) */
    T_TCP_HEADER tcp;       /* TCP Header */
    UB data[ETHERNET_MTU-IP_HEADER_SZ-TCP_HEADER_SZ]; /* TCP Data */
} T_TCP_PKT;

typedef struct t_tcp_pkt2 {
    struct t_tcp_pkt *next; /* Message Pointer for NORTi3 Mailbox */
    T_CTL_HEADER ctl;       /* Header for Internal Control */
    T_ETH_HEADER eth;       /* Ethernet Header */
    T_IP_HEADER ip;         /* IP Header (without option data) */
    T_TCP_HEADER tcp;       /* TCP Header */
    UB *data[2];            /* Pointer to TCP Data (1st Half & Latter Half) */
    UH len[2];              /* Length of TCP Data (1st Half & Latter Half) */
    UB opt[4];              /* TCP Option Area */
} T_TCP_PKT2;

/* TCP/IP Configuration Information */

typedef struct t_tcp_cfg {
    ID tcp_cepid_max;       /* Maximum Number of TCP CEP ID */
    ID tcp_repid_max;       /* Maximum Number of TCP REP ID */
    ID udp_cepid_max;       /* Maximum Number of UDP CEP ID */
    ID tcp_tskid_top;       /* Top Number of Task ID */
    ID tcp_semid_top;       /* Top Number of Semaphore ID */
    ID tcp_mbxid_top;       /* Top Number of Mailbox ID */
    ID tcp_mpfid_top;       /* Top Number of Fixed Memory Pool ID */
    INT arp_table_cnt;      /* Count of ARP Table */
    ID eth_qcnt;            /* Send/Receive Queue count */
    UH tcp_syn_ret;         /* Max of TCP SYN retransmission */
    UH tcp_dat_ret;         /* Max of TCP data retransmission */
    UH tcp_rto_ini;         /* TCP Initial retransmission timeout */
    UH tcp_rto_ubound;      /* TCP Max retransmission timeout */
    UH tcp_rto_lbound;      /* TCP Min retransmission timeout */
    UH tcp_ktime_ini;       /* TCP Initial keep alive timeout */
    UH tcp_ktime_pro;       /* TCP Total timeout for keep alive probe */
    UH tcp_ktime_suc;       /* TCP Keep alive probe interval */
    UH ip_def_ttl;          /* TTL value for IP packets */
    UH ip_reasm_tmo;        /* IP reassembly process timeout */
    UH ip_reass_max;
    UH arp_ret_intval;      /* ARP request retransmission timeout */
    UW arp_cache_tout;      /* ARP cache timeout */
} T_TCP_CFG;

/* TCP Reception Point Control Block */

typedef struct t_tcp_rep {
    UW ipaddr;              /* IP Address */
    UH portno;              /* Port Number (0:REP Not Created) */
    UB repid;               /* TCP Reception Point ID */
} T_TCP_REP;

#define KTIME_PRES  wait(1000)  /* Keep alive timer precision */

/* TCP retransmission structure */
/* Mod 13Mar2003 - rtime added  */
typedef struct t_tcp_retrans{
    UW srtt;                /* Smooth round trip time   */
    UW rttvar;              /* Round trip time variance */
    UW rto;                 /* Retransmission timeout value for next segment */
    UW seq;                 /* Sequence number of retransmitting segment */
    UW rtime;               /* Used for rtt calculation */
    UW retries;             /* Number of retransmission occured */
    UW retry_cnt;           /* Max number of retransmission allowed */
#define TCP_RET_ON  1
#define TCP_RET_OFF 0
    B status;               /* Status of TCP timer */
    B ref;
    UH flag;                /* Used to avoid retransmission of ACK segment */
}T_TCP_RETRANS;

#define TCP_ACK_DELAY   0x01
#define TCP_ACK_NOW     0x02
#define TCP_ACK_OUTD    0x04
#define TCP_ZERO_PROBE  0x08

/* TCP sequence number increment value - added AK 11/Jul/02 */
#define TCP_ISSINC (64*1024L)

#define MIN(x, y)   ((x < y) ? x : y)
#define MAX(x, y)   ((x > y) ? x : y)

/* TCP Communication End Point Control Block */

typedef struct t_tcp_cep {
    UW ipaddr;              /* Local IP Address */
    UH portno;              /* Local Port Number */
    UH dstpor;              /* Remote Port Number */
    UW dstipa;              /* Remote IP Address */
    UB repid;               /* TCP Reception End Point ID */
    UB cepid;               /* TCP Communication End Point ID */
    UB stskid;              /* ID of Task waiting to Send */
    UB rtskid;              /* ID of Task waiting to Receive */
    UB state;               /* TCP Connection State */
    UB sflag;               /* Sending Flags */
    UB rflag;               /* Received Flags */
    UW mss;                 /* Max Transmission Segment size */
    UW seq0;                /* Local Init. Sequence Number */
    UW seq;                 /* Local Sequence Number */
    UW seq1;                /* The ACK number received last time */
    UW mseq;                /* Max Sequence TCP has send */
    UW syn;                 /* Remote Init. Sequence Number */
    UW ack;                 /* Remote Sequence Number (Ack. Number) */
    UH win0;                /* Initial Remoto Window Size */
    UH win;                 /* Window Size */
    UH win1;                /* Current Remoto Window Size */
    UB *sbuf;               /* Top Address of Sending Window Buffer */
    INT sbufsz;             /* Size of Sending Window Buffer */
    UB *rbuf;               /* Top Address of Receiving Window Buffer */
    INT rbufsz;             /* Size of Receiving Window Buffer */
    UW sgetp;               /* Get Pointer for Sending */
    UW sputp;               /* Put Pointer for Sending */
    UW rgetp;               /* Get Pointer for Receiving */
    UW rputp;               /* Put Pointer for Receiving */
    FN sfncd;               /* Send function code */
    FN rfncd;               /* Recv function code */
    INT slen;
    INT rlen;
    VP spar;
    VP rpar;
    TCP_CALLBACK callback;  /* Function Pointer to Callback */
    UW time;                /* Retry Time */
    ID dcls_id;             /* Delayed send FIN */
    T_TCP_PKT *ackpkt;      /* Sending ACK packet */
    UW dtime;               /* Delay ACK time */
    UW hseq;                /* Max send seq when retransmission occurs */
    UB dackcnt;             /* Number of Delay ACK received */
    UB dackflag;            /* Delay ACK flag */
    BOOL dackmd;            /* Receive ACK of retransmit packet */
    UW sgetpf;              /* Sequence of first delay ACK */
    UW cwnd;                /* Congestion window size */
    UW ssth;                /* Slow start threshold value */
    W  smax;                /* Usuable window size */
    T_TCP_RETRANS retry;    /* Used for retransmission */
    UW ktime;               /* Keep alive time         */
    UW itime;               /* Connection idle time    */
} T_TCP_CEP;

/* CEP state */

#define CEP_UNUSED       0x01   /* Unused */
#define CEP_WAIT_P_OPEN  0x02   /* Waiting for the Passive Open */
#define CEP_WAIT_A_OPEN  0x04   /* Waiting for the Active Open */
#define CEP_CONNECTING   0x10   /* Connecting */
#define CEP_TERMINATED   0x20   /* Terminated (FIN Sent) */
#define CEP_DISCONNECTED 0x40   /* Disconnected (RST Received) */
#define CEP_CLOSING      0x08   /* Closing */

#define IS_CREATED(s)       ((s) != 0)
#define IS_USED(s)          ((s) > CEP_UNUSED)

/* UDP Communication End Point Control Block */

typedef struct t_udp_cep {
    UW ipaddr;              /* IP Address */
    UH portno;              /* Port Number */
    UB cepid;               /* UDP Communication End Point ID */
#define UDP_UNICAST   0
#define UDP_BROADCAST 1
#define UDP_MULTICAST 2
    UH options;             /* Option Flag */
    UW maddr;               /* Multcast Group Address Entry */
    UB id_udp_mbx;
    UB id_udp_mpf;
    T_UDP_PKT *pkt;
    TCP_CALLBACK callback;  /* Function Pointer to Callback */
} T_UDP_CEP;

/* UDP Recption Request Message */

typedef struct t_udp_rcv {
    struct t_udp_rcv *next; /* Message Pointer for NORTi3 Mailbox */
    T_IPV4EP *p_dstaddr;
    ER *p_ercd;
    VP buf;
    UH len;
    UB tskid;
} T_UDP_RCV;

/* ICMP Control Block */

typedef struct t_icmp_cb {
    struct t_icmp_cb *next; /* Pointer to Next Control Block */
    FP callback;            /* ICMP Callback Routine */
    UH id;                  /* ICMP Identifier (Using Task ID) */
} T_ICMP_CB;

/* ICMP Callback Function Type */

typedef VP (*ICMP_CALLBACK)(T_ICMP_CB *, T_IP *, T_ICMP_MSG *, INT);

/* ICMP API (NORTi3 Unique) */

ER icmp_def_cbk(T_ICMP_CB *, ICMP_CALLBACK);
ER icmp_snd_dat(UW, UW, T_ICMP_HEADER *, VP, INT);
ER icmp_can_snd(T_ICMP_HEADER *);

/* ARP address type */

#define ARP_UNKNOWN     0x00
#define ARP_DYNAMIC     0x02
#define ARP_STATIC      0x04
#define ARP_PROCESS     0x08

/* ARP Table */

typedef struct t_arp_table {
    UW atype;           /* Address Type */
    UW etime;           /* Elapsed time for ARP refresh */
    UW ipaddr;          /* Network Address */
    UB macaddr[6];      /* Interface Address */
} T_ARP_TABLE;

/* ARP Control Block */

typedef struct t_arp_cb {
    struct t_arp_cb *next; /* Pointer to Next Control Block */
    FP callback;           /* IP Callback Routine */
} T_ARP_CB;

/* ARP Callback Function Type */

typedef VP (*ARP_CALLBACK)(T_ARP *arp_pkt);

/* IP Control Block */

typedef struct t_ip_cb {
    struct t_ip_cb *next; /* Pointer to Next Control Block */
    FP callback;          /* IP Callback Routine */
} T_IP_CB;

/* IGMP Message */

typedef struct t_igmp_msg {
    UB type;    /* IGMP type */
    UB dummy;
    UH cs;      /* Checksum */
    UW group;   /* Group Address */
} T_IGMP_MSG;

#define IGMP_UNUSED     0
#define IGMP_INQUERY    1
#define IGMP_REPORT     2

/* Argument structure for IP_ADD_MEMBERSHIP and IP_DROP_MEMBERSHIP */

typedef struct t_ip_mreq {
    UW imr_multiaddr;     /* IP Multicast address of group */
    UW imr_interface;     /* Local IP address of interface */
} T_IP_MREQ;

/* Multicast Group control block */

typedef struct t_mgrp_cb{
    UW         ipaddr;    /* IP Address */
    UH         portno;    /* Port Number */
    T_UDP_CEP  *cep;      /* UDP Control End Point Pointer */
} T_MGRP_CB;

/* IP Callback Function Type */

typedef VP (*IP_CALLBACK)(T_IP *ip_pkt);

/* Internal Data */

extern const UB unknown_addr[6];

/* Protocol Stack Internal Functions */

ER tcpip_internal_error(ER);
ER tcpip_cretical(BOOL on);

void remove_queued_packet(ID, VP);
BOOL remove_sndque_udppkt(ID mbxid, VP cep, ER code);
void remove_rcvque_udppkt(ID mbxid, VP buf);

BOOL is_broadcast(UW addr);
BOOL is_my_ipaddr(UW);
ER ref_arp_table(UB *, UW);
void set_arp_table(UW, const UB *);
VP arp_reception(T_ARP *);
BOOL arp_request(UW, VP);
void arp_req_retry(void);
void val_arp_table(void);
void ini_arp_table(void);
void val_arp_table(void);

/* IP reassembly internal functions */
T_IP *ip_reassembling(T_IP *pkt);
void ip_frag_timeout(void);
TMRHDR tcpip_timer_func(void);
void ip_evictor(void);
void clr_ip_hashque(struct iphash_que *p_hq);

T_IP *icmp_reception(T_IP *);

TASK ip_snd_tsk(void);
TASK ip_rcv_tsk(void);

ER ip_def_cbk(T_IP_CB *b, IP_CALLBACK callback);
ER arp_def_cbk(T_ARP_CB *b, ARP_CALLBACK callback);

ER tcp_internal_error(ER);
INT tcp_put_sdat(T_TCP_CEP *cep, const UB *buf, INT len);
INT tcp_get_sbuf(UB **p_buf, T_TCP_CEP *cep);
ER tcp_put_sbuf(T_TCP_CEP *cep, INT len);
INT tcp_get_rdat(UB *, T_TCP_CEP *, INT);
INT tcp_get_rbuf(UB **, T_TCP_CEP *);
ER tcp_rel_rbuf(T_TCP_CEP *cep, INT len);
VP tcp_reception(T_TCP_PKT *);
void tcp_transmit(T_TCP_CEP *);
TMO tcp_transmit2(void);
TMO tcp_transmit_ack(TMO t);
ER tcp_can_cep2(T_TCP_CEP *, FN);
BOOL can_ack_delay(W size, T_TCP_CEP *cep, T_TCP_PKT *pkt);

VP udp_reception(T_UDP_PKT *);

T_IP *igmp_reception(T_IP *pkt);
ER igmp_snd_dat(UB type, UW dstaddr, UW grpaddr);
int search_igmp_group(T_IPV4EP * grp_addr);
void clr_mgrp_cb(T_UDP_CEP *cep);
int search_igmp_ip(UW ipaddr);

/* Driver Functions */

ER lan_error(ER);
UH lan_received_len(void);
BOOL lan_read_pkt(void *, int);
void lan_ignore_pkt(void);
BOOL lan_read_pkt_end(void *, int, int);
BOOL lan_write_pkt(const void *head, int hlen, const void *data, int dlen);
BOOL lan_write_pkt3(VP *ptr, UH *len);

/* Driver Low Level Functions (by User) */

ER lan_ini(UB *);
ER lan_wai_rcv(TMO);
ER lan_wai_snd(TMO);
ER lan_get_len(UH *);
ER lan_get_pkt(void *, int);
ER lan_skp_pkt(int);
ER lan_get_end(void);
ER lan_set_len(int);
ER lan_put_pkt(const void *, int);
ER lan_put_dmy(int);
ER lan_put_end(void);
INTHDR lan_int(void);

/* SNMP Function (Update 04/Apr/2002) */

#ifdef SNMP
#define INCLUDED_FROM_NOSNMP
#include "nonsnmp.h"
extern  T_SNMP_MIBCTL      snmp_mib;
extern  T_SNMP_MIBBYP      snmp_mibbyp;
extern  T_SNMP_MIBBYP_IF  *snmp_mibbyp_if;

#define SNMP_SET(_oid,_value)                           snmp_set_mib_s(_oid,_value)
#define SNMP_CUP(_oid,_prm)                             snmp_cup_mib_s(_oid,_prm)
#define SNMP_UPDTCP(_state,_laddr,_lport,_raddr,_rport) snmp_upd_tcp(_state,_laddr,_lport,_raddr,_rport)
#define SNMP_UPDUDP(_state,_ipadr,_port)                snmp_upd_udp(_state,_ipadr,_port)
#define SNMP_UPDNTM(_type,_ifno,_nadr,_padr)            snmp_upd_ip_ntm(_type,_ifno,_nadr,_padr)
#define SNMP_BYPCNT(_tgt,_value)     \
    if(snmp_mib.iniflg == SNMP_FLG_INIT)   (*(_tgt)) = (*(_tgt)) + (_value)

#else
#define SNMP_CUP(_oid,_prm)
#define SNMP_SET(_oid,_value)
#define SNMP_UPDTCP(_state,_laddr,_lport,_raddr,_rport)
#define SNMP_UPDUDP(_state,_ipadr,_port)
#define SNMP_UPDNTM(_type,_ifno,_nadr,_padr)
#define SNMP_BYPCNT(_type,_value)
#endif

#ifdef __cplusplus
}
#endif
#endif /* NONETS_H */
