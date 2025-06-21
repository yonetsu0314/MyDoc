/******************************************************************************
* NORTi Network IP Module                                                     *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Mar/1999 Created.                                                        *
* 03/Oct/2000 (Ver. 4.02 Release)                                             *
* 01/Dec/2000 Last Update.(Ver. 4.03 Release)                                 *
* 31/Jan/2002 IP reassembling & Fragmentation implemented                     *
* 1/Aug/2002 - Fixed IP reassembly memory releasing problems                  *
* 28/Aug/2002 Fixed Memory pool for IP reassembly                             *
* 14/Feb/2003 Added timer function for ARP and IP reassembly                  *
*             The ip_snd_tsk are modularized as ip_set_hd4 and ip_frag_pkt.   *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"
#ifdef PPP
#include "noneppp.h"
#endif

/* To check IP reassembly process */
extern BOOL ipf_timer_flag;

/************************************************************************************
* Module Name   : tcpip_timer_func (Cyclic timer handler)                           *
* Purpose       : This timer function checks the timeout for ARP entry, ARP request *
*                   retransmission and for IP reassembly process.                   *
* Arguments     : None.                                                             *
* Return value  : TMRHDR handler for the timer.                                     *
* Author        : AK                                                                *
* Date          : 24-Feb-2003                                                       *
************************************************************************************/
TMRHDR tcpip_timer_func(void)
{
    static UB timertick;

    timertick++;
    
    if (timertick >= 10)
        timertick = 0;

    if (timertick == 0) {       /* 1 sec */
        ip_timer++;
#ifndef PPP
        if ((ip_timer % ARP_RET_INTVAL) == 0)
            arp_req_retry();

        /* Validate ARP entries for timeout */
        val_arp_table();
#endif
        /* Check for IP reassembly timeout */
        if (ipf_timer_flag) 
            ip_frag_timeout();

    }
}

/*******************************************/
/* IP internal error                       */
/*******************************************/

ER ip_internal_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

/*******************************************/
/* check for Broadcast IP address          */
/*******************************************/

BOOL is_broadcast(UW addr)
{
    UW myip, mask;

    mask = byte4_to_long(subnet_mask);
    myip = byte4_to_long(default_ipaddr);
    if (addr == 0xFFFFFFFF) {
        return TRUE;
    }
    else if ((addr & mask) == (myip & mask)) {
        if (((addr &~mask) | mask) == 0xFFFFFFFF)
            return TRUE;
        if ((addr | mask) == mask)
            return TRUE;
    }

    return FALSE;

}

/*******************************************/
/* check for host IP address               */
/*******************************************/

BOOL is_my_ipaddr(UW addr)
{
    int i;

    if (addr == byte4_to_long(default_ipaddr)) {
        SNMP_BYPCNT(snmp_mibbyp_if[0].ifInUcastPkts, 1);        /* ifInUcastPkts */
        return TRUE;
    }
    for (i = 0; i < TCP_REPID_MAX; i++) {
        if (addr == TCP_REP[i].ipaddr){
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifInUcastPkts, 1);   /* ifInUcastPkts */
            return TRUE;
        }
    }
    for (i = 0; i < TCP_CEPID_MAX; i++) {
        if (addr == TCP_CEP[i].ipaddr) {
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifInUcastPkts, 1);   /* ifInUcastPkts */
            return TRUE;
        }
    }
    for (i = 0; i < UDP_CEPID_MAX; i++) {
        if (addr == UDP_CEP[i].ipaddr) {
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifInUcastPkts, 1);   /* ifInUcastPkts */
            return TRUE;
        }

#ifndef PPP
        /* Broadcasting Packet ? */

        else if (UDP_CEP[i].options & UDP_BROADCAST) {
            if (is_broadcast(addr))
                return TRUE;
        }

        /* Multicast packet ? */

        else if ((UDP_CEP[i].options & UDP_MULTICAST) && (-1 != search_igmp_ip(addr)) ){
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifInNUcastPkts, 1);   /* ifInNUcastPkts */
            return TRUE;
        }
