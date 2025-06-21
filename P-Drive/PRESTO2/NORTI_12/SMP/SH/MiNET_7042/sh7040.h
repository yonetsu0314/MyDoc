/******************************************************************************
* SH7040シリーズ内蔵周辺モジュール定義                                        *
*                                                                             *
*  File name : sh7040.h                                                       *
*  Copyright (c)  1997-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Aug/1997 新規作成 (sh7040.h)                                          MI *
* 25/Apr/2000 sh7042.hに改名, MiNET-7042ボード依存定義を追加               FJ *
* 31/Oct/2000 IAR EW 対応                                                  TK *
* 05/Mar/2001 sh7040.hに戻す, MiNET-7042ボード依存定義を削除               MI *
* 23/Dec/2002 sfr_iset, sfr_iclrを削除                                     MI *
******************************************************************************/

#ifndef SH7040_H
#define SH7040_H

/* 内蔵周辺モジュールのベースアドレス */

#ifdef CPU_SH /* SH用クロスコンパイラでコンパイルされている場合 */
#define IO_BASE ((volatile unsigned char *)0xffff8000L)
#endif

/* 内蔵周辺モジュール入出力マクロ */

UB sfr_in(unsigned n);              /* バイトデータの入力 */
void sfr_out(unsigned n, UB c);     /* バイトデータの出力 */
UH sfr_inw(unsigned n);             /* ワードデータの入力 */
void sfr_outw(unsigned n, UH c);    /* ワードデータの出力 */
UW sfr_inl(unsigned n);             /* ロングワードデータの入力 */
void sfr_outl(unsigned n, UW c);    /* ロングワードデータの出力 */
void sfr_set(unsigned n, UB c);     /* バイトデータのビットセット */
void sfr_clr(unsigned n, UB c);     /* バイトデータのビットクリア */
void sfr_setw(unsigned n, UH c);    /* ワードデータのビットセット */
void sfr_clrw(unsigned n, UH c);    /* ワードデータのビットクリア */
void sfr_iset(unsigned n, UB c);    /* バイトデータの即値ビットセット */
void sfr_iclr(unsigned n, UB c);    /* バイトデータの即値ビットクリア */

#ifdef IO_BASE
#define sfr_in(n)       (*(IO_BASE+(n)))
#define sfr_out(n,c)    (*(IO_BASE+(n))=(c))
#define sfr_inw(n)      (*(volatile UH *)(IO_BASE+(n)))
#define sfr_outw(n,c)   (*(volatile UH *)(IO_BASE+(n))=(c))
#define sfr_inl(n)      (*(volatile UW *)(IO_BASE+(n)))
#define sfr_outl(n,c)   (*(volatile UW *)(IO_BASE+(n))=(c))
#define sfr_set(n,c)    (*(IO_BASE+(n))|=(c))
#define sfr_clr(n,c)    (*(IO_BASE+(n))&=~(c))
#define sfr_setw(n,c)   (*(volatile UH *)(IO_BASE+(n))|=(c))
#define sfr_clrw(n,c)   (*(volatile UH *)(IO_BASE+(n))&=~(c))
#endif

/* シリアルコミュニケーションインターフェース(SCI) チャネル0 */

#define SCI_SMR0    0x1a0   /* シリアルモードレジスタ */
#define SCI_BRR0    0x1a1   /* ビットレートレジスタ */
#define SCI_SCR0    0x1a2   /* シリアルコントロールレジスタ */
#define SCI_TDR0    0x1a3   /* トランスミットデータレジスタ */
#define SCI_SSR0    0x1a4   /* シリアルステータスレジスタ */
#define SCI_RDR0    0x1a5   /* レシーブデータレジスタ */

/* シリアルコミュニケーションインターフェース(SCI) チャネル1 */

