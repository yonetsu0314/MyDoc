/*----------------------------------------------------------------------*
 * File name	: icuhnt.c	/ hunt icu device number		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/icu/RCS/icuhnt.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: icuhnt.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/25 19:00
 * TRAP中に割り込みを禁止していた処理をやめる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/09/19 16:00
 * TRAP中は割り込み禁止とする。
 * TRAP中にハードウェア割り込みが発生すると暴走する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	引き数の関数ポインタを、割り込みベクター･テーブルよりICUの	*/
/*	どのデバイスのチャネルに接続されているかを捜し結果を返す。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	funcp	: 割り込みﾊﾝﾄﾞﾗへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: N	･･･ (ﾃﾞﾊﾞｲｽNO << 8)+(ﾁｬﾈﾙのﾋﾞｯﾄ･ﾊﾟﾀｰﾝ)		*/
/*		  -1	･･･ 該当なし					*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<biosif\icu.h>			/*	"		*/
#include	<dos.h>

int
_icu_dev_hunt(
	V_FUNC		funcp			/* ﾊﾝﾄﾞﾗへのﾎﾟｲﾝﾀ	*/
	)
{
	const struct vects	*p;		/* ﾍﾞｸﾀｰﾃｰﾌﾞﾙへのﾎﾟｲﾝﾀ	*/
	int			err;
//	int			imask_bak;

	if(funcp == NULL)				return(-1);
	p = &vector_strc;

//	imask_bak = get_imask();
//	set_imask(15);
	err = (int)trapa_svc(VECTOR_ICU, 0x3, funcp, p);
//	set_imask( imask_bak );
	return(err);
}
