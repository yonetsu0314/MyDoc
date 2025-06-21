/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/slip.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: slip.c $
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
/* SLIP.C  Edition: 5  Last changed: 8-Apr-99,11:19:42  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    SLIP.C -- Serial Line Internet Protocol (Link Layer) for USNET

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.

 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   08-APR-1999  BSK  goingc() WAITNOMORE fix; comments
 **   31-DEC-1998  RCA  Added ioctl stub
 ** * * * * * *
 **/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#define END        0300    /* indicates end of packet */
#define ESC        0333    /* indicates byte stuffing */
#define ESC_END    0334    /* ESC ESC_END means END data byte */
#define ESC_ESC    0335    /* ESC ESC_ESC means ESC data byte */
#define SL_MAXLEN 576      /* block size, must support TCP 536 byte segment */

extern struct NET nets[];
extern struct CONNECT connblo[];
extern PTABLE ussIPTable;


/** * * * * * *
 ** writE()  External/Internal function to write out a frame
 **
 ** static int writE(int conno, MESS *mess);
 **
 ** PARAMETERS:
 **   (in) conno                A USNET connection number
 **   (in/out) mess             A pointer to a buffer to write out
 **
 ** RETURNS:
 **  -?                         Cannot queue packet or driver error
 **   0                         Frame taken care of (queued)
 **   1                         Frame sent immediately
 **
 ** DESCRIPTION:
 **   This function will frame and write a buffer.
 **
 ** USAGE COMMENTS:
 **   The error codes must be watched by the caller as they may specify
 **   a resultant action.
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   08-APR-1999  BSK  Added comment
 ** * * * * * */
static int writE(int conno, MESS *mess)
{
    int i1, netno;
    unsigned short us1;

    (void)conno;
    netno = mess->netno;
#ifdef MIB2
    nets[netno].ifOutOctets += mess->mlen - mess->offset;
    nets[netno].ifOutUcastPkts++;
#endif
    us1 = mess->id;
    i1 = nets[netno].protoc[1]->writE(netno, mess);
    if (i1 != 0 || us1 <= bWACK)
        return i1;
    WAITFOR(mess->offset==boTXDONE, SIG_WN(netno), nets[netno].tout, i1);
    return 1;
}


/** * * * * * *
 ** screen()  Screen a message from the network.
 **
 ** static int screen(MESS *mess);
 **
 ** PARAMETERS:
 **   (in/out) mess             A pointer to a buffer to screen
 **
 ** RETURNS:
 **  -4                         Do not delete buffer
 **  -3                         Call write
 **  -2                         No further action
 **  -1                         Error occured
 **   n                         Enter in queue number n
 **
 ** DESCRIPTION:
 **   This function takes in a buffer and filters it up the stack if needed.
 **
 ** USAGE COMMENTS:
 **   This is called by nettask() upon packet reception and signal.
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   08-APR-1999  BSK  Added comment
 ** * * * * * */
static int screen(MESS *mess)
{
    int i1;

    mess->offset = MESSH_SZ + LHDRSZ;
    mess->conno = 0;            /* bcast indicator */
#ifdef MIB2
    nets[mess->netno].ifInOctets += mess->mlen - mess->offset;
    nets[mess->netno].ifInUcastPkts++;
#endif
    i1 = ussIPTable.screen(mess);
    if (i1 == -3)
        writE(mess->confix, mess);
    return i1;
}


/** * * * * * *
 ** comec()  Character input interrupt routine
 **
 ** static void comec(int cc, struct NET *netp);
 **
 ** PARAMETERS:
 **   (in) cc                   A character to input
 **   (in/out) netp             A pointer to a network structure
 **
 ** DESCRIPTION:
 **   This function takes in a framed character and maps it to a
 **   data entity in the in buffer.
 **
 ** USAGE COMMENTS:
 **   This is only for use with a transmit interrupt set up by the driver.
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   08-APR-1999  BSK  Signal nettask whether arrive queue full or not
 ** * * * * * */
static void comec(int cc, struct NET *netp)
{
    MESS *mess;

    if (netp->hw.bufin == 0) {
        if ((netp->bufbas = NgetbufIR()) == 0)
            return;
        netp->hw.bufin = (char *)netp->bufbas + MESSH_SZ + LHDRSZ;
        *(short *)(netp->hw.bufin-2) = NC2(0x0800);
        netp->hw.buflim = (char *)netp->bufbas + MAXBUF;
    }
    else if (netp->hw.lastin == ESC) {
        netp->hw.lastin = 0;
        if (cc == ESC_END)
            cc = END;
        else
            cc = ESC;
        goto lab3;
    }
    netp->hw.lastin = cc;
    if (cc == ESC)
        return;
    if (cc == END) {
        mess = netp->bufbas;
        mess->mlen = netp->hw.bufin - (char *)mess;
        mess->netno = netp->netno;
        if (mess->mlen <= MESSH_SZ + LHDRSZ)
            return;
        if (QUEUE_FULL(netp, arrive))
            NrelbufIR(mess);
        else {
            QUEUE_IN(netp, arrive, mess);
        }
        WAITNOMORE_IR(SIG_RN(netp->netno));
        netp->hw.bufin = 0;
    }
    else {
lab3:
        if (netp->hw.bufin == netp->hw.buflim)
            netp->hw.bufin = (char *)netp->bufbas + MESSH_SZ + LHDRSZ;
        *netp->hw.bufin++ = cc;
    }
}

