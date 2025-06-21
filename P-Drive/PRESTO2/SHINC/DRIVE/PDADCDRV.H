/*----------------------------------------------------------------------*
 * File name	: pdadcdrv.h	/ dadc driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/pdadcdrv.h 1.5 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pdadcdrv.h $
 * リビジョン 1.5  1970/01/01  00:00:00  kawamura
 * 2004/07/05  14:42
 * ５段目で零点補正出来るようにする。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/12/29 16:00
 * ADCボード　４枚まで対応する。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * dadcdrv.hのPresto用
 * 
 *----------------------------------------------------------------------*
 *header: p:/presto/shinc/drive/rcs/ndadcdrv.h 1.2 1970/01/01 00:00:00
 *log: ndadcdrv.h
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/12  10:20
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DADCDRV_H)
#define	DADCDRV_H

#define	DAD_WEI_MAX	24		/* 計量器の最大要素数		*/
#define	DAD_AFV_MAX	4		/* AFVｾﾙの	"		*/
#define	DAD_DF_MAX	8		/* 分散ﾌｨｰﾀﾞの	"		*/
#define	DAD_SECT_MAX	2		/* ｾｸｼｮﾝ数			*/
#define	DAD_FIL_MAX	6		/* 5段のﾌｨﾙﾀ構成 + AFV 3rdﾌｨﾙﾀ	*/
#define	DAD_TAP_MAX	64		/* 最大ﾀｯﾌ数			*/
/*typedef unsigned long	DAD_ARY_PTN;*/	/* 0-15:wei/16-19:afv/20-23:df	*/
/*#define	DAD_ARY_MAX	(sizeof(DAD_ARY_PTN)*8)*/

typedef struct dad_ary_ptn {
	unsigned long	wei_bp;
	unsigned short	df_bp;
	unsigned short	afv_bp;
} DAD_ARY_PTN;

void	dadc0_int_hdr(void);		/* dadc#0 interrupt handler	*/
void	dadc1_int_hdr(void);		/* dadc#1 interrupt handler	*/
void	dadc2_int_hdr(void);		/* dadc#2 interrupt handler	*/
void	dadc3_int_hdr(void);		/* dadc#3 interrupt handler	*/

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum dad_id {
	DADC1,				/* DADC borad-1 ID		*/
	DADC2,				/* 	"     2  "		*/
	DADC3,				/* 	"     3  "		*/
	DADC4,				/* 	"     4  "		*/
	DAD_MAX
};
typedef enum dad_id	DAD_ID;

#if	!defined(ADCDRV_H)		/* if not include adcdrv.h	*/
#define	ADC1	DADC1			/* ADC board 1 ID		*/
#define	ADC2	DADC2			/* ADC board 2 ID		*/
#define	ADC3	DADC3			/* ADC board 3 ID		*/
#define	ADC4	DADC4			/* ADC board 4 ID		*/
#endif

int	dad_sys_init(DAD_ID);
int	dad_mzero(DAD_ID, DAD_ARY_PTN, DAD_ARY_PTN *, short *, short *, short *);
int	dad_az5th(DAD_ID, int, short *);
int	dad_azero(DAD_ID, int, short *);
int	dad_az3rd(DAD_ID, int, short *);
int	dad_memrd(DAD_ID, void *, short *, short);
int	dad_memwr(DAD_ID, char *, char *, short);
int	dad_dsp_memrd(DAD_ID, void *, short *, short);
int	dad_dsp_memwr(DAD_ID, char *, char *, short);
int	dad_reset(DAD_ID);
int	dad_afv_adjust(DAD_ID *, int *, long *);
int	dad_afv_config(DAD_ID, short, unsigned long);
int	dad_diag_read(DAD_ID, short, short *, short *);

