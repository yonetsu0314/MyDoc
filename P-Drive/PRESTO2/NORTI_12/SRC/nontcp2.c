/******************************************************************************
* NORTi Network TCP Module 2/2                                                *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 05/Mar/1999 Created.                                                        *
* 03/Oct/2000 (Ver. 4.02 Release)                                             *
* 26/Oct/2000 Modified tcp_transmit2 timeout (PPP)                            *
* 27/Oct/2000 Modified Send RST Packet Process.                               *
* 01/Dec/2000 Change delayed ACK Process.                                     *
* 01/Dec/2000 (Ver. 4.03 Release)                                             *
* 22/Jan/2001 Modified for the transfer of exceeding 4GB.                     *
* 10/Feb/2001 (Ver. 4.04 Release)                                             *
* 27/Feb/2001 Modified tcp_rcv_data, if (len <= 0) → if (len < 0)            *
* 15/Mar/2001 Modified tcp_rcv_data, for zero window.                         *
* 06/Mar/2001 Modified tcp_reception, for flow control.                       *
* 06/Apr/2001 Modified illegal access to p_dstaddr.                           *
* 06/Apr/2001 Modified tcp_rcv_rst, unnecessary callback is called.           *
* 18/Jun/2001 Modified tcp_reception, ETHERNET_MTU → TCP_MTU.                *
* 12/Sep/2001 Modified tcp_get_sdat, TCP_MTU → (cep->win0 >> 2).             *
* 13/Sep/2001 Added part of resending request in tcp_rcv_data.                *
* 14/Sep/2001 Modified adjust_getp.                                           *
* 14/Sep/2001 Modified tcp_rcv_ack, tcp_rcv_data for tcp_cls_cep(TMO_NBLK)    *
* 07/Nov/2001 Modified tcp_rcv_ack for closing seq of FIN+ACK                 *
* 14/Nov/2001 Modified tcp_rcv_ack for reply of Zero Window Probe             *
* 18/Feb/2002 The RST packet transmitting mechanism was changed.              *
* 04/Apr/2002 The function name was changed into tcp_flw_ctl from adjust_getp.*
* 23/Mar/2002 Updated  TCP retransmission improved                            *
*                Exponential backoff, Jacobson & Karn's algorithm implemented *
* 23/Mar/2002 Unnecessary looping of ACK retransmission avoided               *
* 28/Apr/2002 Corrected the Slow start, congestion avoidance and              *
*             Fast recovary  algorithms implementation.                       *
* 1/May/2002  Added code for keep alive timer.                                *
* 21/Jun/2002 Modified zero window probe timeout                              *
* 24/Jun/2002 Modified tcp_transmit function.                                 *
* 25/Jun/2002 Modified tcp_flw_ctl function.                                  *
* 06/Dec/2002 Added Seq validation for TCP reception                          *
* 25/Mar/2003 Modified tcp reception to get target MSS value                  *
* 25/Mar/2003 Modified for SNMP                                               *
******************************************************************************/

#include <string.h>
#include <stdio.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"
#ifdef SOCK
#include "nonsock.h"
#endif
#ifdef PPP
#include "noneppp.h"
#endif

#ifdef SOCK
extern SEL_INFO sel_info[];
#endif

void tcp_cep_unused(T_TCP_CEP *cep);

/************************************************/
/* Search TCP Communication End Point           */
/************************************************/

static T_TCP_CEP *search_tcp_cep(UW da, UH dp, UW sa, UH sp)
{
    T_TCP_CEP *cep;
    T_TCP_CEP *cep2;
    int i;

    cep2 = NULL;

    for (i = 0; i < TCP_CEPID_MAX; i++){
        cep = &TCP_CEP[i];
        if (IS_USED(cep->state)
         && cep->ipaddr == da && cep->portno == dp){
            if (cep->dstipa == sa && cep->dstpor == sp)
                return cep;
            if (cep->state == CEP_WAIT_P_OPEN
             && cep->dstipa == 0 && cep->dstpor == 0){
                cep2 = cep;
                i++;
                break;
            }
        }
    }
    for (; i < TCP_CEPID_MAX; i++){
        cep = &TCP_CEP[i];
        if (IS_USED(cep->state)
         && cep->ipaddr == da && cep->portno == dp
         && cep->dstipa == sa && cep->dstpor == sp)
            return cep;
    }
    return cep2;
}

/************************************************/
/* Calculate TCP Checksum                       */
/************************************************/

static UH tcp_checksum(T_TCP_PKT *pkt)
{
    UW sum;
    UH *p, *end;
    int i;

    /* TCP Pseudo-Header */

    p = (UH *)&pkt->ip.sa;
    sum  = *p++;                /* Source IP Address (High) */
    sum += *p++;                /* Source IP Address (Low) */
    sum += *p++;                /* Destination IP Address (High) */
    sum += *p++;                /* Destination IP Address (Low) */
    sum += htons(PROT_TCP);     /* TCP Protocol Number */
    sum += htons(ntohs(pkt->ip.tl) - IP_HEADER_SZ);

    /* TCP Header */

    p = (UH *)&pkt->tcp;
    sum += *p++;                /* Source Port Number */
    sum += *p++;                /* Destination Port Number */
    sum += *p++;                /* Sequence Number (High) */
    sum += *p++;                /* Sequence Number (Low) */
    sum += *p++;                /* Acknowledgment Number (High) */
    sum += *p++;                /* Acknowledgment Number (Low) */
    sum += *p++;                /* Header Length (Data Offset), Flags */
    sum += *p++;                /* Window Size */
    p++;                        /* Checksum */
    sum += *p++;                /* Urgent Pointer */

    /* TCP Option & Data */

    i = ntohs(pkt->ip.tl) - IP_HEADER_SZ - TCP_HEADER_SZ;
    if (i > 0){
        p = (UH *)pkt->data;
        end = (UH *)((UB *)p + (i & ~1));
        while (p < end)
            sum += *p++;
        if (i & 1)
            sum += (UH)(htons((ntohs(*p) & 0xff00)));
    }

    /* Add Carry and Complement */

    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);
    return (UH)~sum;
}

/************************************************/
/* Seq validation for incoming TCP segments     */
/************************************************/

static B tcp_seq_valid(T_TCP_CEP *cep, UW rxseq, int len, UH rxwin)
{
    UW seq_next;
    W seq_check;

    seq_next = cep->ack;
    if (len == 0) {
        if (rxwin == 0) {
            if (rxseq == seq_next)
                return TRUE;
        }
        else {
            seq_check = (W)(seq_next - rxseq);
            if (seq_check <= 0) {
                seq_check = (W)(rxseq - (seq_next + (UW)rxwin));
                if (seq_check < 0)
                    return TRUE;
            }
            
        }
    }
    else {
        if (rxwin > 0) {
            seq_check = (W)(rxseq - seq_next);
            if (seq_check >= 0) {
                seq_check = (W)(rxseq - (seq_next + (UW)rxwin));
                if (seq_check < 0)
                    return TRUE;
            }
            
            seq_check = (W)(seq_next - (rxseq + (UW)len - 1));
            if (seq_check <= 0) {
                seq_check = (W)((rxseq + (UW)len - 1) - (seq_next + (UW)rxwin));
                if (seq_check < 0)
                    return TRUE;
            }

        }
    }

    return FALSE;
}

/************************************************************************************
* Module Name   : tcp_rtt_cal                                                       *
* Purpose       : This funtion calulates RTO value for retransmission by using the  *
*                 calculated value of RTT.                                          *
*                 (Implemented as per RFC 2988-Computing TCP's Retransmission timer)*
* Arguments     : T_TCP_CEP *cep - TCP CEP structure                                *
*                 UW rtime       - Round trip time of a TCP segment                 *
* Return value  : None.                                                             *
* Author        : AK                                                                *
* Date          : 23-Mar-2002                                                       *
* Modified UW rtime as W rtime and added else cond for rtime - A.K 18/jul/2002      *
************************************************************************************/

static void tcp_rtt_cal(T_TCP_CEP *cep,W rtime)
{
    if (IS_USED(cep->state))            /* Check for valid CEP state */
    {
        if(rtime == 0)
            rtime = 1;

        /* For first measurment of RTO use  the equation        */
        /*  srtt = r                                            */
        /*  rttvar = r / 2                                      */
        /* Else use                                             */
        /*  rttvar = (1-B) * rttvar + B * |srtt - r|            */
        /*  srtt = (1-A) * srtt + (A * r)                       */
        /* Calculate RTO using the equation                     */
        /* RTO = srtt + max(g, k * rttvar)                      */

        if (cep->retry.srtt != 0) {
            rtime = rtime - (cep->retry.srtt >> 3);

            if((cep->retry.srtt += rtime) <= 0)
                cep->retry.srtt = 1;

            if (rtime < 0)
                rtime = -rtime;
            rtime -= (cep->retry.rttvar >> 2);
            cep->retry.rttvar += rtime;
        }
        else{
            cep->retry.srtt = rtime << 3;
            cep->retry.rttvar = MAX((rtime << 1), tTCP_RTO_LBOUND);
        }

        cep->retry.rto = (cep->retry.srtt >> 3) + cep->retry.rttvar;

        /* RTO value should not less than lower bound and       */
        /* should not greater than upper bound                  */
        if (cep->retry.rto > (UW)tTCP_RTO_UBOUND)
            cep->retry.rto = (UW)tTCP_RTO_UBOUND;
        if (cep->retry.rto < (UW)tTCP_RTO_LBOUND)
            cep->retry.rto = (UW)tTCP_RTO_LBOUND;
    }
}

