/*======================================================================
 * File name    : rzr017.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr017.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr017.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr017.hv  2.1  93/11/02 16:57:10  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr017.hv
 * Revision 2.1  93/11/02 16:57:10  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR017_H)
#define	RZR017_H

typedef struct rzr0170t {
	unsigned char	delay;		/* RS遅れ時間			*/
	unsigned char	pool;		/* RSプール時間			*/
} RZR0170T;

typedef struct rzr0172t {
	unsigned char	delay;		/* 遅れ時間			*/
	unsigned char	on;		/* オン時間			*/
} RZR0172T;

typedef struct rzr0171t {
	unsigned long	assign_ptn;	/* Section assign pattern	*/
	unsigned char	wh_ph;		/* WH-PH遅れ時間		*/
	unsigned char	ph_rf;		/* PH-RF遅れ時間		*/
	unsigned char	wh1_bh;		/* WH1-MH遅れ時間		*/
	unsigned char	bh_wh2;		/* MH-WH2遅れ時間		*/
	unsigned char	stagger;	/* セクション内時差排出時間	*/
	unsigned char	sect_stagger;	/* セクション間時差排出時間	*/
	unsigned char	bh_on;		/* BHオン時間			*/
	unsigned char	wh_on;		/* WHオン時間			*/
	unsigned char	ph_on;		/* PHオン時間			*/
} RZR0171T;

/*------------------------------------------------------*/
/*	タイミングパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	ds_delay;	/* 排出信号遅れ時間		*/
	unsigned char	ds_hold_tm;	/* dump confirm signal hold time*/
	unsigned char	err_delay;	/* エラー信号遅れ時間		*/
	unsigned char	es_hold_tm;	/* error signal hold time	*/
	unsigned char	wh_delay;	/* WH遅れ時間			*/
	unsigned char	nof_round;	/* ラウンディンググループ数	*/
	unsigned char	round_base[8];	/* ラウンディング排出基本時間	*/
	unsigned char	min_th_no;	/* 最小TH番号			*/
	unsigned char	nof_th;		/* 該当するchのTH数		*/
	struct rzr0172t	th[8];		/* TH parameter[nof_th]		*/
	unsigned char	min_dth_no;	/* 最小DTH番号			*/
	unsigned char	nof_dth;	/* 該当するchのDTH数		*/
	struct rzr0172t	dth[8];		/* DTH parameter[nof_dth]	*/
	unsigned char	min_rs_no;	/* 最小RS番号			*/
	unsigned char	nof_rs;		/* 該当するchのRS数		*/
	struct rzr0170t	rs[8];		/* RS paramter[nof_rs]		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct rzr0171t	sect[8];	/* section paramter[nof_sect]	*/
} RZR017T;

#if	defined(STORAGE)
struct rzr017t	rzr017s;
#else
#if	!defined(NOEXTERN)
extern struct rzr017t	rzr017s;
#endif
#endif

#endif
