/*----------------------------------------------------------------------*
 * File name	: e2clos.c	/ e2rom file close			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2clos.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2clos.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	ファイルをクローズしEEPROMの資源の返却を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n		: file number					*/
/*									*/
/* 戻り値								*/
/*	int		: error						*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"e2ctl.h"			/* Show current		*/

int
e2romf_close(
	int		file_n
	)
{
	int		err;

	while(e2rom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, e2buf_sem.acadr,
			      e2rom_hd.buf_max+1, VOID_TIME))	return(-1);
	    if(err = SIG_SEM( e2buf_sem.acadr,
			      e2rom_hd.buf_max+1))		return(-1);
	}
	e2rom_mode = -1;
	e2rom_hd.f_flag = F_CLOSE;
	e2rom_hd.e2f_p = 0;
	if(err = SIG_SEM( e2_sem.acadr, E2_SEM_CNT)) return(-1);
	return(0);
}
