/*======================================================================
 * File name    : dsciid.c
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/dsciid.c 1.1
 *----------------------------------------------------------------------
 * Function		: ＩＤファイル
 *----------------------------------------------------------------------
 *$Log: dsciid.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/03/24
 * biosif\scua.hで定義しているTBRDYがsh7706用のため、sciotx.cで再定義する。
 * #undef  TBRDY
 * #define TBRDY   (0x20)  TXバッファー書き込み可
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/09/01 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *				(c) Copyright 2002, ISHIDA CO., LTD.
 *				959-1 SHIMOMAGARI RITTO-CITY
 *				SHIGA JAPAN
 *				(077) 553-4141
 *======================================================================
 */

const char * const dsciid = "$Header: p:/presto2/shlib_v7/drive/scu/sh7615/sci/RCS/dsciid.c 1.3 1970/01/01 00:00:00 chimura Exp $";
