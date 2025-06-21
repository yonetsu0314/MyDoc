/*======================================================================
 * File name    : supapl.c
 * Original		: p:/dacs_i/rcu/main/rcs/supapl.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		アプリケーションセットアップ
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu_v7/bootload/rcs/supapl.c 1.6 1970/01/01 00:00:00 kagatume Exp $
 *$Log: supapl.c $
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/07 8:40
 * FIFO 受信タスクの生成に関する処理を setup_apl_func に追加する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * FIFOの追加に対応する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/19 19:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_SUPAPL_C_
#include <string.h>
#include <kernel.h>
#include <rxif\rx116.h>			/* Show include	*/

#include <drive\piodrva.h>		/*	"		*/
#include <drive\sh7615\scidrv.h>		/*	"		*/
#include <drive\comdrv.h>		/*	"		*/
#include <drive\eth_def.h>				/* 7777777 */
#include <sh7615\bootpara.h>

#define STORAGE
#include <biosif\icu.h>			/* Show include	*/
#include "supapl.h"				/* Show current dir. */
#include "rxdef.h"				/* Show current dir. */
#include "cmfhdl.h"				/* Show current dir. */
#undef STORAGE

#include "cortsk.h"				/* Show Current */
#include "fif001.h"			
#include "vector.h"
#include "rsinout.h"
#include "init.h"


//	T_DINT dint_irq = { TA_HLNG, intPCM, 1 };
//	def_inh(68, &dint_irq);	   /* Ready/Busy割込み level 9 *//*020805*/
//	def_inh(69, &dint_irq);	   /* Ready/Busy割込み level 10 *//*020912 saka */
//void intPCM(void);

void isritm(void);

void fif_int_hdr(void);

#pragma interrupt(arcnet0_int_hdr_)
void arcnet0_int_hdr_(void)
{
	ent_int();
	arcnet0_int_hdr();/*ライブラリーを関数コール*/
	ret_int();
}

#pragma interrupt(arcnet1_int_hdr_)
void arcnet1_int_hdr_(void)
{
	ent_int();
	arcnet1_int_hdr();/*ライブラリーを関数コール*/
	ret_int();
}


#pragma interrupt(pint0_int_hdr)
void pint0_int_hdr(void)
{
	*INT_ACK_REG1 = (char)0xff;		/* Disable Outernal Interrupt */
	ent_int();			/* 割込みハンドラの開始 */
	rsstr232("*** pint0_int_hdr ***");
	ret_int();			/* 割込みハンドラから復帰する */
}


#pragma interrupt(pint1_int_hdr)
void pint1_int_hdr(void)
{
	*INT_ACK_REG1 = (char)0xff;		/* Disable Outernal Interrupt */
	ent_int();			/* 割込みハンドラの開始 */
	rsstr232("*** pint1_int_hdr ***");
	ret_int();			/* 割込みハンドラから復帰する */
}


#pragma interrupt(pint2_int_hdr)
void pint2_int_hdr(void)
{
	*INT_ACK_REG1 = (char)0xff;		/* Disable Outernal Interrupt */
	ent_int();			/* 割込みハンドラの開始 */
	rsstr232("*** pint2_int_hdr ***");
	ret_int();			/* 割込みハンドラから復帰する */
}


#pragma interrupt(pint3_int_hdr)
void pint3_int_hdr(void)
{
	*INT_ACK_REG1 = (char)0xff;		/* Disable Outernal Interrupt */
	ent_int();			/* 割込みハンドラの開始 */
	rsstr232("*** pint3_int_hdr ***");
	ret_int();			/* 割込みハンドラから復帰する */
}


#pragma interrupt(fif_int_hdr_)
void fif_int_hdr_(void)
{
	ent_int();			/* 割込みハンドラの開始 */
//	*INT_ACK_REG0 = (char)0xff;		/* Disable Outernal Interrupt */
	fif_int_hdr();
	ret_int();			/* 割込みハンドラから復帰する */
}


/*------------------------------------------------------*/
/*	割り込みハンドラのベクター･テーブル					*/
/*														*/
/*------------------------------------------------------*/
extern const char	rom_id[];