/*----------------------------------------------------------------------*/
typedef struct	dadpk10 {		/*	<<< argument 2 >>>	*/
	short		wei_max;	/* 最大ｳｪｲ･ﾍｯﾄﾞ数(ﾏｼﾝ依存)	*/
	short		df_max;		/* 最大分散ﾌｨｰﾀﾞ数		*/
	short		afv_max;	/* 最大AFVｾﾙ数(ﾏｼﾝ依存)		*/
	long		afv_typ;	/* AFV TYP係数			*/
	short		*wei_spc;	/* 計量器ｽﾊﾟﾝ係数[]		*/
	short		*df_spc;	/* DFｽﾊﾟﾝ係数[]			*/
	short		*afv_spc;	/* AFVｽﾊﾟﾝ係数[]		*/
	long		*afv_fprm;	/* AFV係数微調ﾊﾟﾗﾒｰﾀ[]		*/
	short		*x_ary;		/* 計量器+AFVの重心のX座標[]	*/
	short		*y_ary;		/* 計量器+AFVの重心のY座標[]	*/
	short		*wei_zad;	/* 計量器零点A/D[DAD_WEI_MAX]	*/
	short		*df_zad;	/* DF零点A/Dﾃﾞｰﾀ[DAD_DF_MAX]	*/
	short		*afv_zad;	/* AFV零点A/Dﾃﾞｰﾀ[DAD_AFV_MAX]	*/
	short		wh_limit;	/* AFV調整WH限界値		*/
	short		afv_limit;	/* AFV調整AFV限界値		*/
	short		afv_onoff_limit;/* AFV補正ON/OFF限界値		*/
} DADPK10;

int	dad_iboot0(DAD_ID, struct dadpk10 *);

/*----------------------------------------------------------------------*/
typedef struct dadpk0 {			/*	<<< argument 2 >>>	*/
	DAD_ARY_PTN	ptn;		/* ｳｪｲ･ﾍｯﾄﾞ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ		*/
					/* 分散ﾌｨｰﾀﾞ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ		*/
					/* AFVｾﾙ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ		*/
	short		*wei_zda;	/* 計量器零点D/A[DAD_WEI_MAX]	*/
	short		*df_zda;	/* DF零点D/Aﾃﾞｰﾀ[DAD_DF_MAX]	*/
	short		*afv_zda;	/* AFV零点D/Aﾃﾞｰﾀ[DAD_AFV_MAX]	*/
	short		*wei_zad;	/* 計量器零点A/D[DAD_WEI_MAX]	*/
	short		*df_zad;	/* DF零点A/Dﾃﾞｰﾀ[DAD_DF_MAX]	*/
	short		*afv_zad;	/* AFV零点A/Dﾃﾞｰﾀ[DAD_AFV_MAX]	*/
} DADPK0;

int	dad_iboot1(DAD_ID, struct dadpk0 *);

/*----------------------------------------------------------------------*/
enum dadspm {
	SPN_M20	= 1,			/* span:20g			*/
	SPN_M40,			/*	40g			*/
	SPN_M200,			/*	200g			*/
	SPN_M400,			/*	400g			*/
	SPN_M500,			/*	500g			*/
	SPN_M1000,			/*	1000g			*/
};
typedef enum dadspm	DADSPM;

enum dadsw0 {
	AFV_SW_OFF,			/* AFV OFF			*/
	AFV_SW_AUTO,			/* AFV AUTO			*/
	AFV_SW_ON,			/* AFV ON			*/
};
typedef enum dadsw0	DADSW0;

typedef struct	dadpk11 {		/*	<<< argument 2 >>>	*/
	short		code;		/* 秤量&ｾﾙ･ﾀｲﾌﾟ			*/
	short		spn_mas;	/* span mass			*/
	short		afv_sw;		/* AFV switch			*/
	unsigned short	f3buf_cnt;	/* 3段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数		*/
	unsigned short	f3stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f3avg_cnt;	/*   "  平均化処理判定値	*/
	unsigned short	f4buf_cnt;	/* 4段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数		*/
	unsigned short	f4stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f4avg_cnt;	/*   "  平均化処理判定値	*/
	unsigned short	f5buf_cnt;	/* 5段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数		*/
	unsigned short	f5stb_cnt;	/*   "  安定判別ｶｳﾝﾄ		*/
	unsigned short	f5avg_cnt;	/*   "  平均化処理判定値	*/
	short		*zr_lmt;	/* 零点許容値[9]		*/
} DADPK11;

