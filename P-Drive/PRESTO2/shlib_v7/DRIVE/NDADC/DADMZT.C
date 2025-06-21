/*----------------------------------------------------------------------*
 * File name	: dadmzt.c 	/ manual zero tracking			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadmzt.c 1.4 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: dadmzt.c $
 * リビジョン 1.4  1970/01/01  00:00:00  m-yamana
 * 2003/07/25  09:15
 * 送信側ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数の初期化位置を変更した。
 * 受信側ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数を初期化していなかったので初期化するようにした。
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
 *Header: p:/lib/drive/ndadc/rcs/dadmzt.c 1.1 1996/07/22 13:37:09 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	指定計量ﾍｯﾄﾞの手動零点補正を行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	ptn		: 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ				*/
/*	*err_ptn	: ｴﾗｰ発生号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ				*/
/*	*err_sts	: ｴﾗｰﾃﾞｰﾀ,ｽﾃｰﾀｽのｽﾄｱｰ･ﾎﾟｲﾝﾀｰ			*/
/*	*zero_ad	: 零点ADﾃﾞｰﾀへのﾎﾟｲﾝﾀ				*/
/*	*zero_da	: 零点DAﾃﾞｰﾀへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*			  1	･･･ 零点ｴﾗｰ				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = MANU_ZERO	*/
	DAD_ARY_PTN	ptn;			/* 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
};

struct txtr {					/* ｺﾝﾌﾟﾘｰﾄ･ﾃｷｽﾄ		*/
	short		pck;			/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ	*/
	short		cmd;			/* command = EOF_ZERO	*/
	DAD_ARY_PTN	err_ptn;		/* error bit pattern	*/
	short		zero_da[DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX];
	short		zero_ad[DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX];
};

int
dad_mzero(					/* stack = 148byte	*/
	DAD_ID		pwb,
	DAD_ARY_PTN	ptn,
	DAD_ARY_PTN	*err_ptn,
	short		*err_sts,
	short		*zero_ad,
	short		*zero_da
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	int		err, i;

	/* ﾊﾟｯｸ問題対処用ﾀﾞﾐｰ変数も一応初期化しておく。*/
	buf.pck = 0;
	ret.pck = 0;

	memset((void *)&dad_afv_err[pwb], 0, sizeof(dad_afv_err[pwb]));
	*err_ptn = 0;
	if((buf.ptn = ptn) == 0)				return(0);
	buf.cmd = (short)MANU_ZERO;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, NULL, (void *)&buf.cmd,
			(unsigned short)(sizeof(buf)-sizeof(buf.pck))) < 0)	return(-1);
	if(err = wai_zero_flg(pwb)) {
		assert(!err);					return(-1);
	}
	if(dad_dpm_read(pwb, (void *)&ret.cmd, (unsigned long)(sizeof(ret)-sizeof(ret.pck)),
			 (unsigned short)0) < 0)		return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&ret);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	*err_ptn = ret.err_ptn;
	for(i =0; ptn ; ptn >>=0x1L, ret.err_ptn >>=0x1L, i++) {
		if(ptn & 0x1) {
			if(ret.err_ptn & 0x1) {
				err_sts[i] = ret.zero_ad[i];
			}
			else {
				zero_ad[i] = ret.zero_ad[i];
				zero_da[i] = ret.zero_da[i];
			}
		}
	}
	if(*err_ptn)						return(1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->ptn);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1, ielm2;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->err_ptn);
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_SHORT(text->zero_da[ielm1]);
		CHG_UL_SHORT(text->zero_ad[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
