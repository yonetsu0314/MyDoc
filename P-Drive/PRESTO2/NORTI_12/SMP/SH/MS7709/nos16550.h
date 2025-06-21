/******************************************************************************
* �V���A�����o�̓f�o�C�X��`�w�b�_                                            *
*                                                                             *
*  File name : nos16550.h                                                     *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 99-08-12 �쐬  TL16C552(MS7709RP01�A���[�h�A�N�Z�X�A��ʂW�r�b�g�j          *
* 99-09-30 MS7750SE01 �Ή��iSMSC Super I/O FDC37C935A �j                      *
*          Super I/O ��Configuration��CPU���������ɂł��s���Ă��������B       *
*          �{���W���[�����ł́A����16550�Ƃ��Ĉ����܂��B                      *
* 16/Apr/2000                                                                 *
* 13/May/2000  Supported MS7615SE01 board                                     *
* 21/Aug/2000  CI, CD �r�b�g�ʒu�����                                        *
* 25/Dec/2002  MS7729R��`�̒ǉ�                                           KA *
******************************************************************************/

#ifndef NOS16550_H
#define NOS16550_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(MS7709)
 #define BTYPE     0
#elif defined(MS7750)
 #define BTYPE     1
#elif defined(MS7750S)
 #define BTYPE     2
#elif defined(MS7709A)
 #define BTYPE     3
#elif defined(MS7727)
 #define BTYPE     4
#elif defined(MS7729R)
 #define BTYPE     5
#elif defined(MS7615)
 #define BTYPE     6
#else
 #define BTYPE     0
#endif

