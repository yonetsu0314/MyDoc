/*======================================================================
 * File name    : setflg.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	システムコールコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/setflg.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: setflg.c $
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
/*	SET_FLG								*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	イベントフラグのセットとクリア。イベントフラグのセット方法の指	*/
/*	定により、イベントフラグのセットとクリアのシステムコールを実行	*/
/*	する。システムコールの戻り値（エラーコード）をコンバートして返	*/
/*	す。								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	opt	: イベントフラグのセット方法の指定			*/
/*	*flg	: イベントフラグの現在の内容を返すエリアへのポインタ	*/
/*	flgaa	: イベントフラグのアクセスアドレス			*/
/*	ptn	: イベントフラグのセットビットパターン			*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -n	･･･ error code					*/
/*----------------------------------------------------------------------*/
int
SET_FLG(
	unsigned int	opt,
	unsigned int	*flg,
	unsigned int	flgaa,
	unsigned int	ptn
	)
{
	T_RFLG		pk_rflg;
	FLGPTN		setptn;
	ER		err;
	 err = ref_flg( (ID)flgaa, &pk_rflg);
	if(err < E_OK)				return( err_code( err));
	*flg = pk_rflg.flgptn;
	setptn = (FLGPTN)ptn;
	switch(opt) {
	case EVF_SET_SET:
		return( err_code( set_flg( (ID)flgaa, setptn)));
	case EVF_SET_RES:
		return( err_code( clr_flg( (ID)flgaa, setptn)));
	case EVF_SET_REPL:
		err = clr_flg( (ID)flgaa, (FLGPTN)0);
		if(err < E_OK)			return( err_code( err));
		return( err_code( set_flg( (ID)flgaa, setptn)));
	case EVF_SET_EXOR:
		setptn ^= pk_rflg.flgptn;
		err = clr_flg( (ID)flgaa, setptn);
		if(err < E_OK)			return( err_code( err));
		return( err_code( set_flg( (ID)flgaa, setptn)));
	default:
		return(-1);
	}
}
