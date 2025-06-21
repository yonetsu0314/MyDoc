/*----------------------------------------------------------------------*
 * File name	: frclos.c	/ flash rom file close			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frclos.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frclos.c $
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
/*	ファイルをクローズしFLASH ROMの資源の返却を行う。		*/
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
#include	"frctl.h"			/* Show current		*/

int
flashromf_close(
	int		file_n
	)
{
	int		err;

	while(flashrom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, frbuf_sem.acadr,
			      	(unsigned int)1, VOID_TIME))	return(-1);
	    if(err = SIG_SEM( frbuf_sem.acadr, (unsigned int)1)) return(-1);
	}
	flashrom_mode = -1;
	flashrom_hd.f_flag = F_CLOSE;
	flashrom_hd.frf_p = 0;
	if(err = SIG_SEM( fr_sem.acadr, FR_SEM_CNT))	 	return(-1);
	return(0);
}
