/*======================================================================
 * File name    : fnctbl.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/fnctbl.c 1.3.4308.1 1998/09/01 14:58:10 yonetsu Exp $
 *$Log: fnctbl.c $
 * リビジョン 1.3.4308.1  1998/09/01  14:58:10  yonetsu
 * 予約交信仕様
 * 
 * リビジョン 1.3  1998/05/15  14:03:45  yonetsu
 * setup_etc の追加
 * 
 * リビジョン 1.2  1997/06/04  10:15:56  yonetsu
 * get_pkt_spec の間違いを修正
 * 
 * リビジョン 1.1  1997/05/28  13:33:08  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#define _FNCTBL_C_

#include <stdio.h>
#include <rxif\rx116.h>
#include "sumfnc.h"
#include "txfnc_id.h"

#include "fnctbl.h"	/* 最後にインクルードすること */

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   受信タスクが起動された時、どの関数をコールするかをテーブルから	*/
/*   検索し、実行する。							*/
/*   複数の定義が有れば、全て実行される。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
void rx_func(
	int dev_no,
	int len,
	const unsigned char *buf
	)
{
	int i;

	for(i=0 ;ftbl[i].dev_no != -1;i++) {
		if(dev_no == ftbl[i].dev_no) {
			if(ftbl[i].apl_rx_func != NULL)
				ftbl[i].apl_rx_func(dev_no,len,buf,ftbl[i].non_accpt_cmd,ftbl[i].accpt_cmd);
		}
	}
	return;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   受信タスクが起動された受信エラーが発生した時、どの関数をコールする */
/*   かをテーブルから検索し、実行する。					*/
/*   複数の定義が有れば、最初に定義されている関数のみ実行される。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
void rx_err_func(
	int dev_no,
	int err_stat
	)
{
	int i;

	for(i=0 ;ftbl[i].dev_no != -1;i++) {
		if(dev_no == ftbl[i].dev_no) {
			if(ftbl[i].rx_err_func != NULL)
				ftbl[i].rx_err_func(dev_no,err_stat);
				return;
		}
	}
	return;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int is_accpt_cmd(
	unsigned char cmd,
	const unsigned char *non_accpt_cmd,
	const unsigned char *accpt_cmd
	)
{
	int	i;

	if(non_accpt_cmd != (unsigned char *)NULL) {
		for(i=0; non_accpt_cmd[i] != '\0'; i++) {
			if(cmd == non_accpt_cmd[i])		return 0;

		}
		return 1;
	}
	if(accpt_cmd != (unsigned char *)NULL) {
		for(i=0; accpt_cmd[i] != '\0'; i++) {
			if(cmd == accpt_cmd[i])			return 	1;
		}
		return 0;
	}
	return 1;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   メッセージＩＤから、デバイス番号を検索し、該当する送信関数を	*/
/*   実行する。								*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
COMRV tx_func(
	int mes_id,
	int len,
	unsigned char *buf
	)
{
	int i;

	for(i=0 ;txtbl[i].id_no != -1;i++) {
		if(mes_id == txtbl[i].id_no) {
			return tx_dev_func(txtbl[i].dev_no,buf,len);
		}
	}
	return COM_FAIL;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   受信タスクが起動された時、どのサムチェックを行うかをテーブル	*/
