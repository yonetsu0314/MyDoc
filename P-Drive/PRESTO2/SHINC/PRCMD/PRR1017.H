/*======================================================================
 * File name	: prr1017.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッドの遅れ時間拡張タイミングデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr1017.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr1017.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/presto/shinc/rzcmd/RCS/rzr217.h 1.1 1970/01/01 00:00:00 chimura Exp
 *log: rzr217.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR1017_H)
#define	PRR1017_H

typedef struct prr10171t {
	unsigned char	parent_delay;	/* 親ヘッド遅れ時間		*/
} PRR10171T;

typedef struct prr10170t {
	unsigned char	nof_parent;	/* 該当するsectの親ヘッドの数	*/
	PRR10171T	pandc[2];	/* parent paramter[nof_parent]	*/
} PRR10170T;

/*------------------------------------------------------*/
/*	タイミングパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr1017t {
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	PRR10170T	sect[8];	/* section paramter[nof_sect]	*/
} PRR1017T;

#if	defined(STORAGE)
struct prr1017t	prr1017s;
#else
#if	!defined(NOEXTERN)
extern struct prr1017t	prr1017s;
#endif
#endif

#endif
