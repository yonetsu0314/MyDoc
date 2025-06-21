/*======================================================================
 * File name    : sumfnc.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	サムチェック用のプロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/sumfnc.h 1.1 1997/05/28 15:16:42 yonetsu Exp $
 *$Log: sumfnc.h $
 * リビジョン 1.1  1997/05/28  15:16:42  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef _SUMFNC_H_
#define _SUMFNC_H_

#include "cmfhdl.h"	/* for COMRV */

COMRV cpl_hex_ascii_sum(unsigned char *buf,int len,char *txrx);
COMRV or30h_ascii_sum(unsigned char *buf,int len,char *txrx);

#endif


