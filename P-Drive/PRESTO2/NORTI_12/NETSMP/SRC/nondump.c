/**************************************************************************
* NORTi Network Packet Dump Routine for Debug                             *
*                                                                         *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                               *
*  All rights reserved.                                                   *
*                                                                         *
* 05/Mar/1999                                                             *
* 15/Mar/1999 '#include nosio3.h' is deleted.                             *
* 25/May/1999 1パケット1行に変更, OVFを表示して待ちが起きないよう変更     *
* 12/Jun/1999 sprintfを使わない, 他                                       *
* 22/Sep/1999 resource ID auto assign added.                              *
* 08/Nov/2001 dump_packetバグ修正                                      MI *
* 15/Apr/2002 analyze_packet(), cnt<=0時のstrncpyサイズ計算修正        HM *
* 08/Sep/2002 ワーニング対策 B * -> char *                             MI *
* 28/Nov/2002 landump_msg 修正, メッセージの場合はそれのみ表示         MI *
***************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nosys3.h"
#include "nosio.h"
#include "nonet.h"
#include "nonitod.h"

#ifndef CH
#define CH      1
#endif
#ifndef DATASZ
#define DATASZ  6+6+2+ETHERNET_MTU
#endif
#ifndef QCNT
#define QCNT    10
#endif

extern SYSTIME SNEAR cdecl SYSCK;

static ID landump_mbxid;
static ID landump_mpfid;

TASK LanDumpTask(void);
const T_CTSK cLanDumpTask = { NULL, TA_HLNG, LanDumpTask, 6, 2048 };

typedef struct t_dump_msg {
    struct t_dump_msg *next;
    H len;
    H cnt;
    UW tim;
    B data[DATASZ];
} T_DUMP_MSG;

const T_CMBX c_landump_mbx = { NULL, TA_TFIFO|TA_MFIFO };
const T_CMPF c_landump_mpf = { NULL, TA_TFIFO, QCNT, sizeof (T_DUMP_MSG) };

/************************************************/
/* Output 1 Charactor                           */
/************************************************/

void dump_putc(int c)
{
    while (put_sio(CH, (UB)c, 40/MSEC) != E_OK)
        ;
}

/************************************************/
/* Output 1 Line                                */
/************************************************/

void dump_puts(const char *s)
{
    UB c;

    while ((c = (UB)*s++) != '\0')
        put_sio(CH, c, TMO_FEVR);
}

/************************************************/
/* Analyze Packet (ICMP)                        */
/************************************************/

static BOOL analyze_packet_icmp(char *s, char *end, UB *d, int cnt)
{
    int j;
    UB c;

    if (cnt <= 0)
        return FALSE;
    if (s + 3 >= end)
        return TRUE;
    c = d[0];
    *s++ = ' ';
    j = end - s;
    switch (c)
    {
    case  3: strncpy(s, (char *)"Destination Unreachable", j); break;
    case  5: strncpy(s, (char *)"Redirect", j); break;
    case  4: strncpy(s, (char *)"Source Quench", j); break;
    case 11: strncpy(s, (char *)"Time Exceeded", j); break;
    case 12: strncpy(s, (char *)"Parameter Problems", j); break;
    case  0: strncpy(s, (char *)"Echo Reply", j); break;
    case  8: strncpy(s, (char *)"Echo Request", j); break;
    case 15: strncpy(s, (char *)"Information Request", j); break;
    case 16: strncpy(s, (char *)"Information Reply", j); break;
    case 13: strncpy(s, (char *)"Timestamp Request", j); break;
    case 14: strncpy(s, (char *)"Timestamp Reply", j); break;
    case 17: strncpy(s, (char *)"Address Mark Request", j); break;
    case 18: strncpy(s, (char *)"Address Mark Reply", j); break;
    case  9: strncpy(s, (char *)"Router Advertisement", j); break;
    case 10: strncpy(s, (char *)"Solicition Message", j); break;
    default: ito0X(s, c, 2); break;
    }
    return TRUE;
}

/************************************************/
/* Analyze Packet (UDP)                         */
/************************************************/

