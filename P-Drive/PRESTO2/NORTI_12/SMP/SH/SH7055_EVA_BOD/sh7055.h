/******************************************************************************
* ＳＨ７０５５ 内部Ｉ／Ｏ定義                                                 *
*                                                                             *
*  File name : sh7055.h                                                       *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 06/Feb/2001 作成                                                            *
* 25/Feb/2003 Green Hills 判定ラベルを変更(__SH7000 -> __ghs)              KA *
******************************************************************************/

#ifndef SH7055_H
#define SH7055_H

/* 内部Ｉ／Ｏのベースアドレス */

#ifdef CPU_SH
#define IO_BASE ((volatile unsigned char *)0xffffe000L)
#endif

/* 内部Ｉ／Ｏ入出力マクロ */

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

#if defined(__GNUC__)
#elif defined(__ghs)
#else                   /* Renesas C */
#define sfr_iset(n,c)   do{set_gbr(IO_BASE+(n));gbr_or_byte(0,c);}while(0)
#define sfr_iclr(n,c)   do{set_gbr(IO_BASE+(n));gbr_and_byte(0,~(c));}while(0)
#endif
#endif

/* バス・コントローラ */

#define BSC_BCR1        0x0c20  /* バス・コントロール・レジスタ1 */
#define BSC_BCR2        0x0c22  /* バス・コントロール・レジスタ2 */
#define BSC_WCR         0x0c24  /* ウェイト・ステート制御レジスタ */
#define BSC_RAMER       0x0c26  /* RAM エミュレーションレジスタ */

/* ＷＤＴ */

#define WDT_TCSR        0x0c10  /* タイマ・コントロール／ステータス・レジスタ */
#define WDT_TCNT        0x0c10  /* タイマ・カウンタ（wr）*/
#define WDT_TCNT_R      0x0c11  /* タイマ・カウンタ（rd）*/
#define WDT_RSTCSR      0x0c12  /* リセｯト・コントロｰル/ステｰタス・レジスタ（wr）*/
#define WDT_RSTCSR_R    0x0c13  /* リセｯト・コントロｰル/ステｰタス・レジスタ（rd）*/

/* ＤＭＡＣ共通 */

#define DMAC_DMAOR      0x0cb0  /* DMA オペレーション・レジスタ */

/* ＤＭＡＣチャネル０ */

#define DMAC_SAR0       0x0cc0  /* DMA ソース・アドレス・レジスタ0 */
#define DMAC_DAR0       0x0cc4  /* DMA デスティネーション・アドレス・レジスタ0 */
#define DMAC_DMATCR0    0x0cc8  /* DMA トランスファ・カウント・レジスタ0 */
#define DMAC_CHCR0      0x0ccc  /* DMA チャネル・コントロール・レジスタ0 */

/* ＤＭＡＣチャネル１ */

#define DMAC_SAR1       0x0cd0  /* DMA ソース・アドレス・レジスタ1 */
#define DMAC_DAR1       0x0cd4  /* DMA デスティネーション・アドレス・レジスタ1 */
#define DMAC_DMATCR1    0x0cd8  /* DMA トランスファ・カウント・レジスタ1 */
#define DMAC_CHCR1      0x0cdc  /* DMA チャネル・コントロール・レジスタ1 */

/* ＤＭＡＣチャネル２ */

#define DMAC_SAR2       0x0ce0  /* DMA ソース・アドレス・レジスタ2 */
#define DMAC_DAR2       0x0ce4  /* DMA デスティネーション・アドレス・レジスタ2 */
#define DMAC_DMATCR2    0x0ce8  /* DMA トランスファ・カウント・レジスタ2 */
#define DMAC_CHCR2      0x0cec  /* DMA チャネル・コントロール・レジスタ2 */

/* ＤＭＡＣチャネル３ */

#define DMAC_SAR3       0x0cf0  /* DMA ソース・アドレス・レジスタ3 */
#define DMAC_DAR3       0x0cf4  /* DMA デスティネーション・アドレス・レジスタ3 */
#define DMAC_DMATCR3    0x0cf8  /* DMA トランスファ・カウント・レジスタ3 */
#define DMAC_CHCR3      0x0cfc  /* DMA チャネル・コントロール・レジスタ3 */

/* 割込みコントローラ */

#define INTC_IPRA       0x0d00  /* 割込み優先レベル設定レジスタA */
#define INTC_IPRB       0x0d02  /* 割込み優先レベル設定レジスタB */
#define INTC_IPRC       0x0d04  /* 割込み優先レベル設定レジスタC */
#define INTC_IPRD       0x0d06  /* 割込み優先レベル設定レジスタD */
#define INTC_IPRE       0x0d08  /* 割込み優先レベル設定レジスタE */
#define INTC_IPRF       0x0d0a  /* 割込み優先レベル設定レジスタF */
#define INTC_IPRG       0x0d0c  /* 割込み優先レベル設定レジスタG */
#define INTC_IPRH       0x0d0e  /* 割込み優先レベル設定レジスタH */
#define INTC_IPRI       0x0d10  /* 割込み優先レベル設定レジスタI */
#define INTC_IPRJ       0x0d12  /* 割込み優先レベル設定レジスタJ */
#define INTC_IPRK       0x0d14  /* 割込み優先レベル設定レジスタK */
#define INTC_IPRL       0x0d16  /* 割込み優先レベル設定レジスタL */
#define INTC_ICR        0x0d18  /* 割込みコントロールレジスタ */
#define INTC_ISR        0x0d1a  /* IRQ ステータスレジスタ */

/* ＳＣＩチャネル０ */

#define SCI_SMR0        0x1000  /* シリアル・モード・レジスタ */
#define SCI_BRR0        0x1001  /* ビットレート・レジスタ */
#define SCI_SCR0        0x1002  /* シリアル・コントロール・レジスタ */
#define SCI_TDR0        0x1003  /* トランスミット・データ・レジスタ */
#define SCI_SSR0        0x1004  /* シリアル・ステータス・レジスタ */
#define SCI_RDR0        0x1005  /* レシーブ・データ・レジスタ */
#define SCI_SDCR0       0x1006  /* シリアルディレクションコントロールレジスタ */

/* ＳＣＩチャネル１ */

#define SCI_SMR1        0x1008  /* シリアル・モード・レジスタ */
#define SCI_BRR1        0x1009  /* ビットレート・レジスタ */
#define SCI_SCR1        0x100a  /* シリアル・コントロール・レジスタ */
#define SCI_TDR1        0x100b  /* トランスミット・データ・レジスタ */
#define SCI_SSR1        0x100c  /* シリアル・ステータス・レジスタ */
#define SCI_RDR1        0x100d  /* レシーブ・データ・レジスタ */
#define SCI_SDCR1       0x100e  /* シリアルディレクションコントロールレジスタ */

