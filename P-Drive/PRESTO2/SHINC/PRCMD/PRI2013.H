/*======================================================================
 * File name	: pri2013.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	状態データ拡張パラメータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri2013.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri2013.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI2013_H)
#define	PRI2013_H

enum pri20130e {		/* FSV切替スイッチ 	不参加スイッチ	*/
	STATUS_0,		/* オフ			オン		*/
	STATUS_1,		/* オフ			オフ		*/
	STATUS_2,		/* オン			オフ		*/
};

/*------------------------------------------------------*/
/*	FSV（BFS）用の状態データ			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri2013t {
	unsigned char	nof_ch;		/* number of channel		*/
	enum pri20130e	cnd[4];		/* 外部スイッチの状態		*/
} PRI2013T;

#if	defined(STORAGE)
struct pri2013t	pri2013s;
#else
#if	!defined(NOEXTERN)
extern struct pri2013t	pri2013s;
#endif
#endif

#endif
