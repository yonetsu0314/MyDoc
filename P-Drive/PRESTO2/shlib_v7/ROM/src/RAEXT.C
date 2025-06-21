/*----------------------------------------------------------------------*
 * File name	: raext.c	/ romable exit				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/rom/src/RCS/raext.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: raext.c $
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
/*	規格－イシダ･オリジナル						*/
/*	削除するタスクのID及びアクセス･アドレスをprintし自タスクを	*/
/*	アボートする。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
/*#include	<dos.h>*/			/* MS-C std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\rxdefd.h>		/* Show drive		*/

#define	TASK_ID		(4/sizeof(int))
#define	TASK_PRI	(6/sizeof(int))

void
exit_task(void)
{
	union aap	curr_tsk;		/* 自ﾀｽｸのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
	int		err;

	_disable();
	curr_tsk.vfp = (void *)0;
	err = TCB_ADR(&curr_tsk.acadr, (unsigned int)0);
	if(curr_tsk.ifp[TASK_PRI] > ABORT_TSK_PRI)
		err = CHG_PRI(curr_tsk.acadr, (unsigned int)ABORT_TSK_PRI);
	printf("\n\rStack Overflow ! Task ID:%4x, Access ADR:%4x",
		curr_tsk.ifp[TASK_ID], curr_tsk.acadr);
	_enable();
	while(1) { ; }
	ABO_TSK((unsigned int)err);
}
