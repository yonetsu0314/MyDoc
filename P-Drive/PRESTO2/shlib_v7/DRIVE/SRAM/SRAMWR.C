/*----------------------------------------------------------------------*
 * File name	: sramwr.c	/ sram text write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramwr.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramwr.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramwr.c 1.1 1995/08/24 13:23:54 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	id		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*buf		: string buffer					*/
/*	len		: number of byte				*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	"sramctl.h"			/* Show current		*/
#include	"sramctxt.h"			/*	"		*/

int
sram_write(
	int		id,			/* id number		*/
	unsigned char 	*buf,			/* string buffer pointer*/
	int		len			/* number of byte	*/
	)
{
	struct sram_ctxt	*ctp;		/* pointer to context	*/
	unsigned char 		*txtp;		/* poiter to text buff	*/

	if(id >= SRAM_DEV_MAX)				return(-1);
	ctp = &sram_ctxt_tbl[id];
	txtp = ctp->iobase +ctp->offset;
	if(ctp->offset+len > ctp->size)			return(-1);
	memcpy((void *)txtp, (void *)buf, (size_t)len);
	return(len);
}

