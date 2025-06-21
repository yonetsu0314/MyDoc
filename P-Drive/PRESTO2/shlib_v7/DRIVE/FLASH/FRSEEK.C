/*----------------------------------------------------------------------*
 * File name	: frseek.c	/ flash rom file seek			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frseek.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frseek.c $
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
#include	<biosif\from.h>		/* Show biosif		*/
#include	"frctl.h"			/* Show current		*/

int
flashromf_seek(
	int		file_n,			/* file number		*/
	int		offset,			/* offset address	*/
	int		origin			/* origin 0/1		*/
	)
{
	int		err;
	unsigned int	sem_cnt = 0;		/* 返却する資源数	*/

	while(flashrom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, frbuf_sem.acadr,
			      	(unsigned int)1, VOID_TIME))	return(-1);
	    sem_cnt = flashrom_hd.buf_max+1;
	}
	if(origin == 0)	      flashrom_hd.frf_p = offset;
	else if(origin == 1)  flashrom_hd.frf_p += offset;
	if(sem_cnt) {
	    if(err = SIG_SEM(frbuf_sem.acadr, (unsigned int)1))	return(-1);
	}
	return(offset);
}
