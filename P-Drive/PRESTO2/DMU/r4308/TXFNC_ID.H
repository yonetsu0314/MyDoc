/*======================================================================
 * File name    : txfnc_id.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/txfnc_id.h 1.3 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: txfnc_id.h $
 * リビジョン 1.3  1970/01/01  00:00:00  yonetsu
 * 2000/02/07 10:04
 * ID を追加.
 * 
 * リビジョン 1.2  1998/05/15  10:48:04  yonetsu
 * モデムコントロール用定義の追加
 * 
 * リビジョン 1.1  1997/05/28  13:10:49  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_TXFNC_ID_H_
#define	_TXFNC_ID_H_

#include "cmfhdl.h"

enum tx_function_idno {
	TX_RCU_NORMAL = COM_DN_MAX, /* HIC から RCU に送るための TAG なしデータ	 */
	TX_RCU_PRE , 		    /* HIC から RCU に送るための予約交信データ	 */
	TX_RCU_DDE_HOST,  	 /* HIC から RCU に送るためのDDEホスト連動データ */
	TX_RCU_STD_HOST,  	 /* HIC から RCU に送るための標準ホスト連動データ*/
	TX_RCU_MODEM_HOST,	 /* HIC から RCU に送るためのモデムホスト連動データ*/
	TX_EXT_C1_PRE,	  	 /* 外部 C1側への予約交信データ		  	 */
	TX_EXT_C2_PRE,	  	 /* 外部 C2側への予約交信データ		  	 */
	TX_EXT_C1C2_PRE,  	 /* 外部 C1C2側への予約交信データ		 */
	TX_EXT_DDE_HOST,  	 /* ホストへのDDEホスト連動データ		 */
	TX_EXT_STD_HOST,  	 /* ホストへの標準ホスト連動データ		 */
	TX_EXT_ONEWAY,	  	 /* １方向重量データ				 */
	TX_EXT_MODEM_HOST	 /* モデムへのホスト連動データ		 	 */
};

#endif
