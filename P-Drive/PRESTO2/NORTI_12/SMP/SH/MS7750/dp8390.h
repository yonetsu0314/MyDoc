/******************************************************************************
* NORTi  Network LAN Controller Definitions                                   *
* for National Semiconductor DP8390/8390x and Realtek RTL80x9(AS).            *
*                                                                             *
*  Copyright (c) 1999-2000, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
*  Written by Yasuo Tomita of Tygnus Computer Works.                          *
*                                                                             *
*  5/Apr/1999 Created to port to MS7709RP01 board.                       [YT] *
*  3/Oct/1999 Modified some codes to support NS DP83905 and DP83907.     [YT] *
*             Added support for Realtek RTL80x9(AS).                     [YT] *
******************************************************************************/

/*
 * This file contains register descriptions for National Semiconductor 
 * Ethernet controllers DP8390 (NIC), DP83901 (SNIC), DP83902/3 (ST-NIC), 
 * DP83905 (AT/LANTIC) and DP83907 (AT/LANTIC II).
 */

/************** Register Offsets **************************************/

	/* page 0 read or read/write registers */
#define	CMDR   		0x00	/* command (for read and write) */
#define CLDA0  		0x01	/* current local DMA address 0 (for read) */
#define CLDA1  		0x02	/* current local DMA address 1 (for read) */
#define BNRY   		0x03	/* boundary pointer (for read and write) */
#define TSR    		0x04	/* trnasmit status register (for read) */
#define NCR    		0x05	/* number of collision register (for read) */
#define FIFO   		0x06	/* FIFO (for read) */
#define ISR    		0x07	/* interrupt status register (for read and write) */
#define CRDA0  		0x08	/* current remote DMA address 0 (for read) */
#define CRDA1  		0x09	/* current remote DMA address 1 (for read) */
#define RSR    		0x0c	/* receive status register (for read) */
#define CNTR0  		0x0d	/* tally count 0 for frame alginment errors (for read) */
#define CNTR1  		0x0e	/* tally count 1 for CRC errors (for read) */
#define CNTR2  		0x0f	/* tally count 2 for missed packet errors (for read) */

	/* page 0 write registers */
#define PSTART 		0x01	/* page start register */
#define PSTOP  		0x02	/* page stop register */
#define TPSR   		0x04	/* trnasmit page start register */
#define TBCR0  		0x05	/* trnasmit byte count register 0 */
#define TBCR1  		0x06	/* trnasmit byte count register 1 */
#define RSAR0  		0x08	/* remote start address register 0 */
#define RSAR1  		0x09	/* remote start address register 1 */
#define RBCR0  		0x0a	/* remote byte count register 0 */
#define RBCR1  		0x0b	/* remote byte count register 1 */
#define RCR    		0x0c	/* receive configuration register */
#define TCR    		0x0d	/* trnasmit configuration register */
#define DCR    		0x0e	/* data configuration register */
#define IMR    		0x0f	/* interrupt mask register */

	/* page 1 registers */
#define PAR    		0x01	/* physical address register base */
#define PARsize		6		/* size of physical address space */
#define CURR   		0x07	/* current page register */
#define MAR    		0x08	/* multicast addr reg base fro rd and WR */
#define MARsize		8		/* size of multicast address space */

/********** Register Command/Status/Mask Bits *************************/

	/*----- CMDR command bits -----*/
#define CMDR_STP	0x01	/* stop the chip */
#define CMDR_STA	0x02	/* start the chip */  
#define CMDR_TXP	0x04	/* initiate transmission of a packet */
#define CMDR_RRE	0x08	/* remote read */
#define CMDR_RWR	0x10	/* remote write */
#define CMDR_RD2	0x20	/* abort/complte remote DMA (no DMA used) */
#define CMDR_PS0	0x00	/* select register page 0 */
#define CMDR_PS1	0x40	/* select register page 1 */
#define CMDR_PS2	0x80	/* select register page 2 */
#define CMDR_PS3	0xc0	/* select register page 3 (Realtek only) */

	/*----- ISR status bits -----*/
#define ISR_PRX 	0x01	/* packet received with no errors */
#define ISR_PTX 	0x02	/* packet trasmitted with no errors */
#define ISR_RXE 	0x04	/* received with errors */
#define ISR_TXE 	0x08	/* trasmitted with errors */
#define ISR_OVW 	0x10	/* overwrite warning */
#define ISR_CNT 	0x20	/* counter overflow (MSB of one of the tally counters is set) */
#define ISR_RDC 	0x40	/* remote DMA completed */
#define ISR_RST 	0x80	/* reset state indicator */

	/*----- IMR mask bits -----*/
#define IMR_PRXE 	0x01	/* packet received interrupt enable */
#define IMR_PTXE	0x02	/* packet trasmitted interrupt enable */
#define IMR_RXEE	0x04	/* received error interrupt enable */
#define IMR_TXEE	0x08	/* trasmit error interrupt enable */
#define IMR_OVWE	0x10	/* overwrite warning interrupt enable */
#define IMR_CNTE	0x20	/* counter overflow interrupt enable */
#define IMR_RDCE	0x40	/* remote DMA completeinterrupt enable */

	/*----- DCR command bits -----*/
