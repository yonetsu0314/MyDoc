/*----------------------------------------------------------------------*
 * File name	: mazf001.h	/ auto zero 3rd filter  monitor header  *
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mazf001.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mazf001.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti0/rcs/mazf001.h 1.1 1970/01/01 00:00:00
 *Log: mazf001.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2001, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* Function	:							*/
/*	monitor用のコマンド及び通信テキストを定義。			*/
/*----------------------------------------------------------------------*/

#ifndef	MAZF001_H
#define	MAZF001_H

#if	!defined(AZF001_H) && !defined(MAZF001_C)
#define	mon_dsp_zero( argc, argv )\
	mon_dsp_az_f3rd( argc, argv )
#endif

#undef	ad_azero
#define	ad_azero(p1, p2, p3, p4)\
	mazf_dad_az(p1, p2, p3, p4, argv)

int	mon_dsp_az_f3rd( int argc, char *argv[]);
int	mazf_dad_az( int pwb, int ch_no, short *ad, short *da, char *argv[]);

#endif
