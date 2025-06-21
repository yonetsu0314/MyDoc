/******************************************************************************
* NORTi Network ARP Module                                                    *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 12/Feb/1999  Created                                                        *
* 03/Oct/2000  Ver. 4.02 Release                                              *
* 04/Apr/2002  lan_read_pkt_end parameter changed in arp_reception.           *
* 14/Apr/2003  Added arp_req_retry, ini_arp_tab, clr_arp_tab,                 *
*               arp_add_entry, arp_del_entry functions                        *
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

const UB unknown_addr[] = { 0, 0, 0, 0, 0, 0 };

/********************************************/
/* ARP Internal error                       */
/********************************************/

ER arp_internal_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

/************************************************/
/* ARP Request Retransmission                   */
/************************************************/
void arp_req_retry()
{
    T_IP *pkt;
    ER ercd;

    pkt = NULL;

    if ((ercd = prcv_msg((T_MSG **)&pkt, ID_IP_RETRY_MBX)) == E_OK) {
        ercd = snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
        if (ercd != E_OK) 
            arp_internal_error(ercd);
    }
}

/************************************************/
/* ARP TABLE Initialize                         */
/************************************************/

void ini_arp_table()
{
    T_ARP_TABLE *t;
    int i;

    for (i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        t->ipaddr = 0L;
        t->etime = 0L;
        memset(t->macaddr, 0, 6);
        t->atype = ARP_UNKNOWN;
    }
}

/************************************************/
/* Validate ARP TABLE timeout entries           */
/************************************************/

void val_arp_table(void)
{
    T_ARP_TABLE *t;
    UW ctime, rtime;
    int i;

    ctime = ip_timer;

    /* Check elapsed time for ARP entry */
    for(i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        rtime = ctime - t->etime;
        if (t->atype != ARP_STATIC &&t->ipaddr != 0L && rtime >= ARP_CACHE_TOUT) {
            /* ARP entry timedout */
            t->ipaddr = 0L;
            t->etime = 0L;
            t->atype = ARP_UNKNOWN;
            memset(t->macaddr, 0, 6);
        }
    }
}

/********************************************/
/* Refer ARP table entries                  */
/********************************************/

ER ref_arp_table(UB *macaddr, UW ipaddr)
{
    T_ARP_TABLE *t;
    UW mask, gateway, myip;
    int i;

    mask    = byte4_to_long(subnet_mask);
    myip    = byte4_to_long(default_ipaddr);
    gateway = byte4_to_long(default_gateway);
    if ((ipaddr & mask) != (myip & mask)) {
        if (gateway != 0)
            ipaddr = gateway;
        else
            return -1;
    }

    for (i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        if (t->ipaddr == ipaddr) {
            if (memcmp(t->macaddr, unknown_addr, 6) != 0) {
               if (macaddr != NULL)
                   memcpy(macaddr, t->macaddr, 6);
                t->etime = ip_timer;
                return E_OK;
            }
            /* Avoid ARP Flooding */
            if ((UW)(t->etime + ARP_CACHE_TOUT + 1) <= ip_timer)
                return E_TMOUT;
            return -1;
        }
    }
    return -1;
}

/********************************************/
/* Set the mac, ip address in ARP table     */
/********************************************/

void set_arp_table(UW ipaddr, const UB *macaddr)
{
    T_ARP_TABLE *t;
    static int n;
    int i;
    UW rtime;

    for (i = 0; i < ARP_TABLE_CNT; i++) {
        if (arp_table[i].ipaddr == ipaddr) {
            t = &arp_table[i];
            /* Should not update STATIC entries */
            if (t->atype == ARP_STATIC) 
                return;
            goto L2;
        }
    }

    if (macaddr == NULL)
        return;

    for (i = 0; i < ARP_TABLE_CNT; i++) {
        if (arp_table[i].ipaddr == 0L) {
            t = &arp_table[i];
            goto L1;
        }
    }

    /* Find and use old ARP entry */
    n = 0;
    rtime = (UW)-1;
    for (i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        if (t->atype != ARP_STATIC && (ip_timer - t->etime) > rtime) {
            rtime = ip_timer - t->etime;
            n = i;
        }
    }
    t = arp_table + n;

    SNMP_UPDNTM(SNMP_NTMT_DELETE, 1, t->ipaddr, t->macaddr);
L1:
    SNMP_UPDNTM(SNMP_NTMT_STATIC, 1, ipaddr, macaddr);
    t->atype = ARP_DYNAMIC;
    t->ipaddr = ipaddr;
L2:
    if (memcmp(t->macaddr, unknown_addr, 6) == 0) {
        t->etime = ip_timer;
        t->etime = t->etime - ARP_CACHE_TOUT - 1;
        SNMP_UPDNTM(SNMP_NTMT_DELETE, 1, t->ipaddr, t->macaddr);
    }

    if (macaddr == NULL) {
        t->ipaddr = 0L;
        t->etime  = 0L;
        t->atype = ARP_UNKNOWN;
        SNMP_UPDNTM(SNMP_NTMT_DELETE, 1, t->ipaddr, t->macaddr);
    }
    else if (macaddr != NULL && memcmp(t->macaddr, macaddr, 6)) {
        memcpy(t->macaddr, macaddr, 6);
         /* Set time for new entry */
        t->etime = ip_timer;
        SNMP_UPDNTM(SNMP_NTMT_STATIC, 1, t->ipaddr, t->macaddr);
    }
}

