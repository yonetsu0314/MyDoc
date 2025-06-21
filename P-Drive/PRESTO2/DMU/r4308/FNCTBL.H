/*======================================================================
 * File name    : fnctbl.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/mhic/main/ccw/rcs/fnctbl.h 1.2.4308.3 1970/01/01 00:00:00 ryoji Exp $
 *$Log: fnctbl.h $
 * リビジョン 1.2.4308.3  1970/01/01  00:00:00  ryoji
 * 2007/03/10 05:00:00
 * 予約３桁仕様でも使用できるよう 予約番号の桁数をディップスイッチ(SW3-4)で
 * 可変できるようにする。桁数は、OFFの場合 2 で、ONの場合 3 となる。
 * 
 * リビジョン 1.2.4308.2  1970/01/01  00:00:00  yonetsu
 * 2000/02/07 09:51
 * RCUからのロム番号要求に応答する処理の追加。
 * 
 * リビジョン 1.2.4308.1  1998/09/01  14:48:46  yonetsu
 * 予約交信仕様
 * 
 * リビジョン 1.2  1998/05/15  14:06:19  yonetsu
 * sup_ftbl を追加し、立ち上げ時に常駐タスクを起動できるようにする。
 * 
 * リビジョン 1.1  1997/05/28  13:37:09  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_FNCTBL_H_
#define	_FNCTBL_H_

/* 
	ここに下記テ－ブル内で呼ばれる関数のプロトタイプが含まれている
	ヘッダーファイルをインクルードすること。
*/
#include	"hif001.h"
#include	"pcom001.h"
#include	"romno.h"

#ifdef _FNCTBL_C_	/* この宣言は重要 */

#include	<rzcmd/rzcmd.h>
#include 	"pktdef.h"
#include	"pktchg.h"

enum { BIN_DT=0 , TXT_DT };

struct tx_ftbl {
	int id_no;				/* tx function id */
	int dev_no;
};


/* どのテキストを、どのポートから送信するかを定義する */

static struct tx_ftbl  txtbl[] = {
    { TX_EXT_C1_PRE	, CON_DN0 },
    { TX_EXT_C2_PRE	, CON_DN1 },
    { TX_RCU_NORMAL	, RCU_DN  },
    { TX_RCU_PRE	, RCU_DN  },
    { TX_RCU_DDE_HOST	, -1	  },
    { TX_RCU_STD_HOST	, -1	  },
    { TX_EXT_C1C2_PRE	, -1	  },
    { TX_EXT_DDE_HOST	, -1	  },
    { TX_EXT_STD_HOST	, -1	  },
    { TX_EXT_ONEWAY	, -1	  },
    { -1 }					/* end of table */
};

struct func_assign {
	int   dev_no;				      	  /* 論理デバイス番号 				*/
	int   data_kind;			      	  /* 送受信データの種類 binary or text  	*/
	COMRV (*sumf)(unsigned char *,int ,char *);	  /* sum function 				*/
	void  (*apl_rx_func)(int,int,const unsigned char *,
				 const unsigned char *,
				 const unsigned char *);  /* 受信時に呼ばれる関数			*/
	unsigned char *non_accpt_cmd;			  /* 定義された受信関数で受信拒否するコマンド列	*/
	unsigned char *accpt_cmd;			  /* 定義された受信関数で受信可能なコマンド列	*/
	void  (*rx_err_func)(int,int); 			  /* 受信エラー時に呼ばれる関数			*/
	void  *pkt;			     	     	  /* 通信仕様			   		*/
	void  *(*mod_pkt)(int,void *);	     	      	  /* 通信仕様に変更がある場合に呼ばれる関数 	*/
};

