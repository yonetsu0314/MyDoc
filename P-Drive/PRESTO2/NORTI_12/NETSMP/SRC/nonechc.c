/******************************************************************************
* NORTi Network Echo Client Sample                                            *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 16/Feb/1999                                                                 *
* 04/Apr/1999                                                                 *
* 23/Sep/1999 CEP,REP ID auto assign added.(Create tcp_vcre_cep,tcp_vcre_rep) *
* 23/Apr/2000 Change tcp_con_cep timeout 5000->6000                           *
* 15/Jan/2003 udp_echo_sub()Ç©ÇÁtcpip_creticalÇçÌèú                       KA *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonitod.h"
#include "nonteln.h"
#include "nonecho.h"

UW input_ipaddr(B *prompt);

/* Window Buffer */

#define WINDOW_SIZE 100
static UH echocli_swin[WINDOW_SIZE/2];  /* Sending Window Buffer */
static UH echocli_rwin[WINDOW_SIZE/2];  /* Receiving Window Buffer */

/* Creation Information */

ER udpecho_client_cbk(ID, FN, VP);
ER tcpecho_client_cbk(ID, FN, VP);
const T_UDP_CCEP c_udpecho_client_cep = { 0, { IPV4_ADDRANY, UDP_PORTANY }, (FP)udpecho_client_cbk };
const T_TCP_CCEP c_tcpecho_client_cep = { 0, echocli_swin, sizeof echocli_swin, echocli_rwin, sizeof echocli_rwin, (FP)tcpecho_client_cbk };

/* Local Data */

#define ECHO_BUFSZ      1024
static UH echo_buf[ECHO_BUFSZ/2];   /* UH and /2 for Word Aglinement */
static ID client_tskid;

#ifdef NBLK
static int tcpecho_len;
#endif

/************************************************/
/* UDP Based Echo Client Callback               */
/************************************************/

ER udpecho_client_cbk(ID cepid, FN fncd, VP parblk)
{
    return E_OK;
}

/************************************************/
/* UDP Based Echo Client Subroutine             */
/************************************************/

int udp_echo_sub(ID cepid, UW ipaddr, B *buf, int len, int bufsz)
{
    T_IPV4EP dstaddr;

    dstaddr.ipaddr = ipaddr;
    dstaddr.portno = UDP_PORT_ECHO;

    len = udp_snd_dat(cepid, &dstaddr, buf, len, 5000/MSEC);
    if (len <= 0)
        goto END;

    len = udp_rcv_dat(cepid, &dstaddr, buf, bufsz, 5000/MSEC);

END:

    return len;
}

/************************************************/
/* UDP Based Echo Client Process (for test)     */
/************************************************/

void udp_echo_client(ID cepid)
{
    B *buf = (B *)echo_buf;
    int bufsz = sizeof echo_buf;
    UW ipaddr;
    int len;
    ER ercd;

    puts((B *)"** UDP Based Echo Client **");


    if(cepid == 0)
    {   cepid = udp_vcre_cep(&c_udpecho_client_cep);
        if(cepid <= 0)
        {   puts((B *)"UDP Communication End Point can't be created!");
            return;
        }
    }
    else
    {   ercd = udp_cre_cep(cepid, &c_udpecho_client_cep);
        if (ercd != E_OK)
        {   puts((B *)"UDP Communication End Point can't be created!");
            return;
        }
    }

    ipaddr = input_ipaddr((B *)"Destination IP Address = ");

    for (;;)
    {
        print((B *)"Tx>");
        input(buf, bufsz);
        len = strlen((char *)buf);
        if (len == 0)
        {   puts((B *)"");
            break;
        }
        len = udp_echo_sub(cepid, ipaddr, buf, len, bufsz);
        if (len <= 0)
        {   puts((B *)"\r\nUDP Communication Error!");
            break;
        }
        if (len != 0)
        {   buf[len] = '\0';
            print((B *)" Rx>");
            puts(buf);
        }
    }
    udp_del_cep(cepid);
}

/************************************************/
/* TCP Based Echo Client Callback               */
/************************************************/