int	dad_cboot0(DAD_ID, struct dadpk11 *);

/*----------------------------------------------------------------------*/
enum dadsc {
					/*-----  max weigh limit  ------*/
	W40G		= 1,		/* max-40g			*/
	W80G,				/* max-80g			*/
	W160G,				/* max-160g			*/
	W400G,				/* max-400g			*/
	W800G,				/* max-800g			*/
	W1600G,				/* max-1600g			*/
	W1000G,				/* max-1000g			*/
	W2000G,				/* max-2000g			*/
	W4000G,				/* max-4000g			*/
	DAD_WMAX,
					/*-----  cell type  ------------*/
	C1_2L		= (0x1 << 8),	/* NLC-1.2L			*/
	C4L		= (0x2 << 8),	/* NLC-4L			*/
	C6L		= (0x3 << 8),	/* NLC-6L			*/
	C12L		= (0x4 << 8),	/* NLC-12L			*/
	C24L		= (0x5 << 8),	/* NLC-24L			*/
	DAD_CMAX	= (0x6 << 8),
};

enum dadfc {
	FIL_CODE0,			/* ﾌｨﾙﾀ係数ｺｰﾄﾞ0		*/
	FIL_CODE1,			/* ﾌｨﾙﾀ係数ｺｰﾄﾞ1		*/
	FIL_CODE2,			/* ﾌｨﾙﾀ係数ｺｰﾄﾞ2		*/
	FIL_CODE3,			/* ﾌｨﾙﾀ係数ｺｰﾄﾞ3		*/
	FIL_CODE4,			/* ﾌｨﾙﾀ係数ｺｰﾄﾞ4		*/
	FIL_CODE_MAX,
};

struct dadfip {				/* ﾌｨﾙﾀ係数関係のﾊﾟﾗﾒｰﾀ構造	*/
	unsigned short	thin;		/* 間引き数			*/
	unsigned short	dig;		/* 有効桁増加数			*/
	unsigned short	tap;		/* ﾀｯﾌﾟ数			*/
	unsigned short	code;		/* 係数ｺｰﾄﾞ			*/
};

typedef struct dadpk1 {			/*	<<< argument 2 >>>	*/
	short		cell_t;		/* cellﾀｲﾌﾟ&秤量		*/
	short		z_targ;		/* zero target			*/
	short		*z_prm;		/* 零点許容値[6]		*/
	struct dadfip  *fipp[DAD_FIL_MAX];
} DADPK1;

typedef struct dadrp0 {			/*   <<< Return parameter >>>	*/
	unsigned short	f3tmc;		/* 3RD stage filter time const	*/
	unsigned short	f4tmc;		/* 4TH		"		*/
	unsigned short	f5tmc;		/* 5TH		"		*/
} DADRP0;

int	dad_cboot1(DAD_ID, struct dadrp0 *, struct dadpk1 *);

/*----------------------------------------------------------------------*/
enum dades1 {				/* ｽﾊﾟﾝ係数ｴﾗｰ			*/
	SER_LOLMT	= -32768,	/* 下限未満の場合		*/
	SER_CALC	= -1,		/* 計算ｴﾗｰ			*/
	SER_NOLOAD	= 0,		/* ﾉｰﾛｰﾄﾞ･ｴﾗｰ			*/
	SER_UPLMT	= 32767,	/* 上限を超えた場合		*/
};
typedef enum dades1	DADES1;

int	dad_span(DAD_ID, int, short *);

/*----------------------------------------------------------------------*/
typedef struct dadsck {			/*   <<< Return parametr >>>	*/
	short		ref1;		/* ADC1の基準ﾃﾞｰﾀ		*/
	short		ref0;		/* ADC0   "			*/
} DADSCK;

