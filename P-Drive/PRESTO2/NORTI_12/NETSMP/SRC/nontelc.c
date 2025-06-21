/******************************************************************************
* NORTi Network TELNET Client Sample                                          *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 20/Jun/1999                                                                 *
* 23/Sep/1999 CEP,REP ID auto assign added. (Create tcp_vcre_cep,tcp_vcre_rep)*
* 22/Jan/2001 Change Timeout tcp_rcv_dat, 3000Å®100MSEC                       *
* 14/May/2003 Added reply DONT for all WILL options (except ECHO)          PK *
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

extern const ID ID_TELNET_CEP;

/* Window Buffer */

#define WINDOW_SIZE 100
static UH telnet_swin[WINDOW_SIZE/2];  /* Sending Window Buffer */
static UH telnet_rwin[WINDOW_SIZE/2];  /* Receiving Window Buffer */

/* Creation Information */

const T_TCP_CCEP telnet_ccep = { 0, telnet_swin, sizeof telnet_swin, telnet_rwin, sizeof telnet_rwin, (FP)NULL };

/* Local Data */

#define TELNET_BUFSZ      1024
static UH telnet_buf[TELNET_BUFSZ/2];   /* UH and /2 for Word Aglinement */

/* Constant */

#define TELNET_TMOUT   (5000/MSEC)

/************************************************/
/* Input IP Address                             */
/************************************************/

UW input_ipaddr(B *prompt)
{
    static UB d[4];
    B buf[16];
    B *p;
    int i, len;
    UW ipaddr;

    for (;;)
    {
        for (;;)
        {
            p = buf;
            for (i = 0; i < 4; i++)
            {   itou((char *)p, d[i], itoulen(d[i])); p += strlen((char *)p);
                if (i != 3)
                    strcat((char *)p++, (char *)".");
            }
            print(prompt);
            print(buf);
            len = strlen((char *)buf);
            while (len-- > 0)
                print((B *)"\b");
            input(buf, sizeof buf);
            puts((B *)"");
            if (buf[0] == '\0')
                break;
            p = buf;
            for (i = 0; i < 4; i++)
            {   d[i] = (UB)atoi((char *)p);
                if (i != 3)
                {   p = (B *)strchr((char *)p, '.');
                    if (p == NULL)
                        break;
                    p++;
                }
            }
            if (i == 4)
                break;
        }
        ipaddr = byte4_to_long(d);
        if (ipaddr != 0L)
            break;
    }
    return ipaddr;
}

/************************************************/
/* TELNET Client Process                        */
/************************************************/

ER telnet_client(ID cepid, UW ipaddr)
{
    B *buf = (B *)telnet_buf;
    int bufsz = sizeof telnet_buf;
  #if 0
    B s[16];
  #endif
    INT len, i, j, c;
    ER ercd;
    T_IPV4EP myaddr;
    T_IPV4EP dstaddr;

    if(cepid == 0)
    {   ercd = cepid = tcp_vcre_cep(&telnet_ccep);
        if(cepid <= 0){
            puts((B *)"TCP Communication End Point can't be created!");
            return ercd;
        }
    }
    else
    {   ercd = tcp_cre_cep(cepid, &telnet_ccep);
        if (ercd != E_OK) {
            puts((B *)"TCP Communication End Point can't be created!");
            return ercd;
        }
    }

    myaddr.ipaddr = IPV4_ADDRANY;
    myaddr.portno = TCP_PORTANY;
    dstaddr.ipaddr = ipaddr;
    dstaddr.portno = TCP_PORT_TELNET;

    ercd = tcp_con_cep(cepid, &myaddr, &dstaddr, TELNET_TMOUT);
    if (ercd != E_OK) {
        puts((B *)"TCP connection error!");
        goto L1;
    }
    puts((B *)"connected");

    tcp_snd_dat(cepid, "\xff""\xfc""\x01", 3, TELNET_TMOUT); /* <IAC><WONT><Echo> */

    for (;;) {
        if (kbhit()) {
            c = getch();
            if (c == 0x03) { /* CTRL-C */
                print((B *)"\r\n");
                break;
            }
            if (c == '\r')
                len = tcp_snd_dat(cepid, "\r\n", 2, TELNET_TMOUT);
            else {
                buf[0] = (B)c;
                len = tcp_snd_dat(cepid, buf, 1, TELNET_TMOUT);
            }
            if (len <= 0)
                break;
        }
        len = tcp_rcv_dat(cepid, buf, bufsz, 100/MSEC);
        if (len == E_TMOUT)
            continue;
        if (len <= 0)
            break;

        for (i = 0; i < len;) {
            c = (UB)buf[i++];
            if (c != 0xff) {
                putch(c);
                continue;
            }
            for (j = 0; j < 3; j++) {
                if (j != 0) c = (UB)buf[i++];
              #if 0
                switch (c) {
                case 0xfb: strcpy(s, "<WILL>"); break;
                case 0xfc: strcpy(s, "<WONT>"); break;
                case 0xfd: strcpy(s, "<DO>");   break;
                case 0xfe: strcpy(s, "<DONT>"); break;
                case 0x01: strcpy(s, "<Echo>"); break;
                case 0x03: strcpy(s, "<SGA>");  break;
                case 0x18: strcpy(s, "<Type>"); break;
                default: s[0] = '<'; ito0x(s + 1, c, 2); s[3] = '>'; s[4] = '\0'; break;
                }
                print(s);
              #endif
            }
            if ((UB)buf[i-2] == 0xfd) {
                buf[i-2] = (B)0xfc; /* All <DO> -> <WONT> */
                j = tcp_snd_dat(cepid, &buf[i-3], 3, TELNET_TMOUT);
                if (j <= 0)
                    break;
            }
            /* Added by PK on 14/05/03 */
            if ((UB)buf[i-2] == 0xfb && (UB)buf[i-1] != (B)0x01) {
                    buf[i-2] = (B)0xfe; /* All <WILL> -> <DONT> */
                j = tcp_snd_dat(cepid, &buf[i-3], 3, TELNET_TMOUT);
                if (j <= 0)
                    break;
            }
        }
    }

    ercd = tcp_cls_cep(cepid, TELNET_TMOUT);
    if (ercd != E_OK)
        puts((B *)"TCP closing error!");
L1:
    tcp_del_cep(cepid);
    return E_OK;
}

/************************************************/
/* TELNET Command                               */
/************************************************/

int telnet_command(int argc, char *argv[])
{
    UW ipaddr = 0L;
    int i, c;

    /* Arguments Convert */

    for (i = 1; i < argc; i++) {
        c = argv[i][0];
        if (c >= '0' && c <= '9')
            ipaddr = ascii_to_ipaddr((B *)argv[i]);
    }

    if (ipaddr == 0L) {
        ipaddr = input_ipaddr((B *)"Destination IP Address = ");
    }

    /* FTP Client Process */

    telnet_client(0, ipaddr);

    return 0;
}

/* end */
