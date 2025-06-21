/*----------------------------------------------------------------------*
 * File name	: dadbot3.c	/ change parameter boot			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadbot3.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadbot3.c $
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
 *Header: p:/lib/drive/ndadc/rcs/dadbot3.c 1.1 1996/07/22 13:36:24 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADC-DSPへ設定変更が可能なパラメータのブートを行う。なおフィ	*/
/*	ルタ係数が変更された場合、一定時間生成されるデータは不確定	*/
/*	となるため、それに該当する係数が変更された場合、データ不確	*/
/*	定を示す時間(/10 ms)を返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ﾎﾞｰﾄﾞ･ﾅﾝﾊﾞｰ					*/
/*	*ret		: ﾘﾀｰﾝ･ﾊﾟﾗﾒｰﾀのﾌｨﾙﾀ時間構造体へのﾎﾟｲﾝﾀ		*/
/*	*arg		: 引き数のﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄへのﾎﾟｲﾝﾀ			*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 成功				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<assert.h>			/*	"		*/
#include	<stddef.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\typedefs.h>		/* Show custom		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

#define		TIME_BASE	10		/* 10ms timer pitch	*/
typedef unsigned short	_F_T[DAD_FIL_MAX];
static _F_T		fil_time[DAD_MAX];

struct fprm {					/* ﾌｨﾙﾀ係数のﾊﾟﾗﾒｰﾀ構造	*/
	unsigned short	thin;			/* 間引き数		*/
	unsigned short	time;			/* ﾌｨﾙﾀ時間		*/
	unsigned short	dig;			/* 有効桁増加数		*/
	unsigned short	tap;			/* ﾀｯﾌﾟ数		*/
	unsigned short	code;			/* 係数ｺｰﾄﾞ		*/
};

struct txt {
	short		cmd;			/* command = CHG_BOOT1	*/
	unsigned short	cell_t;			/* cell/秤量		*/
	short		z_targ;			/* zero target		*/
	short	 	z_prm[2];		/* 零点許容値[6]	*/
	struct fprm	fip[DAD_FIL_MAX];	/* 4段のﾌｨﾙﾀ･ﾊﾟﾗﾒｰﾀ +	*/
						/* AFVｾﾙのﾌｨﾙﾀﾊﾟﾗﾒｰﾀ	*/
	unsigned short	ttlfiltm;		/* ﾌｨﾙﾀ時間のﾄｰﾀﾙ	*/
};

struct txtr {
	short		cmd;			/* = EOF_CHG_BOOT1	*/
	short		code;			/* return code		*/
};

int
dad_cboot1(					/* Stack = 582byte	*/
	DAD_ID			pwb,
	struct dadrp0		*rtp,
	struct dadpk1		*arg
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	struct dadfip	*fip;			/* ﾌｨﾙﾀ･ﾃﾞｰﾀへのﾎﾟｲﾝﾀｰ	*/
	_F_T		tm;			/* ﾌｨﾙﾀ時間		*/
	unsigned short	ttm = (unsigned short)0;/* ﾌｨﾙﾀ時間のﾄｰﾀﾙ	*/
	char		*p;
	int		err, i, j;
	int		iyamana;

	ret.cmd = (short)0;
	ret.code = (short)0;
	err = 0;
	iyamana = 0;

/*	係数がAFVｾﾙの場合、ﾌｨﾙﾀのﾄｰﾀﾙ時間に加算しない	*/
	for(j = 0; j < DAD_FIL_MAX; ++j) {
		tm[j] = (unsigned short)((((arg->fipp[j]->tap-1)*arg->fipp[j]->thin)/2)+1);
		if(j < DAD_FIL_MAX - 1){
			ttm += tm[j];
		}
	}
	buf.cmd = (short)CHG_BOOT1;
	buf.cell_t = arg->cell_t;
	buf.z_targ = arg->z_targ;
	memcpy((void *)&buf.z_prm, (void *)arg->z_prm, sizeof(buf.z_prm));
	p = (char *)&buf.fip;
	for(j = 0; j < DAD_FIL_MAX ; ++j) {
		fip = arg->fipp[j];
		*(WORD_PTR)p = fip->thin;
		p += sizeof(fip->thin);
		*(WORD_PTR)p = (unsigned short)((tm[j]+ TIME_BASE-1)/TIME_BASE);
		p += sizeof(_F_T) /DAD_FIL_MAX;
		*(WORD_PTR)p = fip->dig;
		p += sizeof(fip->dig);
		*(WORD_PTR)p = (unsigned short)(i = fip->tap);
		p += sizeof(fip->tap);
		*(WORD_PTR)p = fip->code;
		p += sizeof(fip->code);
/*		_fmemcpy(p, fip->coef, i *= sizeof(fip->coef[0]));*/
/*		p += i;*/
	}
	*(WORD_PTR)p = (unsigned short)((ttm+ TIME_BASE-1)/TIME_BASE);
	p += sizeof(ttm);

#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, NULL, (void *)&buf,
		 (unsigned short)sizeof(buf))< 0)
								return(-1);
	if(err = wai_chg_boot_flg(pwb, CHG_BOOT1_COMPL)) {
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

	memcpy( (void *)&fil_time[pwb], (void *)&tm, sizeof(tm));
	_disable();
	rtp->f4tmc = (unsigned short)((ttm +TIME_BASE-1)/TIME_BASE);
	rtp->f3tmc = (unsigned short)((ttm -tm[DAD_FIL_MAX-2] +TIME_BASE-1)/TIME_BASE);
	_enable();
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
int	ielm1, ielm2;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_USHORT(text->cell_t);
	CHG_UL_SHORT(text->z_targ);
	for (ielm1 = 0; ielm1 < 2; ++ielm1) {
		CHG_UL_SHORT(text->z_prm[ielm1]);
	}
	for (ielm1 = 0; ielm1 < DAD_FIL_MAX; ++ielm1) {
		CHG_UL_USHORT(text->fip[ielm1].thin);
		CHG_UL_USHORT(text->fip[ielm1].time);
		CHG_UL_USHORT(text->fip[ielm1].dig);
		CHG_UL_USHORT(text->fip[ielm1].tap);
		CHG_UL_USHORT(text->fip[ielm1].code);
	}
	CHG_UL_USHORT(text->ttlfiltm);
	return;
}
static void chg_ul_aft_read(struct txtr *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->code);
	return;
}
#endif	/* MAS_BIG_ENDIAN */
