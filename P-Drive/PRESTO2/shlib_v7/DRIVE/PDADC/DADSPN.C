/*----------------------------------------------------------------------*
 * File name	: dadspn.c	/ dadc span adjustment			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadspn.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadspn.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/02/03 19:00
 * スパン調整コマンドのDFとAFVのビットパターンを０に初期化する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadspn.c 1.4 1970/01/01
 *Log: dadspn.c
 * リビジョン 1.4  1970/01/01  00:00:00  m-yamana
 * 2003/07/25  09:15
 * ・前リビジョンで、受信側テキストのパック問題が修正しきれていなかったので修正した。
 *   （構造体にダミーメンバを追加した。）
 * ・ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数を冒頭で初期化するようにした。
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
 *Header: p:/lib/drive/ndadc/rcs/dadspn.c 1.1 1996/07/22 13:37:21 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	スパン調整を行う。コマンド発行後、スパン完了テキストをシス	*/
/*	テム･コールで待つため、呼びだしたタスクはディスパッチする。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	ch_no		: 指定号機ﾅﾝﾊﾞｰ					*/
/*	*sp_coef	: ｽﾊﾟﾝ係数へのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  1	･･･ ｽﾊﾟﾝ･ｴﾗｰ				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = SPAN_ADJ	*/
	DAD_ARY_PTN	ptn;			/* 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
};

struct txtr {					/* span complete	*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = EOF_SPAN	*/
	DAD_ARY_PTN	err_ptn;		/* error bit pattern	*/
	short		sp_coef[DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX];
};

int
dad_span(					/* stack = 78byte	*/
	DAD_ID		pwb,
	int		ch_no,
	short		*sp_coef
	)
{
	struct txt	buf;			/* 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧ	*/
	struct txtr	ret;			/* return text		*/
	int		err;

	/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数も一応初期化しておく。*/
	buf.pck = 0;
	ret.pck = 0;

	if(--ch_no < 0)						return(-1);
	buf.cmd = (short)SPAN_ADJ;
	buf.ptn.wei_bp = 0x1L << ch_no;
	buf.ptn.df_bp  = 0;
	buf.ptn.afv_bp = 0;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&err, (void *)&buf.cmd,
			(unsigned short)(sizeof(buf)-sizeof(buf.pck))) < 0)	return(-1);
	if(err = wai_span_flg(pwb)) {
		assert(!err);					return(-1);
	}
	if(dad_dpm_read(pwb, (void *)&ret.err_ptn, (sizeof(ret)-sizeof(ret.pck)),
			(unsigned short)sizeof(ret.cmd)) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&ret);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	*sp_coef = ret.sp_coef[ch_no];
	if(ret.err_ptn.wei_bp & buf.ptn.wei_bp)			return(1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->ptn.wei_bp);
	CHG_UL_SHORT(text->ptn.df_bp);
	CHG_UL_SHORT(text->ptn.afv_bp);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->err_ptn.wei_bp);
	CHG_UL_SHORT(text->err_ptn.df_bp);
	CHG_UL_SHORT(text->err_ptn.afv_bp);
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_SHORT(text->sp_coef[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