/************************************************/
/* Put Sending Data to Window Buffer            */
/************************************************/

INT tcp_put_sdat(T_TCP_CEP *cep, const UB *buf, INT len)
{
    W size;
    INT i, len2;

    size = cep->sbufsz - (cep->sputp - (cep->seq - cep->seq0));
    if (size <= 0)
        return 0;
    if ((W)len > size)
        len = (INT)size;

    i = (INT)(cep->sputp % cep->sbufsz);
    if (i + len <= cep->sbufsz)
        memcpy(cep->sbuf + i, buf, len);
    else{
        len2 = cep->sbufsz - i;
        memcpy(cep->sbuf + i, buf, len2);
        buf += len2;
        len2 = len - len2;
        memcpy(cep->sbuf, buf, len2);
    }
    cep->sputp += len;
    return len;
}

/************************************************/
/* Get Sending Buffer from Window Buffer        */
/************************************************/

INT tcp_get_sbuf(UB **p_buf, T_TCP_CEP *cep)
{
    W size;
    INT i, len;

    size = cep->sbufsz - (cep->sputp - (cep->seq - cep->seq0));
    if (size == 0)
        return 0;
    if (size > 0x7fffL)
        len = 0x7fff;
    else
        len = (INT)size;

    i = (INT)(cep->sputp % cep->sbufsz);
    if (i + len > cep->sbufsz)
        len = cep->sbufsz - i;
    *p_buf = cep->sbuf + i;
    return len;
}

/*******************************************************/
/* Put Buffer Containing Sending Data to Window Buffer */
/*******************************************************/

ER tcp_put_sbuf(T_TCP_CEP *cep, INT len)
{
    W size;

    size = cep->sbufsz - (cep->sputp - cep->sgetp);
    if ((W)len > size)
        return E_OBJ;

    cep->sputp += len;
    return E_OK;
}

/************************************************/
/* Get optimized receive window size            */
/************************************************/

UH rwin_size(T_TCP_CEP *cep)
{
    UH  win = (UH)(cep->rbufsz - (cep->rputp - cep->rgetp));

    if (cep->rbufsz > (TCP_MTU*4)){
        if (win < (cep->rbufsz/4))
            return 0;
    }
    else if (cep->rbufsz > TCP_MTU){
        if (win < TCP_MTU)
            return 0;
    }
    else {
        if (win < cep->rbufsz) {
            /* 26 Jan2003 AK - Send actual win size */
            return win;
        }
    }
    return win;
}

/************************************************/
/* Can ACK be transmitted ?                     */
/************************************************/

static BOOL can_transmit_ack(UH win1, UH win2, T_TCP_CEP *cep)
{
    /* 10Feb2003 - ACK for every second segment */
    if (cep->rgetp == cep->rputp) {
        cep->dackflag = 0;
        return TRUE;
    }

    /* 16-11-2002 AK - Send this ACK from tcp_rcv_dat */
    if (win1 == 0)
        return FALSE;

    if ((TCP_MTU*4) < cep->rbufsz) {
        if (win1 < (cep->rbufsz/4) && (cep->rbufsz/4) <= win2)
            return TRUE;
    }
    else if (cep->rbufsz > TCP_MTU){
        if (win1 < TCP_MTU && TCP_MTU <= win2)
            return TRUE;
    }
    else{
        if (win1 < cep->rbufsz && cep->rbufsz <= win2)
            return TRUE;
    }
    return FALSE;
}

/************************************************/
/* Get Received Data from Window Buffer         */
/************************************************/

INT tcp_get_rdat(UB *buf, T_TCP_CEP *cep, INT len)
{
    INT i, len2;
    W size;
    UH win1, win2;

    size = cep->rputp - cep->rgetp;
    if (size <= 0)
        return 0;

    win1 = (UH)(cep->rbufsz - size);
    if ((W)len > size)
        len = (INT)size;

    i = (INT)(cep->rgetp % cep->rbufsz);
    if (i + len <= cep->rbufsz)
        memcpy(buf, cep->rbuf + i, len);
    else {
        len2 = cep->rbufsz - i;
        memcpy(buf, cep->rbuf + i, len2);
        buf += len2;
        len2 = len - len2;
        memcpy(buf, cep->rbuf, len2);
    }
    cep->rgetp += len;
    win2 = (UH)(cep->rbufsz - (cep->rputp - cep->rgetp));
    
    if (can_transmit_ack(win1, win2, cep)) {
        if(cep->ackpkt != NULL){
            cep->dtime = SYSCK.ltime;
            tcp_transmit_ack(0);
        }
        else{
            cep->sflag |= ACK_FLAG;
            tcp_transmit(cep);
        }
    }
  
    return len;
}

/**********************************************************/
/* Get Buffer Containing Received Data from Window Buffer */
/**********************************************************/

INT tcp_get_rbuf(UB **p_buf, T_TCP_CEP *cep)
{
    INT i, len;
    W size;

    size = cep->rputp - cep->rgetp;
    if (size == 0)
        return 0;
    if (size > 0x7fffL)
        len = 0x7fff;
    else
        len = (INT)size;

    i = (INT)(cep->rgetp % cep->rbufsz);
    *p_buf = cep->rbuf + i;

    if (i + len >= cep->rbufsz)
        len = cep->rbufsz - i;

    return len;
}

/************************************************/
/* Release Receiving Buffer to Window Buffer    */
/************************************************/

ER tcp_rel_rbuf(T_TCP_CEP *cep, INT len)
{
    W size;
    UH win1, win2;

    size = cep->rputp - cep->rgetp;
    if ((W)len > size)
        return E_OBJ;
    win1 = (UH)(cep->rbufsz - size);
    cep->rgetp += len;
    win2 = (UH)(cep->rbufsz - (cep->rputp - cep->rgetp));

    if (can_transmit_ack(win1, win2, cep)){
        if(cep->ackpkt != NULL){
            cep->dtime = SYSCK.ltime;
            tcp_transmit_ack(0);
        }
        else{
            cep->sflag |= ACK_FLAG;
            tcp_transmit(cep);
        }
    }

    return E_OK;
}

/************************************************/
/* Get Sending Data from Window Buffer          */
/************************************************/

static INT tcp_get_sdat(UB *buf, T_TCP_CEP *cep, W *p_size, BOOL retry)
{
    UW pos;
    W size;
    W seq_check;
    int i, len, len2;

    if (retry)
        pos = cep->seq - cep->seq0;
    else {
        pos = cep->sgetp;
        /* Dont transmit new data during retransmission */
        if (cep->hseq != 0 && !(cep->dackmd)) {
            seq_check = ((W)((cep->hseq - cep->seq0) - pos));
            if ( seq_check < 0)
                return 0;
        }
    }

    size = cep->sputp - pos;

    if (size <= 0)
        return 0;

    *p_size = size;

    if (size > (W)cep->mss)
        size = cep->mss;

    if (retry) {
        /* <YS-010914-004> Do not send until get enough space */
        if (cep->win1 == 0) {
            buf[0] = 0;
            return -1; /* To Send Zero Window Probe */
        }
        if (size > (W)cep->win1)
            len = cep->win1;
        else
            len = (int)size;
    }
    else {
        /* Do not send until get enough space */
        if (cep->smax == 0 || (cep->win == 0 && cep->dackmd == FALSE)) {
            if ((cep->time = SYSCK.ltime) == 0)
                cep->time = (UW)-1;
            return 0;
        }

        if (size > (W)cep->smax)
            len = cep->smax;
        else
            len = (int)size;

        if (cep->smax != 0)
            cep->smax -= len;
    }

    i = (int)(pos % cep->sbufsz);

    if (i + len <= cep->sbufsz)
        memcpy(buf, cep->sbuf + i, len);
    else {
        len2 = cep->sbufsz - i;
        memcpy(buf, cep->sbuf + i, len2);
        buf += len2;
        len2 = len - len2;
        memcpy(buf, cep->sbuf, len2);
    }
    cep->sgetp = pos + len;
    return len;
}

/************************************************/
/* Put Received Data to Window Buffer           */
/************************************************/

static INT tcp_put_rdat(T_TCP_PKT *pkt, T_TCP_CEP *cep, UB *buf, INT len)
{
    UW pos;
    W size;
    W i, len2;
#ifdef SOCK
    int ctr;
#endif

    pos = htonl(pkt->tcp.seq) - (cep->syn + 1);

    size = (W)(pos - cep->rputp);
    if (size > 0)
        return (INT)(cep->rputp - pos);

    i = pos + len - cep->rputp;
    if (i <= 0)
        return len;
    pos += len - i;
    buf += len - i;
    len = i;

    size = cep->rbufsz - (cep->rputp - cep->rgetp);
    if (size == 0)
        return 0;
    if ((W)len > size)
        len = (INT)size;

    i = (INT)(pos % cep->rbufsz);
    if (i + len <= cep->rbufsz)
        memcpy(cep->rbuf + i, buf, len);
    else {
        len2 = cep->rbufsz - i;
        memcpy(cep->rbuf + i, buf, len2);
        buf += len2;
        len2 = len - len2;
        memcpy(cep->rbuf, buf, len2);
    }
    cep->rputp += len;

#ifdef SOCK /* ready to read data */
    for (ctr=0; ctr < MAX_SELECT_INFO; ctr++) {
        if (sel_info[ctr].cepid == cep->cepid) {
            if (sel_info[ctr].taskid > 0)
                wup_tsk(sel_info[ctr].taskid);
        }
    }
#endif

    return (INT)(cep->rputp - (ntohl(pkt->tcp.seq) - (cep->syn + 1)));
}

