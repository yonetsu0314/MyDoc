/*----------------------------------------------------------------------*
 * File name	: e2seek.c	/ e2rom file seek			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2seek.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2seek.c $
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
/* 説明									*/
/*	ファイルポインタの移動。origin:が0の場合ファイルポインタ	*/
/*	=offset、1の場合ファイルポインタ+offset。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n	: file number						*/
/*	offset	: offset address					*/
/*	origin	: origin						*/
/*									*/
/* 戻り値								*/
/*	int	: offset address					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\e2rom.h>		/* Show biosif		*/
#include	"e2ctl.h"			/* Show current		*/

int
e2romf_seek(
	int		file_n,			/* file number		*/
	int		offset,			/* offset address	*/
	int		origin			/* origin 0/1		*/
	)
{
	int		err;
	unsigned int	sem_cnt = 0;		/* 返却する資源数	*/

	while(e2rom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, e2buf_sem.acadr,
			      e2rom_hd.buf_max+1, VOID_TIME))	return(-1);
	    sem_cnt = e2rom_hd.buf_max+1;
	}
	if(origin == 0)	      e2rom_hd.e2f_p = offset;
	else if(origin == 1)  e2rom_hd.e2f_p += offset;
	if(sem_cnt) {
	    if(err = SIG_SEM(e2buf_sem.acadr, sem_cnt))		return(-1);
	}
	return(offset);
}
