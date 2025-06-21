/*======================================================================
 * File name    : icr001.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: ＩＣＵ受信タスク
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/icr001.c 1.1.4102.1 1997/05/28 12:12:52 yonetsu Exp $
 *$Log: icr001.c $
 * リビジョン 1.1.4102.1  1997/05/28  12:12:52  yonetsu
 * 受信処理、エラー処理を外部の関数で行えるようにする。
 * rx_dev_func(),rx_func(),rx_err_func() の各関数を外部で定義
 * することが必要。
 * これら３つの関数は、他の受信タスクでも共通のため、デバイス
 * 番号で区別し、処理すること。
 * 
 * リビジョン 1.1  1996/05/31  16:31:42  TGL
 * *** 空ログメッセージ ***
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<assert.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show include		*/

#include	"rxdef.h"			/* Show current dir.	*/
#include	"cmfhdl.h"			/* Show current dir.	*/
#include	"fnctbl.h"

#include	"icr001.h"			/* Show current dir.	*/
/*----------------------------------------------------------------------*/
/*	icu_rcv_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＩＣＵから受信したテキストの解析を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	init_code	: タスクに与える初期値				*/
/*									*/
/* 戻り値								*/
/*	void		: 無し						*/
/*----------------------------------------------------------------------*/

static void
icu_rcv_tsk(
	int	init_code		/* タスクに与える初期値	*/
	)
{
	int		len = init_code;	/* テキスト長		*/
	unsigned char	buf[ICU_MES_MAX];	/* メッセージ・バッファ	*/

	for(;;) {
		SLP_TSK();
		len = rx_dev_func(ICU_DN,buf,sizeof(buf));
		if(len > 0)		rx_func(ICU_DN,len,buf);
		else if(len < 0)	rx_err_func(ICU_DN,len);
	}
}

/*----------------------------------------------------------------------*/
/*	setup_icu_rcv							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ＩＣＵからの受信タスクの生成及び起動を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	void	: 無し							*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/

int
setup_icu_rcv(void)
{
	struct pktsk	tsk_pkt;		/* タスク生成用パケット	*/
	int		err;			/* error code		*/

	tsk_pkt.tsk_adr = icu_rcv_tsk;
	tsk_pkt.priority = ICU_RCV_TSK_PRI;
	tsk_pkt.stk_size = ICU_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	err = CRE_TSK( &icurcv_tsk.acadr, ICU_RCV_TSK_ID, &tsk_pkt);
	if(err) {
		assert(!err);					return(-1);
	}
	err = STA_TSK( icurcv_tsk.acadr, 0);
	if(err) {
		assert(!err);					return(-1);
	}
	return(0);
}
