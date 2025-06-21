/*******************************************************************************
* NORTi Network TCP Module 1/2                                                 *
*                                                                              *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                    *
*  All rights reserved.                                                        *
*                                                                              *
* 05/Mar/1999 Created.                                                         *
* 03/Oct/2000 (Ver. 4.02 Release)                                              *
* 22/Jan/2001 Modified for the transfer of exceeding 4GB.                      *
* 10/Feb/2001 Change tcp_con_cep (get port number)                             *
* 06/Apr/2001 Deleted cep->rpar = p_dstaddr in tcp_con_cep.                    *
* 14/Sep/2001 Modified tcp_cls_cep for TMO_NBLK.                               *
* 04/Apl/2002 Added initialization of cwnd and ssth.                           *
* 23/Mar/2002 Modified tcp_acp_cep and tcp_con_cep for retransmission.         *
* 13/May/2002 Added code for Keep alive timer                                  *
* 24/Jun/2002 Modified tcp_snd_dat                                             *
* 24/Jun/2002 Modified tcp_acp_cep and tcp_con_cep for flow control            *
* 25/Mar/2003 Modified for SNMP                                                *
*******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

UH rwin_size(T_TCP_CEP *cep);
/************************************************/
/* TCP Internal Error                           */
/************************************************/

ER tcp_internal_error(ER ercd)
{
    return tcpip_internal_error(ercd);
}

/************************************************/
/* TCP Dummy Callback                           */
/************************************************/

ER tcp_dummy_cbk(ID cepid, FN fncd, VP parblk)
{
    return E_OK;
}

/************************************************/
/* TCP Closing Common Process                   */
/************************************************/

void tcp_cep_unused(T_TCP_CEP *cep)
{
    cep->state = CEP_UNUSED;
    if (cep->ackpkt != NULL){
        rel_blf(cep->ackpkt->ctl.mpfid, cep->ackpkt);
        cep->ackpkt = NULL;
    }
    cep->dcls_id = 0;
    SNMP_UPDTCP(SNMP_CSTS_DELETETCB, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);
}

/************************************************/
/* Create TCP Reception End Point               */
/************************************************/

