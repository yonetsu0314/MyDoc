/*----------------------------------------------------------------------*
 * File name	: afvtsk.c	/ afv adjustment task			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/afvtsk.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: afvtsk.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/afvtsk.c 1.1 1970/01/01
 *Log: afvtsk.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<limits.h>			/*	"		*/
#include	<dos.h>				/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	"rxdef.h"			/* Show APL		*/
#include	"madif.h"			/*	"		*/
#include	"admtxt.h"			/* current		*/
/*#include	"afvsmp.h"				"		*/

static union aap	afv_tsk = { 0L };
volatile unsigned long	afv_ptn = 0L;		/* afv adjust ptattern	*/
void	abort(void);
/*----------------------------------------------------------------------*/
/*	afv_adj_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	100ms毎にAFV調整ｺﾏﾝﾄﾞ及びﾒﾓﾘｰﾘｰﾄﾞｺﾏﾝﾄﾞを発行し、AFV係数,	*/
/*	振動ﾃﾞｰﾀのP-P値を表示する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	adc_id		: adc board number				*/
/*									*/
/* 戻り値								*/
/*	void		: 無し						*/
/*----------------------------------------------------------------------*/

static void far
afv_adj_tsk(
	unsigned int	adc_id
	)
{
	long		adj_interval = 100;	/* AFV調整の間隔(ms)	*/
	long		coef= 0;		/* AFV係数		*/
	short		peak[4];		/* vib peak値		*/
	unsigned long	ptn;
	int		err, i, j, n;

	while(ptn = afv_ptn) {
		for(i = 0; ptn; i++,ptn >>= 1) {
			if(ptn & 0x1) {
				err = HLT_TSK(&adj_interval);
				j = i/ ADB_WEI_MAX;
				n = (i% ADB_WEI_MAX)+1;
			    	err = dad_afv_adjust( (DAD_ID *)&j, &n, &coef );
			    	err = dad_dsp_memrd( j, (void *)&peak,
						(short *)0x390b, (short)4);
			    	printf("\r\n %2d %6ld %4d %4d",
					n, coef, peak[1], peak[0]);
			}
	    	}
	}
	adc_id++;
	err = EXD_TSK();
}

/*----------------------------------------------------------------------*/
/*	sta_afv_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	afv_adj_tsk()をｸﾘｴｰﾄしｽﾀｰﾄさせる。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	unsigned long *	: AFV指定号機ﾊﾟﾀｰﾝへのﾎﾟｲﾝﾀ			*/
/*									*/
/* 戻り値								*/
/*	int		:  0	･･･ 成功				*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#define	WORK	(4096)
#define	MANAFV_TSK_ID	MONSTB_TSK_ID

int
sta_afv_tsk(
	unsigned long	ptn
	)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/
	int		err;

	if((afv_ptn)||(! ptn))					return(-1);
	_disable();
	afv_ptn = ptn;
	_enable();
	afv_tsk.ifp = (int *)&afv_tsk.acadr;
	tsk_pkt.tsk_adr = (void *)afv_adj_tsk;
	tsk_pkt.priority = MONAFV_TSK_PRI;
	tsk_pkt.stk_size = WORK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	if(err = CRE_TSK( &afv_tsk.acadr, (unsigned int)MANAFV_TSK_ID, &tsk_pkt)) {
		assert(!err);					return(-1);
	}
	if(err = STA_TSK( afv_tsk.acadr, (unsigned int)DADC1)) {
		assert(!err);					return(-1);
	}
	return(0);
}

/*----------------------------------------------------------------------*/
/*	stp_afv_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	afv_adj_tsk()を停止させる。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void : 								*/
/*									*/
/* 戻り値								*/
/*	int		:  0	･･･ 成功				*/
/*----------------------------------------------------------------------*/

int
stp_afv_tsk()
{
	_disable();
	afv_ptn = 0;
	_enable();
	return(0);
}
