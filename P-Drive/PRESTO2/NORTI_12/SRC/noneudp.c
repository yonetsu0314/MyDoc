/******************************************************************************
* NORTi Network UDP Module                                                    *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Mar/1999 Created.                                                        *
* 12/Oct/2000 (Ver. 4.02 Release)                                             *
* 21/Oct/2000 Modified udp_rcv_dat_now (LittleEndian)                         *
* 21/Oct/2000 Added udp_snd_dat,udp_rcv_dat parameter check (E_PAR)           *
* 01/Dec/2000 (Ver. 4.03 Release)                                             *
* 10/Feb/2001 Change tcp_con_cep (get port number)                            *
* 05/Sep/2001 Deleted (cep->pkt = pkt) in make_udp_pkt and udp_snd_dat        *
* 25/Sep/2001 Change igmp_snd_dat parameter IGMP_UNUSED ¨ IGMP_UNUSED        *
* 27/Feb/2002 udp_rcv_dat_que, SIZEOF_INT != 4,initialize variable m      HM  *
* 26/Mar/2002 Modified udp_reception_sub1 pkt->data = (UB *)bp                *
* 1/Aug/2002 - A.K - Fixed IP reassembly memory releasing problems            *
* 28/Aug/2002 Fixed Memory pool for IP reassembly                             *
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
/************************************************/
/* UDP Internal Error                           */
/************************************************/

ER udp_internal_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

/************************************************/
/* UDP Dummy Callback                           */
/************************************************/

ER udp_dummy_cbk(ID cepid, FN fncd, VP parblk)
{
    T_IPV4EP dstaddr;

    if (fncd == TEV_UDP_RCV_DAT)
        return udp_rcv_dat(cepid, &dstaddr, NULL, 0, TMO_FEVR);
    else
        return E_OK;
}

/************************************************/
/* Copy the fragment packet data to user buffer */
/* 28-Aug-2002 AK - Created                     */
/************************************************/

ER udp_cpy_dat(VP data, T_IP *pkt, int len)
{
    struct iphash_que *hpkt;
    T_IP *fpkt;
    int flen, offset, i;

    if (!pkt->ctl.rcheck)
        return -1;

    hpkt = pkt->ctl.cep;
    fpkt = hpkt->fragments;             /* Get first fragment */
    offset = 0;
    i = 8;

    while (len) {
        if(!fpkt) 
            break;
        flen = ((T_IPF *)fpkt)->len - i;
        memcpy((UB *)data + offset, (UB *)fpkt->data + i, flen);
        i = 0;
        offset += flen;
        len -= flen;
        if (len <= 0)
            break;
        fpkt = ((T_IPF *)fpkt)->nextf;  /* Next fragment */
    }
    return 1;
}

/************************************************/
/* Create UDP Communication End Point           */
/************************************************/

ER udp_cre_cep(ID cepid, const T_UDP_CCEP *pk_ccep)
{
    T_UDP_CEP *cep;
    ER ercd;
    UW a;
    int i;
    UH n;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if CEP Already Created */

    cep = &UDP_CEP[cepid];
    if (cep->portno != 0) {
        ercd = E_OBJ;
        goto ERR;
    }

    /* Error if Port Number Already Used */

    a = pk_ccep->myaddr.ipaddr;
    if (a == IPV4_ADDRANY && cep->options != UDP_BROADCAST)
        a = byte4_to_long(default_ipaddr);
    n = pk_ccep->myaddr.portno;
    if (n == UDP_PORTANY)
       n = udp_portany();
    else {
        for (i = 0; i < UDP_CEPID_MAX; i++) {
           if (UDP_CEP[i].portno == n && UDP_CEP[i].ipaddr == a) {
               ercd = E_OBJ;
               goto ERR;
            }
        }
    }

    /* Mailbox and Memorypool Creation */

    cep = &UDP_CEP[cepid];
    if (cep->id_udp_mbx == 0) {
        ercd = vcre_mbx(&c_tcpip_mbx);
        if(ercd < 0)
            goto ERR;
        cep->id_udp_mbx = ercd;
    }
    else {
        ercd = cre_mbx(cep->id_udp_mbx, &c_tcpip_mbx);
        if (ercd != E_OK)
            goto ERR;
    }
    if (cep->id_udp_mpf == 0) {
        ercd = vcre_mpf(&c_udp_mpf);
        if (ercd < 0)
            goto ERR;
        cep->id_udp_mpf = ercd;
    }
    else {
        ercd = cre_mpf(cep->id_udp_mpf, &c_udp_mpf);
        if (ercd != E_OK) {
            del_mbx(cep->id_udp_mbx);
            goto ERR;
        }
    }

    /* Setup UDP CEP Control Block */

    cep->ipaddr = a;
    cep->portno = n;
    if (pk_ccep->callback == (FP)NADR || pk_ccep->callback == (FP)NULL)
        cep->callback = NULL;
    else
        cep->callback = (TCP_CALLBACK)pk_ccep->callback;
    cep->pkt = NULL;

    /* SNMP udpConnState */

    SNMP_UPDUDP(SNMP_CSTS_ESTABLISHED, cep->ipaddr, cep->portno);

    /* Normal Completion */

    return tcpip_cretical(FALSE);

    /* Abnormal Completion */
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}

