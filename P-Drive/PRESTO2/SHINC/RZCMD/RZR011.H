/*======================================================================
 * File name    : rzr011.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr011.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr011.hv  2.1  93/11/02 15:41:10  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr011.hv
 * Revision 2.1  93/11/02 15:41:10  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR011_H)
#define	RZR011_H

enum rzr0110e {				/* 補正値自動更新		*/
	RNWCOMP_OFF,			/* しない			*/
	RNWCOMP_ON,			/* する				*/
};

enum rzr0111e {				/* 計算方式			*/
	CALC_NORML,			/* 標準				*/
	CALC_EXPND,			/* 拡張				*/
	CALC_MTRIX,			/* 差分マトリックス		*/
};

enum rzr0112e {				/* ミックス補正方式		*/
	MIX_CNSCT,			/* 順次				*/
	MIX_TNMNT,			/* 相互				*/
};

enum rzr0113e {				/* オーバースケール		*/
	OVSCL_DUMP,			/* 自動排出			*/
	OVSCL_NSTOP,			/* ｎヘッド停止			*/
};

enum rzr0114e {				/* 過量				*/
	OVWGT_DUMP,			/* 自動排出			*/
	OVWGT_NSTOP,			/* ｎ回停止			*/
};

enum rzr0115e {				/* ホッパエラー			*/
	HPERR_AUTO,			/* 自動解除			*/
	HPERR_NSTOP,			/* ｎヘッド停止			*/
};

enum rzr0116e {				/* 自動零点エラー		*/
	AZERR_AUTO,			/* 自動解除			*/
	AZERR_NSTOP,			/* ｎヘッド停止			*/
};

enum rzr0117e {				/* エラー解除動作		*/
	ERREJ_ALL,			/* 一括排出			*/
	ERREJ_EACH,			/* 個別排出			*/
};

/*------------------------------------------------------*/
/*	組み合わせ計算特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr011t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	wh_active;	/* WH active head[nof_head/4]	*/
	unsigned long	bh_active;	/* BH active head[nof_head/4]	*/
	unsigned char	compensation;	/* compensation value		*/
	enum rzr0110e	renew_compensation;/* renew compensation control*/
	enum rzr0111e	calc_mode;	/* calculate control		*/
	enum rzr0112e	mix_mode;	/* mix calculate control	*/
	enum rzr0113e	over_scale;	/* over scale process control	*/
	enum rzr0114e	over_weight;	/* over weight process control	*/
	enum rzr0115e	hop_error;	/* hopper error process control	*/
	enum rzr0116e	az_error;	/* autozero errror process control*/
	enum rzr0117e	err_rej_type;	/* error reject process control	*/
} RZR011T;

#if	defined(STORAGE)
struct rzr011t	rzr011s;
#else
#if	!defined(NOEXTERN)
extern struct rzr011t	rzr011s;
#endif
#endif

#endif
