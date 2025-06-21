/*----------------------------------------------------------------------*
 * File name	: pioprd.c	/ read icu port address			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/ppi/RCS/pioprd.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: pioprd.c $
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
/*	ICUに接続されているﾊﾟﾗﾚﾙI/Oのﾎﾟｰﾄ･ｱﾄﾞﾚｽ&ﾏｽｸ･ﾋﾞｯﾄﾊﾟﾀｰﾝへの	*/
/*	ﾎﾟｲﾝﾀｰを返す。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	int		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*	channel		: ICUﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	const void *	: table address					*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

const void *
_pio_icu_port(
	int	devno,				/* device number	*/
	int	channel
	)
{
	void	*err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (void *)trapa_svc(VECTOR_PIO, 0x4, devno, channel);
//	set_imask( imask_bak );
	return(err);
}