#if BTYPE == 0
 #ifndef SH7709_H
  #include "sh7709.h"
 #endif
 #define UART0_RBR   0xB0800000
 #define UART0_THR   0xB0800000
 #define UART0_DLL   0xB0800000
 #define UART0_IER   0xB0800002
 #define UART0_DLM   0xB0800002
 #define UART0_IIR   0xB0800004
 #define UART0_FCR   0xB0800004
 #define UART0_LCR   0xB0800006
 #define UART0_MCR   0xB0800008
 #define UART0_LSR   0xB080000A
 #define UART0_MSR   0xB080000C
 #define UART0_SCR   0xB080000E
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB0840000
 #define UART1_THR   0xB0840000
 #define UART1_DLL   0xB0840000
 #define UART1_IER   0xB0840002
 #define UART1_DLM   0xB0840002
 #define UART1_IIR   0xB0840004
 #define UART1_FCR   0xB0840004
 #define UART1_LCR   0xB0840006
 #define UART1_MCR   0xB0840008
 #define UART1_LSR   0xB084000A
 #define UART1_MSR   0xB084000C
 #define UART1_SCR   0xB084000E
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 1
 #ifndef SH7750_H
  #include "sh7750.h"
 #endif
 #define UART0_RBR   0xB04007F0
 #define UART0_THR   0xB04007F0
 #define UART0_DLL   0xB04007F0
 #define UART0_IER   0xB04007F2
 #define UART0_DLM   0xB04007F2
 #define UART0_IIR   0xB04007F4
 #define UART0_FCR   0xB04007F4
 #define UART0_LCR   0xB04007F6
 #define UART0_MCR   0xB04007F8
 #define UART0_LSR   0xB04007FA
 #define UART0_MSR   0xB04007FC
 #define UART0_SCR   0xB04007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB04005F0
 #define UART1_THR   0xB04005F0
 #define UART1_DLL   0xB04005F0
 #define UART1_IER   0xB04005F2
 #define UART1_DLM   0xB04005F2
 #define UART1_IIR   0xB04005F4
 #define UART1_FCR   0xB04005F4
 #define UART1_LCR   0xB04005F6
 #define UART1_MCR   0xB04005F8
 #define UART1_LSR   0xB04005FA
 #define UART1_MSR   0xB04005FC
 #define UART1_SCR   0xB04005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 2
 #ifndef SH7750S_H
  #include "sh7750S.h"
 #endif
 #define UART0_RBR   0xB04007F0
 #define UART0_THR   0xB04007F0
 #define UART0_DLL   0xB04007F0
 #define UART0_IER   0xB04007F2
 #define UART0_DLM   0xB04007F2
 #define UART0_IIR   0xB04007F4
 #define UART0_FCR   0xB04007F4
 #define UART0_LCR   0xB04007F6
 #define UART0_MCR   0xB04007F8
 #define UART0_LSR   0xB04007FA
 #define UART0_MSR   0xB04007FC
 #define UART0_SCR   0xB04007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB04005F0
 #define UART1_THR   0xB04005F0
 #define UART1_DLL   0xB04005F0
 #define UART1_IER   0xB04005F2
 #define UART1_DLM   0xB04005F2
 #define UART1_IIR   0xB04005F4
 #define UART1_FCR   0xB04005F4
 #define UART1_LCR   0xB04005F6
 #define UART1_MCR   0xB04005F8
 #define UART1_LSR   0xB04005FA
 #define UART1_MSR   0xB04005FC
 #define UART1_SCR   0xB04005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 3
 #ifndef SH7709A_H
  #include "sh7709A.h"
 #endif
 #define UART0_RBR   0xB04007F0
 #define UART0_THR   0xB04007F0
 #define UART0_DLL   0xB04007F0
 #define UART0_IER   0xB04007F2
 #define UART0_DLM   0xB04007F2
 #define UART0_IIR   0xB04007F4
 #define UART0_FCR   0xB04007F4
 #define UART0_LCR   0xB04007F6
 #define UART0_MCR   0xB04007F8
 #define UART0_LSR   0xB04007FA
 #define UART0_MSR   0xB04007FC
 #define UART0_SCR   0xB04007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB04005F0
 #define UART1_THR   0xB04005F0
 #define UART1_DLL   0xB04005F0
 #define UART1_IER   0xB04005F2
 #define UART1_DLM   0xB04005F2
 #define UART1_IIR   0xB04005F4
 #define UART1_FCR   0xB04005F4
 #define UART1_LCR   0xB04005F6
 #define UART1_MCR   0xB04005F8
 #define UART1_LSR   0xB04005FA
 #define UART1_MSR   0xB04005FC
 #define UART1_SCR   0xB04005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 4
 #ifndef SH7727_H
  #include "sh7727.h"
 #endif
 #define UART0_RBR   0xB04007F0
 #define UART0_THR   0xB04007F0
 #define UART0_DLL   0xB04007F0
 #define UART0_IER   0xB04007F2
 #define UART0_DLM   0xB04007F2
 #define UART0_IIR   0xB04007F4
 #define UART0_FCR   0xB04007F4
 #define UART0_LCR   0xB04007F6
 #define UART0_MCR   0xB04007F8
 #define UART0_LSR   0xB04007FA
 #define UART0_MSR   0xB04007FC
 #define UART0_SCR   0xB04007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB04005F0
 #define UART1_THR   0xB04005F0
 #define UART1_DLL   0xB04005F0
 #define UART1_IER   0xB04005F2
 #define UART1_DLM   0xB04005F2
 #define UART1_IIR   0xB04005F4
 #define UART1_FCR   0xB04005F4
 #define UART1_LCR   0xB04005F6
 #define UART1_MCR   0xB04005F8
 #define UART1_LSR   0xB04005FA
 #define UART1_MSR   0xB04005FC
 #define UART1_SCR   0xB04005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 5
 #ifndef SH7729R_H
  #include "sh7729R.h"
 #endif
 #define UART0_RBR   0xB04007F0
 #define UART0_THR   0xB04007F0
 #define UART0_DLL   0xB04007F0
 #define UART0_IER   0xB04007F2
 #define UART0_DLM   0xB04007F2
 #define UART0_IIR   0xB04007F4
 #define UART0_FCR   0xB04007F4
 #define UART0_LCR   0xB04007F6
 #define UART0_MCR   0xB04007F8
 #define UART0_LSR   0xB04007FA
 #define UART0_MSR   0xB04007FC
 #define UART0_SCR   0xB04007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0xB04005F0
 #define UART1_THR   0xB04005F0
 #define UART1_DLL   0xB04005F0
 #define UART1_IER   0xB04005F2
 #define UART1_DLM   0xB04005F2
 #define UART1_IIR   0xB04005F4
 #define UART1_FCR   0xB04005F4
 #define UART1_LCR   0xB04005F6
 #define UART1_MCR   0xB04005F8
 #define UART1_LSR   0xB04005FA
 #define UART1_MSR   0xB04005FC
 #define UART1_SCR   0xB04005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif
#if BTYPE == 6
 #ifndef SH7615_H
  #include "sh7615.h"
 #endif
 #define UART0_RBR   0x220007F0
 #define UART0_THR   0x220007F0
 #define UART0_DLL   0x220007F0
 #define UART0_IER   0x220007F2
 #define UART0_DLM   0x220007F2
 #define UART0_IIR   0x220007F4
 #define UART0_FCR   0x220007F4
 #define UART0_LCR   0x220007F6
 #define UART0_MCR   0x220007F8
 #define UART0_LSR   0x220007FA
 #define UART0_MSR   0x220007FC
 #define UART0_SCR   0x220007FE
 #define INT_UART0   INT_IRL8
 #define IP_UART0    8

 #define UART1_RBR   0x220005F0
 #define UART1_THR   0x220005F0
 #define UART1_DLL   0x220005F0
 #define UART1_IER   0x220005F2
 #define UART1_DLM   0x220005F2
 #define UART1_IIR   0x220005F4
 #define UART1_FCR   0x220005F4
 #define UART1_LCR   0x220005F6
 #define UART1_MCR   0x220005F8
 #define UART1_LSR   0x220005FA
 #define UART1_MSR   0x220005FC
 #define UART1_SCR   0x220005FE
 #define INT_UART1   INT_IRL12
 #define IP_UART1    12
