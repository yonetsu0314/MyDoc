/*----------------------------------------------------------------------*
 * File name	: daddfd.c	/ dadc df data read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/daddfd.c 1.2 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: daddfd.c $
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
 *Header: p:/lib/drive/ndadc/rcs/daddfd.c 1.1 1996/07/22 13:36:33 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	分散ﾌｨｰﾀﾞの計量ﾃﾞｰﾀをﾘｰﾄﾞする。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: ADC board number					*/
/*	ch	: DF号機number						*/
/*		    DF #1	･･･ 0					*/
/*		    DF #4	･･･ 3					*/
/*	*sts	: DFｽﾃｰﾀｽを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*		    DSP_ERR0	(0x01)	･･･ ﾃﾞｰﾀの前段までが無効	*/
/*		    DSP_ERR1	(0x02)	･･･ ﾌｨﾙﾀ計算中のｵｰﾊﾞｰ･ﾌﾛｰ	*/
/*		    DSP_ERR2	(0x04)	･･･ ﾌｨﾙﾀ計算中のｱﾝﾀﾞｰ･ﾌﾛｰ	*/
/*		    DSP_CALERR	(0x80)	･･･ 計算ｴﾗｰ			*/
/*	*dat	: DFﾃﾞｰﾀを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct rp3 *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command = DF_DAT_REQ	*/
};

struct rp3 {					/* Return paramter	*/
	short		dat[DAD_DF_MAX];	/* DFﾃﾞｰﾀ[df_max]	*/
	unsigned short	stat[DAD_DF_MAX];	/* error status[]	*/
};

int
dad_df_read(					/* stack = 20byte	*/
	DAD_ID			pwb,
	int			ch,
	unsigned short		*sts,
	short			*dat
	)
{
	struct txt	buf;			/* text buffer		*/
	struct rp3	rtp;			/* return parameter	*/

	if((ch < 0)||(ch >= DAD_DF_MAX))			return(-1);
	buf.cmd = (short)DF_DAT_REQ;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&rtp, (void *)&buf, 
			(unsigned short)sizeof(buf)) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&rtp);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	*sts = rtp.stat[ch];
	*dat = rtp.dat[ch];
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	return;
}
static void chg_ul_aft_read(struct rp3 *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1, ielm2;
	for (ielm1 = 0; ielm1 < DAD_DF_MAX; ++ielm1) {
		CHG_UL_SHORT(text->dat[ielm1]);
		CHG_UL_USHORT(text->stat[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
