/*======================================================================
 * File name    : rzr023.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量目標データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr023.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr023.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzr023.h 2.2 1994/11/16 19:32:39 hiroe
 *Log: rzr023.h
 * リビジョン 2.2  1994/11/16  19:32:39  hiroe
 * ミックス補正に相互補正を追加する(MIX_7x7)
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR023_H)
#define	RZR023_H

enum rzr0230e {				/* 単重更新			*/
	PNWPW_OFF,			/* しない			*/
	PNWPW_1_1,			/* する(1/1)			*/
	PNWPW_1_2,			/* する(1/2)			*/
	PNWPW_1_4,			/* する(1/4)			*/
};

enum rzr0231e {				/* ミックス補正			*/
	MIX_OFF,			/* しない			*/
	MIX_ON,				/* する（順次補正）		*/
	MIX_7X7,			/* 相互補正			*/
};

enum rzr0232e {				/* 平均値制御			*/
	AVE_OFF,			/* しない			*/
	AVE_EEC,			/* する(EEC)			*/
	AVE_USA,			/* する(USA)			*/
};

typedef struct rzr0230t {
	unsigned long	assign_ptn;	/* Section assign pattern	*/
	long		target_wt;	/* 計量目標重量			*/
	unsigned short	upper_wt;	/* 上限重量			*/
	unsigned short	tne;		/* T.N.E			*/
	short		target_cnt;	/* 計量目標個数			*/
	unsigned char	upper_cnt;	/* 上限個数			*/
	unsigned char	lower_cnt;	/* 下限個数			*/
	unsigned short	piece_wt;	/* 単重				*/
	unsigned char	target_head;	/* 目標選択ヘッド数		*/
	enum rzr0230e	pw_ctrl;	/* 単重自動更新モード		*/
	enum rzr0231e	mix_ctrl;	/* 混合補正モード		*/
} RZR0230T;

/*------------------------------------------------------*/
/*	計量目標値のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr023t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr0230t	sect[8];	/* section paramter[nof_sect]	*/
	long		target_wt;	/* チャネル計量目標重量		*/
	unsigned short	upper_wt;	/* チャネル上限重量		*/
	unsigned short	tne;		/* チャネルT.N.E		*/
	short		target_cnt;	/* チャネル計量目標個数		*/
	unsigned char	upper_cnt;	/* チャネル上限個数		*/
	unsigned char	lower_cnt;	/* チャネル下限個数		*/
	unsigned short	speed;		/* 計量能力			*/
	unsigned char	dump_cnt;	/* 排出回数			*/
	enum rzr0232e	avg_ctrl;	/* 平均値制御モード		*/
	unsigned char	cyc_of_under;	/* 計量発生頻度			*/
} RZR023T;

#if	defined(STORAGE)
struct rzr023t	rzr023s;
#else
#if	!defined(NOEXTERN)
extern struct rzr023t	rzr023s;
#endif
#endif

#endif
