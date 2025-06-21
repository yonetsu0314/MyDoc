/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/sh7615.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: sh7615.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* E-DMAC割り込みのベクタ番号と優先順位。任意に設定すべし。 */
/* ベクタ番号は72～127の範囲 */
/* Interrupt vector number and priority for E-DMAC. */
/* Available vector numbers are from 72 to 127. */
/*#define EDMAC_VECTNO    68		/* ベクタ番号 */
/*#define EDMAC_PRIOR     4		/* 優先順位 */
#define EDMAC_VECTNO    73		/* ベクタ番号 */
#define EDMAC_PRIOR     13		/* 優先順位 */

/* このボードのPHYTER(DP83843)に対するPHY Address */
#define PHYAD 0x01		/* PHY Address for this board */

/* Address mask to convert between cached and non-cached address */
#define NCADDR_MASK     0x20000000	/* Cached -> non-cached. OR mask */
/*#define NCADDR_MASK     0x40000000	/* Cached -> non-cached. OR mask */
#define CADDR_MASK      0x1fffffff	/* Non-cached -> cached. AND mask */

/* SH7615 register difinitions */

/**** Register address difinitions *************************************/
/* Bit mask of Status Register (SR) */
#define IMASK	0x000000F0

/* Registers in Interrupt Control Unit */
#define IPRA	0xFFFFFEE2		/* Interrupt Priority A (16bit) */
#define IPRB	0xFFFFFE60		/* Interrupt Priority B (16bit) */
#define IPRC	0xFFFFFEE6		/* Interrupt Priority C (16bit) */
#define IPRD	0xFFFFFE40		/* Interrupt Priority D (16bit) */
#define IPRE	0xFFFFFEC0		/* Interrupt Priority E (16bit) */
#define VCRA	0xFFFFFE62		/* Interrupt Vector A (16bit) */
#define VCRC	0xFFFFFE66		/* Interrupt Vector C (16bit) */
#define VCRD	0xFFFFFE68		/* Interrupt Vector D (16bit) */
#define VCRE	0xFFFFFE42		/* Interrupt Vector E (16bit) */
#define VCRF	0xFFFFFE44		/* Interrupt Vector F (16bit) */
#define VCRG	0xFFFFFE46		/* Interrupt Vector G (16bit) */
#define VCRH	0xFFFFFE48		/* Interrupt Vector H (16bit) */
#define VCRI	0xFFFFFE4A		/* Interrupt Vector I (16bit) */
#define VCRJ	0xFFFFFE4C		/* Interrupt Vector J (16bit) */
#define VCRK	0xFFFFFE4E		/* Interrupt Vector K (16bit) */
#define VCRL	0xFFFFFE50		/* Interrupt Vector L (16bit) */
#define VCRM	0xFFFFFE52		/* Interrupt Vector M (16bit) */
#define VCRN	0xFFFFFE54		/* Interrupt Vector N (16bit) */
#define VCRO	0xFFFFFE56		/* Interrupt Vector O (16bit) */
#define VCRP	0xFFFFFEC2		/* Interrupt Vector P (16bit) */
#define VCRQ	0xFFFFFEC4		/* Interrupt Vector Q (16bit) */
#define VCRR	0xFFFFFEC6		/* Interrupt Vector R (16bit) */
#define VCRS	0xFFFFFEC8		/* Interrupt Vector S (16bit) */
#define VCRT	0xFFFFFECA		/* Interrupt Vector T (16bit) */
#define VCRU	0xFFFFFECC		/* Interrupt Vector U (16bit) */
#define VCRWDT	0xFFFFFEE4		/* Interrupt Vector WDT (16bit) */
#define VCRDMA0	0xFFFFFFA0		/* Interrupt Vector DMA0 (32bit) */
#define VCRDMA1	0xFFFFFFA8		/* Interrupt Vector DMA1 (32bit) */
#define ICR	0xFFFFFEE0		/* Interrupt Control (16bit) */
#define IRQCSR	0xFFFFFEE8		/* IRQ Control/Status (16bit) */

/* Registers in 16bit Free-running timer unit */
#define TIER	0xFFFFFE10		/* Timer interrupt enable (8bit) */
#define FTCSR	0xFFFFFE11		/* Free-running timer control/status (8bit) */
#define FRC	0xFFFFFE12		/* Free-running count (16bit) */
#define OCR	0xFFFFFE14		/* Output compare A/B (16bit) */
#define TCR	0xFFFFFE16		/* Timer control (8bit) */
#define TOCR	0xFFFFFE17		/* Timer output compare control (8bit) */
#define FICR	0xFFFFFE18		/* Input capture (16bit) */

