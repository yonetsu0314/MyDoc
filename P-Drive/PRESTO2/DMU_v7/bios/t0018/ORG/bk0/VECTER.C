/*======================================================================
 * File name    : supapl.c
 * Original		: p:/dacs_i/rcu/main/rcs/supapl.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		アプリケーションセットアップ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/vecter.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: vecter.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/05/11 20:00
 * 起動用biosからアプリ用biosへの切り替え処理bios_initをベクタテーブル
 * に登録する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_SUPAPL_C_
#include "dmu5562.h"

INTHDR UBrk(void);
INTHDR GnrlIlgl(void);
INTHDR dummy(void);
INTHDR ADDerror(void);
INTHDR Slot(void);
INTHDR DMAerror(void);

INTHDR sci1_errint_hdr(void);
INTHDR sci1_rxint_hdr(void);
INTHDR sci1_txint_hdr(void);

INTHDR sci2_errint_hdr(void);
INTHDR sci2_rxint_hdr(void);
INTHDR sci2_txint_hdr(void);

void	sci1_errint_top(void);
void	sci1_rxint_top(void);
void	sci1_txint_top(void);

void	sci2_errint_top(void);
void	sci2_rxint_top(void);
void	sci2_txint_top(void);

#pragma interrupt(UBrk)
void UBrk(void)
{
	rsstr("*** Break UBrk ***");
}

#pragma interrupt(GnrlIlgl)
void GnrlIlgl(void)
{
	rsstr("*** Break GnrlIlgl ***");
}

#pragma interrupt( dummy )
void dummy( void )
{
	rsstr("dummy ");
}

#pragma interrupt( ADDerror )
void  ADDerror(void)
{	/* CPU address error */
	rsstr(" CPU address error.\n");
}

#pragma interrupt( Slot )
void Slot(void)
{	/* slot illegal instruction */
	rsstr(" Slot illegal instruction.\n");
}

#pragma interrupt( DMAerror )
void DMAerror(void)
{	/* DMA Addess Error */
	rsstr(" DMA Address Error.\n");
}

#pragma interrupt(sci1_errint_hdr)
void sci1_errint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci1_errint_top();
}
#pragma interrupt(sci1_rxint_hdr)
void sci1_rxint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci1_rxint_top();
}
#pragma interrupt(sci1_txint_hdr)
void sci1_txint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci1_txint_top();
}

#pragma interrupt(sci2_errint_hdr)
void sci2_errint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci2_errint_top();
}
#pragma interrupt(sci2_rxint_hdr)
void sci2_rxint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci2_rxint_top();
}
#pragma interrupt(sci2_txint_hdr)
void sci2_txint_hdr(void){		// ハンドラ内では、変数を定義してはいけない
	sci2_txint_top();
}

void
sci1_errint_top(){
	sfr_in(SCIF_SCFRDR1);
	sfr_inw(SCIF_SC1SSR1);
	sfr_out(SCIF_SCSCR1,0x00);
	wait0(1);
	sfr_outw(SCIF_SC1SSR1,0x0060);
	sfr_out(SCIF_SCSCR1,0x70);

}
void
sci1_rxint_top(){
	sfr_in(SCIF_SCFRDR1);
	sfr_clrw(SCIF_SC1SSR1,0x0002);
}

void
sci1_txint_top(){
	sfr_clr(SCIF_SCSCR1,0x80);	/* initialize TIE */
	return;
}

void
sci2_errint_top(){
	sfr_in(SCIF_SCFRDR2);
	sfr_inw(SCIF_SC1SSR2);
	sfr_out(SCIF_SCSCR2,0x00);
	wait0(1);
	sfr_outw(SCIF_SC1SSR2,0x0060);
	sfr_out(SCIF_SCSCR2,0x70);

}
void
sci2_rxint_top(){
	sfr_in(SCIF_SCFRDR2);
	sfr_clrw(SCIF_SC1SSR2,0x0002);
}

void
sci2_txint_top(){
	sfr_clr(SCIF_SCSCR2,0x80);	/* initialize TIE */
	return;
}


