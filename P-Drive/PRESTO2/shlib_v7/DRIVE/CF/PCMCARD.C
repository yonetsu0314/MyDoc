/*======================================================================
 * File name    : pcmcard.c
 * Original		: r:/c/astra-s/exp/rcs/pcmcard.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/cf/RCS/pcmcard.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmcard.c $
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
/*      ファイル名      : PCMCARD.C                                     */
/* ==================================================================== */
/*      機能分類        : ＰＣＭＣＩＡカードサービス			        */
/*                      : (ＰＣＭＣＩＡ準拠ではない)                    */
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
#include <stdlib.h>
//#include </itron4/kernel.h>/*020726*/
#include <kernel.h>/*020726*/
#include <sh7615\7615.h>
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#include "drive\pcmtpl.h"

	/*==========================================================
							構造体定義
	==========================================================*/
typedef struct client_callback_t {
	struct client_callback_t *next;
	void (*fptr)( int sock, int type, int event );
} client_callback_t;

	/*==========================================================
							静的変数定義
	==========================================================*/
static void (*IntFunc[1])( int no );		/* 割り込みハンドラ	*/
static int driver_id[1];
static client_callback_t callback;

	/*==========================================================
							大域変数定義
	==========================================================*/
	/*==========================================================
						プロトタイプ定義
	==========================================================*/

	/*==========================================================
				ＰＣＭＣＩＡドライバエントリの定義
	==========================================================*/
int pcmc_ata_driver( int sock, int type, PCMEVT *evt );

typedef int (*subfn_t)( int sock, int type, PCMEVT *evt );
const static subfn_t s_table[] = {
	(subfn_t)pcmc_ata_driver,
};
#define NFUNC (sizeof(s_table)/sizeof(subfn_t))


/*--------------------------------------------------------------------
		クライアントコールバック関数が登録されているかチェック
 --------------------------------------------------------------------*/
int is_register_client_callback( void *client )
{
	int i = 0;

	while( s_table[i] != NULL ) {
		if( s_table[i] == (subfn_t)client )
			return 0;
		i++;
	}
	return -1;
}


/*--------------------------------------------------------------------
			クライアントコールバック関数にイベントを渡す
 --------------------------------------------------------------------*/
static void client_callback_call( int sock, int type )
{
	client_callback_t *p = &callback;

	while( p != NULL ) {
		if( p->fptr != NULL )
			p->fptr( sock, GetDriverID(sock), type );
		p = p->next;
	}
}
/*--------------------------------------------------------------------
					各ドライバにイベントを渡す
 --------------------------------------------------------------------*/
static void carry_event_driver( int sock, int type, PCMEVT *evt )
{
	int i=0;

	for( i=0; s_table[i] != NULL; i++ ) {
		s_table[i]( sock, type, evt );
	}
}

/*--------------------------------------------------------------------
					カード検知の変化に対する処理
 --------------------------------------------------------------------*/
static void card_detect_ctl( int sock, PCMEVT *evt )
{
	if( evt->sock[sock].event & SS_DETECT ) {
		/* カードが差し込まれた時 */
		if( !(evt->sock[sock].last & SS_DETECT) ) {
			carry_event_driver( sock, SS_INSERTION, evt );
			client_callback_call( sock, SS_INSERTION );
		}
	}
	/* カードが抜かれた時 */
	else if( evt->sock[sock].last & SS_DETECT ) {
		client_callback_call( sock, SS_EJECTION );	/* 順序逆 */
		carry_event_driver( sock, SS_EJECTION, evt );
	}
}

/*--------------------------------------------------------------------
					ステータス変化に対する処理
 --------------------------------------------------------------------*/
static void card_stschg_ctl( int sock, PCMEVT *evt )
{
	if( evt->sock[sock].event & SS_STSCHG ) {
		if( !(evt->sock[sock].last & SS_STSCHG) ) {
			carry_event_driver( sock, SS_STSCHG, evt );
			client_callback_call( sock, SS_STSCHG );
		}
	}
}
/*--------------------------------------------------------------------
					バッテリ変化に対する処理
		　（一度発生したら復帰することはないこととする）
 --------------------------------------------------------------------*/
static void card_batchk_ctl( int sock, PCMEVT *evt )
{
	/* バッテリ警告状態 */
	if( evt->sock[sock].event & SS_BATWARN ) {
		if( !(evt->sock[sock].last & SS_BATWARN) ) {
			carry_event_driver( sock, SS_BATWARN, evt );
			client_callback_call( sock, SS_BATWARN );
		}
	}
	/* バッテリ死亡状態 */
	if( evt->sock[sock].event & SS_BATDEAD ) {
		if( !(evt->sock[sock].last & SS_BATDEAD) ) {
			carry_event_driver( sock, SS_BATDEAD, evt );
			client_callback_call( sock, SS_BATDEAD );
		}
	}
}
/*--------------------------------------------------------------------
					レディ／ビジーの変化に対する処理
 --------------------------------------------------------------------*/