/********************************************/
/* Call back function called at the time of */
/* ICMP packet reception                    */
/********************************************/

static BOOL chk_callback(T_ARP *pkt)
{
    ARP_CALLBACK func;
    T_ARP_CB *b;
    B flg;

    flg = FALSE;
    b = pARP_CB;
    while (b != NULL) {
        func = (ARP_CALLBACK)b->callback;
        if (func != NULL) {
            if (!(*func)(pkt)) {
                flg = TRUE;
                break;
            }
        }
        b = b->next;
    }
    return flg;
}

/********************************************/
/* Process for ARP request reception        */
/********************************************/

static VP arp_req_reception(T_ARP *pkt)
{
    UW ipaddr;
    ER ercd;

    ipaddr = byte4_to_long(pkt->tpa);

    if (!is_my_ipaddr(ipaddr)) {
        if (!chk_callback(pkt))
             return pkt;
        else
             return NULL;
    }

    pkt->ope = htons(ARP_RES);
    memcpy(pkt->tha, pkt->sha, 6);          /* Target Hardware Address */
    memcpy(pkt->tpa, pkt->spa, 4);          /* Target Protocol Address */
    memcpy(pkt->sha, ethernet_addr, 6);     /* Source Hardware Address */
    long_to_byte4(pkt->spa, ipaddr);        /* Source Protocol Address */

    ipaddr = byte4_to_long(pkt->tpa);
    set_arp_table(ipaddr, pkt->tha);

    memcpy(pkt->eth.da, pkt->eth.sa, 6);
    pkt->eth.etype = htons(ETYPE_ARP);

    SNMP_BYPCNT(snmp_mibbyp_if[0].ifInNUcastPkts, 1);   /* ifInNUcastPkts */

    ercd = snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
    if (ercd != E_OK) {
        arp_internal_error(ercd);
        return pkt;
    }
    return NULL;
}

/********************************************/
/* Process for ARP response reception       */
/********************************************/

static VP arp_res_reception(T_ARP *pkt)
{
    T_IP *ip_pkt;
    UW ipaddr;

    ipaddr = byte4_to_long(pkt->spa);
    set_arp_table(ipaddr, pkt->sha);

    if (prcv_msg((T_MSG **)&ip_pkt, ID_IP_RETRY_MBX) == E_OK)
        snd_msg(ID_IP_SND_MBX, ip_pkt);

    return pkt;
}

/********************************************/
/* Process for received ARP packet          */
/********************************************/

VP arp_reception(T_ARP *pkt)
{
    if (!lan_read_pkt_end(&pkt->htype, 28, 28))
        return pkt;

    if ((ntohs(pkt->htype) != 1)
     || (ntohs(pkt->ptype) != ETYPE_IP)
     || (pkt->hlen != 6)
     || (pkt->plen != 4))
        return pkt;

    switch (ntohs(pkt->ope)) {
    case ARP_REQ: return arp_req_reception(pkt);
    case ARP_RES: return arp_res_reception(pkt);
    default:      return pkt;
    }
}

/********************************************/
/* Construct a ARP request                  */
/********************************************/