/*   から検索し、実行する。						*/
/*   データが、テキストモードに定義されている時のみ実行される。		*/
/*   バイナリーモードの時は、実行されない。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
COMRV sum_func(
	int dev_no,
	unsigned char *buf,
	int len,
	char *txrx
	)
{
	int i;

	for(i=0 ; ftbl[i].dev_no != -1 ; i++) {
		if(dev_no == ftbl[i].dev_no) {
			if((ftbl[i].data_kind == TXT_DT) && (ftbl[i].sumf != NULL))
				return ftbl[i].sumf(buf,len,txrx);
			else if(len <= 0)	return COM_FAIL;
			else			return 0;
		}
	}
	return COM_FAIL;	/* device error */
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   受信タスクが起動された時、read function を使用するか		*/
/*   gets function を使用するかをテーブルから検索し実行する。		*/
/*			又は						*/
/*   送信関数が呼ばれた時、write function を使用するか			*/
/*   puts function を使用するかをテーブルから検索し実行する。		*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
static struct {
	int dev_no;
	COMRV (*gets_func)(unsigned char *buf,int max);
	COMRV (*read_func)(unsigned char *buf,int max);
	COMRV (*puts_func)(unsigned char *buf,int max);
	COMRV (*write_func)(unsigned char *buf,int max);
} rxtx_dev_func_tbl[] = {
 {   CON_DN0 , con0_gets , con0_read , con0_puts , con0_write  },
 {   CON_DN1 , con1_gets , con1_read , con1_puts , con1_write  },
 {   HST_DN  , hst_gets  , hst_read  , hst_puts  , hst_write  },
 {   HIC_DN  , hic_gets  , hic_read  , hic_puts  , hic_write  },
 {   RCU_DN  , rcu_gets  , rcu_read  , rcu_puts  , rcu_write  },
 {   ICU_DN  , icu_gets  , icu_read  , icu_puts  , icu_write  },
 {   AUX0_DN , aux0_gets , aux0_read , aux0_write , aux0_write },
 {   AUX1_DN , aux1_gets , aux1_read , aux1_write , aux1_write },
 {   AUX2_DN , aux2_gets , aux2_read , aux2_puts , aux2_write },
 {   AUX3_DN , aux3_gets , aux3_read , aux3_puts , aux3_write },
 {   AUX4_DN , aux4_gets , aux4_read , aux4_puts , aux4_write },
 {   AUX5_DN , aux5_gets , aux5_read , aux5_puts , aux5_write },
 {	-1   }
};

COMRV rx_dev_func(
	int dev_no,
	unsigned char *buf,
	int max
	)
{
	int i,len;
	int data_kind = -1;

	len = COM_FAIL;
	for(i=0 ; ftbl[i].dev_no != -1 ; i++) {
		if(dev_no == ftbl[i].dev_no) {
			data_kind = ftbl[i].data_kind;
			break;
		}
	}
	len = COM_FAIL;
	for(i=0 ;rxtx_dev_func_tbl[i].dev_no != -1;i++) {
		if(dev_no == rxtx_dev_func_tbl[i].dev_no) {
			if(data_kind == TXT_DT) 
				len = rxtx_dev_func_tbl[i].gets_func(buf,max);
			else if(data_kind == BIN_DT)
				len = rxtx_dev_func_tbl[i].read_func(buf,max);
			else {
				rxtx_dev_func_tbl[i].read_func(buf,max); /* とりあえず読む */
				len = COM_FAIL;
			}
			break;
		}
	}
	return len;
}

COMRV tx_dev_func(
	int dev_no,
	unsigned char *buf,
	int len
	)
{
	int i;
	int data_kind = -1;

	for(i=0 ; ftbl[i].dev_no != -1 ; i++) {
		if(dev_no == ftbl[i].dev_no) {
			data_kind = ftbl[i].data_kind;
			break;
		}
	}
	for(i=0 ;rxtx_dev_func_tbl[i].dev_no != -1;i++) {
		if(dev_no == rxtx_dev_func_tbl[i].dev_no) {
			if(data_kind == TXT_DT) 
				return rxtx_dev_func_tbl[i].puts_func(buf,len);
			else if(data_kind == BIN_DT)
				return rxtx_dev_func_tbl[i].write_func(buf,len);
		}
	}
	return COM_FAIL;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*   デバイスがソフト的に存在するかをチェックする。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int is_device_exist(
	int dev_no
	)
{
	int	i;

	for(i=0; ftbl[i].dev_no != -1 ;i++ ) {
		if(dev_no == ftbl[i].dev_no)
			return 1;
	}
	return 0;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*                                                 			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
void *get_pkt_spec(
	int dev_no
	)
{
	int i;

	for(i=0 ; ftbl[i].dev_no != -1 ; i++) {
		if(dev_no == ftbl[i].dev_no) {
			if(ftbl[i].mod_pkt != NULL)	
				return ftbl[i].mod_pkt(dev_no,ftbl[i].pkt);
			return ftbl[i].pkt;
		}
	}
	return NULL;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*                                                 			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int is_arcnet(
	int no
	)
{
	int	dev_no = no;
	int	i;

	if(no >= COM_DN_MAX) {
		for(i=0 ;txtbl[i].id_no != -1;i++) {
			if(no == txtbl[i].id_no) {
				dev_no = txtbl[i].dev_no;
				break;
			}
		}
	}

	if(dev_no == HST_DN)	return 1;
	else if(dev_no == HIC_DN)	return 1;
	else if(dev_no == RCU_DN)	return 1;
	else if(dev_no == ICU_DN)	return 1;

	return 0;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*                                                 			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int mesid_to_devno(
	int mes_id
	)
{
	int	i;
	
	for(i=0 ;txtbl[i].id_no != -1;i++) {
		if(mes_id == txtbl[i].id_no) {
			return txtbl[i].dev_no;
		}
	}
	return -1;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*                                                 			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int devno_comp_mesid(
	int dev_no,
	int mes_id
	)
{
	int	i;
	
	for(i=0 ;txtbl[i].id_no != -1;i++) {
		if((dev_no == txtbl[i].dev_no) && (mes_id == txtbl[i].id_no)) {
			return 1;
		}
	}
	return 0;
}

/*----------------------------------------------------------------------*/
/*									*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*                                                 			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/* 戻り値								*/
/*----------------------------------------------------------------------*/
int setup_etc(void)
{
	int	i,err;

	for(i=0 ;sup_ftbl[i] != NULL;i++) {
		err = sup_ftbl[i]();
		if(err)		return -1;
	}
	return 0;
}
