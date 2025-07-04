/*----------------------------------------------------------------------*
 * File_name	: scuputc.c	/ m66230 putc								*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/m66230/RCS/scuputc.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuputc.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	m66230のﾄﾗﾝｽﾐｯﾀにﾃﾞｰﾀの書き込みを行う。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	data		: 書き込むﾃﾞｰﾀ											*/
/*																		*/
/* 戻り値																*/
/*	int		: 0															*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif						*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE	*/
#include	<dos.h>

int
_m66230_putc(
	int	ch,
	int	data						/* char data						*/
	)
{
	int	err;
	err = (int)trapa_svc(VECTOR_MPSC, 0x3, ch, data);
	return(err);
}
