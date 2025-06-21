/*----------------------------------------------------------------------*
 * File name	: supmon.c	/ setup moniter				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/supmon.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: supmon.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/supmon.c 1.1 1970/01/01
 *Log: supmon.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#define	P5348
#include	<drive\serial.h>		/* Show DRIVE directory	*/
#include	<drive\scudrv.h>		/*	"		*/
#include	<drive\pdadcdrv.h>		/*	"		*/
#include	<drive\dadcsim.h>		/*	"		*/

#ifdef	_SIM
#define	STORAGE
#include	"rxdef.h"			/* Show APL directory	*/
#undef	STORAGE
#endif
#include	"cmfhdl.h"			/* Show APL directory	*/
#include	"supapl.h"			/*	"		*/
#if	!defined(_MOPT)
#define	_MOPT
#endif
#include	"montsk.h"			/*	"		*/
#include	"admtxt.h"			/* Show current dir	*/


/*----------------------------------------------------------------------*/
/*	setup_mon_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	モニター用タスクの生成及び起動を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/

int	setup_mon_rcv(void);

int
setup_mon_tsk(void)
{
	if(setup_mon_rcv())				return(-1);
	return(0);
}