/* Registers in serial communication interface with FIFO 1 (SCIF) */
#define SCSMR1	0xFFFFFCC0		/* Serial mode (8bit) */
#define SCBRR1	0xFFFFFCC2		/* Bit rate (8bit) */
#define SCSCR1	0xFFFFFCC4		/* Serial Control (8bit) */
#define SCFTDR1	0xFFFFFCC6		/* Transmit FIFO data (8bit) */
#define SC1SSR1	0xFFFFFCC8		/* Serial status 1 (16bit) */
#define SC2SSR1	0xFFFFFCCA		/* Serial status 2 (8bit) */
#define SCFRDR1	0xFFFFFCCC		/* Receive FIFO data (8bit) */
#define SCFCR1	0xFFFFFCCE		/* FIFO control (8bit) */
#define SCFDR1	0xFFFFFCD0		/* FIFO data count (16bit) */
#define SCFER1	0xFFFFFCD2		/* FIFO error (16bit) */
#define SCIMR1	0xFFFFFCD4		/* IrDA mode (8bit) */

/* Registers in serial communication interface with FIFO 2 (SCIF) */
#define SCSMR2	0xFFFFFCE0		/* Serial mode (8bit) */
#define SCBRR2	0xFFFFFCE2		/* Bit rate (8bit) */
#define SCSCR2	0xFFFFFCE4		/* Serial Control (8bit) */
#define SCFTDR2	0xFFFFFCE6		/* Transmit FIFO data (8bit) */
#define SC1SSR2	0xFFFFFCE8		/* Serial status 1 (16bit) */
#define SC2SSR2	0xFFFFFCEA		/* Serial status 2 (8bit) */
#define SCFRDR2	0xFFFFFCEC		/* Receive FIFO data (8bit) */
#define SCFCR2	0xFFFFFCEE		/* FIFO control (8bit) */
#define SCFDR2	0xFFFFFCF0		/* FIFO data count (16bit) */
#define SCFER2	0xFFFFFCF2		/* FIFO error (16bit) */
#define SCIMR2	0xFFFFFCF4		/* IrDA mode (8bit) */

/* Bit difinition for SCIF */
#define SCIF_ER		0x0080		/* Receive Error */
#define SCIF_TEND	0x0040		/* Transmit End */
#define SCIF_TDFE	0x0020		/* Transmit FIFO Data Empty */
#define SCIF_BRK	0x0010		/* Break */
#define SCIF_FER	0x0008		/* Framing Error */
#define SCIF_PER	0x0004		/* Parity Error */
#define SCIF_RDF	0x0002		/* Receive Data Full */
#define SCIF_DR		0x0001		/* Receive Data Ready */

/* Registers in 16bit timer pulse unit (TPU) */
#define TCR0	0xFFFFFC50		/* Timer control 0 (8bit) */
#define TMDR0	0xFFFFFC51		/* Timer mode 0 (8bit) */
#define TIOR0H	0xFFFFFC52		/* Timer I/O control 0H (8bit) */
#define TIOR0L	0xFFFFFC53		/* Timer I/O control 0L (8bit) */
#define TIER0	0xFFFFFC54		/* Timer interrupt enable 0 (8bit) */
#define TSR0	0xFFFFFC55		/* Timer status 0 (8bit) */
#define TCNT0	0xFFFFFC56		/* Timer count 0 (16bit) */
#define TGR0A	0xFFFFFC58		/* Timer general 0A (16bit) */
#define TGR0B	0xFFFFFC5A		/* Timer general 0B (16bit) */
#define TGR0C	0xFFFFFC5C		/* Timer general 0C (16bit) */
#define TGR0D	0xFFFFFC5E		/* Timer general 0D (16bit) */

#define TCR1	0xFFFFFC60		/* Timer control 1 (8bit) */
#define TMDR1	0xFFFFFC61		/* Timer mode 1 (8bit) */
#define TIOR1	0xFFFFFC62		/* Timer I/O control 1 (8bit) */
#define TIER1	0xFFFFFC64		/* Timer interrupt enable 1 (8bit) */
#define TSR1	0xFFFFFC65		/* Timer status 1 (8bit) */
#define TCNT1	0xFFFFFC66		/* Timer count 1 (16bit) */
#define TGR1A	0xFFFFFC68		/* Timer general 1A (16bit) */
#define TGR1B	0xFFFFFC6A		/* Timer general 1B (16bit) */

