/******************************************************************************
* NORTi Network Ping Command Sample                                           *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 21/Mar/1999                                                                 *
* 03/Apr/1999 表示される IP アドレス違いを修正                                *
* 08/Feb/2002 Changed literal string type(B->char) for CC32R               OK *
*             written 'B str[] = "?";' then it is error in CC32R              *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonitod.h"
#include "nonteln.h"

#define MAX_PING_SIZE   128

extern SYSTIME SNEAR cdecl SYSCK;

/* Message String */

const char msg_icmp_err[]   = "ICMP Internal Error.\r\n";
const char msg_ping_tmout[] = "Error: No answer\r\n";
const char msg_ping_abort[] = "\r\nCommand Aborted: ping\r\n";
const char msg_ping_addr[]  = "Invalid IP address.\r\n";
const char msg_ping_help[]  = "Usage: ping host [timeout [count]]\r\n"
                           "host     Host IP address\r\n"
                           "timeout  Timeout in msec to wait for each reply\r\n"
                           "count    Number of echo requests to send\r\n";

/* Loacal Variable */

static ID ping_ctrl_c_tid;
static B ping_ctrl_c_flg;

/* Ping Control Block */

typedef struct t_ping_cb {
    T_ICMP_CB *next;        /* Pointer to Next Control Block (*1) */
    FP callback;            /* ICMP Callback Routine (*1) */
    UH id;                  /* ICMP Identifier (Using Task ID) */
    UH seq;                 /* ICMP Sequence Number */
    UH len;                 /* ICMP Message Length */
    B ng;                   /* Data NG */
    B rsv;                  /* Reserved */
    UW ipa;                 /* Host IP Address */
    UW rtt;                 /* Round Trip Time [ms] */
    B data[MAX_PING_SIZE];  /* Data of ICMP Message */
} T_PING_CB;

/* (*1) 'next' and 'callback' members are same as T_ICMP_CB */

/************************************************/
/* Print Result Message of Ping                 */
/************************************************/

void print_ping_result(INT len, UW ipaddr, UH seq, UW rtt, BOOL ng)
{
    B buf[80];
    B *s = buf;
    INT n;

    /* ddd bytes from ddd.ddd.ddd.ddd */

    ito_u((char *)s, (UH)len, 3); s += 3;
    memcpy(s, " bytes from ", 12); s += 12;
    s += ipaddr_to_ascii(s, ipaddr);

    /* : icmp_seq = ddd, time = ddddd ms */

    memcpy(s, ": icmp_seq = ", 13); s += 13;
    n = itoulen(seq);
    itou((char *)s, seq, n); s += n;

    memcpy(s, ", time = ", 9); s += 9;
    n = ltoulen(rtt);
    ltou((char *)s, rtt, n); s += n;
    memcpy(s, " ms", 3 + 1); s += 3;

    if (ng)
        memcpy(s, ", data NG", 9 + 1);

    puts(buf);
}

/************************************************/
/* ICMP Callback Routine for Ping               */
/************************************************/

VP ping_callback(T_ICMP_CB *cb, T_IP *pkt, T_ICMP_MSG *icmp, INT len)
{
    T_PING_CB *ping = (T_PING_CB *)cb;

    if ((icmp->type != TYPE_ECHO_REP)
     || (icmp->id != ping->id))
        return pkt;

    ping->rtt = (SYSCK.ltime - *(UW *)icmp->data) * MSEC;
    ping->ng = FALSE;
    if ((ping->len != (UH)len)
     || (memcmp(ping->data + 4, icmp->data + 4, len - 8 - 4) != 0))
        ping->ng = TRUE;
    ping->seq = icmp->seq;
    ping->ipa = pkt->ip.sa;
    if (ping->id != 0)
        wup_tsk(ping->id);
    return NULL;
}

/************************************************/
/* Ping Process                                 */
/************************************************/