DADSCK *	dad_self_chk(DAD_ID);

/*----------------------------------------------------------------------*/
#define	DSP_ERR0	(0x01)		/* ﾃﾞｰﾀの前段までが無効		*/
#define	DSP_ERR1	(0x02)		/* ﾌｨﾙﾀ計算中のｵｰﾊﾞｰ･ﾌﾛｰ	*/
#define	DSP_ERR2	(0x04)		/* ﾌｨﾙﾀ計算中のｱﾝﾀﾞｰ･ﾌﾛｰ	*/
#define	DSP_UNSTBL	(0x20)		/* 秤不安定			*/
#define	DSP_AFV_CMPERR	(0x40)		/* AFV補正ｴﾗｰ			*/
#define	DSP_CALERR	(0x80)		/* 計算ｴﾗｰ			*/

typedef struct	dades10 {		/*   <<< Return parametr >>>	*/
	DAD_ARY_PTN	ptn;		/* error head bit pattern	*/
	short		stat;		/* error status			*/
} DADES10;

int	dad_wei_read(DAD_ID, DADES10 *, unsigned short *, short *,
					unsigned short *, short *,
					unsigned short *, short *);

int	dad_df_read(DAD_ID, int, unsigned short *, short *);

/*----------------------------------------------------------------------*/
enum dadstg {				/*	<<< argument 2 >>>	*/
	DAD_ORG		= 0x01,		/* ADC原波形ﾃﾞｰﾀ		*/
	DAD_1ST		= 0x02,		/* ADC1段目出力ﾃﾞｰﾀ		*/
	DAD_2ND		= 0x04,		/* ADC2段目出力ﾃﾞｰﾀ		*/
	DAD_3RD		= 0x08,		/* ADC3段目出力+AC成分ﾃﾞｰﾀ	*/
	DAD_4TH		= 0x10,		/* ADC4段目出力ﾃﾞｰﾀ		*/
	DAD_5TH		= 0x20,		/* ADC5段目出力ﾃﾞｰﾀ		*/
/*-------------------------------------*/
	DAD_AFV0_3RD	= 0x01,		/* AFV補正前3段目ﾃﾞｰﾀ		*/
	DAD_AFV1_3RD	= 0x02,		/* AFV補正後3段目ﾃﾞｰﾀ		*/
};
typedef enum dadstg	DADSTG;

int	dad_sta_dsp_mon(DAD_ID, short, DAD_ARY_PTN, unsigned long);
int	dad_stp_dsp_mon(DAD_ID);
int	dad_sta_mon(DAD_ID, short, DAD_ARY_PTN, unsigned long);
int	dad_stp_mon(DAD_ID);

/*------------------------------------------------------*/
/* Function:						*/
/*	割り込みﾊﾝﾄﾞﾗ					*/
/*------------------------------------------------------*/
#define	adc0_int_hdr	dadc0_int_hdr
#define	adc1_int_hdr	dadc1_int_hdr
#define	adc2_int_hdr	dadc2_int_hdr
#define	adc3_int_hdr	dadc3_int_hdr

/*------------------------------------------------------*/
/* Function:						*/
/*	ｼｽﾃﾑ･ｲﾆｼｬﾗｲｽﾞ					*/
/*------------------------------------------------------*/
#define	ad_sys_init(bord_no)	dad_sys_init(bord_no);

/*------------------------------------------------------*/
/* Function:						*/
/*	自動零点調整					*/
/*------------------------------------------------------*/
#define	ad_azero(bord_no, ch_no, zero_ad, zero_da)\
	dad_azero(bord_no, ch_no, zero_ad)

/*------------------------------------------------------*/
/* Function:						*/
/*	ｽﾊﾟﾝ調整					*/
/*------------------------------------------------------*/
#define	ad_span(bord_no, ch_no, span_coef)\
	dad_span(bord_no, ch_no, span_coef)

#endif	/* DADCDRV_H	*/