/*********************************************************/
/* Create UDP Communication End Point (Auto ID Assigned) */
/*********************************************************/

ER udp_vcre_cep(const T_UDP_CCEP *pk_ccep)
{
    T_UDP_CEP *cep;
    ER ercd;
    int i;
    UW a;
    ID cepid;
    UH n;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Serch unused CEP */
    for (cepid=UDP_CEPID_MAX;;) {
        if (cepid == 0) {
            ercd = E_ID;
            goto ERR;
        }
        cep = &UDP_CEP[--cepid];
        if (!cep->portno) {
            ercd = cepid;
            break;
        }
    }

    /* Error if Port Number Already Used */

    a = pk_ccep->myaddr.ipaddr;
    if (a == IPV4_ADDRANY)
        a = byte4_to_long(default_ipaddr);
    n = pk_ccep->myaddr.portno;
    if (n == UDP_PORTANY)
        n = udp_portany();
    else {
        for (i = 0; i < UDP_CEPID_MAX; i++) {
            if (UDP_CEP[i].portno == n && UDP_CEP[i].ipaddr == a) {
                ercd = E_OBJ;
                goto ERR;
            }
        }
    }

    /* Mailbox and Memorypool Creation */

    cep = &UDP_CEP[cepid];
    if (cep->id_udp_mbx == 0) {
        ercd = vcre_mbx(&c_tcpip_mbx);
        if (ercd < 0)
            goto ERR;
        cep->id_udp_mbx = ercd;
    }
    else {
        ercd = cre_mbx(cep->id_udp_mbx, &c_tcpip_mbx);
        if (ercd != E_OK)
            goto ERR;
    }
    if (cep->id_udp_mpf == 0) {
        ercd = vcre_mpf(&c_udp_mpf);
        if (ercd < 0)
            goto ERR;
        cep->id_udp_mpf = ercd;
    }
    else {
        ercd = cre_mpf(cep->id_udp_mpf, &c_udp_mpf);
        if (ercd != E_OK) {
            del_mbx(cep->id_udp_mbx);
            goto ERR;
        }
    }

    /* Setup UDP CEP Control Block */

    cep->ipaddr = a;
    cep->portno = n;
    if (pk_ccep->callback == (FP)NADR || pk_ccep->callback == (FP)NULL)
        cep->callback = NULL;
    else
        cep->callback = (TCP_CALLBACK)pk_ccep->callback;
    cep->pkt = NULL;

    /* Normal Completion */

    ercd = tcpip_cretical(FALSE);
    if(ercd != E_OK)
        return ercd;
    else {
        /* SNMP udpConnState */
        SNMP_UPDUDP(SNMP_CSTS_ESTABLISHED, cep->ipaddr, cep->portno);
        return ++cepid;
    }

    /* Abnormal Completion */
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}


/************************************************/
/* Delete UDP Communication End Point           */
/************************************************/

