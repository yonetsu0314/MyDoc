/*======================================================================
 * File name    : initsk.c
 * Original		: p:/dacs_i/rcu/main/rcs/initsk.c 1.3
 *----------------------------------------------------------------------
 * Function 	:
 *		イニシャルタスク
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu_v7/bootload/rcs/initsk.c 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: initsk.c $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * ソフトウェア封印に対応するためnetwork設定のバックアップをFLASHから
 * E2ROMへ移動する
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・アプリへのジャンプはマルチタスク状態になる前に行うようにする。
 * ・IPアドレス設定に関する処理をイーサネット初期化前に行うようにする。
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/09 10:00
 * init_tskにおいてAply_CheckSumに条件コンパイルを設ける
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * 起動時のモード判別処理を整理する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/02 9:30
 * LEDタスクをLEDSET.Cに移動し、プログラムを整理
 * 
 * リビジョン 1.19  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:00
 * main関数においてシリアルバッファをクリアする処理を関数化する
 * 
 * リビジョン 1.18  1970/01/01  00:00:00  kagatume
 * 2004/02/11 17:00
 * FIFOINT以外は割り込みマスクする
 * pint_mask をすべてマスクする
 * 
 * リビジョン 1.17  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.16  1970/01/01  00:00:00  kagatume
 * 2004/01/06 14:30
 * init_tsk に割り込みマスクに関する処理を追加する
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * FIFOの追加に対応する
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  kagatume
 * 2003/12/22 17:10
 * comW() を削除する
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  kagatume
 * 2003/12/12 19:00
 * WCUとの通信用extern宣言を追加する
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2003/12/11 16:00
 * comX()を変更し、WCUブートローダと通信ができるようにする
 * remmon()内のdly_tskをコメントにする
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2003/12/01 17:10
 * comN() においてDMUアプリのプログラム名、バージョン番号を認識できるように
 * 変更する。
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2003/11/17 19:30
 * コンパイラのバージョンアップ(Ver 7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * comJ()SRAMﾊﾞｯｸｱｯﾌﾟdataフラッシュ書き込みﾊﾞｸﾞ修正
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  sakaguti
 * WDT状態の取得をやめ文字列コマンド追加
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * 立上げ時WCUへSPではなくCRを送るようにした
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * SRAM F.S.を初期化せずにマウントする
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * bootpara.hをshinc\sh7615に変更
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * remmon()動作時、割込み禁止になっていたのを修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 割込みマスク追加
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
#define	_INITSK_C_
#include <machine.h>			/* set_cr etc	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <assert.h>				/*	"		*/
#include <rxif\rx116.h>			/* Show include */
#include <custom\typedefs.h>	/*	"		*/
#define	HEAPSIZE	32768		/* 32K bytes heap area */
#include <rom\xmcb.h>			/* Show include */

#define	P5348
#define	P5280
#include <biosif\icu.h>			/* Show include dir. */
#include <biosif\pio.h>			/*	"		*/
#include <biosif\mpsc.h>		/*	"		*/
#include <biosif\scua.h>		/*	"		*/
#include <biosif\tcu.h>			/*	"		*/
#include <biosif\xam.h>			/*  " 		*/
#include <biosif\init.h>			/*  " 		*/

#include <drive\serial.h>		/* Show include dir. */
#include <drive\fifdrv.h>		/*	"		*/
#include <drive\e2rdrv.h>		/*			*/
#include <drive\mpscdrv.h>		/*	"		*/
#include <drive\sh7615\scidrv.h>		/*	"		*/
#include <drive\comdrv.h>		/*	"		*/
#include <drive\sramdrv.h>		/*	"		*/
#include <drive\rtcdrv.h>      	/*  "    	*/
#include <drive\eth_drv.h>      	/*  "    	*/
#include <drive\pcmcard.h>      	/*  "    	*/
#include <drive\pcmsock.h>      	/*  "    	*/
#include <drive\atadrv.h>      	/*  "    	*/
#include <drive\lan_init.h>
#include <dos_util.h>
#include <fsys.h>
#include <kernel.h>
#include <sh7615\sh7615.h>
#include <sh7615\7615.h>
#include <sh7615\bootpara.h>
#include <sh7615\dmupara.h>
#include <sh7615\flash.h>

