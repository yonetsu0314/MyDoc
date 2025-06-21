/*======================================================================
 * File name    : initsk.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: イニシャルタスク
 *----------------------------------------------------------------------
 *$Header: p:/mhic/main/ccw/rcs/initsk.c 1.1.4102.1 1997/05/28 14:23:11 yonetsu Exp $
 *$Log: initsk.c $
 * リビジョン 1.1.4102.1  1997/05/28  14:23:11  yonetsu
 * パケットのパラメータの設定を外部で行えるようにする。
 * 
 * リビジョン 1.1  1996/08/13  19:24:28  ryoji
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#include	<string.h>
#include	<assert.h>			/* ANSI std	*/
#include	<rxif\rx116.h>			/* Show include	*/
#include	<custom\typedefs.h>		/*	"	*/
#define	HEAPSIZE	16384			/* 16K bytes heap area */
#include	<rom\xmcb.h>			/* Show include	*/

#include	<biosif\icu.h>			/* Show include dir. */

#include	<biosif\ppia.h>			/*	"		*/

#include	<biosif\mpsc.h>			/*	"		*/
#include	<biosif\scua.h>			/*	"		*/
#include	<biosif\tcu.h>			/*	"		*/

#include	<drive\serial.h>		/* Show include dir	*/
#include	<drive\sh7615\scidrv.h>		/*	"		*/
#include	<drive\pcmcard.h>		/*	"		*/
#include	<drive\e2srdrv.h>		/*	"		*/
#include	<drive\eth_drv.h>		/*	"		*/
#include	<drive\sramdrv.h>		/*	"		*/
#include	<drive\rtcdrv.h>		/*	"		*/
#include	<drive\piodrva.h>		/*	"		*/
#include	<drive\comdrva.h>		/*	"		*/
#include	<sh7615\bootpara.h>		/*	"		*/

#include	"rxdef.h"			/* Show current dir. */
#include	"supapl.h"			/* Show current dir. */
#include	"cmfhdl.h"			/* Show current dir. */
#include	"fnctbl.h"			/* Show current dir. */

static void	halt_func(int);

#define	START_ADR	(int (*)())0xf5000000
#define	START_ID	0xa6e8

/*----------------------------------------------------------------------*/
/*	init_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ユーザーＩ／Ｏの初期化、ＡＰＬタスクの起動を行いinit_tskを削除	*/
/*	する。								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

void
init_tsk(void)
{
	int		err;		/* error code */

	/* ハードウェア、システム関連の初期化 */
	scif_sys_init(SCIF1, get_pkt_spec(CON_DN0), con_rxaa[SCIF1]);
	scif_sys_init(SCIF2, get_pkt_spec(CON_DN1), con_rxaa[SCIF2]);

	SetPcmciaIntHandler(0,0,(void *)NULL);	/* CF割込みハンドラのセット	*/
	init_sys_ether();			/* Ethernet割込みハンドラのセット*/

	err = e2s_sys_init((unsigned short)128);/* EEPROMデバイスの初期化	*/
	assert(! err);


	err = _pio_init(PIO0);
	assert(! err);
	err = _pio_init(PIO1);
	assert(! err);

	err = arcnet_sys_init( ARCNET0, get_pkt_spec(RCU_DN), com_rxaa[ARCNET0]);
	assert(! err);
	err = arcnet_sys_init( ARCNET1, get_pkt_spec(HIC_DN), com_rxaa[ARCNET1]);
	assert(! err);

/* 外割り込みサービスの初期化
	err = pio_sys_init( SINTP0, sgaa[SINTP0]);
	assert(! err);
	err = pio_sys_init( SINTP1, sgaa[SINTP1]);
	assert(! err);
*/

	sram_sys_init((unsigned short)0);	/* SRAMデバイスの初期化		*/
	rtc_sys_init();				/* リアルタイムクロック初期化*/
	InitCardCtl();				/* ＣＦカード制御の初期化	*/
	InitFileSystem();			/* ファイルシステム初期化	*/
	InitSramDisk(0);			/* ＳＲＡＭディスク初期化	*/
	InitAtaDisk();				/* ＡＴＡディスクの初期化	*/

	err = setup_apl_func();
	if(err)  halt_func(err);

	int_mask( ~(ARCINT0|ARCINT1) );	
/*
	pint_mask(~(PINT0));
*/
	EXD_TSK();
}

/*----------------------------------------------------------------------*/
/*	halt_func							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＡＰＬプログラムを停止する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	err	: error code						*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

static void
halt_func(
	int		err
	)
{
	for(;;) {
	}
	err++;
}