/************************************************/
/* Make ACK Packet                              */
/************************************************/
/* Added 29/08/2002 - AK - process invalid TCP packets */
static void make_tcp_ack_pkt(T_TCP_PKT *pkt, T_TCP_CEP *cep)
{
    UW tmp1;
    UH tmp2;
    UH flag = 0;

    pkt->ip.prot = PROT_TCP;
    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    tmp1 = ntohl(pkt->ip.sa);
    pkt->ip.sa = pkt->ip.da;
    pkt->ip.da = htonl(tmp1);
    tmp2 = ntohs(pkt->tcp.sp);
    pkt->tcp.sp = pkt->tcp.dp;
    pkt->tcp.dp = htons(tmp2);

    if ((cep->state == CEP_TERMINATED || cep->state == CEP_CLOSING) \
        && !(cep->sflag & FIN_FLAG)) 
        pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp + 1);
    else
        pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp);

    pkt->tcp.ack = htonl(cep->ack);
    flag = ACK_FLAG;

    pkt->tcp.flag =
       htons((UH)(((sizeof (T_TCP_HEADER) / 4) << 12) | flag));

    pkt->tcp.win = rwin_size(cep);
    pkt->tcp.win = htons(pkt->tcp.win);
    pkt->tcp.urgp = 0;
    pkt->ip.tl = htons((UH)(IP_HEADER_SZ + TCP_HEADER_SZ));
    pkt->tcp.cs = tcp_checksum(pkt);
}

/************************************************/
/* Make RST Packet                              */
/************************************************/

static void make_tcp_rst_pkt(T_TCP_PKT *pkt, T_TCP_CEP *cep)
{
    UW tmp1;
    UH tmp2;

    pkt->ip.prot = PROT_TCP;
    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    tmp1 = ntohl(pkt->ip.sa);
    pkt->ip.sa = pkt->ip.da;
    pkt->ip.da = htonl(tmp1);
    tmp2 = ntohs(pkt->tcp.sp);
    pkt->tcp.sp = pkt->tcp.dp;
    pkt->tcp.dp = htons(tmp2);

    tmp1 = ntohl(pkt->tcp.seq);
    pkt->tcp.seq = pkt->tcp.ack;
    if (ntohs(pkt->tcp.flag) & (FIN_FLAG|SYN_FLAG))
        tmp1+=1;
    pkt->tcp.ack = htonl(tmp1);

    pkt->tcp.flag =
       htons((UH)(((sizeof (T_TCP_HEADER) / 4) << 12) | (ACK_FLAG|RST_FLAG)));
    pkt->tcp.win = 0;
    pkt->tcp.urgp = 0;
    pkt->ip.tl = htons((UH)(IP_HEADER_SZ + TCP_HEADER_SZ));
    pkt->tcp.cs = tcp_checksum(pkt);

    SNMP_BYPCNT(snmp_mibbyp.tcpOutRsts, 1);  /* tcpOutRsts */
}

/************************************************/
/* Make TCP Packet                              */
/************************************************/

static W make_tcp_pkt(T_TCP_PKT *pkt, T_TCP_CEP *cep, BOOL retry)
{
    W seq_check;
    W size = 0;
    UW maxseq;
    UH flag;
    INT len;
    BOOL zerow = FALSE;

    flag = cep->sflag;
    cep->sflag &= ~(ACK_FLAG|RST_FLAG);
    if (flag & (RST_FLAG|SYN_FLAG)) {
        len = 0;
        if (flag & RST_FLAG) { /* Send RST */
            make_tcp_rst_pkt(pkt, cep);
            return size;
        }
    }
    else {
        if (cep->state == CEP_UNUSED || cep->state == CEP_DISCONNECTED)
            len = 0;
        else
            len = tcp_get_sdat(pkt->data, cep, &size, retry);
    }

    if (len == 0) {
        if (flag == 0)
            return -1;
        if (flag == (ACK_FLAG|FIN_FLAG) && (cep->rflag & FIN_FLAG))
            flag &= ~FIN_FLAG;
        size = 0;  /* Set this to avoid looping with ACK segment */
    }

    if (cep->dackflag & TCP_ZERO_PROBE) {
        cep->dackflag &= ~TCP_ZERO_PROBE;
        len = -1;
    }
        

    seq_check = (W)(cep->sputp - (cep->seq - cep->seq0));
    if (seq_check > 0)   /* Window Buffer not empty */
        flag &= ~FIN_FLAG;

    if (len != 0 || !(flag & (RST_FLAG|SYN_FLAG)))
        flag |= ACK_FLAG;

    if (len == -1) { /* To Send Zero Window Probe */
        len = 0;
        zerow = TRUE;
    }
    else if (len != 0)
        flag |= PSH_FLAG;

    memset(pkt->eth.da, 0, 6);
    pkt->eth.etype = htons(ETYPE_IP);

    pkt->ip.da   = htonl(cep->dstipa);
    pkt->ip.prot = PROT_TCP;
    pkt->ip.sa   = htonl(cep->ipaddr);
    pkt->tcp.sp  = htons(cep->portno);
    pkt->tcp.dp  = htons(cep->dstpor);

    if (zerow) /* ZeroWindowProbe */
        pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp - 1);
    else if (len == 0 && (cep->rflag & FIN_FLAG)) /* ACK of FIN */
        pkt->tcp.seq = htonl(cep->seq);
    else {
        if ((cep->state == CEP_TERMINATED || cep->state == CEP_CLOSING) \
        && !(cep->sflag & FIN_FLAG)) 
            pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp - len + 1);
        else    
           pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp - len);
    }   
        
    pkt->tcp.ack = htonl(cep->ack);
    pkt->tcp.win = rwin_size(cep);

    pkt->ctl.cep = cep;
    pkt->tcp.win = htons(pkt->tcp.win);
    pkt->tcp.urgp = 0;

    /* Used to avoid retransmission of ACK segments */
    cep->retry.flag = flag;         
    maxseq = htonl(pkt->tcp.seq);
    if (cep->mseq == 0) {
        if (flag & SYN_FLAG) maxseq++;
        cep->mseq = maxseq;
    }
    else {
        maxseq += len;
        if (flag & FIN_FLAG || flag & SYN_FLAG)
            maxseq ++;

        seq_check = (W)(maxseq - cep->mseq);
        if (seq_check > 0)
            cep->mseq = maxseq;
    }

    if (flag & SYN_FLAG) {
        pkt->tcp.flag =
            htons((UH)((((sizeof (T_TCP_HEADER) + 4) / 4) << 12) | flag));
        pkt->data[0] = 2;
        pkt->data[1] = 4;
        pkt->data[2] = (UB)((TCP_MTU) >> 8);
        pkt->data[3] = (UB)(TCP_MTU);
        len += 4;
    }
    else {
        pkt->tcp.flag =
            htons((UH)(((sizeof (T_TCP_HEADER) / 4) << 12) | flag));
    }
    pkt->ip.tl = htons((UH)(IP_HEADER_SZ + TCP_HEADER_SZ + len));
    pkt->tcp.cs = tcp_checksum(pkt);

    seq_check = (W)(maxseq - cep->mseq);
    if (len != 0 && (retry || (seq_check < 0))) {   /* 再送回数 */
            SNMP_BYPCNT(snmp_mibbyp.tcpRetransSegs, 1);  /* tcpRetransSegs */
    }

    return size;
}

/************************************************/
/* ACK Reception                                */
/************************************************/

