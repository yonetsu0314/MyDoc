/*======================================================================
 * File name    : regster.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/regster.c 1.4.29.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: regster.c $
 * リビジョン 1.4.29.3  1970/01/01  00:00:00  chimura
 * 2005/05/11 WCR1のWAITSELビットを１にする。
 * 
 * リビジョン 1.4.29.2  1970/01/01  00:00:00  sakaguti
 * SDRAMをｵｰﾄﾌﾟﾘﾁｬｰｼﾞ･ﾓｰﾄﾞに変更
 * 
 * リビジョン 1.4.29.1  1970/01/01  00:00:00  kawamura
 * 2004/07/29  11:32
 * ＲＯＭに対するアクセスに対して最大限のウェイトを入れる。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2004/01/14 16:00
 * p:/presto/wcu/bios/rcs/regster.c 1.7　と同じにする。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/19 17:00
 * エリア５、６をPCMCIAモードでアクセスする。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/09/05 16:00
 * エリア５のウェイトを８サイクルに変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<sh7706\7706.h>

#define SD_BANK1 (*(volatile short*)0x0c000000)
#define SD_BANK2 (*(volatile short*)0x0c400000)
#define SD_BANK3 (*(volatile short*)0x0c800000)
#define SD_BANK4 (*(volatile short*)0x0cC00000)
#define	SDMR	(*(volatile short*)0xffffe880)

#define	ADCRESET	(*(volatile char *)0xb0020000)

static	unsigned long drd = 0;
void
init_registers(void)
{
	CCR = (unsigned int)0x08;		/* Cache force purge */
	/*　ポート設定　*/
/*---------------------------------------------------
	bit7: CS6
	bit6: CS5
	bit5: CS4
	bit4: CS3
	bit3: CS2
	bit2: WE3
	bit1: WE2
	bit0:(BS ) FPGARST 出力
  ---------------------------------------------------*/
	PFC.PCCR.WORD = (unsigned short)0x0001;
/*---------------------------------------------------
	bit7:(CE2B)   SEL6   出力
	bit6:(CE2A)   SEL5   出力
	bit5:(IOIS16) FPGACS 出力
	bit4: CKE
	bit3:(CASU)   FPGAWR 出力
	bit2: CASL
	bit1:(RASU)   CCLK   出力
	bit0: RASL
  ---------------------------------------------------*/
	PFC.PDCR.WORD = (unsigned short)0x5444;
/*---------------------------------------------------
	bit7:(IRQOUT)  FPGAD7 入出力
	bit6:(TCLK)    FPGAD6 入出力
	bit5:(STATUS1) FPGAD5 入出力
	bit4:(STATUS2) FPGAD4 入出力
	bit3:(DRAK1)   FPGAD3 入出力
	bit2:(DRAK0)   FPGAD2 入出力
	bit1:(DACK1)   FPGAD1 入出力
	bit0:(DACK0)   FPGAD0 入出力
  ---------------------------------------------------*/
	PFC.PECR.WORD = (unsigned short)0xaaaa; /* プルアップ付き入力に設定 */
/*---------------------------------------------------
	bit7: ---
	bit6: DREQ1  未使用
	bit5:(DREQ0) PROGRAM 出力
	bit4:(IRQ4)  INTA    出力
	bit3: IRQ3
	bit2: IRQ2
	bit1: IRQ1
	bit0: IRQ0
  ---------------------------------------------------*/
	PFC.PHCR.WORD = (unsigned short)0x0500;
//	PFC.PHCR.WORD = (unsigned short)0x0555;//for debug
/*---------------------------------------------------
	bit7: ---
	bit6: ---
	bit5: ---
	bit4: ---
	bit3: AN3  未使用
	bit2:(AN2) BUSY 入力
	bit1:(AN1) INIT 入力
	bit0:(AN0) DONE 入力
  ---------------------------------------------------*/
	PFC.PJCR.WORD = (unsigned short)0x00aa;
/*---------------------------------------------------
	bit7: ---
	bit6: ---
	bit5: CTS2
	bit4: RTS2
	bit3:(SCK2) DTR2 出力
	bit2: TXD2/RXD2
	bit1:(SCK0) DSR2 入力
	bit0: TXD0/RXD0
  ---------------------------------------------------*/
	PFC.SCPCR.WORD = (unsigned short)0x0048;

	/*　クロックパルスジェネレータ設定　*/
/*---------------------------------------------------
	ウォッチドッグタイマ　ＯＦＦ
	インターバルタイマモード
	クロック分周　Ｐφ／４０９６
	タイマオーバーフロー　５３．３ｍＳ
  ---------------------------------------------------*/
	CPG.WTCNT.WRITE = (unsigned short)0x5A00;
	CPG.WTCSR.WRITE = (unsigned short)0xA507;
/*---------------------------------------------------
	クロックモード２において
	ＣＫＩＯ＝水晶＊４
	Ｉ：Ｂ：Ｐ＝２：１：１／２より
	Ｉ：Ｂ：Ｐ＝８：４：２　の９．８３０４ＭＨｚ
  ---------------------------------------------------*/
	CPG.FRQCR.WORD = (unsigned short)0x0112;

	/*　バスコントローラ設定　*/
/*---------------------------------------------------
	エリア０：通常メモリ
	エリア２：通常メモリ
	エリア３：ＳＤＲＡＭ
	エリア５：ＰＣＭＣＩＡ
	エリア６：ＰＣＭＣＩＡ
  ---------------------------------------------------*/
	BSC.BCR1.WORD = (unsigned short)0x000B;
/*---------------------------------------------------
	エリア６：８ｂｉｔバス
	エリア５：８ｂｉｔバス
	エリア４：８ｂｉｔバス
	エリア３：３２ｂｉｔバス
	エリア２：１６ｂｉｔバス
  ---------------------------------------------------*/
	BSC.BCR2.WORD = (unsigned short)0x15E0;
