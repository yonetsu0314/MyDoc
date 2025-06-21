/*----------------------------------------------------------------------*
 * File name	:	sh7615.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/sh7615/RCS/sh7615.h 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sh7615.h $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/14 17:00
 * 前リビジョンと同様に#define sfr_clrw(n,c)
 * を変更する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/13 15:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくすため
 * #define sfr_clr(n,c) を変更する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＳＨ２用インクルード
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

/******************************************************************************
* ＳＨ７６１５ 内部Ｉ／Ｏ定義                                                 *
*                                                                             *
*  File name : sh7615.h                                                       *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 11/Apr/2000 作成                                                      MiSPO *
******************************************************************************/

#ifndef SH7615_H
#define SH7615_H

/* 内部Ｉ／Ｏのベースアドレス */

#ifdef CPU_SH
#define IO_BASE ((volatile unsigned char *)0xfffff000L)
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
void sfr_iset(unsigned n, UB c);    /* バイトデータの即値ビットセット*/
void sfr_iclr(unsigned n, UB c);    /* バイトデータの即値ビットクリア*/

#ifdef IO_BASE
#define sfr_in(n)       (*(IO_BASE+(n)))
#define sfr_out(n,c)    (*(IO_BASE+(n))=(c))
#define sfr_inw(n)      (*(volatile UH *)(IO_BASE+(n)))
#define sfr_outw(n,c)   (*(volatile UH *)(IO_BASE+(n))=(c))
#define sfr_inl(n)      (*(volatile UW *)(IO_BASE+(n)))
#define sfr_outl(n,c)   (*(volatile UW *)(IO_BASE+(n))=(c))
#define sfr_set(n,c)    (*(IO_BASE+(n))|=(c))
#define sfr_clr(n,c)    (*(IO_BASE+(n))&=(UB)~(c))
#define sfr_setw(n,c)   (*(volatile UH *)(IO_BASE+(n))|=(c))
#define sfr_clrw(n,c)   (*(volatile UH *)(IO_BASE+(n))&=(UH)~(c))
#define sfr_setl(n,c)   (*(volatile UW *)(IO_BASE+(n))|=(c))
#define sfr_clrl(n,c)   (*(volatile UW *)(IO_BASE+(n))&=~(c))

#if defined(__GNUC__)
#elif defined(__SH7000)
#else                   /* Hitachi C */
#define sfr_iset(n,c)   do{set_gbr(IO_BASE+(n));gbr_or_byte(0,c);}while(0)
#define sfr_iclr(n,c)   do{set_gbr(IO_BASE+(n));gbr_and_byte(0,~(c));}while(0)
#endif
#endif

/* シリアルＩ／Ｏ(SIO) チャネル0 */

#define SIO_SIRDR0    0xc00   /* レシーブデータレジスタ0 */
#define SIO_SITDR0    0xc02   /* トランスミットデータレジスタ0 */
#define SIO_SICTR0    0xc04   /* シリアルコントロールレジスタ0 */
#define SIO_SISTR0    0xc06   /* シリアルステータスレジスタ0 */

/* シリアルＩ／Ｏ(SIO) チャネル1 */

#define SIO_SIRDR1    0xc10   /* レシーブデータレジスタ1 */
#define SIO_SITDR1    0xc12   /* トランスミットデータレジスタ1 */
#define SIO_SICTR1    0xc14   /* シリアルコントロールレジスタ1 */
#define SIO_SISTR1    0xc16   /* シリアルステータスレジスタ1 */

/* シリアルＩ／Ｏ(SIO) チャネル2 */

#define SIO_SIRDR2    0xc20   /* レシーブデータレジスタ2 */
#define SIO_SITDR2    0xc22   /* トランスミットデータレジスタ2 */
#define SIO_SICTR2    0xc24   /* シリアルコントロールレジスタ2 */
#define SIO_SISTR2    0xc26   /* シリアルステータスレジスタ2 */

/* 16ビットタイマパルスユニット(TPU) 共通 */

#define TPU_TSTR      0xc40   /* タイマスタートレジスタ */
#define TPU_TSYR      0xc41   /* タイマシンクロレジスタ */

/* 16ビットタイマパルスユニット(TPU) チャネル0 */

