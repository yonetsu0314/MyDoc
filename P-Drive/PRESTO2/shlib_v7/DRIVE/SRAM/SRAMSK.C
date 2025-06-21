/*----------------------------------------------------------------------*
 * File name	: sramsk.c	/ sram file seek			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramsk.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramsk.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramsk.c 1.1 1995/08/24 13:23:52 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/* 説明									*/
/*	ファイルポインタの移動。origin:が0の場合ファイルポインタ	*/
/*	=offset、1の場合ファイルポインタ+offset。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	file_n	: file number						*/
/*	offset	: offset address					*/
/*	origin	: origin						*/
/*									*/
/* 戻り値								*/
/*	int	: offset address					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/*	"		*/
#include	"sramctl.h"			/* Show current		*/
#include	"sramctxt.h"			/*	"		*/

int
sram_seek(
	int		id,			/* file number		*/
	int		offset,			/* offset address	*/
	int		origin			/* origin 0/1		*/
	)
{
	if(id >= SRAM_DEV_MAX)				  return(-1);
	if(origin == 0)	      sram_ctxt_tbl[id].offset  = (unsigned short)offset;
	else if(origin == 1)  sram_ctxt_tbl[id].offset += (unsigned short)offset;
	return(offset);
}
