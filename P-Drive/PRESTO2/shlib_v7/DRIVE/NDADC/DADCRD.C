/*----------------------------------------------------------------------*
 * File name	: dadcrd.c	/ dadc text read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/rcs/dadcrd.c 1.5 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadcrd.c $
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2005/05/11
 * 前リビジョンでmemcpyの使用をやめたが、データを誤って読みだす原因がWCR1の
 * WAITSELをセットしていなかったためとわかったので、memcpyに戻す。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2005/04/29
 * WCU-ADC間のデータリードでmemcpyを使用すると誤ったデータを読み出す
 * 不具合対策として、memcpyの使用をやめ、1バイトずつ読み出すように変更する。
 * 
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
 *Header: p:/lib/drive/ndadc/rcs/dadcrd.c 1.1 1996/07/22 13:36:27 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信バッファの内容を、指定バイト数目からテキスト単位でユー	*/
/*	ザー･バッファにコピーする。 バッファの内容はバイナリーテキ	*/
/*	ストを対象としているため、先頭に管理用ヘッダー部としてデー	*/
/*	タ長(int)が付加されているが、テキストからは除去され、sizeof	*/
/*	(int)-2した値が戻り値として返される。				*/
/*									*/
/*	<<< text format >>>						*/
/*		 <--------------       len       -------------->	*/
/*		+-------+-------+--------- // ------------------+	*/
/*		|  len  | cmd   |	parameter		|	*/
/*		+-------+-------+--------- // ------------------+	*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: board number					*/
/*	*buf		: 転送先へのﾎﾟｲﾝﾀ				*/
/*	max		: 転送先のﾊﾞｯﾌｧｻｲｽﾞ				*/
/*	offset 		: ﾃｷｽﾄのｺﾋﾟｰを開始するoffset			*/
/*									*/
/* 戻り値								*/
/*	int		: N	･･･ 正常終了(length)			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\dadc.h>			/* Show biosif		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\ndadcdrv.h>		/*	"		*/
#include	"dadctl.h"			/* Show current		*/
#include	"dadctxt.h"			/*	"		*/

static union aap	dad_rx_sem[DAD_MAX] = {
		0L				/* DSPRXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ*/
};
static unsigned char	dad_rx_nest[DAD_MAX] = {
		(unsigned char)0		/* RD関数のﾈｽﾃｨﾝｸﾞ回数	*/
};

int
dad_dpm_read(					/* stack = 18byte	*/
	DAD_ID		pwb,
	void		*buf,
	unsigned long	max,
	unsigned short	offset
	)
{
	struct dadctxt		*ctp;		/* pnt to DADC context	*/
	unsigned char		*txtp;		/* poiter to text buff	*/
	short			len;		/* number of byte	*/
	int			err =0;
	int			iack, iyamana;

	if(max == 0)						return(-1);
	ctp = &dadc_ctxt_tbl[pwb];
	txtp = ctp->iobase + ctp->rdtxt;
	ADV_WAI_SEM(err, dad_rx_nest[pwb], ENDLESS,
		    dad_rx_sem[pwb].acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);

	len = *WD_PTR(txtp);
	if (len != 0) {
		len -= (short)(sizeof(len)+ offset);
		if ((len > 0) && (len <= max)) {
			memcpy(buf, (void *)(txtp+(sizeof(len)+ offset)), (size_t)len);
#ifdef	MAS_P5561
			inverse_dadc((char *)buf, len);
#endif
		} else {
			len = -1;
		}
	} else {
		len = -1;
	}

	iack = CMD_ACK;
#ifdef	MAS_P5561
	INVERSION(iack);
#endif
	do {
		iyamana = verify_puti((unsigned short)iack, (unsigned short *)(ctp->iobase+ctp->rdcmd));
	} while(iyamana);

	ADV_SIG_SEM(err, dad_rx_nest[pwb], dad_rx_sem[pwb].acadr);
	if(err)							return(err|INT_MIN);
	return((int)len);
}

/*----------------------------------------------------------------------*/
/*	init_dad_dpm_read						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: ADC board number					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信関係の処理の初期化を行う。					*/
/*----------------------------------------------------------------------*/

int
init_dad_dpm_read(
	DAD_ID		pwb
	)
{
	unsigned int	id[DAD_MAX];
	int		err;

	id[DADC1] = DAD1RX_SEM_ID;
	id[DADC2] = DAD2RX_SEM_ID;
	if(err = CRE_SEM( FIFO, &dad_rx_sem[pwb].acadr,
			  id[pwb], (unsigned int)1))		return(-1);
	if(err = WAI_SEM( ENDLESS, dad_rx_sem[pwb].acadr,
			  (unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
