/*----------------------------------------------------------------------*
 * File name	: icuena.c	/ enable icu				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/icu/RCS/icuena.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: icuena.c $
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
/*	指定ICUのﾁｬﾈﾙのﾏｽｸ解除を行う。patternの"1"となっているﾋﾞｯﾄ	*/
/*	が割り込み可能となる。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dev_no		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*	pattern		: ICUﾏｽｸ･ﾋﾞｯﾄﾊﾟﾀｰﾝ				*/
/*									*/
/* 戻り値								*/
/*	int		: bit0-7	･･･ ﾘｾｯﾄ後のﾋﾞｯﾄ･ﾊﾟﾀｰﾝ		*/
/*			: bit8-15	･･･ ﾘｾｯﾄ前のﾋﾞｯﾄ･ﾊﾟﾀｰﾝ		*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

int
_ena_icu(
	int	dev_no,				/* device number	*/
	int	pattern				/* ICUﾏｽｸ･ﾋﾞｯﾄﾊﾟﾀｰﾝ	*/
	)
{
	int	err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (int)trapa_svc(VECTOR_ICU, 0x1, dev_no, pattern);
//	set_imask( imask_bak );
	return(err);
}
