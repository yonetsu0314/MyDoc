/******************************************************************************
* Header file for SERIAL COM driver program for MC1543.                       *
*                                                                             *
*  File name : nos1543.h                                                       *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 18/Sept/2001     ; file created  by SP                                      *
******************************************************************************/
#ifndef SERIAL_H
#define SERIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PCI_H
#include "pci7751.h"
#endif

#ifndef MC1543_H
#include "mc1543.h"
#endif

/**** UART1 register addresses ***********************************************/
#define UART1_RBR   (UART1_BASE + 0x0)  /* Receive Buffer Register */
#define UART1_THR   (UART1_BASE + 0x0)  /* Transmit Holding Register */
#define UART1_DLL   (UART1_BASE + 0x0)  /* Divisor Latch LSB Register */
#define UART1_DLM   (UART1_BASE + 0x1)  /* Divisor Latch MSB Register */
#define UART1_IER   (UART1_BASE + 0x1)  /* Interrupt Enable Register */
#define UART1_IIR   (UART1_BASE + 0x2)  /* Interrupt Identification Register */
#define UART1_FCR   (UART1_BASE + 0x2)  /* FIFO Control Register */
#define UART1_LCR   (UART1_BASE + 0x3)  /* Line Control Register */
#define UART1_MCR   (UART1_BASE + 0x4)  /* Moden Control Register */
#define UART1_LSR   (UART1_BASE + 0x5)  /* Line Status Register */
#define UART1_MSR   (UART1_BASE + 0x6)  /* Moden Status Register */
#define UART1_SCR   (UART1_BASE + 0x7)  /* Scratch Register */

/**** UART3 register addresses ***********************************************/
#define UART3_RBR   (UART3_BASE + 0x0)  /* Receive Buffer Register */
#define UART3_THR   (UART3_BASE + 0x0)  /* Transmit Holding Register */
#define UART3_DLL   (UART3_BASE + 0x0)  /* Divisor Latch LSB Register */
#define UART3_DLM   (UART3_BASE + 0x1)  /* Divisor Latch MSB Register */
#define UART3_IER   (UART3_BASE + 0x1)  /* Interrupt Enable Register */
#define UART3_IIR   (UART3_BASE + 0x2)  /* Interrupt Identification Register */
#define UART3_FCR   (UART3_BASE + 0x2)  /* FIFO Control Register */
#define UART3_LCR   (UART3_BASE + 0x3)  /* Line Control Register */
#define UART3_MCR   (UART3_BASE + 0x4)  /* Moden Control Register */
#define UART3_LSR   (UART3_BASE + 0x5)  /* Line Status Register */
#define UART3_MSR   (UART3_BASE + 0x6)  /* Moden Status Register */
#define UART3_SCR   (UART3_BASE + 0x7)  /* Scratch Register */

/**** UARTx_IER register bit definitions *************************************/
#define UART_IER_EMSI   0x08    /* Enable Modem Status Interrupt */
#define UART_IER_ELSI   0x04    /* Enable Rx Line Status Interrupt */
#define UART_IER_ETHREI 0x02    /* Enable Tx Hold Reg Empty Interrupt */
#define UART_IER_ERDAI  0x01    /* Enable Rx Data Available Interrupt */

/**** UARTx_IIR register bit definitions *************************************/
#define UART_IIR_ID     0x06    /* Mask for Interrupt ID */
#define UART_IIR_NOINT  0x01    /* No Interrupt Pending */

#define UART_IIR_RLSI   0x06    /* Rx Line Status Interrupt */
#define UART_IIR_RDI    0x04    /* Rx data Available Interrupt */
#define UART_IIR_THRI   0x02    /* Tx Holding Reg empty Interrupt */
#define UART_IIR_MSI    0x00    /* Modem Status Interrupt */

/**** UARTx_FCR register bit definitions *************************************/
#define UART_FCR_TRIGGER_14     0xC0    /* Mask for trigger set at 14 */
#define UART_FCR_TRIGGER_8      0x80    /* Mask for trigger set at 8 */
#define UART_FCR_TRIGGER_4      0x40    /* Mask for trigger set at 4 */
#define UART_FCR_TRIGGER_1      0x00    /* Mask for trigger set at 1 */
#define UART_FCR_TRIGGER_MASK   0xC0    /* Mask for the FIFO trigger range */
#define UART_FCR_CLEAR_XMIT     0x04    /* Clear the XMIT FIFO */
#define UART_FCR_CLEAR_RCVR     0x02    /* Clear the RCVR FIFO */
#define UART_FCR_ENABLE_FIFO    0x01    /* Enable the FIFO */

