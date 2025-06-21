/*----------------------------------------------------------------------*
 * File name	: dadwhd.c	/ dadc weight head data read		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadwhd.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadwhd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadwhd.c 1.2 1970/01/01
 *Log: dadwhd.c
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
 *Header: p:/lib/drive/ndadc/rcs/dadwhd.c 1.1 1996/07/22 13:37:24 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	全号機の計量ﾃﾞｰﾀをﾘｰﾄﾞする。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: ADC board number					*/
/*	*err	: AFVｴﾗｰ情報を返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*		    typedef struct  dades10 {				*/
/*			DAD_ARY_PTN	ptn;	error head bit pattern	*/
/*			int		stat;	error status		*/
/*		    } DADES10;						*/
/*									*/
/*	*sts3	: 3段目ｽﾃｰﾀｽを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*		    DSP_ERR0	(0x01)	･･･ ﾃﾞｰﾀの前段までが無効	*/
/*		    DSP_ERR1	(0x02)	･･･ ﾌｨﾙﾀ計算中のｵｰﾊﾞｰ･ﾌﾛｰ	*/
/*		    DSP_ERR2	(0x04)	･･･ ﾌｨﾙﾀ計算中のｱﾝﾀﾞｰ･ﾌﾛｰ	*/
/*		    DSP_UNSTBL	(0x20)	･･･ 秤不安定			*/
/*		    DSP_AFV_CMPERR	(0x40)･･･ AFV補正ｴﾗｰ		*/
/*		    DSP_CALERR	(0x80)	･･･ 計算ｴﾗｰ			*/
/*	*dat3	: 3段目ﾃﾞｰﾀを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*	*sts4	: 4段目ｽﾃｰﾀｽを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*		    DSP_AFV_CMPERRを除き、sts3と同じ			*/
/*	*dat4	: 4段目ﾃﾞｰﾀを返すｴﾘｱへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 正常終了					*/
/*		  1	･･･ AFVｼｽﾃﾑｴﾗｰ					*/
/*		  -1	･･･ ｴﾗｰ						*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct rp2 *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {					/* text format		*/
	short		cmd;			/* command = WEI_DAT_REQ*/
};

struct rp2 {					/* Return paramter	*/
	short		wd3rd[DAD_WEI_MAX];	/* 3段目ﾃﾞｰﾀ[]		*/
	unsigned short	sts3rd[DAD_WEI_MAX];	/*    " error status[]	*/
	short		wd4th[DAD_WEI_MAX];	/* 4段目ﾃﾞｰﾀ[]		*/
	unsigned short	sts4th[DAD_WEI_MAX];	/*    " error status[]	*/
	short		wd5th[DAD_WEI_MAX];	/* 5段目ﾃﾞｰﾀ[]		*/
	unsigned short	sts5th[DAD_WEI_MAX];	/*    " error status[]	*/
};

int
dad_wei_read(					/* stack = 134byte	*/
	DAD_ID			pwb,
	struct dades10		*err,
	unsigned short		*sts3,
	short			*dat3,
	unsigned short		*sts4,
	short			*dat4,
	unsigned short		*sts5,
	short			*dat5
	)
{
	struct txt	buf;			/* text buffer		*/
	struct rp2	rtp;			/* return parameter	*/

	buf.cmd = (short)WEI_DAT_REQ;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)&rtp, (void*)&buf,
			(unsigned short)sizeof(buf)) < 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read(&rtp);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	memcpy((void *)sts3, (void *)&rtp.sts3rd, sizeof(rtp.sts3rd));
	memcpy((void *)dat3, (void *)&rtp.wd3rd, sizeof(rtp.wd3rd));
	memcpy((void *)sts4, (void *)&rtp.sts4th, sizeof(rtp.sts4th));
	memcpy((void *)dat4, (void *)&rtp.wd4th, sizeof(rtp.wd4th));
	memcpy((void *)sts5, (void *)&rtp.sts5th, sizeof(rtp.sts5th));
	memcpy((void *)dat5, (void *)&rtp.wd5th, sizeof(rtp.wd5th));
	if(dad_afv_err[pwb].stat) {
		memcpy((void *)err, (void *)&dad_afv_err[pwb], sizeof(*err));
		memset((void *)&dad_afv_err[pwb], 0,
				sizeof(dad_afv_err[pwb]));	return(1);
	}
	memset((void *)err, 0, sizeof(*err));
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	return;
}
static void chg_ul_aft_read(struct rp2 *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
int	ielm1;
	for (ielm1 = 0; ielm1 < DAD_WEI_MAX; ++ielm1) {
		CHG_UL_SHORT(text->wd3rd[ielm1]);
		CHG_UL_USHORT(text->sts3rd[ielm1]);
		CHG_UL_SHORT(text->wd4th[ielm1]);
		CHG_UL_USHORT(text->sts4th[ielm1]);
		CHG_UL_SHORT(text->wd5th[ielm1]);
		CHG_UL_USHORT(text->sts5th[ielm1]);
	}
	return;
}
#endif	/* MAS_BIG_ENDIAN */
