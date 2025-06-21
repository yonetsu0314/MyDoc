/*======================================================================
 * File name    : ax0rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX0 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax0rtsk.h 1.1 1996/12/04 10:56:20 yonetsu Exp $
 *$Log: ax0rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:20  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX0RTSK_H_
#define	_AX0RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux0_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux0rcv_tsk	rxtsk[AUX0_DN]
#endif

#endif