static struct func_assign  ftbl[] = {
 { /* con0_rcv_tsk */
   	CON_DN0  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	NULL , 				/* sum function 					   */
	precom_to_rcu , 		/* 受信時に呼ばれる関数					   */
	NULL,				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL,				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL , 				/* 受信エラー時に呼ばれる関数				   */
	(void *)&con0pkt ,		/* 通信仕様   						   */
	con_baud_chg 			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 },
 { /* con1_rcv_tsk */
   	CON_DN1  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	NULL , 				/* sum function 					   */
	precom_to_rcu , 		/* 受信時に呼ばれる関数					   */
	NULL,				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL,				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL , 				/* 受信エラー時に呼ばれる関数				   */
	(void *)&con1pkt ,		/* 通信仕様   						   */
	con_baud_chg 			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 },
 { /* hst_rcv_tsk */
   	HST_DN  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	cpl_hex_ascii_sum , 		/* sum function    					   */
	NULL , 				/* 受信時に呼ばれる関数   				   */
	NULL ,				/* 定義された受信関数で受信拒否するコマンド列   	   */
	NULL ,			 	/* 定義された受信関数で受信可能なコマンド列   		   */
	NULL ,			 	/* 受信エラー時に呼ばれる関数   			   */
	(void *)&hstpkt , 		/* 通信仕様     					   */
	NULL 				/* 通信仕様に変更がある場合に呼ばれる関数   		   */
 },
 { /* hic_rcv_tsk */
	HIC_DN  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	cpl_hex_ascii_sum , 		/* sum function 					   */
	NULL , 				/* 受信時に呼ばれる関数					   */
	NULL , 				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL ,				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL ,				/* 受信エラー時に呼ばれる関数				   */
	(void *)&hicpkt , 		/* 通信仕様   						   */
	NULL 				/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 },
 { /* rcu_rcv_tsk */	/* TAG ありコマンドを処理する */
	RCU_DN  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	cpl_hex_ascii_sum , 		/* sum function 					   */
	rcu_to_precom , 		/* 受信時に呼ばれる関数					   */
	NULL , 				/* 定義された受信関数で受信拒否するコマンド列		   */
	"01" , 				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL , 				/* 受信エラー時に呼ばれる関数				   */
	(void *)&rcupkt , 		/* 通信仕様   						   */
	rcu_pkt_chg 			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 }, 
 { /* rcu_rcv_tsk */	/* TAG なしコマンドを処理する */
	RCU_DN  , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	cpl_hex_ascii_sum , 		/* sum function 					   */
	rom_no_req ,	 		/* 受信時に呼ばれる関数					   */
	NULL , 				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL , 				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL , 				/* 受信エラー時に呼ばれる関数				   */
	(void *)&rcupkt , 		/* 通信仕様   						   */
	rcu_pkt_chg			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 }, 
 { /* aux0_rcv_tsk */
	AUX0_DN , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	NULL , 				/* sum function 					   */
	precom_to_rcu , 		/* 受信時に呼ばれる関数					   */
	NULL , 				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL , 				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL ,	 			/* 受信エラー時に呼ばれる関数				   */
	(void *)&aux0pkt , 		/* 通信仕様   						   */
	aux_baud_chg			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 },
 { /* aux1_rcv_tsk */
	AUX1_DN , TXT_DT , 		/* 論理デバイス番号 *//* 送受信データの種類 binary or text */
	NULL , 				/* sum function 					   */
	precom_to_rcu , 		/* 受信時に呼ばれる関数					   */
	NULL , 				/* 定義された受信関数で受信拒否するコマンド列		   */
	NULL , 				/* 定義された受信関数で受信可能なコマンド列		   */
	NULL , 				/* 受信エラー時に呼ばれる関数				   */
	(void *)&aux1pkt , 		/* 通信仕様   						   */
	aux_baud_chg			/* 通信仕様に変更がある場合に呼ばれる関数 		   */
 }, 

/*-----------------------------------*/

 {  -1 	}  /* end of table */
};

/* イニシャルタスクでセットアップしたいタスクを書く */

typedef int ((*PFI)(void));	/* 関数のポインター */

static PFI  sup_ftbl[] = {
	precom_setup,		/* preset com	*/
/*-----------------------------------*/
	NULL
};

#endif	/* #ifdef _FNCTBL_C_  */

void rx_func(int dev_no,int len,const unsigned char *buf);
void rx_err_func(int dev_no,int err_stat);
int is_accpt_cmd(unsigned char cmd,const unsigned char *non_accpt_cmd,const unsigned char *accpt_cmd);
COMRV tx_func(int mes_id,int len,unsigned char *buf);
COMRV sum_func(int dev_no,unsigned char *buf,int len,char *txrx);
COMRV rx_dev_func(int dev_no,unsigned char *buf,int max);
COMRV tx_dev_func(int dev_no,unsigned char *buf,int len);
int is_device_exist(int dev_no);
void *get_pkt_spec(int dev_no);
int is_arcnet(int no);
int mesid_to_devno(int mes_id);
int devno_comp_mesid(int dev_no,int mes_id);
int setup_etc(void);

#endif

