/******************************************************************************
* Header file for PCMCIA Controller MR-SHPC-01 V2                             *
*                                                                             *
*  File name : mrshpc01v2.h                                                   *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 04/Oct/2001     ; file created                                    [SP]      *
* 07/Apr/2002     ; corrected for new file system                   [SP]      *
* 20/Oct/2002     ; Board Specific header file (ms7751.h) added     [SP]      *
******************************************************************************/
#ifndef MRSHPC_H
#define MRSHPC_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef MS7751_H
#include "ms7751.h"
#endif


#ifndef MRSHPC_REG      /* Define Base address for PCMCIA controller */
#define MRSHPC_REG      0xB83FFF00
#define MRSHPC_MEM_WIN  0xB8400000      /* Memory Window Area */
#define MRSHPC_ATR_WIN  0xB8500000      /* Attribute Window Area */
#define MRSHPC_IO_WIN   0xB8600000      /* I/O Window Area */
#endif

/**** The Controller Data I/O access functions declared here ****************/
#define port_out(n,c)   (*((volatile UB *)(n) ) = c )
#define port_in(n)      (*(volatile UB *)(n) )
#define port_set(n,c)   (*(volatile UB *)(n) |= c )
#define port_clr(n,c)   (*(volatile UB *)(n) &= ~c )
#define port_outw(n,c)  (*((volatile UH *)(n) ) = c )
#define port_inw(n)     (*(volatile UH *)(n) )
#define port_setw(n,c)  (*(volatile UH *)(n) |= c )
#define port_clrw(n,c)  (*(volatile UH *)(n) &= ~c )

/*** PCMCIA Controller Register Offset addresses defined here ***************/
#define SHPC_MODE   (MRSHPC_REG + 0xE4) /* Mode Control Register */
#define SHPC_OPTION (MRSHPC_REG + 0xE6) /* Option Control Register */
#define SHPC_STATUS (MRSHPC_REG + 0xE8) /* Card Status Register */
#define SHPC_ISR    (MRSHPC_REG + 0xEA) /* Interrupt Status Register */
#define SHPC_ICR    (MRSHPC_REG + 0xEC) /* Interrupt Control Register */
#define SHPC_CPWCR  (MRSHPC_REG + 0xEE) /* CARD Power Control register */
#define SHPC_MW0CR1 (MRSHPC_REG + 0xF0) /* Mem Window0 Control register1 */
#define SHPC_MW1CR1 (MRSHPC_REG + 0xF2) /* Mem Window1 Control register1 */
#define SHPC_IOWCR1 (MRSHPC_REG + 0xF4) /* I/O Window Control register1 */
#define SHPC_MW0CR2 (MRSHPC_REG + 0xF6) /* Mem Window0 Control register2 */
#define SHPC_MW1CR2 (MRSHPC_REG + 0xF8) /* Mem Window1 Control register2 */
#define SHPC_IOWCR2 (MRSHPC_REG + 0xFA) /* I/O Window Control register2 */
#define SHPC_CDCR   (MRSHPC_REG + 0xFC) /* Card Control Register */
#define SHPC_ID     (MRSHPC_REG + 0xFE) /* Chip Information Register */

/*** Bit Definations for Mode Control Register *****************/
#define MR_MODE_SELCLK      0x0010      /* Select CPU clock or Ext clock */
#define MR_MODE_SELMODE     0x0020      /* Select SH4 mode */

/*** Bit Definations for Option Control Register *****************/
#define MR_OPTION_SELLED    0x0001      /* Select LED terminal */
#define MR_OPTION_SELSPK    0x0002      /* Select Speaker Terminal */
#define MR_OPTION_LEDOUT    0x0004      /* Set LED terminal data */
#define MR_OPTION_SPKOUT    0x0008      /* Set SPEAKER terminal data */

/*** Bit Definations for Card Status Register *****************/
#define MR_STATUS_BVD1      0x0001      /* Battery Status Bit-1 */
#define MR_STATUS_BVD2      0x0002      /* Battery Status Bit-2 */
#define MR_STATUS_CARD_IN   0x000C
#define MR_STATUS_WPS       0x0010
#define MR_STATUS_RDY       0x0020
#define MR_STATUS_PWON      0x0040
#define MR_STATUS_VS1       0x0080
#define MR_STATUS_VS2       0x0100
#define MR_STATUS_PCIC_RDY  0x0200
#define MR_STATUS_RA2N      0x3C00
#define MR_STATUS_ENDIAN    0x4000      /* Byte Order Select */

