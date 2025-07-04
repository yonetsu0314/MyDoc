/*======================================================================
 * File name    : rcr001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: ＲＣＵへのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/rcr001.h 1.1.4102.1 1997/05/28 13:25:36 yonetsu Exp $
 *$Log: rcr001.h $
 * リビジョン 1.1.4102.1  1997/05/28  13:25:36  yonetsu
 * タスクに関係のない extern 宣言を削除
 * 
 * リビジョン 1.1  1996/05/31  16:31:59  TGL
 * *** 空ログメッセージ ***
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		RCR001_H
#define		RCR001_H

int		setup_rcu_rcv(void);

#ifdef	CMFHDL_H
#define		rcurcv_tsk	rxtsk[RCU_DN]
#endif

#endif
