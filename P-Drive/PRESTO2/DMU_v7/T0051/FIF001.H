/*======================================================================
 * File name    : fif001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	fifへのテキスト送信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/fif001.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fif001.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		FIF001_H
#define		FIF001_H

int		setup_fif_snd( void );
int		setup_fif_rcv( void );

#ifdef	STORAGE
union aap fif_rcv_tsk_aa;
#else
extern union aap fif_rcv_tsk_aa;
#endif

#endif