ER tcp_cre_rep(ID repid, const T_TCP_CREP *pk_crep)
{
    T_TCP_REP *rep;
    ER ercd=E_OK;
    UW a;
    UH n;
    int i;

    /* Parameter Check */

    repid--;
    if ((UINT)repid >= (UINT)TCP_REPID_MAX)
        return E_ID;
    if (pk_crep->myaddr.portno == TCP_PORTANY)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if REP Already Created */

    rep = &TCP_REP[repid];
    if (rep->portno != 0){
        ercd = E_OBJ;
        goto END;
    }

    /* Error if Port Number Already Used */

    a = pk_crep->myaddr.ipaddr;
    if (a == IPV4_ADDRANY)
        a = byte4_to_long(default_ipaddr);
    n = pk_crep->myaddr.portno;
    for (i = 0; i < TCP_REPID_MAX; i++){
        if (TCP_REP[i].portno == n && TCP_REP[i].ipaddr == a){
            ercd = E_OBJ;
            goto END;
        }
    }

    /* Setup TCP REP Control Block */

    rep = &TCP_REP[repid];
    rep->ipaddr = a;
    rep->portno = n;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/*****************************************************/
/* Create TCP Reception End Point (Auto ID Assigned) */
/*****************************************************/

ER tcp_vcre_rep(const T_TCP_CREP *pk_crep)
{
    T_TCP_REP *rep;
    ER ercd=E_OK;
    UW a;
    UH n;
    ID repid;
    int i;

    /* Parameter Check */

    if (pk_crep->myaddr.portno == TCP_PORTANY)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Serch unused REP */

    for (repid=TCP_REPID_MAX;;){
        if(repid == 0){
            ercd = E_ID;
            goto END;
        }
        rep = &TCP_REP[--repid];
        if(!rep->portno)
            break;
    }

    /* Error if Port Number Already Used */

    a = pk_crep->myaddr.ipaddr;
    if (a == IPV4_ADDRANY)
        a = byte4_to_long(default_ipaddr);
    n = pk_crep->myaddr.portno;
    for (i = 0; i < TCP_REPID_MAX; i++){
        if (TCP_REP[i].portno == n && TCP_REP[i].ipaddr == a){
            ercd = E_OBJ;
            goto END;
        }
    }

    /* Setup TCP REP Control Block */

    rep = &TCP_REP[repid];
    rep->ipaddr = a;
    rep->portno = n;
END:
    tcpip_cretical(FALSE);
    if(ercd != E_OK)
        return ercd;
    else
        return ++repid;
}

/************************************************/
/* Delete TCP Reception End Point               */
/************************************************/

ER tcp_del_rep(ID repid)
{
    T_TCP_REP *rep;
    ER ercd=E_OK;
    T_TCP_CEP *cep=&TCP_CEP[0];
    TCP_CALLBACK func;
    INT parblk;
    int i;

    /* Parameter Check */

    repid--;
    if ((UINT)repid >= (UINT)TCP_REPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Error if REP Not Created */

    rep = &TCP_REP[repid];
    if (rep->portno == 0){
        ercd = E_NOEXS;
        goto END;
    }

    /* Clear TCP REP Control Block */
    rep->ipaddr = 0;
    rep->portno = 0;

    /* Search repid */
    for (i=0; i < TCP_CEPID_MAX; i++){
        cep = &TCP_CEP[i];
        if(cep->repid == repid)
            break;
    }

    /* cancel accept */
    if(i != TCP_CEPID_MAX) {
        if(cep->rfncd == TFN_TCP_ACP_CEP) {
            if (cep->rtskid != FALSE)
                ercd = rel_wai(cep->rtskid);
            else if ((func = cep->callback) != NULL)
            {   parblk = E_DLT;
                cep->rfncd = 0;
                tcp_cep_unused(cep);
                (func)(cep->cepid, TFN_TCP_ACP_CEP, &parblk);
            }
        }
    }

END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Create TCP Communication End Point           */
/************************************************/

ER tcp_cre_cep(ID cepid, const T_TCP_CCEP *pk_ccep)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    if ((pk_ccep->sbuf == (VP)NADR) || (pk_ccep->sbufsz <= 0)
     || (pk_ccep->rbuf == (VP)NADR) || (pk_ccep->rbufsz <= 0))
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (IS_CREATED(cep->state)){
        ercd = E_OBJ;
        goto END;
    }

    /* Setup TCP CEP Control Block */

    cep->state = CEP_UNUSED;
    cep->sbuf = pk_ccep->sbuf;
    cep->sbufsz = pk_ccep->sbufsz;
    cep->rbuf = pk_ccep->rbuf;
    cep->rbufsz = pk_ccep->rbufsz;
    cep->ackpkt = NULL;
    cep->dcls_id = 0;
    /* Add YS-020404-001 */
    cep->cwnd = TCP_MTU;
    cep->ssth = 0xFFFF;

    if (pk_ccep->callback == (FP)NADR || pk_ccep->callback == (FP)NULL)
        cep->callback = NULL;
    else
        cep->callback = (TCP_CALLBACK)pk_ccep->callback;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/*********************************************************/
/* Create TCP Communication End Point (Auto ID Assigned) */
/*********************************************************/

ER tcp_vcre_cep(const T_TCP_CCEP *pk_ccep)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    ID cepid;

    /* Parameter Check */

    if ((pk_ccep->sbuf == (VP)NADR) || (pk_ccep->sbufsz <= 0)
     || (pk_ccep->rbuf == (VP)NADR) || (pk_ccep->rbufsz <= 0))
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* Serch unused CEP */

    for (cepid=TCP_CEPID_MAX;;){
        if(cepid ==0){
            ercd = E_ID;
            goto ERR;
        }
        cep = &TCP_CEP[--cepid];
        if(!IS_CREATED(cep->state))
            break;
    }

    /* Setup TCP CEP Control Block */

    cep->state = CEP_UNUSED;
    cep->sbuf = pk_ccep->sbuf;
    cep->sbufsz = pk_ccep->sbufsz;
    cep->rbuf = pk_ccep->rbuf;
    cep->rbufsz = pk_ccep->rbufsz;
    cep->ackpkt = NULL;
    cep->dcls_id = 0;

    if (pk_ccep->callback == (FP)NADR || pk_ccep->callback == (FP)NULL)
        cep->callback = NULL;
    else
        cep->callback = (TCP_CALLBACK)pk_ccep->callback;

    tcpip_cretical(FALSE);
    return (ER)++cepid;
ERR:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Delete TCP Communication End Point           */
/************************************************/

ER tcp_del_cep(ID cepid)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (IS_USED(cep->state)){
        ercd = E_OBJ;
        goto END;
    }

    /* Clear TCP CEP Control Block */

    cep->state = 0;
    remove_queued_packet(ID_IP_SND_MBX, cep);

    SNMP_UPDTCP(SNMP_CSTS_DELETETCB, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Wait for Connection Request                  */
/************************************************/

ER tcp_acp_cep(ID cepid, ID repid, T_IPV4EP *p_dstaddr, TMO tmout)
{
    T_TCP_CEP *cep;
    T_TCP_REP *rep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    repid--;
    if ((UINT)repid >= (UINT)TCP_REPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    rep = &TCP_REP[repid];
    if (rep->portno == 0){
        ercd = E_NOEXS;
        goto END;
    }
    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (IS_USED(cep->state)){
        ercd = E_OBJ;
        goto END;
    }

    /* Setup TCP CEP Control Block */
    tcp_iss += 0x00001000;
    cep->sflag = 0;
    cep->rflag = 0;
    cep->syn = 0;
    cep->seq0 = cep->seq = tcp_iss & 0x0fffffffL;
    cep->seq1 = 0;
    cep->mseq = 0;
    cep->ack = 0;
    cep->win = 0;
    cep->sgetp = 0;
    cep->sputp = 0;
    cep->rgetp = 0;
    cep->rputp = 0;
    cep->repid = (UB)repid;
    cep->ipaddr = rep->ipaddr;
    cep->portno = rep->portno;
    cep->dstipa = 0;
    cep->dstpor = 0;
    cep->state = CEP_WAIT_P_OPEN;
    cep->time = 0;

    /* Add 20Apr2003 */
    cep->mss = TCP_MTU;

    /* Add YS-020404-002 */
    cep->cwnd = TCP_MTU * 2;
    cep->smax = cep->cwnd;
    cep->ssth = 0xFFFF;

    /* Add AK-29-09-2002 */
    cep->sfncd = 0;
    cep->rfncd = 0;

    /* Add 13Mar2003 - Retransmission Boundries */
    tTCP_RTO_INI = wait(TCP_RTO_INI);
    tTCP_RTO_UBOUND = wait(TCP_RTO_UBOUND);
    tTCP_RTO_LBOUND = wait(TCP_RTO_LBOUND);
    
    /* Initializes the retransmission variables     */
    cep->time = 0;                  /* Retransmission timeout   */
    cep->retry.rto = tTCP_RTO_INI;  /* Initial RTO */
    cep->retry.srtt = 0;            /* Initial srtt value       */
    cep->retry.rttvar = 3;          /* Initial rttvar */
    cep->retry.retries = 0;         /* Reset number of retries  */
    cep->retry.seq = 0;
    cep->retry.status = TCP_RET_OFF;

    cep->dackcnt = 0;               /* Number of Delay ACK received */
    cep->dackmd = FALSE;            /* Receive ACK of retransmit packet */
    cep->sgetpf = 0;                /* Sequence of first delay ACK */
    cep->dackflag = 0;
    cep->hseq = 0L;

    /*  Added AK - Initialize keep alive timeout */
    if ((cep->itime = SYSCK.ltime) == 0)
        cep->itime = (UW)-1;
    cep->ktime  =   TCP_KTIME_INI;  /* Initial keep alive timeout */
    
    SNMP_UPDTCP(SNMP_CSTS_LISTEN, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);

    /* Return if Non-Blocking Call */

    cep->rpar = p_dstaddr;
    cep->rfncd = TFN_TCP_ACP_CEP;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Connection Request */
    cep->rtskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);

    if (ercd == E_OK && cep->state != CEP_CONNECTING)
        ercd = E_CLS;

    if((ercd == E_RLWAI) && (rep->ipaddr == 0))
        ercd = E_DLT;

    cep->rfncd = 0;
    cep->rtskid = FALSE;
    if (ercd != E_OK) {
        SNMP_BYPCNT(snmp_mibbyp.tcpAttemptFails, 1);  /* tcpAttemptFails */
        tcp_cep_unused(cep);
    }
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Request Connection                           */
/************************************************/

ER tcp_con_cep(ID cepid, const T_IPV4EP *p_myaddr, T_IPV4EP *p_dstaddr, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    UW a;
    UH n;
    int i;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (IS_USED(cep->state)){
        ercd = E_OBJ;
        goto END;
    }

    /* Error if Port Number Already Used */

    a = p_myaddr->ipaddr;
    if (a == IPV4_ADDRANY)
        a = byte4_to_long(default_ipaddr);
    n = p_myaddr->portno;
    /* 11/Jul/02 - Modified AK */
    if (n == TCP_PORTANY)
        n = tcp_portany();
    else{
        for (i = 0; i < TCP_REPID_MAX; i++){
            if (TCP_REP[i].portno == n && TCP_REP[i].ipaddr == a){
                ercd = E_OBJ;
                goto END;
            }
        }
        for (i = 0; i < TCP_CEPID_MAX; i++){
            if (TCP_CEP[i].portno == n && TCP_CEP[i].ipaddr == a &&
                IS_USED(TCP_CEP[i].state)){
                ercd = E_OBJ;
                goto END;
            }
        }
    }

    /* Setup TCP CEP Control Block */
    tcp_iss += 0x00001000;
    cep->sflag = SYN_FLAG;
    cep->rflag = 0;
    cep->syn = 0;
    cep->seq0 = cep->seq = tcp_iss & 0x0fffffffL;
    cep->seq1 = 0;
    cep->mseq = 0;
    cep->ack = 0;
    cep->win = 0;
    cep->sgetp = 0;
    cep->sputp = 0;
    cep->rgetp = 0;
    cep->rputp = 0;
    cep->repid = 0;
    cep->ipaddr = a;
    cep->portno = n;
    cep->dstipa = p_dstaddr->ipaddr;
    cep->dstpor = p_dstaddr->portno;
    cep->state = CEP_WAIT_A_OPEN;
    cep->time = 0;

    /* Add 20Apr2003 */
    cep->mss = TCP_MTU;

    /* Add YS-020404-002 */
    cep->cwnd = TCP_MTU * 2;
    cep->smax = cep->cwnd;
    cep->ssth = 0xFFFF;

    /* Add AK-29-09-2002 */
    cep->sfncd = 0;
    cep->rfncd = 0;

    /* Add 13Mar2003 - Retransmission Boundries */
    tTCP_RTO_INI = wait(TCP_RTO_INI);
    tTCP_RTO_UBOUND =   wait(TCP_RTO_UBOUND);
    tTCP_RTO_LBOUND = wait(TCP_RTO_LBOUND);

    /* Initializes the retransmission variables     */
    cep->time = 0;                  /* Retransmission timeout   */
    cep->retry.rto = tTCP_RTO_INI;  /* Initial RTO */
    cep->retry.srtt = 0;            /* Initial srtt value       */
    cep->retry.rttvar = 3;          /* Initial rttvar */
    cep->retry.retries = 0;         /* Reset number of retries  */
    cep->retry.seq = 0;
    cep->retry.status = TCP_RET_OFF;

    cep->dackcnt = 0;               /* Number of Delay ACK received */
    cep->dackmd = FALSE;            /* Receive ACK of retransmit packet */
    cep->sgetpf = 0 ;               /* Sequence of first delay ACK */
    cep->dackflag = 0;
    cep->hseq = 0L;

    /*  Added AK - Initialize keep alive timeout */
    if ((cep->itime = SYSCK.ltime) == 0)
        cep->itime = (UW)-1;
    cep->ktime  =   TCP_KTIME_INI;  /* Initial keep alive timeout */
    
    /* Send SYN */
    tcp_transmit(cep);

    SNMP_BYPCNT(snmp_mibbyp.tcpActiveOpens, 1);  /* tcpActiveOpens */
    SNMP_UPDTCP(SNMP_CSTS_SYNSENT, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);

    /* Return if Non-Blocking Call */
    cep->rfncd = TFN_TCP_CON_CEP;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Connection */
    cep->rtskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);
    cep->rfncd = 0;
    cep->rtskid = FALSE;
    if (ercd != E_OK){
        remove_queued_packet(ID_IP_SND_MBX, cep);
        tcp_cep_unused(cep);
    }
    else if (cep->state != CEP_CONNECTING) {
        ercd = E_CLS;
        SNMP_BYPCNT(snmp_mibbyp.tcpAttemptFails, 1);  /* tcpAttemptFails */
    }
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Terminate Data Transmission                  */
/************************************************/

ER tcp_sht_cep(ID cepid)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (cep->state != CEP_CONNECTING){
        ercd = E_OBJ;
        goto END;
    }

    /* Send FIN */
    cep->sflag |= FIN_FLAG;

    /* Window Buffer Empty */
    if (cep->sputp == (cep->seq - cep->seq0))
        tcp_transmit(cep);
    cep->state = CEP_TERMINATED;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Close TCP Communication End Point            */
/************************************************/

ER tcp_cls_cep(ID cepid, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd, ercdt;

    ercd = E_OK;
    ercdt = E_RLWAI;
    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (cep->state == 0){
        ercd = E_NOEXS;
        goto END;
    }
    if (cep->state == CEP_DISCONNECTED){
        tcp_cep_unused(cep);
        goto END;
    }
    if (cep->state == CEP_WAIT_P_OPEN){
        cep->repid = 0;
        tcp_cep_unused(cep);
        goto END;
    }
    if ((cep->state != CEP_CONNECTING) && (cep->state != CEP_TERMINATED)){
        ercd = E_OBJ;
        goto END;
    }

    /* Send FIN if Connecting */

    if (cep->state == CEP_CONNECTING || /* Add  YS-010914-001 */
       (tmout == TMO_NBLK && cep->state == CEP_TERMINATED)) {

        /* Send RST recv window is zero - AK */
        if (rwin_size(cep) == 0) { 
            ercd = E_RLWAI;
            ercdt = E_OK;
            goto ERR;
        }

        if (cep->sputp == (cep->seq - cep->seq0)) {
            /* Window Buffer Empty */
            cep->sflag |= FIN_FLAG;
            tcp_transmit(cep);
        }
        else if (tmout == TMO_NBLK) {
            cep->sflag |= FIN_FLAG;
            cep->dcls_id = -1;
            ercd = E_WBLK;
            goto END;
        }
        else {
            cep->rfncd = TFN_TCP_CLS_CEP;
            cep->dcls_id = vget_tid();
            tcpip_cretical(FALSE);
            ercd = tslp_tsk(tmout);
            tcpip_cretical(TRUE);
            cep->dcls_id = FALSE;
            if (ercd == E_TMOUT) {
                SNMP_BYPCNT(snmp_mibbyp.tcpEstabResets, 1);  /* tcpEstabResets */
            }
            if (ercd != E_OK)
                 goto ERR;
            if (ercd == E_OK && cep->state != CEP_DISCONNECTED) {
                cep->sflag |= FIN_FLAG;
                tcp_transmit(cep);
            }
            else {
                ercd = E_OBJ;
                /* change state */
                goto ERR; 
            }
        }
    }

    /* Return if FIN has been received already */

    if ((cep->rflag & FIN_FLAG) && (cep->state == CEP_TERMINATED)){
        tcp_cep_unused(cep);
        goto END;
    }
    cep->state = CEP_CLOSING;
    SNMP_UPDTCP(SNMP_CSTS_CLOSING, cep->ipaddr, cep->portno, cep->dstipa, cep->dstpor);

    /* Return if Non-Blocking Call */

    cep->rfncd = TFN_TCP_CLS_CEP;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for FIN Receiving */

    cep->rtskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);

ERR:
    if((ercd == E_TMOUT) || (ercd == E_RLWAI)) {
        cep->sflag = (ACK_FLAG|RST_FLAG);
        tcp_transmit(cep);
        cep->sflag = 0;
    }
    cep->rfncd = 0;
    cep->rtskid = FALSE;
    if (ercd != E_OK)
        remove_queued_packet(ID_IP_SND_MBX, cep);
    tcp_cep_unused(cep);
    if (ercdt == E_OK) ercd = E_OK;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Send TCP Packet                              */
/************************************************/

ER tcp_snd_dat(ID cepid, const VP data, INT len, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    UW sputp;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (cep->state == CEP_DISCONNECTED){
        ercd = E_CLS;
        goto END;
    }
    if (cep->state != CEP_CONNECTING || cep->sfncd != 0){
        ercd = E_OBJ;
        goto END;
    }

    /* Free Size Check and Data Copy */
    ercd = tcp_put_sdat(cep, data, len);
    if (ercd > 0){
        tcp_transmit(cep);
        goto END;
    }
    if (ercd != E_OK)
        goto END;

    cep->spar = (VP)data;
    cep->slen = len;
    sputp = cep->sputp;

    /* Return if Non-Blocking Call */

    cep->sfncd = TFN_TCP_SND_DAT;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Buffer Empty */

    cep->stskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    cep->sfncd = 0;
    tcpip_cretical(TRUE);
    cep->stskid = FALSE;
    if (ercd == E_OK || sputp != cep->sputp)
        ercd = cep->slen;
    else
        remove_queued_packet(ID_IP_SND_MBX, cep);
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Receive TCP Packet                           */
/************************************************/

ER tcp_rcv_dat(ID cepid, VP data, INT len, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    UH win;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (((cep->state != CEP_CONNECTING) && (cep->state != CEP_TERMINATED))
     || (cep->rfncd != 0)){
        ercd = E_OBJ;
        goto END;
    }

    /* Data Copy */

    win = (UH)(cep->rbufsz - (cep->rputp - cep->rgetp));
    ercd = tcp_get_rdat((UB *)data, cep, len);
    if (ercd > 0){
        if (win == 0){
            cep->sflag |= ACK_FLAG;
            tcp_transmit(cep);
        }
        goto END;
    }

    /* End of Data ? */

    if (cep->state == CEP_DISCONNECTED){
        ercd = E_CLS;
        goto END;
    }
    if (cep->rflag & FIN_FLAG)
        goto END;

    /* Return if Non-Blocking Call */

    cep->rpar = data;
    cep->rlen = len;
    cep->rfncd = TFN_TCP_RCV_DAT;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Data */

    cep->rtskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);
    cep->rfncd = 0;
    cep->rtskid = FALSE;

    /* Data Copy */

    len = tcp_get_rdat((UB *)data, cep, len);
    if (ercd == E_OK || (ercd == E_TMOUT && len != 0))
        ercd = len;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Get Sending Buffer                           */
/************************************************/

ER tcp_get_buf(ID cepid, VP *p_buf, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (cep->state == CEP_DISCONNECTED){
        ercd = E_CLS;
        goto END;
    }
    if (cep->state != CEP_CONNECTING || cep->sfncd != 0){
        ercd = E_OBJ;
        goto END;
    }

    /* Get Buffer */

    ercd = tcp_get_sbuf((UB **)p_buf, cep);
    if (ercd > 0)
        goto END;
    if (ercd != E_OK)
        goto END;
    cep->spar = p_buf;
    cep->slen = 0;

    /* Return if Non-Blocking Call */

    cep->sfncd = TFN_TCP_GET_BUF;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Buffer Empty */

    cep->stskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);
    cep->sfncd = 0;
    cep->stskid = FALSE;
    if (ercd == E_OK)
        ercd = cep->slen;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Send Data in Buffer                          */
/************************************************/

ER tcp_snd_buf(ID cepid, INT len)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (cep->state != CEP_CONNECTING || cep->sfncd != 0){
        ercd = E_OBJ;
        goto END;
    }
    if (cep->state == CEP_DISCONNECTED || (cep->rflag & FIN_FLAG)){
        ercd = E_CLS;
        goto END;
    }

    /* Update Buffer Pointers */

    ercd = tcp_put_sbuf(cep, len);
    if (ercd == E_OK)
        tcp_transmit(cep);
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Get Buffer Containing Received Data          */
/************************************************/

ER tcp_rcv_buf(ID cepid, VP *p_buf, TMO tmout)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    INT len;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if (((cep->state != CEP_CONNECTING) && (cep->state != CEP_TERMINATED))
     || (cep->rfncd != 0)){
        ercd = E_OBJ;
        goto END;
    }

    /* Get Buffer */

    ercd = tcp_get_rbuf((UB **)p_buf, cep);
    if (ercd > 0)
        goto END;

    /* End of Data ? */

    if (cep->state == CEP_DISCONNECTED){
        ercd = E_CLS;
        goto END;
    }
    if (cep->rflag & FIN_FLAG)
        goto END;

    /* Return if Non-Blocking Call */

    cep->rpar = p_buf;
    cep->rfncd = TFN_TCP_RCV_BUF;
    if (tmout == TMO_NBLK){
        ercd = E_WBLK;
        goto END;
    }

    /* Wait for Data */

    cep->rtskid = (UB)vget_tid();
    vcan_wup();
    tcpip_cretical(FALSE);
    ercd = tslp_tsk(tmout);
    tcpip_cretical(TRUE);
    cep->rfncd = 0;
    cep->rtskid = FALSE;

    /* Data Copy */

    len = tcp_get_rbuf((UB **)p_buf, cep);
    if (ercd == E_OK || (ercd == E_TMOUT && len != 0))
       ercd = len;
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Release Receiving Buffer                     */
/************************************************/

ER tcp_rel_buf(ID cepid, INT len)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    UH win;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    /* State Check */

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }
    if ((cep->state != CEP_CONNECTING) && (cep->state != CEP_TERMINATED)){
        ercd = E_OBJ;
        goto END;
    }

    /* Release Buffer */

    win = (UH)(cep->rbufsz - (cep->rputp - cep->rgetp));
    ercd = tcp_rel_rbuf(cep, len);
    if (ercd == E_OK){
        if (win == 0){
            cep->sflag |= ACK_FLAG;
            tcp_transmit(cep);
        }
    }
END:
    tcpip_cretical(FALSE);
    return ercd;
}

/************************************************/
/* Send Urgent Data                             */
/************************************************/

ER tcp_snd_oob(ID cepid, const VP data, INT len, TMO tmout)
{
    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    return E_NOSPT;
}

/************************************************/
/* Receive Urgent Data                          */
/************************************************/

ER tcp_rcv_oob(ID cepid, VP data, INT len)
{
    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;
    if (len <= 0)
        return E_PAR;

    return E_NOSPT;
}

/************************************************/
/* Cancel Pending TCP Process                   */
/************************************************/

ER tcp_can_cep(ID cepid, FN fncd)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;
    BOOL flg=FALSE;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        goto END;
    }

    switch(fncd){
    case TFN_TCP_ALL:
    case TFN_TCP_ACP_CEP:
        if(cep->rfncd != TFN_TCP_ACP_CEP)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->rfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->rfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_CON_CEP:
        if(cep->rfncd != TFN_TCP_CON_CEP)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->rfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->rfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_CLS_CEP:
        if(cep->rfncd != TFN_TCP_CLS_CEP)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->rfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->rfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_SND_DAT:
        if(cep->sfncd != TFN_TCP_SND_DAT)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->sfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->sfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_RCV_DAT:
        if(cep->rfncd != TFN_TCP_RCV_DAT)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->rfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->rfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_GET_BUF:
        if(cep->sfncd != TFN_TCP_GET_BUF)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->sfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->sfncd = 0;
        }
        if(fncd != TFN_TCP_ALL)
            break;

    case TFN_TCP_RCV_BUF:
        if(cep->rfncd != TFN_TCP_RCV_BUF)
            ercd = E_OBJ;
        else {
            if((ercd = tcp_can_cep2(cep, cep->rfncd)) != E_OK)
                goto END;
            flg = TRUE;
            cep->rfncd = 0;
        }
        break;

    case TFN_TCP_SND_OOB:
        ercd = E_NOSPT;
        break;

    default:
        ercd = E_PAR;
        break;
    }

    if(flg == TRUE)
       ercd = E_OK;

END:
    tcpip_cretical(FALSE);
    return ercd;
}


/************************************************/
/* Set TCP Communication End Point Options      */
/************************************************/

ER tcp_set_opt(ID cepid, INT optname, const VP optval, INT optlen)
{
    return E_NOSPT;
}

/************************************************/
/* Get TCP Communication End Point Options      */
/************************************************/

ER tcp_get_opt(ID cepid, INT optname, VP optval, INT optlen)
{
    return E_NOSPT;
}

/************************************************/
/* Get TCP Communication End Point Informations */
/************************************************/

ER tcp_cep_status(ID cepid)
{
    T_TCP_CEP *cep;
    ER ercd=E_OK;

    /* Parameter Check */

    cepid--;
    if ((UINT)cepid >= (UINT)TCP_CEPID_MAX)
        return E_ID;

    ercd = tcpip_cretical(TRUE);
    if (ercd != E_OK)
        return ercd;

    cep = &TCP_CEP[cepid];
    if (!IS_CREATED(cep->state)){
        ercd = E_NOEXS;
        return ercd;
    }

    return cep->state;
}

/* end */
