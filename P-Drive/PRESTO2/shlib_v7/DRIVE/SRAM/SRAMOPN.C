/*----------------------------------------------------------------------*
 * File name	: sramopn.c	/ sram open				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramopn.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramopn.c 1.2 1995/10/09 15:49:18 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	SRAMの資源の獲得を行い、ファイルをオープンする。		*/
/*	資源の返却はｸﾛｰｽﾞ関数により行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	id		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/*	"		*/
#include	<rom\handle.h>			/* Show xlib		*/
#include	<custom\custom.h>		/* Show custom		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<rxif\rx116.h>			/*	"		*/
#include	"sramctxt.h"
#include	"sramctl.h"

int
sram_open(
	int	id,
	int	mode
	)
{
	int					err = 0;

	if(id >= SRAM_DEV_MAX)				return(-1);
	ADV_WAI_SEM(err, sram_nest[id], ENDLESS,sram_sem[id].acadr, VOID_TIME);
	if(err)						return(err|INT_MIN);
	sram_ctxt_tbl[id].offset = (unsigned short)0;
	sram_mode[id] = mode;
	iodrv_seek[__SRAM_-1] = sram_seek;
	iodrv_read[__SRAM_-1] = sram_read;
	iodrv_write[__SRAM_-1] = sram_write;
	iodrv_close[__SRAM_-1] = sram_close;
	return(0);
}
