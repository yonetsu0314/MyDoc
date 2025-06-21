/******************************************************************************
* NORTi Network Configuration Header                                          *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Nov/1998                                                                 *
* 22/Mar/1999 pICMP_CB, ID_IP_RETRY_MBX is added.                             *
* 22/May/1999 PRI_TCPIP is added.                                             *
* 26/Jan/2000 mgrp_cb is added.                                               *
* 11/Dec/2000 It became possible, use of mixture of V3 and V4.                *
* 11/Jul/2002 tcp_iss, port_any variables are added.(AK)                      *
* 15/Jul/2002 Macros for TCP retransmission, Keep alive timer are added.      *
* 14/Nov/2002 Macros for TCP, IP reassembly are added.                        *
* 27/Mar/2003 Made equal priority for both IP_SND_TSK and IP_RCV_TSK.         *
* 15/Apr/2003 Added macros for ARP request timeout.                           *
******************************************************************************/

#ifndef NONETC_H
#define NONETC_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef ITRON_H
#define KRNL_V4
#else
#define KRNL_V3
#endif

#include "nonets.h"

/* Network Object ID Definition */

#ifndef INCLUDED_FROM_NONET
#ifndef TCP_REPID_MAX
#define TCP_REPID_MAX   4
#endif
#ifndef TCP_CEPID_MAX
#define TCP_CEPID_MAX   4
#endif
#ifndef UDP_CEPID_MAX
#define UDP_CEPID_MAX   4
#endif
#endif

/* RTOS Object Top ID Definition */

#ifndef INCLUDED_FROM_NONET
#ifndef TCP_TSKID_TOP
#define TCP_TSKID_TOP 0
#endif
#ifndef TCP_SEMID_TOP
#define TCP_SEMID_TOP 0
#endif
#ifndef TCP_MBXID_TOP
#define TCP_MBXID_TOP 0
#endif
#ifndef TCP_MPFID_TOP
#define TCP_MPFID_TOP 0
#endif
#endif

/* RTOS Object ID Count Calcuration */

#define TCP_TSKID_CNT   4
#define TCP_SEMID_CNT   1
#define TCP_MBXID_CNT   (5+(UDP_CEPID_MAX))
#define TCP_MPFID_CNT   (2+(UDP_CEPID_MAX))

/* Multicast Control Block */

#define MULTICAST_GROUP_CNT 10

/* Constants to Create RTOS Object */

#ifndef INCLUDED_FROM_NONET
#ifndef PRI_IP_SND_TSK
#define PRI_IP_SND_TSK  4
#endif
#ifdef PRI_IP_RCV_TSK
#undef PRI_IP_RCV_TSK
#endif
#define PRI_IP_RCV_TSK PRI_IP_SND_TSK
#ifndef SSZ_IP_SND_TSK
#define SSZ_IP_SND_TSK  1024
#endif
#ifndef SSZ_IP_RCV_TSK
#define SSZ_IP_RCV_TSK  1024
#endif
#ifndef ETH_QCNT
#define ETH_QCNT        16
#endif
#ifndef REP_QCNT
#define REP_QCNT        2
#endif
#ifndef TCP_QCNT
#define TCP_QCNT        2
#endif
#ifndef UDP_QCNT
#define UDP_QCNT        2
#endif
#endif

/* MSEC value for protocol stack */

#ifndef INCLUDED_FROM_NONET
UH mSEC = MSEC;
#else
extern UH mSEC;
#define wait(t) (t/mSEC)
#endif

/*********************************************************************************/
/* TCP Retransmission:                                                          */
/*      TCP_RTO_INI     - Timeout Value used for retransmission before          */
/*                          rtt calculations. (secs)                            */
/*      TCP_RTO_LBOUND  - Minimum value of retransmission timeout (secs)        */
/*      TCP_RTO_UBOUND  - Maximum value of retransmission timeout (secs)        */
/*      TCP_SYN_RETRY   - No. of SYN retransmission before connection failure   */
/*      TCP_DATA_RETRY  - No. of Data and FIN retry                              */
/*********************************************************************************/

