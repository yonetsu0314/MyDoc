/******************************************************************************
* NORTi Network ICMP Module                                                   *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Nov/1998 Created.                                                        *
*                                                                             *
* 03/Oct/2000 Last Update. (Ver. 4.02 Release)                                *
* 31/Jan/2002 IP reassembling supported (ICMP reception modified)             *
* 28/Aug/2002 Fixed Memory pool for IP reassembly                             *
* 18/Jun/2003 Modified icmp_reception for callback function                   *
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

struct iphash_que *hpkt;

/************************************************/
/* ICMP Internal Error                          */
/************************************************/

ER icmp_internal_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

/***************************************************/
/* Calculate ICMP Checksum                         */
/* 28-Aug-2002 AK -  Modify for computing checksum */
/*                   for Reassembly packets        */
/***************************************************/
static UH icmp_checksum(T_ICMP_MSG *icmp, int len)
{
    T_IP *fpkt;
    int flen;
    UW sum, sumf;
    UH *p;

    sum = 0;
    sumf = 0;

    /* Dont get odd data byte for reassembly packet */
    if (!hpkt) {
        if(len%2){
            sum = (UW)*(UB*)((UW)icmp+len-1);
#ifndef LITTLE_ENDIAN
            sum = (sum << 8);
#endif
        }
    }

    p = (UH *)icmp;
    sum += *p++;        /* type, code */
    p++;                /* checksum */

    /* Compute checksum for normal packet */
    if (!hpkt) {
        len = (len - 4) / 2;
        while (--len >= 0)
            sum += *p++;
    }
    else {
        /* Compute checksum for reassembly packet */
        fpkt = hpkt->fragments;             /* Get first fragment */
        hpkt = NULL;
        if(!fpkt) { return (UH)-1; }
        flen = ((T_IPF *)fpkt)->len;
        flen = (flen - 4) / 2;
        while (len) {
            while (--flen >= 0)
                    sum += *p++;
            len -= flen;
            fpkt = ((T_IPF *)fpkt)->nextf;  /* Next fragment */
            if (!fpkt) 
                break;
            flen = ((T_IPF *)fpkt)->len;
            if (flen%2) {                   /* Get odd data byte */
                sumf = (UW)*(UB*)((UW)fpkt->data + flen -1);
#ifndef LITTLE_ENDIAN
                sumf = (sumf << 8);
#endif
                sum += sumf;
            }
            flen = flen / 2;
            p = (UH *)fpkt->data;
        }
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);
    return (UH)~sum;
}

/************************************************/
/* ICMP Echo Request Reception                  */
/************************************************/

static BOOL icmp_echo_req_reception(T_IP *pkt, T_ICMP_MSG *icmp, INT len)
{
    ER ercd;
    UW a;

    if (icmp->code != 0)
        return FALSE;

    if (pkt->ctl.rcheck)                    /* Reassembly packet */
        hpkt = pkt->ctl.cep;

    icmp->type = TYPE_ECHO_REP;
    icmp->cs = icmp_checksum(icmp, len);
    hpkt = NULL;
    
    if (!is_broadcast(ntohl(pkt->ip.da))) {
        a = pkt->ip.da;
        pkt->ip.da = pkt->ip.sa;
        pkt->ip.sa = a;
    }
    else {
        a = byte4_to_long(default_ipaddr);
        pkt->ip.da = pkt->ip.sa;
        pkt->ip.sa = htonl(a);
    }
    
    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    pkt->ctl.hedflg = FALSE;
    pkt->ctl.arprty = 0;
    ercd = snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
    if (ercd != E_OK) {
        icmp_internal_error(ercd);
        return FALSE;
    }
    pkt->ip.ttl = IP_DEF_TTL;
    SNMP_BYPCNT(snmp_mibbyp.icmpOutMsgs, 1);
    SNMP_BYPCNT(snmp_mibbyp.icmpOutEchosReps, 1);

    return TRUE;
}

/************************************************/
/* ICMP Reception                               */
/************************************************/

