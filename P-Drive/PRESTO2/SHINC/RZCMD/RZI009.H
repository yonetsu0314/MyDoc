/*======================================================================
 * File name    : rzi009.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	プログラム番号データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzi009.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzi009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzi009.hv  2.1  93/11/02 15:40:20  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzi009.hv
 * Revision 2.1  93/11/02 15:40:20  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI009_H)
#define	RZI009_H


enum rzi0190e {				/* ボード番号			*/
	BORD_ICU,			/* ＷＣＵ			*/
	BORD_WCU,			/* ＩＣＵ			*/
	BORD_DUC,			/* ＤＵＣ			*/
	BORD_EXC,			/* ＥＸＣ			*/
	BORD_ADC,			/* ＡＤＣ 			*/
};

/*------------------------------------------------------*/
/*	プログラム番号パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi009t {
	enum rzi0190e	dev_id;		/* デバイスID番号		*/
	unsigned char	head_no;	/* ヘッド番号			*/
	unsigned char	rom_no[6];	/* プログラム番号		*/
	unsigned char	ver_prefix;	/* バージョン番号		*/
	unsigned char	ver_suffix;	/* レビジョン番号		*/
} RZI009T;

#if	defined(STORAGE)
struct rzi009t	rzi009s;
#else
#if	!defined(NOEXTERN)
extern struct rzi009t	rzi009s;
#endif
#endif

#endif