/* ＳＣＩチャネル２ */

#define SCI_SMR2        0x1010  /* シリアル・モード・レジスタ */
#define SCI_BRR2        0x1011  /* ビットレート・レジスタ */
#define SCI_SCR2        0x1012  /* シリアル・コントロール・レジスタ */
#define SCI_TDR2        0x1013  /* トランスミット・データ・レジスタ */
#define SCI_SSR2        0x1014  /* シリアル・ステータス・レジスタ */
#define SCI_RDR2        0x1015  /* レシーブ・データ・レジスタ */
#define SCI_SDCR2       0x1016  /* シリアルディレクションコントロールレジスタ */

/* ＳＣＩチャネル３ */

#define SCI_SMR3        0x1018  /* シリアル・モード・レジスタ */
#define SCI_BRR3        0x1019  /* ビットレート・レジスタ */
#define SCI_SCR3        0x101a  /* シリアル・コントロール・レジスタ */
#define SCI_TDR3        0x101b  /* トランスミット・データ・レジスタ */
#define SCI_SSR3        0x101c  /* シリアル・ステータス・レジスタ */
#define SCI_RDR3        0x101d  /* レシーブ・データ・レジスタ */
#define SCI_SDCR3       0x101e  /* シリアルディレクションコントロールレジスタ */

/* ＳＣＩチャネル４ */

#define SCI_SMR4        0x1020  /* シリアル・モード・レジスタ */
#define SCI_BRR4        0x1021  /* ビットレート・レジスタ */
#define SCI_SCR4        0x1022  /* シリアル・コントロール・レジスタ */
#define SCI_TDR4        0x1023  /* トランスミット・データ・レジスタ */
#define SCI_SSR4        0x1024  /* シリアル・ステータス・レジスタ */
#define SCI_RDR4        0x1025  /* レシーブ・データ・レジスタ */
#define SCI_SDCR4       0x1026  /* シリアルディレクションコントロールレジスタ */

/* ＡＴＵ－Ⅱ共通 */

#define ATU_TSTR1       0x1401  /* タイマ・スタート・レジスタ1 */
#define ATU_TSTR2       0x1400  /* タイマ・スタート・レジスタ2 */
#define ATU_TSTR3       0x1402  /* タイマ・スタート・レジスタ3 */
#define ATU_PSCR1       0x1404  /* プリスケーラ・レジスタ1 */
#define ATU_PSCR2       0x1406  /* プリスケーラ・レジスタ2 */
#define ATU_PSCR3       0x1408  /* プリスケーラ・レジスタ3 */
#define ATU_PSCR4       0x140a  /* プリスケーラ・レジスタ4 */

/* ＡＴＵ－Ⅱチャネル０ */

#define ATU_TCNT0H      0x1430  /* フリーランニング・レジスタH */
#define ATU_TCNT0L      0x1430  /* フリーランニング・レジスタL */
#define ATU_ICR0AH      0x1434  /* インプットキャプチャ・レジスタAH */
#define ATU_ICR0AL      0x1434  /* インプットキャプチャ・レジスタAL */
#define ATU_ICR0BH      0x1438  /* インプットキャプチャ・レジスタBH */
#define ATU_ICR0BL      0x1438  /* インプットキャプチャ・レジスタBL */
#define ATU_ICR0CH      0x143c  /* インプットキャプチャ・レジスタCH */
#define ATU_ICR0CL      0x143c  /* インプットキャプチャ・レジスタCL */
#define ATU_ICR0DH      0x1420  /* インプットキャプチャ・レジスタDH */
#define ATU_ICR0DL      0x1420  /* インプットキャプチャ・レジスタDL */
#define ATU_ITVRR1      0x1424  /* タイマインターバルインタラプトリクエスト・レジスタ1 */
#define ATU_ITVRR2A     0x1426  /* タイマインターバルインタラプトリクエスト・レジスタ2A */
#define ATU_ITVRR2B     0x1428  /* タイマインターバルインタラプトリクエスト・レジスタ2B */
#define ATU_TIOR0       0x142a  /* タイマI/Oコントロール・レジスタ */
#define ATU_TSR0        0x142c  /* タイマステータス・レジスタ */
#define ATU_TIER0       0x142e  /* タイマインタラプトイネーブル・レジスタ */

/* ＡＴＵ－Ⅱチャネル１ */

#define ATU_TCNT1A      0x1440  /* フリーランニング・レジスタA */
#define ATU_TCNT1B      0x1442  /* フリーランニング・レジスタB */
#define ATU_GR1A        0x1444  /* ジェネラル・レジスタA */
#define ATU_GR1B        0x1446  /* ジェネラル・レジスタB */
#define ATU_GR1C        0x1448  /* ジェネラル・レジスタC */
#define ATU_GR1D        0x144a  /* ジェネラル・レジスタD */
#define ATU_GR1E        0x144c  /* ジェネラル・レジスタE */
#define ATU_GR1F        0x144e  /* ジェネラル・レジスタF */
#define ATU_GR1G        0x1450  /* ジェネラル・レジスタG */
#define ATU_GR1H        0x1452  /* ジェネラル・レジスタH */
#define ATU_OCR1        0x1454  /* アウトプットコンペア・レジスタ */
#define ATU_OSBR1       0x1456  /* オフセットベース・レジスタ */
#define ATU_TIOR1A      0x1459  /* タイマI/Oコントロール・レジスタA */
#define ATU_TIOR1B      0x1458  /* タイマI/Oコントロール・レジスタB */
#define ATU_TIOR1C      0x145b  /* タイマI/Oコントロール・レジスタC */
#define ATU_TIOR1D      0x145a  /* タイマI/Oコントロール・レジスタD */
#define ATU_TCR1B       0x145c  /* タイマコントロール・レジスタB */
#define ATU_TSR1A       0x145e  /* タイマステータス・レジスタA */
#define ATU_TSR1B       0x1460  /* タイマステータス・レジスタB */
#define ATU_TIER1A      0x1462  /* タイマインタラプトイネーブル・レジスタA */
#define ATU_TIER1B      0x1464  /* タイマインタラプトイネーブル・レジスタB */
#define ATU_TRGMDR      0x1466  /* トリガモード・レジスタ */

/* ＡＴＵ－Ⅱチャネル２ */