void halt0(void){for(;;);}
void halt1(void){for(;;);}
void halt2(void){for(;;);}
void halt3(void){for(;;);}
void halt4(void){for(;;);}
void halt5(void){for(;;);}
void halt6(void){for(;;);}
void halt7(void){for(;;);}
void halt8(void){for(;;);}
void halt9(void){for(;;);}
void halt10(void){for(;;);}
void halt11(void){for(;;);}
void halt12(void){for(;;);}
void halt13(void){for(;;);}
void halt14(void){for(;;);}
void halt15(void){for(;;);}
void halt16(void){for(;;);}
void halt17(void){for(;;);}
void halt18(void){for(;;);}
void halt19(void){for(;;);}
void halt20(void){for(;;);}
void halt21(void){for(;;);}
void halt22(void){for(;;);}
void halt23(void){for(;;);}
void halt24(void){for(;;);}
void halt25(void){for(;;);}
void halt26(void){for(;;);}
void halt27(void){for(;;);}
void halt28(void){for(;;);}
void halt29(void){for(;;);}
void halt30(void){for(;;);}
void halt31(void){for(;;);}
void halt32(void){for(;;);}
void halt33(void){for(;;);}
void halt34(void){for(;;);}
void halt35(void){for(;;);}
void halt36(void){for(;;);}
void halt37(void){for(;;);}
void halt38(void){for(;;);}
void halt39(void){for(;;);}
void halt40(void){for(;;);}
void halt41(void){for(;;);}
void halt42(void){for(;;);}
void halt43(void){for(;;);}
void halt44(void){for(;;);}
void halt45(void){for(;;);}
void halt46(void){for(;;);}
void halt47(void){for(;;);}
void halt48(void){for(;;);}
void halt49(void){for(;;);}
void halt50(void){for(;;);}
void halt51(void){for(;;);}
void halt52(void){for(;;);}
void halt53(void){for(;;);}
void halt54(void){for(;;);}
void halt55(void){for(;;);}
void halt56(void){for(;;);}
void halt57(void){for(;;);}
void halt58(void){for(;;);}
void halt59(void){for(;;);}
void halt60(void){for(;;);}
void halt61(void){for(;;);}
void halt62(void){for(;;);}
void halt63(void){for(;;);}
void halt64(void){for(;;);}
void halt65(void){for(;;);}
void halt66(void){for(;;);}
void halt67(void){for(;;);}
void halt68(void){for(;;);}
void halt69(void){for(;;);}
void halt70(void){for(;;);}
void halt71(void){for(;;);}
void halt72(void){for(;;);}
void halt73(void){for(;;);}
void halt74(void){for(;;);}
void halt75(void){for(;;);}
void halt76(void){for(;;);}
void halt77(void){for(;;);}
void halt78(void){for(;;);}
void halt79(void){for(;;);}
void halt80(void){for(;;);}
void halt81(void){for(;;);}
void halt82(void){for(;;);}
void halt83(void){for(;;);}
void halt84(void){for(;;);}
void halt85(void){for(;;);}
void halt86(void){for(;;);}
void halt87(void){for(;;);}
void halt88(void){for(;;);}
void halt89(void){for(;;);}
void halt90(void){for(;;);}
void halt91(void){for(;;);}
void halt92(void){for(;;);}
void halt93(void){for(;;);}
void halt94(void){for(;;);}
void halt95(void){for(;;);}
void halt96(void){for(;;);}
void halt97(void){for(;;);}
void halt98(void){for(;;);}
void halt99(void){for(;;);}
void halt100(void){for(;;);}
void halt101(void){for(;;);}
void halt102(void){for(;;);}
void halt103(void){for(;;);}
void halt104(void){for(;;);}
void halt105(void){for(;;);}
void halt106(void){for(;;);}
void halt107(void){for(;;);}
void halt108(void){for(;;);}
void halt109(void){for(;;);}
void halt110(void){for(;;);}
void halt111(void){for(;;);}
void halt112(void){for(;;);}
void halt113(void){for(;;);}
void halt114(void){for(;;);}
void halt115(void){for(;;);}
void halt116(void){for(;;);}
void halt117(void){for(;;);}
void halt118(void){for(;;);}
void halt119(void){for(;;);}
void halt120(void){for(;;);}
void halt121(void){for(;;);}
void halt122(void){for(;;);}
void halt123(void){for(;;);}
void halt124(void){for(;;);}
void halt125(void){for(;;);}
void halt126(void){for(;;);}
void halt127(void){for(;;);}
void halt128(void){for(;;);}


