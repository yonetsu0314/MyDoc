/*======================================================================
 * File name    : rzi009a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用プログラム番号データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi009a.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi009a.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi009a.h 1.2 1995/11/16 16:14:16 hiroe
 *Log: rzi009a.h
 * リビジョン 1.2  1995/11/16  16:14:16  hiroe
 * 誤記修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI009_H)
#define	RZI009_H

#include	<rzcmd/rzprm.h>

/*------------------------------------------------------*/
/*	プログラム番号パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi009t {
	enum rzprm001e	brd_id;		/* デバイスID番号		*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	node_id;	/* ノード番号			*/
	unsigned char	len;		/* 文字列の長さ			*/
	char		rom_id[128];	/* ROM ID 文字列		*/
} RZI009T;

#if	defined(STORAGE)
struct rzi009t	rzi009s;
#else
#if	!defined(NOEXTERN)
extern struct rzi009t	rzi009s;
#endif
#endif

#define		rz_scan_prgno	rz_scan_prgno_v3
#define		rz_print_prgno	rz_print_prgno_v3

#endif
