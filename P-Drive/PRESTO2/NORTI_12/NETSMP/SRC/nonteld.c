/******************************************************************************
* NORTi Network TELNET Daemon                                                 *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Feb/1999                                                                 *
* 22/Mar/1999 signal is added.                                                *
* 25/Apr/1999 All functions are modified.                                     *
* 01/May/1999 Timeout is modified.                                            *
* 05/May/1999 kbhit and getch are added.                                      *
* 08/May/1999 Option negotiation is added.                                    *
* 29/May/1999 telnet_tmout 10 -> 5 min.                                       *
* 22/Sep/1999 resource ID auto assign added.                                  *
* 16/Nov/2000 Changed from telnet_tmout to TELNET_TMOUT.                      *
* 23/Feb/2002 vsta_tsk is called for 16-bit INT CPU.                       HM *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nosio.h"
#include "nonet.h"
#include "nonteln.h"
#include "noncons.h"

#define CTRL_C      0x03

TASK telnetd_tsk(T_TELNETD *t);
const T_CTSK telnetd_ctsk = { NULL, TA_HLNG, telnetd_tsk, TELNETD_PRI, TELNETD_SSZ+COMMAND_LINE_LEN };
const T_CMBF telnetd_cmbf = { 0, TA_TFIFO, 16, 2 };
extern T_TERMINAL *current_terminal;
extern SYSTIME SNEAR cdecl SYSCK;

#define TELNET_TMOUT  5 * 60 * (1000/MSEC)

/************************************************/
/* Put String to TELNET or Console              */
/************************************************/

BOOL terminal_print(T_TERMINAL *t, const B *s)
{
    if (t == NULL)
    {   t = current_terminal;
        if (t == NULL)
            return FALSE;
    }

    if (t->cepid == 0)
        return console_print((T_CONSOLE *)t, s);
    else
        return telnetd_print((T_TELNETD *)t, s);
}

/************************************************/
/* Put String and CR+LF to TELNET or Console    */
/************************************************/

BOOL terminal_puts(T_TERMINAL *t, const B *s)
{
    if (!terminal_print(t, s))
        return FALSE;
    return terminal_print(t, (B *)"\r\n");
}

/************************************************/
/* Put CR+LF and String to TELNET or Console    */
/************************************************/

BOOL terminal_rputs(T_TERMINAL *t, const B *s)
{
    if (!terminal_print(t, (B *)"\r\n"))
        return FALSE;
    return terminal_print(t, s);
}

/************************************************/
/* Put Charactor to TELNET or Console           */
/************************************************/

BOOL terminal_putch(T_TERMINAL *t, INT c)
{
    B s[2];

    s[0] = (B)c;
    s[1] = '\0';
    return terminal_print(t, s);
}

/************************************************/
/* Get String from TELNET or Console            */
/************************************************/

INT terminal_input(T_TERMINAL *t, B *buf, INT size)
{
    ER ercd, len;
    INT i, skip;
    H c;

    if (t == NULL)
    {   t = current_terminal;
        if (t == NULL)
        {   dly_tsk(100/MSEC);
            return 0;
        }
    }

    for (i = skip = c = 0;;)
    {
        ercd = rcv_mbf(&c, &len, t->rx_mbfid);
        if (ercd != E_OK || len != 2)
        {   t->ercd = ercd;
            return ercd;
        }

        if (c <= 0)
            return c;
        if (i == 0 && c == '\n')    /* Ignore Last LF */
            continue;
        if (skip)
        {   skip--;
            continue;
        }
        if (c == '\r' || c == CTRL_C)   /* CR or CTRL-C */
        {   buf[i] = '\0';
            return c;
        }
        if (c == '\b')      /* BS */
        {   if (i > 0)
            {   i--;
                if (t->echo_on)
                {   if (!terminal_print(t, (B *)"\b \b"))
                        break;
                }
            }
            continue;
        }
        if (c == 0x1b)      /* ESC x x */
        {   skip = 2;
            continue;
        }
        if ((UB)c < 0x20 || (UB)c > 0x7e)
            continue;
        if (i >= size - 1)
            continue;
        buf[i++] = (B)c;
        if (t->echo_on)
        {   if (t->echo_on == '*')
                c = '*';
            if (!terminal_putch(t, c))
                break;
        }
    }
    buf[i] = '\0';
    if (c & 0x80)
        c |= 0xff00;
    return c;       /* Last Charactor */
}

