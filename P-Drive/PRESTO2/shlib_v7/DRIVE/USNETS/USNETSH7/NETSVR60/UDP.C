/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/udp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: udp.c $
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
/* UDP.C  Edition: 3  Last changed: 8-Jan-99,16:52:06  By: RICHARD */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    UDP.C -- User Datagram Protocol for USNET

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

* * * * * * * * * * * * * * * * * * * * * * * * *

  MODIFICATION HISTORY:

  30-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#define NOCHKSUM 0	/* please do not turn off the checksum */

struct Uhdr {			/* UDP header */
    unsigned short myport;	/* source port */
    unsigned short herport;	/* dest port */
    unsigned short tlen;	/* length of UDP packet */
    unsigned short chksum;	/* UDP checksum */
};
#define Uhdr_SZ 8		/* size of header */
#define IP_UDP 17		/* protocol code, for IP */
#define ESTABLISHED   1		/* state machine states */
#define LISTEN       16
#define ACTCONN      17

extern struct CONNECT connblo[];
extern struct NET nets[];
extern PTABLE ussUDPTable;

#ifdef MIB2
struct UDPgroup UDPgroup;
#endif

/* ===========================================================================
   Open function.  Call next level.  Passive open will wait for someone to
   talk to us.
*/

static int opeN(int conno, int flags)
{
    int i1;
    register struct CONNECT *conp;

    conp = &connblo[conno];
    conp->txstat |= flags & S_NOCON;
    conp->prevwindow = conp->herport;
#if NTRACE >= 2
    Nprintf("UO C%d/%x %x\n", conno, conp->myport, conp->herport);
#endif
    if (conp->herport)
    {
	i1 = conp->protoc[1]->opeN(conno, flags);
	if ((flags & S_NOCON) == 0)
            conp->state = ACTCONN;
	return i1;
    }
    conp->rxtout = 0xffffffff;
    conp->state = LISTEN;
    if ((flags | conp->txstat) & S_NOWA)
	return 0;
    WAITFOR(conp->state==ESTABLISHED||(conp->rxstat&S_FATAL), SIG_RC(conno),
	conp->rxtout, i1);
    if (i1)
	return ETIMEDOUT;
    if (conp->state != ESTABLISHED)
	return NE_PROTO;
    return 1;
}


/* ===========================================================================
   Close function, just call next level. */

static int closE(int conno)
{
    struct CONNECT *conp;

    conp = &connblo[conno];
#if NTRACE >= 2
    Nprintf("UC C%d/%x %x\n", conno, conp->myport, conp->herport);
#endif
    return conp->protoc[1]->closE(conno);
}


/* ===========================================================================
   Screening function, checks the coming message.  Searches the connection
   tables for one that is open for the right port.  Return:
	-6  destination unreachable
	-2  no further action
	-1  error occured
	n   please enter in queue number n
*/

static int screen(MESS *mess)
{
    int netno, conno, portno, herport, tlen;
    register struct Uhdr *uhdrp;
    register struct CONNECT *conp;
    struct { short s[2]; unsigned char Iadd1[Iid_SZ], Iadd2[Iid_SZ];
	unsigned short us; } pseudo;

    uhdrp = (struct Uhdr *)((char *)mess + mess->offset);
    netno = mess->netno;
    portno = uhdrp->herport;
    herport = uhdrp->myport;

/* First check the checksum, if non-zero.  IP (and any lower protocol) stores
   offset to Internet addresses temporarily into mess->conno */

#if NOCHKSUM == 0
    if (uhdrp->chksum)
    {
        memcpy((char *)&pseudo.Iadd1, (char*)mess+mess->conno, 2*Iid_SZ);
	tlen = NC2(uhdrp->tlen);
        if (tlen != mess->mlen - mess->offset)
	    goto err1;
        if (tlen & 1)
            *((char *)uhdrp + tlen) = 0;
	pseudo.s[0] = NC2(IP_UDP);
	pseudo.s[1] = uhdrp->tlen;
        pseudo.us = Nchksum((unsigned short *)uhdrp, (tlen+1)/2);
        if (Nchksum((unsigned short *)&pseudo, 14>>1) != (unsigned short)0xffff)
	    goto err1;
    }
#endif

/* Locate the connection block for this message. */

    for (conno=0; conno<NCONNS; conno++)
    {
	conp = &connblo[conno];
	if (conp->blockstat == 0)
	    continue;
	if (conp->myport != portno)
	    continue;
	if (conp->protoc[0] != &ussUDPTable)
	    continue;
/* if we use logical connection (not BSD) and state is ACTCONN or ESTABLISHED
   then we check if the remote host is right */
	if ((conp->state & 1) && (conp->txstat & S_NOCON) == 0)
	{
	    if (mess->target != conp->heriid.l &&
		conp->heriid.l != 0xffffffff &&
		conp->heriid.l != (mess->target|~netconf[mess->confix].Imask.l))
		continue;
	    if (conp->herport == 0) 
	 	conp->herport = herport;
	    if (conp->herport != herport) continue;
	}
	goto lab1;
    }
#ifdef MIB2
    UDPgroup.udpNoPorts++;
#endif
    return -6;

err1:
    return -1;

/* Here we have a good message and found the connection block. */

lab1:
    if (conp->state == LISTEN)
    {
	conp->herport = herport;
	conp->heriid.l = mess->target;
    }

    if (conp->state != ESTABLISHED)
    {
	conp->maxdat = nets[netno].maxblo - mess->offset +
	    MESSH_SZ + LHDRSZ - Uhdr_SZ;
        if (conp->state >= LISTEN) 		/* LISTEN or ACTCONN */
        {
            conp->state = ESTABLISHED;
	    conp->netno = netno;
	    conp->confix = mess->confix;
        }
    }

#if NTRACE >= 2
    Nprintf("US C%d/%x %x D%d\n", conno, conp->myport, conp->herport,
	mess->mlen);
#endif
    return conno;
}


