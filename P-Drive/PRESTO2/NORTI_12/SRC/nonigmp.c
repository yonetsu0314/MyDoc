/******************************************************************************
* NORTi Network IGMP Module                                                   *
*                                                                             *
*  Copyright (c) 2000 - 2003, MiSPO Co., Ltd.                                 *
*  All rights reserved.                                                       *
*                                                                             *
* 19/Nov/2000                                                                 *
* 07/Jun/2000 Little endian supported.                                        *
* 25/Jul/2000 Modified igmp_reception                                         *
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

/************************************************/
/* Serch Group                                  */
/************************************************/
int search_igmp_group(T_IPV4EP *gaddr)
{
    int i;

    for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
        if (mgrp_cb[i].ipaddr == gaddr->ipaddr && 
            mgrp_cb[i].portno == gaddr->portno)
            return i;
    }
    return -1;
}

/************************************************/
/* Serch IP address in Group                    */
/************************************************/
int search_igmp_ip(UW ipaddr)
{
    int i;

    for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
        if (mgrp_cb[i].ipaddr == ipaddr)
            return i;
    }
    return -1;
}


/*******************************************************/
/* Clear Multicast Group control block when delete CEP */
/*******************************************************/

void clr_mgrp_cb(T_UDP_CEP *cep)
{
    int i;

    for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
        if (mgrp_cb[i].cep == cep) {
            mgrp_cb[i].ipaddr = 0L;
            mgrp_cb[i].cep = NULL;
            cep->options = 0;
            break;
        }
    }
}

/************************************************/
/* Calculate IGMP Checksum                      */
/************************************************/
static UH igmp_checksum(T_IGMP_MSG *igmp)
{
    UW sum;
    UH *p;

    p = (UH *)igmp;
    sum = *p++;         /* type, code */
    p++;                /* checksum */
    sum += *p++;        /* group address */
    sum += *p++;        /* group address */
    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);
    return (UH)~sum;
}

/************************************************/
/* IGMP Reception                               */
/************************************************/
T_IP *igmp_reception(T_IP *pkt)
{
    T_IGMP_MSG igmp;
    int len,i;

    len = ntohs(pkt->ip.tl) - IP_HEADER_SZ;

    if (!lan_read_pkt_end(&igmp, len, sizeof(T_IGMP_MSG)))
        return pkt;

    /* Checksum */

    if (igmp.cs != igmp_checksum(&igmp))
        return pkt;

    if (!(igmp.type & IGMP_INQUERY))
        return pkt;

    /* Search Join membership */

    for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
        if (mgrp_cb[i].ipaddr)
            igmp_snd_dat(IGMP_REPORT, mgrp_cb[i].ipaddr, mgrp_cb[i].ipaddr);
    }

    return pkt;

}

/************************************************/
/* Send IGMP Message                            */
/************************************************/

ER igmp_snd_dat(UB type, UW dstaddr, UW grpaddr)
{
    T_IGMP_MSG igmp;
    T_IP *pkt;
    UW ipaddr;
    ER ercd;

    ercd = get_blf((VP *)&pkt, ID_ETH_MPF);
    if (ercd != E_OK)
        return ercd;
    memset(&pkt->ctl, 0, sizeof (T_CTL_HEADER));
    pkt->ctl.mpfid = (H)ID_ETH_MPF;

    pkt->ip.da = htonl(dstaddr);
    ipaddr = byte4_to_long(default_ipaddr);
    pkt->ip.sa = htonl(ipaddr);

    pkt->ip.ver = 0x45;
    pkt->ip.tos = 0;
    pkt->ip.tl = htons(IP_HEADER_SZ + sizeof(igmp));
    pkt->ip.id = htons(ip_ident++);
    pkt->ip.fo = 0;
    pkt->ip.ttl = 1;
    pkt->ip.prot = PROT_IGMP;

    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    pkt->ctl.hedflg = FALSE;
    pkt->ctl.arprty = 0;

    igmp.type = (0x10 | type);
    igmp.dummy = 0;
    igmp.group = htonl(grpaddr);
    igmp.cs = igmp_checksum(&igmp);
    
    memcpy(pkt->data, &igmp, sizeof(igmp));

    return snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
}