#endif

    }

    return FALSE;
}

/*******************************************/
/* Compute IP checksum                     */
/*******************************************/

static UH ip_checksum(T_IP *pkt)
{
    UW sum;
    UH *p;

    p = (UH *)&pkt->ip;
    sum  = *p++;        /* ver, tos */
    sum += *p++;        /* tl */
    sum += *p++;        /* id */
    sum += *p++;        /* fo */
    sum += *p++;        /* ttl,prot */
    p++;                /* hc */
    sum += *p++;        /* sa(high) */
    sum += *p++;        /* sa(low) */
    sum += *p++;        /* da(high) */
    sum += *p++;        /* da(low) */

    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);
    return (UH)~sum;
}

/*******************************************/
/* Get Ethernet Header informaion          */
/*******************************************/

#ifndef PPP

static BOOL read_ethernet_header(T_ETH *pkt)
{
    int len;

    /* Get Packet length */

    len = pkt->eth.len = lan_received_len();
    if (len == 0)
        return FALSE;

    /* Read Ethernet Header */

    if (len < ETH_HEADER_SZ){
        lan_ignore_pkt();
        return FALSE;
    }
    return lan_read_pkt(pkt->eth.da, ETH_HEADER_SZ);
}

#endif

/*******************************************/
/* Callback function called when reception */
/* IP packet                               */
/*******************************************/

static BOOL chk_callback(T_IP *pkt)
{
    T_IP_CB *b;
    BOOL flg=FALSE;
    IP_CALLBACK func;

    b = pIP_CB;
    while (b != NULL) {
        func = (IP_CALLBACK)b->callback;
        if (func != NULL) {
            if (!(*func)(pkt)){
                flg = TRUE;
                break;
            }
        }
        b = b->next;
    }
    return flg;
}

/*******************************************/
/* Process for received IP packets         */
/*******************************************/

VP ip_reception(T_IP *pkt)
{
    T_IP *temp;

    temp = NULL;
    SNMP_BYPCNT(snmp_mibbyp.ipInReceives, 1);       /* ipInReceives */
    
#ifndef PPP
    if (!lan_read_pkt(&(pkt->ip), IP_HEADER_SZ)) {
        SNMP_BYPCNT(snmp_mibbyp.ipInDiscards, 1);   /* ipInDiscards */
        return pkt;
    }

    if (pkt->ip.prot != PROT_IGMP && (!is_my_ipaddr(ntohl(pkt->ip.da)))) {
        if (pkt->ip.prot == PROT_ICMP && (!is_broadcast(ntohl(pkt->ip.da)))) {
            if (!chk_callback(pkt)) {
                lan_ignore_pkt();
                SNMP_BYPCNT(snmp_mibbyp.ipInAddrErrors, 1);     /* ipInAddrErrors */
                return pkt;
            }
            else
                return NULL;
        }
    }
#else
    if (!ppp_read_pkt(&(pkt->ip), IP_HEADER_SZ)) {
        SNMP_BYPCNT(snmp_mibbyp.ipInDiscards, 1);   /* ipInDiscards */
        return pkt;
    }

    if (!is_my_ipaddr(ntohl(pkt->ip.da))) {
        SNMP_BYPCNT(snmp_mibbyp.ipInAddrErrors, 1); /* ipInAddrErrors */
        return pkt;
    }
#endif

    if (pkt->ip.hc != ip_checksum(pkt)) {
#ifndef PPP
        lan_ignore_pkt();
        SNMP_BYPCNT(snmp_mibbyp.ipInHdrErrors, 1);  /* ipInHdrErrors */
#endif
        return pkt;
    }

#ifndef PPP
    set_arp_table(ntohl(pkt->ip.sa), pkt->eth.sa);
#endif

    if (ntohs(pkt->ip.fo) & 0x3fff) {           /* Receives IP Fragment */
        temp = ip_reassembling(pkt);            /* Call IP reassembling */
        if (temp == NULL) return pkt;           /* Receive all fragments or not */
        else if (!temp->ctl.rcheck)
            return NULL;

        pkt = temp;                            /* Send the reassembled packet to upper layer */
        temp = NULL;
    }

    switch (pkt->ip.prot) {
    case PROT_ICMP:
        SNMP_BYPCNT(snmp_mibbyp.ipInDelivers, 1);  /* ipInDelivers */
        return icmp_reception(pkt);
    case PROT_TCP:
        SNMP_BYPCNT(snmp_mibbyp.ipInDelivers, 1);  /* ipInDelivers */
        return tcp_reception((T_TCP_PKT *)pkt);
    case PROT_UDP:
        SNMP_BYPCNT(snmp_mibbyp.ipInDelivers, 1);  /* ipInDelivers */
        return udp_reception((T_UDP_PKT *)pkt);
#ifndef PPP
    case PROT_IGMP:
        SNMP_BYPCNT(snmp_mibbyp.ipInDelivers, 1);  /* ipInDelivers */
        return igmp_reception(pkt);
    default:
        lan_ignore_pkt();
        SNMP_BYPCNT(snmp_mibbyp_if[0].ifInUnknownProtos, 1); /* ifInUnknownProtos */
        SNMP_BYPCNT(snmp_mibbyp.ipInUnknownProtos, 1);       /* ipInUnknownProtos */
        return pkt;
#else
    return pkt;
#endif
    }
}

