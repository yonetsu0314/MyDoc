/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/arcnet.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: arcnet.c $
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
/* ARCNET.C  Edition: 3  Last changed: 8-Jan-99,16:58:14  By: RICHARD */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ARCNET.C -- ARCNET Link Layer for USNET

    Copyright (C) 1994, 1998 By
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

  29-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

struct Ahdr {                   /* synthetic ARCNET header structure */
    unsigned char   to;
    unsigned char   null1[4];
    unsigned char   fsplit;
    unsigned char   from;
    unsigned char   offset;
    unsigned short  moffset;
    unsigned char   protocol;
    unsigned char   split;
    unsigned short  seqno;
};

#define AR_TOUT 1000            /* basic timeout value */
#define AR_MAXLEN 1500          /* maximum block size */

#define	AR_IP 212               /* Internet message */
#define	AR_ARP 213              /* address request message */
#define	AR_RARP 214             /* reverse address request message */

extern struct NET nets[];
extern struct NETCONF netconf[];
extern struct NETDATA netdata[];
extern struct CONNECT connblo[];
extern PTABLE  *const P_tab[];
#ifdef ARP
extern PTABLE   ussARPTable;
#endif


/* ===========================================================================
   Write routine.  If we don't have the target address, we send an ARP message
   and wait for the address.  Creates a header and calls the lower level write.
   Returns:
    success: number of characters sent
    error:   -1
*/

static int      writE(int conno, MESS * mess)
{
    int             i1,
                    netno,
                    confix;
    unsigned short  us1;
    MESS           *mp;
    struct Ahdr    *ahdrp;
    struct NETCONF *confp;
    register struct NET *netp;

/* Prepare some information.
*/
    netno = mess->netno;
    netp = &nets[netno];
    ahdrp = (struct Ahdr *) ((char *) mess + MESSH_SZ);
    if (conno >= 0) {
        if (*((char *) mess + MESSH_SZ + 13) == 0x35)
            ahdrp->protocol = AR_RARP;
        else if (*((char *) mess + MESSH_SZ + 13) == 0x06)
            ahdrp->protocol = AR_ARP;
        else
            ahdrp->protocol = AR_IP;
    }
/* If conno == -1, we return the message to the sender.
*/
    if (conno == -1) {
        ahdrp->to = ahdrp->from;
        goto lab2;
    }
/* Value mess-confix = 255 means full broadcast.
*/
    if (mess->confix == 255) {
        ahdrp->to = 0;
        goto lab2;
    }
/* If we don't have the target hardware address we use ARP to get it.
*/
    confix = netconf[mess->confix].nexthix;
    confp = &netconf[confix];
    if (confp->hwaddyes == 0) {
#ifdef ARP
        if ((mp = Ngetbuf()) == 0)
            goto lab8;
        mp->netno = netno;
        mp->confix = confix;
        ussARPTable.writE(-1, mp);
        ((struct Ahdr *) ((char *) mp + MESSH_SZ))->to = 0;
        ((struct Ahdr *) ((char *) mp + MESSH_SZ))->protocol = AR_ARP;
        mp->id = bRELEASE;
        i1 = netp->protoc[1]->writE(netno, mp);
        if (i1)
            Nrelbuf(mp);
/* Write from network task can not wait.  We mark the message to be sent when
   the ARP reply arrives.  Only one message can be waiting, the rest have to
   be resent.  If no reply arrives, the waiting entry will be freed by the
   network task.
*/
        if (conno < 0) {
            i1 = ETIMEDOUT;
            BLOCKPREE();
            if (confp->ARPwait == 0) {
                confp->ARPwait = (char) conno;
                confp->ARPwaitmp = mess;
                confp->timems = TimeMS();
                i1 = 0;
            }
            RESUMEPREE();
            return i1;
        }
        WAITFOR(confp->hwaddyes, SIG_ARP, AR_TOUT, i1);
        if (i1) {
    lab8:   mess->offset = boTXDONE;
            return NE_TIMEOUT;
        }
        ahdrp = (struct Ahdr *) ((char *) mess + MESSH_SZ);
#else
#if NTRACE >= 1
        Nprintf("no ARP and ARCNET node id 0\n");
#endif
        return EHOSTUNREACH;
#endif
    }
    ahdrp->to = confp->Eaddr.c[0];

/* build the header */

lab2:
    ahdrp->from = netp->id.c[0];
    ahdrp->split = 0;
    ahdrp->seqno = nets[netno].hw.chsout++;

#ifdef MIB2
    netp->ifOutOctets += mess->mlen - MESSH_SZ - LHDRSZ;
    if (ahdrp->to)
        netp->ifOutNUcastPkts++;
    else
        netp->ifOutUcastPkts++;
#endif

    us1 = mess->id;
    i1 = netp->protoc[1]->writE(netno, mess);
    if (i1 != 0 || us1 <= bWACK)
        return i1;
    WAITFOR(mess->offset == boTXDONE, SIG_WN(netno), AR_TOUT, i1);
    return 1;
}


