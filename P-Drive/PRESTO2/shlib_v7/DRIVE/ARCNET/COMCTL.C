/*----------------------------------------------------------------------*
 * File name	: comctl.c	/ ARCNET control
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcnet/RCS/comctl.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: comctl.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comctl.c 1.1 1995/11/07 17:13:10 ryoji
 *----------------------------------------------------------------------*/
/*		(C) Copyright 1994, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＡＲＣＮＥＴチップに対するオペレーションなど			*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<assert.h>			/*	"		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
/*#include	<custom\custom.h>*/		/*	"		*/
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"comctl.h"			/* Show current		*/
#include	"comctxt.h"			/* Show current		*/

/*----------------------------------------------------------------------*/
/*	read_diagr							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	診断ﾚｼﾞｽﾀの内容を調べる。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*pc	: pointer to context					*/
/*	mask 	: 調べようとするﾋﾞｯﾄ					*/
/*	res	: 読みだし後ﾘｾｯﾄするﾋﾞｯﾄ				*/
/*									*/
/* 戻り値								*/
/*	int	: 読みだしたﾃﾞｰﾀ					*/
/*----------------------------------------------------------------------*/

int
read_diagr(
	struct com_ctxt		*pc,
	unsigned char		mask,
	unsigned char		res
	)
{
	int			i;

	disable_interrupt();
	pc->diagr |= (unsigned char)(i = _arcnet_inp(pc, DIAGNR) & mask);
	pc->diagr &= (unsigned char)(~res);
	enable_interrupt();
	return(i);
}


/*----------------------------------------------------------------------*/
/*	arc_stat							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	各種状態取得							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*	cmd	: operating command					*/
/*									*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/

int
arc_stat(
	int		devno,
	enum statspc	cmd
	)
{
	int			ret = 0;	/* return value		*/

	switch ( cmd ) {
		case	GET_RECON:
			ret = com_hd_tbl[devno].recon_cntr;
			break;
		case	TXPND_STAT:
			ret = check_tx_pend(devno);
			break;
	}
	return (ret);
}

/*----------------------------------------------------------------------*/
/*	arc_ctrl							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARCNETﾁｯﾌﾟへのｵﾍﾟﾚｰｼｮﾝ						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno	: device number						*/
/*	cmd	: operating command					*/
/*									*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/

int
arc_ctrl(
	int		devno,
	enum cmdspc	cmd
	)
{
	struct com_ctxt		*pc;		/* pointer to context	*/
	int			err = 0;
	int			ret = 0;	/* return value		*/

	pc = &com_ctxt_tbl[devno];
	ADV_WAI_SEM(err, com_nest[devno], TIME_OUT,
		    com_sem[devno].acadr, &com_time_out);
	assert(!err);
	if (err)					return(-1);
	switch ( cmd ) {
		case	CLR_RX:
			_arcnet_outp(pc, COMMR, CLRRX_PEND);
			break;
		case	CLR_TX:
			_arcnet_outp(pc, COMMR, CLRTX_PEND);
			break;
		default:
			ret = -1;
			break;
	}
	ADV_SIG_SEM(err, com_nest[devno], com_sem[devno].acadr);
	assert(!err);
	if (err)					return(-1);

	return (ret);
}
