/*----------------------------------------------------------------------*
 * File name	: fropen.c	/ flash rom file open			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/fropen.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: fropen.c $
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
/*	FLASH ROMの資源の獲得を行い、ファイルをオープンする。		*/
/*	資源の返却はｸﾛｰｽﾞ関数により行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n		: file number					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: file size					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<drive\iofsys.h>		/*	"		*/
#include	"frctl.h"			/* Show current		*/

int
flashromf_open(
	int		file_n,
	int		mode
	)
{
	int		err;

	if(err = WAI_SEM( ENDLESS, fr_sem.acadr, FR_SEM_CNT,
			  VOID_TIME))				return(-1);
	flashrom_mode = mode;
	flashrom_hd.f_flag = F_OPEN;
	flashrom_hd.frf_p = 0;
	flashrom_hd.flag = 0;
	flashrom_hd.putp = 0;
	flashrom_hd.getp = 0;
	iodrv_read[__FLASHROM_-1] = flashromf_read;
	iodrv_write[__FLASHROM_-1] = flashromf_write;
	iodrv_seek[__FLASHROM_-1] = flashromf_seek;
	iodrv_close[__FLASHROM_-1] = flashromf_close;
	return((int)file_size[file_n]);
}
