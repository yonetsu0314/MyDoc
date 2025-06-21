/******************************************************************************
* NORTi Network Echo Server Sample                                            *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 26/Dec/1998                                                                 *
* 22/Sep/1999 resource ID auto assign added.                                  *
* 12/Feb/2001 Change Task priority 4 -> 5                                  KA *
* 18/Mar/2003 udpecho_tskÇ©ÇÁtcpip_creticalÇçÌèú                             *
* 28/Apr/2003 udpecho_tsk was modified to use call-back                    YS *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonecho.h"

/* Window Buffer */

#define WINDOW_SIZE 1024
static UH tcpecho_swin[WINDOW_SIZE/2];  /* Sending Window Buffer */
static UH tcpecho_rwin[WINDOW_SIZE/2];  /* Receiving Window Buffer */

/* Creation Information */

#if defined(GAIO) && defined(H8300H)
TASK udpecho_tsk(VP_INT);
TASK tcpecho_tsk(VP_INT);
#else
TASK udpecho_tsk(ID);
TASK tcpecho_tsk(int);
#endif
ER udpecho_cbk(ID, FN, VP);
ER tcpecho_cbk(ID, FN, VP);

const T_CTSK c_udpecho_tsk = { NULL, TA_HLNG, udpecho_tsk, 5, ECHO_SSZ };
const T_CTSK c_tcpecho_tsk = { NULL, TA_HLNG, tcpecho_tsk, 5, ECHO_SSZ };
const T_UDP_CCEP c_udpecho_cep = { 0, { IPV4_ADDRANY, UDP_PORT_ECHO }, (FP)udpecho_cbk };
const T_TCP_CREP c_tcpecho_rep = { 0, { IPV4_ADDRANY, TCP_PORT_ECHO } };
const T_TCP_CCEP c_tcpecho_cep = { 0, tcpecho_swin, sizeof tcpecho_swin, tcpecho_rwin, sizeof tcpecho_rwin, (FP)tcpecho_cbk };

/* Local Data */

#define ECHO_BUFSZ      1024
static UH udpecho_buf[ECHO_BUFSZ/2];   /* UH and /2 for Word Aglinement */
static UH tcpecho_buf[ECHO_BUFSZ/2];

static ID echo_tskid;
static T_IPV4EP uecho_dstaddr;
static int uecho_len;
/************************************************/
/* UDP Based Echo Server Callback               */
/************************************************/

ER udpecho_cbk(ID cepid, FN fncd, VP parblk)
{
    switch(fncd)
    {
    case TFN_UDP_RCV_DAT:
        break;
    case TEV_UDP_RCV_DAT:
        uecho_len = udp_rcv_dat(cepid, &uecho_dstaddr, udpecho_buf, sizeof udpecho_buf, TMO_POL);
        wup_tsk(echo_tskid);
        break;
    }
    return E_OK;
}

/************************************************/
/* UDP Based Echo Server Task                   */
/************************************************/

#if defined(GAIO) && defined(H8300H)
TASK udpecho_tsk(VP_INT c)
#else
TASK udpecho_tsk(ID cepid)
#endif
{
#if defined(GAIO) && defined(H8300H)
    ID cepid;
    cepid = (ID) c;
#endif

    echo_tskid = vget_tid();

    for (;;)
    {
        /* Waiting for receive udp echo packet */
        /* It weke uped from udpecho_cbk */

        slp_tsk();

        udp_snd_dat(cepid, &uecho_dstaddr, udpecho_buf, uecho_len, TMO_FEVR);
    }
}

/************************************************/
/* UDP Based Echo Server Intialize              */
/************************************************/

ER udpecho_ini(ID tskid, ID cepid)
{
    ER ercd;

    /* Create Echo Server Task */

    if(tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&c_udpecho_tsk);
        if (ercd < 0)
            return ercd;
        tskid = ercd;
    }
    else{
        ercd = cre_tsk(tskid, &c_udpecho_tsk);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create UDP Commnunication End Point */

    if(cepid == 0)
    {   ercd = cepid = udp_vcre_cep(&c_udpecho_cep);
        if(cepid <= 0)
            return ercd;
    }
    else
    {   ercd = udp_cre_cep(cepid, &c_udpecho_cep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Start Echo Server Task */

    ercd = sta_tsk(tskid, cepid);
    if (ercd != E_OK)
        return ercd;

    return E_OK;
}

/************************************************/
/* TCP Based Echo Server Callback               */
/************************************************/

ER tcpecho_cbk(ID cepid, FN fncd, VP parblk)
{
    return E_OK;
}

/************************************************/
/* TCP Based Echo Server Task                   */
/************************************************/

#if defined(GAIO) && defined(H8300H)
TASK tcpecho_tsk(VP_INT c)
#else
TASK tcpecho_tsk(int cepid_repid)
#endif
{
    T_IPV4EP dstaddr;
    ER len;
    ID cepid, repid;
#if defined(GAIO) && defined(H8300H)
    int cepid_repid;

    cepid_repid = (int) c;
#endif

    repid = cepid_repid & 0xff;
    cepid = (cepid_repid >> 8) & 0xff;

    for (;;)
    {
        if (tcp_acp_cep(cepid, repid, &dstaddr, TMO_FEVR) != E_OK)
        {   dly_tsk(1000/MSEC);
            continue;
        }
        for (;;)
        {   len = tcp_rcv_dat(cepid, tcpecho_buf, sizeof tcpecho_buf, TMO_FEVR);
            if (len <= 0)
                break;
            tcp_snd_dat(cepid, tcpecho_buf, len, TMO_FEVR);
        }
        tcp_cls_cep(cepid, TMO_FEVR);
    }
}

/************************************************/
/* TCP Based Echo Server Intialize              */
/************************************************/

ER tcpecho_ini(ID tskid, ID cepid, ID repid)
{
    ER ercd;

    /* Create Echo Server Task */

    if(tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&c_tcpecho_tsk);
        if (ercd < 0)
            return ercd;
        tskid = ercd;
    }
    else{
        ercd = cre_tsk(tskid, &c_tcpecho_tsk);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TCP Reception End Point */

    if(repid == 0)
    {   ercd = repid = tcp_vcre_rep(&c_tcpecho_rep);
        if (repid <= 0)
            return ercd;
    }
    else
    {   ercd = tcp_cre_rep(repid, &c_tcpecho_rep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TCP Commnunication End Point */

    if(cepid == 0)
    {    ercd = cepid = tcp_vcre_cep(&c_tcpecho_cep);
         if(cepid <= 0)
             return ercd;
    }
    else
    {   ercd = tcp_cre_cep(cepid, &c_tcpecho_cep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Start Echo Server Task */

    ercd = sta_tsk(tskid, (cepid << 8) + repid);
    if (ercd != E_OK)
        return ercd;

    return E_OK;
}

/* end */