/*******************************************/
/* IP Receive Task                         */
/*******************************************/

TASK ip_rcv_tsk(void)
{
    T_ETH *pkt;
    struct iphash_que *reass;
    ER ercd;

    pkt = NULL;

    for (;;) {

        /* Allocate memory for incoming packet */
        if (pkt == NULL) {
            ercd = get_blf((VP *)&pkt, ID_ETH_MPF);
            if (ercd != E_OK) {
                pkt = NULL;
                ip_internal_error(ercd);
                ip_evictor();
                dly_tsk(wait(1000));
                continue;
            }
            memset(&pkt->ctl, 0, sizeof (T_CTL_HEADER));
            pkt->ctl.mpfid = (UB)ID_ETH_MPF;
        }

        /* Wait for the occurrence of a packet */
#ifndef PPP
        ercd = lan_wai_rcv(TMO_FEVR);
        if (ercd != E_OK) {
            ip_internal_error(ercd);
            dly_tsk(wait(1000));
            continue;
        }

        /* Read ethernet header */

        if (!read_ethernet_header(pkt))
            continue;

        /* Process the packet */

        switch (ntohs(pkt->eth.etype)) {
        case ETYPE_IP:
            pkt = ip_reception((T_IP *)pkt);

            /* Clear reassembly fragments if any exists */
            if (pkt != NULL && pkt->ctl.rcheck) {
                reass = pkt->ctl.cep;
                reass->rtime = 1;
                pkt->ctl.rcheck = 0;
                clr_ip_hashque(reass);
                pkt = NULL;
            }
            break;
        case ETYPE_ARP:
            pkt = arp_reception((T_ARP *)pkt);
            break;
        default:
            lan_ignore_pkt();
        }
#else
        ercd = ppp_wai_rcv(TMO_FEVR);
        if (ercd != E_OK) {
            ip_internal_error(ercd);
            dly_tsk(wait(1000));
            continue;
        }

        pkt = ip_reception((T_IP *)pkt);
        /* Clear reassembly fragments if any exists */
        if (pkt != NULL && pkt->ctl.rcheck) {
            reass = pkt->ctl.cep;
            reass->rtime = 1;
            pkt->ctl.rcheck = 0;
            clr_ip_hashque(reass);
            pkt = NULL;
        }
#endif
    }
}

/*******************************************/
/* Set IP header informations for ICMP pkt */
/*******************************************/

static UB *set_icmp_ip_header(T_IP *pkt, int *len1, int *len2)
{
    if(pkt->ctl.hedflg != TRUE)
        pkt->ip.hc = ip_checksum(pkt);

    *len1 = (ntohs(pkt->ip.tl) + ETH_HEADER_SZ);
    *len2 = 0;
    return NULL;
}

/*******************************************/
/* Set IP header informations for UDP pkt  */
/*******************************************/

