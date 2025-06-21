/*----------------------------------------------------------------------*
 * File name	: scuinit.c	/ initialize scu i/o			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/scu/rcs/scuinit.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuinit.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/09/25 19:00
 * TRAP中に割り込みを禁止していた処理をやめる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/09/19 16:00
 * TRAP中は割り込み禁止とする。
 * TRAP中にハードウェア割り込みが発生すると暴走する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	SCUの初期化を行い、I/Oへのアクセス環境へのポインタを返す。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	char_len	: ｷｬﾗｸﾀ長					*/
/*	stp_len		: ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数					*/
/*	pari_sel	: ﾊﾟﾘﾃｨｰ選択					*/
/*									*/
/* 戻り値								*/
/*	int  *	: I/Oｺﾝﾃｷｽﾄへのﾎﾟｲﾝﾀ					*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif		*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE */
#include	<dos.h>

int *
_scu_init(
	int	ch,
	int	char_len,			/* ｷｬﾗｸﾀ長		*/
	int	stp_len,			/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int	pari_sel			/* ﾊﾟﾘﾃｨｰ選択		*/
	)
{
	int *err;
//	int	imask_bak;
	
//	imask_bak = get_imask();
//	set_imask(15);
	err = (int *)trapa_svc(VECTOR_SCU, 0x0, ch, char_len, stp_len, pari_sel);
//	set_imask( imask_bak );
	return(err);
}