B *terminal_gets(T_TERMINAL *t, B *s, INT size)
{
    INT c;

    for (;;)
    {   c = terminal_input(t, s, size);
        if (c == '\r')
            break;
        dly_tsk(1000/MSEC);
    }
    terminal_print(t, (B *)"\r\n");
    return s;
}

/************************************************/
/* Test a Character be Input or not             */
/************************************************/

BOOL terminal_kbhit(T_TERMINAL *t)
{
    T_RMBF rmbf;
    ER ercd;

    if (t == NULL)
    {   t = current_terminal;
        if (t == NULL)
            return FALSE;
    }

    ercd = ref_mbf(&rmbf, t->rx_mbfid);
    if (ercd == E_OK && rmbf.msgsz != 0)
        return TRUE;
    else
        return FALSE;
}

/************************************************/
/* Get a Character                              */
/************************************************/

INT terminal_getch(T_TERMINAL *t)
{
    ER ercd;
    INT len;
    H c;

    if (t == NULL)
    {   t = current_terminal;
        if (t == NULL)
        {   dly_tsk(100/MSEC);
            return 0;
        }
    }

    ercd = rcv_mbf(&c, &len, t->rx_mbfid);
    if (ercd != E_OK || len != 2)
    {   t->ercd = ercd;
        return 0;
    }
    return c;
}

/************************************************/
/* Define Signal Handler                         */
/************************************************/

SIGNAL_FUNC terminal_signal(T_TERMINAL *t, int sig, SIGNAL_FUNC func)
{
    SIGNAL_FUNC old;

    if (t == NULL)
    {   t = current_terminal;
        if (t == NULL)
            return FALSE;
    }

    if (sig != SIGINT)
        return SIG_ERR;
    old = (SIGNAL_FUNC)t->sigint;
    t->sigint = (FP)func;
    return old;
}

/************************************************/
/* TELNET Low Level Put String                  */
/************************************************/

BOOL telnetd_print(T_TELNETD *t, const B *s)
{
    ER ercd;
    INT len;

    t->time = SYSCK.ltime;
    len = strlen((char *)s);
    if (len == 0)
        return TRUE;

    for (;;)
    {   ercd = tcp_snd_dat(t->cepid, s, len, TMO_FEVR);
        if (ercd <= 0)
        {   t->ercd = ercd;
            return FALSE;
        }
        if ((len -= ercd) == 0)
            return TRUE;
        s += ercd;
    }
}

/************************************************/
/* TELNET Low Level Get Charactors              */
/************************************************/

INT telnetd_getch(T_TELNETD *t)
{
    ER len;
    TMO tmo;
    UB c;

    t->time = SYSCK.ltime;
    tmo = TELNET_TMOUT;
    for (;;)
    {   len = tcp_rcv_dat(t->cepid, &c, 1, tmo);
        if (len <= 0)
        {   if (len == E_TMOUT)
            {   tmo = SYSCK.ltime - t->time;
                if ((UW)tmo < (UW)TELNET_TMOUT)
                {   tmo = TELNET_TMOUT - tmo;
                    continue;
                }
            }
            return len;     /* Error Code */
        }
        if (c != 0)
            return c;
    }
}

/************************************************/
/* TELNET Option Negotiation                    */
/************************************************/

INT telnetd_opt_nego(T_TELNETD *t)
{
    INT c;

    telnetd_print(t, (B *)"\xff""\xfb""\x03"     /* <IAC><WILL><suppress GA> */
                          "\xff""\xfb""\x01"     /* <IAC><WILL><echo> */
                          "\xff""\xfe""\x01");   /* <IAC><DONT><echo> */
    for (;;)
    {
        c = telnetd_getch(t);
        if (c <= 0)
            return c;
        c = telnetd_getch(t);
        if (c <= 0)
            return c;
        c = telnetd_getch(t);
        if (c != 0xff)
            return c;
    }
}

