/*======================================================================
 * File name    : dscuid.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＩＤファイル
 *----------------------------------------------------------------------
 *$Log: dscuid.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2004/06/21 18:00
 * 前リビジョンで受信エラー時の処理を修正したが、受信バッファフルをクリア
 * していないために、受信割り込みが解除されない不具合修正。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2004/06/21
 * 受信エラー時の解除処理が正しくなくエラー解除できないため、永遠にエラー解除
 * 割り込みから抜けない不具合修正。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/13 １文字受信で割り込みに変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/drive/_scu/rcs/dscuid.c 1.4 1994/09/17 13:20:16 sato Exp
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

char	*dscuid = "$Header: p:/presto2/shlib_v7/drive/scu/sh7706/scuc/RCS/dscuid.c 1.4 1970/01/01 00:00:00 chimura Exp $";
