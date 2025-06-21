/*======================================================================
 * File name    : cortsk.h
 * Original		: p:/dacs_i/rcu/main/rcs/cortsk.h 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		CON へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/cortsk.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cortsk.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
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
#ifndef _CORTSK_H_
#define _CORTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int setup_con0_rcv_tsk(void);
int setup_con1_rcv_tsk(void);

#ifdef	STORAGE
union aap con0_rcv_tsk_aa;
union aap con1_rcv_tsk_aa;
#else
extern union aap con0_rcv_tsk_aa;
extern union aap con1_rcv_tsk_aa;
#endif

#endif
