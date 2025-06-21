/*======================================================================
 * File name    : hptn.h
 *----------------------------------------------------------------------
 * Function 	:
 *	ヘッド数に依存する型等のマクロ定義。
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/rz_wcu2/rcs/hptn.h 1.1.6308.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: hptn.h $
 * Revision 1.1.6308.2  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:46
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
 * リビジョン 1.1  1996/06/18  18:42:13  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/rz_wcu/rcs/hptn.h 2.2 1994/05/06 20:28:40 ryoji
 *Log: hptn.h
 * リビジョン 2.2  1994/05/06  20:28:40  ryoji
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		HPTN_H
#define		HPTN_H

#ifndef	DUC_MAX
enum lino {
	DUC_LINE1_,
	DUC_LINE2_	= DUC_LINE1_,
	DUC_LINE_MAX
};
#endif

#ifndef RX116_H
#include	<rxif\rx116.h>
#endif


#ifdef	_LP	/*--------------------*//* 32ヘッドモデルの場合		*/

typedef unsigned long	HBP;		/* longのヘッドビットパターン	*/
#define		 LSB	(0x01UL)	/* longの最下位ビット		*/
#define		 CLR_BP	(0UL)		/* longのクリアーパターン	*/
typedef union {
	HBP		sp[2];		/* WH,BHビットパターンの配列	*/
	unsigned char	cp[8];		/* バイトでアクセスする場合	*/
} UBP;
#define	HEAD_MAX	(32)		/* 最大ヘッド数			*/

#else 		/*--------------------*//* 16ヘッドモデルの場合		*/

typedef unsigned short	HBP;		/* shortのヘッドビットパターン	*/
#define		 LSB	(HBP)(0x01)	/* shortの最下位ビット		*/
#define		 CLR_BP	(HBP)(0)	/* shortのクリアーパターン	*/
typedef union {
	HBP		sp[2];		/* WH,BHビットパターンの配列	*/
	unsigned char	cp[4];		/* バイトでアクセスする場合	*/
} UBP;
#define	HEAD_MAX	(16)		/* 最大ヘッド数			*/

#endif		/*--------------------*/

#define	_WH	(0)			/* unionビットパターン配列番号	*/
#define	_BH	(1)			/* unionビットパターン配列番号	*/
#define	HBP_BIT	(sizeof(HBP)*8)		/* HBPのビット数		*/
#define	MSB	(HBP)(LSB << HBP_BIT-1)	/* HBPの最上位ビット		*/
typedef	UBP	CBP;			/* 組合せ計算のビットパターン	*/

enum chno {
	C1_,	
	C2_,	
	C_MAX	
};

enum seno {
	S1_,	
	S2_,	
	S3_,	
	S4_,	
	S5_,	
	S6_,	
	S7_,	
	S8_,	
	S_MAX	
};

enum dfno {
	DF1_,	
	DF2_,	
	DF3_,	
	DF4_,	
	DF5_,	
	DF6_,	
	DF7_,	
	DF8_,	
	DF_MAX	
};

enum cfno {
	CF1_,	
	CF2_,	
	CF3_,	
	CF4_,	
	CF5_,	
	CF6_,	
	CF7_,	
	CF8_,	
	CF_MAX	
};

#endif
