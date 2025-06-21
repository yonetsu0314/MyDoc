/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/ns8390.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ns8390.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* NS8390.H  Edition: 2  Last changed: 17-Feb-98,15:36:34  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    NS8390.H -- National Semiconductor 8390 Hardware Definitions for USNET

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

/* Register descriptions for Ethernet board using the National
   Semiconductor Ethernet controller DP8390. */

#define REGPORT		0xb0000000		/* control registers port address */
#define DATAPORT	0x00040000		/* offset for data port */
#define RESET		0x00080000		/* offset for Issue a read for reset */
#define INTNO		14
/*#define W83CREG		0x00	/* I/O port definition */
/*#define	ADDROM		0x08	/**/

/* page 0 read or read/write registers */

#define	CMDR		0x00	
#define CLDA0  		0x02	/* current local dma addr 0 for read */
#define CLDA1  		0x04	/* current local dma addr 1 for read */
#define BNRY   		0x06	/* boundary reg for rd and wr */
#define TSR    		0x08	/* tx status reg for rd */
#define NCR    		0x0a	/* number of collision reg for rd */
#define FIFO   		0x0c	/* FIFO for rd */
#define ISR    		0x0e	/* interrupt status reg for rd and wr */
#define CRDA0  		0x10	/* current remote dma address 0 for rd */
#define CRDA1  		0x12	/* current remote dma address 1 for rd */
#define RSR    		0x18	/* rx status reg for rd */
#define CNTR0  		0x1a	/* tally cnt 0 for frm alg err for rd */
#define CNTR1  		0x1c	/* tally cnt 1 for crc err for rd */
#define CNTR2  		0x1e	/* tally cnt 2 for missed pkt for rd */

/* page 0 write registers */

#define PSTART 		0x02	/* page start register */
#define PSTOP  		0x04	/* page stop register */
#define TPSR   		0x08	/* tx start page start reg */
#define TBCR0  		0x0a	/* tx byte count 0 reg */
#define TBCR1  		0x0c	/* tx byte count 1 reg */
#define RSAR0  		0x10	/* remote start address reg 0  */
#define RSAR1  		0x12	/* remote start address reg 1 */
#define RBCR0  		0x14	/* remote byte count reg 0 */
#define RBCR1  		0x16	/* remote byte count reg 1 */
#define RCR    		0x18	/* rx configuration reg */
#define TCR    		0x1a	/* tx configuration reg */
#define DCR    		0x1c	/* data configuration reg */
#define IMR    		0x1e	/* interrupt mask reg */

/* page 1 registers */

#define PAR   		0x02	/* physical addr reg base for rd and wr */
#define CURR   		0x0e	/* current page reg for rd and wr */
#define MAR   		0x10	/* multicast addr reg base fro rd and WR */
#define MARsize	8		/* size of multicast addr space */

/*-----W83CREG command bits-----*/
/*#define MSK_RESET  0x80		/* W83CREG masks */
/*#define MSK_ENASH  0x40		/**/
/*#define MSK_DECOD  0x3F		/* memory decode bits, corresponding */
				/* to SA 18-13. SA 19 assumed to be 1 */

/*-----CMDR command bits-----*/
#define MSK_STP		0x0100	/* stop the chip */
#define MSK_STA		0x0200	/* start the chip */  
#define MSK_TXP		0x0400	/* initial txing of a frm */
#define MSK_RRE		0x0800	/* remote read */
#define MSK_RWR		0x1000	/* remote write */
#define MSK_RD2		0x2000	/* no DMA used */
#define MSK_PG0		0x0000	/* select register page 0 */
#define MSK_PG1		0x4000	/* select register page 1 */
#define MSK_PG2		0x8000	/* select register page 2 */

/*-----ISR status bits-----*/
#define MSK_PRX 	0x0100	/* rx with no error */
#define MSK_PTX 	0x0200	/* tx with no error */
#define MSK_RXE 	0x0400	/* rx with error */
#define MSK_TXE 	0x0800	/* tx with error */
#define MSK_OVW 	0x1000	/* overwrite warning */
#define MSK_CNT 	0x2000	/* MSB of one of the tally counters is set */
#define MSK_RDC 	0x4000	/* remote dma completed */
#define MSK_RST		0x8000	/* reset state indicator */

/*-----TSR status bits------*/
#define TSR_PTX		0x0100	/* Packet Transmitted */
#define TSR_COL		0x0400	/* Transmit Collided */
#define TSR_ABT		0x0800	/* Transmit Aborted */
#define TSR_CRS		0x1000	/* Carrier Sense */
#define TSR_FU		0x2000	/* FIFO Underrun */
#define TSR_CDH		0x4000	/* CD Heatbeat */
#define TSR_OWC		0x8000	/* Out of Window Collision */

/*-----DCR command bits-----*/
#define MSK_WTS		0x0100	/* word transfer mode selection */
#define MSK_BOS		0x0200	/* byte order selection */
#define MSK_LAS		0x0400	/* long addr selection */
#define MSK_BMS		0x0800	/* burst mode selection */
#define MSK_ARM		0x1000	/* autoinitialize remote */
#define MSK_FT00	0x0000	/* burst lrngth selection */
#define MSK_FT01	0x2000	/* burst lrngth selection */
#define MSK_FT10	0x4000	/* burst lrngth selection */
#define MSK_FT11	0x6000	/* burst lrngth selection */

/*-----RCR command bits-----*/
#define MSK_SEP		0x0100	/* save error pkts */
#define MSK_AR	 	0x0200	/* accept runt pkt */
#define MSK_AB		0x0400	/* 8390 RCR */
#define MSK_AM	 	0x0800	/* accept multicast  */
#define MSK_PRO		0x1000	/* accept all pkt with physical adr */
#define MSK_MON		0x2000	/* monitor mode */

/*-----TCR command bits-----*/
#define MSK_CRC		0x0100	/* inhibit CRC, do not append crc */
#define MSK_LOOP	0x0200	/* set loopback mode */
#define MSK_BCST	0x0400	/* Accept broadcasts */
#define MSK_LB01	0x0600	/* encoded loopback control */
#define MSK_ATD		0x0800	/* auto tx disable */
#define MSK_OFST	0x1000	/* collision offset enable  */

/*-----receive status bits-----*/
#define SMK_PRX   0x01		/* rx without error */
#define SMK_CRC   0x02		/* CRC error */
#define SMK_FAE   0x04		/* frame alignment error */
#define SMK_FO    0x08		/* FIFO overrun */
#define SMK_MPA   0x10		/* missed pkt */
#define SMK_PHY   0x20		/* physical/multicase address */
#define SMK_DIS   0x40		/* receiver disable. set in monitor mode */
#define SMK_DEF	  0x80		/* deferring */

/*-----transmit status bits-----*/
#define SMK_PTX   0x0100	/* tx without error */
#define SMK_DFR   0x0200	/* non deferred tx */
#define SMK_COL   0x0400	/* tx collided */
#define SMK_ABT   0x0800	/* tx abort because of excessive collisions */
#define SMK_CRS   0x1000	/* carrier sense lost */
#define SMK_FU    0x2000	/* FIFO underrun */
#define SMK_CDH   0x4000	/* collision detect heartbeat */
#define SMK_OWC	  0x8000	/* out of window collision */

#define SWAPS(val) ((((val) & 0xff) << 8) | ((unsigned short)(val) >> 8))
