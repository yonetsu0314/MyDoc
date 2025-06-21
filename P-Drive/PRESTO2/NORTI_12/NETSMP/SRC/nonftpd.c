/******************************************************************************
* NORTi Network FTP Server Sample                                             *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 28/Feb/1999                                                                 *
* 08/May/1999 DELE is added.                                                  *
* 29/May/1999 FTPD_IDLE_TMOUT is added.                                       *
* 09/Jun/1999 APPE is added.                                                  *
* 23/Sep/1999 CEP,REP ID auto assign added. (Create tcp_vcre_cep,tcp_vcre_rep)*
* 26/Nov/1999 Removal warning for exeGCC.                                     *
* 25/Apr/2000 Removal VP_INT for NORTi v4.                                    *
* 12/Feb/2001 Change FTPD_TMOUT 5000 ¨ 30000/MSEC                            *
* 08/Feb/2002 Changed literal string type(B->char) for CC32R               OK *
*             written 'B str[] = "?";' then it is error in CC32R              *
* 23/Feb/2002 vsta_tsk is called for 16-bit INT CPU.                       HM *
* 18/Mar/2003 modified ftp_pass_command for return string from call back   YS *
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
#include "nonfile.h"
#include "nonftp.h"
#include "nonteln.h"

#ifndef ITRON_H
#ifndef VP_INT
#define VP_INT  int
#endif
#endif

/* Creation Information */

TASK ftp_server_tsk(T_FTP *ftp);
const T_CTSK ftp_server_ctsk = { NULL, TA_HLNG, ftp_server_tsk, FTP_SERVER_PRI, FTP_SERVER_SSZ };

/* Timeout */

#define FTPD_TMOUT      (30000/MSEC)
#if 1
#define FTPD_IDLE_TMOUT (5*60*(1000/MSEC))
#else /* for debugging */
#define FTPD_IDLE_TMOUT (30*(1000/MSEC))
#endif

/* Passive Mode Port */

#define FTP_PASV_PORT 1010

/* FTP Server Responce */

const char msg_ftp_openning[]      = "150 Opening data connection\r\n";
const char msg_ftp_successful[]    = "200 Command successful\r\n";
const char msg_ftp_port_ok[]       = "200 PORT Command successful\r\n";
const char msg_ftp_unimplemented[] = "202 Unimplemented command\r\n";
const char msg_ftp_server_ready[]  = "220 FTP Server ready\r\n";
const char msg_ftp_goodbye[]       = "221 Goodbye\r\n";
const char msg_ftp_complete[]      = "226 Transfer complete\r\n";
const char msg_ftp_logged_in[]     = "230 Logged in\r\n";
const char msg_ftp_password[]      = "331 Password required\r\n";
const char msg_ftp_cant_open[]     = "425 Can't open data connection\r\n";
const char msg_ftp_disconnected[]  = "426 Data connection disconnected\r\n";
const char msg_ftp_file_busy[]     = "450 File busy\r\n";
const char msg_ftp_disk_full[]     = "452 Disk full\r\n";
const char msg_ftp_unrecognized[]  = "500 Unrecognized command\r\n";
const char msg_ftp_invalid_arg[]   = "501 Invalid arguments\r\n";
const char msg_ftp_unimple_arg[]   = "504 Unimplemented arguments\r\n";
const char msg_ftp_not_logged_in[] = "530 Not logged in\r\n";
const char msg_ftp_incorrect[]     = "530 Login incorrect\r\n";
const char msg_ftp_not_found[]     = "550 File not found\r\n";
const char msg_ftp_cant_write[]    = "550 Can't write file\r\n";
static B msg_ftp_enter_pasv[55];

/* FTP Command Table */

typedef B *(*FTP_COMMAND)(T_FTP *, const B *);
typedef struct t_ftp_command_table {
    char command[4+1];
    FTP_COMMAND func;
} T_FTP_COMMAND_TABLE;

