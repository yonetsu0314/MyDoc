/*----------------------------------------------------------------------*
 * File name	: e2read.c	/ e2rom file read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2read.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2read.c $
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
/*	EEPROMからの要求byte数*u_bufへ読みだしを行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n	: file number						*/
/*	*u_buf	: user baffer address					*/
/*	byte	: byte data length					*/
/*									*/
/* 戻り値								*/
/*	int	: 要求byte数の残り					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<biosif\e2rom.h>		/* Show biosif		*/
#include	"e2ctl.h"			/* Show current		*/
#include	"e2ctxt.h"			/*	"		*/

int
e2romf_read(
	int			file_n,		/* file number		*/
	unsigned char		u_buf[],	/* user buffer address	*/
	int			byte		/* number of byte	*/
	)
{
	int			err;
	unsigned int		i;
	unsigned int		sem_cnt = 0;	/* 返却する資源数	*/

	while(e2rom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, e2buf_sem.acadr,
			      e2rom_hd.buf_max+1, VOID_TIME))	return(-1);
	    sem_cnt = e2rom_hd.buf_max+1;
	}
	i = e2rom_hd.e2f_p;
	do {
		*(u_buf++) = e2rom_phy[i++];
	} while(--byte);
	e2rom_hd.e2f_p = i;
	if(sem_cnt) {
	    if(err = SIG_SEM(e2buf_sem.acadr, sem_cnt))		return(-1);
	}
	return(byte);
}