#define SCI_SMR1    0x1b0   /* シリアルモードレジスタ */
#define SCI_BRR1    0x1b1   /* ビットレートレジスタ */
#define SCI_SCR1    0x1b2   /* シリアルコントロールレジスタ */
#define SCI_TDR1    0x1b3   /* トランスミットデータレジスタ */
#define SCI_SSR1    0x1b4   /* シリアルステータスレジスタ */
#define SCI_RDR1    0x1b5   /* レシーブデータレジスタ */

/* マルチファンクションタイマパルスユニット(MTU) 共通 */

#define MTU_TSTR    0x240   /* タイマスタートレジスタ */
#define MTU_TSYR    0x241   /* タイマシンクロレジスタ */

/* マルチファンクションタイマパルスユニット(MTU) チャネル0 */

#define MTU_TCR0    0x260   /* タイマコントロールレジスタ0 */
#define MTU_TMDR0   0x261   /* タイマモードレジスタ0 */
#define MTU_TIOR0H  0x262   /* タイマ I/O コントロールレジスタ0H */
#define MTU_TIOR0L  0x263   /* タイマ I/O コントロールレジスタ0L */
#define MTU_TIER0   0x264   /* タイマインタラプトイネーブルレジスタ0 */
#define MTU_TSR0    0x265   /* タイマステータスレジスタ0 */
#define MTU_TCNT0   0x266   /* タイマカウンタ0 */
#define MTU_TGR0A   0x268   /* ジェネラルレジスタ0A */
#define MTU_TGR0B   0x26a   /* ジェネラルレジスタ0B */
#define MTU_TGR0C   0x26c   /* ジェネラルレジスタ0C */
#define MTU_TGR0D   0x26e   /* ジェネラルレジスタ0D */

/* マルチファンクションタイマパルスユニット(MTU) チャネル1 */

#define MTU_TCR1    0x280   /* タイマコントロールレジスタ1 */
#define MTU_TMDR1   0x281   /* タイマモードレジスタ1 */
#define MTU_TIOR1   0x282   /* タイマ I/O コントロールレジスタ1 */
#define MTU_TIER1   0x284   /* タイマインタラプトイネーブルレジスタ1 */
#define MTU_TSR1    0x285   /* タイマステータスレジスタ1 */
#define MTU_TCNT1   0x286   /* タイマカウンタ1 */
#define MTU_TGR1A   0x288   /* ジェネラルレジスタ1A */
#define MTU_TGR1B   0x28a   /* ジェネラルレジスタ1B */

/* マルチファンクションタイマパルスユニット(MTU) チャネル2 */

#define MTU_TCR2    0x2a0   /* タイマコントロールレジスタ2 */
#define MTU_TMDR2   0x2a1   /* タイマモードレジスタ2 */
#define MTU_TIOR2   0x2a2   /* タイマ I/O コントロールレジスタ2 */
#define MTU_TIER2   0x2a4   /* タイマインタラプトイネーブルレジスタ2 */
#define MTU_TSR2    0x2a5   /* タイマステータスレジスタ2 */
#define MTU_TCNT2   0x2a6   /* タイマカウンタ2 */
#define MTU_TGR2A   0x2a8   /* ジェネラルレジスタ2A */
#define MTU_TGR2B   0x2aa   /* ジェネラルレジスタ2B */

/* マルチファンクションタイマパルスユニット(MTU) チャネル3 */

#define MTU_TCR3    0x200   /* タイマコントロールレジスタ3 */
#define MTU_TMDR3   0x202   /* タイマモードレジスタ3 */
#define MTU_TIOR3H  0x204   /* タイマ I/O コントロールレジスタ3H */
#define MTU_TIOR3L  0x205   /* タイマ I/O コントロールレジスタ3L */
#define MTU_TIER3   0x208   /* タイマインタラプトイネーブルレジスタ3 */
#define MTU_TSR3    0x22c   /* タイマステータスレジスタ3 */
#define MTU_TCNT3   0x210   /* タイマカウンタ3 */
#define MTU_TGR3A   0x218   /* ジェネラルレジスタ3A */
#define MTU_TGR3B   0x21a   /* ジェネラルレジスタ3B */
#define MTU_TGR3C   0x224   /* ジェネラルレジスタ3C */
#define MTU_TGR3D   0x226   /* ジェネラルレジスタ3D */

