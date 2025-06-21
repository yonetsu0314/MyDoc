/*----------------------------------------------------------------------*
 * File name	: e2sini.c	/ e2rom deriver initialize		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2sini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2sini.c $
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
/*	EEPROMのヘッダー部の初期化。					*/
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
#include	<biosif\e2rom.h>		/*	bios		*/
#include	<drive\rxdefd.h>		/*  "	drive		*/
#include	<drive\iofsys.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"e2ctxt.h"			/* Show current		*/
#include	"e2ctl.h"			/*	"		*/
#undef		STORAGE

int
e2_sys_init(
	unsigned short	siz_buf
	)
{
	unsigned int	buf_max,		/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
			mask;			/* mask bit pattern	*/
	int		err;

	e2r_ds.ifp = (int *)&e2r_ds.acadr;
	if((buf_max = siz_buf) == 0)				return(-1);
	for(--buf_max, mask =0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			if((e2rom_hd.buff = malloc((size_t)(buf_max+1)))
					== NULL)		return(-1);
			e2rom_hd.buf_max = buf_max;
		}
	}
	if(err = CRE_SEM( FIFO, &e2_sem.acadr, (unsigned int)E2_SEM_ID,
			  E2_SEM_CNT))				return(-1);
	if(err = CRE_SEM( FIFO, &e2buf_sem.acadr, (unsigned int)E2BUF_SEM_ID,
			  buf_max+1))				return(-1);
	if(_e2rom_init(&e2rom_phy, &interval))			return(-1);
	if(init_e2rom_tsk())					return(-1);
	iodrv_open[__E2ROM_-1] = e2romf_open;
	return(0);
}
