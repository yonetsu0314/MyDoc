/*======================================================================
 * File name    : supapl.h
 * Original		: p:/dacs_i/rcu/main/rcs/supapl.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		APLタスクのセット･アップルーチンのプロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/supapl.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: supapl.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * FIFOの追加に対応する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef _SUPAPL_H_
#define _SUPAPL_H_

#if	!defined(STORAGE)
extern union aap	*const scif_rxaa[];
extern union aap	*const mpsc_rxaa[];
extern union aap	*const arcnet_rxaa[];
extern union aap	*const key_rxaa[];
extern union aap	*const fif_rxaa[];
#endif

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int setup_apl_func(void);

#endif