const struct vectsx	vector_strc = {
		65,				/* sizeof(fun) /4	*/
	(void *)rom_id,		/* 128 : Version Information */
	dum127,				/* 127 : system reserved  */
	dum126,				/* 126 : system reserved  */
	dum125,				/* 125 : system reserved  */
	dum124,				/* 124 : system reserved  */
	dum123,				/* 123 : system reserved  */
	dum122,				/* 122 : system reserved  */
	dum121,				/* 121 : system reserved  */
	dum120,				/* 120 : system reserved  */
	dum119,				/* 119 : system reserved  */
	dum118,				/* 118 : system reserved  */
	dum117,				/* 117 : system reserved  */
	dum116,				/* 116 : system reserved  */
	dum115,				/* 115 : system reserved  */
	dum114,				/* 114 : system reserved  */
	dum113,				/* 113 : SIO2 送信データエンプティsystem reserved  */
	dum112,				/* 112 : SIO2 受信データフルsystem reserved  */
	dum111,				/* 111 : SIO2 送信オーバランsystem reserved  */
	dum110,				/* 110 : SIO2 受信オーバランsystem reserved  */
	dum109,				/* 109 : SIO1 送信データエンプティsystem reserved  */
	dum108,				/* 108 : SIO1 受信データフルsystem reserved  */
	dum107,				/* 107 : SIO1 送信オーバランsystem reserved  */
	dum106,				/* 106 : SIO1 受信オーバランsystem reserved  */
	dum105,				/* 105 : SIO0 送信データエンプティsystem reserved  */
	dum104,				/* 104 : SIO0 受信データフルsystem reserved  */
	dum103,				/* 103 : SIO0 送信オーバランsystem reserved  */
	dum102,				/* 102 : SIO0 受信オーバランsystem reserved  */
	sci2_txint_hdr,		/* 101 : SCIF2 送信データエンプティsystem reserved  */
	sci2_errint_hdr,	/* 100 : SCIF2 ブレーク検出system reserved  */
	sci2_rxint_hdr,		/* 99 : SCIF2 受信FIFO full/readysystem reserved  */
	sci2_errint_hdr,	/* 98 : SCIF2 受信エラーsystem reserved  */
	sci1_txint_hdr,		/* 97 : SCIF1 送信データエンプティsystem reserved  */
	sci1_errint_hdr,	/* 96 : SCIF1 ブレーク検出system reserved  */
	sci1_rxint_hdr,		/* 95 : SCIF1 受信FIFO full/readsystem reserved  */
	sci1_errint_hdr,	/* 94 : SCIF1 受信エラーsystem reserved  */
	dum93,				/* 93 : TPU チｬネル2 アンダフローsystem reserved  */
	dum92,				/* 92 : TPU チｬネル2 オーバフローsystem reserved  */
	dum91,				/* 91 : TPU コンペアマッチ/インプットキャプチャ2Bsystem reserved  */
	isritm,				/* 90 : TPU コンペアマッチ/インプットキャプチャ2Asystem reserved  */
	dum89,				/* 89 : TPU チｬネル1 アンダフローsystem reserved  */
	dum88,				/* 88 : TPU チｬネル1 オーバフローsystem reserved  */
	dum87,				/* 87 : TPU コンペアマッチ/インプットキャプチャ1Bsystem reserved  */
	dum86,				/* 86 : TPU コンペアマッチ/インプットキャプチャ1Asystem reserved  */
	dum85,				/* 85 : TPU チｬネル0 オーバフローsystem reserved  */
	dum84,				/* 84 : TPU コンペアマッチ/インプットキャプチャ0Dsystem reserved  */
	dum83,				/* 83 : TPU コンペアマッチ/インプットキャプチャ0Csystem reserved  */
	dum82,				/* 82 : TPU コンペアマッチ/インプットキャプチャ0Bsystem reserved  */
	dum81,				/* 81 : TPU コンペアマッチ/インプットキャプチャ0Asystem reserved  */
	dum80,				/* 80 : FRT タイマオーバフローsystem reserved  */
	dum79,				/* 79 : FRT アウトプットコンペアsystem reserved  */
	dum78,				/* 78 : FRT インプットキャプチャsystem reserved  */
	dum77,				/* 77 : system reserved  */
	dum76,				/* 76 : EhterC/E-DMAC割込みtrapa 0x4C or WDT */
	dum75,				/* 75 : リフレッシュコントローラコンペアマッチtrapa 0x4B  */
	dum74,				/* 74 : ウォッチドックタイマtrapa 0x4A  */
	irhan00,				/* 73 : DMAC チャネル1trapa 0x49  */
	dum72,				/* 72 : DMAC チャネル0trapa 0x48  */
	pint3_int_hdr,				/* 71 : IRQ7 or IRL14,15 0x47 */
	pint2_int_hdr,				/* 70 : IRQ6 or IRL12,13 0x46 */
	pint1_int_hdr,				/* 69 : IRQ5 or IRL10,11 0x45 */
	pint0_int_hdr,				/* 68 : IRQ4 or IRL8,9 0x44   */
	arcnet1_int_hdr_,			/* 67 : IRQ3 or IRL6,7 0x43   */
	arcnet0_int_hdr_,			/* 66 : IRQ2 or IRL4,5 0x42   */
	fif_int_hdr_,				/* 65 : IRQ1 or IRL2,3 0x41   */
	dum64				/* 64 : IRQ0 or IRL1 0x40     */
};


