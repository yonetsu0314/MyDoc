/*----------------------------------------------------------------------*
 * File name	: tmrsini.c	/ timer deriver initialize		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/timer/RCS/tmrsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tmrsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	Timer管理用構造体の初期化、及びTimer減算ﾀｽｸの起動。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ary_elem	: 第２引き数以降の配列の要素数			*/
/*	identify[]	: ﾀｲﾏｰ識別子へのﾎﾟｲﾝﾀｰ				*/
/*			     0	･･･  5ms timer				*/
/*			     1	･･･  10ms timer				*/
/*	number[]	: 1ｱｲﾃﾑのﾀｲﾏｰ数へのﾎﾟｲﾝﾀｰ			*/
/*			     0～MAX_TMR(0の場合そのｱｲﾃﾑは未使用)	*/
/*	*pointer[]	: ﾀｲﾏｰ･ﾚｼﾞｽﾀへのﾎﾟｲﾝﾀｰの配列			*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#define		STORAGE				/* 確保/外部参照	*/
#include	"tmrctl.h"			/* Show current		*/
#undef		STORAGE

extern int init_tmr_tsk(void);

int
tmr_sys_init(
	int		ary_elem,		/* no of element[]	*/
	int		identify[],		/* pointer to identifier*/
	int		number[],		/* pointer to number	*/
	unsigned short 	*pointer[]		/* pointer to timer addr*/
	)
{
/*	int		err; */
	struct tmh	*tmsp[2];

	if((ary_elem == 0)||(ary_elem > ITEM_NUM))		return(-1);
	tmsp[0] = tm5;
	tmsp[1] = tm10;
	while(ary_elem --) {
		if((*number == 0)||(*number > MAX_TMR))		return(-1);
		else {
			tmsp[*identify][item_n[*identify]].no = *number;
			tmsp[*identify][item_n[*identify]].pnt = *pointer;
		}
		++ item_n[*identify];
		++ identify;
		++ number;
		++ pointer;
	}
	tmr_ds.ifp = (int *)&tmr_ds.acadr;
	if(init_tmr_tsk())					return(-1);
	return(0);
}
