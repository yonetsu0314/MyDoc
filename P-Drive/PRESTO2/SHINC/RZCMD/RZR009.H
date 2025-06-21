/*======================================================================
 * File name    : rzr009.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr009.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr009.hv  2.1  93/11/02 15:41:16  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr009.hv
 * Revision 2.1  93/11/02 15:41:16  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR009_H)
#define	RZR009_H

enum rzr0090e {				/* 秤量				*/
	RANGE_NORML,			/* 標準				*/
	RANGE_EXPND,			/* 拡張				*/
};

enum rzr0091e {				/* 予測計量			*/
	ANTICIP_OFF,			/* しない			*/
	ANTICIP_ON,			/* する				*/
};

/*------------------------------------------------------*/
/*	秤計量特性パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr009t {
	unsigned short	stable_time;	/* stable time constant		*/
	unsigned char	stable_count;	/* stable count			*/
	unsigned short	zero_limit_cnt;	/* auto zero error count	*/
	unsigned char	zero_interval;	/* auto zero interval		*/
	unsigned char	stable_cmp_cnt;	/* compare count		*/
	unsigned short	empty_weight;	/* empty weight			*/
	enum rzr0090e	range;		/* weigh range			*/
	unsigned char	smpl_interval;	/* sampling interval		*/
	unsigned char	iir_reset_cnt;	/* IIR filter reset count	*/
	enum rzr0091e	anticipation;	/* anticipate weighing		*/
} RZR009T;

#if	defined(STORAGE)
struct rzr009t	rzr009s;
#else
#if	!defined(NOEXTERN)
extern struct rzr009t	rzr009s;
#endif
#endif

#endif
