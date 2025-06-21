/*----------------------------------------------------------------------*
 * File name	: inireg.c	/ initialize register			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/init/RCS/inireg.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: inireg.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	レジスタの設定を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void					*/
/*									*/
/* 戻り値								*/
/*	int	: 0							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

int
_init_registers(void)
{
	int	err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (int)trapa_svc(VECTOR_INIT, 0x2);
//	set_imask( imask_bak );
	return(err);
}
