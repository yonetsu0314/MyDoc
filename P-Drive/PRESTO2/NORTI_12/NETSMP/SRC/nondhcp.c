/******************************************************************************
* NORTi Network DHCP client sample                                            *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 24/Jan/2000                                                                 *
* 06/Oct/2002 Removed len variable from dhcp_snd_dat. Now all requests        *
*             transmitted with zero padding bytes.                         YS *
* 15/Apr/2003 Added dhcp_reb_data.                                         YS *
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
#include "nondhcp.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

/* DHCP Option */

const UB opt1[] = { 99, 130, 83, 99, DHCP_OP_MSGTYP, 1, DHCPDISCOVER, 255 };

const UB opt2[] =
{
  99, 130, 83, 99,
  DHCP_OP_MSGTYP, 1, DHCPREQUEST,
  DHCP_OP_PRMLST, 6, DHCP_OP_SUBNET, DHCP_OP_ROUTER, DHCP_OP_DNS,
  DHCP_OP_ADDRTM, DHCP_OP_RENETM, DHCP_OP_REBITM
};

const UB opt3[] =
{
  99, 130, 83, 99,
  DHCP_OP_MSGTYP, 1, DHCPREQUEST,
  DHCP_OP_PRMLST, 3, DHCP_OP_ADDRTM, DHCP_OP_RENETM, DHCP_OP_REBITM, 255
};

/* Resource */
ER dhcp_callback(ID cepid, FN fncd, VP parblk);
const T_UDP_CCEP dhcp_cep = {0, {0, UDP_PORT_BOOTPC}, (FP)dhcp_callback};
TASK         dhcp_req_tsk(void);
const T_CTSK dhcp_res_ctsk = {NULL, TA_HLNG, (FP)dhcp_req_tsk, DHCP_REQTSK_PRI, DHCP_REQTSK_SSZ};
TMRHDR ex_ip_lease(void);
static T_DALM dalm = {NULL, TA_HLNG, ex_ip_lease, TTM_REL, {0, 10000/MSEC}};

T_DHCP_CB    dhcp_cb;
UB           local_ipaddr[4];
UB           dhcp_ipaddr[4];
UW           lease_time;
UW           xid = 1711959080;
extern UB           dns_ipaddr[4];

#define REQ_SELECT 1
#define REQ_EXTENS 2

#define DHCP_TMO 3000/MSEC

ER dhcp_callback(ID cepid, FN fncd, VP parblk)
{
    wup_tsk(dhcp_cb.tskid);
    return E_OK;
}


/************************************************/
/* Search DHCP Vender Option                    */
/************************************************/

static UB* search_option(UB tag, UB* opt)
{
    INT i;
    UB n;

    if (opt == NULL)
        return NULL;

    /* Skip Magic cookie */

    opt += 4;

   /* Serach Option */

    for (i = 0; i < 312; i++) {
        if (*opt == 255)
            return NULL;
        if (*opt++ == tag){
            return (++opt);
        }
        n = *opt++;
        opt += n;
    }
    return NULL;
}

/************************************************/
/* Send DHCP Data                               */
/************************************************/
static T_IPV4EP dstaddr;

static ER dhcp_snd_dat(ID cepid, UB code, UB flg, T_DHCP* snd_pkt, T_DHCP* rcv_pkt)
{
    ER ercd;
    UB *p;

    /* Send DHCP */

    memset(snd_pkt, 0, sizeof(T_DHCP));
    snd_pkt->op = BOOT_REQUEST;
    snd_pkt->htype = 1; /* (10MbEthernet) */
    snd_pkt->hlen = 6;
    snd_pkt->xid = htonl(++xid);
    snd_pkt->flag = htons(0x8000);
    memcpy(snd_pkt->chaddr, ethernet_addr, 6);
    dstaddr.portno = UDP_PORT_BOOTPS;

    switch(code) {
    case DHCPDISCOVER:
        dstaddr.ipaddr = 0xFFFFFFFF; /* Broad Cast */
        memcpy(snd_pkt->option, opt1, sizeof(opt1));
        break;
    case DHCPREQUEST:
        /* DHCPREQUEST Selecing state */
        if (flg == REQ_SELECT){
            dstaddr.ipaddr = 0xFFFFFFFF; /* Broad Cast */
            memcpy(snd_pkt->option, opt2, sizeof(opt2));
            p = (UB *)(snd_pkt->option + sizeof(opt2));
            *p++ = DHCP_OP_SERVID;
            *p++ = 4;
            memcpy(p, dhcp_ipaddr, 4);
            p += 4;
            *p++ = DHCP_OP_ADRREQ;
            *p++ = 4;
            memcpy(p, local_ipaddr, 4);
            p += 4;
            *p = 255;
        }
        /* DHCPREQUEST Lease Extension state */
        else {
            snd_pkt->flag = 0;
            snd_pkt->ciaddr = htonl(byte4_to_long(local_ipaddr));
            dstaddr.ipaddr = byte4_to_long(dhcp_ipaddr);
            memcpy(snd_pkt->option, opt3, sizeof(opt3));
        }
        break;
    }

    ercd = udp_rcv_dat(cepid, &dstaddr, (VP)rcv_pkt, sizeof(T_DHCP), TMO_NBLK);
    if (ercd != E_WBLK)
        return ercd;

    ercd = udp_snd_dat(cepid, &dstaddr, snd_pkt, sizeof(T_DHCP), DHCP_TMO);
    if (ercd <= 0)
        return ercd;

    return E_OK;
}