/* マルチファンクションタイマパルスユニット(MTU) チャネル4 */

#define MTU_TCR4    0x201   /* タイマコントロールレジスタ4 */
#define MTU_TMDR4   0x203   /* タイマモードレジスタ4 */
#define MTU_TIOR4H  0x206   /* タイマ I/O コントロールレジスタ4H */
#define MTU_TIOR4L  0x207   /* タイマ I/O コントロールレジスタ4L */
#define MTU_TIER4   0x209   /* タイマインタラプトイネーブルレジスタ4 */
#define MTU_TSR4    0x22d   /* タイマステータスレジスタ4 */
#define MTU_TCNT4   0x212   /* タイマカウンタ4 */
#define MTU_TGR4A   0x21c   /* ジェネラルレジスタ4A */
#define MTU_TGR4B   0x21e   /* ジェネラルレジスタ4B */
#define MTU_TGR4C   0x228   /* ジェネラルレジスタ4C */
#define MTU_TGR4D   0x22a   /* ジェネラルレジスタ4D */

/* マルチファンクションタイマパルスユニット(MTU) チャネル3/4 共通 */

#define MTU_TOER    0x20a   /* タイマoマスタイネーブルレジスタ */
#define MTU_TOCR    0x20b   /* タイマoコントロールレジスタ */
#define MTU_TGCR    0x20d   /* タイマゲートコントロールレジスタ */
#define MTU_TCDR    0x214   /* タイマ周期データレジスタ */
#define MTU_TDDR    0x216   /* タイマデッドタイムデータレジスタ */
#define MTU_TCNTS   0x220   /* タイマサブカウンタ */
#define MTU_TCBR    0x222   /* タイマ周期バッファレジスタ */

/* MTU ポートアウトプットイネーブル(POE) */

#define MTU_ICSR    0x3c0   /* 入力レベルコントロール/ステータスレジスタ */
#define MTU_OCSR    0x3c2   /* 出力レベルコントロール/ステータスレジスタ */

/* 割り込みコントローラ(INTC) */

#define INTC_IPRA   0x348   /* 割り込み優先レベル設定レジスタA */
#define INTC_IPRB   0x34a   /* 割り込み優先レベル設定レジスタB */
#define INTC_IPRC   0x34c   /* 割り込み優先レベル設定レジスタC */
#define INTC_IPRD   0x34e   /* 割り込み優先レベル設定レジスタD */
#define INTC_IPRE   0x350   /* 割り込み優先レベル設定レジスタE */
#define INTC_IPRF   0x352   /* 割り込み優先レベル設定レジスタF */
#define INTC_IPRG   0x354   /* 割り込み優先レベル設定レジスタG */
#define INTC_IPRH   0x356   /* 割り込み優先レベル設定レジスタH */
#define INTC_ICR    0x358   /* 割り込みコントロールレジスタ */
#define INTC_ISR    0x35a   /* IRQ ステータスレジスタ */

/* I/Oポート */

#define IO_PADRH    0x380   /* ポートA データレジスタH */
#define IO_PADRL    0x382   /* ポートA データレジスタL */
#define IO_PBDR     0x390   /* ポートB データレジスタ */
#define IO_PCDR     0x392   /* ポートC データレジスタ */
#define IO_PDDRH    0x3a0   /* ポートD データレジスタH */
#define IO_PDDRL    0x3a2   /* ポートD データレジスタL */
#define IO_PEDR     0x3b0   /* ポートE データレジスタ */
#define IO_PFDR     0x3b3   /* ポートF データレジスタ */

/* ピンファンクションコントローラ(PFC) */

