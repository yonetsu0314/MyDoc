/******************************************************************************
* NORTi Network Initialize & Common Sub-routines                              *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Mar/1999 Created.                                                        *
* 03/Oct/2000 Last Update.(Ver. 4.02 Release)                                 *
* 20/Nov/2000 Modified remove_queued_packet and remove_xxxque_udppkt.         *
* 01/Dec/2000 Last Update.(Ver. 4.03 Release)                                 *
* 10/Feb/2001 Modified tcp_portany, udp_portany (get port number)             *
* 31/Jan/2002 Added support for IP reassembling(AK)                           *
* 14/Apr/2003 Added ARP timer initialization code                             *
* 28/Apr/2003 Added initialization code for mgrb_cp and ipq_hash              *
******************************************************************************/

#include <string.h>
#include <stdlib.h>
#ifndef NULL
#include <stdio.h>
#endif
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"

#ifdef ITRON_H
#define KRNL_V4
#else
#define KRNL_V3
#endif

#ifdef KRNL_V4
#include "nosys4.h"
#else
#include "nosys3.h"
#define V3_T_MSG T_MSG
#endif

#include "nonet.h"
#include "nonets.h"
#include "nonitod.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

/* Internal Data */

#ifdef LITTLE_ENDIAN
static const UH order = 0x1234;
#endif
/* Resource ID */

ID ID_IP_SND_TSK;
ID ID_IP_RCV_TSK;
ID ID_TCPIP_SEM;
ID ID_IP_SND_MBX;
ID ID_IP_RETRY_MBX;
ID ID_ETH_MPF;

/************************************************/
/* TCP/IP Internal Error                        */
/************************************************/

ER tcpip_internal_error(ER ercd)
{
    return ercd;
}

#ifdef LITTLE_ENDIAN
/************************************************/
/* 32-bit Host/Network Byte Order Convertsion   */
/************************************************/

UW htonl(UW hl)
{
    UW nl;

    if (*(UB *)(&order) == 0x12)        /* Big Endian mode */
        return hl;                      /* return the same */

    nl = hl;
    nl <<= 8; hl >>= 8; nl |= (UB)hl;   /* 4 byte order change */
    nl <<= 8; hl >>= 8; nl |= (UB)hl;
    nl <<= 8; hl >>= 8; nl |= (UB)hl;
    return nl;
}

#endif
#ifdef LITTLE_ENDIAN
/************************************************/
/* 16-bit Host/Network Byte Order Convertsion   */
/************************************************/

UH htons(UH hs)
{
    UH ns;

    if (*(UB *)(&order) == 0x12)        /* Big Endian mode */
        return hs;                      /* return the same */

    ns = hs;
    ns <<= 8; hs >>= 8; ns |= (UB)hs;   /* 2 byte order change */
    return ns;
}

#endif
/************************************************/
/* Cretical Section Control                     */
/************************************************/

ER tcpip_cretical(BOOL on)
{
    static PRI oldpri;
    PRI pri;
    ER ercd;

    if (on)
    {   pri = pTCB[vget_tid()]->pri;
        if (pri != PRI_TCPIP)
            ercd = chg_pri(TSK_SELF, PRI_TCPIP);
        else
            ercd = E_OK;
        oldpri = pri;
    }
    else
    {   if (oldpri != PRI_TCPIP)
            ercd = chg_pri(TSK_SELF, oldpri);
        else
            ercd = E_OK;
    }
    return ercd;
}

/************************************************/
/* TCP REP Control Block Initialize             */
/************************************************/

static void ini_tcp_rep(void)
{
    int i;

    for (i=0; i<TCP_REPID_MAX; i++) {
        memset(&TCP_REP[i], 0, sizeof(T_TCP_REP));
        TCP_REP[i].repid = (UB)i+1;
    }
}

/************************************************/
/* TCP CEP Control Block Initialize             */
/************************************************/

static void ini_tcp_cep(void)
{
    int i;

    for (i=0; i<TCP_CEPID_MAX; i++) {
        memset(&TCP_CEP[i], 0, sizeof(T_TCP_CEP));
        TCP_CEP[i].cepid = (UB)i+1;
    }
}

/************************************************/
/* UDP CEP Control Block Initialize             */
/************************************************/

static void ini_udp_cep(void)
{
    int i;

    for (i=0; i<UDP_CEPID_MAX; i++) {
        memset(&UDP_CEP[i], 0, sizeof(T_UDP_CEP));
        UDP_CEP[i].cepid = (UB)i+1;
    }

}

/************************************************/
/* Multicast Group control block Initialize     */
/************************************************/

