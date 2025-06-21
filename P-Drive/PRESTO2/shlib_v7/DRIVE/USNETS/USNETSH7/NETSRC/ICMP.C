/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/icmp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: icmp.c $
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
/* ICMP.C  Edition: 9  Last changed: 24-May-99,10:57:54  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ICMP.C -- Internet Control Message Protocol for USNET

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

  30-APR-1999  SCD  Replace assignments with Nmemcpy() in screen()
  21-APR-1999  NJW  Added host table check for ICMP redirects in screen()
  31-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "icmp.h"

#define IP_ICMP 1               /* protocol code for IP */

extern struct NET nets[];
extern struct NETCONF netconf[];
extern struct CONNECT connblo[];
extern PTABLE  *const P_tab[];
extern PTABLE   ussICMPTable,
                ussIPTable;

#ifdef MIB2
struct ICMPgroup ICMPgroup;
#endif

#ifdef ICMP

/* ===========================================================================
   Open function. */

static int opeN(int conno, int flags)
{
    return connblo[conno].protoc[1]->opeN(conno, flags);
}


/* ===========================================================================
   Close function. */

static int closE(int conno)
{
    return connblo[conno].protoc[1]->closE(conno);
}


/* ===========================================================================
   Write routine.  Calls the lower-level write.  Returns:
        success: number of characters sent
        error:   -1
*/

static int writE(int conno, MESS * mess)
{
    int             mlen;
    struct ICMPhdr *hdrp;

    hdrp = (struct ICMPhdr *) ((char *) mess + mess->offset);
#ifdef MIB2
    ICMPgroup.icmpOutMsgs++;
    if (hdrp->type < sizeof(ICMPgroup.icmptx) / sizeof(long))
        ICMPgroup.icmptx[hdrp->type]++;
#endif
    mlen = mess->mlen - mess->offset;
    hdrp->chksum = 0;
    if (mlen & 1)
        *((char *) hdrp + mlen) = 0;
    hdrp->chksum = ~Nchksum((unsigned short *) hdrp, (mlen + 1) / 2);
    /* protocol code for IP */
    *((char *) mess + MESSH_SZ + LHDRSZ + 9) = IP_ICMP;
    return ussIPTable.writE(conno, mess);
}