/************************************************/
/* TELNET Daemon Task                           */
/************************************************/

TASK telnetd_tsk(T_TELNETD *t)
{
    H c;
    ER ercd;
    BOOL sio;
    SIGNAL_FUNC func;

    sio = (t->cepid == 0);

    for (;;)
    {
        t->logged_in = FALSE;

        /* Accept TCP Connection */

        if (!sio)
        {   while (tcp_acp_cep(t->cepid, t->repid, &t->dstaddr, TMO_FEVR) != E_OK)
                dly_tsk(1000/MSEC);
        }

        /* Start Command Shell Task */

        if (t->shell_tskid != FALSE)
        {   while (vsta_tsk(t->shell_tskid, (VP)t) != E_OK)
            {   ter_tsk(t->shell_tskid);
                dly_tsk(1000/MSEC);
            }
        }

        /* Queue Received Charactors */

        for (;;)
        {
            if (sio)
                c = (H)console_getch((T_CONSOLE *)t);
            else
            {   c = (H)telnetd_getch(t);
                if (c == 0xff) /* IAC */
                {   c = telnetd_opt_nego(t);
                    if (c <= 0)
                        break;
                }
            }

            if (t->logged_in == (B)-1)
                break;

            ercd = snd_mbf(t->rx_mbfid, &c, 2);
            if (ercd != E_OK)
                break;
            if (c == CTRL_C)
            {   if (t->echo_on)
                    terminal_print(t, (B *)"^C");
            }
            if (c == CTRL_C || c <= 0)
            {   func = (SIGNAL_FUNC)t->sigint;
                if (func != SIG_DFL)
                    func(SIGINT);
            }
            if (c == 0)
            {   dly_tsk(1000/MSEC);
                break;
            }
        }

        /* Close TCP */

        if (!sio)
            tcp_cls_cep(t->cepid, TMO_FEVR);
    }
}

/************************************************/
/* TELNET Daemon Intialize                      */
/************************************************/

ER telnetd_ini(T_TELNETD *t, ID tskid, ID mbfid, ID cepid, ID repid)
{
    static const T_TCP_CREP crep = { 0, { IPV4_ADDRANY, TCP_PORT_TELNET } };
    T_TCP_CCEP ccep;
    ER ercd;

    /* Control Block Initialize */

    memset(t, 0, sizeof (T_TELNETD));
    t->telnetd_tskid = tskid;
    t->rx_mbfid = mbfid;
    t->cepid = cepid;
    t->repid = repid;
    t->echo_on = 1;

    /* Create MessageBuffer */

    if(t->rx_mbfid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_mbf(&telnetd_cmbf);
        if (ercd < 0)
            return ercd;
        t->rx_mbfid = ercd;
    }
    else{
        ercd = cre_mbf(t->rx_mbfid, &telnetd_cmbf);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TCP Commnunication End Point */

    memset(&ccep, 0, sizeof (T_TCP_CCEP));
    ccep.sbuf = t->sbuf;
    ccep.sbufsz = sizeof t->sbuf;
    ccep.rbuf = t->rbuf;
    ccep.rbufsz = sizeof t->rbuf;

    if(t->cepid == 0)
    {   ercd = t->cepid = tcp_vcre_cep(&ccep);
        if(t->cepid <= 0)
            return ercd;
    }
    else
    {   ercd = tcp_cre_cep(t->cepid, &ccep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TCP Reception End Point */

    if(t->repid == 0)
    {   ercd = t->repid = tcp_vcre_rep(&crep);
        if(t->repid <= 0 && ercd != E_OBJ)
            return ercd;
    }
    else
    {   ercd = tcp_cre_rep(t->repid, &crep);
        if (ercd != E_OK && ercd != E_OBJ)
            return ercd;
    }

    /* Create TELNET Daemon Task */

    if(t->telnetd_tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&telnetd_ctsk);
        if (ercd < 0)
            return ercd;
        t->telnetd_tskid = ercd;
    }
    else{
        ercd = cre_tsk(t->telnetd_tskid, &telnetd_ctsk);
        if (ercd != E_OK)
            return ercd;
    }

    return E_OK;
}

/* end */
