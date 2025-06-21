/******************************************************************************
* NORTi Network Command Shell for TELNET Sample                               *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Feb/1999                                                                 *
* 22/Sep/1999 resource ID auto assign added.                                  *
* 08/Feb/2002 Changed literal string type(B->char) for CC32R               OK *
*             written 'B str[] = "?";' then it is error in CC32R              *
* 23/Feb/2002 vsta_tsk is called for 16-bit INT CPU.                       HM *
* 25/Apr/2003 Added dly_tsk in shell_tsk                                   YS *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonteln.h"

#define CTRL_C      0x03

const char msg_login_ng[]    = "login incorrect.";
const char msg_telnet_busy[] = "TELNET is already used.";
const char msg_telnet_tmout[]= "timeout.";
T_TERMINAL *current_terminal;

TASK shell_tsk(T_TERMINAL *t);
const T_CTSK shell_ctsk = { NULL, TA_HLNG, shell_tsk, SHELL_PRI, SHELL_SSZ + COMMAND_LINE_LEN };
const T_CMBF shell_cmbf = { 0, TA_TFIFO, COMMAND_LINE_LEN + 2, COMMAND_LINE_LEN };

#ifdef TEST /* for Debugging */
void test(T_TERMINAL *t, B *s);
#endif

/************************************************/
/* Login Process                                */
/************************************************/

BOOL login_proc(T_TERMINAL *t, B *login_prompt, B *passwd_prompt, PASSWD_CHECK callback)
{
    B name[LOGIN_NAME_LEN+1];
    B passwd[PASSWORD_LEN+1];
    INT n, c;
    B echo;

    for (;;)
    {
        /* Get Login Name */

        for (;;)
        {
            if (!terminal_rputs(t, login_prompt))
                return FALSE;
            for (;;)
            {   c = terminal_input(t, name, sizeof name);
                if (c <= 0)
                    return FALSE;
                break;
            }
            if (c == '\r')
                break;
        }

        /* Get Password */

        for (n = 3; n > 0; n--)
        {
            if (!terminal_rputs(t, passwd_prompt))
                return FALSE;
            echo = t->echo_on;
            for (;;)
            {   if (echo)
                    t->echo_on = '*';
                c = terminal_input(t, passwd, sizeof passwd);
                t->echo_on = echo;
                if (c <= 0)
                    return FALSE;
                break;
            }
            if (c != '\r')
                break;

            if (callback == NULL)
                return TRUE;
            t->prompt = (*callback)(t, name, passwd);
            if (t->prompt != NULL)
                return TRUE;
            if (!terminal_rputs(t, (B *)msg_login_ng))
                return FALSE;
        }
    }
}

/************************************************/
/* Command Shell                                 */
/************************************************/

INT command_shell(T_TERMINAL *t)
{
    B s[COMMAND_LINE_LEN+1];
    INT c;

    for (;;)
    {
        /* Put Prompt */

        if (!terminal_rputs(t, t->prompt))
            return 0;

        /* Get String */

        for (;;)
        {   c = terminal_input(t, s, sizeof s);
            if (c <= 0)
                return c;
            break;
        }
        if (s[0] == '\0' || c != '\r')
            continue;

        /* Logout Command */

        if (strcmp((char *)s, (char *)"logout") == 0 || strcmp((char *)s, (char *)"bye") == 0)
        {   terminal_rputs(t, (B *)"");
            return TRUE;
        }

        /* Other Commands */

        puts((B *)"");
      #ifdef TEST /* for Debugging */
        if (memcmp(s, "test", 4) == 0)
        {   test(t, s);
            continue;
        }
      #endif
        if (!telnetd_callback(t, s))
            return TRUE;
    }
}

/************************************************/
/* Command Shell Task                           */
/************************************************/

TASK shell_tsk(T_TERMINAL *t)
{
    int c;

    for (;;)
    {
        dly_tsk(100/MSEC);
        if (!login_proc(t, (B *)"login: ", (B *)"Password: ", (PASSWD_CHECK)t->passwd_check))
            break;

        if (current_terminal != NULL)
        {   if (!terminal_rputs(t, (B *)msg_telnet_busy))
                break;
            continue;
        }
        current_terminal = (T_TERMINAL *)t;

        t->logged_in = TRUE;
        c = command_shell(t);
        if (c == E_TMOUT)
        {   terminal_print(t, (B *)"\r\n");
            terminal_print(t, (B *)msg_telnet_tmout);
            terminal_print(t, (B *)"\r\n");
        }
        break;
    }
    current_terminal = NULL;
    t->logged_in = (B)-1;
    dly_tsk(10/MSEC);
    rel_wai(t->telnetd_tskid);
    ext_tsk();
}

/************************************************/
/* Command Shell Intialize                      */
/************************************************/

ER shell_ini(VP t, ID tskid, ID mbfid, PASSWD_CHECK callback)
{
    T_TERMINAL *tt = (T_TERMINAL *)t;
    ER ercd;

    /* Control Block Initialize (after TELNET Daemon Init.) */

    tt->shell_tskid = tskid;
    tt->tx_mbfid = mbfid;
    tt->passwd_check = (FP)callback;

    /* Create Command Shell Task */

    if(tt->shell_tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&shell_ctsk);
        if (ercd < 0)
            return ercd;
        tt->shell_tskid = ercd;
    }
    else{
        ercd = cre_tsk(tt->shell_tskid, &shell_ctsk);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TELNET Transmission MessageBuffer */

    if(tt->tx_mbfid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_mbf(&shell_cmbf);
        if (ercd < 0)
            return ercd;
        tt->tx_mbfid = ercd;
    }
    else{
        ercd = cre_mbf(tt->tx_mbfid, &shell_cmbf);
        if (ercd != E_OK)
            return ercd;
    }

    /* Start TELNET Daemon Task */

    ercd = vsta_tsk(tt->telnetd_tskid, t);
    if (ercd != E_OK)
        return ercd;

    return E_OK;
}

/* end */