static const B *ftp_user_command(T_FTP *ftp, const B *arg);
static const B *ftp_pass_command(T_FTP *ftp, const B *arg);
static const B *ftp_quit_command(T_FTP *ftp, const B *arg);
static const B *ftp_port_command(T_FTP *ftp, const B *arg);
static const B *ftp_type_command(T_FTP *ftp, const B *arg);
static const B *ftp_retr_command(T_FTP *ftp, const B *arg);
static const B *ftp_stor_command(T_FTP *ftp, const B *arg);
static const B *ftp_appe_command(T_FTP *ftp, const B *arg);
static const B *ftp_list_command(T_FTP *ftp, const B *arg);
static const B *ftp_nlst_command(T_FTP *ftp, const B *arg);
static const B *ftp_dele_command(T_FTP *ftp, const B *arg);
static const B *ftp_pasv_command(T_FTP *ftp, const B *arg);

static const T_FTP_COMMAND_TABLE ftp_command_table[] = {
    "USER", (FTP_COMMAND)ftp_user_command,
    "PASS", (FTP_COMMAND)ftp_pass_command,
    "QUIT", (FTP_COMMAND)ftp_quit_command,
    "PORT", (FTP_COMMAND)ftp_port_command,
    "TYPE", (FTP_COMMAND)ftp_type_command,
    "RETR", (FTP_COMMAND)ftp_retr_command,
    "STOR", (FTP_COMMAND)ftp_stor_command,
    "APPE", (FTP_COMMAND)ftp_appe_command,
    "LIST", (FTP_COMMAND)ftp_list_command,
    "NLST", (FTP_COMMAND)ftp_nlst_command,
    "DELE", (FTP_COMMAND)ftp_dele_command,
    "PASV", (FTP_COMMAND)ftp_pasv_command,
    "",     NULL,
};

/************************************************/
/* Put String to PI Connection                  */
/************************************************/

ER ftp_put_str(T_FTP *ftp, const B *str)
{
    INT len;
    ER ercd;

    len = strlen((char *)str);
    while (len != 0)
    {   ercd = tcp_snd_dat(ftp->cepid1, str, len, FTPD_TMOUT);
        if (ercd < 0)
            return ftp->ercd = ercd;
        len -= ercd;
        str += ercd;
    }
    return E_OK;
}

/************************************************/
/* Get String from PI Connection                */
/************************************************/

ER ftp_get_str(T_FTP *ftp, B *buf, INT bufsz)
{
    ER ercd;
    INT i;
    B c;

    for (i = 0;;)
    {
        ercd = tcp_rcv_dat(ftp->cepid1, &c, 1, FTPD_IDLE_TMOUT);
        if (ercd <= 0)
        {   if (ercd == 0)
                ercd = E_CLS;
            break;
        }
        if (c == '\r')      /* Ignore CR */
            continue;
        if (c == '\n')      /* End if LF */
        {   buf[i] = '\0';
            return E_OK;
        }
        if (i >= bufsz - 1)
            continue;
        buf[i++] = c;
    }
    buf[i] = '\0';
    if (i == 4)
        buf[++i] = '\0';
    return ftp->ercd = ercd;
}

/************************************************/
/* Send Data to DTP Connection                  */
/************************************************/

ER ftp_snd_dat(T_FTP *ftp, const B *dat, INT len)
{
    ER ercd;

    while (len > 0)
    {   ercd = tcp_snd_dat(ftp->cepid2, dat, len, FTPD_TMOUT);
        if (ercd < 0)
            return ftp->ercd = ercd;
        dat += ercd;
        len -= ercd;
    }
    return E_OK;
}

/************************************************/
/* Receive Data from DTP Connection             */
/************************************************/

ER ftp_rcv_dat(T_FTP *ftp, INT *len, B *buf, INT bufsz)
{
    ER ercd;

    for (*len = 0;;)
    {   ercd = tcp_rcv_dat(ftp->cepid2, buf, bufsz, FTPD_TMOUT);
        if (ercd < 0)
            return ftp->ercd = ercd;
        if (ercd == 0)
            return E_OK;
        *len += ercd;
        buf += ercd;
        if ((bufsz -= ercd) <= 0)
            return E_OK;
    }
}

/************************************************/
/* Receive File Data                            */
/************************************************/

