/**************************************************************************
* NORTi Network Console Input/Output Function for TELNETD                 *
*                                                                         *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                               *
*  All rights reserved.                                                   *
*                                                                         *
* 23/Feb/1999                                                             *
* 15/Mar/1999 '#include nosio3.h' is deleted.                             *
* 22/Sep/1999 resource ID auto assign added.                              *
**************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nosio.h"
#include "nonet.h"
#include "noncons.h"
#include "nonteln.h"

#define CTRL_C      0x03

extern const T_CTSK telnetd_ctsk;
extern const T_CMBF telnetd_cmbf;

/************************************************/
/* Console Low Level Put String                 */
/************************************************/

BOOL console_print(T_CONSOLE *t, const B *s)
{
    UB c;
    INT ch = t->ch;

    while ((c = (UB)*s++) != '\0')
        put_sio(ch, c, TMO_FEVR);
    return TRUE;
}

/************************************************/
/* Console Low Level Get String                 */
/************************************************/

INT console_input(T_CONSOLE *t, B *buf, int size)
{
    ER ercd;
    INT i, skip;
    UB c;
    SIGNAL_FUNC func;
    INT ch = t->ch;
    TMO tmout = 40/MSEC;

    for (i = skip = 0;;)
    {
        ercd = get_sio(ch, &c, tmout);
        if (ercd == E_TMOUT)
            return E_TMOUT;
        if (ercd != E_OK)
        {   t->ercd = ercd;
            continue;
        }
        if (i == 0 && c == '\n')    /* Ignore Last LF */
            continue;
        tmout = TMO_FEVR;

        if (skip)
        {   skip--;
            continue;
        }
        if (c == '\r')      /* CR */
        {   buf[i] = '\0';
            return '\r';
        }
        if (c == '\b')      /* BS */
        {   if (i > 0)
            {   i--;
                if (t->echo_on)
                {   put_sio(ch, '\b', TMO_FEVR);
                    put_sio(ch, ' ',  TMO_FEVR);
                    put_sio(ch, '\b', TMO_FEVR);
                }
            }
            continue;
        }
        if (c == CTRL_C)
        {   if (t->echo_on)
            {   put_sio(ch, '^', TMO_FEVR);
                put_sio(ch, 'C', TMO_FEVR);
            }
            buf[i] = '\0';
            func = (SIGNAL_FUNC)t->sigint;
            if (func != SIG_DFL)
                func(SIGINT);
            return CTRL_C;
        }
        if (c == 0x1b)      /* ESC x x */
        {   skip = 2;
            continue;
        }
        if ((UB)c < 0x20 || (UB)c > 0x7e)
            continue;
        if (i >= size - 1)
            continue;
        buf[i++] = c;
        if (t->echo_on)
        {   if (t->echo_on == '*')
                c = '*';
            put_sio(ch, c, TMO_FEVR);
        }
    }
}

/************************************************/
/* Console Low Level Get Character              */
/************************************************/

INT console_getch(T_CONSOLE *t)
{
    ER ercd;
    UB c;

    ercd = get_sio(t->ch, &c, TMO_FEVR);
    if (ercd == E_RLWAI)
        return ercd;
    return c;
}

/************************************************/
/* Console Low Level Input Check                */
/************************************************/

BOOL console_kbhit(T_CONSOLE *t)
{
    T_SIOS sios;

    ref_sio(t->ch, &sios);
    if (sios.rxlen != 0)
        return TRUE;
    else
        return FALSE;
}

/************************************************/
/* Console Intialize                            */
/************************************************/

ER console_ini(T_CONSOLE *t, ID tskid, ID mbfid, INT ch, const B *param)
{
    ER ercd;

    /* Control Block Initialize */

    memset(t, 0, sizeof (T_CONSOLE));
    t->telnetd_tskid = tskid;
    t->rx_mbfid = mbfid;
    t->cepid = 0;
    t->ch = ch;
    t->echo_on = 1;

    /* UART Device Initialize */

    ercd = ini_sio(t->ch, param);
    if (ercd != E_OK)
        return ercd;
    ercd = ctl_sio(t->ch, TSIO_RXE|TSIO_TXE|TSIO_RTSON|TSIO_DTRON);
    if (ercd != E_OK)
        return ercd;

    /* Create MessageBuffer */

    if(t->rx_mbfid == 0){ /* ID auto (Add by Y.Y) */
        ercd = vcre_mbf(&telnetd_cmbf);
        if(ercd < 0)
            return ercd;
        t->rx_mbfid = ercd;
    }
    else{
        ercd = cre_mbf(t->rx_mbfid, &telnetd_cmbf);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TELNET Daemon Task */

    if(t->telnetd_tskid == 0){
        ercd = vcre_tsk(&telnetd_ctsk);/* ID auto (Add by Y.Y) */
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