ER ping_proc(T_PING_CB *ping, UW ipaddr, TMO tmout, INT count, const B *str)
{
    TMO pause = 1000/MSEC;
    T_ICMP_HEADER icmp;
    INT len;
    ER ercd;
    UH seq;

    /* Define ICMP Callback Routine */

    ercd = icmp_def_cbk((T_ICMP_CB *)ping, ping_callback);
    if (ercd != E_OK) {
        print((B *)msg_icmp_err);
        return ercd;
    }

    /* Store Echo String to Compare */

    len = 4 + strlen((char *)str);
    if (len > MAX_PING_SIZE)
        len = MAX_PING_SIZE;
    memcpy(ping->data + 4, str, len - 4);
    ping->len = len + 8;

    /* Idenfier and Init. Sequence Number */

    vcan_wup();
    ping->id = (UH)vget_tid();
    seq = 1;

    /* Send and Receive ICMP Messages */

    while (count-- > 0) {

        /* Make Header of ICMP Echo Message */

        icmp.type = TYPE_ECHO_REQ;
        icmp.code = 0;
        icmp.id = ping->id;
        icmp.seq = seq++;

        /* Send Echo Request */

        *(UW *)ping->data = SYSCK.ltime;
        ercd = icmp_snd_dat(IPV4_ADDRANY, ipaddr, &icmp, ping->data, len);
        if (ercd != E_OK) {
            print((B *)msg_icmp_err);
            break;
        }

        /* Wait Echo Reply */

        ping_ctrl_c_tid = vget_tid();
        ercd = tslp_tsk(tmout);
        ping_ctrl_c_tid = 0;
        if (ping_ctrl_c_flg) {
            print((B *)msg_ping_abort);
            break;
        }
        else if (ercd == E_TMOUT) {
            print((B *)msg_ping_tmout);
            break;
        }
        else if (ercd != E_OK) {
            print((B *)msg_icmp_err);
            break;
        }

        /* Print Result */

        else {
            do {
                print_ping_result(ping->len, ping->ipa, ping->seq, ping->rtt, ping->ng);
            } while (tslp_tsk(0) == E_OK);
        }
        ping_ctrl_c_tid = vget_tid();
        dly_tsk(pause);
        ping_ctrl_c_tid = 0;
        if (ping_ctrl_c_flg) {
            print((B *)msg_ping_abort);
            break;
        }
    }

    /* Remove ICMP Callback Routine */

    icmp_can_snd(&icmp);
    icmp_def_cbk((T_ICMP_CB *)ping, NULL);
    return ercd;
}

/************************************************/
/* Ping CTRL-C Signal Handler                   */
/************************************************/

static void ping_ctrl_c(int sig)
{
    ID id;

    if (sig == SIGINT) {
        ping_ctrl_c_flg = TRUE;
        if ((id = ping_ctrl_c_tid) != 0)
            rel_wai(id);
    }
}

/************************************************/
/* Ping Command Sample                          */
/************************************************/

int ping_command(int argc, char *argv[])
{
    TMO tmout = 10000/MSEC;
    INT count = 3;
    T_PING_CB ping;
    UW ipaddr;

    /* Print Help */

    if (argc < 2) {
        print((B *)msg_ping_help);
        return 0;
    }

    /* Arguments Convert */

    ipaddr = ascii_to_ipaddr((B *)argv[1]);
    if (ipaddr == 0L) {
        print((B *)msg_ping_addr);
        return 0;
    }

    if (argc > 2)
        tmout = atol(argv[2]) / MSEC;
    if (argc > 3)
        count = atoi(argv[3]);

    /* Define CTRL-C Signal Handler */

    ping_ctrl_c_tid = 0;
    ping_ctrl_c_flg = 0;
    signal(SIGINT, ping_ctrl_c);

    /* Ping Process */

    ping_proc(&ping, ipaddr, tmout, count, (B *)"0123456789ABCDEFGHIJ");

    /* Remove CTRL-C Signal Handler */

    signal(SIGINT, SIG_DFL);

    return 0;
}

/* end */
