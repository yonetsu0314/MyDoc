/*======================================================================
 * File name	: prr0007.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	包装機仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0007.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0007.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr007.h 2.2 1970/01/01 00:00:00 kawamura Exp
 *log: rzr007.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0007_H)
#define	PRR0007_H

enum prr00070e {			/* 連動方式			*/
	INTFACE_SLV,			/* スレーブ			*/
	INTFACE_MST,			/* マスター			*/
	INTFACE_SOD,			/* ストロークオンデマンド	*/
	INTFACE_BOD,			/* バッグオンデマンド		*/
	INTFACE_SP1,			/* 特殊連動１			*/
};

enum prr00071e {			/* 回数計量時連動信号		*/
	MLTDMPINI_SELF,			/* 自走（初回のみ）		*/
	MLTDMPINI_SYNC,			/* 同期（毎回入力）		*/
};

enum prr00072e {			/* 回数計量時排出信号		*/
	MLTDMPCNF_FRST,			/* 初回				*/
	MLTDMPCNF_LAST,			/* 最終回			*/
};

enum prr00073e {			/* 手動時連動信号		*/
	MANDMPINI_SELF,			/* 自走				*/
	MANDMPINI_SYNC,			/* 同期				*/
};

enum prr00074e {			/* 手動時排出信号		*/
	MANDMPCNF_OFF,			/* なし				*/
	MANDMPCNF_ON,			/* あり				*/
};

typedef struct prr00070t {
	unsigned long	bm_assign_ptn;	/* bag maker assign pattern	*/
	enum prr00070e	interface;	/* I/F type			*/
	enum prr00071e	mul_dump_ini;	/* multi dump initiate mode	*/
	enum prr00072e	mul_dump_conf;	/* multi dump confirm mode	*/
	enum prr00073e	manu_dump_ini;	/* manual dump initiate mode	*/
	enum prr00074e	manu_dump_conf;	/* manual dump confirm mode	*/
} PRR00070T;

/*------------------------------------------------------*/
/*	包装機連動パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0007t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_bm;		/* number of bag maker		*/
	struct prr00070t	ch[4];	/* Bag parameter[nof_bm]	*/
} PRR0007T;

#if	defined(STORAGE)
struct prr0007t	prr0007s;
#else
#if	!defined(NOEXTERN)
extern struct prr0007t	prr0007s;
#endif
#endif

#endif
