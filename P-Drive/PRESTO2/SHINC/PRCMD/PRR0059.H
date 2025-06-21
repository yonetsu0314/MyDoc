/*======================================================================
 * File name	: prr0059.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0059.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0059.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2005/06/30  12:27
 * 補助組み合わせに対応する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzr059.h 1.1 1994/02/18 13:45:32 ryoji Exp
 *log: rzr059.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0059_H)
#define	PRR0059_H

enum prr00590e {			/* 親ヘッドＲＦ駆動		*/
	PRF_OFF,			/* しない			*/
	PRF_ON				/* する				*/
};

enum prr00591e {			/* 親子計量			*/
	PANDC_OFF,			/* しない			*/
	PANDC_ON,			/* する				*/
	PANDC_ASS			/* 補助組み合わせ		*/
};

enum prr00592e {			/* 排出方法			*/
	PDMP_ALT,			/* 交互				*/
	PDMP_SIM			/* 同時				*/
};

enum prr00593e {			/* 計算参加			*/
	PCAL_OFF,			/* しない			*/
	PCAL_ON				/* する				*/
};

enum prr00594e {			/* 上限以上時の処理		*/
	POVER_OS,			/* オーバースケール処理による	*/
	POVER_PDMP,			/* 親のみ排出			*/
	POVER_PCDMP			/* 親子ミックス排出		*/
};

enum prr00595e {			/* 下限未満時の処理		*/
	PUNDER_ADD,			/* 追加供給			*/
	PUNDER_PDMP			/* 親のみ排出			*/
};

typedef struct prr00590t {
	unsigned char	no;		/* ヘッド番号			*/
	unsigned short	upper_wt;	/* 上限重量			*/
	unsigned short	lower_wt;	/* 下限重量			*/
	enum prr00590e	rf_ctrl;	/* ＲＦ駆動			*/
	unsigned char	ph_fd;		/* PH-供給信号遅れ時間		*/
} PRR00590T;

typedef struct prr00591t {
	unsigned char	nof_phead;	/* number of parent head	*/
	struct prr00590t	phead[2];/* parameter[nof_phead]	*/
	enum prr00591e	on_off;		/* 親子計量する／しない		*/
	enum prr00592e	dump;		/* 同時排出／交互排出		*/
	enum prr00593e	calc;		/* 計算参加／不参加		*/
	unsigned short	az_intvl;	/* 自動零点調整間隔		*/
	enum prr00594e	over;		/* 上限以上時の処理		*/
	enum prr00595e	under;		/* 下限未満時の処理		*/
} PRR00591T;

/*------------------------------------------------------*/
/*	親ヘッド計量目標値のパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0059t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct prr00591t	sect[8];/* parameter[nof_sect]		*/
} PRR0059T;

#if	defined(STORAGE)
struct prr0059t	prr0059s;
#else
#if	!defined(NOEXTERN)
extern struct prr0059t	prr0059s;
#endif
#endif

#endif
