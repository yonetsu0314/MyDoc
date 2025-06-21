/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/bootp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: bootp.c $
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
/* BOOTP.C  Edition: 2  Last changed: 17-Feb-98,14:57:42  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    BOOTP.C -- Bootstrap Protocol for USNET

    Copyright (C) 1993, 1995 By
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

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include <bootp.h>

extern struct NETCONF netconf[];
extern struct CONNECT connblo[];
extern struct NET nets[];

extern struct BOOTP bootrec;
static unsigned short seqno;
static unsigned long inidelay;
static int      eofflg;
static int      bflag;
static MESS    *bptr;

/* This is a user subroutine to get the bootload information from the BOOTP
   server. */

int             BOOTPget(int netno)
{
    int             stat,
                    conno,
                    confix;
    unsigned long   ul1,
                    initime,
                    delay;
    char            buff[32];
    struct NETCONF *confp,
                   *confp2;

/* "random" delay */
    initime = TimeMS();
    delay = (unsigned long) (((int) initime & 0xff) + 100);

/* open the connection block */
    confp = &netconf[nets[netno].confix];
    Nsprintf(buff, "*/%s", confp->pname);
    conno = Nopen(buff, "UDP/IP", 68, 67, 0);
    if (conno < 0)
        return conno;
/* use limited broadcast */
    connblo[conno].heriid.l = 0xffffffff;
    SOCKET_RXTOUT(conno, 1000);

/* create and write message, read answer, several times */
    for (; delay < MAXDELAY; delay <<= 1) {
        memset(&bootrec, 0, sizeof(bootrec));
        bootrec.op = 1;
        bootrec.htype = 1;
        bootrec.hlen = sizeof(struct Eid);
        bootrec.xid = TimeMS();
        bootrec.secs = (bootrec.xid - initime) >> 10;
        memcpy((char *) &bootrec.ciaddr, (char *) &confp->Iaddr, Iid_SZ);
        memcpy(bootrec.chaddr, (char *) &confp->Eaddr, Eid_SZ);
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        Nwrite(conno, (char *) &bootrec, sizeof(struct BOOTP));
        stat = Nread(conno, (char *) &bootrec, sizeof(struct BOOTP));
        if (stat == sizeof(struct BOOTP))
            if (bootrec.op == 2)
                break;
        SOCKET_CLEAR(conno);
        stat = ETIMEDOUT;
    }

/* if we got the record, we update our tables from it */
    if (stat >= 0) {
/* our IP address */
        if (confp->Iaddr.l == 0)
            memcpy((char *) &confp->Iaddr, (char *) &bootrec.yiaddr, Iid_SZ);
/* bootload server */
        if ((confix = GetHostData(bootrec.siaddr, 3)) >= 0) {
            confp2 = &netconf[confix];
            confp2->nexthix = confix;
            if (confp2->name[0] == 0)
                memcpy(confp2->name, bootrec.host, sizeof(confp2->name) - 1);
        }
/* router */
        if ((confix = GetHostData(bootrec.giaddr, 3)) >= 0) {
            confp2 = &netconf[confix];
            confp2->nexthix = confix;
            confp2->flags |= ROUTER;
        }
    }
    Nclose(conno);

/* if our IP address is still 0, this was not a success
*/
    if (confp->Iaddr.l == 0)
        stat = -1;
    return stat;
}

/* This is a user subroutine to open a connection for bootloading. */

int             BOOTPopen(int netno)
{
    int             stat,
                    conno;
    unsigned int    type;
    unsigned char   cc,
                   *cp;
    unsigned long   ul1,
                    delay;
    short           ackbuf[128];
    struct NETCONF *confp;

    bflag = 0;

/* open connection block */
    conno = Nopen((char *) &bootrec.siaddr, "UDP/IP", Nportno(), 69, IPADDR);
    if (conno < 0)
        return conno;
    SOCKET_RXTOUT(conno, 1000);

/* "random" delay */
    confp = &netconf[nets[netno].confix];
    cp = confp->Iaddr.c;
    cc = cp[0] + cp[1] + cp[2] + cp[3];
    inidelay = (unsigned long) cc + 100;

/* send read file request, try to get ACK or DATA for that */
    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        ackbuf[0] = T_RRQ;
        stat = Nsprintf((char *) ackbuf + 2, "%s%coctet", bootrec.file, 0);
        Nwrite(conno, (char *) ackbuf, stat + 3);
        connblo[conno].herport = 0;
        bptr = connblo[conno].protoc[0]->reaD(conno);
        if (bptr == 0)
            continue;
        type = *(unsigned short *) ((char *) bptr + bptr->offset);
        if (type == T_DATA)
            goto lab4;
        Nrelbuf(bptr);
        if (type == T_ACK)
            goto lab5;
    }

/* if we failed we close the connection block */
    Nclose(conno);
    return ETIMEDOUT;

/* return good status */
lab4:
    bflag = 1;
lab5:
    seqno = 1;
    return conno;
}

/* This is a user subroutine to read from a bootload file.  The file must
   first be opened with a call to BOOTPopen. */

int             BOOTPread(int conno, char *buff, int len)
{
    int             i1;
    unsigned int    type,
                    seq;
    unsigned long   ul1,
                    delay;
    short           ackmes[2];

    if (eofflg)
        return 0;
    ackmes[0] = T_ACK;

    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        if (bflag) {
            bflag = 0;
            i1 = bptr->mlen - bptr->offset;
            if (i1 > len)
                return EMSGSIZE;
            len = i1;
            memcpy(buff, (char *) bptr + bptr->offset, len);
            Nrelbuf(bptr);
        } else
            len = Nread(conno, buff, len);
        type = *(unsigned short *) buff;
        seq = (buff[2] << 8) | (unsigned char) buff[3];
        if (len >= 4 && type == T_DATA && seq == seqno) {
            ackmes[1] = NC2(seqno);
            Nwrite(conno, (char *) ackmes, sizeof(ackmes));
            seqno++;
            if (len < 512)
                eofflg = 1;
            return len;
        }
        if (seq == seqno - 1) {
            ackmes[1] = NC2(seq);
            Nwrite(conno, (char *) ackmes, sizeof(ackmes));
        }
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
    }
    return ETIMEDOUT;
}
