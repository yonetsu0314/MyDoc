/*======================================================================
 * File name    : cycwup.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/cycwup.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: cycwup.c $
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

typedef struct cycwup {
	ID		tskid;		/* 起床するタスクＩＤ		*/
	ID		cycid;		/* 定義する周期ハンドラＩＤ	*/
	unsigned int	cnt	;	/* 周期回数			*/
} CYCWUP;

#define	CYC_MAX		(16)
static struct cycwup dcyc[CYC_MAX] = {0};

/*----------------------------------------------------------------------*/
/*	cycwuphdr0							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	周期起床回数が指定されていない場合の周期起床ハンドラ。このハン	*/
/*	ドラにより指定タスクが起床される。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	tsk_id	: タスクＩＤ						*/
/*									*/
/* 戻り値								*/
/*	なし								*/
/*----------------------------------------------------------------------*/
static void
cycwuphdr0(
	VP_INT	tsk_id
	)
{
	wup_tsk( (ID)tsk_id);
}

/*----------------------------------------------------------------------*/
/*	cycwuphdr1							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	周期起床回数が指定されている場合の周期起床ハンドラ。このハンド	*/
/*	ラにより指定タスクが起床される。指定された起床回数を終えると、	*/
/*	周期ハンドラを削除する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	tsk_id	: タスクＩＤ						*/
/*									*/
/* 戻り値								*/
/*	なし								*/
/*----------------------------------------------------------------------*/
static void
cycwuphdr1(
	VP_INT	no
	)
{
	if(dcyc[(int)no].cnt) {
		wup_tsk( dcyc[(int)no].tskid);
		if(!(--dcyc[(int)no].cnt)) {
			dcyc[(int)no].cycid = (ID)0;
			dcyc[(int)no].tskid = (ID)0;
			del_cyc( dcyc[(int)no].cycid);
		}
	}
}

/*----------------------------------------------------------------------*/
/*	CYC_WUP								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	周期起床ハンドラの生成。周期起床ハンドラの起動が絶対時間で指定	*/
/*	されている場合は、相対時間に変更後、周期ハンドラを生成する。周	*/
/*	期起床の回数が設定されている時とそうでない時の周期起床ハンドラ	*/
/*	を分け、回数指定の場合は、登録テーブルの番号を示す値を、回数指	*/
/*	定のない場合（無限回）は、起床タスクのＩＤ番号を周期ハンドラに	*/
/*	渡す。								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	opt	: 絶対時間か相対時間かの指定				*/
/*	tsk_id	: タスクＩＤ						*/
/*	*pkt	: 周期起床情報のパケットを示すポインタ			*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
CYC_WUP(
	unsigned int 	opt,
	unsigned int	tsk_id,
	struct pkcwup 	*pkt
	)
{
	T_CCYC		pk_ccyc = { TA_HLNG|TA_PHS, 0, cycwuphdr0, 0, 0};
	SYSTIM		pk_systim;
	TMO		tim;
	ER		err;
	int		i;
	pk_ccyc.exinf = (VP_INT)tsk_id;
	pk_ccyc.cyctim = (RELTIM)pkt->interval;
	if(opt)	pk_ccyc.cycphs = (RELTIM)pkt->ltime;
	else {
		if(!(pkt->utime || pkt->ltime))	tim = (TMO)0;
		else {
			if(err = get_tim( &pk_systim), err)
						return( err_code( err));
			if(pkt->utime == pk_systim.utime)
				tim = (TMO)(pkt->ltime - pk_systim.ltime);
			else 	tim = (TMO)(~pk_systim.ltime+1 + pkt->ltime);
		}
		pk_ccyc.cycphs = (RELTIM)tim;
	}
	if(!pkt->count) {
		pk_ccyc.cycatr |= TA_STA;
		pk_ccyc.cychdr = (FP)cycwuphdr0;
		return( err_code( acre_cyc( &pk_ccyc)));
	}
	for(i=0; i<CYC_MAX; i++)
		if(!dcyc[i].cycid)	break;
	pk_ccyc.cychdr = (FP)cycwuphdr1;
/*	pk_ccyc.exinf = (VP_INT)i;	*/
/*	dcyc[i].tskid = tsk_id;		*/
	dcyc[i].cnt = pkt->count;
	return( err_code( sta_cyc( dcyc[i].cycid = acre_cyc( &pk_ccyc))));
}