#endif


/*
 * I/O Access Macro
 */
#define port_out(n,c)   (*((volatile char *)(n) ) = c )
#define port_in(n)      (*(volatile char *)(n) )
#define port_set(n,c)   (*(volatile char *)(n) |= c )
#define port_clr(n,c)   (*(volatile char *)(n) &= ~c )
#define port_outw(n,c)  (*((volatile short *)(n) ) = c )
#define port_inw(n)     (*(volatile short *)(n) )
#define port_setw(n,c)   (*(volatile short *)(n) |= c )
#define port_clrw(n,c)   (*(volatile short *)(n) &= ~c )

/*
 * �n�[�h�����ˑ���I/O�A�N�Z�X�}�N��
 *
 */
#define sci_in(n)       (port_inw((n))>>8)
#define sci_out(n,c)    (port_outw(n,((c)<<8)))
#define sci_set(n,c)    (port_setw(n,((c)<<8)))
#define sci_clr(n,c)    (port_clrw(n,((c)<<8)))

/*
 * ����/�X�e�[�^�X�r�b�g��`
 *
 * �������ł́A8�r�b�g�̊e���W�X�^�ł̈ʒu���`���Ă��܂��B
 *   MS7709SE01/MS7750SE01/MS7615SE01�ł́A���[�h�A�N�Z�X����
 *   ��ʂ����o���K�v������܂��B
 *   �|�[�g���o�͗p�̃}�N���̕��ŉ��ʂւ̃V�t�g�͍s���܂��B
 */

/* ���C���R���g���[������W�X�^ */

#define SIO_B5      0x00        /* �L�����N�^��5 */
#define SIO_B6      0x01        /* �L�����N�^��6 */
#define SIO_B7      0x02        /* �L�����N�^��7 */
#define SIO_B8      0x03        /* �L�����N�^��8 */
#define SIO_S1      0x00        /* �X�g�b�v�r�b�g1 */
#define SIO_S2      0x04        /* �X�g�b�v�r�b�g2 */
#define SIO_NONE    0x00        /* �p���e�B�Ȃ� */
#define SIO_ODD     0x08        /* �p���e�B� */
#define SIO_EVEN    0x18        /* �p���e�B���� */
#define SIO_SB      0x40        /* �u���[�N���M */
#define SIO_DLAB    0x80        /* DLAB */

/* ���f���R���g���[������W�X�^ */

#define SIO_DTR     0x01        /* DTR�M��ON(1)/OFF(0) */
#define SIO_RTS     0x02        /* RTS�M��ON(1)/OFF(0) */
#define SIO_OUT1    0x04        /* �������f���̃��Z�b�g */
#define SIO_OUT2    0x08        /* �����ݗv���}�X�N(0) */

/* ���C���X�e�[�^�X */

#define SIO_RxRDY   0x01        /* ��M�f�[�^�L�� */
#define SIO_OE      0x02        /* �I�[�o�����G���[ */
#define SIO_PE      0x04        /* �p���e�B�G���[ */
#define SIO_FE      0x08        /* �t���[�~���O�G���[ */
#define SIO_BD      0x10        /* �u���[�N��Ԍ��o */
#define SIO_TxRDY   0x20        /* ���M�z�[���f�B���O���W�X�^�� */
#define SIO_TxEMP   0x40        /* ���M�V�t�g���W�X�^�� */

/* ���f���X�e�[�^�X */

#define SIO_CTS     0x10        /* ���M�� */
#define SIO_DSR     0x20        /* ���u���f�B */
#define SIO_CI      0x40        /* ���� */
#define SIO_CD      0x80        /* ��M�L�����A���o */

/* �����݋��� */

#define SIO_RxIE    0x01        /* ��M�����݋���(1)/�֎~(0) */
#define SIO_TxIE    0x02        /* ���M�����݋���(1)/�֎~(0) */
#define SIO_LSIE    0x04        /* ��M���C����X�e�[�^�X�����݋���(1)/�֎~(0)*/
#define SIO_MSIE    0x08        /* ���f����X�e�[�^�X�����݋���(1)/�֎~(0) */

/*
 * FCR:FIFO Control Register
 */

/* Received data count control for Receive interrupt    */
#define SIO_TRG01   0x00        /*  1byte       */
#define SIO_TRG04   0x40        /*  4bytes      */
#define SIO_TRG08   0x80        /*  8bytes      */
#define SIO_TRG14   0xc0        /* 16bytes      */

#define SIO_DMA     0x08        /* DMA mode                 */
#define SIO_TXFRST  0x04        /* Transmitter FIFO reset   */
#define SIO_RXFRST  0x02        /* Receiver FIFO reset      */
#define SIO_FIFOEN  0x01        /* Enable FIFO (TX & RX)    */

#ifdef __cplusplus
}
#endif

#endif /* NOS16550_H */