#define ATU_TCNT2A      0x1600  /* フリーランニング・レジスタA */
#define ATU_TCNT2B      0x1602  /* フリーランニング・レジスタB */
#define ATU_GR2A        0x1604  /* ジェネラル・レジスタA */
#define ATU_GR2B        0x1606  /* ジェネラル・レジスタB */
#define ATU_GR2C        0x1608  /* ジェネラル・レジスタC */
#define ATU_GR2D        0x160a  /* ジェネラル・レジスタD */
#define ATU_GR2E        0x160c  /* ジェネラル・レジスタE */
#define ATU_GR2F        0x160e  /* ジェネラル・レジスタF */
#define ATU_GR2G        0x1610  /* ジェネラル・レジスタG */
#define ATU_GR2H        0x1612  /* ジェネラル・レジスタH */
#define ATU_OCR2A       0x1614  /* アウトプットコンペア・レジスタA */
#define ATU_OCR2B       0x1616  /* アウトプットコンペア・レジスタB */
#define ATU_OCR2C       0x1618  /* アウトプットコンペア・レジスタC */
#define ATU_OCR2D       0x161a  /* アウトプットコンペア・レジスタD */
#define ATU_OCR2E       0x161c  /* アウトプットコンペア・レジスタE */
#define ATU_OCR2F       0x161e  /* アウトプットコンペア・レジスタF */
#define ATU_OCR2G       0x1620  /* アウトプットコンペア・レジスタG */
#define ATU_OCR2H       0x1622  /* アウトプットコンペア・レジスタH */
#define ATU_OSBR2       0x1624  /* オフセットベース・レジスタ */
#define ATU_TIOR2A      0x1627  /* タイマI/Oコントロール・レジスタA */
#define ATU_TIOR2B      0x1626  /* タイマI/Oコントロール・レジスタB */
#define ATU_TIOR2C      0x1629  /* タイマI/Oコントロール・レジスタC */
#define ATU_TIOR2D      0x1628  /* タイマI/Oコントロール・レジスタD */
#define ATU_TCR2A       0x162b  /* タイマコントロール・レジスタA */
#define ATU_TCR2B       0x162a  /* タイマコントロール・レジスタB */
#define ATU_TSR2A       0x162c  /* タイマステータス・レジスタA */
#define ATU_TSR2B       0x162e  /* タイマステータス・レジスタB */
#define ATU_TIER2A      0x1630  /* タイマインタラプトイネーブル・レジスタA */
#define ATU_TIER2B      0x1632  /* タイマインタラプトイネーブル・レジスタB */

/* ＡＴＵ－Ⅱチャネル３～５共通 */

#define ATU_TSR3        0x1480  /* タイマステータス・レジスタ3 */
#define ATU_TIER3       0x1482  /* タイマインタラプトイネーブル・レジスタ3 */
#define ATU_TMDR3       0x1484  /* タイマモード・レジスタ */

/* ＡＴＵ－Ⅱチャネル３ */

#define ATU_TCNT3       0x14a0  /* フリーランニング・カウンタ */
#define ATU_GR3A        0x14a2  /* ジェネラル・レジスタA */
#define ATU_GR3B        0x14a4  /* ジェネラル・レジスタB */
#define ATU_GR3C        0x14a6  /* ジェネラル・レジスタC */
#define ATU_GR3D        0x14a8  /* ジェネラル・レジスタD */
#define ATU_TIOR3A      0x14ab  /* タイマI/Oコントロール・レジスタA */
#define ATU_TIOR3B      0x14aa  /* タイマI/Oコントロール・レジスタB */
#define ATU_TCR3        0x14ac  /* タイマコントロール・レジスタ */

/* ＡＴＵ－Ⅱチャネル４ */

#define ATU_TCNT4       0x14c0  /* フリーランニング・カウンタ */
#define ATU_GR4A        0x14c2  /* ジェネラル・レジスタA */
#define ATU_GR4B        0x14c4  /* ジェネラル・レジスタB */
#define ATU_GR4C        0x14c6  /* ジェネラル・レジスタC */
#define ATU_GR4D        0x14c8  /* ジェネラル・レジスタD */
#define ATU_TIOR4A      0x14cb  /* タイマI/Oコントロール・レジスタA */
#define ATU_TIOR4B      0x14ca  /* タイマI/Oコントロール・レジスタB */
#define ATU_TCR4        0x14cc  /* タイマコントロール・レジスタ */

/* ＡＴＵ－Ⅱチャネル５ */

#define ATU_TCNT5       0x14e0  /* フリーランニング・カウンタ */
#define ATU_GR5A        0x14e2  /* ジェネラル・レジスタA */
#define ATU_GR5B        0x14e4  /* ジェネラル・レジスタB */
#define ATU_GR5C        0x14e6  /* ジェネラル・レジスタC */
#define ATU_GR5D        0x14e8  /* ジェネラル・レジスタD */
#define ATU_TIOR5A      0x14eb  /* タイマI/Oコントロール・レジスタA */
#define ATU_TIOR5B      0x14ea  /* タイマI/Oコントロール・レジスタB */
#define ATU_TCR5        0x14ec  /* タイマコントロール・レジスタ */

/* ＡＴＵ－Ⅱチャネル６ */

#define ATU_TCNT6A      0x1500  /* フリーランニング・レジスタA */
#define ATU_TCNT6B      0x1502  /* フリーランニング・レジスタB */
#define ATU_TCNT6C      0x1504  /* フリーランニング・レジスタC */
#define ATU_TCNT6D      0x1506  /* フリーランニング・レジスタD */
#define ATU_CYLR6A      0x1508  /* サイクル・レジスタA */
#define ATU_CYLR6B      0x150a  /* サイクル・レジスタB */
#define ATU_CYLR6C      0x150c  /* サイクル・レジスタC */
#define ATU_CYLR6D      0x150e  /* サイクル・レジスタD */
#define ATU_BFR6A       0x1510  /* バッファ・レジスタA */
#define ATU_BFR6B       0x1512  /* バッファ・レジスタB */
#define ATU_BFR6C       0x1514  /* バッファ・レジスタC */
#define ATU_BFR6D       0x1516  /* バッファ・レジスタD */
#define ATU_DTR6A       0x1518  /* デューティ・レジスタA */
#define ATU_DTR6B       0x151a  /* デューティ・レジスタB */
#define ATU_DTR6C       0x151c  /* デューティ・レジスタC */
#define ATU_DTR6D       0x151e  /* デューティ・レジスタD */
#define ATU_TCR6A       0x1521  /* タイマコントロール・レジスタA */
#define ATU_TCR6B       0x1520  /* タイマコントロール・レジスタB */
#define ATU_TSR6        0x1522  /* タイマステータス・レジスタ */
#define ATU_TIER6       0x1524  /* タイマインタラプトイネーブル・レジスタ */
#define ATU_PWMD        0x1526  /* PWM モード・レジスタ */