void tcp_rcv_ack(T_TCP_PKT *pkt, T_TCP_CEP *cep, int len)
{
    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk, slen;
    W seq_check;
#ifdef SOCK
    W size;
    int ctr;
#endif

    switch (cep->state) {
    case CEP_WAIT_P_OPEN:
        if (cep->dstpor == 0)
            break;
        if (ntohl(pkt->tcp.ack) != cep->seq + 1)
            break;
        cep->seq0 = ++cep->seq;
        cep->win1 = cep->win0 = ntohs(pkt->tcp.win);
        cep->sflag &= ~SYN_FLAG;
        cep->state = CEP_CONNECTING;
        SNMP_UPDTCP(SNMP_CSTS_DELETETCB, cep->ipaddr, cep->portno, 0, 0);
        SNMP_UPDTCP(SNMP_CSTS_ESTABLISHED, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);
        SNMP_BYPCNT(snmp_mibbyp.tcpPassiveOpens, 1);  /* tcpPassiveOpens */

        fncd = cep->rfncd;
        if (fncd != TFN_TCP_ACP_CEP || (ntohs(pkt->tcp.flag) & RST_FLAG))
            break;
        goto L1;

    case CEP_WAIT_A_OPEN:
        if (ntohl(pkt->tcp.ack) != cep->seq + 1) {
            cep->sflag = ACK_FLAG|RST_FLAG;
            cep->seq = ntohl(pkt->tcp.ack);
            break;
        }

        cep->seq0 = ++cep->seq;
        cep->win1 = cep->win0 = ntohs(pkt->tcp.win);
        cep->sflag &= ~SYN_FLAG;
        if (!(ntohs(pkt->tcp.flag) & ACK_FLAG))
            cep->sflag |= RST_FLAG;
        break;

    case CEP_CONNECTING:

        /* Modified on Nov 6, 2001 */
        /* ACK for Zero Window Probe & Keep Alive probe*/
        if (len == 0 && cep->ack == ntohl(pkt->tcp.seq) + 1)
            cep->sflag |= ACK_FLAG; 

        cep->seq = ntohl(pkt->tcp.ack);

#ifdef SOCK /* ready to write data */
    size = cep->sbufsz - (cep->sputp - (cep->seq - cep->seq0));
    if (size > 0) {
        for (ctr=0; ctr < MAX_SELECT_INFO; ctr++) {
            if (sel_info[ctr].cepid == cep->cepid) {
                if (sel_info[ctr].taskid > 0)
                    wup_tsk(sel_info[ctr].taskid);
            }
        }
    }
#endif

        fncd = cep->sfncd;
        if (fncd == TFN_TCP_SND_DAT)
            slen = tcp_put_sdat(cep, cep->spar, cep->slen);
        else if (fncd == TFN_TCP_GET_BUF)
            slen = tcp_get_sbuf(cep->spar, cep);
        /* Pending send FIN */
        else if (cep->dcls_id != 0 && cep->sputp == (cep->seq - cep->seq0)) {
            if (cep->dcls_id != -1)   /* not TMO_NBLK */
                wup_tsk(cep->dcls_id);
            else if (cep->dcls_id == -1) { /* Modified on Nov 27, 2001 */
                cep->state = CEP_CLOSING;
                cep->rfncd = TFN_TCP_CLS_CEP;
            }
            break;
        }
        else
            break;
        if (slen <= 0) /* not enough space in the buffer yet */
            break;

        if (fncd != TFN_TCP_SND_DAT && fncd != TFN_TCP_RCV_DAT  &&
            fncd != TFN_TCP_RCV_BUF && fncd != TFN_TCP_GET_BUF)
            break;

        cep->sfncd = 0;
        if ((id = cep->stskid) != FALSE){
            cep->stskid = FALSE;
            cep->slen = slen;
            wup_tsk(id);
        }
        else if ((func = cep->callback) != NULL){
            parblk = slen;
            (func)(cep->cepid, fncd, &parblk);
        }
        break;

    case CEP_TERMINATED:
        cep->seq = ntohl(pkt->tcp.ack);

        seq_check = (W)(cep->sputp - (cep->seq - 1 - cep->seq0));
        if ( seq_check> 0) /* Window Buffer not empty */
            break;

        if (cep->sputp - (cep->seq - 1 - cep->seq0) == 0) { /* ACK of FIN */
            cep->sflag &= ~FIN_FLAG;
            cep->time = 0;      /* Mod 13Mar2003 */
        }
        break;
    case CEP_CLOSING:
        cep->seq = ntohl(pkt->tcp.ack);
        fncd = cep->rfncd;
        if (len != 0) { /* Already closed */
            tcp_cep_unused(cep);
            goto L1;
        }

        seq_check = (W)(cep->sputp - (cep->seq - 1 - cep->seq0));
        if ( seq_check > 0)   /* Window Buffer not empty */
            break;

        if (cep->sputp - (cep->seq - 1 - cep->seq0) == 0) {   /* ACK of FIN */
            cep->sflag &= ~FIN_FLAG;
            cep->time = 0;      /* Mod 13Mar2003 */
        }
        else
            break;

        /* Modified On Nov 7, 2001 PK */
        if (!(cep->rflag & FIN_FLAG))
            break;

        tcp_cep_unused(cep);
        if (fncd != TFN_TCP_CLS_CEP)
            break;
    L1:
        cep->rfncd = 0;
        if ((id = cep->rtskid) != FALSE){
            cep->rtskid = FALSE;
            wup_tsk(id);
        }
        else if ((func = cep->callback) != NULL){
            parblk = E_OK;
            (func)(cep->cepid, fncd, &parblk);
        }
        break;
    }
}

/************************************************/
/* SYN Reception                                */
/************************************************/

void tcp_rcv_syn(T_TCP_PKT *pkt, T_TCP_CEP *cep)
{
    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk;

    switch (cep->state){
    case CEP_WAIT_P_OPEN:
        cep->sflag |= SYN_FLAG|ACK_FLAG;
        cep->syn = ntohl(pkt->tcp.seq);
        cep->ack = ntohl(pkt->tcp.seq) + 1;
        cep->dstipa = ntohl(pkt->ip.sa);
        cep->dstpor = ntohs(pkt->tcp.sp);
        fncd = cep->rfncd;
        if (fncd != TFN_TCP_ACP_CEP)
            break;
        ((T_IPV4EP *)(cep->rpar))->ipaddr = ntohl(pkt->ip.sa);
        ((T_IPV4EP *)(cep->rpar))->portno = ntohs(pkt->tcp.sp);
        break;

    case CEP_WAIT_A_OPEN:
        cep->sflag |= ACK_FLAG;
        cep->syn = ntohl(pkt->tcp.seq);
        cep->ack = ntohl(pkt->tcp.seq) + 1;
        if (cep->sflag & SYN_FLAG)
            break;
        fncd = cep->rfncd;
        if (fncd != TFN_TCP_CON_CEP)
            break;

        SNMP_UPDTCP(SNMP_CSTS_DELETETCB, cep->ipaddr, cep->portno, 0, 0);
        SNMP_UPDTCP(SNMP_CSTS_ESTABLISHED, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);
        cep->state = CEP_CONNECTING;
        cep->rfncd = 0;
        if ((id = cep->rtskid) != FALSE){
            cep->rtskid = FALSE;
            wup_tsk(id);
        }
        else if ((func = cep->callback) != NULL){
            parblk = E_OK;
            (func)(cep->cepid, fncd, &parblk);
        }
        break;

    default:
        cep->sflag |= ACK_FLAG;
        cep->syn = ntohl(pkt->tcp.seq);
        cep->ack = ntohl(pkt->tcp.seq) + 1;
        break;
    }
}

/************************************************/
/* Data Reception                               */
/************************************************/

static void tcp_rcv_data(T_TCP_PKT *pkt, T_TCP_CEP *cep)
{
    INT i, len;
    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk;
    W seq_check;

    if (cep->state != CEP_CONNECTING && cep->state != CEP_TERMINATED) {
        cep->sflag = (ACK_FLAG|RST_FLAG);
        return;
    }

    if (cep->dstipa == 0)
        return;

    i = (ntohs(pkt->tcp.flag) >> 12) * 4;
    len = ntohs(pkt->ip.tl) - (IP_HEADER_SZ + i);
    i -= TCP_HEADER_SZ;

    len = tcp_put_rdat(pkt, cep, pkt->data + i, len);
    if (len < 0) {
        /* Add  YS-010914-003 */
        cep->ack = (cep->rputp + (cep->syn + 1));
        cep->sflag |= ACK_FLAG;
        cep->dackflag |= TCP_ACK_OUTD;
        return;
    }

    /* 10Feb2003 - ACK for every second segment */
    if (cep->dackflag & TCP_ACK_DELAY)
        cep->dackflag |= TCP_ACK_NOW;
    else
        cep->dackflag |= TCP_ACK_DELAY;

    cep->ack = ntohl(pkt->tcp.seq) + len;
    cep->sflag |= ACK_FLAG;

    fncd = cep->rfncd;

    seq_check = (W)(cep->rputp - cep->rgetp);
    if ( seq_check <= 0)
        return;

    if ((id = cep->rtskid) != FALSE) {
        cep->rfncd = 0;
        cep->rtskid = FALSE;
        cep->rlen = len;
        wup_tsk(id);
    }
    else if ((func = cep->callback) != NULL) {
        /* When using callback, data copy to user buffer here. */
        if (fncd == TFN_TCP_RCV_DAT)
            len = tcp_get_rdat((UB *)cep->rpar, cep, cep->rlen);
        else if (fncd == TFN_TCP_RCV_BUF)
            len = tcp_get_rbuf((UB **)cep->rpar, cep);
        else
            return;
        if (len != 0) {
            cep->rfncd = 0;
            parblk = len;
            (func)(cep->cepid, fncd, &parblk);
            /* Add  YS-010914-001 */
            if (cep->sflag & (FIN_FLAG)){
                cep->state = CEP_CLOSING;
                cep->rfncd = TFN_TCP_CLS_CEP;
            }
        }
    }
}

/************************************************/
/* FIN Reception                                */
/************************************************/

