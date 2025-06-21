/*======================================================================
 * File name    : rzr025a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用プログラム番号要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr025a.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr025a.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr025a.h 1.2 1995/11/16 16:13:23 hiroe
 *Log: rzr025a.h
 * リビジョン 1.2  1995/11/16  16:13:23  hiroe
 * 誤記修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR025_H)
#define	RZR025_H

#include	<rzcmd/rzprm.h>

/*------------------------------------------------------*/
/*	プログラム番号の要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr025t {
	enum rzprm001e	brd_id;		/* デバイスID番号		*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	node_id;	/* ノード番号			*/
} RZR025T;

#if	defined(STORAGE)
struct rzr025t	rzr025s;
#else
#if	!defined(NOEXTERN)
extern struct rzr025t	rzr025s;
#endif
#endif

#define		rz_scan_req_prgno	rz_scan_req_prgno_v3
#define		rz_print_req_prgno	rz_print_req_prgno_v3

#endif
