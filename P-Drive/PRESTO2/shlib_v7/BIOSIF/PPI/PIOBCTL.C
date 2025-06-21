/*----------------------------------------------------------------------*
 * File name	: piobctl.c	/ parallel i/o C bit control		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/ppi/RCS/piobctl.c 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: piobctl.c $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * _pio_bctlrd追加
 * 
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
/*	ﾊﾟﾗﾚﾙI/Oのport-C(0-7)をﾋﾞｯﾄ制御する。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	int	: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ						*/
/*	c	: 出力ﾃﾞｰﾀ						*/
/*									*/
/* 戻り値								*/
/*	int	: 0							*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

#define	PIO_MAX		(4)
static int	pdata[PIO_MAX];

int
_pio_bctl(
	int	devno,				/* device number	*/
	int	bit_position,			/* ﾋﾞｯﾄ位置		*/
	int	on_off				/* set:1,reset:0	*/
	)
{
	int	err;
	
	err = (int)trapa_svc(VECTOR_PIO, 0x3, devno, bit_position, on_off);
	if(err >= 0) pdata[devno] = err;
	return(err);
}


int
_pio_bctlrd(
	int	devno				/* device number	*/
	)
{
	return(pdata[devno]);
}