static BOOL analyze_packet_udp(char *s, char *end, UB *d, int cnt)
{
    UH dd;

    /* Port Number */

    if (cnt <= 5)
        return FALSE;
    if (s + 6 >= end)
        return TRUE;
    *s++ = ' ';
    dd = ((UH)d[0] << 8) + d[1]; s = itou(s, dd, 0);
    if (s + 7 >= end)
        return TRUE;
    strcpy(s, (char *)"->"); s += 2;
    dd = ((UH)d[2] << 8) + d[3]; s = itou(s, dd, 0);

    /* Length */

    if (s + 9 >= end)
        return TRUE;
    strcpy(s, (char *)" Len"); s += 4;
    dd = ((UH)d[4] << 8) + d[5]; s = itou(s, dd, 0);
    return TRUE;
}

/************************************************/
/* Analyze Packet (TCP)                         */
/************************************************/

static BOOL analyze_packet_tcp(char *s, char *end, UB *d, int cnt)
{
    int i;
    UB c;
    UH dd;
    UW ddd;

    /* Port Number */

    if (cnt <= 10)
        return FALSE;
    if (s + 5 >= end)
        return TRUE;
    *s++ = ' ';
    dd = ((UH)d[0] << 8) + d[1];
    s = itou(s, dd, 0);
    if (s + 6 >= end)
        return TRUE;
    strcpy(s, (char *)"->"); s += 2;
    dd = ((UH)d[2] << 8) + d[3];
    s = itou(s, dd, 0);

    /* SEQ and ACK number */

    if (s + 11 >= end)
        return TRUE;
    *s++ = ' ';
    ddd = ((UW)d[4] << 24) + ((UW)d[5] << 16) + ((UH)d[6] << 8) + d[7];
    s = ltou(s, ddd, 0);
    if (s + 11 >= end)
        return TRUE;
    *s++ = '/';
    ddd = ((UW)d[8] << 24) + ((UW)d[9] << 16) + ((UH)d[10] << 8) + d[11];
    s = ltou(s, ddd, 0);

    /* Flag */

    if (cnt <= 13)
        return FALSE;
    c = d[13];
    if (c & 0x20) { if (s + 4 <= end) { strcpy(s, (char *)" URG"); s += 4; }}
    if (c & 0x10) { if (s + 4 <= end) { strcpy(s, (char *)" ACK"); s += 4; }}
    if (c & 0x08) { if (s + 4 <= end) { strcpy(s, (char *)" PSH"); s += 4; }}
    if (c & 0x04) { if (s + 4 <= end) { strcpy(s, (char *)" RST"); s += 4; }}
    if (c & 0x02) { if (s + 4 <= end) { strcpy(s, (char *)" SYN"); s += 4; }}
    if (c & 0x01) { if (s + 4 <= end) { strcpy(s, (char *)" FIN"); s += 4; }}

    /* Window Size */

  #ifdef WIN
    if (cnt <= 15)
        return FALSE;
    if (s + 5 + 5 >= end)
        return;
    strcpy(s, " Win="); s += 5;
    dd = ((UH)d[14] << 8) + d[15];
    s = itou(s, dd, 0);
  #endif

    /* TCP Data */

    if (s + 2 >= end)
        return TRUE;
    *s++ = ' ';
    i = (int)((d[12] >> 4) & 0x0f) * 4;
    for (;;)
    {   if (cnt <= i)
            break;
        if (s + 1 >= end)
            break;
        c = d[i++];
        if (c < 0x20 || (c >= 0x80 && c < 0xa0) || c >= 0xe0)
            c = '.';
        *s++ = (B)c;
    }
    *s++ = '\0';
    return TRUE;
}

/************************************************/
/* Analyze Packet (Main)                        */
/************************************************/