#define TPU_TCR0      0xc50   /* タイマコントロールレジスタ0 */
#define TPU_TMDR0     0xc51   /* タイマモードレジスタ0 */
#define TPU_TIOR0H    0xc52   /* タイマ I/O コントロールレジスタ0H */
#define TPU_TIOR0L    0xc53   /* タイマ I/O コントロールレジスタ0L */
#define TPU_TIER0     0xc54   /* タイマインタラプトイネーブルレジスタ0 */
#define TPU_TSR0      0xc55   /* タイマステータスレジスタ0 */
#define TPU_TCNT0     0xc56   /* タイマカウンタ0 */
#define TPU_TGR0A     0xc58   /* タイマジェネラルレジスタ0A */
#define TPU_TGR0B     0xc5a   /* タイマジェネラルレジスタ0B */
#define TPU_TGR0C     0xc5c   /* タイマジェネラルレジスタ0C */
#define TPU_TGR0D     0xc5e   /* タイマジェネラルレジスタ0D */

/* 16ビットタイマパルスユニット(TPU) チャネル1 */

#define TPU_TCR1      0xc60   /* タイマコントロールレジスタ1 */
#define TPU_TMDR1     0xc61   /* タイマモードレジスタ1 */
#define TPU_TIOR1     0xc62   /* タイマ I/O コントロールレジスタ1 */
#define TPU_TIER1     0xc64   /* タイマインタラプトイネーブルレジスタ1 */
#define TPU_TSR1      0xc65   /* タイマステータスレジスタ1 */
#define TPU_TCNT1     0xc66   /* タイマカウンタ1 */
#define TPU_TGR1A     0xc68   /* タイマジェネラルレジスタ1A */
#define TPU_TGR1B     0xc6a   /* タイマジェネラルレジスタ1B */

/* 16ビットタイマパルスユニット(TPU) チャネル2 */

#define TPU_TCR2      0xc70   /* タイマコントロールレジスタ2 */
#define TPU_TMDR2     0xc71   /* タイマモードレジスタ2 */
#define TPU_TIOR2     0xc72   /* タイマ I/O コントロールレジスタ2 */
#define TPU_TIER2     0xc74   /* タイマインタラプトイネーブルレジスタ2 */
#define TPU_TSR2      0xc75   /* タイマステータスレジスタ2 */
#define TPU_TCNT2     0xc76   /* タイマカウンタ2 */
#define TPU_TGR2A     0xc78   /* タイマジェネラルレジスタ2A */
#define TPU_TGR2B     0xc7a   /* タイマジェネラルレジスタ2B */

/* I/Oポート */

#define IO_PADR       0xc84   /* ポートA データレジスタ */
#define IO_PBDR       0xc8c   /* ポートB データレジスタ */

/* ピンファンクションコントローラ(PFC) */

#define PFC_PACR      0xc80   /* ポートA コントロールレジスタ */
#define PFC_PAIOR     0xc82   /* ポートA IOレジスタ */
#define PFC_PBCR      0xc88   /* ポートB コントロールレジスタ */
#define PFC_PBIOR     0xc8a   /* ポートB IOレジスタ */
#define PFC_PBCR2     0xc8e   /* ポートB コントロールレジスタ2 */

/* 日立ユーザデバッグインターフェイス(H-DUI) */

#define HDUI_SDIR     0xcb0   /* インストラクションレジスタ */
#define HDUI_SDSR     0xcb2   /* ステータスレジスタ */
#define HDUI_SDDRH    0xcb4   /* データレジスタ H */
#define HDUI_SDDRL    0xcb6   /* データレジスタ L */

/* FIFO付きシリアルコミュニケーションインターフェース(SCIF) チャネル1 */