/* routine to send an ICMP reply
*/
void ICMPreply(MESS * mess, int type, int scode)
{
    int             ihdrsz;
    struct ICMPhdr *icmpp;

/* not for non-first fragments */
    if (*(short *) ((char *) mess + MESSH_SZ + LHDRSZ + 6) & NC2(0x1fff))
        return;
    mess->offset = MESSH_SZ + LHDRSZ + Ihdr_SZ;
    icmpp = (struct ICMPhdr *) ((char *) mess + MESSH_SZ + LHDRSZ + Ihdr_SZ);
    ihdrsz = (*((char *) mess + MESSH_SZ + LHDRSZ) & 0xf) << 2;
    Nmemcpy((char *) &icmpp->param[1], (char *) mess + MESSH_SZ + LHDRSZ, ihdrsz + 8);
    icmpp->type = type;
    icmpp->scode = scode;
    *(short *) icmpp->param = 0;
/* type 3/4 = can't fragment stores the block size, based in mess->mlen by
   the caller
*/
    if (type != 3 || scode != 4)
        mess->mlen = 0;
    *((short *) icmpp->param + 1) = mess->mlen;
    mess->mlen = ihdrsz + 8 + MESSH_SZ + LHDRSZ + Ihdr_SZ + ICMPhdr_SZ;
    writE(-1, mess);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
**  screen()     ICMP screening function
**
**  int screen( MESS *mess );
**
**  PARAMETERS:
**     (in)     mess     Pointer to the message buffer to be screened
**
**  DESCRIPTION:
**     Checks if the ICMP message should be handled.  If so,
**     the proper response is generated for the ICMP message.
**
**  RETURNS:
**      n   please enter in queue number n
**     -3   call write
**     -2   no further action
**     -1   error occured
**
**  USAGE/COMMENTS:
**     Called from lower layer screen() function, typically Ethernet 
**     screen().  This is an internal USNET function and is not called
**     by application layer code.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
**  MODIFICATION HISTORY:
**
**  30-APR-1999  SCD  Replace assignments with Nmemcpy() in redirect
**  21-APR-1999  NJW  Added host check on ICMP redirect.  If host acquired
**                      update acquisition of associated routers.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int screen(MESS * mess)
{
    int             i1,
                    conno,
                    mlen,
                    confix,
                    confix2,
                    myport,
                    herport;
    Iid             iid;
    struct CONNECT *conp;
    struct ICMPhdr *hdrp;

#ifdef MIB2
    ICMPgroup.icmpInMsgs++;
#endif
    hdrp = (struct ICMPhdr *) ((char *) mess + mess->offset);
    mlen = mess->mlen - mess->offset;
    if (mlen & 1)
        *((char *) hdrp + mlen) = 0;
    if (Nchksum((unsigned short *) hdrp, (mlen + 1) / 2) != 0xffff)
        goto err1;
    if (hdrp->type == IC_ECHO)
        goto lab2;

    /* Look for the connection block.  If the user specified ICMP,
       we give the message to the user. */

    for (conno = 0; conno < NCONNS; conno++) {
        conp = &connblo[conno];
        if (conp->blockstat == 0)
            continue;
        if (mess->target != conp->heriid.l &&
            conp->heriid.l != 0xffffffff &&
          conp->heriid.l != (mess->target | ~netconf[mess->confix].Imask.l))
            continue;
        if (conp->protoc[0] == &ussICMPTable)
            return conno;
    }
lab2:
#if NTRACE >= 2
    Nprintf("ICMP: type %d code %d\n", hdrp->type, hdrp->scode);
#endif
#ifdef MIB2
    if (hdrp->type < sizeof(ICMPgroup.icmprx) / sizeof(long))
        ICMPgroup.icmprx[hdrp->type]++;
#endif
    switch (hdrp->type) {
    case IC_ECHO:               /* echo request */
        hdrp->type = IC_EREP;
        goto snd;
    case IC_QUEN:               /* quench, force slow start */
    case IC_TIMEX:              /* time exceeded */
    case IC_UNRE:               /* destination unreachable */
    case IC_PARAM:              /* parameter problem */
        /* look for an originating connection block */
        i1 = *((char *) hdrp + 17);
        myport = *(unsigned short *) ((char *) hdrp + ICMPhdr_SZ + Ihdr_SZ);
        herport = *(unsigned short *) ((char *) hdrp + ICMPhdr_SZ + Ihdr_SZ + 2);
        Nmemcpy(iid.c, (char *) hdrp + ICMPhdr_SZ + 16, Iid_SZ);
        for (conno = 0; conno < NCONNS; conno++) {
            conp = &connblo[conno];
            if (conp->blockstat == 0 || conp->myport != myport ||
                conp->herport != herport)
                continue;
            if (mess->netno != conp->netno)
                continue;
            if (conp->protoc[0]->Eprotoc != i1)
                continue;
            if (iid.l != conp->heriid.l)
                continue;
            /* found it here, mark state */
            if (hdrp->type == IC_PARAM)
                conp->rxstat |= S_FATAL;
            else if (hdrp->type == IC_UNRE) {
                conp->rxstat |= S_UNREA;
                if (hdrp->scode >= 1 && hdrp->scode <= 4)
                    conp->rxstat |= S_FATAL;
                WAITNOMORE(SIG_RC(conno));
            } else if (hdrp->type == IC_TIMEX) {    /* time exceeded */
                conp->rxstat |= S_TIMEX;
                WAITNOMORE(SIG_RC(conno));
            } else if (hdrp->type == IC_QUEN) { /* quench, force slow start */
                conp->wackslow = conp->wackmax / 2;
                if (conp->wackslow < MAXWACK)
                    conp->wackslow = MAXWACK;
                conp->wackmax = MAXWACK;
            }
            break;
        }
        return -2;
    case IC_REDI:               /* redirect */
        /* find/create entry for new router, if fails ignore request */
        Nmemcpy(iid.c, (char *) &hdrp->param[0], Iid_SZ);
        if ((confix = GetHostData(iid.l, 3)) < 0)
            break;
            
       /*  Try adding an entry for host.  The IP rule is to treat all
       **  ICMP routing as host-specific, so the subnet mask will be
       **  zero.  This is because ICMP can't tell you what the mask is;
       **  we could try to guess but the rule says no.
       */
        Nmemcpy(iid.c, (char *) &hdrp->param[5], Iid_SZ);
        if ((confix2 = GetHostData(iid.l, 3)) < 0)
            break;

        /*
        **  Check if host is currently being used.  If so, then
        **  acquire the new routers host entry and release the
        **  old routers entry.
        */
        if ( ussHostUsed(confix2) > 0 ) {   /* If host acquired */
          ussHostAcquire(confix);           /*  secure new router entry */
          if( netconf[confix2].nexthix !=   /*  Does not route to self */ 
              confix2 )                     /*    then release old router */
            ussHostRelease( netconf[confix2].nexthix); 
        }

        netconf[confix2].nexthix = confix;
        return -2;
    case IC_TIMEQ:
        if ((netconf[nets[mess->netno].confix].flags & TIMESERVER) == 0)
            break;
        hdrp->type = IC_TIMER;
        iid.l = TimeMS();
        PUTLONG(iid, hdrp->param[0]);
snd:    mess->id = bRELEASE;
        if (writE(-1, mess))
            return -2;
        return -4;
    case IC_TIMER:
        GETLONG(iid, hdrp->param[0]);
        SetTimeMS(iid.l);
        return -2;
    }
err1:
#ifdef MIB2
    ICMPgroup.icmpInErrors++;
#endif
    return -1;
}


/* ===========================================================================
   Read routine.  Calls the lower level read.  Returns:
        success: message address
        error:   0
*/

static MESS *reaD(int conno)
{
    return ussIPTable.reaD(conno);
}


/* ===========================================================================
   Initialize the protocol.
*/

static int init(int netno, char *params)
{
    (void) netno, (void) params;  /* quietly ignore these arguments */
#ifdef MIB2
    memset(&ICMPgroup, 0, sizeof(ICMPgroup));
#endif
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
   Protocol table for ICMP. */

GLOBALCONST
PTABLE ussICMPTable = {
    "ICMP", init, 0, screen, opeN, closE, reaD, writE, ioctl, IP_ICMP, 0
};

#endif
