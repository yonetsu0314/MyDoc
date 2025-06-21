/*----------------------------------------------------------------------*
 * File name	: frread.c	/flash rom file read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frread.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frread.c $
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
/*	 FLASH ROMからの要求byte数*u_bufへ読みだしを行う。		*/
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
#include	<biosif\from.h>			/* Show biosif		*/
#include	"frctl.h"			/* Show current		*/
#include	"frctxt.h"			/*	"		*/

int
flashromf_read(
	int			file_n,		/* file number		*/
	unsigned char		u_buf[],	/* user buffer address	*/
	int			byte		/* number of byte	*/
	)
{
	int			err;
	unsigned int		i;
	unsigned int		sem_cnt = 0;	/* 返却する資源数	*/

	while(flashrom_hd.flag & WR_ON) {
	    if(err = WAI_SEM( ENDLESS, frbuf_sem.acadr,
			      	(unsigned int)1, VOID_TIME))	return(-1);
	    sem_cnt = flashrom_hd.buf_max+1;
	}
	i = flashrom_hd.frf_p;
	do {
		*(u_buf++) = flashrom_phy[i++];
	} while(--byte);
	flashrom_hd.frf_p = i;
	if(sem_cnt) {
	    if(err = SIG_SEM(frbuf_sem.acadr, (unsigned int)1))	return(-1);
	}
	return(byte);
}
