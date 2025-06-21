/*----------------------------------------------------------------------*
 * File name	: e2write.c	/ e2rom file write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2write.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2write.c $
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
/*	*u_bufからEEPROMの書き込み用リングバッファにbyte転送後、	*/
/*	書き込みをイネーブルにする。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n	: file number						*/
/*	*u_buf	: user baffer address					*/
/*	byte	: byte data length					*/
/*									*/
/* 戻り値								*/
/*	int	:  N	･･･ write complete				*/
/*	    	  -1	･･･ error					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show biosif		*/
#include	<biosif\e2rom.h>		/* Show biosif		*/
#include	"e2ctl.h"			/* SHow current		*/

int
e2romf_write(
	int			file_n,		/* file number		*/
	unsigned char		u_buf[],	/* user buffer address	*/
	int			byte		/* number of byte	*/
	)
{
	int		err, len;
	unsigned int	j;

	if(err = WAI_SEM( TIME_OUT, e2buf_sem.acadr, (unsigned int)(len =byte),
			  &e2_time_out))			return(-1);
	j= e2rom_hd.putp;
	do {
		*(e2rom_hd.buff+(j++)) = *(u_buf++);
		j &=  e2rom_hd.buf_max;
	} while(--byte);

	_disable();
	e2rom_hd.putp = j;
	e2rom_hd.flag |= WR_ON;
	_enable();
	if(err = WUP_TSK(e2wr_tsk.acadr))			return(-1);
	return(len);
}
