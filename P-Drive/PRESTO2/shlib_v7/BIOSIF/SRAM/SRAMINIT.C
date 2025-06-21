/*----------------------------------------------------------------------*
 * File name	: sraminit.c	/ initialize sram mem i/o		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/sram/RCS/sraminit.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: sraminit.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/09/25 19:00
 * TRAP中に割り込みを禁止していた処理をやめる。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/19 16:00
 * TRAP中は割り込み禁止とする。
 * TRAP中にハードウェア割り込みが発生すると暴走する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/05 引き数の誤りを修正する。
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
/*	SRAMの物理的情報を得る。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dev_id		: device id					*/
/*									*/
/* 戻り値								*/
/*	const void *	: 物理情報への pointer	･･･ success		*/
/*			  (void  *)0		･･･ fail		*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

const void *
_sram_init(
	int		dev_id			/* board number		*/
	)
{
	void *err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (void *)trapa_svc(VECTOR_SRAM, 0x0, dev_id);
//	set_imask( imask_bak );
	return(err);
}
