/******************************************************************************
* Header file for IO device address definitionsÅiSH7751Åj                     *
*                                                                             *
*  File name : nos7751.h                                                      *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 98-09-06 çÏê¨                                                               *
* 16/Apr/2000                                                                 *
* 10/Aug/2001  modified for SH7751SE01 eva board                              *
******************************************************************************/

#ifndef NOS7751_H
#define NOS7751_H

#include "sh7751.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Serial mode register */

#define SMR_CKS0    0x01    /* Clock select0 */
#define SMR_CKS1    0x02    /* Clock select1 */
#ifdef CH0
#define SMR_MP      0x04    /* Multiprocessor mode set */
#endif
#define SMR_STOP    0x08    /* Stop bit setting */
#define SMR_OE      0x10    /* Parity setting */
#define SMR_PE      0x20    /* Parity enable control */
#define SMR_CHR     0x40    /* data width set - 7bit / 8bit */
#ifdef CH0
#define SMR_CA      0x80    /* communication mode set */
#endif

/* Serial control register */
#ifdef CH0
#define SCR_CKE0    0x01    /* Clock enable-0 */
#endif
#define SCR_CKE1    0x02    /* Clock enable-1 */
#ifdef CH0
#define SCR_TEIE    0x04    /* Transmit end interrupt enable */
#define SCR_MPIE    0x08    /* multiprocessor interrupt enable */
#else
#define SCR_REIE    0x08    /* Receive error interrupt enable */
#endif
#define SCR_RE      0x10    /* Receive enable */
#define SCR_TE      0x20    /* Transmit enable */
#define SCR_RIE     0x40    /* Receive data interrupt enable */
#define SCR_TIE     0x80    /* Transmit interrupt enable */

/* Serial ststus register(SCI) */
#ifdef CH0
#define SSR_MPBT    0x01    /* multiprocessor transmission status bit */
#define SSR_MPB     0x02    /* Multiprocessor status bit */
#define SSR_TEND    0x04    /* Transmit end status */
#define SSR_PER     0x08    /* Parity error status */
#define SSR_FER     0x10    /* Framing error status */
#define SSR_ORER    0x20    /* Overrun error status */
#define SSR_RDRF    0x40    /* Receive data register full status */
#define SSR_TDRE    0x80    /* Transmit data reg empty status */

/* Serial status register (SCIF) */

#else
#define SSR_DR      0x01    /* Receive data ready status */
#define SSR_RDF     0x02    /* Receive FIFO full status */
#define SSR_PER     0x04    /* Parity error status */
#define SSR_FER     0x08    /* Framing error status */
#define SSR_BRK     0x10    /* Break detect status */
#define SSR_TDFE    0x20    /* Transmit FIFO data empty status */
#define SSR_TEND    0x40    /* Transmit end status */
#define SSR_ER      0x80    /* Receive error status */
#endif



/* Serial port register(SCI) */

#ifdef CH0
#define SPB0DT      0x01    /* Serial port break data */
#define SPB0IO      0x02    /* Serail port break I/O */
#define SPB1DT      0x04    /* Serial port clock port data */
#define SPB1IO      0x08    /* Serial port clock port I/O */
#define SPBEIO      0x80    /* Error interrupt only */

/* Serial port register(SCIF) */

#else
#define SPB2DT      0x01    /* Serail port break data */
#define SPB2IO      0x02    /* Serial port break I/O */
#define CTSDT       0x10    /* CTS port data */
#define CTSIO       0x20    /* CTS port I/O */
#define RTSDT       0x40    /* RTS port data */
#define RTSIO       0x80    /* RTS port I/O */

#define MCR_EN      0x08    /* Modem Control Enable */
#endif

#if 0
/* Port SC control register */

#define SCP0MD0     0x0001
#define SCP0MD1     0x0002
#define SCP1MD0     0x0004
#define SCP1MD1     0x0008
#define SCP2MD0     0x0010
#define SCP2MD1     0x0020
#define SCP3MD0     0x0040
#define SCP3MD1     0x0080
#define SCP4MD0     0x0100
#define SCP4MD1     0x0200
#define SCP5MD0     0x0400
#define SCP5MD1     0x0800
#define SCP6MD0     0x1000
#define SCP6MD1     0x2000
#define SCP7MD0     0x4000
#define SCP7MD1     0x8000

/* Port SC data register */

#define SCP0DT      0x01
#define SCP1DT      0x02
#define SCP2DT      0x04
#define SCP3DT      0x08
#define SCP4DT      0x10
#define SCP5DT      0x20
#define SCP6DT      0x40
#define SCP7DT      0x80
#endif

#ifdef __cplusplus
}
#endif
#endif /* NOS7751_H */
