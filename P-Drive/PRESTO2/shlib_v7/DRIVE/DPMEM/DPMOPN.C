/*----------------------------------------------------------------------*
 * File name	: dpmopn.c	/ dpmem open				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmopn.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmopn.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmopn.c 1.1 1993/10/20 20:09:50 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	DP-SRAMをオープンする。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ					*/
/*	mode		: ｱｸｾｽ･ﾓｰﾄﾞ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

#include	<rom\handle.h>			/* Show xlib		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\icu.h>			/* Show biosif		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\dpmdrv.h>		/*	"		*/
#include	"dpmctxt.h"
#include	"dpmctl.h"

int
dpm_open(
	int		channel,
	int		mode
	)
{
	struct dpm_ctxt		*ctp;		/* pointer to contxt	*/
	unsigned char		*rcp;		/* pointer to rd command*/
	volatile int		cmd;
	int			dn, ptn;

	if(channel >= DPM_DEV_MAX)				  return(-1);
	ctp = &dpm_ctxt_tbl[channel];
	rcp = ctp->iobase+ctp->rdcmd;
	_dis_icu(dn = dpm_icu_inf[channel].uc[1],
		ptn = dpm_icu_inf[channel].uc[0]);
	if(ctp->mem_type == BYTE_M) {
		cmd = *rcp;
		while(verify_putc((unsigned char)CMD_ACK, (void *)rcp));
	}
	else if(ctp->mem_type == WORD_M) {
		cmd = *WD_PTR rcp;
		while(verify_puti((unsigned int)CMD_ACK, (void *)rcp));
	}
	dpm_mode[channel] = mode;
	iodrv_read[__DPMEM_-1] = dpm_read;
	iodrv_write[__DPMEM_-1] = dpm_write;
	iodrv_close[__DPMEM_-1] = dpm_close;
	_ena_icu(dn, ptn);
	return(0);
}
