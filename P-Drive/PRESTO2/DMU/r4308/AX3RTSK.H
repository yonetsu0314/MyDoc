/*======================================================================
 * File name    : ax3rtsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		AUX3 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax3rtsk.h 1.1 1996/12/04 10:56:42 yonetsu Exp $
 *$Log: ax3rtsk.h $
 * リビジョン 1.1  1996/12/04  10:56:42  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_AX3RTSK_H_
#define	_AX3RTSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int		setup_aux3_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		aux3rcv_tsk	rxtsk[AUX3_DN]
#endif

#endif