static void card_ready_ctl( int sock, PCMEVT *evt )
{
	if( evt->sock[sock].event & SS_READY ) {
		if( !(evt->sock[sock].last & SS_READY) ) {
			carry_event_driver( sock, SS_READY, evt );
			client_callback_call( sock, SS_READY );
		}
	}
}
/*--------------------------------------------------------------------
					書込み禁止の変化に対する処理
 --------------------------------------------------------------------*/
static void card_wprot_ctl( int sock, PCMEVT *evt )
{
	if( evt->sock[sock].event & SS_WRPROT ) {
		/* ライトプロテクト状態 */
		if( !(evt->sock[sock].last & SS_WRPROT) ) {
			carry_event_driver( sock, SS_WRPROT, evt );
			client_callback_call( sock, SS_WRPROT );
		}
	}
	/* ライトプロテクト状態が解除 */
	else if( evt->sock[sock].last & SS_WRPROT ) {
		carry_event_driver( sock, SS_WRPROT_OFF, evt );
		client_callback_call( sock, SS_WRPROT_OFF );
	}
}

/*--------------------------------------------------------------------
					カード状態の変化に対する処理
 --------------------------------------------------------------------*/
static void CardCtl( PCMEVT *evt )
{
	int i = 0;

 	if( evt->sock[i].event != evt->sock[i].last ) {
		card_detect_ctl( i, evt );		/* カード検知		*/
		card_stschg_ctl( i, evt );		/* ステータス変化	*/
		card_batchk_ctl( i, evt );		/* バッテリチェック	*/
		card_ready_ctl( i, evt );		/* レディ／ビジー	*/
		card_wprot_ctl( i, evt );		/* 書込み禁止		*/
	} 
}

/*====================================================================
				ユーザーＰＣカードイベントハンドラのセット
 ====================================================================*/
int SetUserCardHandler( void (*fptr)( int sock, int type, int event ) )
{
	callback.fptr = fptr;
	callback.next = NULL;
	return 0;
}

/*====================================================================
						カード制御の初期化
 ====================================================================*/
void InitCardCtl( void )
{
	SetPcmciaHandler( (void (*)(void *))CardCtl );
}

/*====================================================================
						カード制御の終了
 ====================================================================*/
void ExitCardCtl( void )
{
	SetPcmciaHandler( NULL );
}

/*====================================================================
					ソケット０カード割り込み関数
 ====================================================================*/
void int0_pcmcia0( void )
{
	if( IntFunc[0] != NULL ) {
		IntFunc[0]( 0 );
	}
//	INTC.IRQCSR.BIT.IRQ1F &= 0;	/* 割り込みフラグクリア	*//*020729*/
}

#pragma interrupt( intPCM )
void intPCM( void )
{
	ent_int();
	int0_pcmcia0();
	ret_int();
}

/*====================================================================
				ＰＣＭＣＩＡ割り込みハンドラ関数のセット
 ====================================================================*/
void SetPcmciaIntHandler( int sock, int mode, void (*fptr)( int no ) )
{
	T_DINT dint_irq = { TA_HLNG, intPCM, 1 };
	def_inh(68, &dint_irq);	   /* Ready/Busy割込み level 9 *//*020805*/
	def_inh(69, &dint_irq);	   /* Ready/Busy割込み level 10 *//*020912 saka */
	if( fptr == NULL ) {
		if( sock == 0 ) {
			INTC.IPRC.WORD &= (unsigned short)0xf0ff;/*IRL5の優先レベルは固定(5)*//*020729*/
		}
	}        
	else {
		IntFunc[sock] == fptr;
		if( sock == 0 ) {
			def_inh(68, &dint_irq);	   /* Ready/Busy割込み level 9 *//*020729*/
			def_inh(69, &dint_irq);	   /* Ready/Busy割込み level 10 *//*020912 saka */
		}
	}
}

/*====================================================================
						カードのリセット
 ====================================================================*/
void ResetCard( int sock, unsigned char *config )
{
	*(config+CISREG_COR) = COR_SOFT_RESET;
//	dly_tsk((RELTIM)100/MSEC1);/*020801*/
	*(config+CISREG_COR) = 0x00;
	dly_tsk((RELTIM)25/MSEC);/*020801*/

}

/*====================================================================
					カードドライバＩＤの登録
 ====================================================================*/
void SetDriverID( int sock, int id )
{
	if( sock == 0 )
		driver_id[sock] = id;
}

/*====================================================================
					カードドライバＩＤの取得
 ====================================================================*/
int GetDriverID( int sock )
{
	if( sock == 0 )
		return driver_id[sock];
	return -1;
}


/*====================================================================
 ====================================================================*/