/* ＡＴＵ－Ⅱチャネル７ */

#define ATU_TCNT7A      0x1580  /* フリーランニング・レジスタA */
#define ATU_TCNT7B      0x1582  /* フリーランニング・レジスタB */
#define ATU_TCNT7C      0x1584  /* フリーランニング・レジスタC */
#define ATU_TCNT7D      0x1586  /* フリーランニング・レジスタD */
#define ATU_CYLR7A      0x1588  /* サイクル・レジスタA */
#define ATU_CYLR7B      0x158a  /* サイクル・レジスタB */
#define ATU_CYLR7C      0x158c  /* サイクル・レジスタC */
#define ATU_CYLR7D      0x158e  /* サイクル・レジスタD */
#define ATU_BFR7A       0x1590  /* バッファ・レジスタA */
#define ATU_BFR7B       0x1592  /* バッファ・レジスタB */
#define ATU_BFR7C       0x1594  /* バッファ・レジスタC */
#define ATU_BFR7D       0x1596  /* バッファ・レジスタD */
#define ATU_DTR7A       0x1598  /* デューティ・レジスタA */
#define ATU_DTR7B       0x159a  /* デューティ・レジスタB */
#define ATU_DTR7C       0x159c  /* デューティ・レジスタC */
#define ATU_DTR7D       0x159e  /* デューティ・レジスタD */
#define ATU_TCR7A       0x15a1  /* タイマコントロール・レジスタA */
#define ATU_TCR7B       0x15a0  /* タイマコントロール・レジスタB */
#define ATU_TSR7        0x15a2  /* タイマステータス・レジスタ */
#define ATU_TIER7       0x15a4  /* タイマインタラプトイネーブル・レジスタ */

/* ＡＴＵ－Ⅱチャネル８ */

#define ATU_DCNT8A      0x1640  /* ダウンカウンタA */
#define ATU_DCNT8B      0x1642  /* ダウンカウンタB */
#define ATU_DCNT8C      0x1644  /* ダウンカウンタC */
#define ATU_DCNT8D      0x1646  /* ダウンカウンタD */
#define ATU_DCNT8E      0x1648  /* ダウンカウンタE */
#define ATU_DCNT8F      0x164a  /* ダウンカウンタF */
#define ATU_DCNT8G      0x164c  /* ダウンカウンタG */
#define ATU_DCNT8H      0x164e  /* ダウンカウンタH */
#define ATU_DCNT8I      0x1650  /* ダウンカウンタI */
#define ATU_DCNT8J      0x1652  /* ダウンカウンタJ */
#define ATU_DCNT8K      0x1654  /* ダウンカウンタK */
#define ATU_DCNT8L      0x1656  /* ダウンカウンタL */
#define ATU_DCNT8M      0x1658  /* ダウンカウンタM */
#define ATU_DCNT8N      0x165a  /* ダウンカウンタN */
#define ATU_DCNT8O      0x165c  /* ダウンカウンタO */
#define ATU_DCNT8P      0x165e  /* ダウンカウンタP */
#define ATU_RLDR8       0x1660  /* リロード・レジスタ */
#define ATU_TCNR        0x1662  /* タイマコネクション・レジスタ */
#define ATU_OTR         0x1664  /* ワンショットパルスターミネート・レジスタ */
#define ATU_DSTR        0x1666  /* ダウンカウントスタート・レジスタ */
#define ATU_TCR8        0x1668  /* タイマコントロール・レジスタ */
#define ATU_TSR8        0x166a  /* タイマステータス・レジスタ */
#define ATU_TIER8       0x166c  /* タイマインタラプトイネーブル・レジスタ */
#define ATU_RLDENR      0x166e  /* リロードイネーブル・レジスタ */

/* ＡＴＵ－Ⅱチャネル９ */

#define ATU_ECNT9A      0x1680  /* イネーブルカウンタA */
#define ATU_ECNT9B      0x1682  /* イネーブルカウンタB */
#define ATU_ECNT9C      0x1684  /* イネーブルカウンタC */
#define ATU_ECNT9D      0x1686  /* イネーブルカウンタD */
#define ATU_ECNT9E      0x1688  /* イネーブルカウンタE */
#define ATU_ECNT9F      0x168a  /* イネーブルカウンタF */
#define ATU_GR9A        0x168c  /* ジェネラル・レジスタA */
#define ATU_GR9B        0x168e  /* ジェネラル・レジスタB */
#define ATU_GR9C        0x1690  /* ジェネラル・レジスタC */
#define ATU_GR9D        0x1692  /* ジェネラル・レジスタD */
#define ATU_GR9E        0x1694  /* ジェネラル・レジスタE */
#define ATU_GR9F        0x1696  /* ジェネラル・レジスタF */
#define ATU_TCR9A       0x1698  /* タイマコントロール・レジスタA */
#define ATU_TCR9B       0x169a  /* タイマコントロール・レジスタB */
#define ATU_TCR9C       0x169c  /* タイマコントロール・レジスタC */
#define ATU_TSR9        0x169e  /* タイマステータス・レジスタ */
#define ATU_TIER9       0x16a0  /* タイマインタラプトイネーブル・レジスタ */

/* ＡＴＵ－Ⅱチャネル１０ */

#define ATU_TCNT10AH    0x16c0  /* フリーランニング・レジスタAH */
#define ATU_TCNT10AL    0x16c0  /* フリーランニング・レジスタAL */
#define ATU_TCNT10B     0x16c4  /* イベントカウンタB */
#define ATU_TCNT10C     0x16c6  /* リロードカウンタC */
#define ATU_TCNT10D     0x16c8  /* 補正カウンタD */
#define ATU_TCNT10E     0x16ca  /* 補正カウンタE */
#define ATU_TCNT10F     0x16cc  /* 補正カウンタF */
#define ATU_TCNT10G     0x16ce  /* フリーランニング・レジスタG */
#define ATU_ICR10AH     0x16d0  /* インプットキャプチャ・レジスタAH */
#define ATU_ICR10AL     0x16d0  /* インプットキャプチャ・レジスタAL */
#define ATU_OCR10AH     0x16d4  /* アウトプットコンペア・レジスタAH */
#define ATU_OCR10AL     0x16d4  /* アウトプットコンペア・レジスタAL */
#define ATU_OCR10B      0x16d8  /* アウトプットコンペア・レジスタB */
#define ATU_RLD10C      0x16da  /* リロード・レジスタC */
#define ATU_GR10G       0x16dc  /* ジェネラル・レジスタG */
#define ATU_TCNT10H     0x16de  /* ノイズキャンセラ・カウンタH */
#define ATU_NCR10       0x16e0  /* ノイズキャンセラ・レジスタ */
#define ATU_TIOR10      0x16e2  /* タイマI/Oコントロール・レジスタ */
#define ATU_TCR10       0x16e4  /* タイマコントロール・レジスタ */
#define ATU_TCCLR10     0x16e6  /* 補正カウンタクリア・レジスタ */
#define ATU_TSR10       0x16e8  /* タイマステータス・レジスタ */
#define ATU_TIER10      0x16ea  /* タイマインタラプトイネーブル・レジスタ */

