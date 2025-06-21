/******************************************************************************
* MS7751SE01 board specific hardware details                                  *
*                                                                             *
*  File name : ms7751.h                                                       *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 20/Oct/2002 created                                                         *
******************************************************************************/

#ifndef MS7751_H
#define MS7751_H

#ifndef SH7751_H
#include "sh7751.h"
#endif

#define PCI_IO_AREA     0xFE240000      /* IO area defination in PCI memory */
#define PCI_MEM_AREA    0xFD000000      /* MEM area defination in PCI memory */

#define PCI_MEM0_BASE   0xb0000000      /* Base address for PCI MEM window0 */
#define PCI_MEM1_BASE   0xd0000000      /* Base address for PCI MEM window1 */
#define SH4_MEM0_BASE   0xac000000      /* Base address for local MEM window0 */
#define SH4_MEM1_BASE   0xad000000      /* Base address for local MEM window1 */

#define PCI_MEM_BASE    0xFD000000      /* PCI memory area */
#define PCI_IO_BASE     0xFE240000      /* PCI IO space address */
#define LANC_IO_BASE    0x00010000      /* 0x00010000 to 0x0001001f for LANC */
#define LANC_MEM_BASE   0x00100000      /* 0x00100000 to 0x0010001f for LANC */

#define PMUC_IO_BASE    0x00010100      /* Power Management controller IO base addr */
#define PMUC_MEM_BASE   0x00100100      /* Power Management controller MEM base addr */

#define USBC_IO_BASE    0x00011000      /* USB controller IO base addr */
#define USBC_MEM_BASE   0x00101000      /* USB controller MEM base addr */

#define SIOC_IO_BASE    0x0             /* Super I/O controller IO base addr */
#define SIOC_MEM_BASE   0x0             /* Super I/O controller MEM base addr */

#define MRSHPC_BASE     0xB8000000      /* MR-SHPC-01 V2 Controller Base Addr */
#define MRSHPC_REG      (MRSHPC_BASE + 0x3FFF00)
                                /* MR-SHPC-01 V2 Controller Register Base Addr */
#define MRSHPC_MEM_WIN  0xB8400000      /* SHPC Memory Window Area */
#define MRSHPC_ATR_WIN  0xB8800000      /* SHPC Attribute Window Area */
#define MRSHPC_IO_WIN   0xB8C00000      /* SHPC I/O Window Area */

#define NCMEM_BASE      0xAC700000      /* Non-Cache memory area base address */
#define NCMEM_SZ        0x00100000      /* 1MB RAM for non-Cache application */

#define SW_BASE         0xB9000000      /* switch base register */
#define SW_REG1         0xB9000000      /* SW register 1    */
#define SW_REG2         0xB9000002      /* SW register 2    */
#define LED_PORT        0xBA000000      /* LED port address */

/* LED bit definitions */
#define LED_CARD_RD     0x0101
#define LED_CARD_WR     0x0202
#define LED_CARD_DET    0x0404
#define LED_CARD_MOUNT  0x0808
#define LED_CARD_ERROR  0x1010
#define LED_SIO_INIT    0x2020
#define LED_LAN_INIT    0x4040
#define LED_BOARD_ERROR 0x8080
#define LED_ALL         0xFFFF

/* Interrupt definitions for MS7751SE01 board */
#define IP_SIO          12              /* Super IO interrupt line */
#define INT_SIO         INT_IRL12       /* Super I/O controller IRQ */

/*-----------------------------------------------------------------
    INT_IRL15       -          Expansion slot Interrupt
    INT_IRL14       -          Expansion slot interrupt
    INT_IRL13       -          PCMCIA controller IRQ-2
    INT_IRL12       -          Super I/O controller IRQ
    INT_IRL11       -          Expansion slot interrupt
    INT_IRL10       -          Expansion slot interrupt
    INT_IRL9        -          PCMCIA controller IRQ-1
    INT_IRL8        -          not defined
    INT_IRL7        -          Expansion slot interrupt
    INT_IRL6        -          Expansion slot interrupt
    INT_IRL5        -          not defined
    INT_IRL4        -          Expansion slot interrupt
    INT_IRL3        -          not defined
    INT_IRL2        -          Expansion slot interrupt
    INT_IRL1        -          PCMCIA controller IRQ-0
--------------------------------------------------------------------*/

/* LED set/clear functions defined here */

void led_out(UH);
UH led_set(UH);
UH led_clr(UH);

#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))
#define led_set(n) (UH)(*(volatile UH *)LED_PORT |= (UH)(n))
#define led_clr(n) (UH)( *(volatile UH *)LED_PORT &= (UH)(~(n)))


#endif

