/*----------------------------------------------------------------------*
 * File name	: dadcwr.c	/ dadc text write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/rcs/dadcwr.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadcwr.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/05/12
 * 前リビジョンでmemcpyの使用をやめたが、データを誤って読みだす原因がWCR1の
 * WAITSELをセットしていなかったためとわかったので、memcpyに戻す。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/04/29
 * WCU-ADC間のデータリードでmemcpyを使用すると誤ったデータを読み出す
 * 不具合対策として、memcpyの使用をやめ、1バイトずつ読み出すように変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadcwr.c 1.3 1970/01/01
 *Log: dadcwr.c
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
 *Header: p:/lib/drive/ndadc/rcs/dadcwr.c 1.1 1996/07/22 13:36:30 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	bufで示されるlenバイトのテキストに戻り値へのポインタ及び、	*/
/*	先頭に管理用データ長のヘッダー部を付加し、デュアルポート	*/
/*	RAMに書き込みを行う。その後実行終了を待ち、エラーの有無を	*/
/*	戻り値で返し、リターンパラメータが含まれる場合retで示され	*/
/*	るエリアに、リターンパラメータ構造体のサイズを除いたパラ	*/
/*	メータ本体をコピーする。					*/
/*	ただしretがNULLの場合はリターン･コード及びパラメータの無い	*/
/*	テキストとして扱われテキストの書き込み処理のみとなる (この	*/
/*	場合の応答はコンプリートのテキストが別途返される)。		*/
/*									*/
/*	<<< DSP text format >>>						*/
/*		+-------+-------+------ // ---------------+-------+	*/
/*		|ﾍｯﾀﾞｰ  |戻り値 |<-----  len  ----------->|戻り値 |	*/
/*		|LEN	|への&	|    CMD & PARAMETER	  |の先頭 |	*/
/*		+-------+-------+------ // ---------------+-------+	*/
/*		          |      buf[0] buf[1]	･･･	   ↑ﾘﾀｰﾝ･ｺｰﾄﾞ 	*/
/*			  +---------------------------------+ orｻｲｽﾞ	*/
/*	    struct retprm {						*/
/*		int  sizeof(struct retprm);  ･･･ ﾘﾀｰﾝ･ｺｰﾄﾞまたは	*/
/*						 ｻｲｽﾞ(member無し)	*/
/*		???  member;		     ･･･ 有無はﾃｷｽﾄに依存	*/
/*	    };								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: board number					*/
/*	*ret		: pointer to return parameter			*/
/*	*buf		: pointer to text buffer			*/
/*	len		: number of byte				*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<stdio.h>			/*	"		*/
#include	<stddef.h>			/*	"		*/
#include	<limits.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\dadc.h>			/* Show biosif		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\pdadcdrv.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/
#include	"dadctxt.h"			/*	"		*/

#ifdef	NDEBUG
#define	error_proc()	return(-1);
#else
#define	error_proc()	continue;
#endif

static union aap	dad_tx_sem[DAD_MAX] = {
 		0L				/* DSPTXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ*/
};
static unsigned char	dad_tx_nest[DAD_MAX] = {
		(unsigned char)0		/* WR関数のﾈｽﾃｨﾝｸﾞ回数	*/
};

int
dad_dpm_write(					/* stack = 22byte	*/
	DAD_ID		pwb,
	void		*ret,
	void		*buf,
	short		len
	)
{
	struct dadctxt		*ctp;		/* pnt to DADC context	*/
	unsigned char		*txtp;		/* pointer to text buff	*/
	unsigned short		retp;		/* offset of return prm	*/
	int			err =0, i;
	long			l;
	short			s;		/* error code or length	*/
	unsigned short		us_yamana;
	int			iyamana;

	ctp = &dadc_ctxt_tbl[pwb];
	txtp = ctp->iobase+ctp->wrtxt;
	if(len +sizeof(len) > ctp->wrtxt_siz)			return(-1);
	ADV_WAI_SEM(err, dad_tx_nest[pwb], ENDLESS,
		    dad_tx_sem[pwb].acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);
	i = 0;

	while(verify_geti((void *)(ctp->iobase+ctp->wrcmd)) != CMD_ACK) {
		if(err = HLT_TSK(&dad_poll_time))		return(err|INT_MIN);
		if(++i > dad_tmout/dad_poll_time)  error_proc();
	}

/* [TEXT LENGTH] */
	retp = (unsigned short)(len +sizeof(len) +sizeof(retp));
	us_yamana = retp;
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_USHORT(us_yamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(us_yamana);
#endif
	memcpy(txtp, &us_yamana, sizeof(us_yamana));

/* [Pointer to return value.] */
	us_yamana = (unsigned short)(ctp->wrtxt + retp);
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_USHORT(us_yamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(us_yamana);
#endif
	memcpy((txtp+sizeof(len)), &us_yamana, sizeof(us_yamana));

/* [TEXT DATA] */
#ifdef	MAS_P5561
	inverse_dadc((char *)buf, len);
#endif
	memcpy((void *)(txtp +(sizeof(len)+sizeof(retp))), buf, (size_t)len);

	us_yamana = 0;
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_USHORT(us_yamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(us_yamana);
#endif
	memcpy((txtp+retp), &us_yamana, sizeof(short));

	us_yamana = DEFAULT;
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_USHORT(us_yamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(us_yamana);
#endif
	do {
		iyamana = verify_puti((unsigned short)us_yamana, (unsigned short *)(ctp->iobase +ctp->wrcmd));
	} while(iyamana);
	if(ret == NULL)
		s = (unsigned short)0;
	else {
		i = 1;
		do {	while((l = verify_geti((void *)(txtp +retp))) == -1L);
			if(++i == 0)  error_proc();
		} while(l == 0);

		s =(short)l;
#ifdef	MAS_BIG_ENDIAN
		CHG_UL_SHORT(s);	/* 整数型変数の上位と下位を入れ替える。*/
#endif	/* MAS_BIG_ENDIAN */

		if((s > 0)&&(s <= 3))
			*(short *)ret = s;
		else {
			if((s -= sizeof(s)) < 2)
				len = (short)-1;
			else {
				memcpy((void *)ret, (void *)(txtp+(retp+sizeof(s))), (size_t)s);
			}
#ifdef	MAS_P5561
				inverse_dadc((char *)ret, s);
#endif
		}
	}
	ADV_SIG_SEM(err, dad_tx_nest[pwb], dad_tx_sem[pwb].acadr);
	if(err)							return(err|INT_MIN);
	return((int)len);
}

/*----------------------------------------------------------------------*/
/*	init_dad_dpm_write						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	bord_no	: ADC board number					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	送信関係の処理の初期化を行う。					*/
/*----------------------------------------------------------------------*/

int
init_dad_dpm_write(
	DAD_ID		bord_no
	)
{
	unsigned int	id[DAD_MAX];
	int		err;

	id[DADC1] = DAD1TX_SEM_ID;
	id[DADC2] = DAD2TX_SEM_ID;
	id[DADC3] = DAD3TX_SEM_ID;
	id[DADC4] = DAD4TX_SEM_ID;
	if(err = CRE_SEM( FIFO, &dad_tx_sem[bord_no].acadr,
			  id[bord_no], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, dad_tx_sem[bord_no].acadr,
			  (unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
