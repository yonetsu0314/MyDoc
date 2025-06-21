/*----------------------------------------------------------------------*
 * File name	: daddiag.c	/ diagnostic data read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/daddiag.c 1.2 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: daddiag.c $
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
 *Header: p:/lib/drive/ndadc/rcs/daddiag.c 1.1 1996/07/22 13:36:35 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	診断用ﾃﾞｰﾀをﾘｰﾄﾞする。なおｽﾄｱｰ先がNULLﾎﾟｲﾝﾀの場合は結果を	*/
/*	出力しない。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: ADC board number					*/
/*	mod	: 診断モード						*/
/*		    1	･･･ 振動解析mode				*/
/*	*wei	: 計量器の診断用ﾃﾞｰﾀを返すｴﾘｱへのﾎﾟｲﾝﾀ			*/
/*	*afv	: AFVｾﾙの診断用ﾃﾞｰﾀを返すｴﾘｱへのﾎﾟｲﾝﾀ			*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct rp2 *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command= DIAG_DAT_REQ*/
	short		mod;			/* diagnostic mode	*/
};

struct rp2 {					/* Return paramter	*/
	short		wei[DAD_WEI_MAX];	/* 計量器診断ﾃﾞｰﾀ[]	*/
	short		df[DAD_DF_MAX];		/* 無効ﾃﾞｰﾀ(0)		*/
	short		afv[DAD_AFV_MAX];	/* AFV診断ﾃﾞｰﾀ[]	*/
};

int
dad_diag_read(					/* stack = 134byte	*/
	DAD_ID		pwb,
	short		mod,
	short		*wei,
	short		*afv
	)
{
	struct txt	buf;			/* text buffer		*/
	struct rp2	rtp;			/* return parameter	*/

	buf.cmd = (short)DIAG_DAT_REQ;
	buf.mod = mod;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&rtp, (void *)&buf,
			(unsigned short)sizeof(buf)) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&rtp);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(wei != NULL)  memcpy((void *)wei, (void *)&rtp.wei, sizeof(rtp.wei));
	if(afv != NULL)  memcpy((void *)afv, (void *)&rtp.afv, sizeof(rtp.afv));
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->mod);
	return;
}
static void chg_ul_aft_read(struct rp2 *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1, ielm2;
	for (ielm1 = 0; ielm1 < DAD_WEI_MAX; ++ielm1) {
		CHG_UL_SHORT(text->wei[ielm1]);
	}
	for (ielm1 = 0; ielm1 < DAD_DF_MAX; ++ielm1) {
		CHG_UL_SHORT(text->df[ielm1]);
	}
	for (ielm1 = 0; ielm1 < DAD_AFV_MAX; ++ielm1) {
		CHG_UL_SHORT(text->afv[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
