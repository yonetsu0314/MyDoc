/*----------------------------------------------------------------------*
 * File name	: dspdrv.h	/ dsp driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/dspdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dspdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DSPDRV_H)
#define	DSPDRV_H

#define	DSP_AFV_MAX	4
#define	DSP_DF_MAX	4

void		dsp_int_hdr(void);

int		dsp_sys_init(int ( *)());
int		dsp_memrd(void *, short *, short);
int		dsp_memwr(short *, void *, short);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

typedef struct	dsppk0 {			/* 引き数のﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄ	*/
	int		wei_max;		/* 最大ｳｪｲ･ﾍｯﾄﾞ数	*/
	int		afv_max;		/* 最大AFVｾﾙ数		*/
	int		df_max;			/* 最大分散ﾌｨｰﾀﾞ数	*/
	short		*wei_spc;		/* 計量器ｽﾊﾟﾝ係数[]	*/
	short		*afv_spc;		/* DFｽﾊﾟﾝ係数[]		*/
	short		*df_spc;		/* DFｽﾊﾟﾝ係数[]		*/
	long		*afv_fprm;		/* AFV係数微調ﾊﾟﾗﾒｰﾀ[]	*/
	short	 	*x0_ary;		/* 計量器の重心のX座標	*/
	short	 	*x1_ary;		/* AFVの重心のX座標	*/
	short	 	*y0_ary;		/* 計量器の重心のY座標	*/
	short	 	*y1_ary;		/* AFVの重心のY座標	*/
} DSPPK0;

int		dsp_iboot(int, struct dsppk0 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum dspspm {
	SPN_M20	= 1,				/* span:20g		*/
	SPN_M40,				/*	40g		*/
	SPN_M200,				/*	200g		*/
	SPN_M400,				/*	400g		*/
	SPN_M500,				/*	500g		*/
	SPN_M1000,				/*	1000g		*/
};
typedef enum dspspm	DSPSPM;

enum dspsw0 {
	AFV_OFF,				/* AFV OFF		*/
	AFV_AUTO,				/* AFV AUTO		*/
	AFV_ON,					/* AFV ON		*/
};
typedef enum dspsw0	DSPSW0;

typedef struct	dsppk1 {			/* 引き数のﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄ	*/
	int		code;			/* 秤量&ｾﾙ･ﾀｲﾌﾟ		*/
	enum dspspm	spn_mas;		/* span mass		*/
	enum dspsw0	afv_sw;			/* AFV switch		*/
	unsigned short	f3buf_cnt;		/* 3段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数	*/
	unsigned short	f3stb_cnt;		/*   "  安定判別ｶｳﾝﾄ	*/
	unsigned short	f3avg_cnt;		/*   "  平均化処理判定値*/
	unsigned short	f4buf_cnt;		/* 4段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数	*/
	unsigned short	f4stb_cnt;		/*   "  安定判別ｶｳﾝﾄ	*/
	unsigned short	f4avg_cnt;		/*   "  平均化処理判定値*/
	short		*zr_lmt;		/* 零点許容値[9]	*/
	short		*wei_zad;		/* 計量器零点A/D[]	*/
	short		*afv_zad;		/* AFV零点A/Dﾃﾞｰﾀ[]	*/
	short		*df_zad;		/* DF零点A/Dﾃﾞｰﾀ[]	*/
} DSPPK1;

int		dsp_cboot(struct dsppk1 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 3 >>>						*/

enum dspes0 {
	ZER_RELLMT	= -32768,	/* 1回の変化量の許容範囲を超える*/
	ZER_ABSLMT	= 32767,	/* 変化量の絶対許容範囲を超える	*/
};
typedef enum dspes0	DSPES0;

typedef struct	dsprp0 {			/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	unsigned long	err_ptn;		/* error bit pattern	*/
	short		zero_ad[sizeof(long)*8];/* enum dspes0 or A/D	*/
} DSPRP0;

int		dsp_zero(unsigned long *, void *, void *, struct dsprp0 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 3 >>>						*/

enum dspes1 {				/* ｽﾊﾟﾝ係数ｴﾗｰ			*/
	SER_LOLMT	= -32768,	/* 下限未満の場合		*/
	SER_CALC	= -1,		/* 計算ｴﾗｰ			*/
	SER_NOLOAD	= 0,		/* ﾉｰﾛｰﾄﾞ･ｴﾗｰ			*/
	SER_UPLMT	= 32767,	/* 上限を超えた場合		*/
};
typedef enum dspes1	DSPES1;

typedef struct	dsprp1 {			/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	unsigned long	err_ptn;		/* error bit pattern	*/
	short		sp_coef[sizeof(long)*8];/* enum dspes1 or 係数	*/
} DSPRP1;

int		dsp_span(unsigned long *, void *, void *, struct dsprp1 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 5 >>>						*/

#define	DSP_CALERR	(0x80)			/* 計算ｴﾗｰ		*/
#define	DSP_AFV_CMPERR	(0x40)			/* AFV補正ｴﾗｰ		*/
#define	DSP_UNSTBL	(0x20)			/* 秤不安定		*/

typedef struct	dsprp2 {			/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	short		wd3rd[sizeof(long)*8];	/* 3段目ﾃﾞｰﾀ[]		*/
	unsigned short	sts3rd[sizeof(long)*8];	/*    " error status[]	*/
	short		wd4th[sizeof(long)*8];	/* 4段目ﾃﾞｰﾀ[]		*/
	unsigned short	sts4th[sizeof(long)*8];	/*    " error status[]	*/
} DSPRP2;

int		dsp_wei_cnvrt(unsigned long *, void *, void *,
				void *, void *, void *, void *,
				struct dsprp2 *);

int		dsp_wei_read(struct dsprp2 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>						*/

typedef struct	dsprp3 {			/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	unsigned short	stat[DSP_DF_MAX];	/* error status[df_max]	*/
	short		df4th[DSP_DF_MAX];	/* DF4段目ﾃﾞｰﾀ[]	*/
} DSPRP3;

int		dsp_df_cnvrt(void *, void *, struct dsprp3 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 4 >>>						*/

typedef struct	dsprp4 {			/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	unsigned long	err_ptn;		/* error bit pattern	*/
	long		afv_fprm[sizeof(long)*8];/* AFV係数微調ﾊﾟﾗﾒｰﾀ[]	*/
} DSPRP4;

int		dsp_afv_adjust(unsigned long *, void *, void *,
				void *, void *, struct dsprp4 *);

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum dspcn0 {				/* start/stop			*/
	DSP_STOP,			/* スタート			*/
	DSP_START,			/* ストップ			*/
};
typedef enum dspcn0	DSPCN0;

int		dsp_afv_judge(enum dspcn0);

/*----------------------------------------------------------------------*/

int		dsp_stable_chk(unsigned long, void *, void *,
				void *, void *, void *, void *);

#endif	/* DSPDRV_H */