/** * * * * * *
 ** goingc()  Character output interrupt routine
 **
 ** static int goingc(struct NET *netp);
 **
 ** PARAMETERS:
 **   (in/out) netp             A pointer to a network structure
 **
 ** RETURNS:
 **  -1                         Finished or error
 **   char                      A character to send
 **
 ** DESCRIPTION:
 **   This function returns the next character to send out from the buffer.
 **
 ** USAGE COMMENTS:
 **   This is only for use with an interrupt set up by the driver.
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   08-APR-1999  BSK  Added comment
 ** * * * * * */
static int goingc(struct NET *netp)
{
    unsigned char cc;
    MESS *mess;

    if (netp->hwflags) {
        if (netp->hwflags & 2) {
            netp->hwflags -= 2;
            return netp->hw.nxtout;
        }
        if (netp->hwflags & 1) {
            netp->hwflags -= 1;
            return END;
        }
        netp->hwflags = 0;
        mess = netp->bufbaso;
        if (mess->offset == netp->netno) {
            mess->offset = boTXDONE;
            if (mess->id <= bWACK) {
                if (mess->id == bRELEASE) {
                    mess->id = bALLOC;
                    NrelbufIR(mess);
                }
            }
            else {
                WAITNOMORE_IR(SIG_WN(netp->netno));
            }
        }
    }
    if (netp->hw.chsout == 0) {
nxt:    if (QUEUE_EMPTY(netp, depart))
            return -1;
        QUEUE_OUT(netp, depart, mess);
        if (mess->offset != netp->netno)
            goto nxt;
        netp->bufbaso = mess;
        netp->hw.bufout = (char *)mess + MESSH_SZ + LHDRSZ;
        netp->hw.chsout = mess->mlen - MESSH_SZ - LHDRSZ;
        return END;
    }
    if (--netp->hw.chsout == 0)
        netp->hwflags = 5;
    cc = *netp->hw.bufout++;
    if (cc == END) {
        netp->hw.nxtout = ESC_END;
        netp->hwflags |= 2;
        cc = ESC;
    }
    else if (cc == ESC) {
        netp->hw.nxtout = ESC_ESC;
        netp->hwflags |= 2;
        cc = ESC;
    }
    return cc;
}


/** * * * * * *
 ** init()  Initialize the interface at the serial level.
 **
 ** static int init(int netno, char *params);
 **
 ** PARAMETERS:
 **   (in) netno                A USNET network number
 **   (in) params               A pointer to driver parameters.
 **
 ** RETURNS:
 **   1                         Success
 **  -1                         Error
 **
 ** DESCRIPTION:
 **   This function initializes the driver and the link layer for
 **   network I/O.  It is called by Portinit().
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   22-OCT-1998  BSK  Created comment
 ** * * * * * */
static int init(int netno, char *params)
{
    int i1;
    register struct NET *netp;

    netp = &nets[netno];
    netp->hw.comec = comec;
    netp->hw.goingc = goingc;
    netp->maxblo = (SL_MAXLEN <= (MAXBUF - MESSH_SZ - LHDRSZ)) ?
                    SL_MAXLEN :
                    MAXBUF - MESSH_SZ - LHDRSZ;
    i1 = netp->protoc[1]->init(netno, params);
    if (i1 < 0)
        return i1;
    netp->sndoff = 0;
    netp->netno = netno;
    netp->tout = netp->bps ? (60000000L / netp->bps) + 1000 : 5000;
#ifdef MIB2
    netp->ifType = 28;
#endif
    return i1;
}


/** * * * * * *
 ** shut()  De-initialize the interface at the serial level.
 **
 ** static void shut(int netno);
 **
 ** PARAMETERS:
 **   (in) netno                A USNET network number
 **
 ** DESCRIPTION:
 **   This function de-initializes the driver and the link layer from
 **   network I/O.  It is called by Portterm().
 **
 ** * * * * * *
 ** MODIFICATION HISTORY:
 **   22-OCT-1998  BSK  Created comment
 ** * * * * * */
static void shut(int netno)
{
    nets[netno].protoc[1]->shut(netno);
}


/** * * * * * *
 ** Open a connection.  This operation does not apply to the link layer
 ** * * * * * */
static int opeN(int conno, int flags)
{
    (void) conno;
    (void) flags;
    return 1;
}


/** * * * * * *
 ** Close a connection.  This operation does not apply to the link layer.
 ** * * * * * */
static int closE(int conno)
{
    (void) conno;
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg);
**
**  PARAMETERS:
**   (in) void *handle          A pointer to the network structure
**   (in) enum ioctlreq request The command to perform
**   (in/out) void *arg         Pointer to the parameters to the command
**   (in) size_t size           Type specifier for arg
**
**  RETURNS:
**     0                        Success
**     ussErrInval              Support for the request not implemented
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.
**
** * * * * * *
** MODIFICATION HISTORY:
**   31-DEC-1998  RCA  Created stub version
** * * * * * */
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;
    (void)request;
    (void)arg;
    (void)size;

    return ussErrInval;
}


/** * * * * * *
 ** Protocol table for SLIP
 ** * * * * * */
GLOBALCONST PTABLE ussSLIPTable = {
    "SLIP", init, shut, screen, opeN, closE, reaDD, writE, ioctl, 0, LHDRSZ
};