#ifndef INCLUDED_FROM_NONET
#ifndef TCP_SYN_RETRY
#define TCP_SYN_RETRY       3       /* 3 retry */
#endif
#ifndef TCP_DATA_RETRY
#define TCP_DATA_RETRY      12      /* 12 retry */
#endif
#ifndef TCP_RTO_INI
#define TCP_RTO_INI         3000    /* 3 sec */
#endif
#ifndef TCP_RTO_UBOUND
#define TCP_RTO_UBOUND      64000   /* 64 sec */
#endif
#ifndef TCP_RTO_LBOUND
#define TCP_RTO_LBOUND      300     /* 300 msec */
#endif
#endif

/*********************************************************************************/
/* TCP Keep Alive Timer:                                                        */
/*      TCP_KTIME_INI   - Keep Alive Timeout (secs)                             */
/*                        Set this value zero will disable Keep Alive timer     */
/*      TCP_KTIME_SUC   - Timeout of successive Keep Alive probes (secs)        */
/*      TCP_KTIME_PRO   - Total probing timeout (secs)                          */
/*********************************************************************************/

#ifndef INCLUDED_FROM_NONET
#ifndef TCP_KTIME_INI
#define TCP_KTIME_INI       7200    /* 7200 seconds */
#endif
#ifndef TCP_KTIME_PRO
#define TCP_KTIME_PRO       600     /* 600 seconds */
#endif
#ifndef TCP_KTIME_SUC
#define TCP_KTIME_SUC       75      /* 75 seconds */
#endif
#endif

/*********************************************************************************/
/* ARP:                                                                         */
/*      ARP_TABLE_CNT   - ARP table size                                        */
/*      ARP_RET_INTVAL  - ARP request retransmission interval timeout (secs)    */
/*      ARP_CACHE_TOUT  - ARP cache timeout (secs)                              */
/* IP:                                                                          */
/*      IP_DEF_TTL      - Time To Live value for IP packets                     */
/* IP Reassembly:                                                               */
/*      IPF_REASM_TMO   - Timeout for IP reassembly process (secs)              */
/*      IPHASH_QSZ      - Max number of simultaneous reassembly process         */
/*********************************************************************************/

#ifndef INCLUDED_FROM_NONET
#ifndef ARP_TABLE_CNT
#define ARP_TABLE_CNT       8
#endif
#ifndef ARP_CACHE_TOUT
#define ARP_CACHE_TOUT      1200    /* 20Min (1200 secs) */
#endif
#ifndef ARP_RET_INTVAL
#define ARP_RET_INTVAL      2       /* 2 seconds */
#endif
#ifndef IP_DEF_TTL
#define IP_DEF_TTL          32      /* Default 32 */
#endif
#ifndef IPF_REASM_TMO
#define IPF_REASM_TMO       2       /* 2 Seconds */
#endif
#ifndef IPHASH_QSZ
#define IPHASH_QSZ          2
#endif
#endif

/* Packets to Refer TCP/IP Configuration Information */

