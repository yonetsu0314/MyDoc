/*======================================================================
 * File name    : supapl.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	ＡＰＬタスクのセット･アップルーチンのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/supapl.h 1.1 1996/05/31 16:32:52 TGL Exp $
 *$Log: supapl.h $
 * リビジョン 1.1  1996/05/31  16:32:52  TGL
 * *** 空ログメッセージ ***
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		SUPAPL_H
#define		SUPAPL_H

#if	!defined(STORAGE)
extern union aap	*const con_rxaa[];
extern union aap	*const com_rxaa[];
extern union aap	*const dpm_rxaa[];
extern union aap	*const mpsc_rxaa[];
extern union aap	*const sgaa[];
#endif

int		setup_apl_func(void);

#endif