/* ===========================================================================
   Screen an arrived message.  Give it to indicated higher level.
   Returns:
    -4  reassembly started, don't free buffer
    -3  call write
    -2  no further action
    -1  error occured
    n   please enter in queue number n
*/

static int      screen(MESS * mess)
{
    int             netno,
                    nxtlev,
                    stat;
    struct Ahdr    *ahdrp;

    ahdrp = (struct Ahdr *) ((char *) mess + MESSH_SZ);
    mess->conno = !ahdrp->to;   /* bcast indicator */
#ifdef MIB2
    nets[mess->netno].ifInOctets += mess->mlen - MESSH_SZ - LHDRSZ;
    if (ahdrp->to)
        nets[mess->netno].ifInUcastPkts++;
    else
        nets[mess->netno].ifInNUcastPkts++;
#endif
    switch (ahdrp->protocol) {
    default:
#ifdef MIB2
        nets[mess->netno].ifInUnknownProtos++;
#endif
        return -1;
    case AR_IP:
        nxtlev = IP;
        break;
    case AR_ARP:
        nxtlev = ARP;
        break;
#ifdef RARP
    case AR_RARP:
        nxtlev = RARP;
        break;
#endif
    }
    mess->offset = MESSH_SZ + LHDRSZ;
    stat = P_tab[nxtlev]->screen(mess);
    if (stat == -3)
        writE(mess->confix, mess);
    return stat;
}


/* ===========================================================================
   Initialize the link level for a network.  Calls the driver
   initialization.  Returns
    success: 1
    error:   -1
*/

static int      init(int netno, char *params)
{
    int             i1;
    struct NET     *netp;

    netp = &nets[netno];
    netp->id = netdata[netp->confix].Eaddr;
    netp->maxblo = AR_MAXLEN <= MAXBUF - MESSH_SZ ? AR_MAXLEN :
        MAXBUF - MESSH_SZ;
    i1 = netp->protoc[1]->init(netno, params);
    if (i1 < 0)
        return i1;
    netconf[netp->confix].Eaddr = netp->id;
    netp->sndoff = MESSH_SZ + 6;
    netp->tout = AR_TOUT;
#ifdef MIB2
    netp->ifType = 1;
#endif
    return i1;
}


/* ===========================================================================
   Shut down a network.  Calls the driver shutdown.
*/

static void     shut(int netno)
{
    nets[netno].protoc[1]->shut(netno);
}


/* ===========================================================================
   Open a connection.
*/

static int      opeN(int conno, int flags)
{
    return 1;
}


/* ===========================================================================
   Close a connection at the link level.
*/

static int      closE(int conno)
{
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size);
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
**  29-DEC-1998  RCA  Created stub version
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
   Protocol table. */

GLOBALCONST
PTABLE ussARCNETTable = {
    "ARCNET", init, shut, screen, opeN, closE,
        reaDD, writE, ioctl, AR_IP, LHDRSZ
};
