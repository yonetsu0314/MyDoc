/*======================================================================
 * File name	: prr0023.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0023.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0023.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/RCS/rzr023.h 2.2 1994/11/16 19:32:39 hiroe Exp
 *log: rzr023.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0023_H)
#define	PRR0023_H

enum prr00230e {			/* 単重更新			*/
	PNWPW_OFF,			/* しない			*/
	PNWPW_1_1,			/* する(1/1)			*/
	PNWPW_1_2,			/* する(1/2)			*/
	PNWPW_1_4,			/* する(1/4)			*/
};

enum prr00231e {			/* ミックス補正			*/
	MIX_OFF,			/* しない			*/
	MIX_ON,				/* する（順次補正）		*/
	MIX_7X7,			/* 相互補正			*/
};

enum prr00232e {			/* 平均値制御			*/
	AVE_OFF,			/* しない			*/
	AVE_EEC,			/* する(EEC)			*/
	AVE_USA,			/* する(USA)			*/
};

enum prr00233e {			/* 第２上限排出			*/
	SECOND_UPPER_DISABLE,		/* 無効				*/
	SECOND_UPPER_ENABLE,		/* 有効				*/
};

typedef struct prr00230t {
	unsigned long	assign_ptn;	/* Section assign pattern	*/
	long		target_wt;	/* 計量目標重量			*/
	unsigned short	upper_wt;	/* 上限重量			*/
	unsigned short	upper_wt2;	/* 第２上限重量			*/
	unsigned short	tne;		/* T.N.E			*/
	short		target_cnt;	/* 計量目標個数			*/
	unsigned char	upper_cnt;	/* 上限個数			*/
	unsigned char	upper_cnt2;	/* 第２上限個数			*/
	unsigned char	lower_cnt;	/* 下限個数			*/
	unsigned short	piece_wt;	/* 単重				*/
	unsigned char	target_head;	/* 目標選択ヘッド数		*/
	enum prr00230e	pw_ctrl;	/* 単重自動更新モード		*/
	enum prr00231e	mix_ctrl;	/* 混合補正モード		*/
	unsigned char	cnt;		/* 優先参加回数			*/
} PRR00230T;

/*------------------------------------------------------*/
/*	計量目標値のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0023t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct prr00230t	sect[8];/* section parameter[nof_sect]	*/
	long		target_wt;	/* チャネル計量目標重量		*/
	unsigned short	upper_wt;	/* チャネル上限重量		*/
	unsigned short	upper_wt2;	/* チャネル第２上限重量		*/
	unsigned short	tne;		/* チャネルT.N.E		*/
	short		target_cnt;	/* チャネル計量目標個数		*/
	unsigned char	upper_cnt;	/* チャネル上限個数		*/
	unsigned char	upper_cnt2;	/* チャネル第２上限個数		*/
	unsigned char	lower_cnt;	/* チャネル下限個数		*/
	unsigned short	speed;		/* 計量能力			*/
	unsigned char	dump_cnt;	/* 排出回数			*/
	enum prr00232e	avg_ctrl;	/* 平均値制御モード		*/
	unsigned short	second_upper_cycle;/* 第２上限排出サイクル	*/
	unsigned char	cyc_of_under;	/* 計量発生頻度			*/
} PRR0023T;

#if	defined(STORAGE)
struct prr0023t	prr0023s;
#else
#if	!defined(NOEXTERN)
extern struct prr0023t	prr0023s;
#endif
#endif

#endif
