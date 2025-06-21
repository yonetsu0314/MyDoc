/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/tftp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: tftp.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* TFTP.C  Edition: 2  Last changed: 17-Feb-98,14:58:44  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    TFTP.C -- TFTP File Transfer Routines for USNET

    Copyright (C) 1993 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#include <stdio.h>
#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#define MAXDELAY 30000		/* maximum delay in retry loop */

#define T_RRQ NC2(1)		/* TFTP read file */
#define T_WRQ NC2(2)		/* TFTP write file */
#define T_DATA NC2(3)		/* TFTP data message */
#define T_ACK NC2(4)		/* TFTP ack message */

extern PTABLE *const P_tab[];
extern struct NET nets[];
extern struct NETCONF netconf[];

static int gflag;
static MESS *gmp;

/* This runs as a server task to send a file to the host.  We pick up the 
   necessary information from the message.  Then we start reading and sending
   the file in 512 byte blocks.  If the last block is full, we send one block
   with no data bytes.
 */

static TASKFUNCTION TFTPtx(void)
{
    MESS *mp;
    int i1, conno, len;
    char *bp;
    static short ackmes[2] = {T_ACK, 0};
    short ackbuf[40];
    short seq;
    unsigned us1;
    unsigned char cc, *cp1;
    unsigned long delay, inidelay;
#if NTRACE >= 1
    long l1;
#endif
    long l2;
    FILE *ffp;

    mp = gmp;
    gflag = 1;
    WAITNOMORE(SIG_GEN);
    l2 = 0;
    seq = 1;
    conno = mp->conno;
    us1 = Nportno();
    connblo[conno].myport = NC2(us1);
    ffp = fopen((char *)mp+mp->offset+2, "rb");
    if (ffp == 0)
	goto err4;

/* "random" delay */
    cp1 = SOCKET_OWNIPADDR(conno).c;
    cc = cp1[0]+cp1[1]+cp1[2]+cp1[3];
    inidelay = (unsigned long)cc + 100;

    bp = (char *)mp + MESSH_SZ;
    *(short *)(bp) = T_DATA;
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    for (;;)
    {
        len = fread(bp+4, 1, 512, ffp);
        if (len < 0)
            break;
        *(short *)(bp+2) = NC2(seq);
        l2 += len;
        for (delay=inidelay; delay<MAXDELAY; delay<<=1)
	{
            Nwrite(conno, bp, len+4);
	    SOCKET_RXTOUT(conno, delay);
            i1 = Nread(conno, (char *)ackbuf, sizeof(ackbuf));
            if (i1 > 0)
	    {
		if (ackbuf[0] == T_ACK && NC2(ackbuf[1]) == seq)
		    goto lab2;
		if (seq == 1 && ackbuf[0] == T_RRQ)
    		    Nwrite(conno, (char *)ackmes, sizeof(ackmes));
	    }
	}
	goto err1;
lab2:	seq++;
	if (len < 512)
	    break;
    }
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("TX %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1, 
	    (l2*1000)/l1);
#endif
err1:
    fclose(ffp);
err4:
    Nclose(conno);
    Nrelbuf(mp);
}

/* This runs as a server task to receive a file from the host.  We pick up the 
   necessary information from the message.  Then we receive data from the host
   in 512 byte blocks and write it into the file.  A less than full block
   signifies the end.
 */

static TASKFUNCTION TFTPrx(void)
{
    MESS *mp;
    int i1, i2, conno, len, seqno;
    short *ip;
    short ackmes[2];
    unsigned short us1;
#if NTRACE >= 1
    long l1;
#endif
    long l2;
    FILE *ffp;

    mp = gmp;
    gflag = 1;
    WAITNOMORE(SIG_GEN);
    ackmes[0] = T_ACK;
    l2 = ackmes[1] = 0;
    conno = mp->conno;
    us1 = Nportno();
    connblo[conno].myport = NC2(us1);
    ffp = fopen((char *)mp+mp->offset+2, "wb");
    Nrelbuf(mp);
    if (ffp == 0)
	goto err4;
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    i1 = Nwrite(conno, (char *)ackmes, sizeof(ackmes));
    if (i1 <= 0)
	goto err1;
    seqno = 1;
    for (;;)
    {
        mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0) 
	    goto err1;
  	ip = (short *)((char *)mp + mp->offset);
        if (ip[0] != T_DATA)
	{
    	    Nrelbuf(mp);
	    goto err1;
	}
	i2 = NC2(ip[1]);
        ackmes[1] = ip[1];
	if (i2 != seqno)
	{
	    if (i2 == seqno-1)
        	Nwrite(conno, (char *)ackmes, sizeof(ackmes));
    	    Nrelbuf(mp);
	    continue;
	}
	len = mp->mlen - mp->offset - 4;
        Nwrite(conno, (char *)ackmes, sizeof(ackmes));
	seqno++;
        if (len)
            i1 = fwrite((char *)mp+mp->offset+4, len, 1, ffp);
    	Nrelbuf(mp);
        if (i1 <= 0)
	    goto err1;
	l2 += len;
        if (len < 512)
	    break;
    }
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("RX %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1, 
	    (l2*1000)/l1);
