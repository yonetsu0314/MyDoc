/*======================================================================
 * File name    : rzr103.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * 計量機の構成仕様拡張テキスト構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr103.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr103.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr103.h 1.1 1970/01/01 00:00:00 itami
 *Log: rzr103.h
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR103_H)
#define	RZR103_H


/*------------------------------------------------------*/
/*	計量機の構成仕様拡張テキスト構造		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr103t {
	unsigned char	nof_wh;		/* WH数					*/
	unsigned char	nof_bb;		/* 下段計量バケット数			*/
	unsigned char	nof_mb;		/* 中段計量バケット数			*/
	unsigned char	nof_wherr;	/* WH-不良排出位置間バケット数		*/
	unsigned char	nof_whwh;	/* WH間バケット数			*/
	unsigned char	nof_whbb;	/* WH-下段計量バケット間バケット数	*/
	unsigned char	nof_bbwh;	/* 下段計量バケット-WH間バケット数	*/
} RZR103T;

#if	defined(STORAGE)
struct rzr103t	rzr103s;
#else
#if	!defined(NOEXTERN)
extern struct rzr103t	rzr103s;
#endif
#endif

#endif
