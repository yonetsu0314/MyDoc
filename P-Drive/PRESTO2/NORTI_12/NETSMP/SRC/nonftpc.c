/******************************************************************************
* NORTi Network FTP Client Sample                                             *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 30/May/1999                                                                 *
* 12/Jun/1999 -d and -v option is added.                                      *
* 26/Jun/1999 responce receiving is modified.                                 *
* 23/Sep/1999 CEP,REP ID auto assign added.(Create tcp_vcre_cep,tcp_vcre_rep) *
* 06/Aug/2000 Added dly_tsk for WinNT                                         *
* 10/Feb/2000 Modified ftp_get_command for (No such file or directory)        *
* 08/Feb/2002 Changed literal string type(B->char) for CC32R               OK *
*             written 'B str[] = "?";' then it is error in CC32R              *
* 28/Apr/2003 Added quit command                                           YS *
* 28/Apr/2003 Modified ftp_dir_command, ftp_get_command and ftp_put_command   *
              connection waiting from server                               YS *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonitod.h"
#include "nonfile.h"
#include "nonteln.h"
#include "nonftp.h"

extern const ID ID_FTPC_CEP1;
extern const ID ID_FTPC_CEP2;
extern const ID ID_FTPC_REP;
extern T_FTP ftpc;
extern T_TERMINAL *current_terminal;

/* Message String */

const char msg_ftpc_addr[] = "Invalid IP address.\r\n";
const char msg_ftpc_help[] = "Usage: ftp [-v] [-d] host\r\n"
                          "host  Host IP address\r\n"
                          "-v    Suppresses Display of Daemon Responses\r\n"
                          "-d    Enables Debugging\r\n";

/* Constant */

#define FTP_TMOUT   (4*15000/MSEC)
#define FTP_TMOUT2  (500/MSEC)

/* Type */

#define ER_LEN      ER

/************************************************/
/* Receive FTP Response Subroutine              */
/************************************************/

static ER_LEN ftp_rcv_rsp(T_FTP *ftp, B *buf, INT bufsz, TMO tmout)
{
    ER ercd;
    int i, j;

    i = j = 0;
    do {
        ercd = tcp_rcv_dat(ftp->cepid1, buf + i, 1, tmout);
        if (tmout == FTP_TMOUT2 && ercd == E_TMOUT)
            return j;
        if (ercd <= 0)
            return ercd;
        tmout = FTP_TMOUT2;
        i += ercd;
        if (buf[i-1] == '\n') {
            if (!(ftp->flag & FTP_OPTION_V))
            {   buf[i] = '\0';
                print(buf);
            }
            if (memcmp(buf, "220", 3) == 0)
                ftp->flag |= FTP_READY;
            else if (memcmp(buf, "331", 3) == 0)
                ftp->flag |= FTP_PASSWD_REQ;
            else if (memcmp(buf, "230", 3) == 0)
                ftp->flag |= FTP_LOGGED_IN;
            else if (memcmp(buf, "221", 3) == 0)
                ftp->flag |= FTP_CLOSED;
            else if (memcmp(buf, "150", 3) == 0)
                return i;
            else if (memcmp(buf, "226", 3) == 0)
                return i;
            else if (memcmp(buf, "200", 3) == 0)
                return i;
            j += i;
            i = 0;
        }
    } while (i < bufsz);
    return i;
}

/************************************************/
/* Send FTP Command Subroutine                  */
/************************************************/

static ER_LEN ftp_snd_cmd(T_FTP *ftp, B *buf, INT len)
{
    TMO tmout;
    ER ercd;
    int i;

    if (ftp->flag & FTP_OPTION_D) {
        print((B *)"---> ");
        print(buf);
    }
    i = 0;
    tmout = FTP_TMOUT;
    do {
        ercd = tcp_snd_dat(ftp->cepid1, buf + i, len - i, tmout);
        if (ercd <= 0)
            return ercd;
        i += ercd;
    } while (i < len);
    return i;
}

/************************************************/
/* Send FTP Command and Reveice Response        */
/************************************************/