void tcp_rcv_fin(T_TCP_PKT *pkt, T_TCP_CEP *cep, INT len)
{
    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk;
    W pos;
    W seq_check;
#ifdef SOCK
    int ctr;
#endif

#ifdef SOCK /* Check for read or write half close */
    for (ctr=0; ctr < MAX_SELECT_INFO; ctr++) {
        if (sel_info[ctr].cepid == cep->cepid) {
            if (sel_info[ctr].taskid > 0)
                wup_tsk(sel_info[ctr].taskid);
        }
    }
#endif

    /* 26Jan2003 AK - Ignore invalid FIN */
    if (cep->state == CEP_WAIT_A_OPEN)
        return;

    /* Modified On Nov 7, 2001 PK */  
    if ((cep->rflag & FIN_FLAG) == FIN_FLAG)
        return;

    if (cep->dstipa == 0) {
        cep->sflag = (ACK_FLAG|RST_FLAG);
        return;
    }

    pos = ntohl(pkt->tcp.seq) - (cep->syn + 1) + len;
    if ((UW)pos != cep->rputp) {
        /* Send ack for FIN with invalid seq (data miss) */
        cep->sflag |= ACK_FLAG;
        return;
    }

    switch (cep->state) {
    case CEP_CONNECTING:
    case CEP_TERMINATED:
        cep->rflag |= FIN_FLAG;
        cep->sflag |= ACK_FLAG;
        cep->ack = pos + cep->syn + 2;

        seq_check = (W)(cep->rputp - cep->rgetp);
        if ( seq_check > 0)   /* Window Buffer not empty */
            break;

        fncd = cep->rfncd;
        if (fncd == TFN_TCP_RCV_DAT || fncd == TFN_TCP_RCV_BUF)
            goto L1;
        break;

    case CEP_CLOSING:
        cep->rflag |= FIN_FLAG;
        cep->sflag |= ACK_FLAG;
        cep->ack = pos + cep->syn + 2;
        tcp_cep_unused(cep);
        fncd = cep->rfncd;
        if (fncd != TFN_TCP_CLS_CEP)
            break;
    L1:
        cep->rfncd = 0;
        if ((id = cep->rtskid) != FALSE) {
            cep->rtskid = FALSE;
            cep->rlen = 0;
            wup_tsk(id);
        }
        else if ((func = cep->callback) != NULL) {
            parblk = 0;
            (func)(cep->cepid, fncd, &parblk);
        }
        break;

    case CEP_UNUSED:
        cep->rflag |= FIN_FLAG;
        cep->sflag |= ACK_FLAG;
        cep->ack = pos + cep->syn + 2;
        break;

    default:
        cep->sflag = (ACK_FLAG|RST_FLAG);
        break;
    }
}

/************************************************/
/* RST Reception                                */
/************************************************/

void tcp_rcv_rst(T_TCP_CEP *cep)
{
    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk;
#ifdef SOCK
    int ctr;
#endif

    if (cep->dstipa == 0)
        return;

#ifdef SOCK /* Check for read or write half close */
    for (ctr=0; ctr < MAX_SELECT_INFO; ctr++) {
        if (sel_info[ctr].cepid == cep->cepid) {
            if (sel_info[ctr].taskid > 0)
                wup_tsk(sel_info[ctr].taskid);
        }
    }
#endif

    id = 0;
    if (cep->dcls_id > 0)
        id = cep->dcls_id;

    switch (cep->state) {
    case CEP_WAIT_P_OPEN:
        tcp_cep_unused(cep);
        cep->sflag = 0;
        fncd = cep->rfncd;
        if (fncd != TFN_TCP_ACP_CEP)
            break;
        goto L1;

    case CEP_WAIT_A_OPEN:
        tcp_cep_unused(cep);
        cep->sflag = 0;
        fncd = cep->rfncd;
        if (fncd != TFN_TCP_CON_CEP)
            break;
    L1:
        if (id > 0)   /* If pending send of FIN */
            rel_wai(cep->dcls_id);
        cep->rfncd = 0;
        cep->sfncd = 0;
        if (((id = cep->rtskid) != FALSE) || ((id = cep->stskid) != FALSE)){
            cep->rtskid = FALSE;
            cep->stskid = FALSE;
            wup_tsk(id);
            break;
        }
        if (fncd != TFN_TCP_SND_DAT && fncd != TFN_TCP_RCV_DAT  &&
            fncd != TFN_TCP_RCV_BUF && fncd != TFN_TCP_GET_BUF  &&
            fncd != TFN_TCP_CLS_CEP && fncd != TFN_TCP_CON_CEP  &&
            fncd != TFN_TCP_ACP_CEP)
            break;

        else if ((func = cep->callback) != NULL){
            parblk = E_CLS;
            (func)(cep->cepid, fncd, &parblk);
        }
        break;

    case CEP_CLOSING:
        fncd = cep->rfncd;
        cep->sflag = 0;
        /* 03/09/2002 A.K. - Change State */
        tcp_cep_unused(cep);
        SNMP_BYPCNT(snmp_mibbyp.tcpEstabResets, 1);  /* tcpEstabResets */
        goto L1;

    default:
        if (cep->sfncd != 0)
            fncd = cep->sfncd;
        else
            fncd = cep->rfncd;
        cep->state = CEP_DISCONNECTED;
        cep->rflag = RST_FLAG;
        cep->sflag = 0;
        cep->rlen = E_CLS;
        goto L1;
    }
}


/************************************************/
/* ACK Transmit                                 */
/************************************************/
TMO tcp_transmit_ack(TMO t)
{
    T_TCP_CEP *cep;
    int i;
    TMO tt;
    ER ercd;

    for (i = 0; i < TCP_CEPID_MAX; i++){
        cep = &TCP_CEP[i];
        if (IS_USED(cep->state) && cep->dstipa != 0    /* 端点使用中 */
         && cep->ackpkt != NULL) {  /* 遅延ACK保留中 */
            tt = (TMO)(cep->dtime - SYSCK.ltime);
            if (tt <= 0) { /* 遅延時間満了 */
                cep->ackpkt->tcp.win = rwin_size(cep);
                cep->ackpkt->tcp.win = htons(cep->ackpkt->tcp.win);
                cep->ackpkt->tcp.ack = htonl(cep->ack);
                cep->ackpkt->tcp.cs = tcp_checksum(cep->ackpkt);
                cep->dtime = 0;
                cep->dackflag = 0;
                SNMP_BYPCNT(snmp_mibbyp.tcpOutSegs, 1);      /* tcpOutSegs */
                ercd = snd_msg(ID_IP_SND_MBX, cep->ackpkt);
                if (ercd != E_OK){
                    tcp_internal_error(ercd);
                    rel_blf(ID_ETH_MPF, cep->ackpkt);
                }
                cep->ackpkt = NULL;
            }
            else {
                if (tt < t)
                    t = tt;
                break;
            }
        }
    }
    return t;

}

/************************************************/
/* Flow Control Sub Routine                     */
/************************************************/
BOOL tcp_flw_ctl(T_TCP_CEP *cep)
{
    BOOL ret=FALSE;
    UW cwnd, oldsgetp;
    W seq_check;

    /* Mod  YS-010914-002 */
    if ((cep->sgetp - (cep->seq - cep->seq0)) == 0) {
        if (!cep->dackmd) {
            cep->time = 0;
            cep->retry.retries = 0;
        }
    }
    
    if (cep->state == CEP_CONNECTING) {
        /* Window update ACK */
        if (cep->seq1  == cep->seq && cep->win != cep->win1 && 
            cep->sgetp != 0 && cep->win != 0) {
                cep->win1 = cep->win;
                return TRUE;
        }

        /* Mod 13Mar2003 - NewReno implementation to process */
        /*                  duplicate ACK                    */
        if (cep->seq1  == cep->seq && cep->win == cep->win1 && 
            cep->sgetp != 0 && cep->win != 0 && cep->rputp == cep->rgetp) {

            if (cep->time != 0)
                cep->dackcnt++;

            switch (cep->dackcnt) {
            case 0:
                 return TRUE;
            case 1:
                 return TRUE;
            case 2: /* Received duplicate ACK 3 times */
                 /* Dont Fast retransmit if already retransmitted */

                seq_check = (W)(cep->seq - cep->hseq);
                 if (cep->hseq != 0 && seq_check <= 0)
                    return -1;

                 cep->hseq = cep->mseq;
                 cep->sgetpf = cep->sgetp;
                 cep->dackmd = TRUE;
                 /* ssth = MAX(FlightSize/2, 2*MSS) */
                 cwnd = (UW)((cep->seq0 + cep->sgetp) - cep->seq1);
                 cwnd = cwnd >> 1;
                 cwnd = (cwnd / cep->mss) * cep->mss;
                 cep->ssth = MAX(cwnd, cep->mss + cep->mss);
                 cep->cwnd = cep->ssth + 3 * cep->mss;
                 cep->sgetp = cep->seq - cep->seq0;
                 return TRUE;
            default:    /* Fast Recovery */
                 cep->cwnd += cep->mss;
                 return TRUE;
            }
        }
        /* Receive ACK of retransmit packet */
        else if (cep->dackmd) {

            seq_check = (W)(cep->sgetp - cep->sgetpf);
            if (seq_check >= 0) {
                cep->dackmd = FALSE; /* Ends Fast recovery */
                if (cep->dackcnt > 3 && (cep->cwnd > cep->ssth))
                    cep->cwnd = cep->ssth;
                cep->dackcnt = 0;
            }
            else {
                 cwnd = (UW)((cep->seq0 + cep->sgetpf) - cep->seq);
                 cwnd = (cwnd / cep->mss) * cep->mss;
                 if (cwnd != 0)
                     cep->cwnd = MIN(cwnd, cep->mss + cep->mss);
                 ret = TRUE;
            }
            cep->sgetp = cep->seq - cep->seq0;
        }
        else {  /* Normal ACK */

            cep->dackcnt = 0;
            /* Reset sgetp after retransmission */
            if (cep->retry.retries > 0) {
                oldsgetp = cep->seq - cep->seq0;
                if (oldsgetp > cep->sgetp)
                    cep->sgetp = oldsgetp;
            }
            /* 4Mar2003 - Change updation of cwnd */
            if (cep->cwnd < cep->ssth) {
                /* Slow start */
                seq_check = (W)(cep->seq - cep->seq1);
                if (cep->seq != cep->seq1 && cep->seq1 != 0 && seq_check > 0)
                    cep->cwnd += (UW)(cep->seq - cep->seq1);
            }
            else {
                /* Avoid congestion cwnd += 2*MSS/cwnd */
                cwnd = cep->mss * cep->mss;
                if (cep->cwnd > cwnd)
                    cep->cwnd += (cwnd / cep->cwnd);
            }
       }

    }
    if (cep->cwnd > 0xFFFF)
        cep->cwnd = 0xFFFF;
    cep->seq1 = cep->seq;
    cep->win1 = cep->win;
    return ret;
}

