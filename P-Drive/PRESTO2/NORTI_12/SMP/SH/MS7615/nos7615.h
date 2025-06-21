/******************************************************************************
* シルアル入出力デバイス定義ヘッダ（SH7615）                                  *
*                                                                             *
*  File name : nos7615.h                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 17/Apr/2000 作成                                                      MiSPO *
******************************************************************************/

#ifndef NOS7615_H
#define NOS7615_H

#include "sh7615.h"
#include "nosio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* シリアルモードレジスタ(SCSMR) */

#define SMR_CKS0    0x01    /* クロックセレクト0 */
#define SMR_CKS1    0x02    /* クロックセレクト1 */
#define SMR_MP      0x04    /* マルチプロセッサモード */
#define SMR_STOP    0x08    /* １ストップビット(0), ２ストップビット(1) */
#define SMR_OE      0x10    /* 偶数パリティ(0), 奇数パリティ(1) */
#define SMR_PE      0x20    /* パリティイネーブル */
#define SMR_CHR     0x40    /* ８ビットデータ(0), ７ビットデータ(1) */
#define SMR_CA      0x80    /* 調歩同期式モード(0), クロック同期式モード(1) */

/* シリアルコントロールレジスタ(SCSCR) */

#define SCR_CKE0    0x01    /* クロックイネーブル0 */
#define SCR_CKE1    0x02    /* クロックイネーブル1 */
#define SCR_MPIE    0x08    /* マルチプロセッサ割り込みイネーブル */
#define SCR_RE      0x10    /* 受信イネーブル */
#define SCR_TE      0x20    /* 送信イネーブル */
#define SCR_RIE     0x40    /* 受信データフル／受信エラー割り込みイネーブル */
#define SCR_TIE     0x80    /* 送信データエンプティ割り込みイネーブル */

/* シリアルステータス1レジスタ(SC1SSR) */

#define SSR_DR      0x0001  /* レシーブデータレディ */
#define SSR_RDF     0x0002  /* レシーブデータレジスタフル */
#define SSR_PER     0x0004  /* パリティエラー */
#define SSR_FER     0x0008  /* フレーミングエラー */
#define SSR_BRK     0x0010  /* ブレーク検出 */
#define SSR_TDFE    0x0020  /* トランスミットFIFOデータエンプティ */
#define SSR_TEND    0x0040  /* トランスミットエンド */
#define SSR_ER      0x0080  /* レシーブエラー */
#define SSR_FER_MSK 0x0f00  /* フレーミングエラー数 */
#define SSR_PER_MSK 0xf000  /* パリティエラー数 */

/* シリアルステータス1レジスタ(SC2SSR) */

#define SSR2_ORER    0x01   /* オーバランエラー */
#define SSR2_EI      0x02   /* 受信データエラー無視イネーブル */
#define SSR2_MPBT    0x04   /* マルチプロセッサビットトランスファー */
#define SSR2_MPB     0x08   /* マルチプロセッサビット */
#define SSR2_N0      0x10   /* クロックビットレート比 */
#define SSR2_N1      0x20   /* クロックビットレート比 */
#define SSR2_RLM     0x40   /* レシーブLSB/MSBファースト選択 */
#define SSR2_TLM     0x80   /* トランスミットLSB/MSBファースト選択 */

/* FIFOコントロールレジスタ(SCFCR) */

#define FCR_LOOP    0x01    /* ループバックテスト */
#define FCR_RFRST   0x02    /* レシーブFIFOデータレジスタリセット */
#define FCR_TFRST   0x04    /* トランスミットFIFOデータレジスタリセット */
#define FCR_MCE     0x08    /* モデムコントロールイネーブル */
#define FCR_TTRG0   0x10    /* トランスミットFIFOデータ数トリガ */
#define FCR_TTRG1   0x20    /* トランスミットFIFOデータ数トリガ */
#define FCR_RTRG0   0x40    /* レシーブFIFOデータ数トリガ */
#define FCR_RTRG1   0x80    /* レシーブFIFOデータ数トリガ */

/* ポートB（SCIF 入出力端子との兼用ポート）*/

#define PB_RXD1     0x4000  /* RxD1 入力端子（PB14）*/
#define PBCR_RXD1   0x2000  /* RxD1 端子機能 */
#define PBCR_RXD1_M 0x3000  /* RxD1 端子機能マスク */

#define PB_TXD1     0x2000  /* TxD1 出力端子（PB13）*/
#define PBCR_TXD1   0x0800  /* TxD1 端子機能 */
#define PBCR_TXD1_M 0x0c00  /* TxD1 端子機能マスク */

#define PB_RTS1     0x1000  /* ~RTS 出力端子（PB12）*/
#define PBCR_RTS1   0x0200  /* ~RTS 端子機能 */
#define PBCR_RTS1_M 0x0300  /* ~RTS 端子機能マスク */

#define PB_CTS1     0x0800  /* ~CTS 入力端子（PB11）*/
#define PBCR_CTS1   0x0080  /* ~CTS 端子機能 */
#define PBCR_CTS1_M 0x00c0  /* ~CTS 端子機能マスク */

#define PB_RXD2     0x0020  /* RxD2 入力端子（PB5）*/
#define PBCR_RXD2   0x0800  /* RxD2 端子機能 */
#define PBCR_RXD2_M 0x0c00  /* RxD2 端子機能マスク */

#define PB_TXD2     0x0010  /* ~RTS 出力端子（PB4）*/
#define PBCR_TXD2   0x0200  /* TxD2 端子機能 */
#define PBCR_TXD2_M 0x0300  /* TxD2 端子機能マスク */

#ifdef __cplusplus
};
#endif
#endif /* NOS7615_H */