#define TCR2	0xFFFFFC70		/* Timer control 2 (8bit) */
#define TMDR2	0xFFFFFC71		/* Timer mode 2 (8bit) */
#define TIOR2	0xFFFFFC72		/* Timer I/O control 2 (8bit) */
#define TIER2	0xFFFFFC74		/* Timer interrupt enable 2 (8bit) */
#define TSR2	0xFFFFFC75		/* Timer status 2 (8bit) */
#define TCNT2	0xFFFFFC76		/* Timer count 2 (16bit) */
#define TGR2A	0xFFFFFC78		/* Timer general 2A (16bit) */
#define TGR2B	0xFFFFFC7A		/* Timer general 2B (16bit) */

#define TSTR	0xFFFFFC40		/* Timer start (8bit) */
#define TSYR	0xFFFFFC41		/* Timer synchronize (8bit) */

/* Register address for EtherC */
#define ECMR	0xFFFFFD60		/* EtherC Mode (32bit) */
#define ECSR	0xFFFFFD64		/* EtherC Status (32bit) */
#define ECSIPR	0xFFFFFD68		/* EtherC Status Interrupt Permission (32bit) */
#define PIR	0xFFFFFD6C		/* PHY Interface (32bit) */
#define MAHR	0xFFFFFD70		/* MAC Address High (32bit) */
#define MALR	0xFFFFFD74		/* MAC Address Low (32bit) */
#define RFLR	0xFFFFFD78		/* Receive Frame Length (32bit) */
#define PSR	0xFFFFFD7C		/* PHY Interface Status (32bit) */
#define TROCR	0xFFFFFD80		/* Tx Retry Over Counter (32bit) */
#define CDCR	0xFFFFFD84		/* Collision Detect Counter (32bit) */
#define LCCR	0xFFFFFD88		/* Lost Carrier Counter (32bit) */
#define CNDCR	0xFFFFFD8C		/* Carrier Not Detect Counter (32bit) */
#define IFLCR	0xFFFFFD90		/* Illegal Frame Length Counter (32bit) */
#define CEFCR	0xFFFFFD94		/* CRC Frame Counter (32bit) */
#define FRECR	0xFFFFFD98		/* Frame Receive Error Counter (32bit) */
#define TSFRCR	0xFFFFFD9C		/* Too-Short Frame Receive Counter (32bit) */
#define TLFRCR	0xFFFFFDA0		/* Too-Long Frame Receive Counter (32bit) */
#define RFCR	0xFFFFFDA4		/* Residual-bit Frame Counter (32bit) */
#define MAFCR	0xFFFFFDA8		/* Multicast Address Frame Counter (32bit) */

/* Register address for E-DMAC */
#define EDMR	0xFFFFFD00		/* E-DMAC Mode (32bit) */
#define EDTRR	0xFFFFFD04		/* E-DMAC Transmit Request (32bit) */
#define EDRRR	0xFFFFFD08		/* E-DMAC Receive Request (32bit) */
#define TDLAR	0xFFFFFD0C		/* Tx Descriptor List Address (32bit) */
#define RDLAR	0xFFFFFD10		/* Rx Descriptor List Address (32bit) */
#define EESR	0xFFFFFD14		/* EtherC/E-DMAC Status Register (32bit) */
#define EESIPR	0xFFFFFD18		/* EtherC/E-DMAC Status Interrupt Permission (32bit) */
#define TRSCER	0xFFFFFD1C		/* Tx/Rx Status Copy Enable (32bit) */
#define RMFCR	0xFFFFFD20		/* Receive Missed-Frame Counter (32bit) */
#define TFTR	0xFFFFFD24		/* Tx FIFO Threshold (32bit) */
#define FDR	0xFFFFFD28		/* FIFO Depth (32bit) */
#define RCR	0xFFFFFD2C		/* Receive Control (32bit) */
#define EDOCR	0xFFFFFD30		/* E-DMAC Operation Control (32bit) */

