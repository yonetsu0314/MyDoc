/*----------------------------------------------------------------------*/
/* File name	: madif.h	/ Multi ADC I/F				*/
/*----------------------------------------------------------------------*
 *$Header: p:/wcu/main/nz_wcu/wcu/rcs/madif.h 1.1.6308.1.6510.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: madif.h $
 * Revision 1.1.6308.1.6510.1  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:23
 * ＳＨ３に対するワーニング対策
 * 
 * リビジョン 1.1.6308.1  1970/01/01  00:00:00  syuji
 * 2002/12/23 12:00
 * ＳＨＣコンパイラー使用によるバグフィックス。
 * 	ＥＮＵＭ宣言のタグ名の誤り修正。
 * 	関数プロトタイプの不一致エラーを修正。
 * 	マクロの２重定義の修正。
 * 	外部参照変数の定義。
 * 	変数名等の’＄’文字使用の禁止。’＿’に変更。
 * 	旧記述様式を改める。
 * 
 * リビジョン 1.1  1999/04/30  14:47:12  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *header: p:/wcu/main/dz_wcu2/rcs/madif.h 1.1 1996/06/18 18:42:42 chimura Exp $
 *log: madif.h $
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1999, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* Function 	:							*/
/*	APLからADCボードの物理的境界を意識させず、ADC I/Oドライバー	*/
/*	にアクセスする手段を提供する。					*/
/*									*/
/*    ・予めadcs.hより定義されているADBRD_MAXが2以上のモデルは、LOG	*/
/*	_ADC_SPLIT番号を境界として、それぞれアクセスするADCボードを	*/
/*	振り分ける。							*/
/*	APL側からのそれぞれの関数呼び出しで、引数のビット･パターン、	*/
/*	あるいは計量器または分散の号機番号を基に、I/Oドライバーの要	*/
/*	求する引数及びリターン･パラメータを変換する関数群が準備され	*/
/*	ている。							*/
/*----------------------------------------------------------------------*/

#if	!defined(MADIF_H)
#define	MADIF_H

#include	<drive\adcdrv.h>		/* Show drive		*/
#include	"adcs.h"			/* Show current		*/

#define	ADB_HBP		(0xffffffffL >>((sizeof(long)*8)-ADB_WEI_MAX))
#define	ADB_DFBP	(0xffffffffL >>((sizeof(long)*8)-ADB_DF_MAX))

#ifdef	DADCDRV_H
/*----------------------------------------------------------------------*/
#define STS_MAX	(DSP_ERR0|DSP_ERR1|DSP_ERR2)

#ifdef	STORAGE
const unsigned char	ead_sts_tbl[] = {
/*-enum dad_ws	0x0		      0x1		             0x2		   0x3 ----*/
		(unsigned char)0,     (unsigned char)UNSTBL,         (unsigned char)TOP,   (unsigned char)(TOP+UNSTBL),
		(unsigned char)BOTTOM,(unsigned char)(BOTTOM+UNSTBL),(unsigned char)UNSTBL,(unsigned char)UNSTBL
};
#else
extern const unsigned char	ead_sts_tbl[];
#endif
/*----------------------------------------------------------------------*/
#endif

enum filter_no {			/* 重量データのタイプ		*/
	AUTO_SELECT,			/* 自動切り替え			*/
	THIRD,				/* ３段目データ			*/
	FOURTH				/* ４段目データ			*/
};

int	mad_mzero( unsigned long hbp, unsigned short dfbp,
		unsigned long *ehbp, unsigned short *edbp, unsigned short *ests);	/*　バグ　shc */
int	mad_azero( int chno, short *ad, short *da);
int	mad_span( int chno, short *coef);
int	mad_msc_read( unsigned long *exbp, unsigned char *sts, short *dat, enum filter_no no);	/*　バグ　shc */
#define	nzr0190e filter_no
int	mad_scl_read( int dfno, short *dat);	/*　バグ　shc */
int	mad_diag_read( int mod, short *wei, short *afv);	/*　バグ　shc */
int	mad_afv_adjust( int *chno, long *coef);	/*　バグ　shc */
int	mad_afv_config( int mode, unsigned long ptn, unsigned long hbp);

#endif