/*---------------------------------------------------
	エリア６サイクル間アイドル：３ｃｙｃｌｅ
	エリア５サイクル間アイドル：３ｃｙｃｌｅ
	エリア４サイクル間アイドル：２ｃｙｃｌｅ
	エリア３サイクル間アイドル：２ｃｙｃｌｅ
	エリア２サイクル間アイドル：２ｃｙｃｌｅ
	エリア０サイクル間アイドル：２ｃｙｃｌｅ
	（１ｃｙｃｌｅ＝ＣＫＩＯ（２５ｎＳ））
  ---------------------------------------------------*/
	BSC.WCR1.WORD = (unsigned short)0xAEA2;
//	BSC.WCR1.WORD = (unsigned short)0x3FF3;	//for FULL SLOW
/*---------------------------------------------------
	エリア６ウェイト　：１０ｃｙｃｌｅ
	エリア５ウェイト　：１０ｃｙｃｌｅ
	エリア４ウェイト　：８ｃｙｃｌｅ
	エリア３レイテンシ：２ｃｙｃｌｅ（ウェイト１）
	エリア２ウェイト　：３ｃｙｃｌｅ
	エリア０ウェイト　：８ｃｙｃｌｅ
	（１ｃｙｃｌｅ＝ＣＫＩＯ（２５ｎＳ））
  ---------------------------------------------------*/
	BSC.WCR2.WORD = (unsigned short)0xFF5F;
//	BSC.WCR2.WORD = (unsigned short)0xFDDB;
//	BSC.WCR2.WORD = (unsigned short)0xFFDF;	//for FULL SLOW
/*---------------------------------------------------
	ＲＡＳプリチャージ　　２
	ＲＡＳ－ＣＡＳ遅延　　２
	書込みプリチャージ　　２
	ＣＡＳビフォアＲＡＳ　４
	バンク・アクティブ・モード
	６４Ｍ（１Ｍｘ１６ｂｉｔｘ４ｂａｎｋ）
	通常リフレッシュ実行
  ---------------------------------------------------*/
//	BSC.MCR.WORD = (unsigned short)0x56A4;
	BSC.MCR.WORD = (unsigned short)0x5624;	//オートプリチャージ
/*---------------------------------------------------
	エリア５ウェイト　：１０ｃｙｃｌｅ
	エリア５アサート遅延　：２．５ｃｙｃｌｅ
	エリア５ネゲート遅延　：２．５ｃｙｃｌｅ
	エリア６ウェイト　：１０ｃｙｃｌｅ
	エリア６アサート遅延　：２．５ｃｙｃｌｅ
	エリア６ネゲート遅延　：２．５ｃｙｃｌｅ
  ---------------------------------------------------*/
//	BSC.PCR.WORD = (unsigned short)0x085BB;
	BSC.PCR.WORD = (unsigned short)0x000AA;
/*---------------------------------------------------
	ＳＤＲＡＭ　４０９６／６４ｍＳは、
	Ｐφ／４の７７ｃｙｃｌｅ(P0=19.66MHz)
  ---------------------------------------------------*/
	BSC.RTCNT = (unsigned short)0xA500;
	BSC.RTCOR = (unsigned short)0xA54D;
/*---------------------------------------------------
	リフレッシュ・カウンタ　Ｐφ／４
  ---------------------------------------------------*/
	BSC.RTCSR.WORD = (unsigned short)0x0A508;

#if defined(APL_SECT) && (APL_SECT==0x0010000) 
	SDMR = (volatile short)0x00ff;
#endif
	drd = SD_BANK1;
	drd = SD_BANK2;
	drd = SD_BANK3;
	drd = SD_BANK4;

/*---------------------------------------------------
	bit7: CS6
	bit6: CS5
	bit5: CS4
	bit4: CS3
	bit3: CS2
	bit2: WE3
	bit1: WE2
	bit0:(BS ) FPGARST 出力 = 1
  ---------------------------------------------------*/
	PC.DR.BYTE = (unsigned char)0x01;
//	PC.DR.BYTE = (unsigned char)0x00;//FOR DEBUG
/*---------------------------------------------------
	bit7:(CE2B)   SEL6   出力 = 0
	bit6:(CE2A)   SEL5   出力 = 0
	bit5:(IOIS16) FPGACS 出力 = 1
	bit4: CKE
	bit3:(CASU)   FPGAWR 出力 = 1
	bit2: CASL
	bit1:(RASU)   CCLK   出力 = 0
	bit0: RASL
  ---------------------------------------------------*/
	PD.DR.BYTE = (unsigned char)0x28;
/*---------------------------------------------------
	bit7: ---
	bit6: DREQ1  未使用
	bit5:(DREQ0) PROGRAM 出力 = 1
	bit4:(IRQ4)  INTA    出力 = 1
	bit3: IRQ3
	bit2: IRQ2
	bit1: IRQ1
	bit0: IRQ0
  ---------------------------------------------------*/
	PH.DR.BYTE = (unsigned char)0x30;
//	PH.DR.BYTE = (unsigned char)0x3f;//FOR DEBUG
/*---------------------------------------------------
	bit7: ---
	bit6: ---
	bit5: CTS2
	bit4: RTS2
	bit3:(SCK2) DTR2 出力 = 1
	bit2: TXD2/RXD2
	bit1:(SCK0) DSR2 入力
	bit0: TXD0/RXD0
  ---------------------------------------------------*/
	SCP.DR.BYTE = (unsigned char)0x08;

	ADCRESET = (char)0x1f;			/* ADC Reset Release */

	CCR = (unsigned int)0x01;		/* Cache enable */
}


