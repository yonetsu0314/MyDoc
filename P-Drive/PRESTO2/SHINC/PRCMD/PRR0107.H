/*======================================================================
 * File name	: prr0107.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	フィーダ共振周期検出開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0107.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0107.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/nzcmd/rcs/nzr051.h 1.1 1970/01/01 00:00:00 kawamura Exp
 *log: nzr051.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0107_H)
#define	PRR0107_H


/*------------------------------------------------------*/
/*	フィーダ共振周期検出開始パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/


typedef struct prr0107t {
	unsigned long	rf_detect_ptn;	/* RF detection pattern	*/
	unsigned long	df_detect_ptn;	/* DF detection pattern	*/
	unsigned long	af_detect_ptn;	/* AF detection pattern	*/
	unsigned long	cf_detect_ptn;	/* CF detection pattern	*/
} PRR0107T;

#if	defined(STORAGE)
struct prr0107t	prr0107s;
#else
#if	!defined(NOEXTERN)
extern struct prr0107t	prr0107s;
#endif
#endif

#endif