/* ＡＴＵ－Ⅱチャネル１１ */

#define ATU_TCNT11      0x15c0  /* フリーランニング・レジスタ */
#define ATU_GR11A       0x15c2  /* ジェネラル・レジスタA */
#define ATU_GR11B       0x15c4  /* ジェネラル・レジスタB */
#define ATU_TIOR11      0x15c6  /* タイマI/Oコントロール・レジスタ */
#define ATU_TCR11       0x15c8  /* タイマコントロール・レジスタ */
#define ATU_TSR11       0x15ca  /* タイマステータス・レジスタ */
#define ATU_TIER11      0x15cc  /* タイマインタラプトイネーブル・レジスタ */

/* パルスコントローラ */

#define APC_POPCR       0x1700  /* パルスアウトプットポートコントロール・レジスタ */

/* 省電力モード制御 */

#define SYS_SBYCR       0x0c14  /* スタンバイコントロール・レジスタ */
#define SYS_SYSCR       0x1708  /* システムコントロール・レジスタ */
#define SYS_MSTCR       0x170a  /* モジュールスタンバイコントロール・レジスタ(WR) */
#define SYS_MSTCR_R     0x170b  /* モジュールスタンバイコントロール・レジスタ(RD) */

/* コンペアマッチタイマ共通 */

#define CMT_CMSTR       0x1710  /* コンペアマッチタイマスタート・レジスタ */

/* コンペアマッチタイマ０ */

#define CMT_CMCSR0      0x1712  /* コンペアマッチタイマコントロール/ステータス・レジスタ */
#define CMT_CMCNT0      0x1714  /* コンペアマッチタイマ・カウンタ */
#define CMT_CMCOR0      0x1716  /* コンペアマッチタイマコンスタント・レジスタ */

/* コンペアマッチタイマ１ */

#define CMT_CMCSR1      0x1718  /* コンペアマッチタイマコントロール/ステータス・レジスタ */
#define CMT_CMCNT1      0x171a  /* コンペアマッチタイマ・カウンタ */
#define CMT_CMCOR1      0x171c  /* コンペアマッチタイマコンスタント・レジスタ */

/* Ａ／Ｄ */

