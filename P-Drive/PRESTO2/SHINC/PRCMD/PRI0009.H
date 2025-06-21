/*======================================================================
 * File name	: pri0009.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0009.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi009a.h 1.2 1995/11/16 16:14:16 hiroe Exp
 *log: rzi009a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0009_H)
#define	PRI0009_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

/*------------------------------------------------------*/
/*	プログラム番号パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0009t {
	enum prprm001e	brd_id;		/* デバイスID番号		*/
	unsigned char	dev_no;		/* デバイス番号			*/
	unsigned char	node_id;	/* ノード番号			*/
	unsigned char	len;		/* 文字列の長さ			*/
	char		rom_id[128];	/* ROM ID 文字列		*/
} PRI0009T;

#if	defined(STORAGE)
struct pri0009t	pri0009s;
#else
#if	!defined(NOEXTERN)
extern struct pri0009t	pri0009s;
#endif
#endif

#endif
