/*----------------------------------------------------------------------*
 * File name	: dadbot1.c	/ dadc change prameter boot		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadbot1.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadbot1.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadbot1.c 1.2 1970/01/01
 *Log: dadbot1.c
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
 *Header: p:/lib/drive/ndadc/rcs/dadbot1.c 1.1 1996/07/22 13:36:19 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV-DSPにチェンジ･パラメータのブートを行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: board number						*/
/*	*arg	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command = CHG_BOOT0	*/
	short		code;			/* 秤量&ｾﾙ･ﾀｲﾌﾟ		*/
	short		spn_mas;		/* span mass		*/
	short		afv_sw;			/* AFV auto/manual sw	*/
	unsigned short	f3buf_cnt;		/* 3段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数	*/
	unsigned short	f3stb_cnt;		/*   "  安定判別ｶｳﾝﾄ	*/
	unsigned short	f3avg_cnt;		/*   "  平均化処理判定値*/
	unsigned short	f4buf_cnt;		/* 4段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数	*/
	unsigned short	f4stb_cnt;		/*   "  安定判別ｶｳﾝﾄ	*/
	unsigned short	f4avg_cnt;		/*   "  平均化処理判定値*/
	unsigned short	f5buf_cnt;		/* 5段目ﾊﾞｯﾌｧﾘﾝｸﾞ回数	*/
	unsigned short	f5stb_cnt;		/*   "  安定判別ｶｳﾝﾄ	*/
	unsigned short	f5avg_cnt;		/*   "  平均化処理判定値*/
	short		zr_lmt[9];		/* 零点許容値		*/
};

struct txtr {
	short	cmd;			/* enum dcmd = EOF_CHG_BOOT0	*/
	short	code;			/* return code		*/
};

int
dad_cboot0(					/* stack = 56byte	*/
	DAD_ID			pwb,
	struct dadpk11		*arg
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	int		err;
	int		iyamana;

	ret.cmd = (short)0;
	ret.code = (short)0;
	err = 0;
	iyamana = 0;

	buf.cmd = (short)CHG_BOOT0;
	buf.code = arg->code;
	buf.spn_mas = arg->spn_mas;
	buf.afv_sw = arg->afv_sw;
	buf.f3buf_cnt = arg->f3buf_cnt;
	buf.f3stb_cnt = arg->f3stb_cnt;
	buf.f3avg_cnt = arg->f3avg_cnt;
	buf.f4buf_cnt = arg->f4buf_cnt;
	buf.f4stb_cnt = arg->f4stb_cnt;
	buf.f4avg_cnt = arg->f4avg_cnt;
	buf.f5buf_cnt = arg->f5buf_cnt;
	buf.f5stb_cnt = arg->f5stb_cnt;
	buf.f5avg_cnt = arg->f5avg_cnt;
	memcpy((void *)&buf.zr_lmt, (void *)arg->zr_lmt, sizeof(buf.zr_lmt));
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&err, (void *)&buf,
			(unsigned short)sizeof(buf)) < 0)	return(-1);
	if(err = wai_chg_boot_flg(pwb, CHG_BOOT0_COMPL)) {
		assert(!err);					return(-1);
	}
	iyamana = dad_dpm_read(pwb, (void *)&ret, sizeof(ret), (unsigned short)0);
#ifdef	MAS_BIG_ENDIAN
	/* [DADITSK.C] dad_int_tsk() switch の直前にて、すでに上位下位を入れ替えている。(だからここでは行わない。) */
	/* chg_ul_aft_read(&ret); */	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	if ((iyamana < 0) || (ret.code < 0))			return(-1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
int	ielm1;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->code);
	CHG_UL_SHORT(text->spn_mas);
	CHG_UL_SHORT(text->afv_sw);
	CHG_UL_USHORT(text->f3buf_cnt);
	CHG_UL_USHORT(text->f3stb_cnt);
	CHG_UL_USHORT(text->f3avg_cnt);
	CHG_UL_USHORT(text->f4buf_cnt);
	CHG_UL_USHORT(text->f4stb_cnt);
	CHG_UL_USHORT(text->f4avg_cnt);
	CHG_UL_USHORT(text->f5buf_cnt);
	CHG_UL_USHORT(text->f5stb_cnt);
	CHG_UL_USHORT(text->f5avg_cnt);
	for (ielm1 = 0; ielm1 < 9; ++ielm1) {
		CHG_UL_SHORT(text->zr_lmt[ielm1]);
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
