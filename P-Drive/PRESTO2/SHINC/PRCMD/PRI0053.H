/*======================================================================
 * File name	: pri0053.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	重量波形データ応答データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0053.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0053.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzi007.h 1.1 1999/05/27 16:34:34 kawamura Exp
 *log: nzi007.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0053_H)
#define	PRI0053_H

#ifndef	PRPRM_H
#include	<prcmd/prprm.h>
#endif

enum pri00530e {			/* 状態				*/
	MFS      = 0x0001,		/* マイナス側フルスケール	*/
	PFS      = 0x0002,		/* プラス側フルスケール		*/
	UNSTABLE = 0x0004,		/* 不安定			*/
};

typedef struct pri00530t {
	short		weight;		/* 重量データ		*/
	enum pri00530e	status;		/* 重量ステータス	*/
} PRI00530T;

/*------------------------------------------------------*/
/*	重量波形データの応答パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0053t {
	enum prprm004e	hopper_id;	/* WH/PH識別フラグ	*/
	unsigned char	wh_no;		/* WH番号		*/
	unsigned short	index;		/* データのインデックス	*/
	unsigned char	nof_data;	/* データの数		*/
	struct pri00530t	data[32];	/* データ	*/
} PRI0053T;

#if	defined(STORAGE)
struct pri0053t	pri0053s;
#else
#if	!defined(NOEXTERN)
extern struct pri0053t	pri0053s;
#endif
#endif

#endif
