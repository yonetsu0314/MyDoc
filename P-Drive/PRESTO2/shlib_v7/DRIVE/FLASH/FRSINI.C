/*----------------------------------------------------------------------*
 * File name	: frsini.c	/ flash rom deriver initialize		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frsini.c $
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
/*	FLASH ROMのヘッダー部の初期化。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	siz_buf	: ﾘﾝｸﾞ･ﾊﾞｯﾌｧ･ｻｲｽﾞ					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<biosif\from.h>			/*	bios		*/
#include	<drive\rxdefd.h>		/*  "	drive		*/
#include	<drive\iofsys.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"frctxt.h"			/* Show current		*/
#include	"frctl.h"			/*	"		*/
#undef		STORAGE

int
flash_sys_init(
	unsigned short	siz_buf
	)
{
	unsigned int	buf_max,		/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
			mask;			/* mask bit pattern	*/
	int		err;

	fr_ds.ifp = (int *)&fr_ds.acadr;
	if((buf_max = siz_buf) == 0)				return(-1);
	for(--buf_max, mask =0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((flashrom_hd.buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			flashrom_hd.buf_max = buf_max;
		}
	}
	if(err = CRE_SEM( FIFO, &fr_sem.acadr, (unsigned int)FR_SEM_ID,
			  FR_SEM_CNT))				return(-1);
	if(err = CRE_SEM( FIFO, &frbuf_sem.acadr, (unsigned int)FRBUF_SEM_ID,
			  (unsigned int)1))			return(-1);
	if(_flashrom_init(&flashrom_phy, &interval))		return(-1);
	if(init_flashrom_tsk())					return(-1);
	iodrv_open[__FLASHROM_-1] = flashromf_open;
	return(0);
}
