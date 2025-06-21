/*======================================================================
 * File name	: prr0003.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	Œv—Ê‹@d—lƒf[ƒ^\‘¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/rcs/prr0003.h 1.3 1970/01/01 00:00:00 hiroe Exp $
 *$Log: prr0003.h $
 * ƒŠƒrƒWƒ‡ƒ“ 1.3  1970/01/01  00:00:00  hiroe
 * 2007/01/15
 * DB-S,DB-D‚Ìƒ‚ƒfƒ‹ID‚ğ’Ç‰Á‚·‚éB
 * 
 * ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  kawamura
 * 2004/03/31  12:07
 * WB-S,WB-D‚Ìƒ‚ƒfƒ‹ID‚ğ’Ç‰Á‚·‚éB
 * 
 * ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  kawamura
 * ‰ŠúƒŠƒrƒWƒ‡ƒ“
 * 
 *----------------------------------------------------------------------
 *header: rzr003.hv  2.1  93/11/02 20:25:44  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr003.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0003_H)
#define	PRR0003_H

enum prr00030e {			/* ƒwƒbƒh”z’u			*/
	HDCNST_CIRC,			/* ‰~Œ`				*/
	HDCNST_LINE,			/* ’¼ü				*/
};

enum prr00031e {			/* Œv—Ê•û®			*/
	MODEL_WS,			/* ‚v|‚r			*/
	MODEL_WT,			/* ‚v|‚s			*/
	MODEL_BS,			/* ‚a|‚r			*/
	MODEL_BD,			/* ‚a|‚c			*/
	MODEL_PS,			/* ‚o|‚r			*/
	MODEL_PD,			/* ‚o|‚c			*/
	MODEL_WD,			/* ‚v|‚c			*/
	MODEL_WBS,			/* ‚v‚a|‚r			*/
	MODEL_WBD,			/* ‚v‚a|‚c			*/
	MODEL_DBS,			/* ‚c‚a|‚r			*/
	MODEL_DBD,			/* ‚c‚a|‚c			*/
};

typedef struct prr00030t {
	unsigned long	assign_ptn;	/* hopper assign pptern		*/
	unsigned char	drv_type;	/* drive type			*/
	unsigned char	direction;	/* ³•s—ÇA‚Q•ûŒü¯•Êq		*/
	unsigned char	level;		/* ’i”(WH‚©‚ç‚ÌˆÊ’u)		*/
	unsigned char	port;		/* EXCƒ|[ƒg”Ô†		*/
} PRR00030T;

typedef struct	prr0003t {
	enum prr00030e	head_construct;		/* radial/linear	*/
	unsigned char	nof_head;		/* number of head	*/
	enum prr00031e	model_id;		/* model ID		*/
	unsigned char	nof_df;			/* number of DF		*/
	unsigned long	df_assign_ptn[8];	/* DF assign pattern	*/
	unsigned char	nof_af;			/* number of DF		*/
	unsigned long	af_assign_ptn[32];	/* AF assign pattern	*/
	unsigned char	nof_rs;			/* number of RS		*/
	struct prr00030t	rs[16];		/* parameter of RS	*/
	unsigned char	nof_dth;		/* number of DTH	*/
	struct prr00030t	dth[16];	/* parameter of DTH	*/
	unsigned char	nof_th;			/* number of TH		*/
	struct prr00030t	th[16];		/* parameter of TH	*/
	unsigned char	nof_duc_net;		/* number of duc net	*/
	unsigned long	duc_assign_ptn[2];	/* DUC network pattern	*/
} PRR0003T;

#if	defined(STORAGE)
struct prr0003t	prr0003s;
#else
#if	!defined(NOEXTERN)
extern struct prr0003t	prr0003s;
#endif
#endif

#endif
