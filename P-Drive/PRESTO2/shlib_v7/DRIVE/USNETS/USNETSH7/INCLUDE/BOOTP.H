/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/bootp.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: bootp.h $
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
/* BOOTP.H  Edition: 2  Last changed: 17-Feb-98,15:02:30  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    BOOTP.H -- Bootstrap Protocol Definitions for USNET

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

#define MAXDELAY 30000          /* maximum delay in retry loop */

#define T_RRQ NC2(1)            /* TFTP read file */
#define T_WRQ NC2(2)            /* TFTP write file */
#define T_DATA NC2(3)           /* TFTP data message */
#define T_ACK NC2(4)            /* TFTP ack message */
#define T_ERROR NC2(5)          /* TFTP error message */

struct BOOTP {                  /* structure for bootload message */
    unsigned char   op;         /* operation */
    unsigned char   htype;      /* hardware type */
    unsigned char   hlen;       /* hardware address length */
    unsigned char   hops;       /* gateway hops */
    unsigned long   xid;        /* transaction ID */
    unsigned short  secs;       /* seconds since boot began */
    unsigned short  unused;
    unsigned long   ciaddr;     /* client IP address */
    unsigned long   yiaddr;     /* 'your' IP address */
    unsigned long   siaddr;     /* server IP address */
    unsigned long   giaddr;     /* gateway IP address */
    unsigned char   chaddr[16]; /* client hardware address */
    unsigned char   host[64];   /* server host name */
    unsigned char   file[128];  /* boot file name */
    unsigned char   vend[64];   /* vendor-specific area */
};

int             BOOTPget(int netno);
int             BOOTPopen(int netno);
int             BOOTPread(int conno, char *buff, int len);
