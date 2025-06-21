/******************************************************************************
* NORTi3 Network DHCP sample  Header                                          *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 25/Feb/2000                                                                 *
* 12/Feb/2001 Change Task priority 4 -> 5  DHCP_REQTSK_PRI                    *
******************************************************************************/
#ifndef NONDHCP_H
#define NONDHCP_H
#ifdef __cplusplus
extern "C" {
#endif

/* Constant */

#define DHCP_REQTSK_PRI    5    /* DHCP Request Task Priority */
#define DHCP_REQTSK_SSZ    512  /* DHCP Request Task Stack Size */

/* DHCP Callback Routine */

typedef void (*DHCP_CALLBACK)(ER);

/* DHCP Control Block */

typedef struct t_dhcp_cb {
    ID cepid;
    ID tskid;
    ID almid;
    DHCP_CALLBACK callback;
} T_DHCP_CB;

/* DHCP Packet */

typedef struct t_dhcp {
    UB      op;         /* Message Operation */
#define BOOT_REQUEST    1
#define BOOT_REPLY      2
    UB      htype;      /* Hardware Address Type */
    UB      hlen;       /* Hardware Address Length */
    UB      hops;       /* Hardware Option */
    UW      xid;        /* Transaction ID */
    UH      secs;       /* Progress Time */
    UH      flag;       /* Flag field */
    UW      ciaddr;     /* Client IP Address */
    UW      yiaddr;     /* Your Address */
    UW      siaddr;     /* Server IP Address */
    UW      giaddr;     /* Gateway IP Address */
    UB      chaddr[16]; /* Client Hardware Address */
    UB      sname[64];  /* Server Name(Option) */
    UB      file[128];  /* Boot File Name */
#define DHCP_OP_SIZ 312
    UB      option[DHCP_OP_SIZ]; /* Option */
} T_DHCP;

/* DHCP Option */

#define DHCP_OP_SUBNET  1   /* Subnet Mask */
#define DHCP_OP_ROUTER  3   /* Router Address */
#define DHCP_OP_DNS     6   /* Name Server Address */
#define DHCP_OP_ADRREQ  50  /* Address Request */
#define DHCP_OP_ADDRTM  51  /* IP Address Lease Time */
#define DHCP_OP_MSGTYP  53  /* Message Type */
#define DHCP_OP_SERVID  54  /* DHCP Server ID */
#define DHCP_OP_PRMLST  55  /* Parameter Request List */
#define DHCP_OP_RENETM  58  /* Renewal Time */
#define DHCP_OP_REBITM  59  /* Rebinding Time */
#define DHCP_OP_CLIENT  61  /* Client ID */

/* DHCP Message Type */

#define DHCPDISCOVER    1
#define DHCPOFFER       2
#define DHCPREQUEST     3
#define DHCPDECLINE     4
#define DHCPACK         5
#define DHCPNAK         6
#define DHCPRELEASE     7

ER dhcp_get_data(ID cepid);
ER dhcp_reb_data(ID cepid);
ER dhcp_sta_tsk(ID tskid, ID cepid, ID almid, DHCP_CALLBACK callback);

extern UB dhcp_ipaddr[4];
extern UB dns_ipaddr[4];

#ifdef __cplusplus
}
#endif
#endif /* NONDHCP_H */
