/*----------------------------------------------------------------------*
 * File name	: mevf01.c	/ monitor event flag			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mevf01.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mevf01.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mevf01.c 1.1 1970/01/01
 *Log: mevf01.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	"rxdef.h"			/* Show apl		*/
#include	"admtxt.h"			/* current		*/

#define		STA_OF_CYCLE	0x1
static union aap	mon_smplsta_evf = { 0L };
void	abort(void);
int	del_sampl_sta_evf(void);
/*----------------------------------------------------------------------*/
/*	init_sampl_sta_evf						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃﾞｰﾀ･ｻﾝﾌﾟﾘﾝｸﾞ開始時の同期ﾌﾗｸﾞを生成する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void								*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  1	･･･ timeout				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
init_sampl_sta_evf(void)
{
	int		err;
	long		time = 60000;
	unsigned int	curr_ptn;

	if(err = CRE_FLG(&mon_smplsta_evf.acadr, (unsigned int)MON_SMPLSTA_EVF_ID)) {
		assert(!err);					return(-1);
	}
	if(err = WAI_FLG(TIME_OUT|EVF_OR|EVF_RES, &curr_ptn,
			 mon_smplsta_evf.acadr, (unsigned int)STA_OF_CYCLE, &time)) {
		if(err == SVCE_TIMEOUT) {
			del_sampl_sta_evf();			return(1);
		}
		assert(!err);					return(-1);
	}
	return(0);
}

/*----------------------------------------------------------------------*/
/*	set_sampl_sta_evf						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃﾞｰﾀ･ｻﾝﾌﾟﾘﾝｸﾞ開始時の同期ﾌﾗｸﾞをｾｯﾄする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void								*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
set_sampl_sta_evf(void)
{
	int		err = 0;
	unsigned int	curr_ptn;

	if(mon_smplsta_evf.acadr) {
		err = SET_FLG(EVF_SET_REPL, &curr_ptn,
				mon_smplsta_evf.acadr, (unsigned int)STA_OF_CYCLE);
	}
	return(err);
}

/*----------------------------------------------------------------------*/
/*	del_sampl_sta_evf						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃﾞｰﾀ･ｻﾝﾌﾟﾘﾝｸﾞ開始時の同期ﾌﾗｸﾞを削除する。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void								*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
del_sampl_sta_evf(void)
{
	int		err;
	union aap	evf;

	if(((err = FLG_ADR(&evf.acadr, (unsigned int)MON_SMPLSTA_EVF_ID)) == SVCE_NOT_EXIST)
	  ||(evf.acadr != mon_smplsta_evf.acadr))		return(0);
	mon_smplsta_evf.acadr = 0;
	if(err = DEL_FLG(evf.acadr))				return(-1);
	return(0);
}
