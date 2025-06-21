/*======================================================================
 * File name	: prr0005.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0005.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0005.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr005.hv  2.1  93/11/02 15:41:36  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr005.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0005_H)
#define	PRR0005_H

enum prr00050e {			/* 供給制御方式			*/
	IFDDTCT_WGHT,			/* 重量（ロードセル）		*/
	IFDDTCT_HGHT,			/* 高さ（光電子スイッチ）	*/
};

typedef struct prr00050t {
	unsigned long	sig_assign_ptn;	/* infeed signal assign pptern	*/
	enum prr00050e	infd_detector;	/* level sensor type		*/
	unsigned char	transient_int;	/* transient interrupt time	*/
	unsigned char	feed_intrpt;	/* low product time		*/
	unsigned short	targ;		/* 目標重量			*/
	unsigned char	upper;		/* 上限偏差(%)			*/
	unsigned char	lower;		/* 下限偏差(%)			*/
} PRR00050T;

/*------------------------------------------------------*/
/*	供給機の制御仕様パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/
typedef struct	prr0005t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sig;	/* number of infeed signal	*/
	struct prr00050t	sig[8];	/*				*/
	unsigned char	nof_cf;		/* number of CF			*/
	unsigned long	cf_assign_ptn[8];/* CF assign pattern		*/
	short	pconst;			/* proportion constant		*/
	short	iconst;			/* integral constant		*/
	short	dconst;			/* differrential constant	*/
	short	filter_sw;		/* filter switch		*/
	short	filter_const;		/* filter constant		*/
} PRR0005T;

#if	defined(STORAGE)
struct prr0005t	prr0005s;
#else
#if	!defined(NOEXTERN)
extern struct prr0005t	prr0005s;
#endif
#endif

#endif