static int analyze_packet(char *buf, int size, int cnt, UW tim, void *data)
{
    static UW tt;
    char *s, *end;
    int i, c;
    UH dd;
    UW t;
    UB *d = data;
    BOOL ok;

    s = buf;
    end = buf + size - 1;
    *s++ = '\r'; *s++ = '\n';

    /* Delta Time */

    t = tim - tt; tt = tim;
    s = lto_u(s, t, 3); *s++ = ':';

    if (cnt <= 0)
    {   strncpy(s, data, end - s);
        return 0;
    }

    /* MAC Address */

  #ifdef MAC
    if (cnt > 11)
    {   if (s + 13 * 2 >= end)
            return cnt;
        for (i = 6; i <= 11; i++)
            s = ito0X(s, d[i], 2);
        *s++ = '-'; *s++ = '>';
        for (i = 0; i <= 5; i++)
            s = ito0X(s, d[i], 2);
    }
  #endif

    /* Frame Type */

    if (cnt <= 13)
        return cnt;
    if (s + 10 >= end)
        return cnt;
    dd = (((UH)d[12] << 8) & 0xff00) + d[13];
    switch (dd)
    {
    case 0x0800: break; /* IP */
    case 0x0600: strcpy(s, (char *)" Xerox XNS"); s += 10; break;
    case 0x0806: strcpy(s, (char *)" ARP");       s += 4;  break;
    case 0x0807: strcpy(s, (char *)" XNS");       s += 4;  break;
    case 0x80d5: strcpy(s, (char *)" IBM SNA");   s += 8;  break;
    case 0x8137: strcpy(s, (char *)" Old IPX");   s += 8;  break;
    case 0x8138: strcpy(s, (char *)" IPX");       s += 4;  break;
    case 0x9000: strcpy(s, (char *)" Loop Back"); s += 10; break;
    default:     strcpy(s, (char *)" FrameType=");s += 11; s = ito0X(s, dd, 4);  break;
    }

    /* When IP */

    if (dd == 0x0800) /* IP */
    {
        /* Protocol Type */

        if (cnt <= 23)
            return cnt;
        if (s + 5 >= end)
            return cnt;
        dd = (((int)d[16] << 8) & 0xff00) + d[17]; /* Total Length */
        if (cnt > (int)dd + 14)
            cnt = (int)dd + 14;
        c = d[23];
        switch (c)
        {
        case  1: strcpy(s, (char *)" ICMP"); s += 5; break;
        case  6: strcpy(s, (char *)" TCP");  s += 4; break;
        case 17: strcpy(s, (char *)" UDP");  s += 4; break;
        default: strcpy(s, (char *)" IP");   s += 3; break;
        }

        i = 14 + (d[14] & 0x0f) * 4; /* Next part of IP header */

        if (c == 1) /* ICMP */
            ok = analyze_packet_icmp(s, end, d + i, cnt - i);
        else if (c == 17) /* UDP */
            ok = analyze_packet_udp(s, end, d + i, cnt - i);
        else if (c == 6) /* TCP */
            ok = analyze_packet_tcp(s, end, d + i, cnt - i);
        else
            ok = TRUE;
        if (!ok)
            strcat(s, (char *)"?");
    }
    return cnt;
}

/************************************************/
/* Dump Packet                                  */
/************************************************/
#ifdef DATA
static void dump_packet(char *buf, void *data, int cnt)
{
    char *s;
    int i, j, k, c, jj, kk;
    UB *d = data;

    if (cnt <= 0)
        return;

    dump_puts((char *)"\r\n");

    for (j = 0; ; j += 16 * 16)
    {
        for (i = 0; i < 16; i++)
        {
            jj = cnt - (j + (i + 1) * 16);
            s = buf;
            s = ito0X(s, j + i * 16, 4); *s++ = ' '; *s++ = ' ';
            for (k = 0; k < 16; k++)
            {   s = ito0X(s, d[k], 2); *s++ = ' ';
                if (k == 7)
                    *s++ = ' ';
            }
            *s= '\0';
            s = buf;
            if (jj < 0)
            {   kk = 16 + jj;
                for (k = 16 + jj; k < 16; k++)
                {   if (k < 8)
                        s[6 + k * 3] = s[6 + k * 3 + 1] = ' ';
                    else
                        s[7 + k * 3] = s[7 + k * 3 + 1] = ' ';
                }
            }
            else
                kk = 16;
            dump_puts(s);
            for (k = 0; k < kk; k++)
            {   c = d[k];
                if (c < 0x20 || (c >= 0x80 && c < 0xa0) || c >= 0xe0)
                    c = '.';
                dump_putc(c);
            }
            dump_puts((char *)"\r\n");
            d += 16;
            if (jj <= 0)
                return;
        }
    }
}
#endif

/************************************************/
/* Packet Dump Task                             */
/************************************************/

