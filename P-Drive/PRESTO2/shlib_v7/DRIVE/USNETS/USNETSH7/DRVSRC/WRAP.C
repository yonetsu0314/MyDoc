/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/wrap.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: wrap.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* WRAP.C  Edition: 7  Last changed: 13-Sep-99,9:21:46  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    WRAP.C -- Wraparound Driver for USNET

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

* * * * * * * * * * * * * * * * * * * * * * * * *

  MODIFICATION HISTORY:

    13-SEP-1999  BSK  Release buffer in write()
    20-JAN-1999  SCD  Added function headers
    31-DEC-1998  RCA  Added ioctl stub

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include <net.h>
#include <local.h>
#include <support.h>

extern struct NET nets[];
extern struct NETCONF netconf[];


/*
** * * * * * *
** writE()      Wraparound write function
**
** int writE(int conno, MESS *mess);
**
** PARAMETERS:
**  (in)     conno  Not used in this function
**  (in)     mess   Pointer to message to be written to controller
**
** RETURNS:
**  0                           Packet transmitted
**
** DESCRIPTION:  
**  Wraparound write routine.  Gets a buffer, copies the message,
**  queues it as arrived.
**
** USAGE/COMMENTS:  
**  Called indirectly using pointer defined in PTABLE
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  13-SEP-1999  BSK  Release buffer after copy
**  20-JAN-1999  SCD  Add function header
**
** * * * * * *
*/
static int writE(int conno, MESS *mess)
{
    int i1, netno;
    struct NET *netp;
    MESS *mp;

    (void)conno;
    netno = mess->netno;

   /* Create and ready receiving packet */
    if ((mp = Ngetbuf()) == 0)
        return NE_NOBUFS;
    i1 = mess->mlen;
#ifdef PPP
    if (nets[netno].protoc[0] == PPP)
        i1 += 2;
#endif
    memcpy((void *)mp, (void *)mess, i1);
    mp->netno = netno;
    mp->id = bALLOC;
    netp = &nets[netno];
    QUEUE_IN(netp, arrive, mp);
    WAITNOMORE(SIG_RN(netno));

   /* Release sending packet */
    mess->offset = boTXDONE;
    if (mess->id <= bWACK) {
        if (mess->id == bRELEASE) {
            mess->id = bALLOC;
            Nrelbuf(mess);
        }
    }
    else
    {
        WAITNOMORE(SIG_WN(netno));
    }

   /* Let nettask() run */
    YIELD();
    YIELD();

    return 0;
}

/*
** * * * * * *
** opeN()      Open a connection
**
** int opeN(int conno, int flag);
**
** PARAMETERS:
**  (in)     conno  Not used in this function
**  (in)     flag   Not used in this function
**
** RETURNS:
**  0         always returns 0
**
** DESCRIPTION:  
**  Program stub 
**
** USAGE/COMMENTS:  
**  Maintains PTABLE format consistency with other stack layers
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * *
*/
static int opeN(int conno, int flags)
{
    (void)conno;
    (void)flags;
    return 1;
}

/*
** * * * * * *
** closE()      Close the connection
**
** int closE(int conno);
**
** PARAMETERS:
**  (in)     conno  Not used in this function
**
** RETURNS:
**  0         always returns 0
**
** DESCRIPTION:  
**  Program stub 
**
** USAGE/COMMENTS:  
**  Maintains PTABLE format consistency with other stack layers
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * *
*/
static int closE(int conno)
{
    (void)conno;
    return 0;
}

/*
** * * * * * *
** init()      Initialize controller
**
** int init(int netno, char *params);
**
** PARAMETERS:
**  (in)     netno   Index into nets[] array
**  (in)     params  Pointer to parameter list
**
** RETURNS:
**  0            Always returns 0
**
** DESCRIPTION:  
**  Set up a dummy physical address
**
** USAGE/COMMENTS:  
**  Called indirectly using pointer defined in PTABLE
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * *
*/
static int init(int netno, char *params)
{
    (void)params;
    memset((char *)&nets[netno].id, 0x76, Eid_SZ);
    return 0;
}

/*
** * * * * * *
** shut()      Shut down controller
**
** void shut(int netno);
**
** PARAMETERS:
**  (in) int netno              Index into nets[] array
**
** DESCRIPTION:
**  Restore the interrupt and turn off the controller
**
** USAGE/COMMENTS:  
**  Called indirectly using pointer defined in PTABLE
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Add function header
**
** * * * * * *
*/
static void shut(int netno)
{
    (void)netno;
}


/*
** * * * * * *
** ioctl()      I/O control function
**
** int ioctl(int handle, int request, void *arg);
**
** PARAMETERS:
**  (in)     handle   likely to be netno if any functions are added
**                    at this level
**  (in)     request  constant for the command to perform
**  (in)     arg      pointer to the parameters to the command, which
**                       will be specific to the command
**
** DESCRIPTION:
**  This function allows special purpose functions specific to this
**  network layer to be performed.  At the moment, there are no functions
**  defined for this layer.
**
** RETURNS:
**  0              Success
**  USS_ERR_INVAL  Support for the request not implemented
**
** USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** MODIFICATION HISTORY:
**
**  30-DEC-1998  RCA  Created stub version
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


/* Protocol table for wraparound driver. */
GLOBALCONST PTABLE ussWrapTable = {
    "wrap", init, shut, 0, opeN, closE, 0, writE, ioctl, 0, MESSH_SZ
};

