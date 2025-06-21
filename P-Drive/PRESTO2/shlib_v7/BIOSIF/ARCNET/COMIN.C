/*----------------------------------------------------------------------*
 * File name	: comin.c	/ input from com20020			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/arcnet/RCS/comin.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: comin.c $
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
/*	COM20020Bよりcharﾃﾞｰﾀを入力する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	contxt		: pointer to I/O context			*/
/*	reg		: rejister number				*/
/*									*/
/* 戻り値								*/
/*	int		: 0x00??	･･･ input data			*/
/*			  -1		･･･ fail			*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

int
_arcnet_inp(
	void		*contxt,
	int		reg
	)
{
	int	err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (int)trapa_svc(VECTOR_ARCNET, 0x3, contxt, reg);
//	set_imask( imask_bak );
	return(err);
}
