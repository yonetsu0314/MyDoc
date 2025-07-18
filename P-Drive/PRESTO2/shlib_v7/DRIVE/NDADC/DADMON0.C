/*----------------------------------------------------------------------*
 * File name	: dadmon0.c	/ afv-dsp monitor start			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadmon0.c 1.2 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: dadmon0.c $
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/30  15:50
 * ・基板(P5561/P5554)に対応させた。
 *   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
 *   定義しないことでP5554(村田氏作成)に対応させた。
 *   
 * ・エンディアン(ビッグ/リトル)に対応させた。
 *   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
 *   定義しないことでリトルエンディアンに対応させた。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dadmon0.c 1.1 1996/07/22 13:36:43 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV-DSPのモニターを開始する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	stg		: 指定ｽﾃｰｼﾞ					*/
/*	ptn		: 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ				*/
/*	rept		: ﾘﾋﾟｰﾄ回数					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command = DSP_STA_MON*/
	short		stg;			/* stage		*/
	DAD_ARY_PTN	ptn;			/* head bit pattern	*/
	unsigned long	rept;			/* repeat count		*/
};

int
dad_sta_dsp_mon(				/* stack = 12byte	*/
	DAD_ID		pwb,
	short		stg,
	DAD_ARY_PTN	ptn,
	unsigned long	rept
	)
{
	struct txt	buf;			/* text buffer		*/
	int		err;

	buf.cmd = (short)DSP_STA_MON;
	buf.stg = stg;
	buf.ptn = ptn;
	buf.rept = rept;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&err, (void *)&buf,
			(unsigned short) sizeof(buf)) < 0)	return(-1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->stg);
	CHG_UL_ULONG(text->ptn);
	CHG_UL_ULONG(text->rept);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	return;
}
#endif	/* MAS_BIG_ENDIAN */