/* Transmit Descriptor */
struct TxDesc_t {
    unsigned long status;		/* Status */
    unsigned short datalen;		/* data length */
    unsigned short padding1;
    void *buffer;			/* Address of buffer */
    unsigned long padding2;
};

/* Bit difinition for Transmit Descriptor */
#define EDESC_TACT	0x80000000	/* 1 - Descriptor valid */
#define EDESC_TDL	0x40000000	/* 0 - continue to next descriptor */
#define EDESC_TFP	0x30000000	/* Frame Position. */
					/*   00 - middle */
					/*   01 - last */
					/*   10 - top */
					/*   11 - single */
#define EDESC_TFE	0x08000000	/* 1 - Tx frame error */
#define EDESC_ITF	0x00000010	/* 1 - Illegal Tx frame length */
#define EDESC_CND	0x00000008	/* 1 - Carrier not detect */
#define EDESC_DLC	0x00000004	/* 1 - Detect loss of carrier */
#define EDESC_CD	0x00000002	/* 1 - Collision Detect */
#define EDESC_TRO	0x00000001	/* 1 - Tx Retry Over */

/* Receive Descriptor */
struct RxDesc_t {
    unsigned long status;		/* Status */
    unsigned short buflen;		/* buffer length */
    unsigned short datalen;		/* data length */
    void *buffer;			/* Address of buffer */
    unsigned long padding2;
};

/* Bit difinition for Receive Descriptor */
#define EDESC_RACT	0x80000000	/* 1 - Descriptor valid */
#define EDESC_RDL	0x40000000	/* 0 - continue to next descriptor */
#define EDESC_RFP	0x30000000	/* Frame Position */
#define EDESC_RFE	0x08000000	/* 1 - Rx frame error */
#define EDESC_RFOF	0x00000200	/* 1 - Rx FIFO Overflow */
#define EDESC_RMAF	0x00000080	/* 1 - Receive multicast address frame */
#define EDESC_RRF	0x00000010	/* 1 - Receive residual-bit frame */
#define EDESC_RTLF	0x00000008	/* 1 - Receive too-long frame */
#define EDESC_RTSF	0x00000004	/* 1 - Receive too-short frame */
#define EDESC_PRE	0x00000002	/* 1 - PHY-LSI receive error */
#define EDESC_CERF	0x00000001	/* 1 - CRC error on received frame */

/* Bit difinition for ECSR,ICSIPR */
#define ECSR_LCNHG	0x00000004	/* LINK signal Changed */
#define ECSR_MPR	0x00000002	/* Magic Packet Receive */
#define ECSR_ICD	0x00000001	/* Illegal Carrier Detection */

/* Bit difinition for EESR,EESIPR,TRSCER */
#define EESR_RFCOF	0x01000000	/* 1 - receive frame counter overflow */
#define EESR_ECI	0x00400000	/* 1 - EtherC status register interrupt */
#define EESR_TC		0x00200000	/* 1 - Tx Complete */
#define EESR_TDE	0x00100000	/* 1 - Tx Descriptor Exhausted */
#define EESR_TFUF	0x00080000	/* 1 - Tx FIFO Underflow */
#define EESR_FR		0x00040000	/* 1 - Frame received */
#define EESR_RDE	0x00020000	/* 1 - Rx Descriptor Exhausted */
#define EESR_RFOF	0x00010000	/* 1 - Rx FIFO Overflow */
#define EESR_ITF	0x00001000	/* 1 - Illegal Tx Frame */
#define EESR_CND	0x00000800	/* 1 - Carrier not detected */
#define EESR_DLC	0x00000400	/* 1 - Detect loss of carrier */
#define EESR_CD		0x00000200	/* 1 - collision detect */
#define EESR_TRO	0x00000100	/* 1 - Tx retry over */
#define EESR_RMAF	0x00000080	/* 1 - Receive Multicast address frame */
#define EESR_RRF	0x00000010	/* 1 - Receive Residual-bit frame */
#define EESR_RTLF	0x00000008	/* 1 - Receive too-long frame */
#define EESR_RTSF	0x00000004	/* 1 - Receive too-short frame */
#define EESR_PRE	0x00000002	/* 1 - PHY-LSI receive error */
#define EESR_CERF	0x00000001	/* 1 - CRC error on received frame */

/* Bit difinition for RCR */
#define RCR_RNC		0x00000001	/* 1 - receive normal mode */