#endif
err1:
    fclose(ffp);
err4:
    Nclose(conno);
#ifdef KILLTASK
    KILLTASK();
#endif
}


/* This runs as a server task for TFTP.  When it receives a file transfer
   request, it creates a task to handle the transfer.  It will never return.
   The created task will close the connections.
*/

void TFTPserv(void)
{
    int i1, i2, conno;
    MESS *mp;

    for (;;)
    {
        conno = Nopen("*", "UDP/IP", 69, 0, 0);
        if (conno < 0)
	    continue;
        SOCKET_RXTOUT(conno, 1000000UL);
        mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0)
	    goto lab1;
	mp->conno = conno;
	i1 = *(short *)((char *)mp + mp->offset);
	gflag = 0;
	gmp = mp;
	if (i1 == T_RRQ)
	    i2 = RUNTASK1(TFTPtx, SERV_PRIOR);
	else if (i1 == T_WRQ)
	    i2 = RUNTASK2(TFTPrx, SERV_PRIOR);
	else
	{
	    Nrelbuf(mp);
lab1:	    Nclose(conno);
	    continue;
	}
	WAITFOR(gflag, SIG_GEN, 1000, i1);
	if (i2 < 0)
	    break;
    }
    Nrelbuf(mp);
    Nclose(conno);
    return;
}

/* This is a user subroutine to send a file using the TFTP protocol.
   Arguments:
	host:  station to send to
	file:  from and to files, separated with a space
	mode:  code for ASCII or OCTET
*/

int TFTPput(char *host, char *file, int mode)
{
    int i1, len, conno;
    unsigned int myport;
    char *bp;
    unsigned char cc, *cp1;
    unsigned long delay, inidelay;
#if NTRACE >= 1
    long l1;
#endif
    long l2;
    short seq;
    short ackbuf[40];
    MESS *mp;
    FILE *ffp;

    Nsscanf(file, "%[^ ] %n", (char *)ackbuf, &i1);
    ffp = fopen((char *)ackbuf, "rb");
    if (ffp == 0)
	return -1;
    myport = Nportno();
    conno = Nopen(host, "UDP/IP", myport, 69, 0);
    if (conno < 0)
	goto err2;
    ackbuf[0] = T_WRQ;
    if (mode == ASCII)
	bp = "netascii";
    else
	bp = "octet";
    if (file[i1] == 0)
	i1 = 0;
    len = Nsprintf((char *)ackbuf+2, "%s%c%s", file+i1, 0, bp);

/* "random" delay */
    cp1 = SOCKET_OWNIPADDR(conno).c;
    cc = cp1[0]+cp1[1]+cp1[2]+cp1[3];
    inidelay = (unsigned long)cc + 100;

    for (delay=inidelay; delay<MAXDELAY/2; delay<<=1)
    {
	connblo[conno].herport = NC2(69);
	Nwrite(conno, (char *)ackbuf, len+3);
	connblo[conno].herport = 0;
	SOCKET_RXTOUT(conno, delay);
	i1 = Nread(conno, (char *)ackbuf, sizeof(ackbuf));
	if (i1 >= 0 && ackbuf[0] == T_ACK)
	    goto lab3;
    }
    goto err1;
lab3:
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    
    if ((mp = Ngetbuf()) == 0)
	goto err1;
    bp = (char *)mp + MESSH_SZ;
    *(short *)bp = T_DATA;
    seq = 1;
    for (l2=0; ; )
    {
        len = fread(bp+4, 1, 512, ffp);
        if (len < 0)
            break;
        *(short *)(bp+2) = NC2(seq);
        l2 += len;
        for (delay=inidelay; delay<MAXDELAY; delay<<=1)
	{
            Nwrite(conno, bp, len+4);
	    SOCKET_RXTOUT(conno, delay);
            i1 = Nread(conno, (char *)ackbuf, sizeof(ackbuf));
            if (i1 > 0 && ackbuf[0] == T_ACK && NC2(ackbuf[1]) == seq)
		goto lab2;
	}
  	Nrelbuf(mp);
	goto err1;
lab2:	seq++;
	if (len < 512)
            break;
    }
    Nrelbuf(mp);
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("TX %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1, 
	    (l2*1000)/l1);
#endif

    Nclose(conno);
    fclose(ffp);
    return 0;

err1:
    Nclose(conno);
err2:
    fclose(ffp);
    return -1;
}

