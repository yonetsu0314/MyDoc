/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/ping.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ping.c $
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
/* PING.C  Edition: 2  Last changed: 17-Feb-98,14:12:56  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    PING.C -- PING Test Program for USNET

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

#include <stdlib.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "icmp.h"


/* This simple utility will send a ping at one-second intervals.  You can
   use any of these forms:
	ping hostname
	ping n.n.n.n
	ping *  (some TCP/IP implementations will not answer)
*/

#define PEER  "emiri"

char UsnetPing( char *to, struct ICMPhdr *hdr, char retry )
{
    int i1, stat, conno, recsiz;
    long li1;
    unsigned long ul1;
    unsigned long *ulp;
    unsigned char *cp;
    static char buff[MAXBUF];
    char *pname;
    struct pingm {struct ICMPhdr hdr; unsigned long ul[MAXBUF/4];}
	pingm, *pp;
	char *host = to;
	char maxretry = 4;
	if( retry > 0 ) {
		maxretry = retry;
	}
/*
    if (Ninit() < 0)
	return -1;

    if (Portinit("*") < 0)
    {
	Nterm();
	return -1;
    }
 */
    recsiz = 64;
    /*ConfDisplay();*/
    pingm.hdr.type = IC_ECHO;
    pingm.hdr.scode = 0;
    pingm.hdr.chksum = 0;
    ulp = (unsigned long *)pingm.hdr.param; 
    ulp[0] = 1; 
    ulp[1] = 2; 
    ulp[2] = 3; 
    ulp[3] = 4; 
    conno = Nopen(host, "ICMP/IP", 0, 0, 0);
    if (conno < 0)
	goto err1;
    i1 = MAXBUF - MESSH_SZ - LHDRSZ - 28;
    if (recsiz > i1)
	goto sizerr;
    SOCKET_RXTOUT(conno, 10);
    for (li1=1; ; li1++)
    {
		stat = 0;
        i1 = Nwrite(conno, (char *)&pingm, recsiz+8);
		for (ul1=TimeMS(); TimeMS()-ul1<1000; )
		{
			YIELD();
			i1 = Nread(conno, buff, sizeof(buff));
			if (i1 <= 0) {
				continue;
			}
			stat = 1;
			pp = (struct pingm *)buff;
			hdr->type = pp->hdr.type;
			hdr->scode = pp->hdr.scode;
			if (pp->hdr.type != IC_EREP) {
				goto err3;
			}
			cp = SOCKET_IPADDR(conno).c;
			Nprintf("%ld reply from %d.%d.%d.%d\n", li1, cp[0], cp[1],
				cp[2], cp[3]);
			break;
		}
		if (stat == 0) {
			hdr->type = -1;
			hdr->scode = -1;
			Nprintf("%ld no reply\n", li1);
    	}
		if (li1 >= maxretry) {
			if( stat == 0 ) {
				goto ret19;
			}
			goto ret9;
		}
    }
err1:
    Nprintf("connection fails\n");
    goto ret29;
synerr:
    Nprintf("syntax: ping [-s size] <host>");
    goto ret39;
sizerr:
    Nprintf("largest usable size %d\n", i1);
    goto ret19;
err3:
    Nprintf("funny response\n");
ret19:
    Nclose(conno);
ret29:
/*    Nterm();*/
ret39:
    return -1;
ret9:
    Nprintf("user terminated\n");
    Nclose(conno);
/*    Nterm();*/
    return 0;
}

