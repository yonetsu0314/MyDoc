/*======================================================================
 * File name    : ax5rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX5 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax5rtsk.h 1.1 1996/12/04 10:56:56 yonetsu Exp $
 *$Log: ax5rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:56  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX5RTSK_H_
#define	_AX5RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux5_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux5rcv_tsk	rxtsk[AUX5_DN]
#endif

#endif
