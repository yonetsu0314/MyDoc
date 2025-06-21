/*----------------------------------------------------------------------*
 * File name	: e2sopen.c		/ serial e2rom file open				*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2sopen.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2sopen.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2sopen.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明																	*/
/*		EE-PROMファイルをオープンする									*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		file_n		: file number										*/
/*		mode		: ｱｸｾｽ･ﾓｰﾄﾞ											*/
/*																		*/
/* 戻り値																*/
/*		int			: file size											*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>						/* Show ROM				*/
#include	<drive\iofsys.h>					/*	"	drive			*/
#include	"e2sctl.h"							/* Show current			*/
#include	"e2sctxt.h"							/*	"					*/

static const unsigned short		shf_tbl[] = { (unsigned short)SHF_93C46, (unsigned short)SHF_93C56 };

int
e2s_romf_open(
	int		file_n,
	int		mode
	)
{
	e2srom_mode = mode;
	e2srom_hd.f_flag = (unsigned short)F_OPEN;
	e2srom_hd.e2f_p = (unsigned short)0;
	e2srom_hd.flag = (unsigned short)0;
	e2srom_hd.putp = (unsigned short)0;
	e2srom_hd.getp = (unsigned short)0;
	iodrv_read[__CHR_STREAM_DEV-1] = (void *)e2s_romf_read;
	iodrv_write[__CHR_STREAM_DEV-1] = (void *)e2s_romf_write;
	iodrv_seek[__CHR_STREAM_DEV-1] = (void *)e2s_romf_seek;
	iodrv_close[__CHR_STREAM_DEV-1] = (void *)e2s_romf_close;
	e2sr_io.shf_cnt = shf_tbl[file_n];
	return((int)file_ssize[file_n]);
}
