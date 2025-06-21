/*======================================================================
 * File name	: prr2009.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＰＨセルデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr2009.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr2009.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR2009_H)
#define	PRR2009_H

enum prr20090e {			/* ＰＨスパン調整	*/
	PH_SPAN_ADJ_OFF,		/* しない		*/
	PH_SPAN_ADJ_ON,			/* する			*/
};

enum prr20091e {			/* ＰＨの重量監視			*/
	PH_WT_CHECK_OFF,		/* しない				*/
	PH_WT_CHECK_ON,			/* する					*/
};

typedef struct prr20091t {
	unsigned short	empty_weight;	/* empty weight		*/
	unsigned short	stick_weight;	/* stick weight		*/
	enum prr20091e	ph_weight_check;/* PHの重量監視		*/
} PRR20091T;

typedef struct prr20092t {
	unsigned short	stable_time;	/* stable time constant	*/
	unsigned char	zero_interval;	/* auto zero interval	*/
	unsigned char	nof_sect;	/* number of sect	*/
	PRR20091T	sect[8];	/* section parameter	*/
} PRR20092T;

/*------------------------------------------------------*/
/*	秤計量特性パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr2009t {
	enum prr20090e	ph_span_adj;	/* PH span adjustment	*/
	unsigned char	nof_ch;		/* number of channel	*/
	PRR20092T	ch[4];		/* channel parameter	*/
} PRR2009T;

#if	defined(STORAGE)
struct prr2009t	prr2009s;
#else
#if	!defined(NOEXTERN)
extern struct prr2009t	prr2009s;
#endif
#endif

#endif
