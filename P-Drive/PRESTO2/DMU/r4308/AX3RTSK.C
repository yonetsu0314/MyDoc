/*======================================================================
 * File name    : ax3rtsk.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: AUX3 受信タスク
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/ax3rtsk.c 1.1.4102.1 1997/05/28 11:42:26 yonetsu Exp $
 *$Log: ax3rtsk.c $
 * リビジョン 1.1.4102.1  1997/05/28  11:42:26  yonetsu
 * 受信処理、エラー処理を外部の関数で行えるようにする。
 * rx_dev_func(),rx_func(),rx_err_func() の各関数を外部で定義
 * することが必要。
 * これら３つの関数は、他の受信タスクでも共通のため、デバイス
 * 番号で区別し、処理すること。
 * 
 * リビジョン 1.1  1996/12/04  10:55:17  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_AX3RTSK_C_
#include <assert.h>			/* ANSI std */
#include <rxif\rx116.h>			/* Show include */

#include "rxdef.h"			/* Show current dir. */
#include "cmfhdl.h"			/* Show current dir. */
#include "fnctbl.h"			/* Show current dir. */

#define STORAGE
#include "ax3rtsk.h"			/* Show current dir. */
#undef STORAGE

/*****************************************************************************
*
*	関数名: static void far aux3_rcv_tsk(unsigned int init_code)
*	機　能: AUX3 から受信したテキストの解析を行う。
*
*	引き数:	unsigned int init_code  -  ﾀｽｸに与える初期値
*
*	戻り値: なし
*
******************************************************************************/
static void aux3_rcv_tsk(int init_code)
{
	int	len = init_code;		/* テキスト長 */
	/*** コンパイルワーニングをなくす為のダミー記述 ***/
	static unsigned char buf[AUX_MES_MAX];	/* メッセージバッファ */

	for(;;) {
		SLP_TSK();
		len = rx_dev_func(AUX3_DN,buf,sizeof(buf));
		if(len > 0)		rx_func(AUX3_DN,len,buf);
		else if(len < 0)	rx_err_func(AUX3_DN,len);
	}
}
/*****************************************************************************
*
*	関数名: int setup_aux3_rcv_tsk(void)
*	機　能: AUX3からの受信タスクの生成及び起動を行う。
*
*	引き数:	なし
*
*	戻り値: int
*			 0	: complete
*			-1	: error code
*
******************************************************************************/
int setup_aux3_rcv_tsk(void)
{
	struct pktsk tsk_pkt;	/* タスク生成用パケット */
	int err;		/* error code */

	tsk_pkt.tsk_adr = aux3_rcv_tsk;
	tsk_pkt.priority = AUX3_RCV_TSK_PRI;
	tsk_pkt.stk_size = AUX3_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	err = CRE_TSK(&aux3rcv_tsk.acadr, AUX3_RCV_TSK_ID, &tsk_pkt);
	assert(!err);
	err = STA_TSK(aux3rcv_tsk.acadr, 0);
	assert(!err);
	return(err);
}
