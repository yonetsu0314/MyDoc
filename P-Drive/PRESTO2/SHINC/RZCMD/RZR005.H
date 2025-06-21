/*======================================================================
 * File name    : rzr005.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	供給機仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr005.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr005.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr005.hv  2.1  93/11/02 15:41:36  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr005.hv
 * Revision 2.1  93/11/02 15:41:36  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR005_H)
#define	RZR005_H

enum rzr0050e {				/* 供給制御方式			*/
	IFDDTCT_WGHT,			/* 重量（ロードセル）		*/
	IFDDTCT_HGHT,			/* 高さ（光電子スイッチ）	*/
};

typedef struct rzr0050t {
	unsigned long	sig_assign_ptn;	/* infeed signal assign pptern
					   [nof_head/4]			*/
	enum rzr0050e	infd_detector;	/* level sensor type		*/
	unsigned char	transient_int;	/* transient interrupt time	*/
	unsigned char	feed_intrpt;	/* low product time		*/
} RZR0050T;

/*------------------------------------------------------*/
/*	供給機の制御仕様パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/
typedef struct 	rzr005t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sig;	/* number of infeed signal	*/
	struct rzr0050t	sig[8];		/*                   		*/
	unsigned char	nof_cf;		/* number of CF			*/
	unsigned long	cf_assign_ptn[8];	/* CF assign pattern
					   [nof_cf][nof_head/4]		*/
	int	pconst;			/* proportion constant		*/
	int	iconst;			/* integral constant		*/
	int	dconst;			/* differrential constant	*/
	int	filter_sw;		/* filter switch		*/
	int	filter_const;		/* filter constant		*/
} RZR005T;

#if	defined(STORAGE)
struct rzr005t	rzr005s;
#else
#if	!defined(NOEXTERN)
extern struct rzr005t	rzr005s;
#endif
#endif

#endif
