/*----------------------------------------------------------------------*
 * File name	: dadbot2.c 	/ initial parameter boot		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadbot2.c 1.4 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: dadbot2.c $
 * リビジョン 1.4  1970/01/01  00:00:00  m-yamana
 * 2003/07/25  09:15
 * ・デバッグ用に冒頭で変数初期化していたロジックを削除した。
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
 *Header: p:/lib/drive/ndadc/rcs/dadbot2.c 1.1 1996/07/22 13:36:22 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADC-DSPへ初期パラメータのブートを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ﾎﾞｰﾄﾞ･ﾅﾝﾊﾞｰ					*/
/*	*prm		: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<stddef.h>			/*	"		*/
#include	<assert.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = INIT_BOOT1	*/
	unsigned long	wei_bp;			/* ｳｪｲ･ﾍｯﾄﾞ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
	unsigned long	df_bp;			/* 分散ﾌｨｰﾀﾞ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
	unsigned long	afv_bp;			/* AFVｾﾙ･ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
	short		z_da[DAD_WEI_MAX+	/* zero D/A		*/
			     DAD_DF_MAX+
                             DAD_AFV_MAX];
	short		z_ad[DAD_WEI_MAX+	/* zero A/D		*/
			     DAD_DF_MAX+
			     DAD_AFV_MAX];
};

struct txtr {
	short		cmd;		/* = EOF_INIT_BOOT1	*/
	short		code;		/* return code		*/
};

int
dad_iboot1(					/* stack = 116byte	*/
	DAD_ID			pwb,
	struct dadpk0		*arg
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	int		err;
	int		iyamana;

	/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数も一応初期化しておく。*/
	buf.pck = 0;

	buf.cmd = (short)INIT_BOOT1;
	buf.wei_bp = arg->wei_bp;
	buf.afv_bp = arg->afv_bp;
	buf.df_bp = arg->df_bp;
	memcpy((void *)&buf.z_da[0], (void *)arg->wei_zda,
		sizeof(buf.z_da[0])*DAD_WEI_MAX);
	memcpy((void *)&buf.z_da[DAD_WEI_MAX], (void *)arg->df_zda,
		sizeof(buf.z_da[0])*DAD_DF_MAX);
	memcpy((void *)&buf.z_da[DAD_WEI_MAX+DAD_DF_MAX], (void *)arg->afv_zda,
		sizeof(buf.z_da[0])*DAD_AFV_MAX);
	memcpy((void *)&buf.z_ad[0], (void *)arg->wei_zad,
		sizeof(buf.z_ad[0])*DAD_WEI_MAX);
	memcpy((void *)&buf.z_ad[DAD_WEI_MAX], (void *)arg->df_zad,
		sizeof(buf.z_ad[0])*DAD_DF_MAX);
	memcpy((void *)&buf.z_ad[DAD_WEI_MAX+DAD_DF_MAX], (void *)arg->afv_zad,
		sizeof(buf.z_ad[0])*DAD_AFV_MAX);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, NULL, (void *)&buf.cmd,
			(unsigned short)(sizeof(buf)-sizeof(buf.pck))) < 0)	return(-1);
	if(err = wai_init_boot_flg(pwb, INIT_BOOT1_COMPL)) {
		assert(!err);					return(-1);
	}
	iyamana = dad_dpm_read(pwb, (void *)&ret, sizeof(ret), (unsigned short)0);
#ifdef	MAS_BIG_ENDIAN
	/* [DADITSK.C] dad_int_tsk() switch の直前にて、すでに上位下位を入れ替えている。(だからここでは行わない。) */
	/* chg_ul_aft_read(&ret); */	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	if ((iyamana < 0)||(ret.code < 0)) {
		return(-1);
	}
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
int	ielm1, ielm2;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->wei_bp);
	CHG_UL_ULONG(text->df_bp);
	CHG_UL_ULONG(text->afv_bp);
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_SHORT(text->z_da[ielm1]);
	}
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_SHORT(text->z_ad[ielm1]);
	}
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->code);
	return;
}
#endif	/* MAS_BIG_ENDIAN */
