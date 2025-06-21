/******************************************************************************
* シルアル入出力デバイス定義ヘッダ (SH7040シリーズ)                           *
*                                                                             *
*  File name : nos7040.h                                                      *
*  Copyright (c) 1997-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Aug/1997 新規作成(nos7040.h)                                          MI *
* 24/Apr/2000 NORTi Ver.4 兼用に改造(nos7040.h)                            FJ *
* 23/Dec/2002 SH7042/44/45で共通のためnos7040.hに戻す, コメント修正        MI *
******************************************************************************/

#ifndef NOS7040_H
#define NOS7040_H

#include "sh7040.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ユーザー定義関数 */

extern void rts0_on(void);      /* 物理チャネル0 RTS信号ON */
extern void rts0_off(void);     /* 物理チャネル0 RTS信号OFF */
extern void rts1_on(void);      /* 物理チャネル1 RTS信号ON */
extern void rts1_off(void);     /* 物理チャネル1 RTS信号OFF */

/*
   ハードウェアに合わせて、外部に上記の関数を定義してください。
   RTS出力が無い場合、rts?_on, rts?_off関数は何もせずリターンしてください。
   あるいは、フロー制御無しNOFLWマクロを定義してnos7040.cをコンパイルすれば、
   上記の関数の定義は必要ありません。
*/

/* シリアルモードレジスタ */

#define SMR_CKS0    0x01    /* クロックセレクト0 */
#define SMR_CKS1    0x02    /* クロックセレクト1 */
#define SMR_MP      0x04    /* マルチプロセッサモード */
#define SMR_STOP    0x08    /* １ストップビット(0), ２ストップビット(1) */
#define SMR_OE      0x10    /* 偶数パリティ(0), 奇数パリティ(1) */
#define SMR_PE      0x20    /* パリティイネーブル */
#define SMR_CHR     0x40    /* ８ビットデータ(0), ７ビットデータ(1) */
#define SMR_CA      0x80    /* 調歩同期式モード(0), クロック同期式モード(1) */

/* シリアルコントロールレジスタ */

#define SCR_CKE0    0x01    /* クロックイネーブル0 */
#define SCR_CKE1    0x02    /* クロックイネーブル1 */
#define SCR_TEIE    0x04    /* 送信終了割り込みイネーブル */
#define SCR_MPIE    0x08    /* マルチプロセッサ割り込みイネーブル */
#define SCR_RE      0x10    /* 受信イネーブル */
#define SCR_TE      0x20    /* 送信イネーブル */
#define SCR_RIE     0x40    /* 受信データフル／受信エラー割り込みイネーブル */
#define SCR_TIE     0x80    /* 送信データエンプティ割り込みイネーブル */

/* シリアルステータスレジスタ */

#define SSR_MPBT    0x01    /* マルチプロセッサビット送信 */
#define SSR_MPB     0x02    /* マルチプロセッサビット受信 */
#define SSR_TEND    0x04    /* 送信終了 */
#define SSR_PER     0x08    /* パリティエラー */
#define SSR_FER     0x10    /* フレーミングエラー */
#define SSR_ORER    0x20    /* オーバランエラー */
#define SSR_RDRF    0x40    /* 受信データレジスタフル */
#define SSR_TDRE    0x80    /* 送信データレジスタエンプティ */

/* ポートＡ(SCI 入出力端子との兼用ポート)*/

#define PA_RXD0     0x01    /* RxD0 入力端子 (PA0)*/
#define PA_TXD0     0x02    /* TxD0 出力端子 (PA1)*/
#define PA_CTS0     0x04    /* /CTS0入力端子 (PA2,/IRQ0)*/
#define PA_RXD1     0x08    /* RxD1 入力端子 (PA3)*/
#define PA_TXD1     0x10    /* TxD1 出力端子 (PA4)*/
#define PA_CTS1     0x20    /* /CTS1入力端子 (PA5,/IRQ1)*/

#ifdef __cplusplus
}
#endif
#endif /* NOS7040_H */