/************************************************/
/* Confirms it can be delay ACK                 */
/************************************************/

BOOL can_ack_delay(W size, T_TCP_CEP *cep, T_TCP_PKT *pkt)
{
    if (size != 0)
        return FALSE;
    if ((pkt->tcp.flag & 0x3F) != ACK_FLAG)
        return FALSE;
    if ((cep->rflag & (FIN_FLAG|SYN_FLAG)) != 0)
        return FALSE;
    if ((cep->sflag & (FIN_FLAG|SYN_FLAG)) != 0)
        return FALSE;
    if (cep->sgetp == 0 && cep->sputp == 0 &&
        cep->rgetp == 0 && cep->rputp == 0)
        return FALSE;
    if (cep->win == 0)
        return FALSE;
    if (cep->dackflag & TCP_ACK_OUTD) {
        cep->dackflag &= ~TCP_ACK_OUTD;
        return FALSE;
    }
    return TRUE;
}

/************************************************/
/* Validation of incoming TCP packets           */
/************************************************/

static ER validate_tcp_rpkt(T_TCP_CEP *cep, T_TCP_PKT *pkt, int len)
{

    UW ack, seq;
    int flag;
    ER ercd;
    UH win;
    B check;
    W seq_check;

    ack = seq = 0;
    check = 0;

    flag = ntohs(pkt->tcp.flag) & 0x3f;

    switch (cep->state)
    {
        case CEP_WAIT_P_OPEN:
                /* if SYN already received */
                if (cep->syn != 0)
                    goto TCPSEQVALID;
                /* Waiting for SYN */
                ack = ntohl(pkt->tcp.ack);
                if (flag & RST_FLAG || flag & ACK_FLAG || ack != 0) {
                    check = 1;
                    goto TCPSEQERR;
                }
                /* SYN reception */
                if (flag & SYN_FLAG)
                    break;

                return -1;

        case CEP_WAIT_A_OPEN:
                if (flag == SYN_FLAG && ntohl(pkt->tcp.ack) == 0)
                    break;
                if (cep->syn == 0) {
                    check = 1;
                    goto TCPACKVALID;
                }
        default:

TCPSEQVALID:
                win  = cep->rbufsz - (cep->rputp - cep->rgetp);
                seq = ntohl(pkt->tcp.seq);

                if (cep->ack == seq + 1) {
                    if ((cep->itime = SYSCK.ltime) == 0)
                        cep->itime = (UW)-1;
                        cep->ktime  =   TCP_KTIME_INI;
                }

                if (tcp_seq_valid(cep, seq, len, win) == FALSE) {
                    if (flag & RST_FLAG) {
                        if (cep->rflag & FIN_FLAG && seq == cep->ack - 1)
                            break;
                    }
                    goto TCPSEQERR;
                }

                if (flag & RST_FLAG)
                    break;

TCPACKVALID:
                ack = ntohl(pkt->tcp.ack);
                if (ack == 0) 
                    break;

                seq_check = (W)(ack - cep->seq);
                if (seq_check >= 0) {
                     seq_check = (W)(ack - cep->mseq);
                     if (seq_check <= 0)
                        break;
                }

                seq_check = (W)(ack - cep->seq);
                if (cep->state == CEP_WAIT_A_OPEN || cep->state == CEP_WAIT_P_OPEN) {
                    check = 1;
                }
                else if (seq_check <= 0)  /* duplicate ACK */
                    break;
                else
                    return -1;                        /* ignore pkt */

TCPSEQERR:
                /* Invalid pkt received */
                if (flag & RST_FLAG) {
                    /* Ignore invalid RST pkt */
                    return -1;
                }

                if (check == 0) 
                    make_tcp_ack_pkt(pkt, cep);
                else 
                    make_tcp_rst_pkt(pkt, cep);

                pkt->ctl.hedflg = FALSE;
                pkt->ctl.arprty = 0;
                SNMP_BYPCNT(snmp_mibbyp.tcpOutSegs, 1);      /* tcpOutSegs */
                ercd = snd_msg(ID_IP_SND_MBX, pkt);
                if (ercd != E_OK) {
                    tcp_internal_error(ercd);
                    return -1;
                }
                return 0;
    }

    return 1;

}
/************************************************/
/* TCP Reception Process                        */
/************************************************/