static void ini_mgrp_blk(void)
{
    int i;
    
    for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
        mgrp_cb[i].ipaddr = 0;
        mgrp_cb[i].portno = 0;
        mgrp_cb[i].cep = NULL;
    }
}

/************************************************/
/* Initialize TCP/IP stack variable             */
/************************************************/

static void ini_tcpip_var(void)
{
    int i;

    /* Control Blocks Initialization */

    ini_tcp_rep();
    ini_tcp_cep();
    ini_udp_cep();

    /* 11/Jul/02 - added AK */
#ifndef PPP
    ini_arp_table();    /* Clear ARP table cache */
#endif

    ip_ident = 0;
    port_any = TCP_PORT_EPHEMERAL;

    /* tcp_ini_iss can be initialized by application if required */
    if (tcp_iss == 0)
        tcp_iss = (tcp_iss + SYSCK.ltime) & 0x0fffffffL;
    ip_timer = 0;

    pICMP_CB = NULL;
    pIP_CB = NULL;
    pARP_CB = NULL;

    ini_mgrp_blk();

    /* Initialize IP reassembly hash queue */
    for (i =0;i < IPHASH_QSZ; i++)
        ipq_hash[i] = NULL;

}

/************************************************/
/* Assign Object ID Numbers                     */
/************************************************/

static void assign_obj_id(void)
{
    int id,i;

    /* Mailbox ID */

    if(ID_IP_RETRY_MBX != 0) {
        id = ID_IP_RETRY_MBX;
        for (i=0; i<UDP_CEPID_MAX; i++)
            UDP_CEP[i].id_udp_mbx = (UB)++id;
    }

    /* Memorypool ID */

    if(ID_ETH_MPF != 0) { /* ID auto */
        id = ID_ETH_MPF;
        for (i=0; i<UDP_CEPID_MAX; i++)
            UDP_CEP[i].id_udp_mpf = (UB)++id;
    }
}

/************************************************/
/* Func to set Initial tcp sequence value       */
/************************************************/

void tcp_set_iss(UW iss)
{
    tcp_iss = iss;
}

/************************************************/
/* TCP/IP Initialize                            */
/************************************************/