#define DCR_WTS		0x01	/* word transfer mode select */
#define DCR_BOS		0x02	/* byte order select */
#define DCR_LAS		0x04	/* long address select */
#define DCR_LS		0x08	/* loopback select */
#define DCR_ARM		0x10	/* autoinitialize remote */
#define DCR_FT00	0x00	/* burst lrngth selection */
#define DCR_FT01	0x20	/* burst lrngth selection */
#define DCR_FT10	0x40	/* burst lrngth selection */
#define DCR_FT11	0x60	/* burst lrngth selection */

	/*----- RCR command bits -----*/
#define RCR_SEP		0x01	/* save error pkts */
#define RCR_AR	 	0x02	/* accept runt pkt */
#define RCR_AB		0x04	/* 8390 RCR */
#define RCR_AM	 	0x08	/* accept multicast  */
#define RCR_PRO		0x10	/* accept all pkt with physical adr */
#define RCR_MON		0x20	/* monitor mode */

	/*----- TCR command bits -----*/
#define TCR_CRC		0x01	/* inhibit CRC, do not append CRC */
#define TCR_LB01	0x02	/* internal NIC module loopback */
#define TCR_LB10	0x04	/* internal ENDEC module loopback */
#define TCR_LB11	0x06	/* external loopback control */
#define TCR_ATD		0x08	/* auto transmit disable */
#define TCR_OFST	0x10	/* collision offset enable  */

	/*----- receive status bits -----*/
#define RSR_PRX     0x01	/* packet received intact, without error */
#define RSR_CRC     0x02	/* CRC error */
#define RSR_FAE     0x04	/* frame alignment error */
#define RSR_FO      0x08	/* FIFO overrun */
#define RSR_MPA     0x10	/* missed pkt */
#define RSR_PHY     0x20	/* physical/multicase address */
#define RSR_DIS     0x40	/* receiver disable. set in monitor mode */
#define RSR_DEF     0x80	/* deferring */

	/*----- transmit status bits -----*/
#define TSR_PTX     0x01	/* transmit without error */
#define TSR_DFR     0x02	/* non deferred transmit */
#define TSR_COL     0x04	/* transmit collided */
#define TSR_ABT     0x08	/* transmit abort because of excessive collisions */
#define TSR_CRS     0x10	/* carrier sense lost */
#define TSR_FU      0x20	/* FIFO underrun */
#define TSR_CDH     0x40	/* collision detect heartbeat */
#define TSR_OWC     0x80	/* out of window collision */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
 * Additional register descriptions for DP83905 (AT/LANTIC) and 
 * DP83907 (AT/LANTIC II).
 */

/************** Register Offsets **************************************/

/* These registers are accessible in I/O port mode only. */
#define DATAPORT	0x10	/* data port window */
#define RSTPORT		0x1f	/* reset port window */

/* These registers are accessible in shared memory mode only. */
#define CTLR1		0x00	/* control register 1 */
#define ATDET		0x01	/* AT detect register (for read only) */
#define CTLR2		0x05	/* control register 2 */
#define NADDR		0x08	/* node address */

	/* page 0 read/write registers */
/* Must issue a read before a write to these registers. */
#define MCRA  		0x0a	/* mode configuration register A */
#define MCRB  		0x0b	/* mode configuration register B */

/********** Register Command/Status/Mask Bits *************************/

	/*----- MCRA command bits -----*/
#define MCRA_IOAD	0x07	/* I/O address */
#define MCRA_INT	0x38	/* interrupt line used */
#define MCRA_FREAD	0x40	/* fast read enable */
#define MCRA_MEMIO	0x80	/* memory of I/O mode selection */

	/*----- MCRB command bits -----*/
#define MCRB_PHYS	0x03	/* physical layer interface: */
#define MCRB_PHYS0	0x00	/*  TPI (10BASE-T compatible squelch level) */
#define MCRB_PHYS1	0x01	/*  Thin Ethernet (10BASE2) */
#define MCRB_PHYS2	0x02	/*  Thick Ethernet (10BASE5, AUI port) */
#define MCRB_PHYS3	0x03	/*  TPI (reduced squelch level) */
#define MCRB_GDLINK	0x04	/* good link disable */
#define MCRB_IO16CON	0x08	/* /IO16 control enable */
#define MCRB_CHRDY	0x10	/* CHRDY from IORD/ and IOWR/ or from BALE */
#define MCRB_BE		0x20	/* bus error condition */
#define MCRB_BPWR	0x40	/* boot PROM write seleciton */
#define MCRB_EELOAD	0x80	/* EEPRM load enable */

	/*----- CTLR1 command bits -----*/
#define CTLR1_RESET	0x80	/* reset */
#define CTLR1_MEME	0x40	/* memory enable (enable shared memory) */
#define CTLR1_DECSA	0x3f	/* memory address decode bits, */
							/*  corresponding to SA 13-18 bits */

	/*----- ATDET mask bits -----*/