BOOL arp_request(UW ipaddr, VP cep)
{
    T_ARP *pkt;
    UW mask, gateway, myip;
    ER ercd;

    mask    = byte4_to_long(subnet_mask);
    myip    = byte4_to_long(default_ipaddr);
    gateway = byte4_to_long(default_gateway);
    if ((ipaddr & mask) != (myip & mask)) {
        if(gateway != 0)
            ipaddr = gateway;
        else {
            arp_internal_error(-1);
            return FALSE;
        }
    }

    set_arp_table(ipaddr, unknown_addr);

    ercd = pget_blf((VP *)&pkt, ID_ETH_MPF);
    if (ercd != E_OK) {
        if(ercd != E_TMOUT)
            arp_internal_error(ercd);
        return FALSE;
    }
    memset(pkt, 0, sizeof (T_ARP));
    pkt->ctl.mpfid = (H)ID_ETH_MPF;
    pkt->ctl.cep = cep;

    pkt->eth.len = ARP_PACKET_SZ;
    memset(pkt->eth.da, -1, 6);
    memcpy(pkt->eth.sa, ethernet_addr, 6);
    pkt->eth.etype = htons(ETYPE_ARP);

    pkt->htype = htons(1);
    pkt->ptype = htons(ETYPE_IP);
    pkt->hlen = 6;
    pkt->plen = 4;
    pkt->ope = htons(ARP_REQ);
    memcpy(pkt->sha, ethernet_addr, 6);
    memcpy(pkt->spa, default_ipaddr, 4);
    long_to_byte4(pkt->tpa, 0L);
    long_to_byte4(pkt->tpa, ipaddr);

    SNMP_BYPCNT(snmp_mibbyp_if[0].ifOutNUcastPkts, 1);   /* ifOutNUcastPkts */

    ercd = snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
    if (ercd != E_OK) {
        arp_internal_error(ercd);
        ercd = rel_blf(pkt->ctl.mpfid, pkt);
        if (ercd != E_OK) {
            arp_internal_error(ercd);
            dly_tsk(wait(1000));
        }
        return FALSE;
    }
    return TRUE;
}

/********************************************/
/* Callback function definition             */
/********************************************/

ER arp_def_cbk(T_ARP_CB *b, ARP_CALLBACK callback)
{

    T_ARP_CB *bb, *bbb;
    ER ercd;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    bbb = (T_ARP_CB *)&pARP_CB;
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

/********************************************/
/* ARP API                                  */
/*  Adds ipaddr and macaddr in ARP table    */
/********************************************/

ER arp_add_entry(UW ipaddr, UB *macaddr, UW type)
{
    T_ARP_TABLE *t;
    UW rtime;
    int i, n;

    /* Check for host IP addr */
    if (ipaddr == byte4_to_long(default_ipaddr))
        return E_OBJ;

    /* Check for NULL macaddr */
    if (macaddr == NULL)
        return E_PAR;

    /* Check for invalid type */
    if (!(type == ARP_STATIC || type == ARP_DYNAMIC))
        return E_OBJ;

    /* Dont Fill all ARP entries as STATIC */
    if (type == ARP_STATIC) {
        for (n=2,i=0; i < ARP_TABLE_CNT; i++) {
            t = &arp_table[i];
            if (t->atype == ARP_STATIC) n++;
            if (n > ARP_TABLE_CNT) return E_NOMEM;
        }
    }
    else {
        if (type != ARP_DYNAMIC) 
            return E_OBJ;
    }

    /* Search for a free entry and add in it */
    for (i=0; i< ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        /* ipaddr already exists, overwrite */
        if (t->ipaddr == ipaddr)
            goto END;
        /* Check for free entry */
        if (t->ipaddr == 0) {
            t->ipaddr = ipaddr;
            goto END;
        }
    }

    /* No free space, find and use old ARP entry */
    n = 0;
    rtime = (UW)-1;
    for (i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        if (t->atype != ARP_STATIC && (ip_timer - t->etime) > rtime) {
            rtime = ip_timer - t->etime;
            n = i;
        }
    }

    if (n != 0)
        t = arp_table + n;
    else
        return E_NOMEM;     /* ARP Table full, cant enter more */

END:
    /* Copy the new entry values */
    memcpy(t->macaddr, macaddr, 6);
    t->atype = type;
    t->etime = ip_timer;
    return E_OK;
}

/********************************************/
/* ARP API                                  */
/*  Removes ipaddr and macaddr in ARP table */
/********************************************/

ER arp_del_entry(UW ipaddr)
{
    T_ARP_TABLE *t;
    int i;

    /* Search and remove the entry from ARP table */
    for (i = 0; i < ARP_TABLE_CNT; i++) {
        t = &arp_table[i];
        /* Check for ip address */
        if (t->ipaddr == ipaddr) {
            t->ipaddr = 0L;
            t->etime = 0L;
            t->atype = ARP_UNKNOWN;
            memset(t->macaddr, 0, 6);
            return E_OK;
        }
    }

    /* Entry not found */
    return E_OBJ;
}

/* End */
