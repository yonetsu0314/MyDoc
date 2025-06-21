/*----------------------------------------------------------------------*
 * File name	: dadfzt.c	/ dadc A/D zero tracking		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadfzt.c 1.4 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: dadfzt.c $
 * リビジョン 1.4  1970/01/01  00:00:00  m-yamana
 * 2003/07/25  09:15
 * ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数を冒頭で初期化するようにした。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  m-yamana
 * 2003/07/23  19:10
 * 送受信テキストのパック問題を修正した。
 * （構造体にダミーメンバを追加した。）
 * 
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
 *Header: p:/lib/drive/ndadc/rcs/dadfzt.c 1.1 1996/07/22 13:36:38 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	指定号機のA/D値のみを操作し零点補正を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	ch_no		: 指定号機ﾅﾝﾊﾞｰ(1～16)				*/
/*	zero_ad		: 零点ADﾃﾞｰﾀへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  1	･･･ 零点ｴﾗｰ				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct rtp *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = ZERO_AD_ADJ*/
	DAD_ARY_PTN	ptn;			/* 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
};

struct rtp {					/* ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀ		*/
	DAD_ARY_PTN	err_ptn;		/* error bit pattern	*/
	short		zero_ad[DAD_WEI_MAX];
};

int
dad_azero(					/* stack = 76byte	*/
	DAD_ID		pwb,
	int		ch_no,
	short		*zero_ad
	)
{
	struct txt	buf;			/* text buffer		*/
	struct rtp	rpkt;

	/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数も一応初期化しておく。*/
	buf.pck = 0;

	if(--ch_no < 0)						return(-1);
	buf.cmd = (short)ZERO_AD_ADJ;
	buf.ptn = 0x1L << ch_no;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&rpkt, (void *)&buf.cmd,
			(unsigned short)(sizeof(buf)-sizeof(buf.pck))) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&rpkt);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	*zero_ad = rpkt.zero_ad[ch_no];
	if(rpkt.err_ptn)					return(1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->ptn);
	return;
}
static void chg_ul_aft_read(struct rtp *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1, ielm2;
	CHG_UL_ULONG(text->err_ptn);
	for (ielm1 = 0; ielm1 < DAD_WEI_MAX; ++ielm1) {
		CHG_UL_SHORT(text->zero_ad[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
