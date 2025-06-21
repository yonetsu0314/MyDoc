/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/telnet.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: telnet.h $
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
/* TELNET.H  Edition: 2  Last changed: 17-Feb-98,15:02:20  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    TELNET.H -- Telnet Definitions for USNET

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

/* Network Virtual Terminal(NVT) cotrol codes */

#define IAC	255                 /* interpret as controls  (=escape) */

/* command code */

#define SE	240                 /* end of subnegotiation */
#define SB	250                 /* subnegotiation */
#define WILL	251             /* I will do */
#define WONT	252             /* I refuse */
#define DO	253                 /* please do */
#define DONT	254             /* please stop doing */

/* printer code */
#define NUL	0                 	/*  */
#define LF	10                 	/*  */
#define CR	13                 	/*  */
#define BEL	7                 	/*  */
#define BS	8                 	/*  */
#define HT	9                 	/*  */
#define VT	11                 	/*  */
#define FF	12                 	/*  */

/* option codes */

#define ECHO		1           /* echo */
#define	NOGOAHEAD	3           /* suppress go-ahead */
#define TERMTYPE	24          /* terminal type */
#define EOROPT		25          /* end of record */

/* suboption codes */

#define ASCII_TERM      1       /* ASCII terminal emulator */
#define SEND            1       /* send */
