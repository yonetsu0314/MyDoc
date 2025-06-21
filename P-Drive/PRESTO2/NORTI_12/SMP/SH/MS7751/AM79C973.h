/******************************************************************************
* Header file for LAN driver program.                                         *
*                                                                             *
*  File name : AM79C973.h                                                     *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 01/July/2001     ; file created  by [SP]                                    *
* 26/Mar/2002      ; PHY control reg added [SP]                               *
******************************************************************************/
#ifndef AM79C973_H
#define AM79C973_H

#ifdef __cplusplus
extern "C" {
#endif

/* define 79C973 port registers for 32-bit access mode */
#define LAN_RDP         0x0010      /* Register data port */
#define LAN_RAP         0x0014      /* Register address port */
#define LAN_RST         0x0018      /* Reset register */
#define LAN_BDP         0x001C      /* BCR data port */

/* BUS control registers for 79C973 ******************************************/
#define LAN_BCR2        0x0002      /* miscellanous configuration */
#define LAN_BCR4        0x0004      /* LED0 status */
#define LAN_BCR5        0x0005      /* LED1 status */
#define LAN_BCR6        0x0006      /* LED2 status */
#define LAN_BCR7        0x0007      /* LED3 status */
#define LAN_BCR9        0x0009      /* Full duplex control */
#define LAN_BCR16       0x0010      /* IO base address */
#define LAN_BCR17       0x0011      /* IO base address */
#define LAN_BCR18       0x0012      /* burst and bus control */
#define LAN_BCR19       0x0013      /* EEPROM control and status */
#define LAN_BCR20       0x0014      /* software style */
#define LAN_BCR22       0x0016      /* PCI latency */
#define LAN_BCR23       0x0017      /* PCI subsystem ID */
#define LAN_BCR24       0x0018      /* PCI subsystem vendor ID */
#define LAN_BCR25       0x0019      /* SRAM size */
#define LAN_BCR26       0x001A      /* SRAM boundary */
#define LAN_BCR27       0x001B      /* SRAM interface control */
#define LAN_BCR28       0x001C      /* expansion bus address lower */
#define LAN_BCR29       0x001D      /* expansion bus address upper */
#define LAN_BCR30       0x001E      /* expansion bus data port */
#define LAN_BCR31       0x001F      /* software timer value */
#define LAN_BCR32       0x0020      /* PHY control and status */
#define LAN_BCR33       0x0021      /* PHY address */
#define LAN_BCR34       0x0022      /* PHY management data register */
#define LAN_BCR35       0x0023      /* PCI vendor ID */
#define LAN_BCR36       0x0024      /* PCI power management */
#define LAN_BCR37       0x0025      /* PCI DATA0 */
#define LAN_BCR38       0x0026      /* PCI DATA1 */
#define LAN_BCR39       0x0027      /* PCI DATA2 */
#define LAN_BCR40       0x0028      /* PCI DATA3 */
#define LAN_BCR41       0x0029      /* PCI DATA4 */
#define LAN_BCR42       0x002A      /* PCI DATA5 */
#define LAN_BCR43       0x002B      /* PCI DATA6 */
#define LAN_BCR44       0x002C      /* PCI DATA7 */


/* Control and Status registers for 79C973 ************************************/
#define LAN_CSR0        0x0000      /* 79C973 status & control register */
#define LAN_CSR1        0x0001      /* Initialization block start address */
#define LAN_CSR2        0x0002      /* Init address higher byte */
#define LAN_CSR3        0x0003      /* Interrupt mask and deferral control */
#define LAN_CSR4        0x0004      /* test and feature control */
#define LAN_CSR5        0x0005      /* Extended control and Interrupt */
#define LAN_CSR6        0x0006      /* RX/TX encoded ring length */
#define LAN_CSR7        0x0007      /* extended control and Interrupts */
#define LAN_CSR8        0x0008      /* Logical address filter low word */
#define LAN_CSR9        0x0009      /* Logical address filter low word */
#define LAN_CSR10       0x000A      /* Logical address filter high word */
#define LAN_CSR11       0x000B      /* Logical address filter low word */
#define LAN_CSR12       0x000C      /* Physical address register low word */
#define LAN_CSR13       0x000D      /* Physical address register mid word */
#define LAN_CSR14       0x000E      /* Physical address register high byte */
#define LAN_CSR15       0x000F      /* Mode register */
#define LAN_CSR16       0x0010      /* Init block start address */
#define LAN_CSR18       0x0012      /* Current receive buffer address */
#define LAN_CSR19       0x0013      /* Current receive buffer address */
#define LAN_CSR20       0x0014      /* Current transmit buffer address */
#define LAN_CSR21       0x0015      /* Current transmit buffer address */
#define LAN_CSR22       0x0016      /* Next receive buffer address */
#define LAN_CSR23       0x0017      /* Next receive buffer address */
#define LAN_CSR24       0x0018      /* Base address of receive ring */
#define LAN_CSR25       0x0019      /* Base address of receive ring */
#define LAN_CSR26       0x001A      /* Next receive descriptor address */
#define LAN_CSR27       0x001B      /* Next receive descriptor address */
#define LAN_CSR28       0x001C      /* current receive descriptor address */
#define LAN_CSR29       0x001D      /* current receive descriptor address */
#define LAN_CSR30       0x001E      /* base address of transmit ring */
#define LAN_CSR31       0x001F      /* base address of transmit ring */
#define LAN_CSR32       0x0020      /* next transmit descriptor address */
#define LAN_CSR33       0x0021      /* next transmit descriptor address */
#define LAN_CSR34       0x0022      /* current transmit descriptor address */
#define LAN_CSR35       0x0023      /* current transmit descriptor address */
#define LAN_CSR36       0x0024      /* 2nd next receive descriptor address */
#define LAN_CSR37       0x0025      /* 2nd next receive descriptor address */
#define LAN_CSR38       0x0026      /* 2nd next transmit descriptor address */
#define LAN_CSR40       0x0028      /* current receive byte count */
#define LAN_CSR41       0x0029      /* current receive status */
#define LAN_CSR42       0x002A      /* current transmit byte /
#define LAN_CSR43       0x002B      /* current transmit status */
#define LAN_CSR44       0x002C      /* next receive byte count */
#define LAN_CSR45       0x002D      /* next receive status */
#define LAN_CSR46       0x002E      /* transmit poll time counter */
#define LAN_CSR47       0x002F      /* transmit polling interval set */
#define LAN_CSR48       0x0030      /* receive poll time counter */
#define LAN_CSR49       0x0031      /* receive polling interval set */
#define LAN_CSR58       0x003A      /* software style set */
#define LAN_CSR60       0x003C      /* previous transmit descriptor address */
#define LAN_CSR62       0x003E      /* previous transmit byte count */
#define LAN_CSR63       0x003F      /* previous transmit status */
#define LAN_CSR64       0x0040      /* next transmit buffer start address */
#define LAN_CSR65       0x0041      /* next transmit buffer start address */
#define LAN_CSR66       0x0042      /* next transmit byte count */
#define LAN_CSR67       0x0043      /* next transmit status */
#define LAN_CSR72       0x0048      /* receive ring counter */
#define LAN_CSR74       0x004A      /* transmit ring counter */
#define LAN_CSR76       0x004C      /* receive ring length */
#define LAN_CSR78       0x004E      /* transmit ring length */
#define LAN_CSR80       0x0050      /* DMA transfer counter and FIFO threshold */
#define LAN_CSR82       0x0052      /* Transmit descriptor pointer lower addr */
#define LAN_CSR84       0x0054      /* DMA address register */
#define LAN_CSR85       0x0055      /* DMA address register */
#define LAN_CSR86       0x0056      /* DMA buffer byte counter */
#define LAN_CSR92       0x005C      /* ring length conversion */
#define LAN_CSR100      0x0064      /* Bus timeout register */
#define LAN_CSR112      0x0070      /* missed frame counter */
#define LAN_CSR114      0x0072      /* received collision count */
#define LAN_CSR116      0x0074      /* power mode register */
#define LAN_CSR122      0x007A      /* receive frame alignment control */
#define LAN_CSR124      0x007C      /* test register */

/* mode controls and other definations ****************************************/
#define SOFTWARE_STYLE  0x0003      /* 32-bit data access */
#define EN_RCV_ALL      0x8000      /* bit15- enable receive all packets */
#define DIS_RCV_BROAD   0x4000      /* bit14- disable broadcast reception */
#define DIS_RCV_PHY     0x2000      /* bit13- disable physical pkt reception */
#define MODE_PORTSEL    0x0180      /* port selection fixed value */
#define INTL_LOOPBACK   0x0040      /* bit6- internal loopback */
#define DIS_RETRY       0x0020      /* bit5- disable transmission retry */
#define FORCE_COLL      0x0010      /* bit4- force collision for testing */
#define DIS_CRC         0x0008      /* bit3- disable CRC transmit */
#define EN_LOOPBACK     0x0004      /* bit2- loopback enable */
#define DIS_TRX         0x0002      /* bit1- disable transmission */
#define DIS_RCV         0x0001      /* bit0- disable reception */

/* CSR0 control and status bit structure **************************************/
#define CSR0_ERR        0x8000      /* common error flag */
#define CSR0_CERR       0x2000      /* collision error flag */
#define CSR0_MISS       0x1000      /* missed frame flag */
#define CSR0_RINT       0x0400      /* receive interrupt flag */
#define CSR0_TINT       0x0200      /* transmit interrupt flag */
#define CSR0_IDON       0x0100      /* Initialization done flag */
#define CSR0_INTR       0x0080      /* common interrupt flag */
#define CSR0_IENA       0x0040      /* interrupt enable flag */
#define CSR0_RXON       0x0020      /* receive ON status */
#define CSR0_TXON       0x0010      /* transmit ON status */
#define CSR0_STOP       0x0004      /* stop DMA activity flag */
#define CSR0_START      0x0002      /* start DMA activity flag */
#define CSR0_INIT       0x0001      /* start INIT process flag */

/* interrupt mask control bits ************************************************/
#define CSR3_MISSM      0x1000      /* missed frame interrupt mask */
#define CSR3_MERRM      0x0800      /* memory error interrupt mask */
#define CSR3_RINTM      0x0400      /* receive interrupt mask */
#define CSR3_TINTM      0x0200      /* transmit interrupt mask */
#define CSR3_IDONM      0x0100      /* Init Over interrupt mask */
#define CSR3_DXSUFLO    0x0040      /* disable trx stop on underflow error */
#define CSR3_LAPPEN     0x0020      /* enable look ahead pkt processing */
#define CSR3_DXMT2PD    0x0010      /* disable transmit two part differal */
#define CSR3_EMBA       0x0008      /* enable modified Back-off algorithm */
#define CSR3_BSWP       0x0004      /* byte swap enable */

/* definition for CSR4 control and status bits ********************************/
#define CSR4_TXDPOLL    0x1000      /* Disable transmit polling */
#define CSR4_APAD_XMT   0x0800      /* auto-pad transmit enable flag */
#define CSR4_ASTRP_RCV  0x0400      /* auto-strap receive enable */
#define CSR4_MFCOM      0x0100      /* missed frame overflow counter mask */
#define CSR4_RCVCCOM    0x0010      /* receive collision counter o/f mask */
#define CSR4_TXSTRT     0x0008      /* Transmit start status */
#define CSR4_TXSTRTM    0x0004      /* Transmit start mask */

/* definition for CSR4 control and status bits ********************************/
#define CSR5_TOKINTD    0x8000      /* Transmit OK int disable */
#define CSR5_LTINTEN    0x4000      /* Last Transmit int enable */
#define CSR5_SINT       0x0800      /* System error int status */
#define CSR5_SINTE      0x0400      /* System error int enable */
#define CSR5_SPND       0x0001      /* Suspend mode control */

/* definition for CSR80 control and status bits *******************************/
#define CSR80_RCVFW     0x1000      /* receive watermark = 64 bytes */
#define CSR80_XMTSP     0x0c00      /* transmit start when whole pkt ready */
#define CSR80_XMTFW     0x0000      /* transmit watermark = 16 bytes */

/* definition for CSR7 Int control and status bits ****************************/
#define CSR7_FSPND      0x8000      /* Fast Suspend Mode */
#define CSR7_RDMD       0x2000      /* Manual RDTE polling command */
#define CSR7_RXDPOLL    0x1000      /* Disable RDTE polling */
#define CSR7_MAPINT     0x0080      /* PHY management Interrupt status */
#define CSR7_MAPINTE    0x0040      /* PHY auto-poll interrupt enable */

/* Physical register addresses ************************************************/
#define LAN_ANR0        0x03c0      /* PHY control register */
#define LAN_ANR1        0x03c1      /* PHY status register */
#define LAN_ANR2        0x03c2      /* PHY identifier */
#define LAN_ANR3        0x03c3      /* PHY identifier */
#define LAN_ANR4        0x03c4      /* Auto negotiation advertisement reg */
#define LAN_ANR5        0x03c5      /* auto neg. link partner ability */
#define LAN_ANR6        0x03c6      /* auto neg expansion */
#define LAN_ANR7        0x03c7      /* auto neg next page */
#define LAN_ANR16       0x03d0      /* interrupt enable and status reg*/
#define LAN_ANR17       0x03d1      /* Extended PHY control and status */
#define LAN_ANR18       0x03d2      /* Descrambler and resynch timer */
#define LAN_ANR19       0x03d3      /* Extended PHY management reg */
#define LAN_ANR24       0x03d8      /* overall status register */

/* Bit definition for Receiver descriptor ***********************************/
#define RMD_OWN         0x80000000  /* OWN bit */
#define RMD_ERR         0x40000000  /* common error flag */
#define RMD_FRAM        0x20000000  /* Framing error flag */
#define RMD_OFLO        0x10000000  /* Overflow error flag */
#define RMD_CRC         0x08000000  /* CRC error status */
#define RMD_BUFF        0x04000000  /* Buffer error flag */
#define RMD_STP         0x02000000  /* Start of packet status */
#define RMD_ENP         0x01000000  /* end of packet status */

/* Bit definition for Transmitter descriptor ***********************************/
#define TMD_OWN         0x80000000  /* OWN bit */
#define TMD_ERR         0x40000000  /* common error flag */
#define TMD_ADDFCS      0x20000000  /* ADD FCS control flag */
#define TMD_LTINT       0x10000000  /* MORE status flag */
#define TMD_STP         0x02000000  /* Start of packet flag */
#define TMD_ENP         0x01000000  /* End of packet flag */
#define TMD_BUFF        0x80000000  /* Buffer error status */
#define TMD_UFLO        0x40000000  /* Underflow error */
#define TMD_EXDEF       0x20000000  /* Excessive deferral flag */
#define TMD_LCOL        0x10000000  /* Late collision flag */
#define TMD_LCAR        0x08000000  /* Loss of carrier error status */
#define TMD_RTRY        0x04000000  /* Retry error status */

/* Receive descriptor header for SWSTYLE = 3 */
#define RMD0    0x0         /* RX descriptor byte0 - message length */
#define RMD1    0x4         /* RX descriptor byte1 - status and buffer length */
#define RMD2    0x8         /* RX descriptor byte2 - RX buffer address */
#define RMD3    0xC         /* RX descriptor byte3 - user space */

/* Transmit descriptor header for SWSTYLE = 3 */
#define TMD0    0x0         /* TX descriptor byte0 - message length */
#define TMD1    0x4         /* TX descriptor byte1 - status and buffer length */
#define TMD2    0x8         /* TX descriptor byte2 - TX buffer address */
#define TMD3    0xC         /* TX descriptor byte3 - user space */

/* Bit definitions for PHY Control register ***********************************/
#define BCR32_APEP      0x0800      /* PHY status auto-poll selection */
#define BCR32_DANAS     0x0080      /* Disable Auto Setup for auto-negotiation */
#define BCR32_XPHYRST   0x0040      /* PHY reset control */
#define BCR32_XPHYANE   0x0020      /* PHY auto-negotiation enable control bit */
#define BCR32_XPHYFD    0x0010      /* PHY Full Duplex selection */
#define BCR32_XPHYSP    0x0008      /* PHY 100 Mbps speed selction */

#define ANR0_SRST       0x8000      /* Software Reset status bit */
#define ANR0_LPBACK     0x4000      /* Internal Loopback enable bit */
#define ANR0_PHYSP      0x2000      /* 100 Mbps speed selection */
#define ANR0_PHYANE     0x1000      /* Auto-Negotiation Enable control bit */
#define ANR0_ISO        0x0400      /* Electrically Isolate PHY */
#define ANR0_RSTANG     0x0200      /* Restart Auto_Negotiation */
#define ANR0_PHYFD      0x0100      /* Full Duplex mode selection */

#define ANR1_ANGEND     0x0020      /* Auto-Negotiation End status bit */
#define ANR1_ANGABLE    0x0008      /* Auto_Negotiation Ablility status flag */

#define ANR24_LNKSTS    0x0008      /* ANR24 link status bit-3 */
#define ANR24_SPEED     0x0001      /* Speed status bit */
#define ANR24_DUPLEX    0x0004      /* Duplex status bit */

    /* LAN error definations **************************************************/
#define E_LINK          (-110)       /* LINK fail error */
#define E_DRIVER_FAIL   (-112)       /* LAN driver fail to recover */
#define E_TXLEN_OVF     (-113)       /* LAN transmit data length over limit */
#define E_RXBUSY        (-114)       /* Rx Busy status */
#define E_TXBUSY        (-115)       /* Tx Busy status */
#define E_RXERR         (-116)       /* Error during Data reception */
#define E_TXERR         (-117)       /* Error during Data transmission */
#define LAN_ERROR_MAX   (0x20)

#ifdef __cplusplus
}
#endif
#endif /* AM79C973_H */
