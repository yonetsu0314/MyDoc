/*======================================================================
 * File name    : rzr003.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	計量機仕様データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr003.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr003.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr003.hv  2.1  93/11/02 20:25:44  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr003.hv
 * Revision 2.1  93/11/02 20:25:44  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR003_H)
#define	RZR003_H

enum rzr0030e {				/* ヘッド配置			*/
	HDCNST_CIRC,			/* 円形				*/
	HDCNST_LINE,			/* 直線				*/
};

enum rzr0031e {				/* 計量方式			*/
	MODEL_WS,			/* Ｗ－Ｓ			*/
	MODEL_WT,			/* Ｗ－Ｔ			*/
	MODEL_BS,			/* Ｂ－Ｓ			*/
	MODEL_BD,			/* Ｂ－Ｄ			*/
	MODEL_PS,			/* Ｐ－Ｓ			*/
	MODEL_PD,			/* Ｐ－Ｄ			*/
};

enum rzr0032e {				/* マルチ連動			*/
	MLTINTLK_OFF,			/* しない			*/
	MLTINTLK_MST,			/* マスター			*/
	MLTINTLK_SLV,			/* スレーブ			*/
};

typedef struct rzr0030t {
	unsigned long	assign_ptn;	/* hopper assign pptern
					   [nof_head/4]			*/
	unsigned char	drv_type;	/* drive type			*/
	unsigned char	direction;	/* 正不良、２方向識別子		*/
	unsigned char	level;		/* 段数(WHからの位置)		*/
	unsigned char	port;		/* EXCポート番号		*/
} RZR0030T;

typedef struct 	rzr003t {
	enum rzr0030e	head_construct;		/* radial/linear	*/
	unsigned char	nof_head;		/* number of head	*/
	enum rzr0031e	model_id;		/* model ID		*/
	unsigned char	nof_df;			/* number of DF		*/
	unsigned long	df_assign_ptn[8];	/* DF loacate pattern
						[nof_df]		*/
	unsigned char	nof_rs;			/* number of RS		*/
	struct rzr0030t	rs[8];			/* P0 parameter of RS	*/
	unsigned char	nof_dth;		/* number of DTH	*/
	struct rzr0030t	dth[8];			/* P0 parameter of DTH	*/
	unsigned char	nof_th;			/* number of TH		*/
	struct rzr0030t	th[8];			/* P0 parameter of TH	*/
	unsigned char	nof_duc_net;		/* number of duc net	*/
	unsigned long	duc_assign_ptn[2];	/* DUC network pattern
					   		[nof_duc_net]	*/
	enum rzr0032e	mlt_intlk_mode;		/* multi interlock mode	*/
	unsigned char	shift_cnt;		/* shift count		*/
} RZR003T;

#if	defined(STORAGE)
struct rzr003t	rzr003s;
#else
#if	!defined(NOEXTERN)
extern struct rzr003t	rzr003s;
#endif
#endif

#endif