T_IP *icmp_reception(T_IP *pkt)
{
    T_ICMP_MSG *icmp;
    T_ICMP_CB *b;
    ICMP_CALLBACK func;
    INT len;

    SNMP_BYPCNT(snmp_mibbyp.icmpInMsgs, 1);

    /* Read ICMP Message */
    hpkt = NULL;

    icmp = (T_ICMP_MSG *)(pkt->data);
    len = ntohs(pkt->ip.tl) - IP_HEADER_SZ;
    
    if (!pkt->ctl.rcheck) {
        /* 23Apr2003 - Should not read more than PATH MTU */
#ifndef PPP
        if (!lan_read_pkt_end(icmp, len, PATH_MTU))
            return pkt;
#else
        if (!ppp_read_pkt_end(icmp, len, PATH_MTU))
            return pkt;
#endif
    }

    if (pkt->ctl.rcheck)                    /* Reassembly packet */
        hpkt = pkt->ctl.cep;

    /* Checksum */
    if (icmp->cs != icmp_checksum(icmp, len)) {
        hpkt = NULL;
        SNMP_BYPCNT(snmp_mibbyp.icmpInErrors, 1);
        return pkt;
    }
    hpkt = NULL;

    /* Each ICMP Process */
    switch (icmp->type) {

    case TYPE_ECHO_REQ:
        SNMP_BYPCNT(snmp_mibbyp.icmpInEchos, 1);
        if (icmp_echo_req_reception(pkt, icmp, len))
            pkt = NULL; /* Packet Area Sent */
        break;
    case TYPE_ECHO_REP:
        SNMP_BYPCNT(snmp_mibbyp.icmpInEchoReps, 1);
    default:
        b = pICMP_CB;
        while (b != NULL) {
            func = (ICMP_CALLBACK)b->callback;
            if (func != NULL) {
                (*func)(b, pkt, icmp, len);
            }
            b = b->next;
        }
        break;
    }
    return pkt;
}

/************************************************/
/* Define ICMP Callback (ICMP API)              */
/************************************************/

ER icmp_def_cbk(T_ICMP_CB *b, ICMP_CALLBACK callback)
{
    T_ICMP_CB *bb, *bbb;
    ER ercd;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    bbb = (T_ICMP_CB *)&pICMP_CB;
    bb = bbb->next;

    /* Remove ICMP Control Block if 'callback' is NULL */

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

    /* Chain ICMP Control Block */

    else {
        b->callback = (FP)callback;
        for (;;) {
            if (bb == NULL)
            {   b->next = NULL;
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

/************************************************/
/* Send ICMP Message (ICMP API)                 */
/************************************************/

ER icmp_snd_dat(UW ipaddr, UW dstaddr, T_ICMP_HEADER *icmp, VP data, INT len)
{
    T_IP *pkt;
    ER ercd;

    hpkt = NULL;

    ercd = get_blf((VP *)&pkt, ID_ETH_MPF);
    if (ercd != E_OK){
        SNMP_BYPCNT(snmp_mibbyp.icmpOutErrors, 1);
        return ercd;
    }
    memset(&pkt->ctl, 0, sizeof (T_CTL_HEADER));
    pkt->ctl.mpfid = (H)ID_ETH_MPF;
    pkt->ctl.cep = icmp; /* for cancel */

    memcpy(pkt->data, icmp, sizeof (T_ICMP_HEADER));
    memcpy(pkt->data + sizeof (T_ICMP_HEADER), data, len);
    len += sizeof (T_ICMP_HEADER);
    icmp = (T_ICMP_HEADER *)pkt->data;
    icmp->cs = icmp_checksum((T_ICMP_MSG *)icmp, len);

    pkt->ip.da = htonl(dstaddr);
    if (ipaddr == IPV4_ADDRANY)
        ipaddr = byte4_to_long(default_ipaddr);
    pkt->ip.sa = htonl(ipaddr);

    pkt->ip.ver = 0x45;
    pkt->ip.tos = 0;
    pkt->ip.tl = htons(IP_HEADER_SZ + len);
    pkt->ip.id = htons(ip_ident++);
    pkt->ip.fo = 0;
    pkt->ip.ttl = IP_DEF_TTL;
    pkt->ip.prot = PROT_ICMP;

    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    pkt->ctl.hedflg = FALSE;
    pkt->ctl.arprty = 0;

    SNMP_BYPCNT(snmp_mibbyp.icmpOutMsgs, 1);

    if (icmp->type == TYPE_ECHO_REQ) {
        SNMP_BYPCNT(snmp_mibbyp.icmpOutEchos, 1);
    }

    return snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
}

/************************************************/
/* Cancel ICMP Message Transfer (ICMP API)      */
/************************************************/

ER icmp_can_snd(T_ICMP_HEADER *icmp)
{
    ER ercd;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;
    remove_queued_packet(ID_IP_SND_MBX, icmp);
    return tcpip_cretical(FALSE);
}

/* end */