static BOOL ftp_snd_rcv(T_FTP *ftp, B *buf, INT len, INT bufsz)
{
    ER_LEN ercd;

    if (len > 0) {
        ercd = ftp_snd_cmd(ftp, buf, len);
        if (ercd <= 0)
            return FALSE;
    }
    ercd = ftp_rcv_rsp(ftp, buf, bufsz, FTP_TMOUT);
    if (ercd <= 0)
        return FALSE;
    return TRUE;
}

/************************************************/
/* FTP Client Commands                          */
/************************************************/

BOOL ftp_wait_ready(T_FTP *ftp)
{
    if (ftp->flag & FTP_READY)
        return TRUE;
    if (!ftp_snd_rcv(ftp, ftp->buf, 0, sizeof ftp->buf))
        return FALSE;
    if (ftp->flag & FTP_READY)
        return TRUE;
    else
        return FALSE;
}

BOOL ftp_wait_close(T_FTP *ftp)
{
    return ftp_snd_rcv(ftp, ftp->buf, 0, sizeof ftp->buf);
}

BOOL ftp_etc_command(T_FTP *ftp, B *s)
{
    INT len;

    strcpy((char *)(ftp->buf), (char *)s);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_user_command(T_FTP *ftp, B *s)
{
    INT len;

    strcpy((char *)(ftp->buf), (char *)"USER ");
    if (s[0] == '\0')
        strcat((char *)(ftp->buf), (char *)"(none)");
    else
        strcat((char *)(ftp->buf), (char *)s);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_pass_command(T_FTP *ftp, B *s)
{
    INT len;

    ftp->flag &= ~FTP_PASSWD_REQ;

    strcpy((char *)(ftp->buf), (char *)"PASS ");
    strcat((char *)(ftp->buf), (char *)s);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_cd_command(T_FTP *ftp, INT argc, B *argv[])
{
    INT len;

    if (argc < 2) {
        puts((B *)"cd <remote-directory>");
        return TRUE;
    }
    strcpy((char *)(ftp->buf), (char *)"CWD ");
    strcat((char *)(ftp->buf), (char *)argv[1]);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_rm_command(T_FTP *ftp, INT argc, B *argv[])
{
    INT len;

    if (argc < 2) {
        puts((B *)"rm <remote-filename>");
        return TRUE;
    }
    strcpy((char *)(ftp->buf), (char *)"DELE ");
    strcat((char *)(ftp->buf), (char *)argv[1]);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_port_command(T_FTP *ftp)
{
    T_TCP_CREP crep;
    ER ercd;
    INT len;
    INT i, j;
    H d;
    B *s;

    memset(&crep, 0, sizeof (T_TCP_CREP));
    crep.myaddr.ipaddr = byte4_to_long(default_ipaddr);
    crep.myaddr.portno = tcp_portany();

    for (i = 10; ; i--) {
        if(ftp->repid == 0)
        {   ftp->repid = ercd = tcp_vcre_rep(&crep);
            if(ercd > 0)
                break;
        }
        else
        {   ercd = tcp_cre_rep(ftp->repid, &crep);
            if (ercd == E_OK)
                break;
        }
        if (ercd != E_OBJ || i == 0)
            return TRUE;
        crep.myaddr.portno = TCP_PORTANY;
    }
    ftp->myaddr = crep.myaddr;

    ftp->dstaddr.ipaddr = 0L;
    ercd = tcp_acp_cep(ftp->cepid2, ftp->repid, &ftp->dstaddr, TMO_NBLK);
    if (ercd != E_WBLK) {
        tcp_del_rep(ftp->repid);
        return FALSE;
    }

    strcpy((char *)(ftp->buf), (char *)"PORT ");
    len = 5;
    s = ftp->buf + len;
    for (i = 0; i < 6; i++) {
        if (i < 4)
            d = (UH)((ftp->myaddr.ipaddr >> (8 * (3 - i))) & 0xff);
        else
            d = (UH)((ftp->myaddr.portno >> (8 * (5 - i))) & 0xff);
        if (i != 0) {
            *s++ = ',';
            len++;
        }
        j = itoulen(d);
        itou((char *)s, d, j);
        s += j;
        len += j;
    }
    memcpy(s, "\r\n", 2 + 1);
    len += 2;
    return ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf);
}

BOOL ftp_dir_command(T_FTP *ftp, INT argc, B *argv[])
{
    ER_LEN len;
    INT i;
    TMO tmout;

    if (!ftp_port_command(ftp))
        return FALSE;

    if (strcmp((char *)argv[0], (char *)"dir") == 0)
        strcpy((char *)(ftp->buf), (char *)"LIST");
    else /* ls */
        strcpy((char *)(ftp->buf), (char *)"NLST");
    for (i = 1; i < argc; i++)  {
        strcat((char *)(ftp->buf), (char *)" ");
        strcat((char *)(ftp->buf), (char *)argv[i]);
    }
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    if (!ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf))
        return FALSE;

    for (i = 5; ftp->dstaddr.ipaddr == 0L;) {
        dly_tsk(1000/MSEC);
        if (--i == 0) {
            tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
            tcp_del_rep(ftp->repid);
            return TRUE;
        }
    }
    ftp->flag |= FTP_DTP_OPEN;

    i = 0;
    tmout = FTP_TMOUT;
    for (;;) {
        len = tcp_rcv_dat(ftp->cepid2, ftp->buf + i, 1, tmout);
        if (len <= 0)
            break;
        i += len;
        if (ftp->buf[i-1] == '\n') {
            if (i >= 2 && ftp->buf[i-2] == '\r')
                ftp->buf[i] = '\0';
            else
            {   ftp->buf[i-1] = '\r';
                ftp->buf[i] = '\n';
                ftp->buf[i+1] = '\0';
            }
            print(ftp->buf);
            i = 0;
        }
        else if (i >= sizeof ftp->buf - 1)
        {   ftp->buf[i] = '\0';
            print(ftp->buf);
            i = 0;
        }
    }
    ftp->flag &= ~FTP_DTP_OPEN;
    tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
    tcp_del_rep(ftp->repid);

    return ftp_snd_rcv(ftp, ftp->buf, 0, sizeof ftp->buf);
}

BOOL ftp_get_command(T_FTP *ftp, INT argc, B *argv[])
{
    ER_LEN len;
    FILE *fp;
    B full;
    UW total;
    INT i;

    if (argc < 2) {
        puts((B *)"get <remote-filename>");
        return TRUE;
    }

    if (!ftp_port_command(ftp))
        return FALSE;

    strcpy((char *)(ftp->buf), (char *)"RETR ");
    strcat((char *)(ftp->buf), (char *)argv[1]);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    if (!ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf)){
        tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
        tcp_del_rep(ftp->repid);
        return FALSE;
    }

    for (i = 5; ftp->dstaddr.ipaddr == 0L;) {
        dly_tsk(1000/MSEC);
        if (--i == 0) {
            tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
            tcp_del_rep(ftp->repid);
            return TRUE;
        }
    }
    ftp->flag |= FTP_DTP_OPEN;
    fp = fopen((const char *)argv[1], (char *)"w");
    if (fp == NULL) {
        puts((B *)"Can't write file.");
        tcp_cls_cep(ftp->cepid2, 1);
        tcp_del_rep(ftp->repid);
        puts((B *)"Connection closed.");
        return FALSE;
    }

    total = 0L;
    full = FALSE;
    for (;;) {
        len = tcp_rcv_dat(ftp->cepid2, ftp->buf, sizeof ftp->buf, FTP_TMOUT);
        if (len <= 0)
            break;
        total += len;
        if (!full) {
            if ((INT)fwrite(ftp->buf, 1, len, fp) != len) {
                puts((B *)"Disk full");
                full = TRUE;
            }
        }
    }
    fclose(fp);
    ftp->flag &= ~FTP_DTP_OPEN;
    tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
    tcp_del_rep(ftp->repid);

    if (!ftp_snd_rcv(ftp, ftp->buf, 0, sizeof ftp->buf))
        return FALSE;

    ltou((char *)(ftp->buf), total, ltoulen(total));
    strcat((char *)(ftp->buf), (char *)" bytes received");
    puts(ftp->buf);
    return TRUE;
}

BOOL ftp_put_command(T_FTP *ftp, INT argc, B *argv[])
{
    ER_LEN len, len2;
    INT i;
    FILE *fp;
    UW total;

    if (argc < 2) {
        puts((B *)"put <remote-filename>");
        return TRUE;
    }

    fp = fopen((const char *)argv[1], (char *)"r");
    if (fp == NULL) {
        puts((B *)"File not found");
        return TRUE;
    }

    if (!ftp_port_command(ftp)) {
        fclose(fp);
        return FALSE;
    }

    strcpy((char *)(ftp->buf), (char *)"STOR ");
    strcat((char *)(ftp->buf), (char *)argv[1]);
    strcat((char *)(ftp->buf), (char *)"\r\n");
    len = strlen((char *)(ftp->buf));
    if (!ftp_snd_rcv(ftp, ftp->buf, len, sizeof ftp->buf)){
        fclose(fp);
        return FALSE;
    }

    for (i = 5; ftp->dstaddr.ipaddr == 0L;) {
        dly_tsk(1000/MSEC);
        if (--i == 0) {
            tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
            tcp_del_rep(ftp->repid);
            fclose(fp);
            return TRUE;
        }
    }
    ftp->flag |= FTP_DTP_OPEN;

    total = 0L;
    for (;;) {
        len = fread(ftp->buf, 1, sizeof ftp->buf, fp);
        if (len == 0)
            break;
        total += len;
        i = 0;
        do {
            len2 = tcp_snd_dat(ftp->cepid2, ftp->buf + i, len, FTP_TMOUT);
            if (len2 <= 0)
                break;
            i += len2;
            len -= len2;
        } while (len > 0);
    }
    fclose(fp);
    ftp->flag &= ~FTP_DTP_OPEN;
    tcp_cls_cep(ftp->cepid2, FTP_TMOUT);
    tcp_del_rep(ftp->repid);

    ltou((char *)(ftp->buf), total, ltoulen(total));
    strcat((char *)(ftp->buf), (char *)" bytes sent");
    puts(ftp->buf);

    return ftp_snd_rcv(ftp, ftp->buf, 0, sizeof ftp->buf);
}

/************************************************/
/* Convert String to argc, argv                 */
/************************************************/

static INT str_to_argc_argv(B *s, B *argv[], INT maxargc)
{
    INT i, argc;

    for (argc = 0; argc < maxargc; ) {
        while (*s == ' ')
            s++;
        if (*s == '\0')
            break;
        argv[argc++] = s++;
        while (*s != ' ') {
            if (*s == '\0')
                goto END;
            s++;
        }
        *s++ = '\0';
    }
END:
    for (i = argc; i < maxargc; i++)
        argv[i] = s; /* '\0' */
    return argc;
}

/************************************************/
/* Input FTP Command                            */
/************************************************/

static INT ftp_input(T_FTP *ftp, B *buf, INT size)
{
    T_TERMINAL *t;
    INT c;

    t = current_terminal;
    if (t == NULL)
        return FALSE;

    if (ftp->flag & FTP_PASSWD_REQ)
        t->echo_on = '*';
    c = terminal_input(t, buf, size);
    if (ftp->flag & FTP_PASSWD_REQ)
        t->echo_on = 1;
    terminal_print(t, (B *)"\r\n");
    if (c <= 0)
        return FALSE;
    return c;
}

/************************************************/
/* FTP Client Process                           */
/************************************************/

ER ftp_client(T_FTP *ftp, ID cepid1, ID cepid2, ID repid, UB option, UW ipaddr)
{
    T_TCP_CCEP ccep;
    T_IPV4EP myaddr;
    T_IPV4EP dstaddr;
    ER ercd;
    BOOL ok;
    INT argc;
    B *argv[4];
    B *s;
    INT i;

    /* FTP Control Block Initialize */

    memset(ftp, 0, sizeof (T_FTP));
    ftp->cepid1 = cepid1;
    ftp->cepid2 = cepid2;
    ftp->repid = repid;
    ftp->flag = option;

    /* Create TCP Commnunication End Points */

    memset(&ccep, 0, sizeof (T_TCP_CCEP));
    ccep.sbuf = ftp->sbuf1;
    ccep.sbufsz = sizeof ftp->sbuf1;
    ccep.rbuf = ftp->rbuf1;
    ccep.rbufsz = sizeof ftp->rbuf1;

    if(ftp->cepid1 == 0)
    {   ercd = ftp->cepid1 = tcp_vcre_cep(&ccep);
        if (ftp->cepid1 <= 0)
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
        if (ftp->cepid2 <= 0)
            goto END1;
    }
    else
    {   ercd = tcp_cre_cep(ftp->cepid2, &ccep);
        if (ercd != E_OK)
            goto END1;
    }

    /* Connect */

    myaddr.ipaddr = IPV4_ADDRANY;
    myaddr.portno = TCP_PORTANY;
    dstaddr.ipaddr = ipaddr;
    dstaddr.portno = TCP_PORT_FTP;

    for (i = 10; ; i--) {
        ercd = tcp_con_cep(ftp->cepid1, &myaddr, &dstaddr, FTP_TMOUT);
        if (ercd == E_OK)
            break;
        if (ercd != E_OBJ || i == 0) {
            puts((B *)"TCP connection error!");
            goto END2;
        }
        myaddr.portno = TCP_PORTANY;
    }

    if (!ftp_wait_ready(ftp)) {
        puts((B *)"Server not ready!");
        goto END;
    }

    /* Main Loop */

    for (;;) {

        /* Display Prompt */

        if (ftp->flag & FTP_PASSWD_REQ)
            print((B *)"Password: ");
        else if (!(ftp->flag & FTP_LOGGED_IN))
            print((B *)"login: ");
        else
            print((B *)"ftp> ");

        /* Input a Command */

        s = ftp->buf + sizeof ftp->buf - 128;
        if (ftp_input(ftp, s, 128) != '\r')
            break;
        argc = str_to_argc_argv(s, argv, sizeof argv / sizeof (B *));

        /* Each Command Execution */

        if (argc == 0 && (ftp->flag & FTP_LOGGED_IN))
            continue;
        else if (ftp->flag & FTP_PASSWD_REQ)
            ok = ftp_pass_command(ftp, argv[0]);
        else if (!(ftp->flag & FTP_LOGGED_IN))
            ok = ftp_user_command(ftp, argv[0]);
        else if (strcmp((char *)argv[0], (char *)"dir") == 0)
            ok = ftp_dir_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"ls") == 0)
            ok = ftp_dir_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"cd") == 0)
            ok = ftp_cd_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"rm") == 0)
            ok = ftp_rm_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"get") == 0)
            ok = ftp_get_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"put") == 0)
            ok = ftp_put_command(ftp, argc, argv);
        else if (strcmp((char *)argv[0], (char *)"bye") == 0)
            ok = ftp_etc_command(ftp, (B *)"QUIT");
        else if (strcmp((char *)argv[0], (char *)"quit") == 0)
            ok = ftp_etc_command(ftp, (B *)"QUIT");
        else if (strcmp((char *)argv[0], (char *)"ascii") == 0)
            ok = ftp_etc_command(ftp, (B *)"TYPE A");
        else if (strcmp((char *)argv[0], (char *)"bin") == 0)
            ok = ftp_etc_command(ftp, (B *)"TYPE I");
        else {
            puts((B *)"Invalid command");
            ok = TRUE;
        }
        if (!ok)
            break;
        if (ftp->flag & FTP_CLOSED) {
            ftp_wait_close(ftp);
            break;
        }
    }

    /* End Process */
END:
    tcp_cls_cep(ftp->cepid1, FTP_TMOUT);
END2:
    tcp_del_cep(ftp->cepid2);
END1:
    tcp_del_cep(ftp->cepid1);
    return ercd;
}

/************************************************/
/* FTP Command Main                             */
/************************************************/

int ftp_command(int argc, char *argv[])
{
    UW ipaddr = 0L;
    UB option;
    INT i;
    B c;

    /* Arguments Convert */

    option = 0;
    for (i = 1; i < argc; i++) {
        c = argv[i][0];
        if (c >= '0' && c <= '9')
            ipaddr = ascii_to_ipaddr((B *)argv[i]);
        else if (argv[i][1] == 'd')
            option |= FTP_OPTION_D;
        else if (argv[i][1] == 'v')
            option |= FTP_OPTION_V;
    }

    if (ipaddr == 0L) {
        if (argc < 2)
           print((B *)msg_ftpc_help);
        else
           print((B *)msg_ftpc_addr);
        return 0;
    }

    /* FTP Client Process */

    ftp_client(&ftpc, 0, 0, 0, option, ipaddr);

    return 0;
}

/* end */
