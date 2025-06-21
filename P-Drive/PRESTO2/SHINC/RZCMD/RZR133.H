/*======================================================================
 * File name    : rzr133.h
 * Original	: rzr033.h
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr133.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr133.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr133.h 1.3 1998/08/07 20:18:07 hiroe
 *Log: rzr133.h
 * リビジョン 1.3  1998/08/07  20:18:07  hiroe
 * coff -> coef
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(RZR133_H)
#define	RZR133_H


/*------------------------------------------------------*/
/*	初期零点調整パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr133t {
	unsigned char	init_zero_flag;		/* 0:通常零点flag 1:初期零点flag */
	unsigned short	ad_coef;		/* AD係数 */
	unsigned short	da_coef;		/* DA係数 */
	unsigned short	range;			/* 秤量 */
} RZR133T;

#if	defined(STORAGE)
struct rzr133t	rzr133s;
#else
#if	!defined(NOEXTERN)
extern struct rzr133t	rzr133s;
#endif
#endif

#endif