const B *ftp_rcv_fil(T_FTP *ftp, FILE *fp)
{
    INT len;
    ER ercd = E_OK;
    B full = FALSE;

    if (fp == NULL)
    {   tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
        ftp->flag &= ~(FTP_STOR|FTP_DTP_OPEN);
        return NULL;
    }

    for (;;)
    {   ercd = ftp_rcv_dat(ftp, &len, ftp->buf, sizeof ftp->buf);
        if (ercd != E_OK || len == 0)
            break;
        if (!full)
        {   len -= (INT)fwrite(ftp->buf, 1, len, fp);
            if (len != 0)
                full = TRUE;
        }
    }
    fclose(ftp->fp);
    ftp->fp = NULL;
    tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
    ftp->flag &= ~(FTP_STOR|FTP_DTP_OPEN);
    if (ercd < 0)
        return (B *)msg_ftp_disconnected;
    if (full)
        return (B *)msg_ftp_disk_full;
    else
        return (B *)msg_ftp_complete;
}

/************************************************/
/* Send File Data                               */
/************************************************/

const B *ftp_snd_fil(T_FTP *ftp, FILE *fp)
{
    INT len;
    ER ercd = E_OK;

    if (fp == NULL)
    {   tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
        ftp->flag &= ~(FTP_RETR|FTP_DTP_OPEN);
        return NULL;
    }

    for (;;)
    {   len = (INT)fread(ftp->buf, 1, sizeof ftp->buf, fp);
        if (len == 0)
            break;
        ercd = ftp_snd_dat(ftp, ftp->buf, len);
        if (ercd != E_OK)
            break;
    }
    fclose(ftp->fp);
    ftp->fp = NULL;
    tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
    ftp->flag &= ~(FTP_RETR|FTP_DTP_OPEN);
    if (ercd == E_OK)
        return (B *)msg_ftp_complete;
    else
        return (B *)msg_ftp_disconnected;
}

/************************************************/
/* Wild Card Check                              */
/************************************************/

static BOOL wildcard_check(const B *w, const B *s)
{
    B c;

    if (*w == '\0' || strcmp((char *)w, (char *)"*.*") == 0)
        return TRUE;
    if (strcmp((char *)w, (char *)s) == 0)
        return TRUE;
    for (;;)
    {   c = *s++;
        if (c == '\0')
            return TRUE;
        if (c == *w)
        {   w++;
            continue;
        }
        if (c == '.')
        {   w = (B *)strchr((char *)w, '.');
            if (w == NULL)
                break;
            w++;
            continue;
        }
        if (*w == '?')
        {   w++;
            continue;
        }
        if (*w == '*')
            continue;
        break;
    }
    return FALSE;
}

/************************************************/
/* Send File List                               */
/************************************************/

const B *ftp_snd_dir(T_FTP *ftp, FILE *fp)
{
    struct _direct *dir = (struct _direct *)(ftp->buf);
    B *s = (B *)(dir + 1);
    B *filename = ftp->buf + FTP_BUFSZ - 16;
    W len;
    INT i;
    ER ercd = E_OK;

    if (fp == NULL)
    {   tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
        ftp->flag &= ~(FTP_LIST|FTP_NLST|FTP_DTP_OPEN);
        return NULL;
    }

    for (i = 0;;)
    {   len = fread(dir, 1, sizeof (struct _direct), fp);
        if (len == 0)
        {   if (ftp->flag & FTP_NLST)
            {   strcpy((char *)s, (char *)"\r\n");
                ercd = ftp_snd_dat(ftp, s, strlen((char *)s));
            }
            break;
        }
        if (dir->d_ino == 0)
            continue;
        strncpy((char *)s, dir->d_name, DIRSIZ);
        strlwr((char *)s);
        if (!wildcard_check(filename, s))
            continue;
        if (!(ftp->flag & FTP_NLST))
        {
          #ifdef DIRECT_FSIZE
            strcat((char *)s, (char *)"            ");
            strcpy((char *)s + 12, dir->fsize);
          #endif
            strcat((char *)s, (char *)"\r\n");
        }
        else
        {   strcat((char *)s, (char *)"              ");
            s[14] = '\0';
            if (++i >= 5)
            {   i = 0;
                strcat((char *)s, (char *)"\r\n");
            }
        }
        ercd = ftp_snd_dat(ftp, s, strlen((char *)s));
        if (ercd != E_OK)
            break;
    }

    fclose(ftp->fp);
    ftp->fp = NULL;
    tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
    ftp->flag &= ~(FTP_LIST|FTP_NLST|FTP_DTP_OPEN);
    if (ercd == E_OK)
        return (B *)msg_ftp_complete;
    else
        return (B *)msg_ftp_disconnected;
}

/************************************************/
/* USER Command                                 */
/************************************************/