/* Bit difinition for EDTRR */
#define EDTRR_TR	0x00000001	/* 1 - receive request */
#define EDRRR_RR	0x00000001	/* 1 - transmit request */

/* Bit difinition for ECMR */
#define ECMR_PRCEF	0x00001000	/* 1 - permit CRC error */
#define ECMR_MPDE	0x00000200	/* 1 - Enable Magic packet */
#define ECMR_RE		0x00000040	/* 1 - Receive Enable */
#define ECMR_TE		0x00000020	/* 1 - Transmit Enable */
#define ECMR_ILB	0x00000008	/* 1 - Internal Loop Back mode */
#define ECMR_ELB	0x00000004	/* 1 - External Loop Back mode */
#define ECMR_DM		0x00000002	/* 1 - Duplex Mode */
#define ECMR_PRM	0x00000001	/* 1 - Promiscuous mode */

/* Bit difinition for PIR */
#define PIR_MDI		0x00000008	/* MII Management Data-in bit */
#define PIR_MDO		0x00000004	/* MII Management Data-out bit */
#define PIR_MMD		0x00000002	/* MII Management Mode bit */
#define PIR_MMC		0x00000001	/* MII Management Clock bit */

/* Bit difinition for PSR */
#define PSR_LMON	0x00000001	/* Link monitor bit */

/* MII Management data frame */
struct MIIframe_t {
    unsigned long pre;			/* Synchronization, all '1' */
    unsigned short st:2;		/* Start, '01' */
    unsigned short op:2;		/* Opcode, '01' - read / '10' - write */
    unsigned short phyad:5;		/* PHY Address */
    unsigned short regad:5;		/* Register Address */
    unsigned short ta:2;		/* TA 'Z0' - read / '10' - write */
    unsigned short data;		/* data to register */
};

union MIIframe {
    struct MIIframe_t st;
    unsigned short b[4];
};

#define PHYREG_BMCR	0x00		/* Basic Mode Control Register */
#define PHYREG_BMSR	0x01		/* Basic Mode Status Register */
#define PHYREG_PHYIDR1	0x02		/* PHY Identifier Register #1 */
#define PHYREG_PHYIDR2	0x03		/* PHY Identifier Register #2 */
#define PHYREG_ANAR	0x04		/* Auto-Negotiation Advertisement Register */
#define PHYREG_ANLPAR	0x05		/* Auto-Negotiation Link Partner Ability Register */
#define PHYREG_ANER	0x06		/* Auto-Negotiation Expansion Register */
#define PHYREG_ANNPTR	0x07		/* Auto-Negotiation Next Page TX */
#define PHYREG_PHYSTS	0x10		/* PHY Status Register */
#define PHYREG_MIPSCR	0x11		/* MII Interrupt PHY Specific Control Register */
#define PHYREG_MIPGSR	0x12		/* MII Interrupt PHY Generic Status Register */
#define PHYREG_DCR	0x13		/* Disconnect Counter */
#define PHYREG_FCSCR	0x14		/* False Carrier Sense Control Register */
#define PHYREG_RECR	0x15		/* Receive Error Counter Register */
#define PHYREG_PCSR	0x16		/* PCS Sub-Layer Configuration and Status Register */
#define PHYREG_LBR	0x17		/* Loopback and Bypass Register */
#define PHYREG_10BTSCR	0x18		/* 10BASE-T Status & Control Register */
#define PHYREG_PHYCTRL	0x19		/* PHY Control Register */

#define PHYSTS_MIIINT	0x0100		/* PHY Status : MII Interrupt Pending */
#define PHYSTS_DUPLEX	0x0004		/* PHY Status : Duplex mode, 1 - Full Duplex */
#define PHYSTS_SPEED10	0x0002		/* PHY Status : Link speed, 1 - 10Mbps */
#define PHYSTS_LNKSTS	0x0001		/* PHY Status : Link valid */

#define MIPSCR_INTEN	0x0002		/* MII Interrupt PHY : Interrupt Enable */

#define EESR_ERRBITS	(EESR_RFCOF | EESR_TDE  | EESR_TFUF | EESR_RDE  | EESR_RFOF \
			 | EESR_ITF | EESR_CND  | EESR_DLC  | EESR_CD   | EESR_TRO  \
			 | EESR_RRF | EESR_RTLF | EESR_RTSF | EESR_CERF | EESR_PRE)
