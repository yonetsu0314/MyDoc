/*======================================================================
 * File name	: pri0047.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶ係数データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0047.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0047.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: DZI203.Hv  1.2  94/02/18 16:42:12  ryoji  Exp
 *log: P:/INCLUDE/DZCMD/RCS/DZI203.Hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0047_H)
#define	PRI0047_H

/*------------------------------------------------------*/
/*	AFV係数パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0047t {
	unsigned char	wh_no;		/* 該当するWH番号		*/
	long		new_coeffi;	/* 新ＡＦＶ係数			*/
	long		old_coeffi;	/* 旧ＡＦＶ係数			*/
} PRI0047T;

#if	defined(STORAGE)
struct pri0047t	pri0047s;
#else
#if	!defined(NOEXTERN)
extern struct pri0047t	pri0047s;
#endif
#endif

#endif
