/*======================================================================
 * File name	: prr0009.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0009.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr009.hv  2.1  93/11/02 15:41:16  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr009.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0009_H)
#define	PRR0009_H

enum prr00090e {			/* 秤量				*/
	RANGE_NORML,			/* 標準				*/
	RANGE_EXPND,			/* 拡張				*/
};

enum prr00091e {			/* AFV mode			*/
	AFV_OFF,
	AFV_AUTO,
	AFV_ON,
};

enum prr00092e {			/* span weight			*/
	SPN_20G,
	SPN_40G,
	SPN_200G,
	SPN_400G,
	SPN_500G,
	SPN_1000G,
};

enum prr00093e {			/* cell type			*/
	NLC_012L,
	NLC_04L,
	NLC_06L,
	NLC_12L,
	NLC_24L,
	NLC_02L,
};

typedef struct	prr00093t {		/*フィルタパラメータ		*/
	unsigned short	thin;		/* 間引き数			*/
	unsigned short	dig;		/* 有効桁増加数			*/
	unsigned short	tap;		/* タップ数			*/
	unsigned short	coef;		/* フィルタ係数			*/
} PRR00093T;

typedef struct prr00090t {
	unsigned char	stable_count;	/* stable count			*/
	unsigned short	zero_limit_cnt;	/* auto zero error count	*/
	unsigned char	stable_cmp_cnt;	/* compare count		*/
	enum prr00090e	range;		/* weigh range			*/
	unsigned char	smpl_interval;	/* sampling interval		*/
	unsigned char	iir_reset_cnt;	/* IIR filter reset count	*/
	enum prr00091e	afv_mode;	/* AFV mode			*/
	unsigned char	filter;		/* filter number		*/
	enum prr00092e	span_wt;	/* span weight			*/
	enum prr00093e	cell_type;	/* cell type			*/
	unsigned char	nof_stage;	/* フィルタステージ数		*/
	struct prr00093t	dadfip[6];/* フィルタパラメータ		*/
	unsigned short	afv_wh_limit;	/* AFV調整WH限界値		*/
	unsigned short	afv_afv_limit;	/* AFV調整AFV限界値		*/
	unsigned short	afv_onoff_limit;/* AFV補正ON/OFF限界値		*/
} PRR00090T;

typedef struct prr00091t {
	unsigned short	empty_weight;	/* empty weight			*/
	unsigned short	stick_weight;	/* stick weight			*/
} PRR00091T;

typedef struct prr00092t {
	unsigned short	stable_time;	/* stable time constant		*/
	unsigned char	zero_interval;	/* auto zero interval		*/
	unsigned char	nof_sect;	/* number of sect		*/
	struct prr00091t	sect[8];/* section parameter		*/
} PRR00092T;

/*------------------------------------------------------*/
/*	秤計量特性パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0009t {
	unsigned char	nof_adc;	/* number of ADC board		*/
	struct prr00090t	adc[4];	/* ADC parameter		*/
	unsigned char	nof_ch;		/* number of channel		*/
	struct prr00092t	ch[4];	/* channel parameter		*/
} PRR0009T;

#if	defined(STORAGE)
struct prr0009t	prr0009s;
#else
#if	!defined(NOEXTERN)
extern struct prr0009t	prr0009s;
#endif
#endif

#endif
