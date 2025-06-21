/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/am79c90.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: am79c90.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* AM7990.H  Edition: 2  Last changed: 17-Feb-98,15:39:14  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    AM7990.H -- AMD 7990 Hardware Definitions for USNET

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

/* Register descriptions for Ethernet compatible with the Advanced Micro
   Devices Ethernet controller Am7990 (Lance). */

#define AM_PORTADDR		0xb0000000		/* port address for AM79C90 */
#define AM_BUFADDR		0xb0080000		/* buffer memory address for AM79C90 (SH3 side) */
#define AM_BUFADDR_AM	0x000000		/* buffer memory address for AM79C90 (79C90 side) */

#define AM_INTNO		14				/* int no for AM79C90 */

#define CSR 		0x00        /* command & status register offset */
#define CSR1 		0x00
#define CSR2 		0x00
#define CSR3 		0x00
#define RAP 		0x04        /* register address port */
/*#define RESET 		0x14        /* reset register, read only */

#define SELCSR0		0           /* select command & status register 0 */
#define SELCSR1		1           /* 1 */
#define SELCSR2		2           /* 2 */
#define SELCSR3		3           /* 3 */

/* bits of command & status register 0 (CSR0) */
#define ERR		0x8000          /* error summary bit */
#define BABL		0x4000      /* babble transmitter timeout error */
#define CERR		0x2000      /* collision error */
#define MISS		0x1000      /* missed packet */
#define MERR		0x0800      /* memory error */
#define RINT		0x0400      /* receiver interrupt */
#define TINT		0x0200      /* transmitter interrupt */
#define IDON		0x0100      /* initialization done */
#define INTR		0x0080      /* interrupt bit */
#define INEA		0x0040      /* interrupt enable */
#define RXON		0x0020      /* receiver on */
#define TXON		0x0010      /* transmitter on */
#define TDMD		0x0008      /* transmit demand */
#define STOP		0x0004      /* stop the chip */
#define STRT		0x0002      /* start the chip */
#define INIT		0x0001      /* initialize */

/* CSR1 contains low 16 bits of address of Initialization Block */

/* CSR2 contains in low byte high 8 bits of address of InitBlock */

/* bits of command & status register 3 (CSR3) */
/*#define RINTM	0x0400          /* receive interrupt mask */
/*#define TINTM	0x0200          /* transmit interrupt mask */
/*#define IDONM	0x0100          /* initialization done mask */
#define BCON		0x0001
#define ACON		0x0002
#define BSWP		0x0004

/* initialization block */
struct INITBL {
    unsigned short  mode;       /* mode bits, see below */
    unsigned char   eaddr[6];   /* Ethernet address */
    unsigned short  hash[4];    /* hash table */
    unsigned short  rxd16;      /* Rx descriptor address, 16 bits */
#ifdef LITTLE
    unsigned char   rxd8;       /* 8 bits */
    unsigned char   rxdlen;     /* upper 3 bits are log2 of RxD ring length */
#else
    unsigned char   rxdlen;     /* upper 3 bits are log2 of RxD ring length */
    unsigned char   rxd8;       /* 8 bits */
#endif
    unsigned short  txd16;      /* Tx descriptor address, 16 bits */
#ifdef LITTLE
    unsigned char   txd8;       /* 8 bits */
    unsigned char   txdlen;     /* upper 3 bits are log2 of TxD ring length */
#else
    unsigned char   txdlen;     /* upper 3 bits are log2 of TxD ring length */
    unsigned char   txd8;       /* 8 bits */
#endif
};
/* mode bits in the initialization block */
#define PROM	0x8000          /* promiscuous mode */
#define INTL	0x0040          /* internal loopback */
#define DRTY	0x0020          /* disable retry */
#define COLL	0x0010          /* force collision */
#define DTCR	0x0008          /* disable tranmsit CRC */
#define LOOP	0x0004          /* loopback */
#define DTX	0x0002              /* disable the transmitter */
#define DRX	0x0001              /* disable the receiver */

/* message descriptor entry */
struct MD {
    unsigned short  adr16;      /* 16 bits of buffer address */
#ifdef LITTLE
    unsigned char   adr8;       /* 8 bits of buffer address */
    unsigned char   flags;      /* see below */
#else
    unsigned char   flags;      /* see below */
    unsigned char   adr8;       /* 8 bits of buffer address */
#endif
    short           bcnt;       /* 2's complement of buffer byte count */
    unsigned short  cnterr;     /* RX: message byte count */
    /* TX: error bits */
};

/* bits in receive descriptor flags */
#define MD_OWN		0x80        /* owver bit  0 = host, 1 = Enet */
#define MD_ERR		0x40        /* error summary bit */
#define MD_FRAM		0x20        /* framing error */
#define MD_OFLO		0x10        /* overflow error */
#define MD_CRC		0x08        /* CRC error */
#define MD_BUFF		0x04        /* buffer error */
#define MD_STP		0x02        /* start of packet */
#define MD_ENP		0x01        /* end of packet */

/* bits in transmit descriptor flags */
#define MD_MORE		0x10
#define MD_ONE		0x08
#define MD_DEF		0x04

/* bits in transmit descriptor cnterr */
#define BUFF		0x8000
#define UFLO		0x4000
#define LCOL		0x1000
#define LCAR		0x0800
#define RTRY		0x0400
#define TDR		0x003F          /* mask for TDR */