/************************************************/
/* Receive DHCP Data                            */
/************************************************/

static ER dhcp_rcv_dat(ID cepid, UB code, UB flg, T_DHCP *pkt)
{
    ER ercd;
    UB *p;

    /* Wait for Receive */

    ercd = tslp_tsk(DHCP_TMO);
    if (ercd != E_OK)
        goto ERR;

    /* Receive DHCP Response */

    for (;;){
        p = search_option(DHCP_OP_MSGTYP, pkt->option);
        if (p != NULL){
            if (ntohl(pkt->xid) == xid && pkt->op == BOOT_REPLY && *p == code)
                break;
        }
        ercd = udp_rcv_dat(cepid, &dstaddr, (VP)pkt, sizeof(T_DHCP), DHCP_TMO);
        if (ercd <= 0)
            goto ERR;
    }

    switch(code) {
    case DHCPOFFER:
        /* Set DHCP Server Address */
        p = search_option(DHCP_OP_SERVID, pkt->option);
        if (p != NULL)
            memcpy(dhcp_ipaddr, p, 4);
        long_to_byte4(local_ipaddr, ntohl(pkt->yiaddr));
        break;
    case DHCPACK:
        /* Set Local IP Address */
        long_to_byte4(default_ipaddr, ntohl(pkt->yiaddr));

        if (flg == REQ_SELECT){
            /* Set Subnetmask */
            p = search_option(DHCP_OP_SUBNET, pkt->option);
            if (p != NULL)
                memcpy(subnet_mask, p, 4);
            /* Set Router Address */
            p = search_option(DHCP_OP_ROUTER, pkt->option);
            if (p != NULL)
                memcpy(default_gateway, p, 4);
            /* Set DNS Address */
            p = search_option(DHCP_OP_DNS, pkt->option);
            if (p != NULL)
                memcpy(dns_ipaddr, p, 4);
        }
        /* Get Time */
        p = search_option(DHCP_OP_RENETM, pkt->option);
        if (p == NULL) {
            p = search_option(DHCP_OP_REBITM, pkt->option);
            if (p == NULL) {
                p = search_option(DHCP_OP_ADDRTM, pkt->option);
                if (p == NULL){
                    ercd = E_OBJ;
                    goto ERR;
                }
            }
        }
        lease_time = byte4_to_long(p);
        break;
    }
ERR:
    if (ercd <= 0)
        return ercd;
    return E_OK;
}

/************************************************/
/* Get DHCP Data                                */
/************************************************/