#define ATDET_ATDET	0x01	/* AT detect */
#
	/*----- CTLR2 command bits -----*/
#define CTLR2_MEM16	0x80	/* 8/16-bit: 0=8bit, 1=16bit */
#define CTLR2_MEMW	0x40	/* momory width: 0=byte, 1=word access */
#define CTLR2_DECLA	0x1f	/* memory address decode bits, */
							/*  corresponding to SA 19-23 bits */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

/*
 * Additional register descriptions for Realtek RTL80x9(AS).
 */

/************** Register Offsets **************************************/

	/* page 0 read registers */
#define _80x9ID0    0x0a	/* RTL80x9 identification register 0 */
#define _80x9ID1    0x0b	/* RTL80x9 identification register 0 */

	/* page 3 read or read/write registers */
#define _9346CR     0x01	/* 9346 command register (for read and write) */
#define BPAGE       0x02	/* BROM page register (for read and write) */
#define CONFIG0     0x03	/* configuration register 0 (for read) */
#define CONFIG1     0x04	/* configuration register 1 (for read and write) */
#define CONFIG2     0x05	/* configuration register 2 (for read and write) */
#define CONFIG3     0x06	/* configuration register 3 (for read and write) */
#define CSNSAV      0x08	/* CSN save register (for read) */
#define HLTCLK      0x09	/* halt clock register (for write) */
#define INTR        0x0a	/* interrupt register, reflects INT7-0 pins (for read) */

	/* page 3 read or write registers (added with RTL80x9AS) */
#define FMWP        0x0c	/* flash memory write protect register (for write) */
#define CONFIG4     0x0e	/* configuration register 4 (for read) */

/********** Register Command/Status/Mask Bits *************************/

	/*----- _9346CR command bits -----*/
#define _9346CR_EEDO    0x01	/* reflects EEDO pin */
#define _9346CR_EEDI    0x02	/* reflects EEDI pin */
#define _9346CR_EESK    0x04	/* reflects EESK pin */
#define _9346CR_EECS    0x08	/* reflects EECS pin */
#define _9346CR_EMM     0xc0	/* select RTL8019 operating mode  (EMM1-0 mask) */
#define _9346CR_NORM    0x00	/* EMM1-0: normal (DP8390 compatible) */
#define _9346CR_AUTO    0x40	/* EMM1-0: auto-load */
#define _9346CR_PROG    0x80	/* EMM1-0: 9346 programming */
#define _9346CR_CRWE    0xc0	/* EMM1-0: config register write enable */

	/*----- CONFIG0 command bits -----*/
#define CONFIG0_BNC     0x04	/* indicates using 10base2 thin cable */
#define CONFIG0_JP      0x08	/* reflects JP pin (indicates RTL80x9 is in jumper mode) */
#define CONFIG0_PNPJP   0x10	/* set when PNP jumper pin is pulled high externally */
#define CONFIG0_AUI     0x20	/* set when external MAU is used on AUI interface */
#define CONFIG0_VERID   0xc0	/* vender ID */

	/*----- CONFIG1 command bits -----*/
#define CONFIG1_IOS     0x0f	/* select I/O base address */
#define CONFIG1_IRQS    0x70	/* IRQ select */
#define CONFIG1_IRQEN   0x80	/* IRQ enable */

	/*----- CONFIG2 command bits -----*/
#define CONFIG2_BS      0x1f	/* select BROM size & memory base address */
#define CONFIG2_BSELB   0x20	/* forces BROM disabled */
#define CONFIG2_PL      0xc0	/* select network meida type (PL1-0 mask) */
#define CONFIG2_PL0     0x00	/* PL1-0: TP/CX auto-detect (10base-T with link test enable) */
#define CONFIG2_PL1     0x40	/* PL1-0: 10base-T with link test disable */
#define CONFIG2_PL2     0x80	/* PL1-0: 10base5 */
#define CONFIG2_PL3     0xc0	/* PL1-0: 10base2 */

	/*----- CONFIG3 command bits -----*/
#define CONFIG3_ACTIVEB 0x01	/* inverse of bit 0 in PnP Activate register */
#define CONFIG3_PWRDN   0x02	/* puts RTL80x9 in power down mode */
#define CONFIG3_SLEEP   0x04	/* puts RTL80x9 in sleep mode */
#define CONFIG3_RESRVD  0x08	/* reserved (must not write a 1 to this bit) */
#define CONFIG3_LEDS    0x30	/* select outputs to LED2-0 pins */
#define CONFIG3_FUDP    0x40	/* full-duplex mode */
#define CONFIG3_PNP     0x80	/* indicates operating in Plan and Play mode */

	/*----- CONFIG4 command bits -----*/
#define CONFIG4_IOMS    0x01	/* set when using SA15-SA0 to decode I/O address */

	/*----- HLTCLK command values -----*/
#define HLTCLK_MODE1    0x52	/* mode 1 - clock running */
#define HLTCLK_MODE2    0x48	/* mode 2 - clock halted */

/* end */
