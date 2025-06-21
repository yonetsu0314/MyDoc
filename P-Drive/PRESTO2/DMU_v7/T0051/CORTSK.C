/*======================================================================
 * File name	: cortsk.c
 * Original		: p:/dacs_i/rcu/main/rcs/cortsk.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 * 		CON通信 受信タスク
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/cortsk.c 1.8 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cortsk.c $
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/26 10:30
 * WCUからのcon0出力をDMUのcon1に出力しないようにcon0_rcv_tskを変更
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2003/12/12 19:30
 * プロトタイプ宣言を追加しワーニングをなくす
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/12/12 19:00
 * con0_rcv_tsk を変更し、WCU
 * と通信できるようにする。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/21 12:00
 * MSEC を定義するためsh7615\bootpara.h をインクルードする
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_CORTSK_C_
#include <stddef.h>
#include <string.h>
#include <kernel.h>
#include <rxif\rx116.h>		/* Show XLIB\INC */

#include <sh7615\bootpara.h>
#include "rxdef.h"			/* Show Current */
#include "cmfhdl.h"			/* Show Current */

#define STORAGE
#include "cortsk.h"			/* Show Current */
#undef STORAGE
#include "rsinout.h"			/* Show Current */
#include "tsk_lan.h"			/* Show Current */


/*****************************************************************************
*
*	関数名: static void far con0_rcv_tsk(unsigned int init_code)
*	機　能: CON0 から受信したテキストの解析を行う。
*
*	引き数:	unsigned int init_code  -  ﾀｽｸに与える初期値
*
*	戻り値: なし
*
******************************************************************************/
static void far con0_rcv_tsk(unsigned int init_code)
{
	/* コンパイルワーニングをなくす為のダミー記述 */
	static unsigned char buf[RCU_MES_MAX];	/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ	*/
	int		nRead,j,i = init_code;	/* error code		*/

	con0_buf_clear();
	for(;;) {
		SLP_TSK();
		nRead = con0_gets((unsigned char *)buf, (int)RCU_MES_MAX);
		if(nRead > 0) {
			if(CmdFd != -1) continue;
			if((int_buf0_count-int_buf0_cntout)<(INT_BUF_size-nRead)){
				if(int_buf0_in <= (int_buf0+INT_BUF_size-nRead)){
					memcpy((void *)int_buf0_in,(void *)buf,(size_t)nRead);
					int_buf0_in += nRead;
					int_buf0_count += nRead;
					if(int_buf0_in >= int_buf0+INT_BUF_size) int_buf0_in = int_buf0;
					continue;
				}
				else{
					i = INT_BUF_size - (int_buf0_in - int_buf0);
					j = nRead - i;
					memcpy((void *)int_buf0_in,(void *)buf,(size_t)i);
					memcpy((void *)int_buf0,(void *)(buf+i),(size_t)j);
					int_buf0_in = int_buf0 + j;
					int_buf0_count += nRead;
					continue;
				}
			}
			for(i=0;i<nRead;++i){
lll:
				if((int_buf0_count-int_buf0_cntout)<INT_BUF_size){
					*int_buf0_in++ = buf[i];
					int_buf0_count++;
					if(int_buf0_in >= int_buf0+INT_BUF_size) int_buf0_in = int_buf0;
				}
				else{
					int_buf0_ovfl = (unsigned char)1;
					for(;;){
						if(chkbuflen()) break;
						dly_tsk((RELTIM)100/MSEC);
					}
					int_buf0_ovfl = (unsigned char)0;
					goto lll;
				}
			}
		}
	}
}


/*****************************************************************************
*
*	関数名: int setup_con0_rcv_tsk(void)
*
*	機　能: CON0 からの受信タスクの生成及び起動を行う。
*
*	引き数:	なし
*
*	戻り値: int
*					 0	: complete
*					-1	: error code
*
******************************************************************************/
int setup_con0_rcv_tsk(void)
{
	struct pktsk tsk_pkt;	/* ﾀｽｸ生成用ﾊﾟｹｯﾄ */
	int err;				/* error code */

	tsk_pkt.tsk_adr = (void *)con0_rcv_tsk;
	tsk_pkt.priority = CON0_RCV_TSK_PRI;
	tsk_pkt.stk_size = CON0_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	err = CRE_TSK(&con0_rcv_tsk_aa.acadr, (unsigned int)CON0_RCV_TSK_ID, &tsk_pkt);
	err = STA_TSK(con0_rcv_tsk_aa.acadr, (unsigned int)0);
	return(err);
}


/*****************************************************************************
*
*	関数名: static void far con1_rcv_tsk(unsigned int init_code)
*	機　能: CON1 から受信したテキストの解析を行う。
*
*	引き数:	unsigned int init_code  -  ﾀｽｸに与える初期値
*
*	戻り値: なし
*
******************************************************************************/
static void far con1_rcv_tsk(unsigned int init_code)
{
	/* コンパイルワーニングをなくす為のダミー記述 */
	static unsigned char buf[RCU_MES_MAX];	/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ	*/
	int		nRead,j,i = init_code;	/* error code		*/

	con1_buf_clear();
	for(;;) {
		SLP_TSK();
		nRead = con1_gets((unsigned char *)buf, (int)RCU_MES_MAX);
		if(nRead > 0) {
			if(rskeysw){
				con0_puts((unsigned char *)buf,nRead);
				continue;
			}
			if(CmdFd != -1) continue;
			if((int_buf1_count-int_buf1_cntout)<(INT_BUF_size-nRead)){
				if(int_buf1_in <= (int_buf1+INT_BUF_size-nRead)){
					memcpy((void *)int_buf1_in,(void *)buf,(size_t)nRead);
					int_buf1_in += nRead;
					int_buf1_count += nRead;
					if(int_buf1_in >= int_buf1+INT_BUF_size) int_buf1_in = int_buf1;
					continue;
				}
				else{
					i = INT_BUF_size - (int_buf1_in - int_buf1);
					j = nRead - i;
					memcpy((void *)int_buf1_in,(void *)buf,(size_t)i);
					memcpy((void *)int_buf1,(void *)(buf+i),(size_t)j);
					int_buf1_in = int_buf1 + j;
					int_buf1_count += nRead;
					continue;
				}
			}
			for(i=0;i<nRead;++i){
lll:
				if((int_buf1_count-int_buf1_cntout)<INT_BUF_size){
					*int_buf1_in++ = buf[i];
					int_buf1_count++;
					if(int_buf1_in >= int_buf1+INT_BUF_size) int_buf1_in = int_buf1;
				}
				else{
					int_buf1_ovfl = (unsigned char)1;
					for(;;){
						if(chkbuflen()) break;
						dly_tsk((RELTIM)100/MSEC);
					}
					int_buf1_ovfl = (unsigned char)0;
					goto lll;
				}
			}
		}
	}
}
/*****************************************************************************
*
*	関数名: int setup_con1_rcv_tsk(void)
*
*	機　能: CON1 からの受信タスクの生成及び起動を行う。
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

	tsk_pkt.tsk_adr = (void *)con1_rcv_tsk;
	tsk_pkt.priority = CON1_RCV_TSK_PRI;
	tsk_pkt.stk_size = CON1_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;
	err = CRE_TSK(&con1_rcv_tsk_aa.acadr, (unsigned int)(CON1_RCV_TSK_ID), &tsk_pkt);
	err = STA_TSK(con1_rcv_tsk_aa.acadr, (unsigned int)0);
	return(err);
}