ER dhcp_get_data(ID cepid)
{
    ER ercd;
    T_DHCP *rcv_pkt, *snd_pkt;

    /* Create CEP for DHCP */

    if (!cepid){
        cepid = udp_vcre_cep(&dhcp_cep);
        if (cepid <= 0)
            return cepid;
    }

    else {
        ercd = udp_cre_cep(cepid, &dhcp_cep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Set Broadcast Option */

    ercd = udp_set_opt(cepid, IP_BROADCAST, (VP)TRUE, 1);
    if (ercd != E_OK)
        goto END2;

    dhcp_cb.tskid = vget_tid();

    /* Get memory buffer for receiving DHCP pakcet */

    ercd = get_blf((VP *)&rcv_pkt, ID_ETH_MPF);
    if (ercd != E_OK)
        goto END1;

    /* Get memory buffer for sending DHCP pakcet */

    ercd = get_blf((VP *)&snd_pkt, ID_ETH_MPF);
    if (ercd != E_OK)
        goto END1;

    dly_tsk(100/MSEC);

    /* Send DHCPDISCOVER */

    ercd = dhcp_snd_dat(cepid, DHCPDISCOVER, 0, snd_pkt, rcv_pkt);
    if (ercd != E_OK)
        goto END1;

    /* Receive DHCPOFFER */

    dly_tsk(100/MSEC);

    ercd = dhcp_rcv_dat(cepid, DHCPOFFER, 0, rcv_pkt);
    if (ercd != E_OK)
        goto END1;

    /* Send DHCPREQUEST */

    ercd = dhcp_snd_dat(cepid, DHCPREQUEST, REQ_SELECT, snd_pkt, rcv_pkt);
    if (ercd != E_OK)
        goto END1;

    /* Receive DHCPACK */

    dly_tsk(100/MSEC);

    ercd = dhcp_rcv_dat(cepid, DHCPACK, REQ_SELECT, rcv_pkt);


END1:
    udp_set_opt(cepid, IP_BROADCAST, (VP)FALSE, 1);
    udp_del_cep(cepid);
    rel_blf(ID_ETH_MPF, rcv_pkt);
    rel_blf(ID_ETH_MPF, snd_pkt);
    return ercd;
END2:
    /* Set Option Broadcast Off */

    udp_set_opt(cepid, IP_BROADCAST, (VP)FALSE, 1);
    udp_del_cep(cepid);

    return ercd;
}

/************************************************/
/* Rebind DHCP Data                             */
/************************************************/

ER dhcp_reb_data(ID cepid)
{
    ER ercd;
    T_DHCP *rcv_pkt, *snd_pkt;

    if (!cepid){
        cepid = udp_vcre_cep(&dhcp_cep);
        if (cepid <= 0)
            return cepid;
    }

    else {
        ercd = udp_cre_cep(cepid, &dhcp_cep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Set Broadcast Option */

    ercd = udp_set_opt(cepid, IP_BROADCAST, (VP)TRUE, 1);
    if (ercd != E_OK) {
        udp_del_cep(cepid);
        return ercd;
    }

    /* Get memory buffer for receiving DHCP pakcet */

    ercd = get_blf((VP *)&rcv_pkt, ID_ETH_MPF);
    if (ercd != E_OK)
        goto ERR;

    /* Get memory buffer for sending DHCP pakcet */

    ercd = get_blf((VP *)&snd_pkt, ID_ETH_MPF);
    if (ercd != E_OK)
        goto ERR;

    dhcp_cb.tskid = vget_tid();

    /* Send DHCPREQUEST */

    ercd = dhcp_snd_dat(cepid, DHCPREQUEST, REQ_EXTENS, snd_pkt, rcv_pkt);
    if (ercd != E_OK)
        goto ERR;

    /* Receive DHCPACK */

    ercd = dhcp_rcv_dat(cepid, DHCPACK, REQ_EXTENS, rcv_pkt);
    if (ercd != E_OK)
        goto ERR;

ERR:
    rel_blf(ID_ETH_MPF, snd_pkt);
    rel_blf(ID_ETH_MPF, rcv_pkt);
    udp_set_opt(cepid, IP_BROADCAST, (VP)FALSE, 1);
    udp_del_cep(cepid);

    return ercd;
}

/************************************************/
/* Start DHCP Task                              */
/************************************************/

ER dhcp_sta_tsk(ID tskid, ID cepid, ID almid, DHCP_CALLBACK callback)
{
    ER ercd;

    /* Create Task */

    if (tskid != 0){
        ercd = cre_tsk(tskid, &dhcp_res_ctsk);
        if (ercd != E_OK)
            return ercd;
    }
    else{
        tskid = vcre_tsk(&dhcp_res_ctsk);
        if (tskid <= 0)
            return tskid;
    }

    dhcp_cb.cepid = cepid;
    dhcp_cb.tskid = tskid;
    dhcp_cb.callback = callback;

    /* Start Task */

    sta_tsk(tskid, 0);

    /* Set Alarm */

  #ifdef DEBUG
    lease_time = 10;
  #endif

    if (!(lease_time&0xFFF00000))
        dalm.almtim.ltime = (lease_time*1000/MSEC);
    else
        dalm.almtim.ltime = 0xFFFFFFFF;

    if (almid != 0){
        ercd = def_alm(almid, &dalm);
        if (ercd != E_OK)
            return ercd;
        dhcp_cb.almid = almid;
    }
    else {
        ercd = vdef_alm(&dalm);
        if (ercd <= 0)
            return ercd;
        dhcp_cb.almid = ercd;
    }


    return E_OK;

}

/************************************************/
/* DHCP Alarm Handler                           */
/************************************************/

TMRHDR ex_ip_lease(void)
{
    wup_tsk(dhcp_cb.tskid);
    ret_tmr();
}

/************************************************/
/* DHCPREQUEST Send Task                        */
/************************************************/

TASK dhcp_req_tsk(void)
{
    ER ercd;
    DHCP_CALLBACK func;

    func = dhcp_cb.callback;

    for(;;) {
        vcan_wup();
        ercd = tslp_tsk(TMO_FEVR);
        if (ercd == E_RLWAI)
            break;

        /* Rebind DHCP Data */

        ercd = dhcp_reb_data(dhcp_cb.cepid);
        if (ercd != E_OK)
            goto ERR;

        /* Restart Alarm */
      #ifdef DEBUG
        lease_time = 10;
      #endif
        if (!(lease_time&0xFFF00000))
            dalm.almtim.ltime = (lease_time*1000/MSEC);
        else
            dalm.almtim.ltime = 0xFFFFFFFF;

        ercd = def_alm(dhcp_cb.almid, &dalm);
        if (ercd != E_OK)
            goto ERR;

ERR:
        if (func != NULL)
            (func)(ercd);
    }
    exd_tsk();
}

/* END */