static UB *set_udp_ip_header(T_UDP_PKT *pkt, int *len1, int *len2)
{
    if (pkt->ctl.hedflg != TRUE) {
        pkt->ip.ver = 0x45;
        pkt->ip.tos = 0;
        pkt->ip.tl = htons( IP_HEADER_SZ + ntohs( pkt->udp.len ) );
        pkt->ip.id = htons(ip_ident++);
        pkt->ip.fo = 0;
        pkt->ip.ttl = IP_DEF_TTL;
        pkt->ip.hc = ip_checksum((T_IP *)pkt);
        *len1 = ETH_HEADER_SZ + IP_HEADER_SZ + UDP_HEADER_SZ;
        *len2 = ntohs(pkt->udp.len) - UDP_HEADER_SZ;
        return pkt->data;
    }
    else {
        *len1 = ETH_HEADER_SZ + ntohs(pkt->ip.tl);
        *len2 = 0;
        return pkt->data;
    }

}

/*******************************************/
/* Set IP header informations for TCP pkt  */
/*******************************************/

static UB *set_tcp_ip_header(T_TCP_PKT *pkt, int *len1, int *len2)
{
    if (pkt->ctl.hedflg != TRUE) {
        pkt->ip.ver = 0x45;
        pkt->ip.tos = 0;
        pkt->ip.id = htons(ip_ident++);
        pkt->ip.fo = 0;
        pkt->ip.ttl = IP_DEF_TTL;
        pkt->ip.hc = ip_checksum((T_IP *)pkt);
    }
    *len1 = ETH_HEADER_SZ + ntohs(pkt->ip.tl);
    *len2 = 0;
    return pkt->data;
}

/*******************************************/
/* Set MAC and IP4 header informations     */
/*******************************************/

static UB *ip_set_hdr4(T_IP **tpkt, int *len1, int *len2)
{
    T_IP *pkt;
    VP data;
    UB ipb4[4];
    ER ercd;
        
    pkt = *tpkt;
    *len1 = 0;

    /* Process IP packet */
#ifndef PPP
    if (ntohs(pkt->eth.etype) == ETYPE_IP) {

        /* Broadcast packet */

        if (is_broadcast(ntohl(pkt->ip.da))) {
            memset(pkt->eth.da, -1, 6);
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutNUcastPkts, 1);   /* ifOutNUcastPkts */
        }

        /* Multicast packet */

        else if ((ntohl(pkt->ip.da) & 0xE0000000) == 0xE0000000) {
            long_to_byte4(ipb4, ntohl(pkt->ip.da));
            pkt->eth.da[5] = ipb4[3];
            pkt->eth.da[4] = ipb4[2];
            pkt->eth.da[3] = (ipb4[1]&0x7F);
            pkt->eth.da[2] = 0x5E;
            pkt->eth.da[1] = 0;
            pkt->eth.da[0] = 1;
            SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutNUcastPkts, 1);   /* ifOutNUcastPkts */
        }

        /* Set Target MAC address */

        else if (byte4_to_long(pkt->eth.da) == 0L) {
            ercd = ref_arp_table(pkt->eth.da, ntohl(pkt->ip.da));
            if (ercd != E_OK) {
                /* ARP request retry timedout */
                if (pkt->ctl.arprty > ARP_RETRY || pkt->ctl.arprty == -1)
                    return NULL;

                /* 12/Jul/02 AK - Dont send ARP REQ for TCP retrans */
                if (pkt->ip.prot != PROT_TCP || pkt->ctl.arprty <= 1)
                    pkt->ctl.arprty++;

                /* To avoid ARP flooding */
                /* Dont send ARP request to same destination */
                /* if the interval time less than 1 second   */
                if (ercd == E_TMOUT) {
                    ercd = snd_msg(ID_IP_RETRY_MBX, (T_MSG *)pkt);
                    if (ercd != E_OK)
                        ip_internal_error(ercd);
                    *tpkt = NULL;
                    return NULL;
                }

                /* Send ARP request */
                if ((pkt->ip.prot == PROT_TCP && pkt->ctl.arprty != 1) || arp_request(ntohl(pkt->ip.da), pkt->ctl.cep)) {
                    ercd = snd_msg(ID_IP_RETRY_MBX, (T_MSG *)pkt);
                    if (ercd != E_OK)
                        ip_internal_error(ercd);
                    *tpkt = NULL;
                }
                return NULL;
            }
            else {
                SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutUcastPkts, 1);   /* ifOutUcastPkts */
            }
        }