ER tcp_ini(void)
{
    ER ercd;

    /* Hardware Initialize */
#ifndef PPP
    ercd = lan_ini(ethernet_addr);
    if (ercd != E_OK)
        return ercd;
#endif
    /* Set each start of resource ID */

    ID_IP_SND_TSK = TCP_TSKID_TOP;
    if (TCP_TSKID_TOP == 0)
        ID_IP_RCV_TSK = 0;
    else
        ID_IP_RCV_TSK = (TCP_TSKID_TOP+1);

    ID_TCPIP_SEM = TCP_SEMID_TOP;
    ID_IP_SND_MBX = TCP_MBXID_TOP;
    if (TCP_MBXID_TOP == 0)
        ID_IP_RETRY_MBX = 0;
    else
        ID_IP_RETRY_MBX = (TCP_MBXID_TOP+1);
    ID_ETH_MPF = TCP_MPFID_TOP;

    /* Initialize TCP/IP stack variables & structures */

    ini_tcpip_var();

    /* Objest ID Numbers Assignment */

    assign_obj_id();

    /* Create TCP/IP Service Call Semaphore */

    if (TCP_SEMID_TOP == 0) { /* ID auto */
        ercd = vcre_sem(&c_tcpip_sem);
        if(ercd < 0)
            return ercd;
        ID_TCPIP_SEM = ercd;
    }
    else {
        ercd = cre_sem(ID_TCPIP_SEM, &c_tcpip_sem);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create IP Task */

    if (TCP_TSKID_TOP == 0) { /* ID auto */
        ercd = vcre_tsk(&c_ip_snd_tsk);
        if(ercd < 0)
            return ercd;
        ID_IP_SND_TSK = ercd;
        ercd = vcre_tsk(&c_ip_rcv_tsk);
        if(ercd < 0)
            return ercd;
         ID_IP_RCV_TSK = ercd;
    }
    else {
        ercd = cre_tsk(ID_IP_SND_TSK, &c_ip_snd_tsk);
        if (ercd != E_OK)
            return ercd;
        ercd = cre_tsk(ID_IP_RCV_TSK, &c_ip_rcv_tsk);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create Mailbox for IP Sending */

    if (TCP_MBXID_TOP == 0) { /* ID auto */
        ercd = vcre_mbx(&c_tcpip_mbx);
        if (ercd < 0)
            return ercd;
        ID_IP_SND_MBX = ercd;
        ercd = vcre_mbx(&c_tcpip_mbx);
        if (ercd < 0)
            return ercd;
        ID_IP_RETRY_MBX = ercd;
    }
    else {
        ercd = cre_mbx(ID_IP_SND_MBX, &c_tcpip_mbx);
        if (ercd != E_OK)
            return ercd;
        ercd = cre_mbx(ID_IP_RETRY_MBX, &c_tcpip_mbx);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create Memorypool for Ethernet Frame */

    if (TCP_MPFID_TOP == 0) { /* ID auto */
        ercd = vcre_mpf(&c_eth_mpf);
        if (ercd < 0)
            return ercd;
        ID_ETH_MPF = ercd;
    }
    else {
        ercd = cre_mpf(ID_ETH_MPF, &c_eth_mpf);
        if (ercd != E_OK)
            return ercd;
    }

    /* Init ARP timer */
    tcpip_timer.cyctim.utime = 0;
    tcpip_timer.cyctim.ltime =  wait(IP_ARP_TIMER);
    ercd = vdef_cyc(&tcpip_timer);
    if(ercd <= 0) 
        return ercd;

    /* Start IP Tasks */

    ercd = sta_tsk(ID_IP_SND_TSK, 0);
    if (ercd != E_OK)
        return ercd;

    ercd = sta_tsk(ID_IP_RCV_TSK, 0);
    if (ercd != E_OK)
        return ercd;

    /* TCP/IP Service Call Enable */

    return sig_sem(ID_TCPIP_SEM);
}

/************************************************/
/* Remove Queued Packet from Mailbox            */
/************************************************/

void remove_queued_packet(ID mbxid, VP cep)
{
    T_MBX NEAR *mbx;
    T_IP *pkt;
    T_IP *ppkt;
    T_IP *rpkt;
    T_IP *que;
#ifdef KRNL_V3
    T_MBX *mtop;
#else
    V4_T_MSGHDR *mtop;
#endif

    mbx = pMBX[mbxid];
    ppkt = que = NULL;
#ifdef KRNL_V3
    mtop = mbx;
#else
    mtop = (V4_T_MSGHDR *)((ADDR)mbx+mbx->moff);
#endif
    pkt = (T_IP *)mtop->top;
    while (pkt != NULL)
    {   if (pkt->ctl.cep == cep)
        {   rpkt = pkt;
            pkt = pkt->next;
            if (ppkt == NULL)
                mtop->top = (V3_T_MSG *)pkt;
            else
                ppkt->next = pkt;
            rpkt->next = que;
            que = rpkt;
        }
        else
        {   ppkt = pkt;
            pkt = pkt->next;
        }
    }
    mtop->end = (V3_T_MSG *)ppkt;

    for (pkt = que; pkt != NULL; )
    {   rpkt = pkt;
        pkt = pkt->next;
        rel_blf(rpkt->ctl.mpfid, rpkt);
    }

    if (mbxid == ID_IP_SND_MBX)
        remove_queued_packet(ID_IP_RETRY_MBX, cep);
}

/************************************************/
/* Remove Queued UDP Sending Packet             */
/************************************************/

BOOL remove_sndque_udppkt(ID mbxid, VP cep, ER code)
{
    T_MBX NEAR *mbx;
    T_IP *pkt;
    T_IP *ppkt;
    T_IP *rpkt;
    T_IP *que;
    ID tskid;
    TCP_CALLBACK func;
    ER ercd;
    BOOL flg=FALSE;
#ifdef KRNL_V3
    T_MBX *mtop;
#else
    V4_T_MSGHDR *mtop;
#endif

    mbx = pMBX[mbxid];
    ppkt = que = NULL;
#ifdef KRNL_V3
    mtop = mbx;
#else
    mtop = (V4_T_MSGHDR *)((ADDR)mbx+mbx->moff);
#endif
    pkt = (T_IP *)mtop->top;
    while (pkt != NULL) {
        if (pkt->ctl.cep == cep) {

            flg = TRUE;

            if (code) {

                /* Release the waiting task */

                tskid = pkt->ctl.tskid;
                if (tskid != FALSE){
                    ercd = rel_wai(tskid);
                    if (ercd != E_OK)
                        tcpip_internal_error(ercd);
                }

                /* Call callback function if any */

                func = pkt->ctl.callback;
                if (func != NULL){
                    pkt->ctl.parblk = code;
                    (func)(((T_UDP_CEP *)(pkt->ctl.cep))->cepid, pkt->ctl.fncd,
                             &pkt->ctl.parblk);
                }
            }

            rpkt = pkt;
            pkt = pkt->next;
            if (ppkt == NULL)
                mtop->top = (V3_T_MSG *)pkt;
            else
                ppkt->next = pkt;
            rpkt->next = que;
            que = rpkt;
        }
        else {
            ppkt = pkt;
            pkt = pkt->next;
        }
    }
    mtop->end = (V3_T_MSG *)ppkt;

    for (pkt = que; pkt != NULL; ) {
        rpkt = pkt;
        pkt = pkt->next;
        rel_blf(rpkt->ctl.mpfid, rpkt);
    }

    if (mbxid == ID_IP_SND_MBX) {
        if(remove_sndque_udppkt(ID_IP_RETRY_MBX, cep, code))
            return TRUE;
    }
    return flg;
}

/************************************************/
/* Remove UDP Queued Packet from Mailbox        */
/************************************************/

void remove_rcvque_udppkt(ID mbxid, VP buf)
{
    T_MBX NEAR *mbx;
    T_UDP_RCV *pkt;
    T_UDP_RCV *ppkt;
    T_UDP_RCV *rpkt;
    T_UDP_RCV *que;
#ifdef KRNL_V3
    T_MBX *mtop;
#else
    V4_T_MSGHDR *mtop;
#endif

    mbx = pMBX[mbxid];
    ppkt = que = NULL;
#ifdef KRNL_V3
    mtop = mbx;
#else
    mtop = (V4_T_MSGHDR *)((ADDR)mbx+mbx->moff);
#endif
    pkt = (T_UDP_RCV *)mtop->top;
    while (pkt != NULL)
    {   if (pkt->buf == buf)
        {   rpkt = pkt;
            pkt = pkt->next;
            if (ppkt == NULL)
                mtop->top = (V3_T_MSG *)pkt;
            else
                ppkt->next = pkt;
            rpkt->next = que;
            que = rpkt;
        }
        else
        {   ppkt = pkt;
            pkt = pkt->next;
        }
    }
    mtop->end = (V3_T_MSG *)ppkt;
}

/************************************************/
/* 4 bytes Array to Long Integer                */
/************************************************/

UW byte4_to_long(const UB *b)
{
    UW d;

    d  = b[0]; d <<= 8;
    d |= b[1]; d <<= 8;
    d |= b[2]; d <<= 8;
    d |= b[3];

    return d;
}

/************************************************/
/* Long Integer to 4 bytes Array                */
/************************************************/

void long_to_byte4(UB *b, UW d)
{
    b[3] = (UB)d; d >>= 8;
    b[2] = (UB)d; d >>= 8;
    b[1] = (UB)d; d >>= 8;
    b[0] = (UB)d;
}

/************************************************/
/* IP Address String to Long Integer            */
/************************************************/

UW ascii_to_ipaddr(B *s)
{
    INT i;
    UW ipaddr;

    ipaddr = 0L;
    for (i = 0;;) {
        ipaddr += (UB)atoi((char *)s);
        if (++i == 4)
            return ipaddr;
        s = (B *)strchr((char *)s, '.');
        if (s == NULL)
            return 0L;
        s++;
        ipaddr <<= 8;
    }
}

/************************************************/
/* Long Integer to IP Address String            */
/************************************************/

INT ipaddr_to_ascii(B *s, UW ipaddr)
{
    INT i, n;
    UB d[4];
    B *ss = s;

    ipaddr = htonl(ipaddr);
    for (i = 3;;) {
        d[i] = (UB)ipaddr;
        if (--i < 0)
            break;
        ipaddr >>= 8;
    }
    for (i = 0;;) {
        n = itoulen(d[i]);
        itou((char *)s, d[i], n);
        s += n;
        if (++i > 3)
            break;
        *s++ = '.';
    }
    *s = '\0';
    return s - ss;
}

/************************************************/
/* Allocate TCP Ephemeral Port Number           */
/************************************************/
/* 11/Jul/02 - Modified AK */
UH tcp_portany(void)
{
    UH i;
L1:
    if (++port_any >= 0xFFFF)
        port_any = TCP_PORT_EPHEMERAL + 1;
    
    for (i = 0; i < TCP_REPID_MAX; i++) {
        if (TCP_REP[i].portno == port_any)
            goto L1;
    }

    for (i = 0; i < TCP_CEPID_MAX; i++) {
        if (TCP_CEP[i].portno == port_any)
            goto L1;
    }

    for (i = 0; i < UDP_CEPID_MAX; i++) {
        if (UDP_CEP[i].portno == port_any)
            goto L1;
    }
    return port_any;
}
/* end */


