/*======================================================================
 * File name	: prr0011.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0011.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0011.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr011.hv  2.1  93/11/02 15:41:10  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr011.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0011_H)
#define	PRR0011_H

enum prr00110e {		/* 補正値自動更新			*/
	RNWCOMP_OFF,		/* しない				*/
	RNWCOMP_ON,		/* する					*/
};

enum prr00111e {		/* 計算方式				*/
	CALC_NORML,		/* 標準					*/
	CALC_EXPND,		/* 拡張					*/
	CALC_MTRIX,		/* 差分マトリックス			*/
};

enum prr00112e {		/* ミックス補正方式			*/
	MIX_CNSCT,		/* 順次					*/
	MIX_TNMNT,		/* 相互					*/
};

enum prr00113e {		/* オーバースケール			*/
	OVSCL_DUMP,		/* 自動排出				*/
	OVSCL_STOP,		/* 即停止				*/
	OVSCL_DISABLE,		/* 不参加（ｎヘッド停止）		*/
};

enum prr00114e {		/* 過量					*/
	OVWGT_DUMP,		/* 自動排出				*/
	OVWGT_NSTOP,		/* ｎ回停止				*/
};

enum prr00115e {		/* ホッパエラー				*/
	HPERR_AUTO,		/* 自動解除				*/
	HPERR_STOP,		/* 即停止				*/
	HPERR_DISABLE,		/* 不参加（ｎヘッド停止）		*/
};

enum prr00116e {		/* 自動零点エラー			*/
	AZERR_AUTO,		/* 自動解除				*/
	AZERR_STOP,		/* 即停止				*/
	AZERR_DISABLE,		/* 不参加（ｎヘッド停止）		*/
};

enum prr00117e {		/* 安定ヘッドの処理			*/
	STABLE_EACH,		/* 毎回安定をチェックする		*/
	STABLE_ONCE,		/* 一旦安定すれば安定チェックをしない	*/
};

enum prr00118e {		/* 再チェックエラー			*/
	RECHECK_DUMP,		/* 自動排出				*/
	RECHECK_STOP,		/* 停止					*/
};

enum prr00119e {		/* RS,DTH,THエラー			*/
	TH_AUTO,		/* 自動解除				*/
	THP_STOP,		/* 即停止				*/
};

enum prr0011ae {		/* エラー解除動作			*/
	ERREJ_ALL,		/* 一括排出				*/
	ERREJ_EACH,		/* 個別排出				*/
};

enum prr0011be {		/* 検定フラグ				*/
	NO_FLAG,		/* 無検定				*/
	NMI_FLAG,		/* ＮＭＩ検定				*/
};
/*------------------------------------------------------*/
/*	組み合わせ計算特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0011t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	wh_active;	/* WH active head[nof_head/4]	*/
	unsigned long	bh_active;	/* BH active head[nof_head/4]	*/
	unsigned char	compensation;	/* compensation value		*/
	enum prr00110e	renew_compensation;/* renew compensation control*/
	enum prr00111e	calc_mode;	/* calculate control		*/
	enum prr00112e	mix_mode;	/* mix calculate control	*/
	enum prr00113e	over_scale;	/* over scale process control	*/
	enum prr00114e	over_weight;	/* over weight process control	*/
	enum prr00115e	hop_error;	/* hopper error process control	*/
	enum prr00116e	az_error;	/* autozero errror process control*/
	enum prr00117e	stable;		/* stable head process control	*/
	enum prr00118e	re_check;	/* re check process control	*/
	enum prr00119e	th_error;	/* TH process control		*/
	unsigned char	nof_stop_head;	/* number of stop head		*/
	unsigned char	nof_stop_cycle;	/* number of stop cycle		*/
	enum prr0011ae	err_rej_type;	/* error reject process control	*/
	unsigned char	multi_cal;	/* multi cal. process control	*/
	enum prr0011be	approval_flag;	/* approval flag		*/
	unsigned short	max_weight;	/* max. weight of approval	*/
	unsigned short	min_weight;	/* min. weight of approval	*/
} PRR0011T;

#if	defined(STORAGE)
struct prr0011t	prr0011s;
#else
#if	!defined(NOEXTERN)
extern struct prr0011t	prr0011s;
#endif
#endif

#endif
