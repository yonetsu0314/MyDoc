/******************************************************************************
* シルアル入出力デバイス定義ヘッダ (SH7727内蔵SCI/SCIF)                       *
*                                                                             *
*  File name : nos7727.h                                                      *
*  Copyright (c)  2001-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 18/Nov/2001 作成                                                            *
******************************************************************************/

#ifndef NOS7727_H
#define NOS7727_H

#include "sh7727.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* シリアルモードレジスタ */

#define SMR_CKS0    0x01    /* クロックセレクト0 */
#define SMR_CKS1    0x02    /* クロックセレクト1 */
#ifdef CH0
#define SMR_MP      0x04    /* マルチプロセッサモード */
#endif
#define SMR_STOP    0x08    /* １ストップビット(0), ２ストップビット(1) */
#define SMR_OE      0x10    /* 偶数パリティ(0), 奇数パリティ(1) */
#define SMR_PE      0x20    /* パリティイネーブル */
#define SMR_CHR     0x40    /* ８ビットデータ(0), ７ビットデータ(1) */
#ifdef CH0
#define SMR_CA      0x80    /* 調歩同期式モード(0), クロック同期式モード(1) */
#endif

/* シリアルコントロールレジスタ */

#define SCR_CKE0    0x01    /* クロックイネーブル0 */
#define SCR_CKE1    0x02    /* クロックイネーブル1 */
#ifdef CH0
#define SCR_TEIE    0x04    /* 送信終了割り込みイネーブル */
#define SCR_MPIE    0x08    /* マルチプロセッサ割り込みイネーブル */
#endif
#define SCR_RE      0x10    /* 受信イネーブル */
#define SCR_TE      0x20    /* 送信イネーブル */
#define SCR_RIE     0x40    /* 受信データフル／受信エラー割り込みイネーブル */
#define SCR_TIE     0x80    /* 送信データエンプティ割り込みイネーブル */

/* シリアルステータスレジスタ(SCI) */

#ifdef CH0
#define SSR_MPBT    0x01    /* マルチプロセッサビット送信 */
#define SSR_MPB     0x02    /* マルチプロセッサビット受信 */
#define SSR_TEND    0x04    /* 送信終了 */
#define SSR_PER     0x08    /* パリティエラー */
#define SSR_FER     0x10    /* フレーミングエラー */
#define SSR_ORER    0x20    /* オーバランエラー */
#define SSR_RDRF    0x40    /* 受信データレジスタフル */
#define SSR_TDRE    0x80    /* 送信データレジスタエンプティ */

/* シリアルステータスレジスタ(SCIF) */

#else
#define SSR_DR      0x01    /* 受信データレディ */
#define SSR_RDF     0x02    /* 受信FIFOフル */
#define SSR_PER     0x04    /* パリティエラー */
#define SSR_FER     0x08    /* フレーミングエラー */
#define SSR_BRK     0x10    /* ブレーク検出 */
#define SSR_TDFE    0x20    /* 送信FIFOデータエンプティ */
#define SSR_TEND    0x40    /* 送信終了 */
#define SSR_ER      0x80    /* 受信エラー */
#endif

/* ポートSCコントロールレジスタ */

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

/* ポートSCデータレジスタ */

#define SCP0DT      0x01
#define SCP1DT      0x02
#define SCP2DT      0x04
#define SCP3DT      0x08
#define SCP4DT      0x10
#define SCP5DT      0x20
#define SCP6DT      0x40
#define SCP7DT      0x80

#ifdef __cplusplus
}
#endif
#endif /* NOS7727_H */
