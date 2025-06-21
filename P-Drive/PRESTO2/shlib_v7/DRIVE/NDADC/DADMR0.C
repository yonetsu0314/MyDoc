/*----------------------------------------------------------------------*
 * File name	: dadmr0.c	/ dadc internal memory read		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadmr0.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadmr0.c $
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
 *Header: p:/lib/drive/ndadc/rcs/dadmr0.c 1.1 1996/07/22 13:36:57 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	AFV-DSPの内部ﾒﾓﾘｰ(*mem)の内容を(*rom_id)へ転送し、実際に読み	*/
/*	込んだbyte数を返す。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	*rom_id		: ﾘｰﾄﾞ･ﾃﾞｰﾀのｽﾄｱｰ先				*/
/*	*mem		: ﾘｰﾄﾞするDSPのﾒﾓﾘｰ･ｱﾄﾞﾚｽ			*/
/*	byte		: ﾘｰﾄﾞするﾊﾞｲﾄ数				*/
/*									*/
/* 戻り値								*/
/*	int		: N	･･･ 正常終了				*/
/*			  0	･･･ ﾘｰﾄﾞ未実行				*/
/*			  -1	･･･ ｴﾗｰ					*/
/*----------------------------------------------------------------------*/

#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);		/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(unsigned char *rom_id);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {				/* text format			*/
	short		cmd;		/* command = MEM_READ		*/
/*	short		*mem;*/		/* ﾘｰﾄﾞするDSPﾒﾓﾘｰへのﾎﾟｲﾝﾀｰ	*/
	short		mem;		/* ﾘｰﾄﾞするDSPﾒﾓﾘｰへのﾎﾟｲﾝﾀｰ	*//* m-yamana */
	short		byte;		/* ﾘｰﾄﾞするﾊﾞｲﾄ数		*/
};

int
dad_dsp_memrd(					/* stack = 6byte	*/
	DAD_ID		pwb,
	void		*rom_id,
	short		*mem,
	short		byte
	)
{
	struct txt	buf;			/* text buffer		*/

	if((buf.byte = byte) <= 0)				return(0);
	buf.cmd = (short)DSP_MEM_READ;
	buf.mem = (short)mem;
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
	if(dad_dpm_write(pwb, (void *)rom_id, (void *)&buf,
			(short) sizeof(buf))	< 0)	return(-1);
#ifdef	MAS_BIG_ENDIAN
	chg_ul_aft_read((unsigned char *)rom_id);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
	return((int)byte);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->mem);	/* m-yamana これはどうしたら？ */
	CHG_UL_SHORT(text->byte);
	return;
}
static void chg_ul_aft_read(unsigned char *rom_id)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
/* unsigned char rom_id[ID_SIZE]; *//* yamana */
	return;
}
#endif	/* MAS_BIG_ENDIAN */
