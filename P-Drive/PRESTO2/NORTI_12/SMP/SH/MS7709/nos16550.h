/******************************************************************************
* シルアル入出力デバイス定義ヘッダ                                            *
*                                                                             *
*  File name : nos16550.h                                                     *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 99-08-12 作成  TL16C552(MS7709RP01、ワードアクセス、上位８ビット）          *
* 99-09-30 MS7750SE01 対応（SMSC Super I/O FDC37C935A ）                      *
*          Super I/O のConfigurationはCPU初期化時にでも行ってください。       *
*          本モジュール内では、只の16550として扱います。                      *
* 16/Apr/2000                                                                 *
* 13/May/2000  Supported MS7615SE01 board                                     *
* 21/Aug/2000  CI, CD ビット位置を訂正                                        *
* 25/Dec/2002  MS7729R定義の追加                                           KA *
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
 * ハード実装依存のI/Oアクセスマクロ
 *
 */
#define sci_in(n)       (port_inw((n))>>8)
#define sci_out(n,c)    (port_outw(n,((c)<<8)))
#define sci_set(n,c)    (port_setw(n,((c)<<8)))
#define sci_clr(n,c)    (port_clrw(n,((c)<<8)))

/*
 * 制御/ステータスビット定義
 *
 * ※ここでは、8ビットの各レジスタでの位置を定義しています。
 *   MS7709SE01/MS7750SE01/MS7615SE01では、ワードアクセスして
 *   上位を取り出す必要があります。
 *   ポート入出力用のマクロの方で下位へのシフトは行います。
 */

/* ラインコントロール･レジスタ */

#define SIO_B5      0x00        /* キャラクタ長5 */
#define SIO_B6      0x01        /* キャラクタ長6 */
#define SIO_B7      0x02        /* キャラクタ長7 */
#define SIO_B8      0x03        /* キャラクタ長8 */
#define SIO_S1      0x00        /* ストップビット1 */
#define SIO_S2      0x04        /* ストップビット2 */
#define SIO_NONE    0x00        /* パリティなし */
#define SIO_ODD     0x08        /* パリティ奇数 */
#define SIO_EVEN    0x18        /* パリティ偶数 */
#define SIO_SB      0x40        /* ブレーク送信 */
#define SIO_DLAB    0x80        /* DLAB */

/* モデムコントロール･レジスタ */

#define SIO_DTR     0x01        /* DTR信号ON(1)/OFF(0) */
#define SIO_RTS     0x02        /* RTS信号ON(1)/OFF(0) */
#define SIO_OUT1    0x04        /* 内蔵モデムのリセット */
#define SIO_OUT2    0x08        /* 割込み要求マスク(0) */

/* ラインステータス */

#define SIO_RxRDY   0x01        /* 受信データ有り */
#define SIO_OE      0x02        /* オーバランエラー */
#define SIO_PE      0x04        /* パリティエラー */
#define SIO_FE      0x08        /* フレーミングエラー */
#define SIO_BD      0x10        /* ブレーク状態検出 */
#define SIO_TxRDY   0x20        /* 送信ホールディングレジスタ空 */
#define SIO_TxEMP   0x40        /* 送信シフトレジスタ空 */

/* モデムステータス */

#define SIO_CTS     0x10        /* 送信可 */
#define SIO_DSR     0x20        /* 装置レディ */
#define SIO_CI      0x40        /* 着呼 */
#define SIO_CD      0x80        /* 受信キャリア検出 */

/* 割込み許可 */

#define SIO_RxIE    0x01        /* 受信割込み許可(1)/禁止(0) */
#define SIO_TxIE    0x02        /* 送信割込み許可(1)/禁止(0) */
#define SIO_LSIE    0x04        /* 受信ライン･ステータス割込み許可(1)/禁止(0)*/
#define SIO_MSIE    0x08        /* モデム･ステータス割込み許可(1)/禁止(0) */

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
