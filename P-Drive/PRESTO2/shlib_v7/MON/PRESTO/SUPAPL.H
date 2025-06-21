/*======================================================================
 * File name    : supspl.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	APLタスクのセット･アップルーチンのプロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/rz_wcu2/rcs/supapl.h 1.1 1996/06/18 18:43:49 chimura Exp $
 *$Log: supapl.h $
 * Revision 1.1  1996/06/18  18:43:49  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/rz_wcu/rcs/supapl.h 2.1 1993/10/20 11:24:34 ryoji
 *Log: supapl.h
 * リビジョン 2.1  1993/10/20  11:24:34  ryoji
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		SUPAPL_H
#define		SUPAPL_H

#if	!defined(STORAGE)
extern union aap	*const con_rxaa;
extern union aap	*const com_rxaa[];
extern union aap	*const inc_rxaa[];
extern union aap	*const dpm_rxaa[];
extern union aap	*const mpsc_rxaa[];
extern union aap	*const sgaa[];
#endif

int		setup_apl_func(void);

#endif
