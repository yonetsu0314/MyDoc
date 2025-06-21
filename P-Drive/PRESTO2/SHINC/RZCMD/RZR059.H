/*======================================================================
 * File name    : rzr059.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	親ヘッド計量目標データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr059.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr059.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzr059.h 1.1 1994/02/18 13:45:32 ryoji
 *Log: rzr059.h
 * リビジョン 1.1  1994/02/18  13:45:32  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR059_H)
#define	RZR059_H

enum rzr0590e {				/* 親ヘッドＲＦ駆動		*/
	PRF_OFF,			/* しない			*/
	PRF_ON				/* する				*/
};

enum rzr0591e {				/* 親子計量			*/
	PANDC_OFF,			/* しない			*/
	PANDC_ON			/* する				*/
};

enum rzr0592e {				/* 排出方法			*/
	PDMP_ALT,			/* 交互				*/
	PDMP_SIM			/* 同時				*/
};

enum rzr0593e {				/* 計算参加			*/
	PCAL_OFF,			/* しない			*/
	PCAL_ON				/* する				*/
};

enum rzr0594e {				/* 上限以上時の処理		*/
	POVER_OS,			/* オーバースケール処理による	*/
	POVER_PDMP,			/* 親のみ排出			*/
	POVER_PCDMP			/* 親子ミックス排出		*/
};

enum rzr0595e {				/* 下限未満時の処理		*/
	PUNDER_ADD,			/* 追加供給			*/
	PUNDER_PDMP			/* 親のみ排出			*/
};

typedef struct rzr0590t {
	unsigned char	no;		/* ヘッド番号			*/
	unsigned short	upper_wt;	/* 上限重量			*/
	unsigned short	lower_wt;	/* 下限重量			*/
	enum rzr0590e	rf_ctrl;	/* ＲＦ駆動			*/
	unsigned char	ph_fd;		/* PH-供給信号遅れ時間		*/
} RZR0590T;

typedef struct rzr0591t {
	unsigned char	nof_phead;	/* number of parent head	*/
	struct rzr0590t	phead[2];	/* PI591 paramter[nof_phead]	*/
	enum rzr0591e	on_off;		/* 親子計量する／しない		*/
	enum rzr0592e	dump;		/* 同時排出／交互排出		*/
	enum rzr0593e	calc;		/* 計算参加／不参加		*/
	unsigned short	az_intvl;	/* 自動零点調整間隔		*/
	enum rzr0594e	over;		/* 上限以上時の処理		*/
	enum rzr0595e	under;		/* 下限未満時の処理		*/
} RZR0591T;

/*------------------------------------------------------*/
/*	親ヘッド計量目標値のパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr059t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr0591t	sect[8];	/* PR592 paramter[nof_sect]	*/
} RZR059T;

#if	defined(STORAGE)
struct rzr059t	rzr059s;
#else
#if	!defined(NOEXTERN)
extern struct rzr059t	rzr059s;
#endif
#endif

#endif
