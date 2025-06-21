/*----------------------------------------------------------------------*
 * File name	: dadbot0.c	/ dadc initial prameter boot		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadbot0.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadbot0.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/01/21 16:15
 * wh_limit　以降のパラメータコピー位置の誤り修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadbot0.c 1.3 1970/01/01
 *Log: dadbot0.c
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
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
 *Header: p:/lib/drive/ndadc/rcs/dadbot0.c 1.1 1996/07/22 13:36:16 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV-DSPに初期パラメータのブートを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: board number						*/
/*	*arg	: ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command = INIT_BOOT	*/
	short		wei_max;		/* 最大ｳｪｲ･ﾍｯﾄﾞ数	*/
	short		df_max;			/* 最大分散ﾌｨｰﾀﾞ数	*/
	short		afv_max;		/* 最大AFVｾﾙ数		*/
	unsigned short	spc[DAD_WEI_MAX+	/* ｽﾊﾟﾝ係数[]		*/
			    DAD_DF_MAX+
			    DAD_AFV_MAX];
	long		afv_typ;		/* AFV TYP係数		*/
	long		afv_fprm[DAD_WEI_MAX];	/* AFV係数微調ﾊﾟﾗﾒｰﾀ[]	*/
	short		xy_mat[2][DAD_WEI_MAX+	/* 重心の座標の2次元[]	*/
				  DAD_AFV_MAX];
	short		z_ad[DAD_WEI_MAX+	/* zero A/D		*/
			     DAD_DF_MAX+
			     DAD_AFV_MAX];
	short		wh_limit;		/* AFV調整WH限界値	*/
	short		afv_limit;		/* AFV調整AFV限界値	*/
	short		afv_onoff_limit;	/* AFV補正ON/OFF限界値	*/
};

struct txtr {
	short		cmd;			/* = EOF_INIT_BOOT0	*/
	short		code;			/* return code		*/
};

int
dad_iboot0(					/* stack = 260byte	*/
	DAD_ID			pwb,
	struct dadpk10		*arg
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	unsigned char	*p;
	int		err;
	unsigned long	i;
	int		iyamana;

	ret.cmd = (short)0;
	ret.code = (short)0;
	err = 0;
	iyamana = 0;

	buf.cmd = (short)INIT_BOOT0;
	memcpy( (void *)&buf.wei_max, (void *)&arg->wei_max,
			sizeof(arg->wei_max)+sizeof(arg->df_max)+sizeof(arg->afv_max));
	memcpy( (void *)&buf.spc[0], (void *)arg->wei_spc,
		 sizeof(buf.spc[0])*DAD_WEI_MAX);
	memcpy( (void *)&buf.spc[DAD_WEI_MAX], (void *)arg->df_spc,
			 sizeof(buf.spc[0])*DAD_DF_MAX);
	buf.afv_typ = arg->afv_typ;
	memcpy( (void *)&buf.spc[DAD_WEI_MAX+DAD_DF_MAX], (void *)arg->afv_spc,
			 sizeof(buf.spc[0])*DAD_AFV_MAX);
	memcpy( (void *)&buf.afv_fprm, (void *)arg->afv_fprm, sizeof(buf.afv_fprm));
	p = (unsigned char *)&buf.xy_mat;
	memcpy( (void *)p, (void *)arg->x_ary, i= sizeof(buf.xy_mat[0][0])*arg->wei_max);
	p += i;
	memcpy( (void *)p, (void *)(&(arg->x_ary)[DAD_WEI_MAX]),
				i= sizeof(buf.xy_mat[0][0])*arg->afv_max);
	p += i;
	memcpy( (void *)p, (void *)arg->y_ary, i= sizeof(buf.xy_mat[0][0])*arg->wei_max);
	p += i;
	memcpy( (void *)p, (void *)(&(arg->y_ary)[DAD_WEI_MAX]),
				i= sizeof(buf.xy_mat[0][0])*arg->afv_max);
	p += i;
	memcpy( (void *)p, (void *)arg->wei_zad, sizeof(buf.z_ad[0])*DAD_WEI_MAX);
	p += sizeof(buf.z_ad[0])*DAD_WEI_MAX;
	memcpy( (void *)p, (void *)arg->df_zad, sizeof(buf.z_ad[0])*DAD_DF_MAX);
	p += sizeof(buf.z_ad[0])*DAD_DF_MAX;
	memcpy( (void *)p, (void *)arg->afv_zad, sizeof(buf.z_ad[0])*DAD_AFV_MAX);
	p += sizeof(buf.z_ad[0])*DAD_AFV_MAX;
	memcpy( (void *)p, (void *)&arg->wh_limit,
			sizeof(arg->wh_limit)+sizeof(arg->afv_limit)+sizeof(arg->afv_onoff_limit));
	p += sizeof(arg->wh_limit)+sizeof(arg->afv_limit)+sizeof(arg->afv_onoff_limit);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&err, (void *)&buf,
			(short)((unsigned long)p-(unsigned long)&buf)) < 0)
								return(-1);
	if(err = wai_init_boot_flg(pwb, INIT_BOOT0_COMPL)) {
		assert(!err);					return(-1);
	}
	iyamana = dad_dpm_read(pwb, (void *)&ret, sizeof(ret), (unsigned short)0);
#ifdef	MAS_BIG_ENDIAN
	/* [DADITSK.C] dad_int_tsk() switch の直前にて、すでに上位下位を入れ替えている。(だからここでは行わない。) */
	/* chg_ul_aft_read(&ret); */	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	if ((iyamana < 0) || (ret.code < 0))			return(-1);
	_disable();
	dad_base[pwb].wei_max = arg->wei_max;
	dad_base[pwb].df_max = arg->df_max;
	dad_base[pwb].afv_max = arg->afv_max;
	_enable();
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
int	ielm1, ielm2;

	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->wei_max);
	CHG_UL_SHORT(text->df_max);
	CHG_UL_SHORT(text->afv_max);
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_USHORT(text->spc[ielm1]);
	}
	CHG_UL_LONG(text->afv_typ);
	for (ielm1 = 0; ielm1 < DAD_WEI_MAX; ++ielm1) {
		CHG_UL_LONG(text->afv_fprm[ielm1]);
	}
	for (ielm2 = 0; ielm2 < 2; ++ielm2) {
		for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_AFV_MAX); ++ielm1) {
			CHG_UL_SHORT(text->xy_mat[ielm2][ielm1]);
		}
	}
	for (ielm1 = 0; ielm1 < (DAD_WEI_MAX+DAD_DF_MAX+DAD_AFV_MAX); ++ielm1) {
		CHG_UL_SHORT(text->z_ad[ielm1]);
	}
	CHG_UL_SHORT(text->wh_limit);
	CHG_UL_SHORT(text->afv_limit);
	CHG_UL_SHORT(text->afv_onoff_limit);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->code);
	return;
}
#endif	/* MAS_BIG_ENDIAN */
