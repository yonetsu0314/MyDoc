/*----------------------------------------------------------------------*
 * File name	: frwrite.c	/ flash rom file write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frwrite.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frwrite.c $
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
/*	*u_bufからFLASH ROMの書き込み用リングバッファにbyte転送後、	*/
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
#include	<biosif\from.h>		/* Show biosif		*/
#include	"frctl.h"			/* SHow current		*/

int
flashromf_write(
	int			file_n,		/* file number		*/
	unsigned char		u_buf[],	/* user buffer address	*/
	int			byte		/* number of byte	*/
	)
{
	int		err, len;
	unsigned int	j;

	len = byte;
	if(err = WAI_SEM( TIME_OUT, frbuf_sem.acadr,
				 (unsigned int)1, &fr_time_out)) return(-1);
	j= flashrom_hd.putp;
	do {
		*(flashrom_hd.buff+(j++)) = *(u_buf++);
		j &=  flashrom_hd.buf_max;
	} while(--byte);

	_disable();
	flashrom_hd.putp = j;
	flashrom_hd.flag |= WR_ON;
	_enable();
	if(err = WUP_TSK(frwr_tsk.acadr))			return(-1);
	return(len);
}
