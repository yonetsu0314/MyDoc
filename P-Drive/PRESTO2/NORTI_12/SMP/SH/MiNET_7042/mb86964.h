/******************************************************************************
* Fujitsu MB86964 Definitions                                                 *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Nov/1998                                                                 *
******************************************************************************/

/* MB86964 内部レジスタ */

#define DLCR0       0x00        /* 送信ステータス */
#define DLCR1       0x01        /* 受信ステータス */
#define DLCR2       0x02        /* 送信割込みイネーブル */
#define DLCR3       0x03        /* 受信割込みイネーブル */
#define DLCR4       0x04        /* 送信モード */
#define DLCR5       0x05        /* 受信モード */
#define DLCR6       0x06        /* コントロール１ */
#define DLCR7       0x07        /* コントロール２ */

#define DLCR8       0x08        /* ノード ID 0 */
#define DLCR9       0x09        /* ノード ID 1 */
#define DLCR10      0x0a        /* ノード ID 2 */
#define DLCR11      0x0b        /* ノード ID 3 */
#define DLCR12      0x0c        /* ノード ID 4 */
#define DLCR13      0x0d        /* ノード ID 5 */
#define DLCR14      0x0e        /* TDR 0 */
#define DLCR15      0x0f        /* TDR 1 */

#define MAR8        0x08        /* マルチスキャンアドレス 0 */
#define MAR9        0x09        /* マルチスキャンアドレス 1 */
#define MAR10       0x0a        /* マルチスキャンアドレス 2 */
#define MAR11       0x0b        /* マルチスキャンアドレス 3 */
#define MAR12       0x0c        /* マルチスキャンアドレス 4 */
#define MAR13       0x0d        /* マルチスキャンアドレス 5 */
#define MAR14       0x0e        /* マルチスキャンアドレス 6 */
#define MAR15       0x0f        /* マルチスキャンアドレス 7 */

#define BMPR8       0x08        /* バッファメモリポート */
#define BMPR10      0x0a        /* 送信パケット数 */
#define BMPR11      0x0b        /* 16 コリジョン */
#define BMPR12      0x0c        /* DMA イネーブル */
#define BMPR13      0x0d        /* DMA バースト/トランシーバモード */
#define BMPR14      0x0e        /* 受信コントロール/トランシーバ割込み */
#define BMPR15      0x0f        /* トランシーバステータス */

/* 送信ステータスレジスタ */

#define DLCR0_TMT_OK        0x80    /* 送信 OK */
#define DLCR0_NET_BSY       0x40    /* ネットビジー */
#define DLCR0_TMT_REC       0x20    /* 送信パケット受信 */
#define DLCR0_SRT_PKT       0x10    /* ショートパケット */
#define DLCR0_JABBER        0x08    /* ジャバー */
#define DLCR0_COL           0x04    /* コリジョンエラー */
#define DLCR0_16COL         0x02    /* 16 コリジョンエラー */
#define DLCR0_BUS_WR_ERR    0x01    /* バスライトエラー */

/* 受信ステータスレジスタ */

#define DLCR1_PKT_RDY       0x80    /* パケットレディ */
#define DLCR1_BUS_RD_ERR    0x40    /* バスリードエラー */
#define DLCR1_DMA_EOP       0x20    /* DMA 転送完了 */
#define DLCR1_RMT_RST       0x10    /* リモートリセット */
#define DLCR1_RX_SRT_PKT    0x08    /* ショートパケットエラー */
#define DLCR1_ALG_ERR       0x04    /* アラインメントエラー */
#define DLCR1_CRC_ERR       0x02    /* CRC エラー */
#define DLCR1_OVRFLO        0x01    /* オーバフローエラー */

/* 送信割込みイネーブルレジスタ */

#define DLCR2_ENA_TMT_OK    0x80    /* TMT OK 割込み発生許可 */
#define DLCR2_ENA_JABBER    0x08    /* JABBER 割込み発生許可 */
#define DLCR2_ENA_COL       0x04    /* COL 割込み発生許可 */
#define DLCR2_ENA_16COL     0x02    /* 16COL 割込み発生許可 */
#define DLCR2_ENA_BUS_WR_ERR 0x01   /* BUS WR ERR 割込み発生許可 */

/* 受信割込みイネーブルレジスタ */

#define DLCR3_ENA_PKT_RDY   0x80    /* PKT RDY 割込み発生許可 */
#define DLCR3_ENA_BUS_RD_ERR 0x40   /* BUS RD ERR 割込み発生許可 */
#define DLCR3_ENA_DMA_EOP   0x20    /* DMA EOP 割込み発生許可 */
#define DLCR3_ENA_RMT_RST   0x10    /* RMT RST 割込み発生許可 */
#define DLCR3_ENA_RX_SRT_ERR 0x08   /* RX SRT ERR 割込み発生許可 */
#define DLCR3_ENA_ALG_ERR   0x04    /* ALG ERR 割込み発生許可 */
#define DLCR3_ENA_CRC_ERR   0x02    /* CRC ERR 割込み発生許可 */
#define DLCR3_ENA_OVRFLO    0x01    /* OVRFLO 割込み発生許可 */

