/*======================================================================
 * File name    : cor1tsk.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: CON1 へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/cortsk.h 1.1 1997/05/28 13:19:54 yonetsu Exp $
 *$Log: cortsk.h $
 * リビジョン 1.1  1997/05/28  13:19:54  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef _COR1TSK_H_
#define _COR1TSK_H_

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int setup_con1_rcv_tsk(void);

#ifdef	CMFHDL_H
#define		con1rcv_tsk	rxtsk[CON_DN1]
#endif

#endif
