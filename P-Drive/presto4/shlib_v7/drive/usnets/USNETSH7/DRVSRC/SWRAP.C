/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/swrap.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: swrap.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* SWRAP.C  Edition: 2  Last changed: 12-Nov-99,8:41:56  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1999 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    SWRAP.C     Serial WRAP driver

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

    12-NOV-1999  BSK  Fix for Multitasking
    25-AUG-1999  BSK  Created

 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <net.h>
#include <local.h>
#include <support.h>

extern struct NET nets[];

/*
** * * * * * *
** writE()    Write a frame from HDLC layer output to input
**
** static int writE(int conno, MESS *mess);
**
** PARAMETERS:
**  (in) int conno              A connection number
**  (in) MESS *mess             A buffer to write
**
** RETURNS
**  0                           Packet enqueued for link
**
** DESCRIPTION:
**  This function will:
**      Enqueue a driver packet
**      Simulate serial driver output/input interrupts by feeding
**          outgoing data directly to the incoming channel.
**
** USAGE/COMMENTS:
**  This is for use only with SLIP and PPP.
**
**  This is very useful for debugging the PPP AHDLC framing and other
**  such things.
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  12-NOV-1999  BSK  Use BLOCKPREE()/RESUMEPREE() for multitasking
**  25-AUG-1999  BSK  Created
**
** * * * * * *
*/
static int writE(int conno, MESS *mess)
{
    struct NET *netp = &nets[mess->netno];

   /* Ready packet for driver */
    mess->offset = mess->netno;
    QUEUE_IN(netp, depart, mess)

   /* Feed link layer outgoing serial data to link layer incoming */
    BLOCKPREE();
    while ((conno = netp->hw.goingc(netp)) != -1)
        netp->hw.comec(conno, netp);
    RESUMEPREE();

   /* Let nettask() run */
    YIELD();
    YIELD();
    return 0;
}

/* init() and shut() don't do anything for the SWRAP driver */
static int init(int netno, char *params) { return 0; }
static void shut(int netno) { return; }

GLOBALCONST PTABLE ussSWrapTable = {
    "swrap", init, shut, 0, 0, 0, 0, writE, 0, 0, MESSH_SZ
};