#ifndef INCLUDED_FROM_NONET
const T_TCP_CFG TCP_CFG =
{   TCP_CEPID_MAX,
    TCP_REPID_MAX,
    UDP_CEPID_MAX,
    TCP_TSKID_TOP,
    TCP_SEMID_TOP,
    TCP_MBXID_TOP,
    TCP_MPFID_TOP,
    ARP_TABLE_CNT,
    ETH_QCNT,
    TCP_SYN_RETRY,
    TCP_DATA_RETRY,
    TCP_RTO_INI,
    TCP_RTO_UBOUND,
    TCP_RTO_LBOUND,
    TCP_KTIME_INI,
    TCP_KTIME_PRO,
    TCP_KTIME_SUC,
    IP_DEF_TTL,
    IPF_REASM_TMO,
    IPHASH_QSZ,
    ARP_RET_INTVAL,
    ARP_CACHE_TOUT,
};
#else
extern const T_TCP_CFG TCP_CFG;
#define TCP_REPID_MAX   (TCP_CFG.tcp_repid_max)
#define TCP_CEPID_MAX   (TCP_CFG.tcp_cepid_max)
#define UDP_CEPID_MAX   (TCP_CFG.udp_cepid_max)
#define TCP_TSKID_TOP   (TCP_CFG.tcp_tskid_top)
#define TCP_SEMID_TOP   (TCP_CFG.tcp_semid_top)
#define TCP_MBXID_TOP   (TCP_CFG.tcp_mbxid_top)
#define TCP_MPFID_TOP   (TCP_CFG.tcp_mpfid_top)
#define ARP_TABLE_CNT   (TCP_CFG.arp_table_cnt)
#define ETH_QCNT        (TCP_CFG.eth_qcnt)
#define TCP_SYN_RETRY   (TCP_CFG.tcp_syn_ret)
#define TCP_DATA_RETRY  (TCP_CFG.tcp_dat_ret)
#define TCP_RTO_INI     (TCP_CFG.tcp_rto_ini)
#define TCP_RTO_UBOUND  (TCP_CFG.tcp_rto_ubound)
#define TCP_RTO_LBOUND  (TCP_CFG.tcp_rto_lbound)
#define TCP_KTIME_INI   (TCP_CFG.tcp_ktime_ini)
#define TCP_KTIME_PRO   (TCP_CFG.tcp_ktime_pro)
#define TCP_KTIME_SUC   (TCP_CFG.tcp_ktime_suc)
#define ARP_CACHE_TOUT  (TCP_CFG.arp_cache_tout)
#define ARP_RET_INTVAL  (TCP_CFG.arp_ret_intval)
#define IP_DEF_TTL      (TCP_CFG.ip_def_ttl)
#define IPF_REASM_TMO   (TCP_CFG.ip_reasm_tmo)
#define IPHASH_QSZ      (TCP_CFG.ip_reass_max)
extern ID ID_IP_SND_TSK;
extern ID ID_IP_RCV_TSK;
extern ID ID_TCPIP_SEM;
extern ID ID_IP_SND_MBX;
extern ID ID_IP_RETRY_MBX;
extern ID ID_ETH_MPF;
#endif

/* Packets to Create RTOS Object */

#ifndef INCLUDED_FROM_NONET
#ifdef KRNL_V4
const V3_T_CSEM c_tcpip_sem   = { NULL, TA_TFIFO, 0, 1 };
const V3_T_CMBX c_tcpip_mbx   = { NULL, TA_TFIFO|TA_MFIFO };
const V3_T_CMPF c_eth_mpf     = { NULL, TA_TFIFO, ETH_QCNT, sizeof (T_ETH) };
const V3_T_CMPF c_udp_mpf     = { NULL, TA_TFIFO, UDP_QCNT, sizeof (T_UDP_PKT) };
const V3_T_CTSK c_ip_snd_tsk  = { NULL, TA_HLNG, ip_snd_tsk,  PRI_IP_SND_TSK,  SSZ_IP_SND_TSK };
const V3_T_CTSK c_ip_rcv_tsk  = { NULL, TA_HLNG, ip_rcv_tsk,  PRI_IP_RCV_TSK,  SSZ_IP_RCV_TSK };
V3_T_DCYC tcpip_timer         = { NULL, TA_HLNG , tcpip_timer_func, TCY_ON , 0 };
#else
const T_CSEM c_tcpip_sem   = { NULL, TA_TFIFO, 0, 1 };
const T_CMBX c_tcpip_mbx   = { NULL, TA_TFIFO|TA_MFIFO };
const T_CMPF c_eth_mpf     = { NULL, TA_TFIFO, ETH_QCNT, sizeof (T_ETH) };
const T_CMPF c_udp_mpf     = { NULL, TA_TFIFO, UDP_QCNT, sizeof (T_UDP_PKT) };
const T_CTSK c_ip_snd_tsk  = { NULL, TA_HLNG, ip_snd_tsk,  PRI_IP_SND_TSK,  SSZ_IP_SND_TSK };
const T_CTSK c_ip_rcv_tsk  = { NULL, TA_HLNG, ip_rcv_tsk,  PRI_IP_RCV_TSK,  SSZ_IP_RCV_TSK };
T_DCYC tcpip_timer         = { NULL, TA_HLNG , tcpip_timer_func, TCY_ON , 0 };
#endif
#else
#ifdef KRNL_V4
extern const V3_T_CSEM c_tcpip_sem;
extern const V3_T_CMBX c_tcpip_mbx;
extern const V3_T_CMPF c_eth_mpf;
extern const V3_T_CMPF c_udp_mpf;
extern const V3_T_CTSK c_ip_snd_tsk;
extern const V3_T_CTSK c_ip_rcv_tsk;
extern V3_T_DCYC tcpip_timer;
#else
extern const T_CSEM c_tcpip_sem;
extern const T_CMBX c_tcpip_mbx;
extern const T_CMPF c_eth_mpf;
extern const T_CMPF c_udp_mpf;
extern const T_CTSK c_ip_snd_tsk;
extern const T_CTSK c_ip_rcv_tsk;
extern T_DCYC tcpip_timer;
#endif
#define PRI_TCPIP   c_ip_rcv_tsk.itskpri
#endif

