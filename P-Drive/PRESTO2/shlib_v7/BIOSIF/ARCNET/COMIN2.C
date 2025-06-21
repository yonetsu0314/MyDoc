/*----------------------------------------------------------------------*
 * File name	: comin2.c	/ input from com20020			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/arcnet/RCS/comin2.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: comin2.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * TRAP中に割り込みを禁止していた処理をやめる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
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
/*	COM20020Bより連続したI/Oよりcharﾃﾞｰﾀを2回入力し、shortﾃﾞｰﾀ	*/
/*	に合成し戻り値として返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	contxt		: pointer to I/O context			*/
/*	reg		: rejister number				*/
/*									*/
/* 戻り値								*/
/*	long		: 0x0000????	･･･ input data			*/
/*			  -1L		･･･ fail			*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

long
_arcnet_inp2(
	void		*contxt,
	int		reg
	)
{
	long	err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (long)trapa_svc(VECTOR_ARCNET, 0x4, contxt, reg);
//	set_imask( imask_bak );
	return(err);
}