static const B *ftp_user_command(T_FTP *ftp, const B *arg)
{
    strncpy((char *)(ftp->user), (char *)arg, sizeof ftp->user - 1);
    ftp->flag &= ~FTP_LOGGED_IN;
    return (B *)msg_ftp_password;
}

/************************************************/
/* PASS Command                                 */
/************************************************/
static const B *ftp_pass_command(T_FTP *ftp, const B *arg)
{
    FTP_CALLBACK func;
    B *s;

    func = (FTP_CALLBACK)ftp->callback;
    if (func != NULL){
        if ((s = (*func)(ftp, ftp->user, arg)) == NULL)
            return (B *)msg_ftp_incorrect;
    }
    else
        s = (B *)msg_ftp_logged_in;

    ftp->flag |= FTP_LOGGED_IN;
    return s;
}

/************************************************/
/* QUIT Command                                 */
/************************************************/

static const B *ftp_quit_command(T_FTP *ftp, const B *arg)
{
    ftp->flag &= ~FTP_LOGGED_IN;
    ftp->flag |= FTP_QUIT;
    return (B *)msg_ftp_goodbye;
}

/************************************************/
/* PORT Command                                 */
/************************************************/

static const B *ftp_port_command(T_FTP *ftp, const B *arg)
{
    T_IPV4EP myaddr;
    T_IPV4EP dstaddr;
    ER ercd;
    INT i;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    if (ftp->flag & FTP_DTP_OPEN)
    {   ftp->flag &= ~FTP_DTP_OPEN;
        ercd = tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
        if (ercd != E_OK)
        {   ftp->ercd = ercd;
            return (B *)msg_ftp_cant_open;
        }
    }

    dstaddr.ipaddr = 0L;
    dstaddr.portno = 0;
    for (i = 0;;)
    {   if (i < 4)
            dstaddr.ipaddr = (dstaddr.ipaddr << 8) + atoi((char *)arg);
        else
            dstaddr.portno = (dstaddr.portno << 8) + atoi((char *)arg);
        if (++i == 6)
            break;
        arg = (B *)strchr((char *)arg, ',');
        if (arg == NULL)
            return (B *)msg_ftp_invalid_arg;
        arg++;
    }

    myaddr.ipaddr = IPV4_ADDRANY;
    myaddr.portno = TCP_PORT_FTPDATA;

    ercd = tcp_con_cep(ftp->cepid2, &myaddr, &dstaddr, FTPD_TMOUT);
    if (ercd != E_OK)
    {   ftp->ercd = ercd;
        return (B *)msg_ftp_cant_open;
    }

    ftp->flag |= FTP_DTP_OPEN;
    return (B *)msg_ftp_port_ok;
}

/************************************************/
/* PASV Command                                 */
/************************************************/

static const B *ftp_pasv_command(T_FTP *ftp, const B *arg)
{
    static T_IPV4EP dstaddr;
    ER ercd;
    INT i;
    UH port;
    char buf[5];

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    if (ftp->flag & FTP_DTP_OPEN)
    {   ftp->flag &= ~FTP_DTP_OPEN;
        ercd = tcp_cls_cep(ftp->cepid2, FTPD_TMOUT);
        if (ercd != E_OK)
        {   ftp->ercd = ercd;
            return (B *)msg_ftp_cant_open;
        }
    }

    ercd = tcp_acp_cep(ftp->cepid2, ftp->repid2, &dstaddr, TMO_NBLK);
    if (ercd != E_WBLK)
    {   ftp->ercd = ercd;
        return (B *)msg_ftp_cant_open;
    }
    strcpy((char *)msg_ftp_enter_pasv, (char *)"227 Entering Passive Mode ");
    strcat((char *)msg_ftp_enter_pasv, (char *)"(");
    for (i=0; i<4; i++){
        itou(buf, default_ipaddr[i], itoulen(default_ipaddr[i]));
        strcat((char *)msg_ftp_enter_pasv, buf);
        strcat((char *)msg_ftp_enter_pasv, (char *)",");
    }
    port = (FTP_PASV_PORT>>8);
    itou(buf, port, itoulen(port));
    strcat((char *)msg_ftp_enter_pasv, buf);
    strcat((char *)msg_ftp_enter_pasv, (char *)",");
    port = (FTP_PASV_PORT&0xFF);
    itou(buf, port, itoulen(port));
    strcat((char *)msg_ftp_enter_pasv, buf);
    strcat((char *)msg_ftp_enter_pasv, (char *)")\r\n");

    ftp->flag |= FTP_DTP_OPEN;
    return msg_ftp_enter_pasv;
}


