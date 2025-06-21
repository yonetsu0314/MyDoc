/*----------------------------------------------------------------------*
 * File name	: dadmw1.c	/ dadc internal memory write		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/dadmw1.c 1.2 1970/01/01 00:00:00 m-yamana Exp $
 *$Log: dadmw1.c $
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
 *Header: p:/lib/drive/ndadc/rcs/dadmw1.c 1.1 1996/07/22 13:37:06 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	(*load)の内容をADCの内部ﾒﾓﾘｰ(*mem)に書き込みを行い、実際に	*/
/*	書き込んだbyte数を返す。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ADC board number				*/
/*	*mem		: ﾗｲﾄするDSPのﾒﾓﾘｰ･ｱﾄﾞﾚｽ			*/
/*	*load		: ﾛｰﾄﾞするﾃﾞｰﾀのｱﾄﾞﾚｽ				*/
/*	byte		: ﾗｲﾄするﾊﾞｲﾄ数					*/
/*									*/
/* 戻り値								*/
/*	int		: N	･･･ 正常終了				*/
/*			  0	･･･ ﾗｲﾄ未実行				*/
/*			  -1	･･･ 通信ｴﾗｰ				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<assert.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dpmsim.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/

#define	MAX_DBUF	8		/* 最大書き込みワード数		*/

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read(struct txtr *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

struct txt {				/* text format			*/
	short		cmd;		/* command = MEM_WRITE		*/
/*	void		*mem; m-yamana*/		/* ﾗｲﾄするDSPﾒﾓﾘｰへのﾎﾟｲﾝﾀｰ	*/
	short		mem;		/* ﾗｲﾄするDSPﾒﾓﾘｰへのﾎﾟｲﾝﾀｰ	*/
	short		byte;		/* ﾗｲﾄするﾊﾞｲﾄ数		*/
	short		data[MAX_DBUF];	/* 可変長ﾃﾞｰﾀ[byte/2]		*/
};

struct txtr {				/* ｺﾝﾌﾟﾘｰﾄ･ﾃｷｽﾄ			*/
	short		cmd;		/* command = EOF_MEM_WRITE	*/
	short		code;		/* return code			*/
};

int
dad_memwr(					/* stack = 34byte	*/
	DAD_ID		pwb,
	char		*mem,
	char		*load,
	short		byte
	)
{
	struct txt	buf;			/* text buffer		*/
	struct txtr	ret;			/* return text		*/
	int		err, n, i, j;
	int		iyamana;

	ret.cmd = (short)0;
	ret.code = (short)0;
	err = 0;
	iyamana = 0;

	byte = (short)((byte/2)*2);
	buf.cmd = (short)MEM_WRITE;
	for(n = byte; n > 0; n -= i) {
		if(n > sizeof(buf.data))  i = sizeof(buf.data);
		else			  i = n;
		memcpy((void *)buf.data, (void *)&load[j =(byte-n)],
						 (size_t)(buf.byte =(short)i));
/*		buf.mem = &mem[j/2];	m-yamana */
		buf.mem = (short)mem[j/2];
#ifdef	MAS_BIG_ENDIAN
	chg_ul_bef_write(&buf);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
		if(dad_dpm_write(pwb, NULL, (void *)&buf, 
				(unsigned short)sizeof(buf)) < 0)
								return(-1);
		if(err = wai_mem_write_flg(pwb)) {
			assert(!err);				return(-1);
		}
		iyamana = dad_dpm_read(pwb, (void *)&ret, sizeof(ret), (unsigned short)0);
#ifdef	MAS_BIG_ENDIAN
		chg_ul_aft_read(&ret);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
		if ((iyamana < 0) || (ret.code < 0))			return(-1);
	}
	return((int)byte);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
int	ielm1, ielm2;
	CHG_UL_SHORT(text->cmd);
	CHG_UL_SHORT(text->mem);	/* m-yamana これはどうしたら？ */
	CHG_UL_SHORT(text->byte);
	for (ielm1 = 0; ielm1 < MAX_DBUF; ++ielm1) {
		CHG_UL_SHORT(text->data[ielm1]);
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