#define SCIF_SCSMR1   0xcc0   /* シリアルモードレジスタ1 */
#define SCIF_SCBRR1   0xcc2   /* ビットレートレジスタ1 */
#define SCIF_SCSCR1   0xcc4   /* シリアルコントロールレジスタ1 */
#define SCIF_SCFTDR1  0xcc6   /* トランスミットFIFOデータレジスタ1 */
#define SCIF_SC1SSR1  0xcc8   /* シリアルステータス1レジスタ1 */
#define SCIF_SC2SSR1  0xcca   /* シリアルステータス2レジスタ1 */
#define SCIF_SCFRDR1  0xccc   /* レシーブFIFOデータレジスタ1 */
#define SCIF_SCFCR1   0xcce   /* FIFOコントロールレジスタ1 */
#define SCIF_SCFDR1   0xcd0   /* FIFOデータ数レジスタ1 */
#define SCIF_SCFER1   0xcd2   /* FIFOエラーレジスタ1 */
#define SCIF_SCIMR1   0xcd4   /* IrDAモードレジスタ1 */

/* FIFO付きシリアルコミュニケーションインターフェース(SCIF) チャネル2 */

#define SCIF_SCSMR2   0xce0   /* シリアルモードレジスタ2 */
#define SCIF_SCBRR2   0xce2   /* ビットレートレジスタ2 */
#define SCIF_SCSCR2   0xce4   /* シリアルコントロールレジスタ2 */
#define SCIF_SCFTDR2  0xce6   /* トランスミットFIFOデータレジスタ2 */
#define SCIF_SC1SSR2  0xce8   /* シリアルステータス1レジスタ2 */
#define SCIF_SC2SSR2  0xcea   /* シリアルステータス2レジスタ2 */
#define SCIF_SCFRDR2  0xcec   /* レシーブFIFOデータレジスタ2 */
#define SCIF_SCFCR2   0xcee   /* FIFOコントロールレジスタ2 */
#define SCIF_SCFDR2   0xcf0   /* FIFOデータ数レジスタ2 */
#define SCIF_SCFER2   0xcf2   /* FIFOエラーレジスタ2 */
#define SCIF_SCIMR2   0xcf4   /* IrDAモードレジスタ2 */

/* イーサネットコントローラ用ダイレクトメモリアクセスコントローラ(E-DMAC) */

#define EDMAC_EDMR    0xd00   /* E-DMACモードレジスタ */
#define EDMAC_EDTRR   0xd04   /* E-DMAC送信要求レジスタ */
#define EDMAC_EDRRR   0xd08   /* E-DMAC受信要求レジスタ */
#define EDMAC_TDLAR   0xd0c   /* 送信ディスクリプタ先頭アドレスレジスタ */
#define EDMAC_RDLAR   0xd10   /* 受信ディスクリプタ先頭アドレスレジスタ */
#define EDMAC_EESR    0xd14   /* EtherC/E-DMACステータスレジスタ */
#define EDMAC_EESIPR  0xd18   /* EtherC/E-DMACステータス割り込み許可レジスタ */
#define EDMAC_TRSCER  0xd1c   /* 送受信ステータスコピー許可レジスタ */
#define EDMAC_RMFCR   0xd20   /* ミスドフレームカウンタレジスタ */
#define EDMAC_TFTR    0xd24   /* 送信FIFOしきい値指定レジスタ */
#define EDMAC_FDR     0xd28   /* FIFO容量設定レジスタ */
#define EDMAC_RCR     0xd2c   /* 受信方式制御レジスタ */
#define EDMAC_EDOCR   0xd30   /* E-DMAC動作制御レジスタ */

/* イーサネットコントローラ(EtherC) */

#define ETHERC_ECMR   0xd60   /* EtherCモードレジスタ */
#define ETHERC_ECSR   0xd64   /* EtherCステータスレジスタ */
#define ETHERC_ECSIPR 0xd68   /* EtherCステータス割り込み許可レジスタ */
#define ETHERC_PIR    0xd6c   /* PHYインターフェイスレジスタ */
#define ETHERC_MAHR   0xd70   /* MACアドレス上位設定レジスタ */
#define ETHERC_MALR   0xd74   /* MACアドレス下位設定レジスタ */
#define ETHERC_RFLR   0xd78   /* 受信フレーム長上限レジスタ */
#define ETHERC_PSR    0xd7c   /* PHYステータスレジスタ */
#define ETHERC_TROCR  0xd80   /* 送信リトライオーバーカウンタレジスタ */
#define ETHERC_CDCR   0xd84   /* 衝突検出カウンタレジスタ */
#define ETHERC_LCCR   0xd88   /* キャリア消失カウンタレジスタ */
#define ETHERC_CNDCR  0xd8c   /* キャリア未検出カウンタレジスタ */
#define ETHERC_IFLCR  0xd90   /* フレーム長異常カウンタレジスタ */
#define ETHERC_CEFCR  0xd94   /* CRCエラーフレーム受信カウンタレジスタ */
#define ETHERC_FRECR  0xd98   /* フレーム受信エラーカウンタレジスタ */
#define ETHERC_TSFRCR 0xd9c   /* 64バイト未満フレーム受信カウンタレジスタ */
#define ETHERC_TLFRCR 0xda0   /* 指定バイト超フレーム受信カウンタレジスタ */
#define ETHERC_RFCR   0xda4   /* 端数ビットフレーム受信カウンタレジスタ */
#define ETHERC_MAFCR  0xda8   /* マルチキャストアドレスフレーム受信カウンタレジスタ */

