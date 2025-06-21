/*======================================================================
 * File name	: prr6023.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	セミオート機計量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr6023.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr6023.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/12/06  15:30
 * ＢＨ非同期供給のON/OFFを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR6023_H)
#define	PRR6023_H

enum prr60230e {			/* 排出方向				*/
	DIR_CNTER,			/* 0:when MIX or SINGLE,dump center	*/
	DIR_C1_MAX_HEAD = DIR_CNTER,	/* 0:when C1,dump max head side		*/
	DIR_C2_MIN_HEAD = DIR_CNTER,	/* 0:when C2,dumo min head side		*/
	DIR_OUT_SIDE,			/* 1:when MIX or SINGLE,dump out side	*/
	DIR_C1_MIN_HEAD = DIR_OUT_SIDE,	/* 1:when C1,dump min head side		*/
	DIR_C2_MAX_HEAD = DIR_OUT_SIDE,	/* 1:when C2,dump max head side		*/
};

enum prr60231e {			/* ＷＨへの非同期供給			*/
	WH_ASYNC_OFF,			/* しない				*/
	WH_ASYNC_ON,			/* する					*/
};

enum prr60232e {			/* ＢＨのみで組み合わせ計算		*/
	BH_ONLY_CAL_OFF,		/* しない				*/
	BH_ONLY_CAL_ON,			/* する					*/
};

enum prr60233e {			/* 集合コンベヤ正逆転動作		*/
	CONV_REV_OFF,			/* しない				*/
	CONV_REV_ON,			/* する					*/
};

enum prr60234e {			/* ＢＨへの非同期供給			*/
	BH_ASYNC_OFF,			/* しない				*/
	BH_ASYNC_ON,			/* する					*/
};

typedef struct prr60230t {
	enum prr60231e	wh_async_supply;/* WHへの非同期供給			*/
	enum prr60232e	only_bh_cal;	/* BHのみで組み合わせ計算		*/
	enum prr60234e	bh_async_supply;/* BHへの非同期供給			*/
} PRR60230T;

/*------------------------------------------------------*/
/*	セミオート機計量データのパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr6023t {
	unsigned char	ch_id;		/* channel number		*/
	enum prr60230e	dump_dir;	/* dump direction		*/
	enum prr60233e	conv_rev;	/* 集合コンベヤ正逆転動作	*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	PRR60230T	sect[8];	/* section parameter[nof_sect]	*/
} PRR6023T;

#if	defined(STORAGE)
struct prr6023t	prr6023s;
#else
#if	!defined(NOEXTERN)
extern struct prr6023t	prr6023s;
#endif
#endif

#endif
