/*======================================================================
 * File name    : rzr041.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	全排出開始データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr041.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr041.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr041.hv  2.1  93/11/02 15:43:40  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr041.hv
 * Revision 2.1  93/11/02 15:43:40  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR041_H)
#define	RZR041_H


enum rzr0411e {				/* 自動零点			*/
	AZERO_OFF,			/* しない			*/
	AZERO_ON,			/* する				*/
};

/*------------------------------------------------------*/
/*	全排出開始パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr041t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned int	infeed;		/* 供給機制御モード		*/
	enum rzr0411e	auto_zero;	/* 自動零点制御モード		*/
} RZR041T;

#if	defined(STORAGE)
struct rzr041t	rzr041s;
#else
#if	!defined(NOEXTERN)
extern struct rzr041t	rzr041s;
#endif
#endif

#endif