/* 16ビットフリーランニングタイマ(FRT) */

#define FRT_TIER      0xe10   /* タイマインタラプトイネーブルレジスタ */
#define FRT_FTCSR     0xe11   /* タイマコントロール／ステータスレジスタ */
#define FRT_FRC       0xe12   /* フリーランニングカウンタ */
#define FRT_FRCH      0xe12   /* 同 H（ﾋﾞｯﾄ15～ 8）*/
#define FRT_FRCL      0xe13   /* 同 L（ﾋﾞｯﾄ 7～ 0）*/
#define FRT_OCRAB     0xe14   /* アウトプットコンペアレジスタＡ/Ｂ */
#define FRT_OCRABH    0xe14   /* 同 H（ﾋﾞｯﾄ15～ 8） */
#define FRT_OCRABL    0xe15   /* 同 L（ﾋﾞｯﾄ 7～ 0）*/
#define FRT_TCR       0xe16   /* タイマコントロールレジスタ */
#define FRT_TOCR      0xe17   /* アウトプットコンペアコントロールレジスタ */
#define FRT_FICR      0xe18   /* インプットキャプチャレジスタ */
#define FRT_FICRH     0xe18   /* 同 H（ﾋﾞｯﾄ15～ 8）*/
#define FRT_FICRL     0xe19   /* 同 L（ﾋﾞｯﾄ 7～ 0）*/

/* 割込みコントローラ */

#define INTC_IPRA     0xee2   /* 割込み優先レベル設定レジスタA */
#define INTC_IPRB     0xe60   /* 割込み優先レベル設定レジスタB */
#define INTC_IPRC     0xee6   /* 割込み優先レベル設定レジスタC */
#define INTC_IPRD     0xe40   /* 割込み優先レベル設定レジスタD */
#define INTC_IPRE     0xec0   /* 割込み優先レベル設定レジスタE */

#define INTC_VCRA     0xe62   /* ベクタ番号設定レジスタA */
#define INTC_VCRB     0xe64   /* ベクタ番号設定レジスタB */
#define INTC_VCRC     0xe66   /* ベクタ番号設定レジスタC */
#define INTC_VCRD     0xe68   /* ベクタ番号設定レジスタD */
#define INTC_VCRE     0xe42   /* ベクタ番号設定レジスタE */
#define INTC_VCRF     0xe44   /* ベクタ番号設定レジスタF */
#define INTC_VCRG     0xe46   /* ベクタ番号設定レジスタG */
#define INTC_VCRH     0xe48   /* ベクタ番号設定レジスタH */
#define INTC_VCRI     0xe4a   /* ベクタ番号設定レジスタI */
#define INTC_VCRJ     0xe4c   /* ベクタ番号設定レジスタJ */
#define INTC_VCRK     0xe4e   /* ベクタ番号設定レジスタK */
#define INTC_VCRL     0xe50   /* ベクタ番号設定レジスタL */
#define INTC_VCRM     0xe52   /* ベクタ番号設定レジスタM */
#define INTC_VCRN     0xe54   /* ベクタ番号設定レジスタN */
#define INTC_VCRO     0xe56   /* ベクタ番号設定レジスタO */
#define INTC_VCRP     0xec2   /* ベクタ番号設定レジスタP */
#define INTC_VCRQ     0xec4   /* ベクタ番号設定レジスタQ */
#define INTC_VCRR     0xec6   /* ベクタ番号設定レジスタR */
#define INTC_VCRS     0xec8   /* ベクタ番号設定レジスタS */
#define INTC_VCRT     0xeca   /* ベクタ番号設定レジスタT */
#define INTC_VCRU     0xecc   /* ベクタ番号設定レジスタU */
#define INTC_VCRWDT   0xee4   /* ベクタ番号設定レジスタ WDT */
#define INTC_VCRDMA0  0xfa0   /* ベクタ番号設定レジスタ DMAC0 */
#define INTC_VCRDMA1  0xfa8   /* ベクタ番号設定レジスタ DMAC1 */
#define INTC_ICR      0xee0   /* 割込みコントロールレジスタ */
#define INTC_IRQCSR   0xee8   /* IRQコントロール/ステｰタスレジスタ */

