/*----------------------------------------------------------------------*
 * File name	: sramrd.c	/ sram text read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramrd.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramrd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramrd.c 1.1 1995/08/24 13:23:48 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	id		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*buf		: string buffer					*/
/*	max 		: maximum length				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ ｽﾄﾘﾝｸﾞ無し				*/
/*			  n	･･･ character length			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	<custom\custom.h>		/*	"		*/
#include	"sramctl.h"			/* Show current		*/
#include	"sramctxt.h"			/*	"		*/

int
sram_read(
	int		id,			/* id number		*/
	unsigned char 	*buf,			/* string buffer pointer*/
	int		max			/* maxmum length	*/
	)
{
	struct sram_ctxt	*ctp;		/* pointer to context	*/
	unsigned char		*txtp;		/* poiter to text buff	*/

	if(id >= SRAM_DEV_MAX)			 	return(-1);
	if(max == 0)					return(0);
	ctp = &sram_ctxt_tbl[id];
	txtp = ctp->iobase +ctp->offset;
	if(ctp->offset+max > ctp->size)			return(-1);
	memcpy((void *)buf, (void *)txtp, (size_t)max);
	return(max);
}

