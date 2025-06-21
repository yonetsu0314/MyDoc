/*----------------------------------------------------------------------*
 * File name	: e2open.c	/ e2rom file open			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2open.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2open.c $
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
/*	EEPROMの資源の獲得を行い、ファイルをオープンする。		*/
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
#include	"e2ctl.h"			/* Show current		*/

int
e2romf_open(
	int		file_n,
	int		mode
	)
{
	int		err;

	if(err = WAI_SEM( ENDLESS, e2_sem.acadr, E2_SEM_CNT,
			  VOID_TIME))				return(-1);
	e2rom_mode = mode;
	e2rom_hd.f_flag = F_OPEN;
	e2rom_hd.e2f_p = 0;
	e2rom_hd.flag = 0;
	e2rom_hd.putp = 0;
	e2rom_hd.getp = 0;
	iodrv_read[__E2ROM_-1] = e2romf_read;
	iodrv_write[__E2ROM_-1] = e2romf_write;
	iodrv_seek[__E2ROM_-1] = e2romf_seek;
	iodrv_close[__E2ROM_-1] = e2romf_close;
	return((int)file_size[file_n]);
}