VP tcp_reception(T_TCP_PKT *pkt)
{
    T_TCP_CEP *cep;
    int flag, len, i;
    BOOL n=FALSE;
    ER ercd=E_OK;
    UW _win;
    UW rtime, _ack, _seq;
    W size;
    W seq_check;

    _ack = _seq = 0;

    SNMP_BYPCNT(snmp_mibbyp.tcpInSegs, 1);  /* tcpInSegs */

#ifndef PPP
    if (!lan_read_pkt(&pkt->tcp, TCP_HEADER_SZ))
        return pkt;
#else
    if (!ppp_read_pkt(&pkt->tcp, TCP_HEADER_SZ))
        return pkt;
#endif

    cep = search_tcp_cep(ntohl(pkt->ip.da), ntohs(pkt->tcp.dp),
          ntohl(pkt->ip.sa), ntohs(pkt->tcp.sp));
    if (cep == NULL || cep->state == CEP_DISCONNECTED) {
#ifndef PPP
        lan_ignore_pkt();
#endif
        if (ntohs(pkt->tcp.flag) & RST_FLAG)
            return pkt;
        make_tcp_rst_pkt(pkt, cep);
        pkt->ctl.hedflg = FALSE;
        pkt->ctl.arprty = 0;
        ercd = snd_msg(ID_IP_SND_MBX, pkt);
        if (ercd != E_OK)
        {   tcp_internal_error(ercd);
            return pkt;
        }
        return NULL;
    }

    len = ntohs(pkt->ip.tl) - (IP_HEADER_SZ + TCP_HEADER_SZ);
#ifndef PPP
    if (!lan_read_pkt_end(pkt->data, len, TCP_MTU)) {
        SNMP_BYPCNT(snmp_mibbyp.tcpInErrs, 1);  /* tcpInErrs */
        return pkt;
    }
#else
    if(len != 0){
        if (!ppp_read_pkt_end(pkt->data, len, TCP_MTU)) {
           SNMP_BYPCNT(snmp_mibbyp.tcpInErrs, 1);  /* tcpInErrs */
           return pkt;
        }
    }
#endif

    flag = ntohs(pkt->tcp.flag) & 0x3f;

    if ((ntohs(pkt->tcp.flag) & 0xf000) == 0x5000) /* without option */
        i = 0;
    else {
        if (flag & SYN_FLAG) {  /* Check for MSS option */
            if (pkt->data[0] == 2 && pkt->data[1] == 4) {
                cep->mss = ((pkt->data[2] << 8) | pkt->data[3]);
                cep->mss = cep->mss > TCP_MTU ? TCP_MTU : cep->mss;
                if (cep->mss < 48) cep->mss = 48; /* Min MSS */
                cep->cwnd = cep->mss * 2;         /* Init cwnd, ssth value */
                cep->smax = cep->cwnd;
            }
        }
        i = (ntohs(pkt->tcp.flag) >> 12) * 4;
        len -= i - TCP_HEADER_SZ;
    }

    if (tcp_checksum(pkt) != pkt->tcp.cs) {
        SNMP_BYPCNT(snmp_mibbyp.tcpInErrs, 1);  /* tcpInErrs */
        tcp_internal_error(ercd);
        return pkt;
    }

    /* Discard if not a valid pkt */
    ercd = validate_tcp_rpkt(cep, pkt, len);
    if (ercd == -1) return pkt;
    if (ercd == 0) return NULL;
    
    _seq = ntohl(pkt->tcp.ack);
    /* Measure the RTT value and calculate new RTO for  */
    /* an ACK of non retransmitted TCP segment          */
    if (cep->retry.retries == 0L && cep->dackmd == FALSE && cep->hseq == 0) {
        if (cep->retry.seq != 0) {

            seq_check = (W)(_seq - cep->retry.seq);
            if (seq_check > 0 && cep->retry.status == TCP_RET_ON) {
                rtime = (UW)(SYSCK.ltime - cep->retry.rtime);
                tcp_rtt_cal(cep, rtime);
                if (cep->state == CEP_WAIT_P_OPEN || cep->state == CEP_WAIT_A_OPEN) {
                    if (cep->retry.rto < tTCP_RTO_INI)
                        cep->retry.rto = tTCP_RTO_INI;
                }
                cep->retry.seq = 0;
                cep->retry.status = TCP_RET_OFF;
            }
        }
    }
    else {
        /* ACK after retransmission */
        cep->retry.status = TCP_RET_OFF;
        cep->retry.seq = 0;
    }

    /* 26Jan2003- Ignore data if FIN already recv */
    if (cep->rflag & FIN_FLAG)
        len = 0;

    if ((cep->itime = SYSCK.ltime) == 0)
        cep->itime = (UW)-1;
    cep->ktime  =   TCP_KTIME_INI;

    cep->win = ntohs(pkt->tcp.win);

    /* Discard ACK of zero window probe */
    if (cep->win == 0 && flag == ACK_FLAG && len == 0) {
            if (cep->win1 == 0) {
                cep->retry.retries = 0;
                return pkt;
            }
            flag = 0;
            cep->dackflag |= TCP_ZERO_PROBE;
            cep->sflag |= ACK_FLAG;
            cep->win1 = 0;
    }
    else
        cep->retry.flag = 0;

    if (flag & ACK_FLAG) tcp_rcv_ack(pkt, cep, len);
    if (flag & SYN_FLAG) tcp_rcv_syn(pkt, cep);
    if (len != 0)        tcp_rcv_data(pkt, cep);
    if (flag & FIN_FLAG) tcp_rcv_fin(pkt, cep, len);
    if (flag & RST_FLAG) tcp_rcv_rst(cep);

    if (flag & ACK_FLAG) {

        n = tcp_flw_ctl(cep);

        seq_check = (W)(cep->seq1 - cep->hseq);
        if (cep->hseq != 0 && seq_check > 0) {
            cep->hseq = 0;
            cep->retry.retries = 0;
            cep->retry.status = TCP_RET_OFF;
        }

        if (n == -1) return pkt;
    }

    if (cep->dstipa == 0 && !(cep->sflag & RST_FLAG))
        return pkt;

    _win = ntohs(pkt->tcp.win);
    _ack = ntohl(pkt->tcp.ack);
    _seq = (cep->seq0 + cep->sgetp);
    if (_ack != 0 && _win != 0 && _seq != 0)
        cep->win = _win = (_win - (UH)(_seq - _ack));
    else
        cep->win = _win;
    
    _win = cep->cwnd;
    _win = (cep->cwnd / cep->mss) * cep->mss;
      cep->smax = MIN(_win, cep->win);
      for (;;) {
        if ((size = make_tcp_pkt(pkt, cep, FALSE)) <= -1)
            break;
        pkt->ctl.arprty = 0;
      #ifndef PPP
        if (can_ack_delay(size, cep, pkt)) {
            /* Release any existing ACK packet */
            if (cep->ackpkt != NULL) /* 送信前の場合はメモリ開放 */
                ercd = rel_blf(cep->ackpkt->ctl.mpfid, cep->ackpkt);
            cep->ackpkt = pkt; /* パケットの保存 */
            cep->dtime = DACK_TIME + SYSCK.ltime;
            /* Release the Transmitting task */
            ercd = rel_wai(ID_IP_SND_TSK);
            if (ercd != E_OK)
                tcp_internal_error(ercd);
            return NULL;
        }
      #endif

        if ((cep->time = SYSCK.ltime) == 0)
            cep->time = (UW)-1;

        if (cep->ackpkt != NULL) {
            ercd = rel_blf(cep->ackpkt->ctl.mpfid, cep->ackpkt);
            cep->ackpkt = NULL;
        }

        /* Initialize the retransmission variables      */
        /* retry count for SYN should be 3              */
        /* retry count for DATA should be 12            */
        cep->retry.retries = 0;
        if(cep->state == CEP_WAIT_P_OPEN || cep->state == CEP_WAIT_A_OPEN)
            cep->retry.retry_cnt = (UW)TCP_SYN_RETRY;
        else
            cep->retry.retry_cnt = (UW)TCP_DATA_RETRY;

        /* Dont try retransmit ACK segments */
        if (cep->retry.flag == ACK_FLAG && (cep->sgetp - (cep->seq - cep->seq0)) == 0 \
            && cep->win != 0) {
            cep->time = 0;
            cep->retry.retries = 0;
            cep->retry.status = TCP_RET_OFF;
        }
        else {  /* set rtt timer */
            if (((cep->retry.flag & PSH_FLAG) || (cep->retry.flag & SYN_FLAG)) \
            && cep->retry.status == TCP_RET_OFF && cep->hseq == 0) {
                cep->retry.seq = ntohl(pkt->tcp.seq);
                cep->retry.status = TCP_RET_ON;
                cep->retry.rtime = cep->time;
            }
        }

        SNMP_BYPCNT(snmp_mibbyp.tcpOutSegs, 1);      /* tcpOutSegs */
        ercd = snd_msg(ID_IP_SND_MBX, pkt);
        if (ercd != E_OK) {
            tcp_internal_error(ercd);
            return pkt;
        }
        /* Added AK - Initialize keep alive timer whenever  */
        /*             a packet to be transmitted.          */
        if ((cep->itime = SYSCK.ltime) == 0)
            cep->itime = (UW)-1;
        cep->ktime  =   TCP_KTIME_INI;  /* Initial keep alive timeout */
        
        pkt = NULL;
        if (size <= 0 || n == TRUE)
            break;
        ercd = pget_blf((VP *)&pkt, ID_ETH_MPF);
        if (ercd != E_OK)
            break;
        memset(&pkt->ctl, 0, sizeof (T_CTL_HEADER));
        pkt->ctl.mpfid = (H)ID_ETH_MPF;
        pkt->ctl.cep = cep;
        pkt->tcp.flag = 0;

    }

    return pkt;
}

/************************************************/
/* TCP Transmition Process                      */
/************************************************/

void tcp_transmit_sub(T_TCP_CEP *cep, BOOL retry)
{
    T_TCP_PKT *pkt;
    ER ercd;
    W size; 
    
    for (;;) {
        ercd = pget_blf((VP *)&pkt, ID_ETH_MPF);
        if (ercd != E_OK)
            return;

        memset(&pkt->ctl, 0, sizeof (T_CTL_HEADER));
        pkt->ctl.mpfid = (H)ID_ETH_MPF;
        pkt->ctl.cep = cep;
        pkt->tcp.flag = 0;

        /* Added AK - send keep alive probe whenever */
        /*            Keep alive timeout occurs.     */
        if (cep->ktime > TCP_KTIME_INI) {
            pkt->eth.etype = htons(ETYPE_IP);
            pkt->ip.prot = PROT_TCP;
            pkt->ip.da   = htonl(cep->dstipa);
            pkt->ip.sa   = htonl(cep->ipaddr);
            pkt->tcp.sp  = htons(cep->portno);
            pkt->tcp.dp  = htons(cep->dstpor);
            pkt->tcp.seq = htonl(cep->seq0 + cep->sgetp - 1); /* Seq for keep alive probe */
            pkt->tcp.ack = htonl(cep->ack);
            pkt->tcp.flag =
                htons((UH)(((sizeof (T_TCP_HEADER) / 4) << 12) | (ACK_FLAG)));
            pkt->tcp.win = rwin_size(cep);
            pkt->tcp.win = htons(pkt->tcp.win);
            pkt->tcp.urgp = 0;
            pkt->ip.tl = htons((UH)(IP_HEADER_SZ + TCP_HEADER_SZ));
            pkt->tcp.cs = tcp_checksum(pkt);
            memset(pkt->eth.da, 0, 6);
        }      
        /* Add 2002/02/18 */
        else if (cep->sflag & RST_FLAG) {
            /* Send RST */
            pkt->eth.etype = htons(ETYPE_IP);
            pkt->ip.prot = PROT_TCP;
            pkt->ip.da   = htonl(cep->dstipa);
            pkt->ip.sa   = htonl(cep->ipaddr);
            pkt->tcp.sp  = htons(cep->portno);
            pkt->tcp.dp  = htons(cep->dstpor);
            pkt->tcp.seq = htonl(cep->seq);
            pkt->tcp.ack = htonl(cep->ack);
            pkt->tcp.flag =
            htons((UH)(((sizeof (T_TCP_HEADER) / 4) << 12) | (ACK_FLAG|RST_FLAG)));
            pkt->tcp.win = 0;
            pkt->tcp.urgp = 0;
            pkt->ip.tl = htons((UH)(IP_HEADER_SZ + TCP_HEADER_SZ));
            pkt->tcp.cs = tcp_checksum(pkt);
            memset(pkt->eth.da, 0, 6);
            cep->sflag &= ~(ACK_FLAG|RST_FLAG);
            size = 0;
            SNMP_BYPCNT(snmp_mibbyp.tcpOutRsts, 1);  /* tcpOutRsts */
        }
        else {
            if ((size = make_tcp_pkt(pkt, cep, retry)) <= -1){
                rel_blf(ID_ETH_MPF, pkt);
                if ((cep->seq - cep->seq0) == cep->sgetp)
                    if (cep->win1 > (cep->mss))
                        cep->time = 0;
                return;
            }
        }
        /* Initialize the retransmission variables      */
        /* retry count for SYN should be 3              */
        /* retry count for DATA should be 12            */
        if (!retry) {
            cep->retry.retries = 0;
            if(cep->state == CEP_WAIT_P_OPEN || cep->state == CEP_WAIT_A_OPEN)
                cep->retry.retry_cnt = (UW)TCP_SYN_RETRY;
            else
                cep->retry.retry_cnt = (UW)TCP_DATA_RETRY;
        }

        if ((cep->time = SYSCK.ltime) == 0)
            cep->time = (UW)-1;

        /* Dont try retransmit ACK segments */
        if (cep->retry.flag == ACK_FLAG && (cep->sgetp - (cep->seq - cep->seq0)) == 0 \
            && cep->win != 0) {
            cep->time = 0;
            cep->retry.retries = 0;
            cep->retry.status = TCP_RET_OFF;
        }
        else {  /* set rtt timer */
            if (((cep->retry.flag & PSH_FLAG) || (cep->retry.flag & SYN_FLAG)) \
            && cep->retry.status == TCP_RET_OFF && cep->hseq == 0) {
                cep->retry.seq = ntohl(pkt->tcp.seq);
                cep->retry.status = TCP_RET_ON;
                cep->retry.rtime = cep->time;
            }
        }

        /* 5Mar2003 - Clear existing delayed pkt */
        if (cep->ackpkt != NULL) {
            ercd = rel_blf(cep->ackpkt->ctl.mpfid, cep->ackpkt);
            cep->ackpkt = NULL;
        }

        SNMP_BYPCNT(snmp_mibbyp.tcpOutSegs, 1);      /* tcpOutSegs */
        pkt->ctl.hedflg = FALSE;
        pkt->ctl.arprty = 0;
        ercd = snd_msg(ID_IP_SND_MBX, pkt);
        if (ercd != E_OK) {
            tcp_internal_error(ercd);
            rel_blf(ID_ETH_MPF, pkt);
        }
        /* Added AK - Initialize keep alive timer whenever  */
        /*             a packet to be transmitted.          */
        if (cep->ktime <= TCP_KTIME_INI) {
            if ((cep->itime = SYSCK.ltime) == 0)
                cep->itime = (UW)-1;
            cep->ktime  =   TCP_KTIME_INI;
        }

        /* Modified AK - break if buffer is empty */
        if (size <= 0 || retry)
            break;
      }
}

