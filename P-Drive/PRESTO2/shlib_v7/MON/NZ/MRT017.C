/*----------------------------------------------------------------------*
 * File name	: mrt017.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt017.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt017.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt017.c 1.1 1996/07/22 19:25:35
 *Log: mrt017.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

#define		BUF_MAX		512
static unsigned char near	*dsp_phy = (void *)0x1000;

/*----------------------------------------------------------------------*/
/*	mon_arc_showmap							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARCNETの参加状況を表示する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc		: no. of argumunt				*/
/*	*argv[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
mon_arc_showmap(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	char far	*map_tbl_ptr;		/* map table への＆	*/

	switch(argc) {
	case 2:
		break;

	default:
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}

	if((map_tbl_ptr = get_arc_map( atoi(argv[2]) ))==(char far *)NULL) {
		mon_text_snd( ARC_SHOWMAP_FAIL, argl );	return(-1);
	}
	argl[0] = map_tbl_ptr;
	return( mon_text_snd( ARC_SHOWMAP_SUCES, argl) );
}