/************************************************/
/* TYPE Command                                 */
/************************************************/

static const B *ftp_type_command(T_FTP *ftp, const B *arg)
{
    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    if (arg[0] == 'I') /* Image */
        return (B *)msg_ftp_successful;
    else
        return (B *)msg_ftp_unimple_arg;
}

/************************************************/
/* RETR Command                                 */
/************************************************/

static const B *ftp_retr_command(T_FTP *ftp, const B *arg)
{
    ftp->flag |= FTP_RETR;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    ftp->fp = fopen((const char *)arg, (char *)"r");
    if (ftp->fp == NULL)
        return (B *)msg_ftp_not_found;
    else
        return (B *)msg_ftp_openning;
}

/************************************************/
/* STOR Command                                 */
/************************************************/

static const B *ftp_stor_command(T_FTP *ftp, const B *arg)
{
    ftp->flag |= FTP_STOR;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    ftp->fp = fopen((const char *)arg, (char *)"w");
    if (ftp->fp == NULL)
        return (B *)msg_ftp_cant_write;
    else
        return (B *)msg_ftp_openning;
}

/************************************************/
/* APPE Command                                 */
/************************************************/

static const B *ftp_appe_command(T_FTP *ftp, const B *arg)
{
    ftp->flag |= FTP_STOR;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    ftp->fp = fopen((const char *)arg, (char *)"a");
    if (ftp->fp == NULL)
        return (B *)msg_ftp_cant_write;
    else
        return (B *)msg_ftp_openning;
}

/************************************************/
/* LIST Command                                 */
/************************************************/

static const B *ftp_list_command(T_FTP *ftp, const B *arg)
{
    B *filename = ftp->buf + FTP_BUFSZ - 16;

    ftp->flag |= FTP_LIST;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    strncpy((char *)filename, (char *)arg, 16);
    ftp->fp = fopen((char *)".", (char *)"r");

    if (ftp->fp == NULL)
        return (B *)msg_ftp_not_found;
    else
        return (B *)msg_ftp_openning;
}

static const B *ftp_nlst_command(T_FTP *ftp, const B *arg)
{
    if (strcmp((char *)arg, (char *)"-l") != 0)
        ftp->flag |= FTP_NLST;
    return ftp_list_command(ftp, (B *)"");
}

/************************************************/
/* DELE Command                                 */
/************************************************/

static const B *ftp_dele_command(T_FTP *ftp, const B *arg)
{
    int r;

    if (!(ftp->flag & FTP_LOGGED_IN))
        return (B *)msg_ftp_not_logged_in;

    r = remove((const char *)arg);
    if (r != 0)
        return (B *)msg_ftp_not_found;
    else
        return (B *)msg_ftp_successful;
}

/************************************************/
/* FTP Server Task                              */
/************************************************/

TASK ftp_server_tsk(T_FTP *ftp)
{
    const T_FTP_COMMAND_TABLE *t;
    const B *s;

    for (;;)
    {
        /* Accept Connection */

        while (tcp_acp_cep(ftp->cepid1, ftp->repid, &ftp->dstaddr, TMO_FEVR) != E_OK)
            dly_tsk(1000/MSEC);

        s = (B *)msg_ftp_server_ready;

        /* FTP Process Loop */

        for (;;)
        {
            /* Send FTP Response */

            if (s != NULL)
            {   if (ftp_put_str(ftp, s) != E_OK)
                    break;
            }

            /* Receive FTP Command */

            memset(ftp->buf, 0, 4 + 1 + 1);
            if (ftp_get_str(ftp, ftp->buf, sizeof ftp->buf) != E_OK)
                break;

            /* Jump Each Command */

            for (t = ftp_command_table; t->func != NULL; t++)
            {   if (memcmp(t->command, ftp->buf, 4) == 0)
                    break;
            }
            if (t->func != NULL)
            {   s = (B *)strchr((char *)(ftp->buf), ' ');
                if (s == NULL)
                    s = (*t->func)(ftp, (B *)"");
                else
                    s = (*t->func)(ftp, s + 1);
            }
            else
                s = (B *)msg_ftp_unimplemented;

            /* Send FTP Response */

            if (s != NULL)
            {   if (ftp_put_str(ftp, s) != E_OK)
                    break;
            }

            /* Quit or File Transfer */

            if (ftp->flag & FTP_QUIT)
            {   while (ftp_get_str(ftp, ftp->buf, sizeof ftp->buf) > 0)
                    ;
                break;
            }
            else if ((ftp->flag & FTP_STOR) && (ftp->flag & FTP_DTP_OPEN))
                s = ftp_rcv_fil(ftp, ftp->fp);
            else if ((ftp->flag & FTP_RETR) && (ftp->flag & FTP_DTP_OPEN))
                s = ftp_snd_fil(ftp, ftp->fp);
            else if ((ftp->flag & FTP_LIST) && (ftp->flag & FTP_DTP_OPEN))
                s = ftp_snd_dir(ftp, ftp->fp);
            else
                s = NULL;
        }

        /* Close Connection */

        tcp_cls_cep(ftp->cepid1, FTPD_TMOUT);
        ftp->flag = 0;
    }
}