/*** Bit Definations for Interrupt Status Register *****************/
#define MR_ISR_BAT_DEAD     0x0001      /* Battery Dead status */
#define MR_ISR_BAT_WARN     0x0002      /* Low Battery warning */
#define MR_ISR_RDY_CHG      0x0004
#define MR_ISR_CD_DETECT    0x0008      /* Card Detect status */
#define MR_ISR_PWR_GOOD     0x0010      /* Card Power Healthy status */
#define MR_ISR_IREQ_CHG     0x0020
#define MR_ISR_STSCHG_RI    0x0040

/*** Bit Definations for Interrupt Control Register *****************/
#define MR_ICR_EN_BATDE     0x0001      /* Battery Dead INT Enable */
#define MR_ICR_EN_BATWARN   0x0002      /* Battery Warn INT Enable */
#define MR_ICR_EN_RDY       0x0004      /* Enable RDY/BSY INT */
#define MR_ICR_EN_DETECT    0x0008      /* Enable Card Detect INT */
#define MR_ICR_EN_CPGOOD    0x0010      /* Enable Card Power Change INT */
#define MR_ICR_MNG_IRQ      0x00E0      /* Interrupt Steering */
#define MR_ICR_RING_IRQ     0x0700      /* RING INT steering */
#define MR_ICR_CARD_IRQ     0x3800      /* CARD IRQ steering */
#define MR_ICR_PSYS_IRQ     0x4000      /* Select Level/Edge type INT */

/*** Bit Definations for Card Power Control Register *****************/
#define MR_CPWCR_VPP0       0x0001
#define MR_CPWCR_VPP1       0x0002
#define MR_CPWCR_VCC3V      0x0004      /* Set 3V power supply for Card */
#define MR_CPWCR_VCC5V      0x0008      /* Set 5V power supply for Card */
#define MR_CPWCR_VCCPWR     0x0010      /* Set Card Power ON */
#define MR_CPWCR_AUTOPWR    0x0020      /* Automatic Power control for Card */
#define MR_CPWCR_ENCARD     0x0040      /* Enable Card IN/OUT signals */
#define MR_CPWCR_SUSPEND    0x0080      /* Set Suspend mode */
#define MR_CPWCR_PWRDN      0x0100      /* Set Power Down Mode */
#define MR_CPWCR_RSTCARD    0x0200      /* Reset PC Card */
#define MR_CPWCR_CDPWR_MSK  0x0400      /* Mask Card Good Power status */

/*** Bit Definations for MEM & IO window Control Register1 *****************/
#define MR_START_ADDR       0x003F      /* Window Start Address */
#define MR_SETUP            0x00C0      /* Set SETUP time */
#define MR_HOLD             0x0300      /* Set up Hold time /
#define MR_WIDTH            0x7C00      /* Set Command Pulse Width time */
#define MR_WIN_ENABLE       0x8000      /* Enable the window */

/*** Bit Definations for MEM & IO window Control Register2 *****************/
#define MR_CARD_ADDR        0x00FF      /* Set CARD address */
#define MR_CREG             0x0100      /* Set CREG terminal output signal */
#define MR_SIZE             0x0200      /* Set Data Bus Size for Card */
#define MR_WR_PROTECT       0x0400      /* Disable Write access to Window */
#define MR_SWAP             0x0800      /* Enable Data Swap in 16-bit mode */

/*** Bit Definations for Card Control Register *****************/
#define MR_CDCR_EN_INPACK   0x0001
#define MR_CDCR_EN_SPK      0x0002      /* Enable Speaker OUT */
#define MR_CDCR_EN_LED      0x0004      /* Enable LED OUT */
#define MR_CDCR_IO_CARD     0x0008      /* Set I/O card type */


#define SHPC_DEVICE_ID  0x5333          /* Device ID */
#define MR_WIN_TIMING   0x0A80

ER      PCCard_init(void);
ER      PCCard_check(void);
void    PCCard_pwr_ON(void);
ER      PCCard_end(void);
void    PCCard_open(void);
UH      PCCard_mem_readw( UW addr );
UB      PCCard_mem_readb( UW addr );
void    PCCard_mem_writew( UW addr, UH data);
void    PCCard_mem_writeb( UW addr, UB data);
UH      PCCard_atr_readw( UW addr );
UB      PCCard_atr_readb( UW addr );
void    PCCard_atr_writew( UW addr, UH data);
void    PCCard_atr_writeb( UW addr, UB data);
UH      PCCard_io_inpw( UW addr );
UB      PCCard_io_inpb( UW addr );
void    PCCard_io_outw( UW addr, UH data);
void    PCCard_io_outb( UW addr, UB data);

#ifdef __cplusplus
}
#endif
#endif /**** MRSHPC_H *******/
