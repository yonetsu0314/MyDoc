/*---------------------------------------------------------------------
 * $Header: p:/presto2/shinc/usnets/RCS/local.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: local.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * SH2用TCP/IPドライバ・インクルード
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* LOCAL.H  Edition: 9  Last changed: 15-Sep-99,9:25:16  By: BRIANS */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995, 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway, Suite 100
    Hillsboro, Oregon 97124
    (503) 844-6614
    http://www.ussw.com

    LOCAL.H -- Local User-Level Configuration for USNET

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
    MODIFICATION HISTORY
    15-SEP-1999  BTS  Add: USSBUFALIGN added
    30-OCT-1998  BSK  Chg: PPP conditionally included
* * * * * * * * * * * * * * * * * * * * * * * * *
*/
#include <..\config.h>

/* ===========================================================================
   Maximum number of network connections in one host: ethernet cards,
   serial ports and so on. */
#define NNETS 1
/* ===========================================================================
   Maximum number of logical connections ("sockets") that can be concurrently
   open.  When estimating your need, consider that a TCP close leaves the
   connection block reserved for a few seconds. */
#define NCONNS 14
/* ===========================================================================
   Maximum entries in NETCONF, which serves as configuration table, ARP cache
   and routing table.  This must at least equal the total number of hosts
   active at any one time. */
#define NCONFIGS 20
/* ===========================================================================
   Number of message buffers.  USNET contains a large number of dynamic
   queues, so there is no exact right value for NBUFFS.  Too few buffers will
   hurt performance.  A rule of thumb: 5 for each active physical or logical
   connection. */
#define NBUFFS 50
/* ===========================================================================
   Message buffer size, 36 + largest packet size.  (But if you use the ODI
   driver, 92 + packet size.)  Maximum packet for Ethernet 1500, ARCNET 1500,
   SLIP 576 (not firm), PPP 1500 (not firm). */
#define MAXBUF 1592

/* ===========================================================================
   Number of bytes to align Start of buffers to.  Also aligns the start
   of the Data area after the MESSH structure in a buffer.
   Always use MESSH_SZ when refering to the data offset of a buffer !!!

   This is a processor specific option critical to the operation of the
   Protocol Stack.  It should not be changed from the default without a
   thorough understanding of the impacted data structures.
*/
#define USSBUFALIGN 16
/*#define USSBUFALIGN 20*/

/* ===========================================================================
   Variable defining if the code to fragment and reassemble IP messages is
   included.  Do not fragment packets if you can help it.
    0 = neither
    1 = reassembly
    3 = fragmentation and reassembly */
#define FRAGMENTATION 3
/* ===========================================================================
   IP option support.  This is required in the standard, but little used and
   perhaps getting obsolete.  Requires 90 bytes extra per connection block. */
#define IPOPTIONS
/* ===========================================================================
   IP Multicasting Conformance Level.  Set as follows:
   Level 0: No support for IP multicasting
   Level 1: Support for sending but not receiving multicast IP datagrams
   Level 2: Full support for IP multicasting */
#define USS_IP_MC_LEVEL 0
/* ===========================================================================
   BSD socket keepalive time, milliseconds.  Default 2 hours but unactive as
   required by the standard.  To use, uncomment the line, change the value as
   needed.  Do not use this feature if you can help it. Must be defined as
   an unsigned long to prevent problems during comparisons. */
/* #define KEEPALIVETIME 7200000UL */
/* ===========================================================================
   Variable defined if information for the MIB II database for SNMP is
   collected.  Undefine if SNMP not used. */
#define MIB2
/* ===========================================================================
   Define as 1 if host is to relay, 2 if not.  The TCP/IP standard requires
   relaying to be off as a default. */
#define RELAYING 2
/* ===========================================================================
   For speed we do checksum in assembler */
#define chksum_INASM
/* ===========================================================================
   DHCP code, 1 = code included, 2 = called automatically.  Dynamic Host
   Configuration Protocol is used to get the IP address and other parameters
   from a DHCP server, for instance Windows NT. */
#define DHCP 2
/* ===========================================================================
   DNS resolver, 1 = code included, 2 = called automatically.  The Domain Name
   System resolver supports global host names, such as ussw.com.  At least one
   DNS server must be defined, either automatically with DHCP, or with the
   flag DNSVER in the configuration file NETCONF.C. */
#undef DNS
/*#define DNS 1*/
/* ==========================================================================
   The USNET stack must know that PPP code may or may not be included. */
#undef PPP
/* ===========================================================================
   Local host name and login password */
/*#define LOCALHOSTNAME(val) strcpy(val, "none")*/
#define LOCALHOSTNAME(val)

#define USERID_LEN 12
#define PASSWD_LEN 12
extern unsigned char Usnet_UserID[USERID_LEN+1];
extern unsigned char Usnet_PassWd[PASSWD_LEN+1];
#define USERID Usnet_UserID
#define PASSWD Usnet_PassWd

/* ===========================================================================
   local initialization and termination code for Ninit() and Nterm() */
int             Nclkinit(void); /* timer setup routine */
void            Nclkterm(void);
#define LOCALSETUP() Nclkinit()
#define LOCALSHUTOFF() Nclkterm()