#define PFC_PAIORH  0x384   /* ポートA IOレジスタH */
#define PFC_PAIORL  0x386   /* ポートA IOレジスタL */
#define PFC_PACRH   0x388   /* ポートA コントロールレジスタH */
#define PFC_PACRL1  0x38c   /* ポートA コントロールレジスタL1 */
#define PFC_PACRL2  0x38e   /* ポートA コントロールレジスタL2 */
#define PFC_PBIOR   0x394   /* ポートB IOレジスタ */
#define PFC_PBCR1   0x398   /* ポートB コントロールレジスタ1 */
#define PFC_PBCR2   0x39a   /* ポートB コントロールレジスタ2 */
#define PFC_PCIOR   0x396   /* ポートC IOレジスタ */
#define PFC_PCCR    0x39c   /* ポートC コントロールレジスタ */
#define PFC_PDIORH  0x3a4   /* ポートD IOレジスタH */
#define PFC_PDIORL  0x3a6   /* ポートD IOレジスタL */
#define PFC_PDCRH1  0x3a8   /* ポートD コントロールレジスタH1 */
#define PFC_PDCRH2  0x3aa   /* ポートD コントロールレジスタH2 */
#define PFC_PDCRL   0x3ac   /* ポートD コントロールレジスタL */
#define PFC_PEIOR   0x3b4   /* ポートE IOレジスタ */
#define PFC_PECR1   0x3b8   /* ポートE コントロールレジスタ1 */
#define PFC_PECR2   0x3ba   /* ポートE コントロールレジスタ2 */
#define PFC_IFCR    0x3c8   /* IRQOUT 機能コントロールレジスタ */

/* コンペアマッチタイマ(CMT) 共通 */

#define CMT_CMSTR   0x3d0   /* コンペアマッチタイマスタートレジスタ */

/* コンペアマッチタイマ(CMT) チャネル0 */

#define CMT_CMCSR0  0x3d2   /* コンペアマッチタイマコントロール/ステータスレジスタ0 */
#define CMT_CMCNT0  0x3d4   /* コンペアマッチタイマカウンタ0 */
#define CMT_CMCOR0  0x3d6   /* コンペアマッチタイマコンスタントレジスタ0 */

/* コンペアマッチタイマ(CMT) チャネル1 */

#define CMT_CMCSR1  0x3d8   /* コンペアマッチタイマコントロール/ステータスレジスタ1 */
#define CMT_CMCNT1  0x3da   /* コンペアマッチタイマカウンタ1 */
#define CMT_CMCOR1  0x3dc   /* コンペアマッチタイマコンスタントレジスタ1 */

/* A/D変換器 */

#define AD_DRA      0x3f0   /* A/D データレジスタA */
#define AD_DRB      0x3f2   /* A/D データレジスタB */
#define AD_DRC      0x3f4   /* A/D データレジスタC */
#define AD_DRD      0x3f6   /* A/D データレジスタD */
#define AD_DRE      0x3f8   /* A/D データレジスタE */
#define AD_DRF      0x3fa   /* A/D データレジスタF */
#define AD_DRG      0x3fc   /* A/D データレジスタG */
#define AD_DRH      0x3fe   /* A/D データレジスタH */
#define AD_CSR      0x3e0   /* A/D コントロール/ステータスレジスタ */
#define AD_CR       0x3e1   /* A/D コントロールレジスタ */

/* ユーザブレークコントローラ(UBC) */

#define UBC_UBARH   0x600   /* ユーザブレークアドレスレジスタH */
#define UBC_UBARL   0x602   /* ユーザブレークアドレスレジスタL */
#define UBC_UBAMRH  0x604   /* ユーザブレークアドレスマスクレジスタH */
#define UBC_UBAMRL  0x606   /* ユーザブレークアドレスマスクレジスタL */
#define UBC_UBBR    0x608   /* ユーザブレークバスサイクルレジスタ */

/* ウォッチドッグタイマ(WDT) */

#define WDT_TCSR    0x610   /* タイマコントロール/ステータスレジスタ */
#define WDT_TCNT    0x610   /* タイマカウンタ(書き込み) */
#define WDT_TCNT_R  0x611   /* タイマカウンタ(読み出し) */
#define WDT_RSTCSR  0x612   /* リセットコントロール/ステータスレジスタ(書き込み) */
#define WDT_RSTCSR_R 0x613  /* リセットコントロール/ステータスレジスタ(読み出し) */