ER udp_del_cep(ID cepid)
{
    T_UDP_CEP *cep;
    T_UDP_RCV *data;
    ER ercd;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if CEP Not Created */

    cep = &UDP_CEP[cepid];
    if (cep->portno == 0) {
        ercd = E_NOEXS;
        goto ERR;
    }

    /* Queued Sending Packet Removal */

    remove_sndque_udppkt(ID_IP_SND_MBX, cep, E_DLT);

    /*  Release receive waiting Task */

    do {
        ercd = prcv_msg((T_MSG **)&data, cep->id_udp_mbx);
        if (ercd == E_OK) {
            if (data->tskid != FALSE) {
                *(data->p_ercd) = E_DLT; /* Delete */
                rel_wai(data->tskid);
            }
            if (cep->callback != NULL) {
                ercd = E_DLT;
                (cep->callback)(cep->cepid, TFN_UDP_RCV_DAT, &ercd);
            }
        }
    } while(ercd == E_OK);

    ercd = E_OK;

    /* Mailbox and Memorypool Deletion */

    cep = &UDP_CEP[cepid];
    del_mbx(cep->id_udp_mbx);
    del_mpf(cep->id_udp_mpf);
    if (!TCP_MBXID_TOP)
        cep->id_udp_mbx = 0;
    if (!TCP_MPFID_TOP)
        cep->id_udp_mpf = 0;

    /* udpConnState */

    SNMP_UPDUDP(SNMP_CSTS_DELETETCB, cep->ipaddr, cep->portno);

    /* Clear TCP REP Control Block */

    cep->ipaddr  = 0;
    cep->portno  = 0;
    cep->options = 0;

    /* Clear Multicast Information */
#ifndef PPP
    clr_mgrp_cb(cep);
#endif

    /* Normal Completion */

    return tcpip_cretical(FALSE);

    /* Abnormal Completion */
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Search UDP Comminication End Point           */
/************************************************/

static int search_udp_cep(UW ipaddr, UH portno, T_UDP_CEP **p_cep, int *n)
{
    T_UDP_CEP *cep;
    int i;
    UW mask;

#ifndef PPP
    /* Broadcast */
    mask = byte4_to_long(subnet_mask);
    if (((ipaddr &~mask) | mask) == 0xFFFFFFFF) {
        for (i = *n; i < UDP_CEPID_MAX; i++) {
            cep = &UDP_CEP[i];
            if ((cep->options & UDP_BROADCAST) && (portno == cep->portno)) {
                if (ipaddr == 0xFFFFFFFF || (ipaddr & mask) == (cep->ipaddr & mask)) {
                    *n = ++i;
                    *p_cep = cep;
                    return UDP_BROADCAST;
                }
            }
        }
        *n = -1;
        return UDP_BROADCAST;
    }
    /* Multicast */
    else if ((ipaddr & 0xE0000000) == 0xE0000000) {
        for (i = 0; i < MULTICAST_GROUP_CNT; i++) {
            if (mgrp_cb[i].ipaddr == ipaddr && 
                mgrp_cb[i].portno == portno){
                *p_cep = mgrp_cb[i].cep;
                return UDP_MULTICAST;
            }
        }
    }
#endif
    for (i = 0; i < UDP_CEPID_MAX; i++) {
        cep = &UDP_CEP[i];
        if (ipaddr == cep->ipaddr && portno == cep->portno) {
            *p_cep = cep;
            return UDP_UNICAST;
        }
    }

    return -1;
}

/************************************************/
/* Calculate UDP Checksum                       */
/************************************************/

static UH udp_checksum(T_UDP_PKT *pkt)
{
    UW sum;
    int i;
    UH *p;


    /* UDP Pseudo-Header */

    p = (UH *)&pkt->ip.sa;
    sum  = *p++;                /* Source IP Address (High) */
    sum += *p++;                /* Source IP Address (Low) */
    sum += *p++;                /* Destination IP Address (High) */
    sum += *p++;                /* Destination IP Address (Low) */
    sum += htons(PROT_UDP);     /* UDP Protocol Number */
    sum += pkt->udp.len;        /* UDP Packet Length */

    /* UDP Header */

    p = (UH *)&pkt->udp;
    sum += *p++;                /* Source Port Number */
    sum += *p++;                /* Destination Port Number */
    sum += *p++;                /* UDP Packet Length */

    p = (UH *)pkt->data;
    i = (ntohs(pkt->udp.len) - sizeof (T_UDP_HEADER)) / 2; 
    while (--i >= 0)
          sum += *p++;

    if (ntohs(pkt->udp.len) & 1) /* Last Byte and Pad Byte */
        sum += (UH)(htons((ntohs(*p) & 0xff00)));

    /* Add Carry and Complement */
    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);
    if ((UH)sum == 0xffff)
        return 0xffff;

    return (UH)~sum;
}

