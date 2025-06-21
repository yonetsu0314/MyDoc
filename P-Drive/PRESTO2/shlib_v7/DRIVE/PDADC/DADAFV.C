/*----------------------------------------------------------------------*
 * File name	: dadafv.c	/ dadc afv adjustment			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadafv.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadafv.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadafv.c 1.2
 *Log: dadafv.c
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/30  15:50
 * ・基板(P5561/P5554)に対応させた。
 *   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
 *   定義しないことでP5554(村田氏作成)に対応させた。
 *   
 * ・エンディアン(ビッグ/リトル)に対応させた。
 *   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
 *   定義しないことでリトルエンディアンに対応させた。
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<limits.h>			/* ANSI std		*/
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


/*----------------------------------------------------------------------*/
/*	dad_afv_config							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV調整のﾓｰﾄﾞを設定する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: ADC board number					*/
/*	mode	: 調整ﾓｰﾄﾞ						*/
/*		    0    ･･･ ﾏﾆｭｱﾙ					*/
/*		    1	 ･･･ ｵｰﾄ(自動零点時に調整)			*/
/*	ptn	: 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ					*/
/*	void								*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/

struct txt {					/* text format		*/
	short		cmd;			/* command = AFV_ADJ	*/
	short		mode;			/* AUTO/MANU		*/
	DAD_ARY_PTN	ptn;			/* 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
};

int
dad_afv_config(					/* stack = 18byte	*/
	DAD_ID		pwb,
	short		mode,
	unsigned long	ptn
	)
{
	struct txt	buf;			/* 送信ﾃｷｽﾄ･ﾊﾞｯﾌｧ	*/
	int		err =0;

	buf.cmd = (short)AFV_ADJ;
	buf.mode = mode;
	buf.ptn.wei_bp = ptn;
	buf.ptn.df_bp  = 0;
	buf.ptn.afv_bp = 0;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if((dad_dpm_write(pwb, (void *)&err, (void *)&buf, 
		(unsigned short)sizeof(buf)) < 0)||(err < 0))	return(-1);
	return(0);
}

/*----------------------------------------------------------------------*/
/*	dad_afv_adjust							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV調整を行う。マニュアル調整時(*ch_no= 1-32)の場合、即		*/
/*	コマンド発行後、係数が求められる。一方オートの場合は、AFV	*/
/*	調整の実行件がADCに委ねられているため、処理タイミング及び	*/
/*	対象のchannelは結果を受けるまではわからないシステムとなっ	*/
/*	ている。							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*pwb	: ADC board numberへのﾎﾟｲﾝﾀ				*/
/*	*ch_no	: 指定号機ﾅﾝﾊﾞｰへのﾎﾟｲﾝﾀ				*/
/*		    *ch_no   0    ･･･ 係数のﾘｰﾄﾞのみ(AUTO時)		*/
/*		             1-32 ･･･ 手動調整				*/
/*	*coef	: AFV微調値を返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

struct txtr {					/* AFV adj complete	*/
	short		cmd;			/* command = EOF_AFV	*/
	DAD_ARY_PTN	ptn;			/* 指定号機ﾋﾞｯﾄ･ﾊﾟﾀｰﾝ	*/
	long		afv_fprm;		/* AFV係数微調ﾊﾟﾗﾒｰﾀ	*/
};

int
dad_afv_adjust(					/* stack = 18byte	*/
	DAD_ID		*pwb,
	int		*ch_no,
	long		*coef
	)
{
	struct txtr	ret;			/* AFV完了ﾃｷｽﾄへのﾎﾟｲﾝﾀ	*/
	unsigned long	ptn;			/* 実行ﾊﾟﾀｰﾝ		*/
	int		err, i;
	DAD_ID		j;

	if(i = *ch_no) {
		if((--i <= 0)
		||(dad_afv_config(j= *pwb, (short)0, ptn= 0x1L<<i)))	return(-1);
		if(err = wai_afv_flg(j)) {
			assert(!err);				return(-1);
		}
	} else {
		if(err = wai_afv_msg(&j)) {
			assert(!err);				return(-1);
		}
	}
	if(dad_dpm_read(j, (void *)&ret.ptn, sizeof(ret),
			(unsigned short)sizeof(ret.cmd)) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&ret);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	if(*ch_no == 0) {
		for(i= 0; ret.ptn.wei_bp; ret.ptn.wei_bp>>=1, i++) {
			if(ret.ptn.wei_bp & 0x1) {
				*ch_no = ++i;	break;
			}
		}
		*pwb = j;
	}
	*coef = ret.afv_fprm;
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->mode);
	CHG_UL_ULONG(text->ptn.wei_bp);
	CHG_UL_SHORT(text->ptn.df_bp);
	CHG_UL_SHORT(text->ptn.afv_bp);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_ULONG(text->ptn.wei_bp);
	CHG_UL_SHORT(text->ptn.df_bp);
	CHG_UL_SHORT(text->ptn.afv_bp);
	CHG_UL_LONG(text->afv_fprm);
	return;
}
#endif	/* MAS_BIG_ENDIAN */