/************************************************/
/* Start FTP Server                             */
/************************************************/

ER ftp_sta_srv(T_FTP *ftp)
{
    return vsta_tsk(ftp->tskid, (VP)ftp);
}

/************************************************/
/* Create FTP Server                            */
/************************************************/

ER ftp_cre_srv(T_FTP *ftp, ID tskid, ID cepid1, ID cepid2, ID repid, FTP_CALLBACK callback)
{
    static const T_TCP_CREP crep = { 0, { IPV4_ADDRANY, TCP_PORT_FTP } };
    static const T_TCP_CREP crep2 = { 0, { IPV4_ADDRANY, FTP_PASV_PORT } };
    T_TCP_CCEP ccep;
    ER ercd;

    /* FTP Control Block Initialize */

    memset(ftp, 0, sizeof (T_FTP));
    ftp->tskid = tskid;
    ftp->cepid1 = cepid1;
    ftp->cepid2 = cepid2;
    ftp->repid = repid;
    ftp->callback = (FP)callback;

    /* Create TCP Commnunication End Points */

    memset(&ccep, 0, sizeof (T_TCP_CCEP));
    ccep.sbuf = ftp->sbuf1;
    ccep.sbufsz = sizeof ftp->sbuf1;
    ccep.rbuf = ftp->rbuf1;
    ccep.rbufsz = sizeof ftp->rbuf1;

    if(ftp->cepid1 == 0)
    {   ercd = ftp->cepid1 = tcp_vcre_cep(&ccep);
        if(ftp->cepid1 <= 0)
            return ercd;
    }
    else
    {   ercd = tcp_cre_cep(ftp->cepid1, &ccep);
        if (ercd != E_OK)
            return ercd;
    }

    ccep.sbuf = ftp->sbuf2;
    ccep.sbufsz = sizeof ftp->sbuf2;
    ccep.rbuf = ftp->rbuf2;
    ccep.rbufsz = sizeof ftp->rbuf2;

    if(ftp->cepid2 == 0)
    {   ercd = ftp->cepid2 = tcp_vcre_cep(&ccep);
        if(ftp->cepid2 <= 0)
            return ercd;
    }
    else
    {   ercd = tcp_cre_cep(ftp->cepid2, &ccep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create TCP Reception End Point */

    if(ftp->repid == 0)
    {   ercd = ftp->repid = tcp_vcre_rep(&crep);
        if(ftp->repid <= 0)
            return ercd;
    }
    else
    {
        ercd = tcp_cre_rep(ftp->repid, &crep);
        if (ercd != E_OK && ercd != E_OBJ)
            return ercd;
    }

    /* Create TCP Reception End Point for Passive mode */

    ercd = ftp->repid2 = tcp_vcre_rep(&crep2);
    if (ercd <= 0)
        return ercd;

    /* Create FTP Server Task */

    if(ftp->tskid == 0){
        ercd = vcre_tsk(&ftp_server_ctsk);
        if (ercd < 0)
            return ercd;
        ftp->tskid = ercd;
    }
    else{
        ercd = cre_tsk(ftp->tskid, &ftp_server_ctsk);
        if (ercd != E_OK)
            return ercd;
    }

    return E_OK;
}

/* end */
