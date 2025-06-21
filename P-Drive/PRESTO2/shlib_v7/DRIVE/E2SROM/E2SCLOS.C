/*----------------------------------------------------------------------*
 * File name	: e2sclos.c		/ serial e2rom file close				*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2sclos.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2sclos.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2sclos.c $
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
/*		ファイルをクローズする											*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		file_n	: file number											*/
/*																		*/
/* 戻り値																*/
/*		int		: error													*/
/*----------------------------------------------------------------------*/

#include	"e2sctl.h"							/* Show current			*/

int
e2s_romf_close(
	int		file_n
	)
{
	e2srom_mode = -1;
	e2srom_hd.f_flag = (unsigned short)F_CLOSE;
	e2srom_hd.e2f_p = (unsigned short)0;
	return(0);
}
