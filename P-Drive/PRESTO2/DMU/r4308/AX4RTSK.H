/*======================================================================
 * File name    : ax4rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX4 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax4rtsk.h 1.1 1996/12/04 10:56:49 yonetsu Exp $
 *$Log: ax4rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:49  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX4RTSK_H_
#define	_AX4RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux4_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux4rcv_tsk	rxtsk[AUX4_DN]
#endif

#endif