/**** UARTx_LCR register bit definitions *************************************/
#define UART_LCR_DLAB   0x80    /* Divisor latch access bit */
#define UART_LCR_SBC    0x40    /* Set break control */
#define UART_LCR_SPAR   0x20    /* Stick parity (?) */
#define UART_LCR_EPS    0x10    /* Even parity select */
#define UART_LCR_PEN    0x08    /* Parity Enable */
#define UART_LCR_STB    0x04    /* Stop bits: 0=1 stop bit, 1= 2 stop bits */
#define UART_LCR_WLEN5  0x00    /* Wordlength: 5 bits */
#define UART_LCR_WLEN6  0x01    /* Wordlength: 6 bits */
#define UART_LCR_WLEN7  0x02    /* Wordlength: 7 bits */
#define UART_LCR_WLEN8  0x03    /* Wordlength: 8 bits */

/**** UARTx_MCR register bit definitions *************************************/
#define UART_MCR_LOOP   0x10    /* Enable loopback test mode */
#define UART_MCR_ENINT  0x08    /* SIO interrupt enable */
#define UART_MCR_OUT1   0x04    /* Out1 complement */
#define UART_MCR_RTS    0x02    /* RTS if set, RTS off */
#define UART_MCR_DTR    0x01    /* DTR if set, DTR off */

/**** UARTx_LSR register bit definitions *************************************/
#define UART_LSR_FIFO   0x80    /* Rx FIFO error                */
#define UART_LSR_TEMT   0x40    /* Transmitter empty            */
#define UART_LSR_THRE   0x20    /* Transmit-hold-register empty */
#define UART_LSR_BI     0x10    /* Break interrupt indicator    */
#define UART_LSR_FE     0x08    /* Frame error indicator        */
#define UART_LSR_PE     0x04    /* Parity error indicator       */
#define UART_LSR_OE     0x02    /* Overrun error indicator      */
#define UART_LSR_DR     0x01    /* Receiver data ready          */

/**** UARTx_MSR register bit definitions *************************************/
#define UART_MSR_DCD        0x80    /* Data Carrier Detect */
#define UART_MSR_RI         0x40    /* Ring Indicator */
#define UART_MSR_DSR        0x20    /* Data Set Ready */
#define UART_MSR_CTS        0x10    /* Clear to Send */
#define UART_MSR_DDCD       0x08    /* Delta DCD */
#define UART_MSR_TERI       0x04    /* Trailing edge ring indicator */
#define UART_MSR_DDSR       0x02    /* Delta DSR */
#define UART_MSR_DCTS       0x01    /* Delta CTS */
#define UART_MSR_ANY_DELTA  0x0F    /* Any of the delta bits! */

/**** UARTx_DLL Baud Rate settings  ******************************************/
#define UART_DLL_4800       24      /* Baud rates    4,800bps   */
#define UART_DLL_9600       12      /* Baud rates    9,600bps   */
#define UART_DLL_19200      6       /* Baud rates   19,200bps   */
#define UART_DLL_38400      3       /* Baud rates   38,400bps   */
#define UART_DLL_57600      2       /* Baud rates   57,600bps   */
#define UART_DLL_115200     1       /* Baud rates  115,200bps   */

/**** UARTx_DLM Baud Rate settings  ******************************************/
#define UART_DLM_4800       0       /* Baud rates    4,800bps   */
#define UART_DLM_9600       0       /* Baud rates    9,600bps   */
#define UART_DLM_19200      0       /* Baud rates   19,200bps   */
#define UART_DLM_38400      0       /* Baud rates   38,400bps   */
#define UART_DLM_57600      0       /* Baud rates   57,600bps   */
#define UART_DLM_115200     0       /* Baud rates  115,200bps   */

#ifdef __cplusplus
}
#endif
#endif  /* SERIAL_H */
