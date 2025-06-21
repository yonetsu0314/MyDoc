/*======================================================================
 * File name    : cretsk.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/cretsk.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: cretsk.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
*/
#include	<rxif\rx116.h>			/* Show include		*/
#include	<kernel.h>
#include	"errcode.h"


/*----------------------------------------------------------------------*/
/*	CRE_TSK								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	タスクの生成。タスク生成情報のパケットにタスクの属性、タスクと	*/
/*	する関数へのポインタ、タスク起動時優先度、スタックサイズをセッ	*/
/*	トする。また、タスクのアセスアドレスをタスクＩＤとする。システ	*/
/*	ムコールの戻り値（エラーコード）をコンバートして返す。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*tskaa	: タスクのアクセスアドレスを示すポインタ		*/
/*	tsk_id	: タスクＩＤ						*/
/*	*pkt	: タスク生成情報のパケットを示すポインタ		*/
/*									*/
/*	[kernel.h]							*/
/*	typedef struct v4_t_ctsk					*/
/*	{								*/
/*		ATR tskatr;		task attributes			*/
/*		VP_INT exinf;		extended information		*/
/*		FP task;		task start address		*/
/*		PRI itskpri;		initial task priority		*/
/*		SIZE stksz;		stack size			*/
/*		VP stk;			stack top address		*/
/*		B *name;		task name pointer		*/
/*	} V4_T_CTSK;							*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
CRE_TSK(
	unsigned int 	*tskaa,
	unsigned int	tsk_id,
	struct pktsk 	*pkt
	)
{
	T_CTSK		pk_ctsk = { TA_HLNG, NULL, NULL, 0, (SIZE)0, NULL, ""};
	pk_ctsk.task = (FP)pkt->tsk_adr;
	pk_ctsk.itskpri = (PRI)pkt->priority;
	pk_ctsk.stksz = (SIZE)pkt->stk_size;
	*tskaa = tsk_id;
	return( err_code( cre_tsk( (ID)tsk_id, &pk_ctsk)));
}
