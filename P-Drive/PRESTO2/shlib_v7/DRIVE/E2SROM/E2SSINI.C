/*----------------------------------------------------------------------*
 * File name	: e2ssini.c	/ serial e2rom deriver initialize			*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2ssini.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2ssini.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2ssini.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/12 I/O操作関数の定義方法を変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		EEPROMへのアクセス環境を初期化									*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		siz_buf	: ﾘﾝｸﾞ･ﾊﾞｯﾌｧ･ｻｲｽﾞ										*/
/*																		*/
/* 戻り値																*/
/*		int		: 0	･･･ 正常終了										*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>								/* ANSI std			*/
#include	<stdlib.h>								/*	"				*/
#include	<rom\handle.h>							/*	"				*/
#include	<drive\iofsys.h>						/*	drive			*/

#define		STORAGE									/* 確保/外部参照	*/
#include	"e2sctxt.h"								/* Show current		*/
#include	"e2sctl.h"								/*	"				*/
#undef		STORAGE

int
e2s_sys_init(
	unsigned short	siz_buf
	)
{
	unsigned short	buf_max,						/* 最大ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
					mask;							/* mask bit pattern	*/

	if((buf_max = siz_buf) == 0)							return(-1);
	for(--buf_max, mask =(unsigned short)0x8000; mask; mask >>= 1) {
		if(buf_max & mask) {
			while(mask >>= 1)  buf_max |= mask;
			e2srom_hd.buf_max = buf_max;
			buf_max += 1;
			buf_max *= (1+BYTEACCESS);
			if((e2srom_hd.buff = (unsigned short *)malloc((size_t)buf_max)) == NULL)	return(-1);
		}
	}
	e2sr_io.fnc.out_handle_ce = _eesrom_handle_ce;
	e2sr_io.fnc.out_handle_di = _eesrom_handle_di;
	e2sr_io.fnc.out_handle_sk = _eesrom_handle_sk;
	e2sr_io.fnc.in_handle_do  = _eesrom_handle_do;
	iodrv_open[__CHR_STREAM_DEV-1] = (void *)e2s_romf_open;
	return(0);
}