#define AD_ADDR0H       0x1800  /* A/D データ・レジスタ0  H（Bit9～2）*/
#define AD_ADDR0L       0x1801  /* A/D データ・レジスタ0  L（Bit1～0）*/
#define AD_ADDR1H       0x1802  /* A/D データ・レジスタ1  H（Bit9～2）*/
#define AD_ADDR1L       0x1803  /* A/D データ・レジスタ1  L（Bit1～0）*/
#define AD_ADDR2H       0x1804  /* A/D データ・レジスタ2  H（Bit9～2）*/
#define AD_ADDR2L       0x1805  /* A/D データ・レジスタ2  L（Bit1～0）*/
#define AD_ADDR3H       0x1806  /* A/D データ・レジスタ3  H（Bit9～2）*/
#define AD_ADDR3L       0x1807  /* A/D データ・レジスタ3  L（Bit1～0）*/
#define AD_ADDR4H       0x1808  /* A/D データ・レジスタ4  H（Bit9～2）*/
#define AD_ADDR4L       0x1809  /* A/D データ・レジスタ4  L（Bit1～0）*/
#define AD_ADDR5H       0x180a  /* A/D データ・レジスタ5  H（Bit9～2）*/
#define AD_ADDR5L       0x180b  /* A/D データ・レジスタ5  L（Bit1～0）*/
#define AD_ADDR6H       0x180c  /* A/D データ・レジスタ6  H（Bit9～2）*/
#define AD_ADDR6L       0x180d  /* A/D データ・レジスタ6  L（Bit1～0）*/
#define AD_ADDR7H       0x180e  /* A/D データ・レジスタ7  H（Bit9～2）*/
#define AD_ADDR7L       0x180f  /* A/D データ・レジスタ7  L（Bit1～0）*/
#define AD_ADDR8H       0x1810  /* A/D データ・レジスタ8  H（Bit9～2）*/
#define AD_ADDR8L       0x1811  /* A/D データ・レジスタ8  L（Bit1～0）*/
#define AD_ADDR9H       0x1812  /* A/D データ・レジスタ9  H（Bit9～2）*/
#define AD_ADDR9L       0x1813  /* A/D データ・レジスタ9  L（Bit1～0）*/
#define AD_ADDR10H      0x1814  /* A/D データ・レジスタ10 H（Bit9～2）*/
#define AD_ADDR10L      0x1815  /* A/D データ・レジスタ10 L（Bit1～0）*/
#define AD_ADDR11H      0x1816  /* A/D データ・レジスタ11 H（Bit9～2）*/
#define AD_ADDR11L      0x1817  /* A/D データ・レジスタ11 L（Bit1～0）*/
#define AD_ADDR12H      0x1820  /* A/D データ・レジスタ12 H（Bit9～2）*/
#define AD_ADDR12L      0x1821  /* A/D データ・レジスタ12 L（Bit1～0）*/
#define AD_ADDR13H      0x1822  /* A/D データ・レジスタ13 H（Bit9～2）*/
#define AD_ADDR13L      0x1823  /* A/D データ・レジスタ13 L（Bit1～0）*/
#define AD_ADDR14H      0x1824  /* A/D データ・レジスタ14 H（Bit9～2）*/
#define AD_ADDR14L      0x1825  /* A/D データ・レジスタ14 L（Bit1～0）*/
#define AD_ADDR15H      0x1826  /* A/D データ・レジスタ15 H（Bit9～2）*/
#define AD_ADDR15L      0x1827  /* A/D データ・レジスタ15 L（Bit1～0）*/
#define AD_ADDR16H      0x1828  /* A/D データ・レジスタ16 H（Bit9～2）*/
#define AD_ADDR16L      0x1829  /* A/D データ・レジスタ16 L（Bit1～0）*/
#define AD_ADDR17H      0x182a  /* A/D データ・レジスタ17 H（Bit9～2）*/
#define AD_ADDR17L      0x182b  /* A/D データ・レジスタ17 L（Bit1～0）*/
#define AD_ADDR18H      0x182c  /* A/D データ・レジスタ18 H（Bit9～2）*/
#define AD_ADDR18L      0x182d  /* A/D データ・レジスタ18 L（Bit1～0）*/
#define AD_ADDR19H      0x182e  /* A/D データ・レジスタ19 H（Bit9～2）*/
#define AD_ADDR19L      0x182f  /* A/D データ・レジスタ19 L（Bit1～0）*/
#define AD_ADDR20H      0x1830  /* A/D データ・レジスタ20 H（Bit9～2）*/
#define AD_ADDR20L      0x1831  /* A/D データ・レジスタ20 L（Bit1～0）*/
#define AD_ADDR21H      0x1832  /* A/D データ・レジスタ21 H（Bit9～2）*/
#define AD_ADDR21L      0x1833  /* A/D データ・レジスタ21 L（Bit1～0）*/
#define AD_ADDR22H      0x1834  /* A/D データ・レジスタ22 H（Bit9～2）*/
#define AD_ADDR22L      0x1835  /* A/D データ・レジスタ22 L（Bit1～0）*/
#define AD_ADDR23H      0x1836  /* A/D データ・レジスタ23 H（Bit9～2）*/
#define AD_ADDR23L      0x1837  /* A/D データ・レジスタ23 L（Bit1～0）*/
#define AD_ADDR24H      0x1840  /* A/D データ・レジスタ24 H（Bit9～2）*/
#define AD_ADDR24L      0x1841  /* A/D データ・レジスタ24 L（Bit1～0）*/
#define AD_ADDR25H      0x1842  /* A/D データ・レジスタ25 H（Bit9～2）*/
#define AD_ADDR25L      0x1843  /* A/D データ・レジスタ25 L（Bit1～0）*/
#define AD_ADDR26H      0x1844  /* A/D データ・レジスタ26 H（Bit9～2）*/
#define AD_ADDR26L      0x1845  /* A/D データ・レジスタ26 L（Bit1～0）*/
#define AD_ADDR27H      0x1846  /* A/D データ・レジスタ27 H（Bit9～2）*/
#define AD_ADDR27L      0x1847  /* A/D データ・レジスタ27 L（Bit1～0）*/
#define AD_ADDR28H      0x1848  /* A/D データ・レジスタ28 H（Bit9～2）*/
#define AD_ADDR28L      0x1849  /* A/D データ・レジスタ28 L（Bit1～0）*/
#define AD_ADDR29H      0x184a  /* A/D データ・レジスタ29 H（Bit9～2）*/
#define AD_ADDR29L      0x184b  /* A/D データ・レジスタ29 L（Bit1～0）*/
#define AD_ADDR30H      0x184c  /* A/D データ・レジスタ30 H（Bit9～2）*/
#define AD_ADDR30L      0x184d  /* A/D データ・レジスタ30 L（Bit1～0）*/
#define AD_ADDR31H      0x184e  /* A/D データ・レジスタ31 H（Bit9～2）*/
#define AD_ADDR31L      0x184f  /* A/D データ・レジスタ31 L（Bit1～0）*/
#define AD_ADCSR0       0x1818  /* A/D コントロール／ステータス・レジスタ0 */
#define AD_ADCR0        0x1819  /* A/D コントロール・レジスタ0 */
#define AD_ADTRGR0      0x176e  /* A/D トリガ・レジスタ0 */
#define AD_ADCSR1       0x1838  /* A/D コントロール／ステータス・レジスタ1 */
#define AD_ADCR1        0x1839  /* A/D コントロール・レジスタ1 */
#define AD_ADTRGR1      0x172e  /* A/D トリガ・レジスタ1 */
#define AD_ADCSR2       0x1858  /* A/D コントロール／ステータス・レジスタ2 */
#define AD_ADCR2        0x1859  /* A/D コントロール・レジスタ2 */
#define AD_ADTRGR2      0x172f  /* A/D トリガ・レジスタ2 */

/* ポートＡ～Ｌ */

#define PA_DR           0x1726  /* ポートA データ・レジスタ */
#define PB_DR           0x1738  /* ポートB データ・レジスタ */
#define PC_DR           0x173e  /* ポートC データ・レジスタ */
#define PD_DR           0x1746  /* ポートD データ・レジスタ */
#define PE_DR           0x1754  /* ポートE データ・レジスタ */
#define PF_DR           0x174e  /* ポートF データ・レジスタ */
#define PG_DR           0x1764  /* ポートG データ・レジスタ */
#define PH_DR           0x172c  /* ポートH データ・レジスタ */
#define PJ_DR           0x176c  /* ポートJ データ・レジスタ */
#define PK_DR           0x1778  /* ポートK データ・レジスタ */
#define PL_DR           0x175e  /* ポートL データ・レジスタ */

/* ピン・ファンクション・コントローラ */

#define PFC_PAIOR       0x1720  /* ポートA ＩＯレジスタ */
#define PFC_PACRH       0x1722  /* ポートA コントロール・レジスタH */
#define PFC_PACRL       0x1724  /* ポートA コントロール・レジスタL */
#define PFC_PBIOR       0x1730  /* ポートB ＩＯレジスタ */
#define PFC_PBCRH       0x1732  /* ポートB コントロール・レジスタH */
#define PFC_PBCRL       0x1734  /* ポートB コントロール・レジスタL */
#define PFC_PBIR        0x1736  /* ポートB インバート・レジスタ */
#define PFC_PCIOR       0x173a  /* ポートC ＩＯレジスタ */
#define PFC_PCCR        0x173c  /* ポートC コントロール・レジスタ */
#define PFC_PDIOR       0x1740  /* ポートD ＩＯレジスタ */
#define PFC_PDCRH       0x1742  /* ポートD コントロール・レジスタH */
#define PFC_PDCRL       0x1744  /* ポートD コントロール・レジスタL */
#define PFC_PEIOR       0x1750  /* ポートE ＩＯレジスタ */
#define PFC_PECR        0x1752  /* ポートE コントロール・レジスタ */
#define PFC_PFIOR       0x1748  /* ポートF ＩＯレジスタ */
#define PFC_PFCRH       0x174a  /* ポートF コントロール・レジスタH */
#define PFC_PFCRL       0x174c  /* ポートF コントロール・レジスタL */
#define PFC_PGIOR       0x1760  /* ポートG ＩＯレジスタ */
#define PFC_PGCR        0x1762  /* ポートG コントロール・レジスタ */
#define PFC_PHIOR       0x1728  /* ポートH ＩＯレジスタ */
#define PFC_PHCR        0x172a  /* ポートH コントロール・レジスタ */
#define PFC_PJIOR       0x1766  /* ポートJ ＩＯレジスタ */
#define PFC_PJCRH       0x1768  /* ポートJ コントロール・レジスタH */
#define PFC_PJCRL       0x176a  /* ポートJ コントロール・レジスタL */
#define PFC_PKIOR       0x1770  /* ポートK ＩＯレジスタ */
#define PFC_PKCRH       0x1772  /* ポートK コントロール・レジスタH */
#define PFC_PKCRL       0x1774  /* ポートK コントロール・レジスタL */
#define PFC_PKIR        0x1776  /* ポートK インバート・レジスタ */
#define PFC_PLIOR       0x1756  /* ポートL ＩＯレジスタ */
#define PFC_PLCRH       0x1758  /* ポートL コントロール・レジスタH */
#define PFC_PLCRL       0x175a  /* ポートL コントロール・レジスタL */
#define PFC_PLIR        0x175c  /* ポートL インバート・レジスタ */