/* 低消費電力状態 */

#define SYS_SBYCR   0x614   /* スタンバイコントロールレジスタ */

/* バスステートコントローラ(BSC) */

#define BSC_BCR1    0x620   /* バスコントロールレジスタ1 */
#define BSC_BCR2    0x622   /* バスコントロールレジスタ2 */
#define BSC_WCR1    0x624   /* ウェイトコントロールレジスタ1 */
#define BSC_WCR2    0x626   /* ウェイトコントロールレジスタ2 */
#define BSC_DCR     0x62a   /* DRAM エリアコントロールレジスタ */
#define BSC_RTCSR   0x62c   /* リフレッシュタイマコントロール/ステータスレジスタ */
#define BSC_RTCNT   0x62e   /* リフレッシュタイマカウンタ */
#define BSC_RTCOR   0x630   /* リフレッシュタイムコンスタントレジスタ */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル0 */

#define DMAC_SAR0   0x6c0   /* DMA ソースアドレスレジスタ0 */
#define DMAC_DAR0   0x6c4   /* DMA デスティネーションアドレスレジスタ0 */
#define DMAC_TCR0   0x6c8   /* DMA トランスファカウントレジスタ0 */
#define DMAC_CHCR0  0x6cc   /* DMA チャネルコントロールレジスタ0 */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル1 */

#define DMAC_SAR1   0x6d0   /* DMA ソースアドレスレジスタ1 */
#define DMAC_DAR1   0x6d4   /* DMA デスティネーションアドレスレジスタ1 */
#define DMAC_TCR1   0x6d8   /* DMA トランスファカウントレジスタ1 */
#define DMAC_CHCR1  0x6dc   /* DMA チャネルコントロールレジスタ1 */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル2 */

#define DMAC_SAR2   0x6e0   /* DMA ソースアドレスレジスタ2 */
#define DMAC_DAR2   0x6e4   /* DMA デスティネーションアドレスレジスタ2 */
#define DMAC_TCR2   0x6e8   /* DMA トランスファカウントレジスタ2 */
#define DMAC_CHCR2  0x6ec   /* DMA チャネルコントロールレジスタ2 */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル3 */

#define DMAC_SAR3   0x6f0   /* DMA ソースアドレスレジスタ3 */
#define DMAC_DAR3   0x6f4   /* DMA デスティネーションアドレスレジスタ3 */
#define DMAC_TCR3   0x6fa   /* DMA トランスファカウントレジスタ3 */
#define DMAC_CHCR3  0x6fe   /* DMA チャネルコントロールレジスタ3 */

/* ダイレクトメモリアクセスコントローラ(DMAC) 共通 */

#define DMAC_DMAOR  0x6b0   /* DMA オペレーションレジスタ */

/* データトランスファコントローラ(DTC) */

#define DTC_DTEA    0x700   /* DTC イネーブルレジスタA */
#define DTC_DTEB    0x701   /* DTC イネーブルレジスタB */
#define DTC_DTEC    0x702   /* DTC イネーブルレジスタC */
#define DTC_DTED    0x703   /* DTC イネーブルレジスタD */
#define DTC_DTEE    0x704   /* DTC イネーブルレジスタE */
#define DTC_DTCSR   0x706   /* DTC コントロール/ステータスレジスタ */
#define DTC_DTBR    0x708   /* DTC 情報ベースレジスタ */

/* キャッシュメモリ(CAC) */

#define CAC_CCR     0x740   /* キャッシュコントロールレジスタ */

/* 割り込みベクタ番号 */

