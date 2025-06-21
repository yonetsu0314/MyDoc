/*======================================================================
 * File name    : ax2rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX2 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax2rtsk.h 1.1 1996/12/04 10:56:36 yonetsu Exp $
 *$Log: ax2rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:36  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX2RTSK_H_
#define	_AX2RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux2_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux2rcv_tsk	rxtsk[AUX2_DN]
#endif

#endif