/* ユーザブレーク・コントローラ */

#define UBC_UBARH       0x0c00  /* ブレーク・アドレス・レジスタＨ */
#define UBC_UBARL       0x0c02  /* ブレーク・アドレス・レジスタＬ */
#define UBC_UBAMRH      0x0c04  /* ブレーク・アドレス・マスク・レジスタＨ */
#define UBC_UBAMRL      0x0c06  /* ブレーク・アドレス・マスク・レジスタＬ */
#define UBC_UBBR        0x0c08  /* ブレーク・バスサイクル・レジスタ */
#define UBC_UBCR        0x0c0a  /* ブレーク・コントロール・レジスタ */

/* フラッシュメモリ */

#define FLASH_FLMCR1    0x0800  /* フラッシュメモリコントロール・レジスタ1 */
#define FLASH_FLMCR2    0x0801  /* フラッシュメモリコントロール・レジスタ2 */
#define FLASH_EBR1      0x0802  /* ブロック指定・レジスタ1 */
#define FLASH_EBR2      0x0803  /* ブロック指定・レジスタ2 */

/* 割込みベクタ番号 */

#define INT_NMI         11      /* NMI */
#define INT_UB          12      /* ユーザブレーク */
#define INT_UDI         14      /* H-UDI ブレーク */
#define INT_IRQ0        64      /* 外部端子 IRQ0 */
#define INT_IRQ1        65      /* 外部端子 IRQ1 */
#define INT_IRQ2        66      /* 外部端子 IRQ2 */
#define INT_IRQ3        67      /* 外部端子 IRQ3 */
#define INT_IRQ4        68      /* 外部端子 IRQ4 */
#define INT_IRQ5        69      /* 外部端子 IRQ5 */
#define INT_IRQ6        70      /* 外部端子 IRQ6 */
#define INT_IRQ7        71      /* 外部端子 IRQ7 */
#define INT_DEI0        72      /* DMAC チャネル0 */
#define INT_DEI1        74      /* DMAC チャネル1 */
#define INT_DEI2        76      /* DMAC チャネル2 */
#define INT_DEI3        78      /* DMAC チャネル3 */
#define INT_ITV1        80      /* ATU-II チャネル0 INTV1 */
#define INT_ITV2A       81      /* ATU-II チャネル0 INTV2A */
#define INT_ITV2A       81      /* ATU-II チャネル0 INTV2B */
#define INT_ICI0A       84      /* ATU-II チャネル0 ICI0A */
#define INT_ICI0B       86      /* ATU-II チャネル0 ICI0B */
#define INT_ICI0C       88      /* ATU-II チャネル0 ICI0C */
#define INT_ICI0D       90      /* ATU-II チャネル0 ICI0D */
#define INT_OVI0        92      /* ATU-II チャネル0 OVI0 */
#define INT_IMI1A       96      /* ATU-II チャネル1 IMI1A */
#define INT_CMI1        96      /* ATU-II チャネル1 CMI1 */
#define INT_IMI1B       97      /* ATU-II チャネル1 IMI1B */
#define INT_IMI1C       98      /* ATU-II チャネル1 IMI1C */
#define INT_IMI1D       99      /* ATU-II チャネル1 IMI1D */
#define INT_IMI1E       100     /* ATU-II チャネル1 IMI1E */
#define INT_IMI1F       101     /* ATU-II チャネル1 IMI1F */
#define INT_IMI1G       102     /* ATU-II チャネル1 IMI1G */
#define INT_IMI1H       103     /* ATU-II チャネル1 IMI1H */
#define INT_OVI1A       104     /* ATU-II チャネル1 OVI1A */
#define INT_OVI1B       104     /* ATU-II チャネル1 OVI1B */
#define INT_IMI2A       108     /* ATU-II チャネル2 IMI1A */
#define INT_CMI2A       108     /* ATU-II チャネル2 CMI1A */
#define INT_IMI2B       109     /* ATU-II チャネル2 IMI1B */
#define INT_CMI2B       109     /* ATU-II チャネル2 CMI1B */
#define INT_IMI2C       110     /* ATU-II チャネル2 IMI1C */
#define INT_CMI2C       110     /* ATU-II チャネル2 CMI1C */
#define INT_IMI2D       111     /* ATU-II チャネル2 IMI1D */
#define INT_CMI2D       111     /* ATU-II チャネル2 CMI1D */
#define INT_IMI2E       112     /* ATU-II チャネル2 IMI1E */
#define INT_CMI2E       112     /* ATU-II チャネル2 CMI1E */
#define INT_IMI2F       113     /* ATU-II チャネル2 IMI1F */
#define INT_CMI2F       113     /* ATU-II チャネル2 CMI1F */
#define INT_IMI2G       114     /* ATU-II チャネル2 IMI1G */
#define INT_CMI2G       114     /* ATU-II チャネル2 CMI1G */
#define INT_IMI2H       115     /* ATU-II チャネル2 IMI1H */
#define INT_CMI2H       115     /* ATU-II チャネル2 CMI1H */
#define INT_OVI2A       116     /* ATU-II チャネル2 OVI1A */
#define INT_OVI2B       116     /* ATU-II チャネル2 OVI1B */
#define INT_IMI3A       120     /* ATU-II チャネル3 IMI3A */
#define INT_IMI3B       121     /* ATU-II チャネル3 IMI3B */
#define INT_IMI3C       122     /* ATU-II チャネル3 IMI3C */
#define INT_IMI3D       123     /* ATU-II チャネル3 IMI3D */
#define INT_OVI3        124     /* ATU-II チャネル3 OVI3 */
#define INT_IMI4A       128     /* ATU-II チャネル4 IMI4A */
#define INT_IMI4B       129     /* ATU-II チャネル4 IMI4B */
#define INT_IMI4C       130     /* ATU-II チャネル4 IMI4C */
#define INT_IMI4D       131     /* ATU-II チャネル4 IMI4D */
#define INT_OVI4        132     /* ATU-II チャネル4 OVI4 */
#define INT_IMI5A       136     /* ATU-II チャネル5 IMI5A */
#define INT_IMI5B       137     /* ATU-II チャネル5 IMI5B */
#define INT_IMI5C       138     /* ATU-II チャネル5 IMI5C */
#define INT_IMI5D       139     /* ATU-II チャネル5 IMI5D */
#define INT_OVI5        140     /* ATU-II チャネル5 OVI5 */
#define INT_CMI6A       144     /* ATU-II チャネル6 CMI6A */
#define INT_CMI6B       145     /* ATU-II チャネル6 CMI6B */
#define INT_CMI6C       146     /* ATU-II チャネル6 CMI6C */
#define INT_CMI6D       147     /* ATU-II チャネル6 CMI6D */
#define INT_CMI7A       148     /* ATU-II チャネル7 CMI7A */
#define INT_CMI7B       149     /* ATU-II チャネル7 CMI7B */
#define INT_CMI7C       150     /* ATU-II チャネル7 CMI7C */
#define INT_CMI7D       151     /* ATU-II チャネル7 CMI7D */
#define INT_OSI8A       152     /* ATU-II チャネル8 OSI8A */
#define INT_OSI8B       153     /* ATU-II チャネル8 OSI8B */
#define INT_OSI8C       154     /* ATU-II チャネル8 OSI8C */
#define INT_OSI8D       155     /* ATU-II チャネル8 OSI8D */
#define INT_OSI8E       156     /* ATU-II チャネル8 OSI8E */
#define INT_OSI8F       157     /* ATU-II チャネル8 OSI8F */
#define INT_OSI8G       158     /* ATU-II チャネル8 OSI8G */
#define INT_OSI8H       159     /* ATU-II チャネル8 OSI8H */
#define INT_OSI8I       160     /* ATU-II チャネル8 OSI8I */
#define INT_OSI8J       161     /* ATU-II チャネル8 OSI8J */
#define INT_OSI8K       162     /* ATU-II チャネル8 OSI8K */
#define INT_OSI8L       163     /* ATU-II チャネル8 OSI8L */
#define INT_OSI8M       164     /* ATU-II チャネル8 OSI8M */
#define INT_OSI8N       165     /* ATU-II チャネル8 OSI8N */
#define INT_OSI8O       166     /* ATU-II チャネル8 OSI8O */
#define INT_OSI8P       167     /* ATU-II チャネル8 OSI8P */
#define INT_CMI9A       168     /* ATU-II チャネル9 CMI9A */
#define INT_CMI9B       169     /* ATU-II チャネル9 CMI9B */
#define INT_CMI9C       170     /* ATU-II チャネル9 CMI9C */
#define INT_CMI9D       171     /* ATU-II チャネル9 CMI9D */
#define INT_CMI9E       172     /* ATU-II チャネル9 CMI9E */
#define INT_CMI9F       174     /* ATU-II チャネル9 CMI9F */
#define INT_CMI10A      176     /* ATU-II チャネル10 CMI10A */
#define INT_CMI10B      178     /* ATU-II チャネル10 CMI10B */
#define INT_ICI10A      180     /* ATU-II チャネル10 ICI10A */
#define INT_CMI10G      180     /* ATU-II チャネル10 CMI10G */
#define INT_IMI11A      184     /* ATU-II チャネル11 IMI11A */
#define INT_IMI11B      186     /* ATU-II チャネル11 IMI11B */
#define INT_OVI11       187     /* ATU-II チャネル11 OVI11 */
#define INT_CMTI0       188     /* CMT チャネル0 CMTI0 */
#define INT_ADI0        190     /* A/D チャネル0 ADI0 */
#define INT_CMTI1       192     /* CMT チャネル1 CMTI1 */
#define INT_ADI1        194     /* A/D チャネル1 ADI1 */
#define INT_ADI2        196     /* A/D チャネル2 ADI2 */
#define INT_ERI0        200     /* SCI チャネル0 ERI0 */
#define INT_RXI0        201     /* SCI チャネル0 RXI0 */
#define INT_TXI0        202     /* SCI チャネル0 TXI0 */
#define INT_TEI0        203     /* SCI チャネル0 TEI0 */
#define INT_ERI1        204     /* SCI チャネル1 ERI1 */
#define INT_RXI1        205     /* SCI チャネル1 RXI1 */
#define INT_TXI1        206     /* SCI チャネル1 TXI1 */
#define INT_TEI1        207     /* SCI チャネル1 TEI1 */
#define INT_ERI2        208     /* SCI チャネル2 ERI2 */
#define INT_RXI2        209     /* SCI チャネル2 RXI2 */
#define INT_TXI2        210     /* SCI チャネル2 TXI2 */
#define INT_TEI2        211     /* SCI チャネル2 TEI2 */
#define INT_ERI3        212     /* SCI チャネル3 ERI3 */
#define INT_RXI3        213     /* SCI チャネル3 RXI3 */
#define INT_TXI3        214     /* SCI チャネル3 TXI3 */
#define INT_TEI3        215     /* SCI チャネル3 TEI3 */
#define INT_ERI4        216     /* SCI チャネル4 ERI4 */
#define INT_RXI4        217     /* SCI チャネル4 RXI4 */
#define INT_TXI4        218     /* SCI チャネル4 TXI4 */
#define INT_TEI4        219     /* SCI チャネル4 TEI4 */
#define INT_ERS0        220     /* HCAN チャネル0 ERS0 */
#define INT_OVR0        221     /* HCAN チャネル0 OVR0 */
#define INT_RM0         222     /* HCAN チャネル0 RM0 */
#define INT_SLE0        223     /* HCAN チャネル0 SLE0 */
#define INT_ITI         224     /* WDT ITI */
#define INT_ERS1        228     /* HCAN チャネル1 ERS1 */
#define INT_OVR1        229     /* HCAN チャネル1 OVR1 */
#define INT_RM1         230     /* HCAN チャネル1 RM1 */
#define INT_SLE1        231     /* HCAN チャネル1 SLE1 */

#endif /* SH7055_H */