#define INT_NMI     11      /* NMI */
#define INT_UB      12      /* ユーザブレーク */
#define INT_IRQ0    64      /* 外部端子 IRQ0 */
#define INT_IRQ1    65      /* 外部端子 IRQ1 */
#define INT_IRQ2    66      /* 外部端子 IRQ2 */
#define INT_IRQ3    67      /* 外部端子 IRQ3 */
#define INT_IRQ4    68      /* 外部端子 IRQ4 */
#define INT_IRQ5    69      /* 外部端子 IRQ5 */
#define INT_IRQ6    70      /* 外部端子 IRQ6 */
#define INT_IRQ7    71      /* 外部端子 IRQ7 */
#define INT_DEI0    72      /* DMAC チャネル0 */
#define INT_DEI1    76      /* DMAC チャネル1 */
#define INT_DEI2    80      /* DMAC チャネル2 */
#define INT_DEI3    84      /* DMAC チャネル3 */
#define INT_TGI0A   88      /* MTU コンペアマッチ/インプットキャプチャ0A */
#define INT_TGI0B   89      /* MTU コンペアマッチ/インプットキャプチャ0B */
#define INT_TGI0C   90      /* MTU コンペアマッチ/インプットキャプチャ0C */
#define INT_TGI0D   91      /* MTU コンペアマッチ/インプットキャプチャ0D */
#define INT_TCI0V   92      /* MTU チｬネル0 オーバフロー */
#define INT_TGI1A   96      /* MTU コンペアマッチ/インプットキャプチャ1A */
#define INT_TGI1B   97      /* MTU コンペアマッチ/インプットキャプチャ1B */
#define INT_TCI1V   100     /* MTU チｬネル1 オーバフロー */
#define INT_TCI1U   101     /* MTU チｬネル1 アンダフロー */
#define INT_TGI2A   104     /* MTU コンペアマッチ/インプットキャプチャ2A */
#define INT_TGI2B   105     /* MTU コンペアマッチ/インプットキャプチャ2B */
#define INT_TCI2V   108     /* MTU チｬネル2 オーバフロー */
#define INT_TCI2U   109     /* MTU チｬネル2 アンダフロー */
#define INT_TGI3A   112     /* MTU コンペアマッチ/インプットキャプチャ3A */
#define INT_TGI3B   113     /* MTU コンペアマッチ/インプットキャプチャ3B */
#define INT_TGI3C   114     /* MTU コンペアマッチ/インプットキャプチャ3C */
#define INT_TGI3D   115     /* MTU コンペアマッチ/インプットキャプチャ3D */
#define INT_TCI3V   116     /* MTU チｬネル3 オーバフロー */
#define INT_TGI4A   120     /* MTU コンペアマッチ/インプットキャプチャ4A */
#define INT_TGI4B   121     /* MTU コンペアマッチ/インプットキャプチャ4B */
#define INT_TGI4C   122     /* MTU コンペアマッチ/インプットキャプチャ4C */
#define INT_TGI4D   123     /* MTU コンペアマッチ/インプットキャプチャ4D */
#define INT_TCI4V   124     /* MTU チｬネル4 オーバフロー/アンダフロー */
#define INT_ERI0    128     /* SCI チャネル0 受信エラー */
#define INT_RXI0    129     /* SCI チャネル0 受信データフル */
#define INT_TXI0    130     /* SCI チャネル0 送信データエンプティ */
#define INT_TEI0    131     /* SCI チャネル0 送信終了 */
#define INT_ERI1    132     /* SCI チャネル1 受信エラー */
#define INT_RXI1    133     /* SCI チャネル1 受信データフル */
#define INT_TXI1    134     /* SCI チャネル1 送信データエンプティ */
#define INT_TEI1    135     /* SCI チャネル1 送信終了 */
#define INT_ADI     136     /* A/D エンド */
#define INT_SWDTCE  140     /* DTC ソフトウェア起動 */
#define INT_CMI0    144     /* CMT チャネル0 コンペアマッチ */
#define INT_CMI1    148     /* CMT チャネル1 コンペアマッチ */
#define INT_ITI     152     /* ウォッチドックタイマ（インターバルタイマ） */
#define INT_CMI     153     /* リフレッシュコントローラコンペアマッチ */
#define INT_OEI     156     /* MTU POE */

#endif /* SH7040_H */
