/*======================================================================
 * File name    : rzr025.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr025.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr025.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr025.hv  2.1  93/11/02 15:42:28  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr025.hv
 * Revision 2.1  93/11/02 15:42:28  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR025_H)
#define	RZR025_H


enum rzr0250e {				/* ボード番号			*/
	BORD_ICU,			/* ＷＣＵ			*/
	BORD_WCU,			/* ＩＣＵ			*/
	BORD_DUC,			/* ＤＵＣ			*/
	BORD_EXC,			/* ＥＸＣ			*/
	BORD_ADC,			/* ＡＤＣ 			*/
};

/*------------------------------------------------------*/
/*	プログラム番号の要求パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr025t {
	enum rzr0250e	dev_id;		/* デバイスID番号		*/
	unsigned char	head_no;	/* ヘッド番号			*/
} RZR025T;

#if	defined(STORAGE)
struct rzr025t	rzr025s;
#else
#if	!defined(NOEXTERN)
extern struct rzr025t	rzr025s;
#endif
#endif

#endif
