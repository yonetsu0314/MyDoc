/*---------------------------------------------------------------------
 * $Header: p:/presto2/shinc/usnets/RCS/icmp.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: icmp.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * SH2用TCP/IPドライバ・インクルード
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* ICMP.H  Edition: 3  Last changed: 17-Feb-98,15:04:18  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ICMP.H -- Internet Control Message Protocol Definitions for USNET

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
*/

struct ICMPhdr {                /* ICMP header format */
    char            type;       /* message type */
    char            scode;      /* status code */
    unsigned short  chksum;     /* checksum */
    unsigned NLONG  param[6][4 / sizeof(NLONG)];    /* parameters */
};
#define ICMPhdr_SZ 8            /* size of header */
#define IC_EREP 0               /* echo reply */
#define IC_UNRE 3               /* destination unreachable */
#define IC_QUEN 4               /* source quench */
#define IC_REDI 5               /* redirect */
#define IC_ECHO 8               /* echo request */
#define IC_TIMEX 11             /* time exceeded */
#define IC_PARAM 12             /* paramameter unintelligible */
#define IC_TIMEQ 13             /* time query */
#define IC_TIMER 14             /* time reply */
#define IC_INREQ 15             /* information request */
#define IC_INREP 16             /* information reply */