ER tcpecho_client_cbk(ID cepid, FN fncd, VP parblk)
{
    switch (fncd)
    {
    case TFN_TCP_CON_CEP:
        wup_tsk(client_tskid);
        break;

    case TFN_TCP_RCV_DAT:
#ifdef NBLK
        tcpecho_len = *(INT *)parblk;
#endif
        wup_tsk(client_tskid);
        break;

    case TFN_TCP_SND_DAT:
        wup_tsk(client_tskid);
        break;

    case TFN_TCP_CLS_CEP:
        wup_tsk(client_tskid);
        break;
    }
    return E_OK;
}

/************************************************/
/* TCP Based Echo Client Subroutine             */
/************************************************/

int tcp_echo_sub(ID cepid, B *buf, int len, int bufsz)
{
    ER ercd;
    int i;

    i = 0;
    do
    {
      #ifdef NBLK
        ercd = tcp_snd_dat(cepid, buf + i, len - i, TMO_NBLK);
        if (ercd == E_WBLK)
        {   ercd = slp_tsk();
            if (ercd == E_OK)
                ercd = tcpecho_len;
        }
      #else
        ercd = tcp_snd_dat(cepid, buf + i, len - i, TMO_FEVR);
      #endif
        if (ercd <= 0)
            return ercd;
        i += ercd;
    } while (i < len);

    i = 0;
    do
    {
      #ifdef NBLK
        ercd = tcp_rcv_dat(cepid, buf + i, bufsz - i, TMO_NBLK);
        if (ercd == E_WBLK)
            ercd = tslp_tsk(1000/MSEC);
      #else
        ercd = tcp_rcv_dat(cepid, buf + i, bufsz - i, 1000/MSEC);
      #endif
        if (ercd <= 0)
            break;
        i += ercd;
    } while (i < len);

    return i;
}

/************************************************/
/* TCP Based Echo Client Process (for test)     */
/************************************************/

void tcp_echo_client(ID cepid)
{
    B *buf = (B *)echo_buf;
    int bufsz = sizeof echo_buf;
    UW ipaddr;
    int len;
  #ifdef DEBUG
    int i;
  #endif
    ER ercd;
    T_IPV4EP myaddr;
    T_IPV4EP dstaddr;

    puts((B *)"** TCP Based Echo Client **");

    if(cepid == 0)
    {   cepid = tcp_vcre_cep(&c_tcpecho_client_cep);
        if (cepid <= 0)
        {   puts((B *)"TCP Communication End Point can't be created!");
            return;
        }
    }
    else
    {   ercd = tcp_cre_cep(cepid, &c_tcpecho_client_cep);
        if (ercd != E_OK)
        {   puts((B *)"TCP Communication End Point can't be created!");
            return;
        }
    }
    client_tskid = vget_tid();

    ipaddr = input_ipaddr((B *)"Destination IP Address = ");

    myaddr.ipaddr = IPV4_ADDRANY;
    myaddr.portno = TCP_PORTANY;
    dstaddr.ipaddr = ipaddr;
    dstaddr.portno = TCP_PORT_ECHO;

  #ifdef NBLK
    ercd = tcp_con_cep(cepid, &myaddr, &dstaddr, TMO_NBLK);
    if (ercd == E_WBLK)
        ercd = slp_tsk();
  #else
    ercd = tcp_con_cep(cepid, &myaddr, &dstaddr, 8000/MSEC);
  #endif
    if (ercd != E_OK)
    {   puts((B *)"TCP connection error!");
        goto L1;
    }
  #ifdef DEBUG
    for (i = 0; i < 10; i++)
    {
        strcpy(buf, "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        len = strlen(buf);
        len = tcp_echo_sub(cepid, buf, len, bufsz);
        if (len <= 0)
            break;
    }
  #else
    for (;;)
    {
        print((B *)"Tx>");
        input(buf, bufsz);
        len = strlen((char *)buf);
        if (len == 0)
        {   puts((B *)"");
            break;
        }
        len = tcp_echo_sub(cepid, buf, len, bufsz);
        if (len <= 0)
            break;
        buf[len] = '\0';
        print((B *)" Rx>");
        puts(buf);
    }
  #endif

  #ifdef NBLK
    ercd = tcp_cls_cep(cepid, TMO_NBLK);
    if (ercd == E_WBLK)
        ercd = slp_tsk();
  #else
    ercd = tcp_cls_cep(cepid, 1000/MSEC);
  #endif
    if (ercd != E_OK)
        puts((B *)"TCP closing error!");
L1:
    tcp_del_cep(cepid);
}

/* end */
