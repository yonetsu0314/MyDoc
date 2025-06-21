/*----------------------------------------------------------------------*
 * File name	: e2sseek.c		/ serial e2rom file seek				*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2sseek.c 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2sseek.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2sseek.c $
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
/*		ファイルポインタの移動。origin:が0の場合ファイルポインタ		*/
/*		=offset、1の場合ファイルポインタ+offset。						*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		file_n	: file number											*/
/*		offset	: offset address										*/
/*		origin	: origin												*/
/*																		*/
/* 戻り値																*/
/*		int		: offset address										*/
/*----------------------------------------------------------------------*/

#include	"e2sctl.h"							/* Show current			*/

int
e2s_romf_seek(
	int		file_n,								/* file number			*/
	int		offset,								/* offset address		*/
	int		origin								/* origin 0/1			*/
	)
{
	int		i = offset/2;
//	int		i = offset;

	if(origin == 0)	      e2srom_hd.e2f_p = (unsigned short)i;
	else if(origin == 1)  e2srom_hd.e2f_p += (unsigned short)i;
	return(i*2);
//	return(i);
}
