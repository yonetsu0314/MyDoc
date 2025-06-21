/*======================================================================
 * File name    : rzr037.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	運転開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr037.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr037.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr037.hv  2.1  93/11/02 15:42:50  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr037.hv
 * Revision 2.1  93/11/02 15:42:50  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR037_H)
#define	RZR037_H


enum rzr0370e {				/* 供給制御			*/
	INFD_OFF,			/* しない			*/
	INFD_ON,			/* する				*/
};

/*------------------------------------------------------*/
/*	運転開始パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr037t {
	unsigned char	ch_id;		/* channel number		*/
	enum rzr0370e	infeed;		/* 供給機制御モード		*/
} RZR037T;

#if	defined(STORAGE)
struct rzr037t	rzr037s;
#else
#if	!defined(NOEXTERN)
extern struct rzr037t	rzr037s;
#endif
#endif

#endif