/* Internal Data Definition */

#ifndef INCLUDED_FROM_NONET
T_TCP_REP TCP_REP[TCP_REPID_MAX];
T_TCP_CEP TCP_CEP[TCP_CEPID_MAX];
T_UDP_CEP UDP_CEP[UDP_CEPID_MAX];
T_ARP_TABLE arp_table[ARP_TABLE_CNT];
T_MGRP_CB mgrp_cb[MULTICAST_GROUP_CNT];
const T_TCP_REP *TCP_REP_END = &TCP_REP[TCP_REPID_MAX];
const T_TCP_CEP *TCP_CEP_END = &TCP_CEP[TCP_CEPID_MAX];
const T_UDP_CEP *UDP_CEP_END = &UDP_CEP[UDP_CEPID_MAX];
const T_ARP_TABLE *ARP_TABLE_END = &arp_table[ARP_TABLE_CNT];
UH tTCP_RTO_INI;
UH tTCP_RTO_UBOUND;
UH tTCP_RTO_LBOUND;
UH ip_ident;
UW tcp_iss = 0;
UH port_any;
UW ip_timer;
B in_tcp_callback;
T_ICMP_CB *pICMP_CB;
T_IP_CB *pIP_CB;
T_ARP_CB *pARP_CB;
struct iphash_que *ipq_hash[IPHASH_QSZ];    /* IP hash queue array */
#else
extern T_TCP_REP TCP_REP[];
extern T_TCP_CEP TCP_CEP[];
extern T_UDP_CEP UDP_CEP[];
extern T_ARP_TABLE arp_table[];
extern T_MGRP_CB mgrp_cb[];
extern const T_TCP_REP *TCP_REP_END;
extern const T_TCP_CEP *TCP_CEP_END;
extern const T_UDP_CEP *UDP_CEP_END;
extern const T_ARP_TABLE *ARP_TABLE_END;
extern UH tTCP_RTO_INI;
extern UH tTCP_RTO_UBOUND;
extern UH tTCP_RTO_LBOUND;
extern UH ip_ident;
extern UW tcp_iss;
extern UH port_any;
extern UW ip_timer;
extern B in_tcp_callback;
extern T_ICMP_CB *pICMP_CB;
extern T_IP_CB *pIP_CB;
extern T_ARP_CB *pARP_CB;
extern struct iphash_que *ipq_hash[];
#endif

#ifdef __cplusplus
}
#endif
#endif /* NONETC_H */