/* ===========================================================================
   Read routine.  Calls the lower level read.  Takes the information from
   the header, gives the caller the message.  Returns:
	success: message address
	error:   0
*/

static MESS *reaD(int conno)
{
    int mlen, hdroff;
    MESS *mess;
    register struct Uhdr *uhdrp;
    register struct CONNECT *conp;

    conp = &connblo[conno];
    mess = conp->protoc[1]->reaD(conno);
    if (mess == 0)
	return 0;
    hdroff = mess->offset;
    mess->offset = hdroff + Uhdr_SZ;
    uhdrp = (struct Uhdr *)((char *)mess + hdroff);
    mlen = NC2(uhdrp->tlen);
    mess->mlen = mlen + hdroff;
/* If this was a passive no-connection open, take remote address and port */
    if (conp->prevwindow == 0 && (conp->txstat&S_NOCON))
    {
	conp->heriid.l = mess->target;
        conp->herport = uhdrp->myport;
    }
#if NTRACE >= 2
    hdroff = mess->offset;
    Nprintf("UR C%d/%x %x D%d %02x%02x%02x%02x\n", conno, conp->myport, 
	conp->herport, mlen-Uhdr_SZ, 
	((unsigned char *)mess)[hdroff], ((unsigned char *)mess)[hdroff+1],
	((unsigned char *)mess)[hdroff+2], ((unsigned char *)mess)[hdroff+3]);
#endif
#ifdef MIB2
    UDPgroup.udpInDatagrams++;
#endif
    return mess;
}


/* ===========================================================================
   Write routine.  Creates a header and calls the lower-level write.  Returns:
	success: number of characters sent
	error:   -1
*/

static int writE(int conno, MESS *mess)
{
    int hdroff, mlen;
    register struct Uhdr *uhdrp;
    register struct CONNECT *conp;
    struct { short s[2]; unsigned char Iadd1[Iid_SZ], Iadd2[Iid_SZ]; } pseudo;

    conp = &connblo[conno];
    mess->offset -= Uhdr_SZ;
    hdroff = mess->offset;
    uhdrp = (struct Uhdr *)((char *)mess + hdroff);
    mlen = mess->mlen - hdroff;
    if (mlen & 1)
        *((char *)uhdrp + mlen) = 0;
    uhdrp->tlen = NC2(mlen);
    uhdrp->myport = conp->myport;
    uhdrp->herport = conp->herport;
    *(Iid *)&pseudo.Iadd1 = conp->heriid;
    *(long *)&pseudo.Iadd2 = netconf[nets[conp->netno].confix].Iaddr.l;
    pseudo.s[0] = NC2(IP_UDP);
    pseudo.s[1] = uhdrp->tlen;
#if NOCHKSUM == 0
    uhdrp->chksum = Nchksum((unsigned short *)&pseudo, 12>>1);
    uhdrp->chksum = ~Nchksum((unsigned short *)uhdrp, (mlen+1)/2);
    if (uhdrp->chksum == 0)
    	uhdrp->chksum = 0xffff;
#else
    uhdrp->chksum = 0;
#endif
#if NTRACE >= 2
    hdroff += Uhdr_SZ;
    Nprintf("UW C%d/%x D%d %02x%02x%02x%02x\n", conno, conp->myport,
	mlen-Uhdr_SZ,
	((unsigned char *)mess)[hdroff], ((unsigned char *)mess)[hdroff+1],
	((unsigned char *)mess)[hdroff+2], ((unsigned char *)mess)[hdroff+3]);
#endif
#ifdef MIB2
    UDPgroup.udpOutDatagrams++;
#endif
/* protocol code for IP */
    *((char *)mess + MESSH_SZ+LHDRSZ+9) = IP_UDP;
    mess->id = bRELEASE;	/* no-wait write for UDP */
    return conp->protoc[1]->writE(conno, mess);
}


/* ===========================================================================
   Initialize the protocol.  
*/

static int init(int netno, char *params)
{
#ifdef MIB2
    memset(&UDPgroup, 0, sizeof(UDPgroup));
#endif
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg);
**
**  PARAMETERS:
**     (in)     handle   likely to be netno if any functions are added
**                       at this level
**     (in)     request  constant for the command to perform
**     (in)     arg      pointer to the parameters to the command, which
**                       will be specific to the command
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.  At the moment, there are no functions
**     defined for this layer.
**
**  RETURNS:
**     0              Success
**     USS_ERR_INVAL  Support for the request not implemented
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  31-DEC-1998  RCA  Created stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */

    return ussErrInval;
}


/* ===========================================================================
   Protocol table for UDP. */

GLOBALCONST
PTABLE ussUDPTable = {"UDP", init, 0, screen, opeN, closE, reaD, writE,
    ioctl, IP_UDP, Uhdr_SZ};

