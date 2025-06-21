/*======================================================================
 * File name    : rzr007.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	包装機仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr007.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr007.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr007.h 2.2 1970/01/01 00:00:00 kawamura
 *Log: rzr007.h
 * リビジョン 2.2  1970/01/01  00:00:00  kawamura
 * 2002/01/24 10:58
 * 連動方式に特殊連動１を追加する。
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR007_H)
#define	RZR007_H

enum rzr0070e {				/* 連動方式			*/
	INTFACE_SLV,			/* スレーブ			*/
	INTFACE_MST,			/* マスター			*/
	INTFACE_SOD,			/* ストロークオンデマンド	*/
	INTFACE_BOD,			/* バッグオンデマンド		*/
	INTFACE_SP1,			/* 特殊連動１（古川連動）	*/
};

enum rzr0071e {				/* 回数計量時連動信号		*/
	MLTDMPINI_SELF,			/* 自走（初回のみ）		*/
	MLTDMPINI_SYNC,			/* 同期（毎回入力）		*/
};

enum rzr0072e {				/* 回数計量時排出信号		*/
	MLTDMPCNF_FRST,			/* 初回				*/
	MLTDMPCNF_LAST,			/* 最終回			*/
};

enum rzr0073e {				/* 手動時連動信号		*/
	MANDMPINI_SELF,			/* 自走				*/
	MANDMPINI_SYNC,			/* 同期				*/
};

enum rzr0074e {				/* 手動時排出信号		*/
	MANDMPCNF_OFF,			/* なし				*/
	MANDMPCNF_ON,			/* あり				*/
};

typedef struct rzr0070t {
	unsigned long	bm_assign_ptn;	/* bag maker assign pattern
					   [nof_cf][nof_head/4]		*/
	enum rzr0070e	interface;	/* I/F type			*/
	enum rzr0071e	mul_dump_ini;	/* multi dump initiate mode	*/
	enum rzr0072e	mul_dump_conf;	/* multi dump confirm mode	*/
	enum rzr0073e	manu_dump_ini;	/* manual dump initiate mode	*/
	enum rzr0074e	manu_dump_conf;	/* manual dump confirm mode	*/
} RZR0070T;

/*------------------------------------------------------*/
/*	包装機連動パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr007t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_bm;		/* number of bag maker		*/
	struct rzr0070t	ch[2];		/* Bag paramter[nof_bm]		*/
} RZR007T;

#if	defined(STORAGE)
struct rzr007t	rzr007s;
#else
#if	!defined(NOEXTERN)
extern struct rzr007t	rzr007s;
#endif
#endif

#endif
