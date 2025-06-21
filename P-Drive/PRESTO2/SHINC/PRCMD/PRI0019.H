/*======================================================================
 * File name	: pri0019.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	受信完了データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0019.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0019.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzi019.hv  2.2  93/12/26 16:12:08  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzi019.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0019_H)
#define	PRI0019_H


/*------------------------------------------------------*/
/*	受信完了のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0019t {
	unsigned char	command;	/* 受信コマンド			*/
	unsigned char	ch_id;		/* channel number		*/
} PRI0019T;

#if	defined(STORAGE)
struct pri0019t	pri0019s;
#else
#if	!defined(NOEXTERN)
extern struct pri0019t	pri0019s;
#endif
#endif

#endif