#define	TSKID_MAX	250	/* Maximum ID for TASK				*/
#define	SEMID_MAX	200	/* Maximum ID for SEMAPHORE			*/
#define	TPRI_MAX	31	/* Maximum Priority				*/
#define	FLGID_MAX	99	/* Maximum ID for EVENT FLAG			*/
#define	MBXID_MAX	99	/* Maximum ID for MAIL BOX			*/
#define	MBFID_MAX	1	/* Maximum ID for MESSAGE BUFFER		*/
#define	PORID_MAX	1	/* Maximum ID for RENDEVOUZ PORT		*/
#define	MPLID_MAX	1	/* Maximum ID for VALIABLE LENGTH MEMORY POOL	*/
#define	MPFID_MAX	1	/* Maximum ID for FIXED LENGTH MEMORY POOL	*/
#define	CYCNO_MAX	32	/* Maximum ID for CYCRIC HANDLER		*/
#define	ALMNO_MAX	1	/* Maximum ID for ALARM HANDLER			*/
#define	TMRQSZ		256
#define	CYCQSZ		256
#define	ALMQSZ		256
#define	ISTKSZ		2048	/* Stack size for Interval timer interrupt	*/
#define	TSTKSZ		2048

#include <nocfg4.h>

#define	STORAGE
#include "remmon.h"
#undef	STORAGE
#include "cmfhdl.h"
#include "rxdef.h"
#include "supapl.h"				/* Show current dir. */
#include "rsinout.h"
#include "stat_eth.h"
#include "init.h"
#include "bootcmds.h"

#define TSK_MAIN    1       /* Main task ID */
#define TSK_LED     7       /* LED task ID */
#define TSK_RRR     8       /* LED task ID */

TASK init_tsk(void);
TASK LedTask(void);
TASK RRRTask(void);
TASK CFTask(void);

