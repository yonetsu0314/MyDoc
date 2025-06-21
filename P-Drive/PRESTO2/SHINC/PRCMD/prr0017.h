/*======================================================================
 * File name	: prr0017.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	タイミングデータ構造
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prr0017.h 1.1 2003-11-28 23:55:29+09 kawamura Exp $
 *$Log: prr0017.h $
 *Revision 1.1  2003-11-28 23:55:29+09  kawamura
 *初期リビジョン
 *
 *----------------------------------------------------------------------
 *header: rzr017.hv  2.1  93/11/02 16:57:10  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr017.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0017_H)
#define	PRR0017_H

typedef struct prr00170t {
	unsigned char	delay;		/* RS遅れ時間			*/
	unsigned char	pool;		/* RSプール時間			*/
} PRR00170T;

typedef struct prr00172t {
	unsigned char	delay;		/* 遅れ時間			*/
	unsigned char	on;		/* オン時間			*/
} PRR00172T;

typedef struct prr00171t {
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
} PRR00171T;

/*------------------------------------------------------*/
/*	タイミングパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	ds_delay;	/* dump confirm signal delay	*/
	unsigned char	ds_hold_tm;	/* dump confirm signal hold time*/
	unsigned char	op1_delay;	/* option signal 1 delay	*/
	unsigned char	op1_hold_tm;	/* option signal 1 hold time	*/
	unsigned char	op2_delay;	/* option signal 2 delay	*/
	unsigned char	op2_hold_tm;	/* option signal 2 hold time	*/
	unsigned char	wh_delay;	/* WH遅れ時間			*/
	unsigned char	nof_round;	/* ラウンディンググループ数	*/
	unsigned char	round_base[8];	/* ラウンディング排出基本時間	*/
	unsigned char	min_th_no;	/* 最小TH番号			*/
	unsigned char	nof_th;		/* 該当するchのTH数		*/
	struct prr00172t	th[16];	/* TH parameter[nof_th]		*/
	unsigned char	min_dth_no;	/* 最小DTH番号			*/
	unsigned char	nof_dth;	/* 該当するchのDTH数		*/
	struct prr00172t	dth[16];/* DTH parameter[nof_dth]	*/
	unsigned char	min_rs_no;	/* 最小RS番号			*/
	unsigned char	nof_rs;		/* 該当するchのRS数		*/
	struct prr00170t	rs[16];	/* RS parameter[nof_rs]		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	struct prr00171t	sect[8];/* section parameter[nof_sect]	*/
} PRR0017T;

#if	defined(STORAGE)
struct prr0017t	prr0017s;
#else
#if	!defined(NOEXTERN)
extern struct prr0017t	prr0017s;
#endif
#endif

#endif