/*------------------------------------------------------*/
/*	アクセス･アドレスへのポインターの配列				*/
/*														*/
/*------------------------------------------------------*/

union aap *const scif_rxaa[] = {
		&con0_rcv_tsk_aa,		/* SCU1 RX task AA */
		&con1_rcv_tsk_aa		/* SCU2 RX task AA */
};

union aap	*const fif_rxaa[] = {
		&fif_rcv_tsk_aa			/* FIFO RX/TX task AA 	*/
};

union aap *const arcnet_rxaa[] = {
		(void *)0
};

union aap *const key_rxaa[] = {
		(void *)0
};

/*****************************************************************************
*
*	関数名: int setup_apl_func(void)
*
*	機　能: APLタスクを生成する。
*
*	引き数:	なし
*
*	戻り値: int
*					 0	: complete
*					-1	: error code
*
******************************************************************************/
int setup_apl_func(void)
{
	int err;	/* error code */

	apl_ds.ifp = (int *)&apl_ds.acadr;
	if((err = port_tbl_init()) < 0) return(-1);
	if((err = setup_con0_rcv_tsk()) < 0) return(-1);
	if((err = setup_con1_rcv_tsk()) < 0) return(-1);
	if((err = setup_fif_rcv()) < 0) return(-1);
	return(0);
}


