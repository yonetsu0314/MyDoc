/*======================================================================
 * File name	: prr0133.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	新ＡＦＤデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0133.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0133.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0133_H)
#define	PRR0133_H

enum prr01330e {			/* 清掃要求				*/
	CLN_REQ_OFF,			/* 清掃要求しない			*/
	CLN_REQ_ON,			/* 清掃要求する				*/
};

enum prr01331e {			/* 供給過大状態時間操作			*/
	DF_OW_OFF,			/* 供給過大状態時間操作しない		*/
	DF_OW_ON,			/* 供給過大状態時間操作する		*/
};

enum prr01332e {			/* 分散重量制御				*/
	DFW_CTRL_OFF,			/* 分散重量制御しない			*/
	DFW_CTRL_ON,			/* 分散重量制御する			*/
};

typedef struct prr01330t {
	char		tbl[15];	/* fuzzy table				*/
	char		zero_d_num;	/* 空検出時操作量			*/
	char		p_os_c_num;	/* オーバースケール検出時操作量		*/
	unsigned char	trb_off_head;	/* ヘッド数低下停止ヘッド数		*/
	enum prr01330e	cln_req_sw;	/* 清掃要求検知				*/
	unsigned short	good_effi;	/* 高稼働率判定値			*/
} PRR01330T;

typedef struct prr01331t {
	unsigned char	nof_sect;	/* 該当するchのセクション数		*/
	PRR01330T	sect[8];	/* AFD parameter[nof_sect]		*/
} PRR01331T;

typedef struct prr01332t {
	short		df_fuzzy_gain;	/* ファジーテーブル操作ゲイン		*/
	unsigned short	df_l_detect;	/* 分散操作許容サイクル数		*/
	enum prr01331e	df_ow_ctrl;	/* 供給過大状態時間操作			*/
	enum prr01332e	dfw_on_off;	/* 分散重量制御ON/OFF			*/
	char		df_e_detect1;	/* 分散操作許容空ヘッド数（バラツキ小）	*/
	char		df_e_detect2;	/* 分散操作許容空ヘッド数（バラツキ大）	*/
	short		df_w_max;	/* 分散重量制御最大値			*/
	short		df_w_min;	/* 分散重量制御最小値			*/
	unsigned char	low_wt;		/* AFD制御停止分散重量			*/
} PRR01332T;

/*------------------------------------------------------*/
/*	ＡＦＤヘッドデータパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0133t {
	unsigned char	nof_ch;		/* number of channnel			*/
	PRR01331T	ch[4];		/* channel parameter			*/
	unsigned char	nof_infeed;	/* number of infeed			*/
	PRR01332T	ifd[8];		/* infeed parameter[nof_infeed]		*/
} PRR0133T;

#if	defined(STORAGE)
struct prr0133t	prr0133s;
#else
#if	!defined(NOEXTERN)
extern struct prr0133t	prr0133s;
#endif
#endif

#endif
