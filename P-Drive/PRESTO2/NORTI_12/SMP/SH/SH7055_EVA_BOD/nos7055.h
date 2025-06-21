/******************************************************************************
* シルアル入出力デバイス定義ヘッダ（SH7055）                                  *
*                                                                             *
*  File name : nos7055.h                                                      *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 14/Jan/2001                                                                 *
******************************************************************************/

#ifndef NOS7055_H
#define NOS7055_H

#include "sh7055.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

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

/* ポート（SCI 入出力端子との兼用ポート）*/

#define PA_RXD0     0x8000  /* RxD0 入力端子（PA15）*/
#define PA_TXD0     0x4000  /* TxD0 出力端子（PA14）*/
#define PC_RXD1     0x0002  /* RxD1 入力端子（PC2）*/
#define PC_TXD1     0x0001  /* TxD1 出力端子（PC1）*/
#define PC_RXD2     0x0008  /* RxD2 入力端子（PC4）*/
#define PC_TXD2     0x0004  /* TxD2 出力端子（PC3）*/
#define PB_RXD3     0x0200  /* RxD3 入力端子（PB9）*/
#define PB_TXD3     0x0100  /* TxD3 出力端子（PB8）*/
#define PB_RXD4     0x0800  /* RxD4 入力端子（PB11）*/
#define PB_TXD4     0x0400  /* TxD4 出力端子（PB10）*/

#ifdef __cplusplus
}
#endif
#endif /* NOS7055_H */
