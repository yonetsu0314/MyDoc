/*----------------------------------------------------------------------*
 * File name	: sramsini.c	/ sram system initialize		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramsini.c 1.1 1995/08/24 13:23:50 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｾﾏﾌｫ及びBIOSｺﾝﾃｷｽﾄの生成を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	id		: ﾃﾞﾊﾞｲｽIDﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			: -1 ･･･異常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<biosif\sram.h>			/*	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\sramdrv.h>		/*	"		*/
#include	<drive\rxdefd.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"sramctxt.h"			/* Show current		*/
#include	"sramctl.h"			/*	"		*/
#undef		STORAGE

int
sram_sys_init(
	unsigned short		id
	)
{
	struct sram_ctxt	*ctp;		/* pointer to contxt	*/
	const struct srampinf	*phy_inf;	/* & to physical info	*/
	int			err;

	if(id > SRAM_DEV_MAX)		return(-1);
	ctp = &sram_ctxt_tbl[id];
	if(err = CRE_SEM( FIFO, &sram_sem[id].acadr,sram_sem_id[id], (unsigned int)1))
		return(-1);
	if(err = WAI_SEM( ENDLESS, sram_sem[id].acadr,(unsigned int)1, VOID_TIME))
		return(-1);
	if((phy_inf = _sram_init((int)id)) == NULL)
		return(-1);
	ctp->iobase = phy_inf->iobase;
	ctp->size = phy_inf->mem_size;
	ctp->offset = (unsigned short)0;
	iodrv_open[__SRAM_-1] = sram_open;
	return(0);
}