/************************************************/
/* Get UDP Header Memory Block                  */
/************************************************/

static T_UDP_PKT *get_udp_blf(T_UDP_CEP *cep)
{
    T_UDP_PKT *pkt;
    ER ercd;
    ID mpfid;

    mpfid = cep->id_udp_mpf;
    ercd = pget_blf(&pkt, mpfid);
    if (ercd != E_OK) {
        if (ercd != E_TMOUT)
            udp_internal_error(ercd);
        return NULL;
    }

    memset(&(pkt->ctl), 0, sizeof (T_CTL_HEADER));
    pkt->ctl.cep = cep;
    pkt->ctl.mpfid = (UB)mpfid;
    return pkt;
}

/************************************************/
/* Make UDP Packet                              */
/************************************************/

static void make_udp_pkt(T_UDP_PKT *pkt, T_UDP_CEP *cep, const T_IPV4EP *p, const VP data, INT len)
{
    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    pkt->ip.prot = PROT_UDP;
    pkt->ip.sa = htonl(cep->ipaddr);
    pkt->ip.da = htonl(p->ipaddr);

    pkt->udp.sp = htons(cep->portno);
    pkt->udp.dp = htons(p->portno);
    pkt->udp.len = htons(UDP_HEADER_SZ + len);
    pkt->data = (UB *)data;
    pkt->udp.cs = udp_checksum(pkt);
}

/************************************************/
/* Send UDP Packet                              */
/************************************************/