/* ウォッチドッグタイマ(WDT) */

#define WDT_WTCSR     0xe80   /* タイマコントロール／ステータスレジスタ */
#define WDT_WTCNT     0xe80   /* タイマカウンタ（ﾗｲﾄ）*/
#define WDT_WTCNT_R   0xe81   /* タイマカウンタ（ﾘｰﾄﾞ）*/
#define WDT_RSTCSR    0xe82   /* リセｯトコントロｰル/ステｰタスレジスタ（ﾗｲﾄ）*/
#define WDT_RSTCSR_R  0xe83   /* リセｯトコントロｰル/ステｰタスレジスタ（ﾘｰﾄﾞ）*/

/* ユーザブレークコントローラ(UCB) */

#define UBC_BARAH     0xf00   /* ブレークアドレスレジスタAH */
#define UBC_BARAL     0xf02   /* ブレークアドレスレジスタAL */
#define UBC_BAMRAH    0xf04   /* ブレークアドレスマスクレジスタAH */
#define UBC_BAMRAL    0xf06   /* ブレークアドレスマスクレジスタAL */
#define UBC_BBRA      0xf08   /* ブレークバスサイクルレジスタA */
#define UBC_BARBH     0xf20   /* ブレークアドレスレジスタBH */
#define UBC_BARBL     0xf22   /* ブレークアドレスレジスタBL */
#define UBC_BAMRBH    0xf24   /* ブレークアドレスマスクレジスタBH */
#define UBC_BAMRBL    0xf26   /* ブレークアドレスマスクレジスタBL */
#define UBC_BBRB      0xf28   /* ブレークバスサイクルレジスタB */
#define UBC_BARCH     0xf40   /* ブレークアドレスレジスタCH */
#define UBC_BARCL     0xf42   /* ブレークアドレスレジスタCL */
#define UBC_BAMRCH    0xf44   /* ブレークアドレスマスクレジスタCH */
#define UBC_BAMRCL    0xf46   /* ブレークアドレスマスクレジスタCL */
#define UBC_BDRCH     0xf50   /* ブレークデータレジスタCH */
#define UBC_BDRCL     0xf52   /* ブレークデータレジスタCL */
#define UBC_BDMRCH    0xf54   /* ブレークデータマスクレジスタCH */
#define UBC_BDMRCL    0xf56   /* ブレークデータマスクレジスタCL */
#define UBC_BBRC      0xf48   /* ブレークバスサイクルレジスタC */
#define UBC_BETRC     0xf58   /* 実行回数ブレークレジスタC */
#define UBC_BARDH     0xf60   /* ブレークアドレスレジスタDH */
#define UBC_BARDL     0xf62   /* ブレークアドレスレジスタDL */
#define UBC_BAMRDH    0xf64   /* ブレークアドレスマスクレジスタDH */
#define UBC_BAMRDL    0xf66   /* ブレークアドレスマスクレジスタDL */
#define UBC_BDRDH     0xf70   /* ブレークデータレジスタDH */
#define UBC_BDRDL     0xf72   /* ブレークデータレジスタDL */
#define UBC_BDMRDH    0xf74   /* ブレークデータマスクレジスタDH */
#define UBC_BDMRDL    0xf76   /* ブレークデータマスクレジスタDL */
#define UBC_BBRD      0xf68   /* ブレークバスサイクルレジスタD */
#define UBC_BETRD     0xf78   /* 実行回数ブレークレジスタD */
#define UBC_BRCRH     0xf30   /* ブレークコントロールレジスタH */
#define UBC_BRCRL     0xf32   /* ブレークコントロールレジスタL */
#define UBC_BRFR      0xf10   /* ブランチフラグレジスタ */
#define UBC_BRSRH     0xf14   /* ブランチソースレジスタH */
#define UBC_BRSRL     0xf16   /* ブランチソースレジスタL */
#define UBC_BRDRH     0xf18   /* ブランチデスティネーションレジスタH */
#define UBC_BRDRL     0xf1a   /* ブランチデスティネーションレジスタL */


