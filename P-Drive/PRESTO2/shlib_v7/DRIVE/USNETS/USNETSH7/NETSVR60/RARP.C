/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/rarp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: rarp.c $
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
/* RARP.C  Edition: 4  Last changed: 22-Apr-99,19:53:38  By: STEVE */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    RARP.C -- Reverse Address Resolution Protocol for USNET

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

  22-APR-1999  SCD  Add macro to select RARP server capability in screen
  31-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#define MAXDELAY 30000          /* maximum delay in retry loop */

/* We assume that the compiler will not place any padding into the following
   structure.  (If it did, structures would be pretty much useless.)
*/
struct ARM {                    /* address resolution message */
    char message_header[MESSH_SZ];
    char link_header[LHDRSZ];
    short hardware;             /* 1 = Ethernet */
    short protocol;             /* 0800 = Internet */
    char halen, ialen;          /* lengths of addresses */
    short opcode;               /* 1=request, 2=reply */
    unsigned char shadd[Eid_SZ];/* sender hardware address */
    unsigned char siadd[Iid_SZ];/* sender Internet address */
    unsigned char thadd[Eid_SZ];/* target hardware address */
    unsigned char tiadd[Iid_SZ];/* target Internet address */
};
#define ARM_SZ 28               /* size of structure */
#define EA_REQ NC2(3)           /* RARP request */
#define EA_RES NC2(4)           /* RARP response */
#define ET_IP NC2(0x0800)       /* Internet message */
#define ET_RARP NC2(0x8035)     /* reverse address request message */

#define MH(mb) ((struct ARM *)mb)

extern struct NET nets[];
extern struct NETCONF netconf[];

/* ===========================================================================
   Define to enable RARP reply.  RARP request is always enabled */
#define RARPSERVER


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  RARPget()      Get IP address
**
**  int RARPget(int netno);
**
**  PARAMETERS:
**     (in)     netno    netno to do RARP request on
**
**  DESCRIPTION:
**     Subroutine to get our own IP address from a RARP server.  Called
**     by user, not automatically.  There is no guarantee that RARP a
**     server is available, and the user may have different methods
**     (BOOTP, DHCP) in mind, so we can't make this automatic.
**
**  RETURNS:
**      1         RARP done
**      0         RARP in progress
**    < 0         error code
**
**  USAGE/COMMENTS:  
**     If the local IP address is not zero, routine will return immediately.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  SCD  Added function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int RARPget(int netno)
{
    int i1, conno, stat;
    unsigned long delay;
    struct NET *netp;
    struct NETCONF *confp;

    netp = &nets[netno];
    confp = &netconf[netp->confix];
    if (confp->Iaddr.l != 0)
        return 1;
    conno = Nopen("*", "RARP", 0, 0, 0);
    if (conno < 0)
        return conno;
    for (delay = netp->tout; delay < MAXDELAY; delay <<= 1) {
        stat = Nwrite(conno, 0, 0);
        if (stat < 0)
            break;
        WAITFOR(confp->Iaddr.l != 0, SIG_WN(netno), delay, stat);
        if (stat == 0) {
            stat = 1;
            break;
        }
        stat = ETIMEDOUT;
    }
    Nclose(conno);
    return stat;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  writE()      Write routine
**
**  int RARPget(int netno);
**  int writE(int conno, MESS *mess)
**
**  PARAMETERS:
**     (in)     conno    USNET connection number
**     (in)     mess     pointer to message buffer to be written
**
**  DESCRIPTION:
**     This function creates a header and calls the lower level write.
**
**  RETURNS:
**     0 <      Success - number of characters sent
**     -1       Error
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  SCD  Added function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int writE(int conno, MESS *mess)
{
    mess->offset = MESSH_SZ + LHDRSZ;
    mess->mlen = MESSH_SZ + LHDRSZ + ARM_SZ;
    mess->confix = 255;
    MH(mess)->opcode = EA_REQ;
    MH(mess)->hardware = NC2(1);
    MH(mess)->protocol = ET_IP;
    MH(mess)->halen = Eid_SZ;
    MH(mess)->ialen = Iid_SZ;
/* "target" address is our own in RARP */
    memcpy((char *)&MH(mess)->thadd,
        (char *)&nets[connblo[conno].netno].id, Eid_SZ);
    memcpy((char *)&MH(mess)->shadd, (char *)&MH(mess)->thadd, Eid_SZ);
    memset((char *)&MH(mess)->tiadd, 0, Iid_SZ);
    *(short *)((char *)mess + MESSH_SZ+12) = ET_RARP;
    return nets[mess->netno].protoc[0]->writE(conno, mess);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  screen()      Screen an arrived message
**
**  int screen(MESS * mess);
**
**  PARAMETERS:
**     (in)     mess     pointer to message buffer to be screen
**
**  DESCRIPTION:
**     Screen an arrived message.  If we know the requesting host's
**     Internet address, we reply.  If we don't, we take it from the
**     message.
**
**  RETURNS:
**      -3      call write
**      -2      no further action
**      -1      error occured
**       n      please enter in queue number n
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  SCD  Add macro to select RARP server capability
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int screen(MESS *mess)
{
    int i1, status;
    struct NETCONF *confp;
    struct NET *netp;

    status = -2;
    netp = &nets[mess->netno];
    for (i1 = 0; i1 < NCONFIGS; i1++) {
        confp = &netconf[i1];
        if (memcmp((char *)&confp->Eaddr, (char *)&MH(mess)->thadd,
                MH(mess)->halen) == 0) {
            if (confp->Iaddr.l == 0) {
                memcpy((char *)&confp->Iaddr, (char *)&MH(mess)->tiadd,
                    Iid_SZ);
                WAITNOMORE(SIG_WN(mess->netno));
            }
#ifdef RARPSERVER
            if (MH(mess)->opcode == EA_REQ) {
                memcpy((char *)&MH(mess)->tiadd, (char *)&confp->Iaddr, Iid_SZ);
                memcpy((char *)&MH(mess)->shadd, (char *)&netp->id, Eid_SZ);
                memcpy((char *)&MH(mess)->siadd,
                    (char *)&netconf[netp->confix].Iaddr, Iid_SZ);
                MH(mess)->opcode = EA_RES;
                status = -3;
            }
#endif
#if NTRACE >= 1
            Nprintf("RARP %02x%02x%02x%02x%02x%02x -> %d.%d.%d.%d\n",
                confp->Eaddr.c[0], confp->Eaddr.c[1], confp->Eaddr.c[2],
                confp->Eaddr.c[3], confp->Eaddr.c[4], confp->Eaddr.c[5], 
                confp->Iaddr.c[0], confp->Iaddr.c[1], confp->Iaddr.c[2],
                confp->Iaddr.c[3]);
#endif
            break;
        }
    }
    return status;
}


/* ===========================================================================
   Open a connection. 
*/

static int opeN(int conno, int flag)
{
    (void) conno, (void) flag;  /* quietly ignore these arguments */
    return 0;
}


/* ===========================================================================
   Close a connection. 
*/

static int closE(int conno)
{
    (void) conno;  /* quietly ignore this argument */
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
   Protocol table. */

GLOBALCONST
PTABLE ussRARPTable = {"RARP", 0, 0, screen, opeN, closE, reaDD, writE,
    ioctl, ET_RARP, 0};

