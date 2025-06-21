/******************************************************************************
* NORTi Network Common Definitions/Declarations                               *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 18/Feb/1999                                                                 *
* 22/Mar/1999 ascii_to_ipaddr,ipaddr_to_ascii are added.                      *
* 23/Sep/1999 CEP,REP ID auto assign added. (Create tcp_vcre_cep,tcp_vcre_rep)*
* 19/Jan/2000 added Connection End Point Option.                              *
* 20/Apr/2000 added dhcp_ipaddr, dns_ipaddr.                                  *
* 25/Apr/2000 added ITRON4.0 error code                                       *
* 14/Apr/2003 added ARP APIs                                                  *
******************************************************************************/

#ifndef NONET_H
#define NONET_H
#ifdef __cplusplus
extern "C" {
#endif

#define VP  void *

/************************************/
/* Data Structure                   */
/************************************/

/* Packet to Store IP Address and Port Number */

typedef struct t_ipv4ep {
    UW ipaddr;              /* IP Address */
    UH portno;              /* Port Number */
} T_IPV4EP;

/* Packet to Create TCP Reception Point */

typedef struct t_tcp_crep {
    ATR repatr;             /* TCP Reception Point Attribute */
    T_IPV4EP myaddr;        /* Local IP Address and Port Number */
} T_TCP_CREP;

/* Packet to Create TCP Communication End Point */

typedef struct t_tcp_ccep {
    ATR cepatr;             /* TCP Communication End Point Attribute */
    VP sbuf;                /* Top Address of Sending Window Buffer */
    INT sbufsz;             /* Size of Sending Window Buffer */
    VP rbuf;                /* Top Address of Receiving Window Buffer */
    INT rbufsz;             /* Size of Receiving Window Buffer */
    FP callback;            /* Address of Callback Routine */
} T_TCP_CCEP;

/* Packet to Create UDP Communication End Point */

typedef struct t_udp_ccep {
    ATR cepatr;             /* UDP Communication End Point Attribute */
    T_IPV4EP myaddr;        /* Local IP Address and Port Number */
    FP callback;            /* Address of Callback Routine */
} T_UDP_CCEP;

/************************************/
/* Common Constants                 */
/************************************/

/* TCP Function Codes */

#define TFN_TCP_CRE_REP (-0x201)
#define TFN_TCP_DEL_REP (-0x202)
#define TFN_TCP_CRE_CEP (-0x203)
#define TFN_TCP_DEL_CEP (-0x204)
#define TFN_TCP_ACP_CEP (-0x205)
#define TFN_TCP_CON_CEP (-0x206)
#define TFN_TCP_SHT_CEP (-0x207)
#define TFN_TCP_CLS_CEP (-0x208)
#define TFN_TCP_SND_DAT (-0x209)
#define TFN_TCP_RCV_DAT (-0x20a)
#define TFN_TCP_GET_BUF (-0x20b)
#define TFN_TCP_SND_BUF (-0x20c)
#define TFN_TCP_RCV_BUF (-0x20d)
#define TFN_TCP_REL_BUF (-0x20e)
#define TFN_TCP_SND_OOB (-0x20f)
#define TFN_TCP_RCV_OOB (-0x210)
#define TFN_TCP_CAN_CEP (-0x211)
#define TFN_TCP_SET_OPT (-0x212)
#define TFN_TCP_GET_OPT (-0x213)
#define TFN_TCP_ALL     0

/* UDP Function Codes */

#define TFN_UDP_CRE_CEP (-0x221)
#define TFN_UDP_DEL_CEP (-0x222)
#define TFN_UDP_SND_DAT (-0x223)
#define TFN_UDP_RCV_DAT (-0x224)
#define TFN_UDP_CAN_CEP (-0x225)
#define TFN_UDP_SET_OPT (-0x226)
#define TFN_UDP_GET_OPT (-0x227)
#define TFN_UDP_ALL     0

/* Event Codes */

#define TEV_TCP_RCV_OOB 0x201   /* Urgent Data Received */
#define TEV_UDP_RCV_DAT 0x221   /* UDP Packet Received */

/* Main Error Codes */

#ifndef E_OK
#ifdef V3
#define E_OK        0       /* Normal Completion */
#define E_SYS       (-5)    /* System Error */
#define E_NOMEM     (-10)   /* Insufficient Memory */
#define E_NOSPT     (-17)   /* Feature Not Supported */
#define E_RSATR     (-24)   /* Reserved Attribute */
#define E_PAR       (-33)   /* Parameter Error */
#define E_ID        (-35)   /* Invalid ID Number */
#define E_NOEXS     (-52)   /* Object Not Existent */
#define E_OBJ       (-63)   /* Invalid Object State */
#define E_MACV      (-65)   /* Memory Access Violation */
#define E_QOVR      (-73)   /* Queuing Overflow */
#define E_DLT       (-81)   /* Object Deleted While Waiting */
#define E_TMOUT     (-85)   /* Polling Failure or Timeout Exceeded */
#define E_RLWAI     (-86)   /* Process Cancelled or Waiting State Forcibly Released */
#else
#define E_OK        0       /* Normal Completion */
#define E_SYS       (-5)    /* System Error */
#define E_NOMEM     (-33)   /* Insufficient Memory */
#define E_NOSPT     (-9)    /* Feature Not Supported */
#define E_RSATR     (-11)   /* Reserved Attribute */
#define E_PAR       (-17)   /* Parameter Error */
#define E_ID        (-18)   /* Invalid ID Number */
#define E_NOEXS     (-42)   /* Object Not Existent */
#define E_OBJ       (-41)   /* Invalid Object State */
#define E_MACV      (-26)   /* Memory Access Violation */
#define E_QOVR      (-43)   /* Queuing Overflow */
#define E_DLT       (-51)   /* Object Deleted While Waiting */
#define E_TMOUT     (-50)   /* Polling Failure or Timeout Exceeded */
#define E_RLWAI     (-49)   /* Process Cancelled or Waiting State Forcibly Released */
#endif
#endif
#ifndef E_WBLK
#define E_WBLK      (-83)   /* Non-blocking Call Accepted */
#define E_CLS       (-87)   /* Connection Disconnected */
#define E_BOVR      (-89)   /* Buffer Overflow */
#endif

/* Timeout Specifications */

#ifndef TMO_POL
#define TMO_POL     0L      /* Polling */
#define TMO_FEVR    (-1L)   /* Wait Forever */
#endif
#ifndef TMO_NBLK
#define TMO_NBLK    (-2L)   /* Non-Blocking Call */
#endif

/* Special IP Address and Port Number */

#define IPV4_ADDRANY    0   /* IP Address Specification Omitted */
#define TCP_PORTANY     0   /* TCP Port Number Specification Omitted */
#define UDP_PORTANY     0   /* UDP Port Number Specification Omitted */

/************************************/
/* Utility Macros or Functions      */
/************************************/

/* Convert Byte Order */

#ifndef LITTLE_ENDIAN
#define htonl(x)    (x)
#define htons(x)    (x)
#define ntohl(x)    (x)
#define ntohs(x)    (x)
#else
UW htonl(UW);
UH htons(UH);
#define ntohl(x)    htonl(x)
#define ntohs(x)    htons(x)
#endif

/* Obtain Error Codes */

#define mainercd(x) ((int)((B)(x)))
#define subercd(x)  ((int)((B)((x)>>8)))

/************************************/
/* Service Call Prototypes          */
/************************************/

/* NORTi3 Network Unique */

ER tcp_ini(void);

/* TCP Service Calls */

ER tcp_cre_rep(ID, const T_TCP_CREP *);
ER tcp_vcre_rep(const T_TCP_CREP *pk_crep);
ER tcp_del_rep(ID);
ER tcp_cre_cep(ID, const T_TCP_CCEP *);
ER tcp_vcre_cep(const T_TCP_CCEP *pk_ccep);
ER tcp_del_cep(ID);
ER tcp_acp_cep(ID, ID, T_IPV4EP *, TMO);
ER tcp_con_cep(ID, const T_IPV4EP *, T_IPV4EP *, TMO);
ER tcp_sht_cep(ID);
ER tcp_cls_cep(ID, TMO);
ER tcp_snd_dat(ID, const VP, INT, TMO);
ER tcp_rcv_dat(ID, VP, INT, TMO);
ER tcp_get_buf(ID, VP *, TMO);
ER tcp_snd_buf(ID, INT);
ER tcp_rcv_buf(ID, VP *, TMO);
ER tcp_rel_buf(ID, INT);
ER tcp_snd_oob(ID, const VP, INT, TMO);
ER tcp_rcv_oob(ID, VP, INT);
ER tcp_can_cep(ID, FN);
ER tcp_set_opt(ID, INT, const VP, INT);
ER tcp_get_opt(ID, INT, VP, INT);

/* UDP Service Calls */

ER udp_cre_cep(ID, const T_UDP_CCEP *);
ER udp_vcre_cep(const T_UDP_CCEP *pk_ccep);
ER udp_del_cep(ID);
ER udp_snd_dat(ID, const T_IPV4EP *, const VP, INT, TMO);
ER udp_rcv_dat(ID, T_IPV4EP *, VP, INT, TMO);
ER udp_can_cep(ID, FN);
ER udp_set_opt(ID, INT, const VP, INT);
ER udp_get_opt(ID, INT, VP, INT);

/* ARP */
ER arp_add_entry(UW , UB *, UW);
ER arp_del_entry(UW);

/* etc. */

ER landump_ini(ID, ID, ID);
UW byte4_to_long(const UB *);
void long_to_byte4(UB *, UW);
UW ascii_to_ipaddr(B *);
INT ipaddr_to_ascii(B *, UW);
UH tcp_portany(void);
#define udp_portany() tcp_portany()   /* 11/Jul/02 - Modified AK */
UW input_ipaddr(B *);
void tcp_set_iss(UW);

/************************************/
/* TCP Well-known Port Numbers      */
/************************************/

#define TCP_PORT_MUX        1       /* TCP Multiplexor */
#define TCP_PORT_RJE        5       /* Remote Job Entry */
#define TCP_PORT_ECHO       7       /* Echo */
#define TCP_PORT_DISCARD    9       /* Discard */
#define TCP_PORT_SYSTAT     11      /* Active Users */
#define TCP_PORT_DAYTIME    13      /* Daytime */
#define TCP_PORT_NETSTAT    15      /* Network Status Program */
#define TCP_PORT_QOTD       17      /* Quote Of The Day */
#define TCP_PORT_CHARGEN    19      /* Character Generator */
#define TCP_PORT_FTPDATA    20      /* File Transfer Protocol (data) */
#define TCP_PORT_FTP        21      /* File Transfer Protocol */
#define TCP_PORT_TELNET     23      /* Terminal Connection */
#define TCP_PORT_SMTP       25      /* Simple Mail Transport Protocol */
#define TCP_PORT_TIME       37      /* Time */
#define TCP_PORT_NAME       42      /* Host Name Server */
#define TCP_PORT_WHOIS      43      /* Who Is */
#define TCP_PORT_NAMESERVER 53      /* Domain Name Server */
#define TCP_PORT_FINGER     79      /* Finger */
#define TCP_PORT_DCP        93      /* Device Control Protocol */
#define TCP_PORT_SUPDUP     95      /* SUPDUP Protocol */
#define TCP_PORT_HOSTNAMES  101     /* NIC Host Name Server */
#define TCP_PORT_ISOTSAP    102     /* ISO-TSAP */
#define TCP_PORT_X400       103     /* X-400 Mail Service */
#define TCP_PORT_X400SND    104     /* X-400 Mail Sending */
#define TCP_PORT_POP3       110     /* Post Office Protocol version3 */
#define TCP_PORT_SUNRPC     111     /* SUN Remote Procedure Call */
#define TCP_PORT_AUTH       113     /* Authentication Service */
#define TCP_PORT_UUCPPATH   117     /* UUCP Path Service */
#define TCP_PORT_NNTP       119     /* USENET News Transfer Protocol */
#define TCP_PORT_PWDGEN     129     /* Password Generator Protocol */
#define TCP_PORT_NETBIOSSSN 139     /* NETBIOS Session Service */
#define TCP_PORT_EPHEMERAL  1050    /* Top Number of Ephemeral Port */

/************************************/
/* UDP Well-known Port Numbers      */
/************************************/

#define UDP_PORT_ECHO       7       /* Echo */
#define UDP_PORT_DISCARD    9       /* Discard */
#define UDP_PORT_SYSTAT     11      /* Active Users */
#define UDP_PORT_DAYTIME    13      /* Daytime */
#define UDP_PORT_NETSTAT    15      /* Who Is Up Or NETSTAT */
#define UDP_PORT_QOTD       17      /* Quote Of The Day */
#define UDP_PORT_CHARGEN    19      /* Character Generator */
#define UDP_PORT_TIME       37      /* Time */
#define UDP_PORT_NAME       42      /* Host Name Server */
#define UDP_PORT_WHOIS      43      /* Who Is */
#define UDP_PORT_NAMESERVER 53      /* Domain Name Server */
#define UDP_PORT_BOOTPS     67      /* Bootstrap Protocol Server */
#define UDP_PORT_BOOTPC     68      /* Bootstrap Protocol Client */
#define UDP_PORT_TFTP       69      /* Trivial File Transfer */
#define UDP_PORT_SUNRPC     111     /* Sun Microsystems RPC */
#define UDP_PORT_NTP        123     /* Network Time Protocol */
#define UDP_PORT_SNMP       161     /* SNMP Net Monitor */
#define UDP_PORT_SNMP_TRAP  162     /* SNMP Trap */
#define UDP_PORT_BIFF       512     /* Unix Comsat */
#define UDP_PORT_WHO        513     /* Unix Rwho Daemon */
#define UDP_PORT_SYSLOG     514     /* System Log */
#define UDP_PORT_TIMED      525     /* Time Daemon */
#define UDP_PORT_EPHEMERAL  1050    /* Top Number of Ephemeral Port */

/************************************/
/* Misc. Macros                     */
/************************************/

#define ETHERNET_MTU        1500    /* Maximum Transfer Unit of Ethernet */

/************************************/
/* User Defined Address             */
/************************************/

extern UB ethernet_addr[6];
extern UB default_ipaddr[4];
extern UB subnet_mask[4];
extern UB default_gateway[4];
extern UB dhcp_ipaddr[4];
extern UB dns_ipaddr[4];

/************************************/
/* Connection End Point Option     */
/************************************/
#define IP_MULTICAST_IF     2    /* IP Multicast Interface   */
#define IP_MULTICAST_TTL    3    /* IP Multicast TimetoLive  */
#define IP_ADD_MEMBERSHIP   5    /* Add  an IP Group Membership */
#define IP_DROP_MEMBERSHIP  6    /* Drop an IP group membership */
#define IP_BROADCAST        10   /* IP Broad cast on/off */

#ifdef __cplusplus
}
#endif
#endif /* NONET_H */
