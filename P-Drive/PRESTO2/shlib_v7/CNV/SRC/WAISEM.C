/*======================================================================
 * File name    : waisem.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/waisem.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: waisem.c $
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
/*	WAI_SEM								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	セマフォ資源の獲得。セマフォの待ち方の指定によるタイムアップ有	*/
/*	り無しによりシステムコールを選択し、獲得資源数回実行する。シス	*/
/*	テムコールの戻り値（エラーコード）をコンバートして返す。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	opt	: セマフォの待ち方の指定				*/
/*	semaa	: セマフォのアクセスアドレス				*/
/*	cnt	: セマフォの獲得資源数					*/
/*	*tm	: セマフォの待ち時間					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
WAI_SEM(
	unsigned int	opt,
	unsigned int	semaa,
	unsigned int	cnt,
	long  		*tm
	)
{
	ER		err;
	SYSTIM		pk_systim, ttmp;
	TMO		tim;
	if(opt & TIME_OUT) {
		if(cnt == 1)
			return( err_code( twai_sem( (ID)semaa, (TMO)*tm)));
		if(err = get_tim( &pk_systim), err)
						return( err_code( err));
		tim = (TMO)*tm;
		for(; cnt; cnt--){
			if(err = twai_sem( (ID)semaa, tim), err)
						return( err_code( err));
			ttmp.utime = pk_systim.utime;
			ttmp.ltime = pk_systim.ltime;
			if(err = get_tim( &pk_systim), err)
						return( err_code( err));
			if(ttmp.utime == pk_systim.utime)
				tim -= (TMO)(pk_systim.ltime - ttmp.ltime);
			else 
				tim -= (TMO)(pk_systim.ltime + ~ttmp.ltime+1);
			if(tim <= 0)		return( err_code( E_TMOUT));
		}
		return( err_code( err));
	} else {
		if(cnt == 1)	return( err_code( wai_sem( (ID)semaa)));
		for(; cnt; cnt--){
			if(err = wai_sem( (ID)semaa), err)
						return( err_code( err));
		}
		return( err_code( err));
	}
}