#pragma interrupt( dum0 )
void dum0( void ){ ent_int(); rsstr232("dum0 "); ret_int(); }
#pragma interrupt( dum1 )
void dum1( void ){ ent_int(); rsstr232("dum1 "); ret_int(); }
#pragma interrupt( dum2 )
void dum2( void ){ ent_int(); rsstr232("dum2 "); ret_int(); }
#pragma interrupt( dum3 )
void dum3( void ){ ent_int(); rsstr232("dum3 "); ret_int(); }
#pragma interrupt( dum4 )
void dum4( void ){ ent_int(); rsstr232("dum4 "); ret_int(); }
#pragma interrupt( dum5 )
void dum5( void ){ ent_int(); rsstr232("dum5 "); ret_int(); }
#pragma interrupt( dum6 )
void dum6( void ){ ent_int(); rsstr232("dum6 "); ret_int(); }
#pragma interrupt( dum7 )
void dum7( void ){ ent_int(); rsstr232("dum7 "); ret_int(); }
#pragma interrupt( dum8 )
void dum8( void ){ ent_int(); rsstr232("dum8 "); ret_int(); }
#pragma interrupt( dum9 )
void dum9( void ){ ent_int(); rsstr232("dum9 "); ret_int(); }
#pragma interrupt( dum10 )
void dum10( void ){ ent_int(); rsstr232("dum10 "); ret_int(); }
#pragma interrupt( dum11 )
void dum11( void ){ ent_int(); rsstr232("dum11 "); ret_int(); }
#pragma interrupt( dum12 )
void dum12( void ){ ent_int(); rsstr232("dum12 "); ret_int(); }
#pragma interrupt( dum13 )
void dum13( void ){ ent_int(); rsstr232("dum13 "); ret_int(); }
#pragma interrupt( dum14 )
void dum14( void ){ ent_int(); rsstr232("dum14 "); ret_int(); }
#pragma interrupt( dum15 )
void dum15( void ){ ent_int(); rsstr232("dum15 "); ret_int(); }
#pragma interrupt( dum16 )
void dum16( void ){ ent_int(); rsstr232("dum16 "); ret_int(); }
#pragma interrupt( dum17 )
void dum17( void ){ ent_int(); rsstr232("dum17 "); ret_int(); }
#pragma interrupt( dum18 )
void dum18( void ){ ent_int(); rsstr232("dum18 "); ret_int(); }
#pragma interrupt( dum19 )
void dum19( void ){ ent_int(); rsstr232("dum19 "); ret_int(); }
#pragma interrupt( dum20 )
void dum20( void ){ ent_int(); rsstr232("dum20 "); ret_int(); }
#pragma interrupt( dum21 )
void dum21( void ){ ent_int(); rsstr232("dum21 "); ret_int(); }
#pragma interrupt( dum22 )
void dum22( void ){ ent_int(); rsstr232("dum22 "); ret_int(); }
#pragma interrupt( dum23 )
void dum23( void ){ ent_int(); rsstr232("dum23 "); ret_int(); }
#pragma interrupt( dum24 )
void dum24( void ){ ent_int(); rsstr232("dum24 "); ret_int(); }
#pragma interrupt( dum25 )
void dum25( void ){ ent_int(); rsstr232("dum25 "); ret_int(); }
#pragma interrupt( dum26 )
void dum26( void ){ ent_int(); rsstr232("dum26 "); ret_int(); }
#pragma interrupt( dum27 )
void dum27( void ){ ent_int(); rsstr232("dum27 "); ret_int(); }
#pragma interrupt( dum28 )
void dum28( void ){ ent_int(); rsstr232("dum28 "); ret_int(); }
#pragma interrupt( dum29 )
void dum29( void ){ ent_int(); rsstr232("dum29 "); ret_int(); }
#pragma interrupt( dum30 )
void dum30( void ){ ent_int(); rsstr232("dum30 "); ret_int(); }
#pragma interrupt( dum31 )
void dum31( void ){ ent_int(); rsstr232("dum31 "); ret_int(); }
#pragma interrupt( dum32 )
void dum32( void ){ ent_int(); rsstr232("dum32 "); ret_int(); }
#pragma interrupt( dum33 )
void dum33( void ){ ent_int(); rsstr232("dum33 "); ret_int(); }
#pragma interrupt( dum34 )
void dum34( void ){ ent_int(); rsstr232("dum34 "); ret_int(); }
#pragma interrupt( dum35 )
void dum35( void ){ ent_int(); rsstr232("dum35 "); ret_int(); }
#pragma interrupt( dum36 )
void dum36( void ){ ent_int(); rsstr232("dum36 "); ret_int(); }
#pragma interrupt( dum37 )
void dum37( void ){ ent_int(); rsstr232("dum37 "); ret_int(); }
#pragma interrupt( dum38 )
void dum38( void ){ ent_int(); rsstr232("dum38 "); ret_int(); }
#pragma interrupt( dum39 )
void dum39( void ){ ent_int(); rsstr232("dum39 "); ret_int(); }
#pragma interrupt( dum40 )
void dum40( void ){ ent_int(); rsstr232("dum40 "); ret_int(); }
#pragma interrupt( dum41 )
void dum41( void ){ ent_int(); rsstr232("dum41 "); ret_int(); }
#pragma interrupt( dum42 )
void dum42( void ){ ent_int(); rsstr232("dum42 "); ret_int(); }
#pragma interrupt( dum43 )
void dum43( void ){ ent_int(); rsstr232("dum43 "); ret_int(); }
#pragma interrupt( dum44 )
void dum44( void ){ ent_int(); rsstr232("dum44 "); ret_int(); }
#pragma interrupt( dum45 )
void dum45( void ){ ent_int(); rsstr232("dum45 "); ret_int(); }
#pragma interrupt( dum46 )
void dum46( void ){ ent_int(); rsstr232("dum46 "); ret_int(); }
#pragma interrupt( dum47 )
void dum47( void ){ ent_int(); rsstr232("dum47 "); ret_int(); }
#pragma interrupt( dum48 )
void dum48( void ){ ent_int(); rsstr232("dum48 "); ret_int(); }
#pragma interrupt( dum49 )
void dum49( void ){ ent_int(); rsstr232("dum49 "); ret_int(); }
#pragma interrupt( dum50 )
void dum50( void ){ ent_int(); rsstr232("dum50 "); ret_int(); }
#pragma interrupt( dum51 )
void dum51( void ){ ent_int(); rsstr232("dum51 "); ret_int(); }
#pragma interrupt( dum52 )
void dum52( void ){ ent_int(); rsstr232("dum52 "); ret_int(); }
#pragma interrupt( dum53 )
void dum53( void ){ ent_int(); rsstr232("dum53 "); ret_int(); }
#pragma interrupt( dum54 )
void dum54( void ){ ent_int(); rsstr232("dum54 "); ret_int(); }
#pragma interrupt( dum55 )
void dum55( void ){ ent_int(); rsstr232("dum55 "); ret_int(); }
#pragma interrupt( dum56 )
void dum56( void ){ ent_int(); rsstr232("dum56 "); ret_int(); }
#pragma interrupt( dum57 )
void dum57( void ){ ent_int(); rsstr232("dum57 "); ret_int(); }
#pragma interrupt( dum58 )
void dum58( void ){ ent_int(); rsstr232("dum58 "); ret_int(); }
#pragma interrupt( dum59 )
void dum59( void ){ ent_int(); rsstr232("dum59 "); ret_int(); }
#pragma interrupt( dum60 )
void dum60( void ){ ent_int(); rsstr232("dum60 "); ret_int(); }
#pragma interrupt( dum61 )
void dum61( void ){ ent_int(); rsstr232("dum61 "); ret_int(); }
#pragma interrupt( dum62 )
void dum62( void ){ ent_int(); rsstr232("dum62 "); ret_int(); }
#pragma interrupt( dum63 )
void dum63( void ){ ent_int(); rsstr232("dum63 "); ret_int(); }
#pragma interrupt( dum64 )
void dum64( void ){ ent_int(); rsstr232("dum64 "); ret_int(); }
#pragma interrupt( dum65 )
void dum65( void ){ ent_int(); rsstr232("dum65 "); ret_int(); }
#pragma interrupt( dum66 )
void dum66( void ){ ent_int(); rsstr232("dum66 "); ret_int(); }
#pragma interrupt( dum67 )
void dum67( void ){ ent_int(); rsstr232("dum67 "); ret_int(); }
#pragma interrupt( dum68 )
void dum68( void ){ ent_int(); rsstr232("dum68 "); ret_int(); }
#pragma interrupt( dum69 )
void dum69( void ){ ent_int(); rsstr232("dum69 "); ret_int(); }
#pragma interrupt( dum70 )
void dum70( void ){ ent_int(); rsstr232("dum70 "); ret_int(); }
#pragma interrupt( dum71 )
void dum71( void ){ ent_int(); rsstr232("dum71 "); ret_int(); }
#pragma interrupt( dum72 )
void dum72( void ){ ent_int(); rsstr232("dum72 "); ret_int(); }
#pragma interrupt( dum73 )
void dum73( void ){ ent_int(); rsstr232("dum73 "); ret_int(); }
#pragma interrupt( dum74 )
void dum74( void ){ ent_int(); rsstr232("dum74 "); ret_int(); }
#pragma interrupt( dum75 )
void dum75( void ){ ent_int(); rsstr232("dum75 "); ret_int(); }
#pragma interrupt( dum76 )
void dum76( void ){ ent_int(); rsstr232("dum76 "); ret_int(); }
#pragma interrupt( dum77 )
void dum77( void ){ ent_int(); rsstr232("dum77 "); ret_int(); }
#pragma interrupt( dum78 )
void dum78( void ){ ent_int(); rsstr232("dum78 "); ret_int(); }
#pragma interrupt( dum79 )
void dum79( void ){ ent_int(); rsstr232("dum79 "); ret_int(); }
#pragma interrupt( dum80 )
void dum80( void ){ ent_int(); rsstr232("dum80 "); ret_int(); }
#pragma interrupt( dum81 )
void dum81( void ){ ent_int(); rsstr232("dum81 "); ret_int(); }
#pragma interrupt( dum82 )
void dum82( void ){ ent_int(); rsstr232("dum82 "); ret_int(); }
#pragma interrupt( dum83 )
void dum83( void ){ ent_int(); rsstr232("dum83 "); ret_int(); }
#pragma interrupt( dum84 )
void dum84( void ){ ent_int(); rsstr232("dum84 "); ret_int(); }
#pragma interrupt( dum85 )
void dum85( void ){ ent_int(); rsstr232("dum85 "); ret_int(); }
#pragma interrupt( dum86 )
void dum86( void ){ ent_int(); rsstr232("dum86 "); ret_int(); }
#pragma interrupt( dum87 )
void dum87( void ){ ent_int(); rsstr232("dum87 "); ret_int(); }
#pragma interrupt( dum88 )
void dum88( void ){ ent_int(); rsstr232("dum88 "); ret_int(); }
#pragma interrupt( dum89 )
void dum89( void ){ ent_int(); rsstr232("dum89 "); ret_int(); }
#pragma interrupt( dum90 )
void dum90( void ){ ent_int(); rsstr232("dum90 "); ret_int(); }
#pragma interrupt( dum91 )
void dum91( void ){ ent_int(); rsstr232("dum91 "); ret_int(); }
#pragma interrupt( dum92 )
void dum92( void ){ ent_int(); rsstr232("dum92 "); ret_int(); }
#pragma interrupt( dum93 )
void dum93( void ){ ent_int(); rsstr232("dum93 "); ret_int(); }
#pragma interrupt( dum94 )
void dum94( void ){ ent_int(); rsstr232("dum94 "); ret_int(); }
#pragma interrupt( dum95 )
void dum95( void ){ ent_int(); rsstr232("dum95 "); ret_int(); }
#pragma interrupt( dum96 )
void dum96( void ){ ent_int(); rsstr232("dum96 "); ret_int(); }
#pragma interrupt( dum97 )
void dum97( void ){ ent_int(); rsstr232("dum97 "); ret_int(); }
#pragma interrupt( dum98 )
void dum98( void ){ ent_int(); rsstr232("dum98 "); ret_int(); }
#pragma interrupt( dum99 )
void dum99( void ){ ent_int(); rsstr232("dum99 "); ret_int(); }
#pragma interrupt( dum100 )
void dum100( void ){ ent_int(); rsstr232("dum100 "); ret_int(); }
#pragma interrupt( dum101 )
void dum101( void ){ ent_int(); rsstr232("dum101 "); ret_int(); }
#pragma interrupt( dum102 )
void dum102( void ){ ent_int(); rsstr232("dum102 "); ret_int(); }
#pragma interrupt( dum103 )
void dum103( void ){ ent_int(); rsstr232("dum103 "); ret_int(); }
#pragma interrupt( dum104 )
void dum104( void ){ ent_int(); rsstr232("dum104 "); ret_int(); }
#pragma interrupt( dum105 )
void dum105( void ){ ent_int(); rsstr232("dum105 "); ret_int(); }
#pragma interrupt( dum106 )
void dum106( void ){ ent_int(); rsstr232("dum106 "); ret_int(); }
#pragma interrupt( dum107 )
void dum107( void ){ ent_int(); rsstr232("dum107 "); ret_int(); }
#pragma interrupt( dum108 )
void dum108( void ){ ent_int(); rsstr232("dum108 "); ret_int(); }
#pragma interrupt( dum109 )
void dum109( void ){ ent_int(); rsstr232("dum109 "); ret_int(); }
#pragma interrupt( dum110 )
void dum110( void ){ ent_int(); rsstr232("dum110 "); ret_int(); }
#pragma interrupt( dum111 )
void dum111( void ){ ent_int(); rsstr232("dum111 "); ret_int(); }
#pragma interrupt( dum112 )
void dum112( void ){ ent_int(); rsstr232("dum112 "); ret_int(); }
#pragma interrupt( dum113 )
void dum113( void ){ ent_int(); rsstr232("dum113 "); ret_int(); }
#pragma interrupt( dum114 )
void dum114( void ){ ent_int(); rsstr232("dum114 "); ret_int(); }
#pragma interrupt( dum115 )
void dum115( void ){ ent_int(); rsstr232("dum115 "); ret_int(); }
#pragma interrupt( dum116 )
void dum116( void ){ ent_int(); rsstr232("dum116 "); ret_int(); }
#pragma interrupt( dum117 )
void dum117( void ){ ent_int(); rsstr232("dum117 "); ret_int(); }
#pragma interrupt( dum118 )
void dum118( void ){ ent_int(); rsstr232("dum118 "); ret_int(); }
#pragma interrupt( dum119 )
void dum119( void ){ ent_int(); rsstr232("dum119 "); ret_int(); }
#pragma interrupt( dum120 )
void dum120( void ){ ent_int(); rsstr232("dum120 "); ret_int(); }
#pragma interrupt( dum121 )
void dum121( void ){ ent_int(); rsstr232("dum121 "); ret_int(); }
#pragma interrupt( dum122 )
void dum122( void ){ ent_int(); rsstr232("dum122 "); ret_int(); }
#pragma interrupt( dum123 )
void dum123( void ){ ent_int(); rsstr232("dum123 "); ret_int(); }
#pragma interrupt( dum124 )
void dum124( void ){ ent_int(); rsstr232("dum124 "); ret_int(); }
#pragma interrupt( dum125 )
void dum125( void ){ ent_int(); rsstr232("dum125 "); ret_int(); }
#pragma interrupt( dum126 )
void dum126( void ){ ent_int(); rsstr232("dum126 "); ret_int(); }
#pragma interrupt( dum127 )
void dum127( void ){ ent_int(); rsstr232("dum127 "); ret_int(); }

