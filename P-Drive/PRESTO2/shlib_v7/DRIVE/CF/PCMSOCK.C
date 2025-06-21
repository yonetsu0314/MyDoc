/*======================================================================
 * File name    : pcmsock.c
 * Original		: r:/c/astra-s/exp/rcs/pcmsock.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/cf/rcs/pcmsock.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmsock.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 05.03.18 CD_CHKをBIOSに移動
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*                                                                      */
/*      ファイル名      : PCMSOCK.C                                     */
/* ==================================================================== */
/*      機能分類        : ＰＣＭＣＩＡソケットサービスＡＰＩ	        */
/*      作成日付        : 1997.12. 5 < Fri >                            */
/*      作成者名        :                                               */
/*                                                                      */
/*  履歴管理                                                            */
/* ------------+----------+-------------------------------------------- */
/*  日 付      | 管理番号 | 担当者名及び変更内容                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#include <stddef.h>
//#include </itron4/kernel.h>/*020726*/
#include <kernel.h>/*020726*/
#include <sh7615\7615.h>
#include <biosif\cf.h>
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#include "drive\pcmtpl.h"

	/*==========================================================
						内部レジスタ宣言
	==========================================================*/
static char fake[1];
static char pcm_busy;
static void (*PcmcFptr)( PCMEVT *evt );
static PCMEVT PcmcEvent;


/*---------------------------------------------------------------------
				ソケットサービスのモニタ関数
 ---------------------------------------------------------------------*/
static void ss_monitor( void )
{
	unsigned long *events;
	int i=0;

	if( !pcm_busy )	{
		events = &PcmcEvent.sock[i].event;
		if( fake[i] ) {
			*events =  0;
		}
		else {
			*events = _cd_chk( i ) ? SS_DETECT : 0;			/* カード検知 		*/
		}
	}
}

/*---------------------------------------------------------------
				ＰＣＭＣＩＡイベント処理
 ---------------------------------------------------------------*/
static void PcmciaHandler( PCMEVT *evt )
{
	if( PcmcFptr != NULL ) {
		PcmcFptr( evt );
		PcmcEvent.sock[0].last = PcmcEvent.sock[0].event;
		if( fake[0] ) {
			fake[0] = 0;
		}
	}
	pcm_busy = 0;
}

/*****************************************************************
				ＰＣＭＣＩＡイベント処理の登録
*****************************************************************/
void SetPcmciaHandler( void (*fptr)( void *evt ) )
{
	PcmcFptr = (void (*)(PCMEVT *))fptr;
}

/*****************************************************************
				ＰＣＭＣＩＡイベント処理
*****************************************************************/
void PcmciaEvent( void )
{
	ss_monitor();
	pcm_busy = 1;
	if(PcmcEvent.sock[0].event != PcmcEvent.sock[0].last) {
		PcmciaHandler( &PcmcEvent );
		return;
	}
	pcm_busy = 0;
}

/*****************************************************************
				指定ソケットを擬似的にイジェクトする
*****************************************************************/
int PcmciaEjectFake( int psock )
{
	if( psock == 0  )  {
		pcm_busy = 1;
		fake[psock] = 1;
		pcm_busy = 0;
		return 0;
	}
	return -1;
}

/*====================================================================
				ＰＣＭＣＩＡソケットのハードテスト
 ====================================================================*/
int PcmciaHardTest( int sock )
{
	tuple_t tuple;
	cisparse_t parse;
	unsigned char buf[128];

	/* ＣＤ信号の検査 */
	if( !_cd_chk( sock )) {
		return -3;
	}
	/* アトリビュートメモリの検査 */
	tuple.Attributes = 0;
	tuple.DesiredTuple = CISTPL_FUNCID;
	tuple.TupleData = buf;
	tuple.TupleDataMax = 64;
	tuple.TupleOffset = 0;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -2;
	}
	return 0;
}