/* ダイレクトメモリアクセスコントローラ(DMAC) 共通 */

#define DMAC_DMAOR    0xfb0   /* DMA オペレーションレジスタ */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル0 */

#define DMAC_SAR0     0xf80   /* DMA ソースアドレスレジスタ0 */
#define DMAC_DAR0     0xf84   /* DMA デスティネーションアドレスレジスタ0 */
#define DMAC_TCR0     0xf88   /* DMA トランスファカウントレジスタ0 */
#define DMAC_CHCR0    0xf8c   /* DMA チャネルコントロールレジスタ0 */
#define DMAC_VCRDMA0  0xfa0   /* ベクタ番号設定レジスタ DMAC0 */
#define DMAC_DRCR0    0xe71   /* DMA 要求/応答選択制御レジスタ0 */

/* ダイレクトメモリアクセスコントローラ(DMAC) チャネル1 */

#define DMAC_SAR1     0xf90   /* DMA ソースアドレスレジスタ1 */
#define DMAC_DAR1     0xf94   /* DMA デスティネーションアドレスレジスタ1 */
#define DMAC_TCR1     0xf98   /* DMA トランスファカウントレジスタ1 */
#define DMAC_CHCR1    0xf9c   /* DMA チャネルコントロールレジスタ1 */
#define DMAC_VCRDMA1  0xfa8   /* ベクタ番号設定レジスタ DMAC1 */
#define DMAC_DRCR1    0xe72   /* DMA 要求/応答選択制御レジスタ1 */

/* バスコントローラ(BSC) */

#define BSC_BCR1      0xfe0   /* バスコントロールレジスタ1 */
#define BSC_BCR2      0xfe4   /* バスコントロールレジスタ2 */
#define BSC_BCR3      0xffc   /* バスコントロールレジスタ3 */
#define BSC_WCR1      0xfe8   /* ウェイトコントロールレジスタ1 */
#define BSC_WCR2      0xfc0   /* ウェイトコントロールレジスタ2 */
#define BSC_WCR3      0xfc4   /* ウェイトコントロールレジスタ3 */
#define BSC_MCR       0xfec   /* 個別メモリコントロールレジスタ */
#define BSC_RTCSR     0xff0   /* リフレｯシｭタイマコントロｰル/ステｰタスレジスタ */
#define BSC_RTCNT     0xff4   /* リフレｯシュタイマカウンタ */
#define BSC_RTCOR     0xff8   /* リフレｯシュタイムコンスタントレジスタ */

/* キャッシュ */

#define SYS_CCR       0xe92   /* キャッシュコントロールレジスタ */

/* 低消費電力 */

#define SYS_SBYCR1    0xe91   /* スタンバイコントロールレジスタ1 */
#define SYS_SBYCR2    0xe93   /* スタンバイコントロールレジスタ2 */

/* 内蔵発振回路 */

#define SYS_FMR       0xe90   /* 周波数変更レジスタ */

/* 割込みベクタ番号 */

