/*======================================================================
 * File name    : ax1rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX1 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax1rtsk.h 1.1 1996/12/04 10:56:28 yonetsu Exp $
 *$Log: ax1rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:28  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX1RTSK_H_
#define	_AX1RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux1_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux1rcv_tsk	rxtsk[AUX1_DN]
#endif

#endif