static const struct scipk defpk0 = {
		C8BITLEN,	/* ｷｬﾗｸﾀ長 */
		C1STPBIT,	/* stop bit */
		NO_PARI,	/* ﾊﾟﾘﾃｨｰ選択 */
		C57KBPS,	/* 115.2KBPS */
		(unsigned short)2048,		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ */
		(unsigned short)2048		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ */
};
static const struct fifpk	defpk5 = {
		2048,                           /* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
		2048                            /* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
};

static const T_CFLG cflg = { TA_WMUL, 0, "" };

/*--------------------------------------------------------------------------*/
/*	タスクの定義															*/
/*--------------------------------------------------------------------------*/
const T_CTSK ctsk_main = { TA_HLNG, NULL, init_tsk, 8, 4096, NULL, (B *)"maintask" };
const T_CTSK ctsk_led  = { TA_HLNG, NULL, LedTask,  8, 4096, NULL, (B *)"ledtask" };
const T_CTSK ctsk_rrr  = { TA_HLNG, NULL, RRRTask,  2, 1024, NULL, (B *)"rrrtask" };
const T_CTSK ctsk_CF   = { TA_HLNG, NULL, CFTask, CF_TSK_PRI, 1024, NULL, (B *)"cftask" };/*020814*/


/*****************************************************************************
*
*	関数名: TASK init_tsk(void)
*	機　能: ユーザーI/Oの初期化、APLタスクの起動を行いinit_tskを削除
*			する。
*
*	引き数:	なし
*
*	戻り値: なし
*
******************************************************************************/
TASK init_tsk(void)
{
	int		tmp;

	int_mask(0x0f);
	pint_mask(0x0f);

	scif_sys_init(SCIF1, (const struct scipk *)&defpk0, scif_rxaa[SCIF1]);
	scif_sys_init(SCIF2, (const struct scipk *)&defpk0, scif_rxaa[SCIF2]);
	fif_sys_init(FIFO1, (const struct fifpk *)&defpk5, fif_rxaa[FIFO1]);
	e2_sys_init((unsigned short)1024);
	sram_sys_init((unsigned short)0);
	rtc_sys_init();
	
	setup_apl_func();

	InitFileSystem();	/* ファイルシステム初期化 *//*020813*/
	InitSramDisk(0);	/* ＳＲＡＭディスク初期化(フォーマットせず) */
	InitAtaDisk();		/* ＡＴＡディスクの使用開始 */

	cre_tsk(CF_TSK_ID, &ctsk_CF);
	sta_tsk(CF_TSK_ID, 0); /* ＣＦ検出タスク起動 */
	chdir("e:\\");

	e2s_sys_init((unsigned short)128);

	if(tmp = (PA.DR.BIT.B11 == 0)){	/* dipsw 3:ONの場合 デフォルトIP 192.168.0.11をセット */
		set_default_ipaddr();
		rsstr("Default DIPSW mode.\n");
	}
	
	set_Tcpip_toUsnet();

	cre_tsk(TSK_RRR,  &ctsk_rrr);
	cre_tsk(TSK_LED,  &ctsk_led);
	sta_tsk(TSK_RRR, 0);	/* モニタに関係 */
	sta_tsk(TSK_LED, 0);	/* LED点灯に関係 */

	int_mask(~FIFOINT);		/* FIFOINT以外は割込みマスクする */
	pint_mask(0x0f);
	dly_tsk( (RELTIM)100);
	set_imask(0);
	
	if (aply_start_check()) remmon();
	
	set_mode(RESET_CMD);
	strcpy( APLY_START_STATUS, "AplStat" );
	dmu_jump_to_reset();	/* アプリ起動 */
}


/*----------------------------------------------------------------
				通信タスク
----------------------------------------------------------------*/
TASK RRRTask(void){
	InitLanSystem();
	FTPserver();		/* ＦＴＰ通信に関するタスク起動 */
	SocketServer();		/* ソケット通信に関するタスク起動 */
	EXD_TSK();
}

/*----------------------------------------------------------------
				CF検出タスク
----------------------------------------------------------------*/
TASK CFTask(void)
{
	/* C/F検出（0.5秒ポーリング） */
	for(;;){
		PcmciaEvent();
		dly_tsk((RELTIM)500/MSEC);/*020801*/
	}
	return;
}


/*----------------------------------------------------------------
				ライブラリ関数使用の変数の初期化
----------------------------------------------------------------*/
static void _INIT_IOLIB(void){
    FILE *fp;

    for(fp=_iob; fp<_iob+_NFILE; fp++ ){
        fp->_bufptr=NULL;
        fp->_bufcnt=0;
        fp->_buflen=0;
        fp->_bufbase=NULL;
        fp->_ioflag1=(char)0;
        fp->_ioflag2=(char)0;
        fp->_iofd=(char)0;
    }

    if(freopen( "stdin" , "r", stdin)==NULL)
        stdin->_ioflag1=(char)0xff;
    stdin->_ioflag1 |= _IOUNBUF;

    if(freopen( "stdout" , "w", stdout)==NULL)
        stdout->_ioflag1=(char)0xff;
    stdout->_ioflag1 |= _IOUNBUF;

    if(freopen( "stderr" , "w", stderr)==NULL)
        stderr->_ioflag1=(char)0xff;
    stderr->_ioflag1 |= _IOUNBUF;
}

static void _INITLIB( void ){
	_INIT_IOLIB();
}


/*----------------------------------------------------------------
				メイン関数
----------------------------------------------------------------*/
void
main(void)
{
	ID	j;

	InitCardCtl();/*020814*/

    sysini();

	SetPcmciaIntHandler(0,0,(void *)NULL);	/* intPCM def_inh(68, &dint_irq); */
	_INITLIB();

	con0_buf_clear();
//	con1_buf_clear();

	/* イベントフラグ生成 */
	for( j=1; j<FLG_MAX; j++ ) cre_flg( j, &cflg );

	cre_tsk(TSK_MAIN, &ctsk_main);

	sta_tsk(TSK_MAIN, 0);

	init_sys_ether();		/* irhan00 def_int((INHNO)EDMAC_VECTNO, &dint_etherhdr); */

    intsta();               /* Start interval timer interrupt */

    syssta();                   /* enter into multi task */
}