TASK LanDumpTask(void)
{
    T_DUMP_MSG *msg;
  #ifdef DATA
    int cnt;
  #endif
    static B buf[2+80+1];

    dump_puts((char *)"\r\nLAN packets dump program started\r\n");

    for (;;)
    {
        if (rcv_msg(&msg, landump_mbxid) != E_OK)
            continue;
        if (msg->len == 0) {
            dump_puts((char *)msg->data);
            rel_blf(landump_mpfid, msg);
            continue;
        }
      #if (defined(TCP) && defined(ARP))
        if ((((UH *)(msg->data))[12/2] != 0x0806)
         && (((UB *)(msg->data))[23] != 6)) {
            rel_blf(landump_mpfid, msg);
            continue;
        }
      #elif defined(ARP)
        if (((UH *)(msg->data))[12/2] != 0x0806) {
            rel_blf(landump_mpfid, msg);
            continue;
        }
      #elif defined(TCP)
        if (((UB *)(msg->data))[23] != 6) {
            rel_blf(landump_mpfid, msg);
            continue;
        }
      #endif

      #ifdef DATA
        cnt = analyze_packet((char *)buf, sizeof buf, msg->cnt, msg->tim, msg->data);
      #else
        analyze_packet((char *)buf, sizeof buf, msg->cnt, msg->tim, msg->data);
      #endif

        dump_puts((char *)buf);
      #ifdef DATA
        dump_packet((char *)buf, msg->data, cnt);
      #endif
        rel_blf(landump_mpfid, msg);
    }
}

/************************************************/
/* End of Dump Data                             */
/************************************************/

void landump_end(VP b)
{
    if (landump_mbxid != 0 && b != NULL)
        snd_msg(landump_mbxid, (T_MSG *)b);
}

/************************************************/
/* Receided/Transfer Packet Length              */
/************************************************/

void landump_len(VP *b, int len)
{
    UW t;
    T_DUMP_MSG *msg;
    T_RMPF rmpf;
    B ovf;

    t = SYSCK.ltime;

    if (landump_mpfid == 0)
    {   *b = NULL;
        return;
    }
    ref_mpf(&rmpf, landump_mpfid);
    if (rmpf.frbcnt == 0)
    {   ovf = TRUE;
        len = 0;
        while (prcv_msg(&msg, landump_mbxid) == E_OK)
            rel_blf(landump_mpfid, msg);
    }
    else
        ovf = FALSE;

    if (pget_blf(&msg, landump_mpfid) != E_OK)
    {   *b = NULL;
        return;
    }
    msg->len = (H)len;
    msg->cnt = 0;
    msg->tim = t;
    if (ovf)
        strcpy((char *)(msg->data), (char *)"OVF");
    if (len <= 0)
    {   landump_end(msg);
        *b = NULL;
    }
    else
        *b = (VP)msg;
}

/************************************************/
/* Message for Debug                            */
/************************************************/

void landump_msg(char *s)
{
    T_DUMP_MSG *msg;

    if (landump_mpfid == 0)
        return;
    if (pget_blf(&msg, landump_mpfid) != E_OK)
        return;
    msg->len = 0;
    strcpy((char *)(msg->data), s);
    landump_end(msg);
}

/************************************************/
/* Receided/Transfer Data                       */
/************************************************/

void landump_dat(VP b, const void *data, int len)
{
    T_DUMP_MSG *msg;

    if (landump_mpfid == 0)
        return;
    msg = b;
    if (msg == NULL)
        return;
    if (len > (int)(sizeof msg->data - msg->cnt))
        len = sizeof msg->data - msg->cnt;
    if (data != NULL)
        memcpy((VP)(msg->data + msg->cnt), data, len);
    else
        memset((VP)(msg->data + msg->cnt), 0, len);
    msg->cnt += len;
}

/************************************************/
/* Packet Dump Initialize                       */
/************************************************/

ER landump_ini(ID tskid, ID mbxid, ID mpfid)
{
    ER ercd;

    /* Create LAN dump task */

    if(tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&cLanDumpTask);
        if (ercd < 0)
            return ercd;
        tskid = ercd;
    }
    else{
        ercd = cre_tsk(tskid, &cLanDumpTask);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create Mailbox for LAN dump */

    if(mbxid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_mbx(&c_landump_mbx);
        if (ercd < 0)
            return ercd;
        mbxid = ercd;
    }
    else{
        ercd = cre_mbx(mbxid, &c_landump_mbx);
        if (ercd != E_OK)
            return ercd;
    }
    landump_mbxid = mbxid;

    /* Create Memory pool for LAN dump */

    if(mpfid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_mpf(&c_landump_mpf);
        if (ercd < 0)
            return ercd;
        mpfid = ercd;
    }
    else{
        ercd = cre_mpf(mpfid, &c_landump_mpf);
        if (ercd != E_OK)
            return ercd;
    }
    landump_mpfid = mpfid;

    return sta_tsk(tskid, 0);
}

/* end */