/*------------------------------------------------------*/
/*	割り込みハンドラのベクター･テーブル					*/
/*														*/
/*------------------------------------------------------*/
#pragma section VECT
const void (*const vector_table[])(void) = {
	INIT,				/* パワーオンリセット PC 初期値 */
	INIT_STACK,			/* パワーオンリセット SP 初期値 */
	INIT,				/* マニュアルリセット PC 初期値 */
	INIT_STACK,			/* マニュアルリセット SP 初期値 */
	GnrlIlgl,			/* (4)general illigal instruction */
	halt5,				/* (5)reserve for system use */
	Slot,				/* (6)slot illigal instruction */
	halt7,				/* (7)reserve for system use */
	halt8,				/* (8)reserve for system use */
	ADDerror,			/* (9)CPU address error */
	DMAerror,			/* (10)DMA address error */
	UBrk,				/* (11)NMI */
	UBrk,				/* (12)user break */
	halt13,				/* 13 : system reserved  */
	halt14,				/* 14 : system reserved  */
	halt15,				/* 15 : system reserved  */
	halt16,				/* 16 : system reserved  */
	halt17,				/* 17 : system reserved  */
	halt18,				/* 18 : system reserved  */
	halt19,				/* 19 : system reserved  */
	halt20,				/* 20 : system reserved  */
	halt21,				/* 21 : system reserved  */
	halt22,				/* 22 : system reserved  */
	halt23,				/* 23 : system reserved  */
	halt24,				/* 24 : system reserved  */
	halt25,				/* 25 : system reserved  */
	halt26,				/* 26 : system reserved  */
	halt27,				/* 27 : system reserved  */
	halt28,				/* 28 : system reserved  */
	halt29,				/* 29 : system reserved  */
	halt30,				/* 30 : system reserved  */
	halt31,				/* 31 : system reserved  */
	halt32,				/* 32 : trapa 0x20  int 80h(scu_io) */
	tcu_io,				/* ,33 : trapa 0x21  int 81h(tcu_io) */
	halt34,				/* 34 : trapa 0x22  int 82h(mpsc_io) */
	port_io,			/* ,35 : trapa 0x23  int 83h(ppi_io) */
	halt36,				/* 36 : trapa 0x24  int 84h(adc_io) */
	icu_io,				/* ,37 : trapa 0x25  int 85h(icu_io) */
	e2rom_io,			/* ,38 : trapa 0x26  int 86h(e2rom_io) */
	halt39,				/* 39 : trapa 0x27  int 87h(dpmem_io) */
	rtc_io,				/* ,40 : trapa 0x28  int 88h(clk_io) */
	halt41,				/* 41 : trapa 0x29  int 89h(xam_io) */
	sram_io,			/* ,42 : trapa 0x2a  int 8ah(sram_io) */
	halt43,				/* 43 : trapa 0x2b  int 8bh(from_io) */
	com_io,				/* ,44 : trapa 0x2c  int 8ch(com_io) */
	halt45,				/* 45 : trapa 0x2d  int 8dh(i2net_io) */
	halt46,				/* 46 : trapa 0x2e  int 8eh(dsp_io) */
	halt47,				/* 47 : trapa 0x2f  int 8fh(gdc_io) */
	halt48,				/* 48 : trapa 0x30  int 90h(dac_io) */
	halt49,				/* 49 : trapa 0x31  int 91h(cnt_io) */
	halt50,				/* 50 : trapa 0x32  int 92h(brk/retxa) */
	halt51,				/* 51 : trapa 0x33  int 93h(vfd_io) */
	bios_init,			/* 52 : trapa 0x34  int 94h(bios_init) */
	halt53,				/* 53 : trapa 0x35  */
	halt54,				/* 54 : trapa 0x36  */
	halt55,				/* 55 : trapa 0x37  */
	halt56,				/* 56 : trapa 0x38  */
	halt57,				/* 57 : trapa 0x39  */
	halt58,				/* 58 : trapa 0x3a  */
	halt59,				/* 59 : trapa 0x3b  */
	halt60,				/* 60 : trapa 0x3c  */
	halt61,				/* 61 : trapa 0x3d  */
	halt62,				/* 62 : trapa 0x3e  */
	halt63,				/* 63 : trapa 0x3f  */
	halt64,				/* 64 : IRQ0 or IRL1 0x40     */
	halt65,				/* 65 : IRQ1 or IRL2,3 0x41   */
	halt66,				/* 66 : IRQ2 or IRL4,5 0x42   */
	halt67,				/* 67 : IRQ3 or IRL6,7 0x43   */
	halt68,				/* 68 : IRQ4 or IRL8,9 0x44   */
	halt69,				/* 69 : IRQ5 or IRL10,11 0x45 */
	halt70,				/* 70 : IRQ6 or IRL12,13 0x46 */
	halt71,				/* 71 : IRQ7 or IRL14,15 0x47 */
	halt72,				/* 72 : DMAC チャネル0trapa 0x48  */
	halt73,				/* 73 : DMAC チャネル1trapa 0x49  */
	halt74,				/* 74 : ウォッチドックタイマtrapa 0x4A  */
	halt75,				/* 75 : リフレッシュコントローラコンペアマッチtrapa 0x4B  */
	halt76,				/* 76 : EhterC/E-DMAC割込みtrapa 0x4C or WDT */
	halt77,				/* 77 : system reserved  */
	halt78,				/* 78 : FRT インプットキャプチャsystem reserved  */
	halt79,				/* 79 : FRT アウトプットコンペアsystem reserved  */
	halt80,				/* 80 : FRT タイマオーバフローsystem reserved  */
	halt81,				/* 81 : TPU コンペアマッチ/インプットキャプチャ0Asystem reserved  */
	halt82,				/* 82 : TPU コンペアマッチ/インプットキャプチャ0Bsystem reserved  */
	halt83,				/* 83 : TPU コンペアマッチ/インプットキャプチャ0Csystem reserved  */
	halt84,				/* 84 : TPU コンペアマッチ/インプットキャプチャ0Dsystem reserved  */
	halt85,				/* 85 : TPU チｬネル0 オーバフローsystem reserved  */
	halt86,				/* 86 : TPU コンペアマッチ/インプットキャプチャ1Asystem reserved  */
	halt87,				/* 87 : TPU コンペアマッチ/インプットキャプチャ1Bsystem reserved  */
	halt88,				/* 88 : TPU チｬネル1 オーバフローsystem reserved  */
	halt89,				/* 89 : TPU チｬネル1 アンダフローsystem reserved  */
	halt90,				/* 90 : TPU コンペアマッチ/インプットキャプチャ2Asystem reserved  */
	halt91,				/* 91 : TPU コンペアマッチ/インプットキャプチャ2Bsystem reserved  */
	halt92,				/* 92 : TPU チｬネル2 オーバフローsystem reserved  */
	halt93,				/* 93 : TPU チｬネル2 アンダフローsystem reserved  */
	sci1_errint_hdr,	/* 94 : SCIF1 受信エラーsystem reserved  */
	sci1_rxint_hdr,		/* 95 : SCIF1 受信FIFO full/readsystem reserved  */
	sci1_errint_hdr,	/* 96 : SCIF1 ブレーク検出system reserved  */
	sci1_txint_hdr,		/* 97 : SCIF1 送信データエンプティsystem reserved  */
	sci2_errint_hdr,	/* 98 : SCIF2 受信エラーsystem reserved  */
	sci2_rxint_hdr,		/* 99 : SCIF2 受信FIFO full/readysystem reserved  */
	sci2_errint_hdr,	/* 100 : SCIF2 ブレーク検出system reserved  */
	sci2_txint_hdr,		/* 101 : SCIF2 送信データエンプティsystem reserved  */
	halt102,				/* 102 : SIO0 受信オーバランsystem reserved  */
	halt103,				/* 103 : SIO0 送信オーバランsystem reserved  */
	halt104,				/* 104 : SIO0 受信データフルsystem reserved  */
	halt105,				/* 105 : SIO0 送信データエンプティsystem reserved  */
	halt106,				/* 106 : SIO1 受信オーバランsystem reserved  */
	halt107,				/* 107 : SIO1 送信オーバランsystem reserved  */
	halt108,				/* 108 : SIO1 受信データフルsystem reserved  */
	halt109,				/* 109 : SIO1 送信データエンプティsystem reserved  */
	halt110,				/* 110 : SIO2 受信オーバランsystem reserved  */
	halt111,				/* 111 : SIO2 送信オーバランsystem reserved  */
	halt112,				/* 112 : SIO2 受信データフルsystem reserved  */
	halt113,				/* 113 : SIO2 送信データエンプティsystem reserved  */
	halt114,				/* 114 : system reserved  */
	halt115,				/* 115 : system reserved  */
	halt116,				/* 116 : system reserved  */
	halt117,				/* 117 : system reserved  */
	halt118,				/* 118 : system reserved  */
	halt119,				/* 119 : system reserved  */
	halt120,				/* 120 : system reserved  */
	halt121,				/* 121 : system reserved  */
	halt122,				/* 122 : system reserved  */
	halt123,				/* 123 : system reserved  */
	halt124,				/* 124 : system reserved  */
	halt125,				/* 125 : system reserved  */
	halt126,				/* 126 : system reserved  */
	(void *)bios_id,				/* 127 : system reserved  */
	halt128				/* Version Information */
};
#pragma section

