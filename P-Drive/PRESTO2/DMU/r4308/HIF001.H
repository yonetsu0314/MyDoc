/*======================================================================
 * File name    : hif001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: HIF BOX との通信タスクのプロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/hif001.h 1.1 1997/05/28 15:33:02 yonetsu Exp $
 *$Log: hif001.h $
 * リビジョン 1.1  1997/05/28  15:33:02  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1996, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		HIF001_H
#define		HIF001_H

#include "cmfhdl.h"	/* for HST_MES_MAX */


/*----------------------------------------------------------
  標準のホスト連動で、ＨＩＦとの通信する時のテキスト
  フォーマットの定義
------------------------------------------------------------*/

enum ctrl {
	CONT_INFO =1 ,
	CONT_ACK ,
	CONT_NAK ,
	CONT_ENQ
};

struct hif_async_net_header {	/* HIF BOX - NHIC(MHIC) の通信テキスト構造 */
	unsigned char dst_adrs;
	unsigned char dst_port;
	unsigned char src_adrs;
	unsigned char src_port;
	unsigned char control;
	unsigned char cmd;
	unsigned char param;
};

union hif_async_net_text {
	struct hif_async_net_header hdr;
	unsigned char buf[HST_MES_MAX];
};

/*----------------------------------------------------------
  ホスト連動をアークネットで行う場合のテキストフォーマット
------------------------------------------------------------*/

#define	HIF_ARC_MASTER_ADRS_DEFAULT (255)

struct hif_arcnet_header {	/* PC - NHIC(MHIC) の通信テキスト構造 	*/
	unsigned char	src;		/* ソースアドレス		*/
	unsigned char	dst;		/* ディスティネーションアドレス	*/
/*-- 上記は、内部処理用で、実際のテキストには現れない --*/
	unsigned char	c1c2;
	unsigned char	cmd;
	unsigned char	param;
};

union hif_arc_text {
	struct hif_arcnet_header hdr;
	unsigned char buf[HST_MES_MAX];
};

/*----------------------------------------------------------
  ＲＣＵと通信が、 arcnet の時の通信フォーマットの定義
------------------------------------------------------------*/

struct arc_rcu_pkt {	/* RCU - NHIC(MHIC) アークネットの通信のテキスト構造 	*/
	unsigned char	src;			/* ソースアドレス		*/
	unsigned char	dst;			/* ディスティネーションアドレス	*/
/*-- 上記は、内部処理用で、実際のテキストには現れない --*/
	unsigned char	src_memo[4];		/* ソースネーム			*/
	unsigned char	dst_memo[4];		/* ディスティネーションネーム	*/
	unsigned char	tag;			/* タグ				*/
	unsigned char	cmd;			/* コマンド			*/
	unsigned char	ext_ptr[4];		/* 拡張ポインター		*/
	unsigned char	param;			/* パラメータの先頭		*/
};

/*----------------------------------------------------------
  メールボックスで通信するための、メッセージ内容
------------------------------------------------------------*/

struct hif_txt_msg {
	int			 priority;	/* 優先順位		*/
	int			 try_cnt;	/* ENQ 送信回数		*/
	long			 try_tmr;	/* ENQ 送信間隔		*/
	int			 len;		/* テキスト長		*/
	int			 c1c2;		/* C1 or C2		*/
	union hif_async_net_text text;		/* テキスト		*/
};

struct hif_stat_msg {
	int		priority;	/* 優先順位		*/
	int		stat;		/* ENQ,ACK,NAK		*/
	int		c1c2;		/* C1 or C2		*/
};

int hif_acync_nettx_setup(void);
void rcu_to_hif(int dev_no,int len,const unsigned char *buf,
		const unsigned char *non_accpt_cmd,
		const unsigned char *accpt_cmd		);
void send_mail_hif_txt_msg(int c1c2,struct hif_txt_msg *msg);
void hif_to_rcu(int dev_no,int len,const unsigned char *buf,
		const unsigned char *non_accpt_cmd,
		const unsigned char *accpt_cmd		);
void hif_rx_err(int dev_no,int err_stat);
int hif_async_nettx_setup(void);
void send_last_rx_stat(int my_adrs,int stat);

#endif
