/*======================================================================
 * File name	: cor1tsk.c
 * Original	:
 *----------------------------------------------------------------------
 * Function 	: CON1通信 受信タスク
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/cortsk.c 1.1 1997/05/28 12:07:02 yonetsu Exp $
 *$Log: cortsk.c $
 * リビジョン 1.1  1997/05/28  12:07:02  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_COR1TSK_C_
#include <assert.h>		/* Show MSC\INC */
#include <rxif\rx116.h>		/* Show XLIB\INC */

#include "rxdef.h"		/* Show Current */
#include "cmfhdl.h"		/* Show Current */
#include "fnctbl.h"		/* Show Current */

#define STORAGE
#include "cor1tsk.h"		/* Show Current */
#undef STORAGE

/*****************************************************************************
*
*	関数名: static void far con_rcv_tsk(unsigned int init_code)
*	機　能: CON から受信したテキストの解析を行う。
*
*	引き数:	unsigned int init_code  -  ﾀｽｸに与える初期値
*
*	戻り値: なし
*
******************************************************************************/
static void con1_rcv_tsk(int init_code)
{
	int len = init_code;
	/* コンパイルワーニングをなくす為のダミー記述 */
	static unsigned char buf[RCU_MES_MAX];	/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ	*/

	for(;;) {
		SLP_TSK();
#if 1 /* chimura */
		len = rx_dev_func(CON_DN1,buf,1);
#else
		len = rx_dev_func(CON_DN1,buf,sizeof(buf));
#endif
		if(len > 0) {
			len = con1_text_check(buf, len);
			if(len) {
				rx_func(CON_DN1,len,buf);
			}
		}
		else if(len < 0)	rx_err_func(CON_DN1,len);
	}
}
/*****************************************************************************
*
*	関数名: int setup_con_rcv_tsk(void)
*
*	機　能: CON からの受信タスクの生成及び起動を行う。
*
*	引き数:	なし
*
*	戻り値: int
*					 0	: complete
*					-1	: error code
*
******************************************************************************/
int setup_con1_rcv_tsk(void)
{
	struct pktsk tsk_pkt;	/* ﾀｽｸ生成用ﾊﾟｹｯﾄ */
	int err;				/* error code */

	tsk_pkt.tsk_adr = con1_rcv_tsk;
	tsk_pkt.priority = CON1_RCV_TSK_PRI;
	tsk_pkt.stk_size = CON1_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	err = CRE_TSK(&con1rcv_tsk.acadr, CON1_RCV_TSK_ID, &tsk_pkt);
	assert(!err);
	err = STA_TSK(con1rcv_tsk.acadr, 0);
	assert(!err);
	return(err);
}