void tcp_transmit(T_TCP_CEP *cep) { tcp_transmit_sub(cep, FALSE); }

/************************************************************************************
* Module Name   : tcp_timeout                                                       *
* Purpose       : This funtion is called whenever retransmission fails after a      *
*                 particular number of retries. This function wakes up the tasks    *
*                 waiting for transmission and change the state of CEP to CLOSED.   *
* Arguments     : T_TCP_CEP *cep - TCP CEP structure                                *
* Return value  : None.                                                             *
* Author        : AK                                                                *
* Date          : 23-Mar-2002                                                       *
************************************************************************************/

void tcp_timeout(T_TCP_CEP *cep) {

    TCP_CALLBACK func;
    ID id;
    FN fncd;
    INT parblk;
        
    if (cep->state == CEP_CONNECTING ||  cep->rflag & FIN_FLAG) {
        SNMP_BYPCNT(snmp_mibbyp.tcpEstabResets, 1);  /* tcpEstabResets */
    }

    remove_queued_packet(ID_IP_SND_MBX, cep);   /* remove packet from mailbox   */
        
    cep->rflag = 0;                             /* Reset receive and send flag  */
    cep->sflag = 0;
    cep->rlen = E_CLS;

    if (cep->dcls_id > 0)                       /* Release task waiting for FIN */
        rel_wai(cep->dcls_id);
            
    tcp_cep_unused(cep);                        /* Close the CEP                */

    if (cep->sfncd != 0)
        fncd = cep->sfncd;
     else
        fncd = cep->rfncd;

    if (((id = cep->rtskid) != FALSE) || ((id = cep->stskid) != FALSE)) {
        cep->rtskid = FALSE;
        cep->stskid = FALSE;
        wup_tsk(id);
    }
    else if ((func = cep->callback) != NULL) {   /* Call the functions if any    */
         parblk = E_CLS;                         /* present                      */
        (func)(cep->cepid, fncd, &parblk);
    }
}

TMO tcp_transmit2(void)
{
    T_TCP_CEP *cep;
    W t, tt;
    int i;

    t = wait(2000);

    for (i=0; i < TCP_CEPID_MAX; i++) {         /* Check all cep's for keep alive timeout */
        cep = &TCP_CEP[i];
        if (TCP_KTIME_INI > 0) {
            if (cep->state > CEP_WAIT_P_OPEN) {     /* If cep is used */
                tt = cep->itime + (cep->ktime * KTIME_PRES) - SYSCK.ltime;
                if(tt <= 0) {                       /* Keep alive time out  */
                    if(cep->state < CEP_CONNECTING) /*  Yet connection not established */
                        tcp_timeout(cep);           /*  Close the socket    */
                    else{   
                        cep->ktime += TCP_KTIME_SUC;    /*  Increase keep alive time value  */
                        if(cep->ktime > (UW)(TCP_KTIME_INI + TCP_KTIME_PRO))
                            tcp_timeout(cep);       /*  Close the socket    */
                        else
                            tcp_transmit_sub(cep, TRUE);    /* Send probe */
                    }
                }
                if (tt < t)                         /* Set remaining timeout */
                    t = tt;
            }
        }   
        if (IS_USED(cep->state) && cep->dstipa != 0 && cep->time != 0) {
            tt = (cep->time + cep->retry.rto) - SYSCK.ltime;
            if (tt > 0) {
                if (t > tt)
                    t = tt;
                continue;
            }        
            /* Added AK - Congestion avoidance algorithm                */
            /* congestion avoidance algorithm implemented here          */
            if (cep->retry.retries == 0L && cep->win != 0) {
                if (!(cep->retry.flag & SYN_FLAG)) {
                    cep->cwnd = (UW)((cep->seq0 + cep->sgetp) - cep->seq1);
                    cep->cwnd = cep->cwnd >> 1;
                    cep->cwnd = (cep->cwnd / cep->mss) * cep->mss;
                    cep->ssth = MAX(cep->cwnd, cep->mss + cep->mss);
                    cep->cwnd = cep->mss;
                }
                cep->dackmd = FALSE;
                cep->dackcnt = 0;
                if (cep->retry.flag & PSH_FLAG)
                    cep->hseq = cep->mseq;
            }

            /* check if retransmission count exceeds total number   */
            /* of retry counts. Then stop the retransmission and    */
            /* close the socket.                                    */
            if (++cep->retry.retries > cep->retry.retry_cnt) {
                cep->retry.srtt = 0;
                tcp_timeout(cep);
            }   
            /* Retransmit the segment and double the RTO value for  */
            /* next retransmission. (Exponential backoff algorithm) */
            else {
                /* 12/Jul/02 AK - remove any pending TCP seg  */
                remove_queued_packet(ID_IP_RETRY_MBX, cep);
                if (cep->retry.flag & SYN_FLAG)
                     cep->sflag = cep->retry.flag;
                if (cep->win == 0 && !(cep->retry.flag & SYN_FLAG))
                    cep->retry.retries = 0;

                tcp_transmit_sub(cep, TRUE);
                cep->retry.rto = cep->retry.rto << 1;
                if (cep->retry.rto > (UW)tTCP_RTO_UBOUND)
                    cep->retry.rto = (UW)tTCP_RTO_UBOUND;
            }
        }
    }
    tcp_iss += (UW)TCP_ISSINC;
    if (tcp_iss >= 0x80000000L)
        tcp_iss &= 0x7fffffffL;

    return t;
}

/************************************************/
/* TCP Cancel CEP Subroutine                   */
/************************************************/
ER tcp_can_cep2(T_TCP_CEP *cep, FN fncd)
{
    TCP_CALLBACK func;
    ER ercd;
    ID id;
    INT parblk;

    ercd = E_OBJ;
    if (((id = cep->rtskid) != FALSE) || ((id = cep->stskid) != FALSE))
        ercd = rel_wai(id);
    else if ((func = cep->callback) != NULL) {
        remove_queued_packet(ID_IP_SND_MBX, cep);
        if (fncd == TFN_TCP_ACP_CEP || fncd == TFN_TCP_CON_CEP || fncd == TFN_TCP_CLS_CEP ||
           (fncd == TFN_TCP_ALL && (cep->rfncd == TFN_TCP_ACP_CEP ||  cep->rfncd == TFN_TCP_CON_CEP ||
           cep->rfncd == TFN_TCP_CLS_CEP)))
            tcp_cep_unused(cep);
        parblk = E_RLWAI;
        ercd = E_OK;
        (func)(cep->cepid, fncd, &parblk);
    }
    return ercd;
}
/* end */