#define INT_NMI         11    /* NMI */
#define INT_UB          12    /* ユーザブレーク */
#define INT_IRL1        64    /* IRL1  オートベクタ */
#define INT_IRL2        65    /* IRL2  オートベクタ */
#define INT_IRL3        65    /* IRL3  オートベクタ */
#define INT_IRL4        66    /* IRL4  オートベクタ */
#define INT_IRL5        66    /* IRL5  オートベクタ */
#define INT_IRL6        67    /* IRL6  オートベクタ */
#define INT_IRL7        67    /* IRL7  オートベクタ */
#define INT_IRL8        68    /* IRL8  オートベクタ */
#define INT_IRL9        68    /* IRL9  オートベクタ */
#define INT_IRL10       69    /* IRL10 オートベクタ */
#define INT_IRL11       69    /* IRL11 オートベクタ */
#define INT_IRL12       70    /* IRL12 オートベクタ */
#define INT_IRL13       70    /* IRL13 オートベクタ */
#define INT_IRL14       71    /* IRL14 オートベクタ */
#define INT_IRL15       71    /* IRL15 オートベクタ */

/* 割込みベクタ番号 （★前詰めで定義）*/

#define INT_DEI0        72      /* DMAC チャネル0 */
#define INT_DEI1        73      /* DMAC チャネル1 */
#define INT_ITI         74      /* ウォッチドックタイマ（インターバル･タイマ）*/
#define INT_CMI         75      /* リフレッシュコントローラコンペアマッチ */
#define INT_EINT        76      /* EhterC/E-DMAC割込み */
/*                      77   */ /* reserved */
#define INT_ICI         78      /* FRT インプットキャプチャ */
#define INT_OCI         79      /* FRT アウトプットコンペア */
#define INT_OVI         80      /* FRT タイマオーバフロー */
#define INT_TGI0A       81      /* TPU コンペアマッチ/インプットキャプチャ0A */
#define INT_TGI0B       82      /* TPU コンペアマッチ/インプットキャプチャ0B */
#define INT_TGI0C       83      /* TPU コンペアマッチ/インプットキャプチャ0C */
#define INT_TGI0D       84      /* TPU コンペアマッチ/インプットキャプチャ0D */
#define INT_TCI0V       85      /* TPU チｬネル0 オーバフロー */
#define INT_TGI1A       86      /* TPU コンペアマッチ/インプットキャプチャ1A */
#define INT_TGI1B       87      /* TPU コンペアマッチ/インプットキャプチャ1B */
#define INT_TCI1V       88      /* TPU チｬネル1 オーバフロー */
#define INT_TCI1U       89      /* TPU チｬネル1 アンダフロー */
#define INT_TGI2A       90      /* TPU コンペアマッチ/インプットキャプチャ2A */
#define INT_TGI2B       91      /* TPU コンペアマッチ/インプットキャプチャ2B */
#define INT_TCI2V       92      /* TPU チｬネル2 オーバフロー */
#define INT_TCI2U       93      /* TPU チｬネル2 アンダフロー */
#define INT_ERI1        94      /* SCIF1 受信エラー */
#define INT_RXI1        95      /* SCIF1 受信FIFO full/ready */
#define INT_BRI1        96      /* SCIF1 ブレーク検出 */
#define INT_TXI1        97      /* SCIF1 送信データエンプティ */
#define INT_ERI2        98      /* SCIF2 受信エラー */
#define INT_RXI2        99      /* SCIF2 受信FIFO full/ready */
#define INT_BRI2        100     /* SCIF2 ブレーク検出 */
#define INT_TXI2        101     /* SCIF2 送信データエンプティ */
#define INT_RERI0       102     /* SIO0 受信オーバラン */
#define INT_TERI0       103     /* SIO0 送信オーバラン */
#define INT_RDFI0       104     /* SIO0 受信データフル */
#define INT_TDEI0       105     /* SIO0 送信データエンプティ */
#define INT_RERI1       106     /* SIO1 受信オーバラン */
#define INT_TERI1       107     /* SIO1 送信オーバラン */
#define INT_RDFI1       108     /* SIO1 受信データフル */
#define INT_TDEI1       109     /* SIO1 送信データエンプティ */
#define INT_RERI2       110     /* SIO2 受信オーバラン */
#define INT_TERI2       111     /* SIO2 送信オーバラン */
#define INT_RDFI2       112     /* SIO2 受信データフル */
#define INT_TDEI2       113     /* SIO2 送信データエンプティ */

/* DPS用メモリ */
#define XRAM_START    0x1000E000
#define YRAM_START    0x1001E000

#endif /* SH7615_H */
