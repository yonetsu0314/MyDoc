/*======================================================================
 * File name    : adcs.h
 *----------------------------------------------------------------------
 * Function 	:
 * 	ＡＤＣボード機種依存定数定義
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/nz_wcu/wcu/rcs/adcs.h 3.1.6510.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: adcs.h $
 * リビジョン 3.1.6510.1  1970/01/01  00:00:00  kawamura
 * 2003/07/30  14:16
 * ＳＨ３に対するワーニング対策
 * 
 * リビジョン 3.1  1998/08/28  16:35:02  kawamura
 * ﾘﾋﾞｼﾞｮﾝ2.1.4300.1と同内容。
 * 
 * リビジョン 2.1.4300.1  1998/08/11  10:01:57  kawamura
 * ＮＭＩ検定仕様。
 * 
 * リビジョン 2.1  1997/11/27  18:57:25  kawamura
 * 検定仕様。スパン係数要求、応答コマンドの追加。
 * 
 * リビジョン 1.2  1996/08/29  08:58:37  chimura
 * AFVとDFのスパン係数のデフォルト値を変更する。
 * DFのスパン係数に関しては、セルタイプ 6L スパン分銅 200g のみの
 * 対応しかしていないので、セルの変更時に注意が必要である。
 * 
 * リビジョン 1.1  1996/08/22  18:32:24  chimura
 * 初期リビジョン
 * 
 *
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/nz_wcu/rcs/adcs.h 1.1 1996/08/12 09:58:06 chimura
 *Log: adcs.h
 * リビジョン 1.1  1996/08/12  09:58:06  chimura
 *----------------------------------------------------------------------
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		ADCS_H
#define		ADCS_H

#define	ADP_MAX		(2)
#define	RANGE_MAX	(2)
#ifdef	P5385
#define	AD_CH_MAX	(24)		/* チャンネル数／1ADCボード	*/
#else
#define	AD_CH_MAX	(20)		/* チャンネル数／1ADCボード	*/
#endif

#define	ADB_WEI_MAX	16		/* 1ボードの最大WH数		*/
#define	ADB_DF_MAX	4		/*	"   最大DF数		*/
#define	WH1_CH		(1)
#define	DF1_CH		(ADB_WEI_MAX+1)

#ifdef	_LP	/*--------------------*//* 32ヘッドモデルの場合		*/
#define	ADBRD_MAX	(2)		/* 2 ADC board			*/
#define	LOG_ADC_SPLIT	(12)		/* 論理的分割番号		*/
#else		/*--------------------*//* 16ヘッドモデルの場合		*/
#define	ADBRD_MAX	(1)		/* 1 ADC board			*/
#define	LOG_ADC_SPLIT	(16)		/* 論理的分割番号		*/
#endif		/*--------------------*/

/*-----        1       2         3        4        5        6        7        8  -----*/
#define	DEF_SPAN_AD			/* span-D/A * AD_CH_MAX		*/\
       {(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,\
	(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,\
	(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0}    \

#define	DEF_SPAN_DA			/* span係数 * AD_CH_MAX		*/\
       {(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,\
	(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,(short)0    ,\
	(short)56928,(short)56928,(short)56928,(short)56928,(short)28000,(short)28000,(short)28000,(short)28000}

#define DEF_DF_SPAN			/* df span 係数*/\
	{{(short)7116,(short)14232,(short)0,(short)0,(short)0,(short)0},\
	 {(short)0,(short)0,(short)20579,(short)0    ,(short)0   ,(short)0},\
	 {(short)0,(short)0,(short)56928,(short)0    ,(short)0   ,(short)0},\
	 {(short)0,(short)0,(short)7116 ,(short)14232,(short)0   ,(short)0},\
	 {(short)0,(short)0,(short)0    ,(short)0    ,(short)9036,(short)18072}}

#define DEF_SPAN_DATE			/* スパン調整日時	*/\
	{0,	0}

#endif