#endif
        /* Initialize IP header information */
        switch (pkt->ip.prot) {
            default: /* PROT_ICMP or IGMP */
                data = set_icmp_ip_header(pkt, len1, len2);
                break;
            case PROT_UDP:
                data = set_udp_ip_header((T_UDP_PKT *)pkt, len1, len2);
                break;
            case PROT_TCP:
                data = set_tcp_ip_header((T_TCP_PKT *)pkt, len1, len2);
                break;
        }
        SNMP_BYPCNT(snmp_mibbyp.ipOutRequests, 1);  /* ipOutRequests */

#ifndef PPP

    }

    /* For ARP Packet */
    else {
        *len1 = pkt->eth.len;
        *len2 = 0;
        data = NULL;
    }

    /* Set MAC address */
    memcpy(pkt->eth.sa, ethernet_addr, 6);
#endif
    return data;
}

/*******************************************/
/* IP fragmentation and transmission       */
/*******************************************/

static ER ip_frag_pkt(T_IP *pkt, VP data) {

    struct iphash_que *reass;           /* IP reassembly reference */
    T_IP *tpkt, *npkt, *fpkt;           /* Used to copy fragment packets */
    int len, len1, len2;                /* Variable used for IP fragmentation */
    int num_frag, fsize, count;         /* Fragment variables */
    UB *udpdata;                        /* Pointer to UDP data for fragmentation */
    UB hsize;                           /* Size of UDP header */
    ER ercd;

    udpdata = data;                     /* Data to fragment */
    fpkt = pkt;

#ifndef PPP
    if (ntohs(pkt->eth.etype) == ETYPE_IP && !(ntohs(pkt->ip.fo) == IP_DF)) {
#else
    if (!(ntohs(pkt->ip.fo) == IP_DF)) {
#endif
        len = ntohs(pkt->ip.tl) - IP_HEADER_SZ;

        reass = NULL;
        tpkt = NULL;

        /* Get the number of fragments */
        num_frag = len / PATH_MTU;
        if (len % PATH_MTU != 0)
            num_frag++;

        /* Allocate memory for fragment pkt */
        ercd = get_blf((VP *)&tpkt, ID_ETH_MPF);
        if (ercd != E_OK) {
            ip_internal_error(ercd);
            SNMP_BYPCNT(snmp_mibbyp.ipFragFails, 1);   /* ipFragFails */
            goto clrfrag;
        }

        /* Initialize Control header */
        memset(&tpkt->ctl, 0, sizeof (T_CTL_HEADER));
        hsize  = 8;                                    /* Used to copy UDP header */
            
        npkt = NULL;
        /* Set header informations to fragment packet */
        if (pkt->ip.prot == PROT_UDP)
            memcpy(tpkt, pkt, sizeof(T_UDP_PKT));
        else {
            memcpy(tpkt, pkt, sizeof(T_IP));
            if (pkt->ctl.rcheck) {
                reass = pkt->ctl.cep;
                npkt = reass->frag2;
            }
            if (!npkt) {
                SNMP_BYPCNT(snmp_mibbyp.ipFragFails, 1);   /* ipFragFails */
                goto clrfrag;
            }
        }

        /* Set the data to fragment packet */
        for (count = 0; count < num_frag - 1; count++) {

            if (pkt->ip.prot == PROT_UDP)
                memcpy((UB *)((UB *)tpkt->data + hsize), udpdata + (PATH_MTU * count) - 8 + hsize, PATH_MTU - hsize);
            else {
                memcpy(tpkt->data, pkt->data, PATH_MTU);
                pkt = npkt;
                if (!pkt) {
                    SNMP_BYPCNT(snmp_mibbyp.ipFragFails, 1);   /* ipFragFails */
                    goto clrfrag;
                }
                pkt->ip.prot = PROT_ICMP;
                npkt = (T_IP *)((T_IPF *)npkt)->nextf;
            }
            hsize = 0;
                
            /* Initializes IP header information */
            tpkt->ip.tl = htons(PATH_MTU + IP_HEADER_SZ);
            tpkt->ip.fo  =  htons(IP_MF);           /* Set MORE fragment flag */
            tpkt->ip.fo |= htons((count * PATH_MTU) >> 3 );
            tpkt->ip.hc = ip_checksum((T_IP *)tpkt);

            /* Set len1 and len2 for driver calls */
            len1 = (ntohs(tpkt->ip.tl) + ETH_HEADER_SZ);
            len2 = 0;

            SNMP_BYPCNT(snmp_mibbyp.ipFragCreates, 1);   /* ipFragCreates */
#ifndef PPP
            /* Transmit the fragment packet to network */
            for (;;) {
                ercd = lan_wai_snd(TMO_FEVR);
                if (ercd != E_OK) {
                    ip_internal_error(ercd);
                    goto clrfrag;
                }
                break;
            }
            lan_write_pkt(tpkt->eth.da, len1, tpkt->data, len2);
#else
            /* write data to PPP interface */
            ppp_write_pkt(&(tpkt->ip), (len1-ETH_HEADER_SZ), tpkt->data, len2);
#endif
        }

        /* Build the last fragment */
        fsize = len - (PATH_MTU * count);          /* Get the remaining data */

        if(pkt->ip.prot == PROT_UDP)
            memcpy((UB *)tpkt->data, (UB *)udpdata + (PATH_MTU * count) - 8, fsize);
        else
            memcpy(tpkt->data, pkt->data, fsize);

        tpkt->ip.tl = htons(fsize + IP_HEADER_SZ);
        tpkt->ip.fo = htons((count * PATH_MTU) >> 3 );
        tpkt->ip.hc = ip_checksum((T_IP *)tpkt);

        len1 = (ntohs(tpkt->ip.tl) + ETH_HEADER_SZ);
        len2 = 0;
                
        SNMP_BYPCNT(snmp_mibbyp.ipFragCreates, 1); /* ipFragCreates */
#ifndef PPP
        for (;;) {                                 /* Send the packet */
            ercd = lan_wai_snd(TMO_FEVR);
            if (ercd != E_OK) {
                ip_internal_error(ercd);
                goto clrfrag;
            }
            break;
        }
        lan_write_pkt(tpkt->eth.da, len1, tpkt->data, len2);
#else
        /* write data to PPP interface */
        ppp_write_pkt(&(tpkt->ip), (len1-ETH_HEADER_SZ), tpkt->data, len2);
#endif
        SNMP_BYPCNT(snmp_mibbyp.ipFragOKs, 1);   /* ipFragOKs */
clrfrag:
        ercd = rel_blf(ID_ETH_MPF, tpkt);
        if (ercd != E_OK)
            ip_internal_error(ercd);
    }

    /* Release memory allocated for reassembly packet */
    if (fpkt->ip.prot != PROT_UDP && reass) {
        reass->rtime = 0;
        clr_ip_hashque(reass);
        pkt = NULL;
        reass = NULL;
        return E_OK;
    }

    return 1;
}

/*******************************************/
/* IP Send Task                            */
/*******************************************/

TASK ip_snd_tsk(void)
{
    T_IP *pkt = NULL;
    VP data;
    int len1, len2;
    ER ercd;
    ID tskid;
    TCP_CALLBACK func;
    TMO t;
    UW  ipaddr;

    /* Used to Check IP reassembly process */
    ipf_timer_flag = FALSE;

#ifndef PPP
    /* To send initial ARP broadcast packet */
    ipaddr = byte4_to_long(default_ipaddr);
    if(ipaddr != 0)
        arp_request(ipaddr, NULL);
#endif

    for (;;) {
        if (pkt != NULL) {

            /* Release waiting task */

            tskid = pkt->ctl.tskid;
            if (tskid != FALSE) {

                /* ARP request retry timed out */

                if (pkt->ctl.arprty > ARP_RETRY && pkt->ctl.arprty != -1) {
                    ercd = rel_wai(tskid);
                    if (ercd != E_OK)
                        ip_internal_error(ercd);
                }
                else if (pkt->ctl.arprty != -1) {
                    ercd = wup_tsk(tskid);
                    if (ercd != E_OK)
                        ip_internal_error(ercd);
                }
            }

            /* Call the callback function if any */

            func = pkt->ctl.callback;
            if (func != NULL) {

                /* Check for ARP request retry timeout */

                if (pkt->ctl.arprty > ARP_RETRY) {
                  pkt->ctl.parblk = E_TMOUT;
                }
                (func)(((T_UDP_CEP *)(pkt->ctl.cep))->cepid, pkt->ctl.fncd, &pkt->ctl.parblk);
            }

            /* Release memory allocated for this packet */
            ercd = rel_blf(pkt->ctl.mpfid, pkt);
            if (ercd != E_OK) {
                ip_internal_error(ercd);
            }
            pkt = NULL;
        }

        /* Check for TCP timeout */

        t = tcp_transmit2();

        /* Check for transmission of ACK packet */

        t = tcp_transmit_ack(t);

        /* Wait to receive packet from upper layer for transmission */
        ercd = trcv_msg((T_MSG **)&pkt, ID_IP_SND_MBX, t);
        if (ercd != E_OK) {
            if (ercd == E_RLWAI || ercd == E_TMOUT)
                continue;
            else {
                ip_internal_error(ercd);
                dly_tsk(wait(1000));
                pkt = NULL;
                continue;
            }
        }

       /* 31Jan2003 AK - Process IPV4 packet */
       if (ntohs(pkt->eth.etype) == ETYPE_IP || ntohs(pkt->eth.etype) == ETYPE_ARP) {

            /* Fill IP header informations and MAC address */
            data = ip_set_hdr4(&pkt, &len1, &len2);
            if (data == NULL && len1 == 0)
                continue;

            /* Fragment packet if total length > PATH_MTU size */
            if (ntohs(pkt->eth.etype) == ETYPE_IP && \
               ((ntohs(pkt->ip.tl) - IP_HEADER_SZ) > PATH_MTU)) {

                /* Construct and send all fragments */
                ercd = ip_frag_pkt(pkt, data);
                if (ercd == E_OK)
                    pkt = NULL;
                continue;
            }
        }

#ifndef PPP
      for (;;) {
           /* Wait to send data to the network driver */
            ercd = lan_wai_snd(TMO_FEVR);
            if (ercd != E_OK) {
                ip_internal_error(ercd);
                continue;
            }
           /* Write data to network driver */
           lan_write_pkt(pkt->eth.da, len1, data, len2);
           break;
      }
#else
      /* write data to PPP interface */
      ppp_write_pkt(&(pkt->ip), (len1-ETH_HEADER_SZ), data, len2);
#endif

    }
}

/*******************************************/
/* Callback function definition            */
/*******************************************/

ER ip_def_cbk(T_IP_CB *b, IP_CALLBACK callback)
{

    T_IP_CB *bb, *bbb;
    ER ercd;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    bbb = (T_IP_CB *)&pIP_CB;
    bb = bbb->next;

    /* Remove IP Control Block if 'callback' is NULL */

    if (callback == NULL) {
        for (;;) {
            if (bb == NULL)
                break;
            if (bb == b) {
                bbb->next = bb->next;
                break;
            }
            bbb = bb;
            bb = bb->next;
        }
    }

    /* Chain IP Control Block */

    else {
        b->callback = (FP)callback;
        for (;;) {
            if (bb == NULL){
                b->next = NULL;
                bbb->next = b;
                break;
            }
            if (bb == b)
                break;
            bbb = bb;
            bb = bb->next;
        }
    }

    return tcpip_cretical(FALSE);
}

/* End */
