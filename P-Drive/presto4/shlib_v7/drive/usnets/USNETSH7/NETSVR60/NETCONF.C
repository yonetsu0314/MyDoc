/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/netconf.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: netconf.c $
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
/* NETCONF.C  Edition: 12  Last changed: 25-Aug-99,14:34:52  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com
*/
#include "net.h"
#include "local.h"
#include "support.h"


/* The following definitions can be used to fill in the subnet mask field
   for an entry in the netdata[] table.
*/

/* The following definition is the netdata[] table.  Each entry in the table
   contains the following fields:

      - host name: Must be less than eight characters
      - port or network name:  Must be less than eight characters
      - network class:   See A,B,C,X above.
      - Internet address
      - Ethernet address, only needed in RARP servers, and when Ethernet board
	has no address ROM
      - flags: 0, NOTUSED, ROUTER, DIAL, DNSVER
      - link layer: Ethernet, ARCNET, SLIP, PPP
      - driver, see README.TXT for selection
      - adapter, such us PCMCIA, 0 if none
      - driver initialization data, use data below as model

Hints for setting up this table:

    Leave in only valid entries.
    Entries on the same subnet should have the same network name.
    The entries that are commented out are intended as examples.
    Trying to initialize non-existant hardware usually causes a crash.
    For external hosts, like a UNIX workstation, the last 4 fields are unused
        and can be given as 0.
*/