ER udp_snd_dat(ID cepid, const T_IPV4EP *p_dstaddr, const VP data, INT len, TMO tmout)
{
    T_UDP_CEP *cep;
    T_UDP_PKT *pkt;
    ER ercd;
    B flg;

    /* Parameter Check */
    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;
    if ((UW)data & 1) /* Odd Address ? */
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if CEP Not Created */
    cep = &UDP_CEP[cepid];
    if (cep->portno == 0) {
        ercd = E_NOEXS;
        goto ERR;
    }

    pkt = get_udp_blf(cep);
    if (pkt == NULL) {
        ercd = E_QOVR;
        goto ERR;
    }
    if (tmout == TMO_NBLK) {
        pkt->ctl.callback = cep->callback;
        pkt->ctl.fncd = TFN_UDP_SND_DAT;
        pkt->ctl.parblk = len;
    }
    else
        pkt->ctl.tskid = (UB)vget_tid();

    make_udp_pkt(pkt, cep, p_dstaddr, data, len);
    vcan_wup();
    pkt->ctl.hedflg = FALSE;
    pkt->ctl.arprty = 0;

    ercd = snd_msg(ID_IP_SND_MBX, (T_MSG *)pkt);
    if (ercd != E_OK)
        goto ERR;

    tcpip_cretical(FALSE);
    if (tmout == TMO_NBLK)
        return E_WBLK;
    rot_rdq(TPRI_RUN);
    ercd = tslp_tsk(tmout);
    if (ercd != E_OK) {
        if(ercd == E_RLWAI && cep->ipaddr == 0 && cep->portno == 0)
            return E_DLT; /* Deleted CEP by udp_del_cep */
        else {
            flg = remove_sndque_udppkt(ID_IP_SND_MBX, cep, 0);
            if (!flg && ercd != E_RLWAI)
                return len;
            return ercd;
        }
    }
    SNMP_BYPCNT(snmp_mibbyp.udpOutDatagrams, 1);  /* udpOutDatagrams */
    return len;

    /* Abnormal Completion */
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Receive UDP Packet Now                       */
/************************************************/

static ER udp_rcv_dat_now(T_IPV4EP *p, UB *data, INT len, T_UDP_PKT *pkt)
{
    struct iphash_que *reass;
    ER ercd;
    ER len2;

    p->ipaddr = ntohl(pkt->ip.sa);
    p->portno = ntohs(pkt->udp.sp);
    len2 = ntohs(pkt->udp.len) - UDP_HEADER_SZ;
    
    if (!((T_IP *)pkt)->ctl.rcheck) {
#ifndef PPP
        if (!lan_read_pkt_end(data, len2, len)) {
            len2 = 0;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
#else
        if (!ppp_read_pkt_end(data, len2, len)) {
            len2=0;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
#endif
    }
    else {
        if (len2 > len) 
            len2 = E_BOVR;
        else
            len = len2;

        if (!udp_cpy_dat(data, (T_IP *)pkt, len))        /* Get data from reassembly buffer */
            len2 = E_OBJ;
    }

    if (pkt->udp.cs != 0 && len2 > 0) {
        pkt->data = data;
        if (pkt->udp.cs != udp_checksum(pkt)) {
            len2 = 0;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
    }

    if (((T_IP *)pkt)->ctl.rcheck) {
        reass = ((T_IP *)pkt)->ctl.cep; 
        reass->rtime = 0;
        pkt->ctl.rcheck = 0;
        clr_ip_hashque(reass);
        pkt = NULL;
    }
    else
        ercd = rel_blf(pkt->ctl.mpfid, pkt);

    if (ercd != E_OK)
        udp_internal_error(ercd);

    return len2;
    
}

/************************************************/
/* Queue UDP Reception Request                  */
/************************************************/

static ER udp_rcv_dat_que(T_UDP_CEP *cep, T_IPV4EP *p, VP buf, ER *len, TMO tmout)
{
    T_UDP_RCV *data;
    ER ercd;
    INT m;

#if (SIZEOF_INT == 4)
    m = 4 - ((INT)buf & 3);
    if (m == 4)
        m = 0;
    data = (T_UDP_RCV *)((UW)buf + m);
#else
    m = 0; /* 4.04.00a */
    data = buf;
#endif
    if (*len - m < sizeof (T_UDP_RCV)) /* Size for System Use */
        return E_PAR;

    data->p_dstaddr = (T_IPV4EP *)p;
    data->p_ercd = len;
    data->buf = buf;
    data->len = *len;
    if (tmout == TMO_NBLK)
        data->tskid = FALSE;
    else
        data->tskid = (UB)vget_tid();

    ercd = snd_msg(cep->id_udp_mbx, (T_MSG *)data);
    if (tmout == TMO_NBLK)
        return E_WBLK;
    return ercd;
}

/************************************************/
/* Receive UDP Packet                           */
/************************************************/

ER udp_rcv_dat(ID cepid, T_IPV4EP *p_dstaddr, VP data, INT len, TMO tmout)
{
    T_UDP_CEP *cep;
    T_UDP_PKT *pkt;
    T_UDP_RCV *udp_rcv;
    ER ercd, len2;

    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;
    if ((UW)data & 1) /* Odd Address ? */
        return E_PAR;
    if (tmout == TMO_POL && ID_IP_RCV_TSK != vget_tid())
        return E_PAR;
    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    cep = &UDP_CEP[cepid];
    if (cep->portno == 0) {
        ercd = E_NOEXS;
        goto ERR;
    }

    pkt = cep->pkt;
    if (pkt != NULL) {
        len = udp_rcv_dat_now(p_dstaddr, data, len, pkt);
        cep->pkt = NULL;
        tcpip_cretical(FALSE);
        return len;
    }
    else {
        len2 = len;
        ercd = udp_rcv_dat_que(cep, p_dstaddr, data, &len2, tmout);
        if (ercd != E_OK)
            goto ERR;
        vcan_wup();
        tcpip_cretical(FALSE);
        ercd = tslp_tsk(tmout);
        if (ercd < E_OK) {
            if (ercd == E_RLWAI && cep->ipaddr == 0 && cep->portno == 0) {
                udp_rcv = (T_UDP_RCV *)data;
                if (*udp_rcv->p_ercd == E_DLT)
                    return E_DLT;
            }
            remove_rcvque_udppkt(cep->id_udp_mbx, data);
            return ercd;
        }
        return len2;
    }

    /* Abnormal Completion */
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* UDP Reception Sub-routine 1                  */
/************************************************/

static ER udp_reception_sub1(T_UDP_RCV *data, T_UDP_PKT *pkt, T_UDP_CEP *cep)
{
    VP bp;
    ID tskid;
    ER *p_ercd;
    ER ercd, len,len2;

    p_ercd = data->p_ercd;
    data->p_dstaddr->ipaddr = ntohl(pkt->ip.sa);
    data->p_dstaddr->portno = ntohs(pkt->udp.sp);
    tskid = data->tskid;
    len = ntohs(pkt->udp.len) - UDP_HEADER_SZ;
    bp = data->buf;

    len2 = data->len;

    if(!((T_IP *)pkt)->ctl.rcheck) {
#ifndef PPP
        if (!lan_read_pkt_end(bp, len, data->len))
            len = E_OBJ;
#else
        if (!ppp_read_pkt_end(bp, len, data->len))
            len = E_OBJ;

        else if (len > len2) {
            len = E_BOVR;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
#endif
    }
    else {
        if (len > len2)
            len = E_BOVR;
        else
            len2 = len;
        
        /* Get data from reassembly buffer */
        if (!udp_cpy_dat(bp, (T_IP *)pkt, len2))
            len = E_OBJ;
    }

    if (pkt->udp.cs != 0 && len > 0) {
            pkt->data = (UB *)bp;
        if (pkt->udp.cs != udp_checksum(pkt)) {
            len = E_OBJ;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
    }

    if (tskid != FALSE) {
        *p_ercd = (ER)len;
        ercd = wup_tsk(tskid);
        if (ercd != E_OK)
            udp_internal_error(ercd);
    }
    else if (cep->callback != NULL)
        (cep->callback)(cep->cepid, TFN_UDP_RCV_DAT, &len);

    return len;

}

/************************************************/
/* UDP Reception Sub-routine 2                  */
/************************************************/

static T_UDP_PKT *udp_reception_sub2(T_UDP_PKT *pkt, T_UDP_CEP *cep)
{
    INT len;

    cep->pkt = pkt;
    len = ntohs(pkt->udp.len) - UDP_HEADER_SZ;
    if (cep->callback != NULL) {
       (cep->callback)(cep->cepid, TEV_UDP_RCV_DAT, &len);
        if (cep->pkt == NULL)
            return NULL;
    }

    cep->pkt = NULL;
#ifndef PPP
    if (!((T_IP *)pkt)->ctl.rcheck) {
        lan_ignore_pkt();
    }
#endif
    return pkt;
}

/************************************************/
/* UDP Reception Sub-routine 3                  */
/************************************************/

#ifdef BC_MHOME

static ER udp_reception_sub3(UB **data, T_UDP_PKT *pkt, T_UDP_CEP *cep)
{
    VP *bp;
    ER len,len2;

    len = 0;
#ifndef PPP
    len = ntohs(pkt->udp.len) - UDP_HEADER_SZ;
    bp = (VP *)&pkt->data;
    bp = (VP *)(((UB *)bp)+4);
    len2 = (ETHERNET_MTU-(IP_HEADER_SZ+UDP_HEADER_SZ));

    if (!pkt->ctl->rcheck) {
       if (!lan_read_pkt_end(bp, len, len2))
           len = E_OBJ;
       else if (len > len2) {
           len = E_BOVR;
            *data = (UB *)bp;
           SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
       }
    }
    else {
        if (len > len2) 
            len = E_BOVR;
        else
            len2 = len;
        udp_cpy_dat(*data, pkt, len);
        bp = *data;
    }
    if (pkt->udp.cs != 0 && len > 0) {
        pkt->data = (UB *)bp;
        if (pkt->udp.cs != udp_checksum(pkt)) {
            len = E_OBJ;
            SNMP_BYPCNT(snmp_mibbyp.udpInErrors, 1);  /* udpInErrors */
        }
        *data = (UB *)bp;
    }
#endif

    return len;
}

#endif

/************************************************/
/* UDP Reception Process                        */
/************************************************/

VP udp_reception(T_UDP_PKT *pkt)
{
    T_UDP_CEP *cep=NULL;
    T_UDP_RCV *data;
    ER ercd;
    int n;
    UW ipaddr;
    UH portno;
#ifdef BC_MHOME
    int len, typ;
    ER *p_ercd;
    UB *dp;
    ID tskid;
#endif

    n = 0;

    if (!((T_IP *)pkt)->ctl.rcheck) {
#ifndef PPP
        if (!lan_read_pkt(&pkt->udp, UDP_HEADER_SZ))
            return pkt;
#else
        if (!ppp_read_pkt(&pkt->udp, UDP_HEADER_SZ))
            return pkt;
#endif
    }

    ipaddr = ntohl(pkt->ip.da);
    portno = ntohs(pkt->udp.dp);

#ifndef BC_MHOME
    search_udp_cep(ipaddr, portno, &cep, &n);
#else
    typ = search_udp_cep(ipaddr, portno, &cep, &n);
#endif
    if (cep == NULL) {
#ifndef PPP
    if (!((T_IP *)pkt)->ctl.rcheck)
            lan_ignore_pkt();
#endif
        SNMP_BYPCNT(snmp_mibbyp.udpNoPorts, 1);  /* udpNoPorts */
        return pkt;
    }
    SNMP_BYPCNT(snmp_mibbyp.udpInDatagrams, 1);  /* udpInDatagrams */


    ercd = prcv_msg((T_MSG **)&data, cep->id_udp_mbx);
#ifndef BC_MHOME
    if (ercd == E_OK)
        udp_reception_sub1(data, pkt, cep);
    else if (ercd == E_TMOUT)
        pkt = udp_reception_sub2(pkt, cep);
    else
        udp_internal_error(ercd);
#else /* Broad cast multi home support */
    if (typ == UDP_UNICAST || typ == UDP_MULTICAST) {
        if (ercd == E_OK)
            udp_reception_sub1(data, pkt, cep);
        else if (ercd == E_TMOUT)
            pkt = udp_reception_sub2(pkt, cep);
        else{
            udp_internal_error(ercd);
            goto END;
        }
    }
    else {
        if (ercd == E_OK)
            len = udp_reception_sub3(&dp, pkt, cep);
        else {
            udp_internal_error(ercd);
            if(!pkt->ctl->rcheck)
               lan_ignore_pkt();
            goto END;
        }
        for (;;) {
            data->p_dstaddr->ipaddr = ipaddr;
            data->p_dstaddr->portno = portno;
            if (len > data->len)
                len = data->len;
            tskid = data->tskid;
            p_ercd = data->p_ercd;
            if (len > 0)
                memcpy(data->buf, dp, len);
            if (tskid != FALSE){
                *p_ercd = (ER)len;
                ercd = wup_tsk(tskid);
                if (ercd != E_OK)
                 udp_internal_error(ercd);
            }
            else if (cep->callback != NULL)
                (cep->callback)(cep->cepid, TFN_UDP_RCV_DAT, &len);
            search_udp_cep(ipaddr, portno, &cep, &n);
            if (n == -1)
                break;
            ercd = prcv_msg((T_MSG **)&data, cep->id_udp_mbx);
            if (ercd != E_OK){
                udp_internal_error(ercd);
                goto END;
            }
        }
    }
END:
#endif
    return pkt;
}

/************************************************/
/* Cancel Pending UDP Process                   */
/************************************************/

ER udp_can_cep(ID cepid, FN fncd)
{
    T_UDP_CEP *cep;
    T_UDP_RCV *data;
    ER ercd;
    B sflg, rflg;

    sflg = rflg = FALSE;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;

    if (fncd != TFN_UDP_SND_DAT && fncd != TFN_UDP_RCV_DAT && fncd != TFN_UDP_ALL)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if CEP Not Created */

    cep = &UDP_CEP[cepid];
    if (cep->portno == 0) {
        ercd = E_NOEXS;
        goto ERR;
    }

    if (fncd == TFN_UDP_RCV_DAT || fncd == TFN_UDP_ALL) {
        do {
            ercd = prcv_msg((T_MSG **)&data, cep->id_udp_mbx);
            if (ercd == E_OK) {
                rflg = TRUE;
                if (data->tskid != FALSE) {
                    *(data->p_ercd) = E_DLT; /* Delete */
                    rel_wai(data->tskid);
                }
                if (cep->callback != NULL) {
                    ercd = E_RLWAI;
                    (cep->callback)(cep->cepid, TFN_UDP_RCV_DAT, &ercd);
                }
            }
        } while(ercd == E_OK);
    }
    if (fncd == TFN_UDP_SND_DAT || fncd == TFN_UDP_ALL) {
        sflg = remove_sndque_udppkt(ID_IP_SND_MBX, cep, E_RLWAI);
        /*cep->pkt = NULL;*/
    }

    tcpip_cretical(FALSE);
    if (sflg || rflg)
       return E_OK;
    else 
       return E_OBJ;

ERR:
    tcpip_cretical(FALSE);
    return ercd;

}

/************************************************/
/* Set UDP Communication End Point Options      */
/************************************************/

ER udp_set_opt(ID cepid, INT optname, const VP optval, INT optlen)
{
#ifndef PPP
    T_UDP_CEP *cep;
    T_IPV4EP gaddr;
    int n;
    ER ercd;

    ercd = E_OK;
    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)UDP_CEPID_MAX)
        return E_ID;

    /* Error if CEP Not Created */

    cep = &UDP_CEP[cepid];
    if (cep->portno == 0)
       return E_NOEXS;

    switch (optname) {
    case IP_ADD_MEMBERSHIP:
        if (cep->options != 0)
            return E_OBJ;
        /* Check duplicat address */
        n = search_igmp_group((T_IPV4EP *)optval);
        if (n != -1)
            return E_OBJ;
        /* Check entry space */
        gaddr.ipaddr = 0;
        gaddr.portno = 0;
        n = search_igmp_group(&gaddr);
        if (n == -1)
            return E_OBJ;
        memcpy(&gaddr, optval, sizeof(T_IPV4EP));
        mgrp_cb[n].ipaddr = gaddr.ipaddr;
        mgrp_cb[n].portno = gaddr.portno;
        mgrp_cb[n].cep = cep;
        cep->options = UDP_MULTICAST;
        /* Mod 3March2003 - changed IGMP_UNUSED to IGMP_REPORT */
        ercd = igmp_snd_dat(IGMP_REPORT, gaddr.ipaddr, gaddr.ipaddr);
        break;
    case IP_DROP_MEMBERSHIP:
        n = search_igmp_group((T_IPV4EP *)optval);
        if (n == -1)
            return E_OBJ;
        mgrp_cb[n].ipaddr = 0L;
        mgrp_cb[n].portno = 0;
        mgrp_cb[n].cep = NULL;
        cep->options = 0;
        break;
    case IP_BROADCAST:
        if ((BOOL)optval == TRUE){
            if (cep->options != 0)
                return E_OBJ;
            cep->options = UDP_BROADCAST;
        }
        else
            cep->options = 0;
        break;
    default:
        return E_PAR;

    }
    return ercd;
#else
    return E_NOSPT;
#endif
}

/************************************************/
/* Get UDP Communication End Point Options      */
/************************************************/

ER udp_get_opt(ID cepid, INT optname, VP optval, INT optlen)
{
    return E_NOSPT;
}

/* end */
