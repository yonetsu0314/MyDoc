/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/ethernet.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ethernet.h $
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
/* ETHERNET.H  Edition: 2  Last changed: 17-Feb-98,15:04:24  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ETHERNET.H -- Ethernet Definitions for USNET

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

/* definition file for Ethernet structures and values */

#define	ET_MINLEN 60            /* minimum message length */
#define	ET_MAXLEN 1500          /* maximum message length */
#define ET_TOUT 1000            /* basic timeout value */

/* We assume that the compiler will not place any padding into the following
   structure.  (If it did, structures would be pretty much useless.)
*/
struct Ehdr {                   /* Ethernet header */
    char            message_header[MESSH_SZ];
    unsigned char   to[Eid_SZ];
    unsigned char   from[Eid_SZ];
    unsigned short  type;
};
#define Ehdr_SZ 14              /* size of header */
#define	ET_IP NC2(0x0800)       /* Internet message */
#define	ET_ARP NC2(0x0806)      /* address request message */
#define	ET_RARP NC2(0x8035)     /* reverse address request message */
