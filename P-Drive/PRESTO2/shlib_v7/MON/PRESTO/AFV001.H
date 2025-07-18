/*======================================================================
 * File name    : afv001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	AFV調整関連定数を定義する
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/dz_wcu2/rcs/afv001.h 1.1 1996/06/18 18:41:04 chimura Exp $
 *$Log: afv001.h $
 * Revision 1.1  1996/06/18  18:41:04  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/dz_wcu/rcs/afv001.h 3.2 1994/05/27 09:52:52 kabumoto
 *Log: afv001.h
 * リビジョン 3.2  1994/05/27  09:52:52  kabumoto
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		AFV001_H
#define		AFV001_H

int	setup_afv_adj(int ch, int cnt);
void	req_afv_file_write(void);
int	setup_afv_backup(unsigned int **tskaa);

#endif