/* 送信モードレジスタ */

#define DLCR4_COL3_COL0     0xf0    /* コリジョンカウント */
#define DLCR4_TST1          0x08    /* チップテスト1 */
#define DLCR4_NCNTRL        0x04    /* 0:CNTRL 端子 High, 1:Low */
#define DLCR4_NLBC          0x02    /* ループバックコントロール */
#define DLCR4_DSC           0x01    /* ディセーブルキャリアディテクト */

/* 受信モードレジスタ */

#define DLCR5_TST2          0x80    /* チップテスト2 */
#define DLCR5_BUF_EMP       0x40    /* 受信バッファ空 */
#define DLCR5_ACPT_BAD_PKT  0x20    /* 不良パケット受信 */
#define DLCR5_ADD_SIZE      0x10    /* アドレスサイズ（1:5バイトのみ比較）*/
#define DLCR5_ENA_SRT_PKT   0x08    /* イネーブルショートパケット */
#define DLCR5_ENA_RMT_RST   0x04    /* イネーブルリモートリセット */
#define DLCR5_AM1           0x02    /* アドレスマッチモード */
#define DLCR5_AM0           0x01    /* 〃 */

/* コントロール１レジスタ */

#define DLCR6_NENA_DLC      0x80    /* 1:イニシャライズ, 0:動作許可 */
#define DLCR6_100NS         0x40    /* 外付けSRAM 100ns */
#define DLCR6_SB            0x20    /* システムデータバス8ビット */
#define DLCR6_BB            0x10    /* バッファメモリデータバス8ビット */
#define DLCR6_TX_BUF_SIZE1  0x08    /* 送信バッファサイズ */
#define DLCR6_TX_BUF_SIZE0  0x04    /* 〃 */
#define DLCR6_BUF_SIZE1     0x02    /* 外付けバッファメモリサイズ */
#define DLCR6_BUF_SIZE0     0x01    /* 〃 */

/* コントロール２レジスタ */

#define DLCR7_IDENT1        0x80    /* コントローラ種別 */
#define DLCR7_IDENT0        0x40    /* 00:MB86960A, 01:MB86964, 11:MB86965A */
#define DLCR7_NSTBY         0x20    /* 0:スタンバイモード */
#define DLCR7_RDYPNSEL      0x10    /* RDYPNSEL 端子値 */
#define DLCR7_RBS1          0x08    /* レジスタバンク選択 */
#define DLCR7_RBS0          0x04    /* 〃 */
#define DLCR7_EOP           0x02    /* 0:EOP 入力クティブ Low, 1:High */
#define DLCR7_BYTE_SWAP     0x01    /* バイトスワップ */

/* 16回コリジョンコントロールレジスタ */

#define BMPR11_16COL_CNT2   0x04
#define BMPR11_16COL_CNT1   0x02
#define BMPR11_16COL_CNT0   0x01

/* DMA イネーブルレジスタ */

#define BMPR12_DMA_RENA     0x02    /* DMA リード動作イネーブル */
#define BMPR12_DMA_TENA     0x01    /* DMA ライト動作イネーブル */

/* DMA バースト／トランシーバモードコントロールレジスタ */

#define BMPR13_IO_BASE_UNLOCK 0x80
#define BMPR13_LOWER_SQUELCH  0x40
#define BMPR13_LINK_TEST_ENA  0x20
#define BMPR13_AUI            0x10
#define BMPR13_AUTO_PORT_SEL  0x08
#define BMPR13_STP            0x04
#define BMPR13_DMA_BRST1      0x02
#define BMPR13_DMA_BRST0      0x01

/* 受信コントロール／トランシーバ割込みイネーブルレジスタ */

#define BMPR14_ENA_RLD_INT  0x80
#define BMPR14_ENA_LLD_INT  0x40
#define BMPR14_ENA_RJAB_INT 0x20
#define BMPR14_SKIP_RX_PKT  0x04
#define BMPR14_ENA_SQE_INT  0x02
#define BMPR14_ENA_FILTER   0x01

/* トランシーバステータスレジスタ */

#define BMPR15_RLD          0x80
#define BMPR15_LLD          0x40
#define BMPR15_RJAB         0x20
#define BMPR15_RMT_PORT     0x10
#define BMPR15_POL_REV      0x08
#define BMPR15_SQE          0x02

/* 受信パケットステータス */

#define PKT_RDY             0x80
#define GOOD_PKT            0x20
#define OVRFLO              0x01

/* end */