/* This is a user subroutine to receive a file using the TFTP protocol.
   Arguments:
	host:  station where the file is
	file:  from and to files, separated with a space
	mode:  code for ASCII or OCTET
*/

int TFTPget(char *host, char *file, int mode)
{
    int i1, i2, len, conno, seqno, eofflg;
    char *cp;
    unsigned char cc, *cp1;
    unsigned long delay, inidelay;
#if NTRACE >= 1
    long l1;
#endif
    long l2;
    short ackbuf[40];
    short ackmes[2];
    short *ip;
    MESS *mp;
    FILE *ffp;

    ackmes[0] = T_ACK;
    ackmes[1] = 0;
    conno = Nopen(host, "UDP/IP", Nportno(), 69, 0);
    if (conno < 0)
	return -1;
    if (mode == ASCII)
	cp = "netascii";
    else
	cp = "octet";
    Nsscanf(file, "%[^ ] %n", (char *)ackbuf, &i1);
    if (file[i1] == 0)
	i1 = 0;
    ackbuf[0] = T_RRQ;
    len = Nsprintf((char *)ackbuf+2, "%s%c%s", file+i1, 0, cp);

/* "random" delay */
    cp1 = SOCKET_OWNIPADDR(conno).c;
    cc = cp1[0]+cp1[1]+cp1[2]+cp1[3];
    inidelay = (unsigned long)cc + 100;

    for (delay=inidelay; delay<MAXDELAY/2; delay<<=1)
    {
	connblo[conno].herport = NC2(69);
	if (ackbuf[0] != T_ACK)
            Nwrite(conno, (char *)ackbuf, len+3);
	connblo[conno].herport = 0;
	ackbuf[0] = 0;
	SOCKET_RXTOUT(conno, delay);
	mp = connblo[conno].protoc[0]->reaD(conno);
	if (mp == 0)
	    continue;
	ackbuf[0] = *(short *)((char *)mp + mp->offset);
	if (ackbuf[0] == T_ACK)
	    continue;
	if (ackbuf[0] == T_DATA)
	    goto lab3;
    }
    goto err2;
lab3:
    Nsscanf(file, "%[^ ]", (char *)ackbuf);
    ffp = fopen((char *)ackbuf, "wb");
    if (ffp == 0)
	goto err2;
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    seqno = 1;
    l2 = eofflg = 0;
    for (;;)
    {
	if (l2)
            mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0) 
	{
	    if (eofflg) break;
	    goto err1;
	}
 	ip = (short *)((char *)mp + mp->offset);
	len = mp->mlen - mp->offset - 4;
        if (ip[0] != T_DATA)
	{
    	    Nrelbuf(mp);
	    if (eofflg) break;
	    goto err1;
	}
	i2 = NC2(ip[1]);
        ackmes[1] = ip[1];
	if (i2 != seqno)
	{
	    if (i2 == seqno-1)
        	Nwrite(conno, (char *)ackmes, sizeof(ackmes));
    	    Nrelbuf(mp);
	    continue;
	}
        Nwrite(conno, (char *)ackmes, sizeof(ackmes));
	seqno++;
	if (len)
            i1 = fwrite((char *)mp+mp->offset+4, len, 1, ffp);
    	Nrelbuf(mp);
        if (i1 <= 0)
	    goto err1;
	l2 += len;
        if (len < 512)
	    eofflg = 1;
    }
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("RX %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1, 
	    (l2*1000)/l1);
#endif

    fclose(ffp);
    Nclose(conno);
    return 0;

err1:
    fclose(ffp);
err2:
    Nclose(conno);
    return -1;
}

