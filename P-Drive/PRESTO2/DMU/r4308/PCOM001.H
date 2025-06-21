/*======================================================================
 * File name    : pcom001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/pcom001.h 1.4 1970/01/01 00:00:00 ryoji Exp $
 *$Log: pcom001.h $
 * リビジョン 1.4  1970/01/01  00:00:00  ryoji
 * 2007/03/10 05:00:00
 * 予約３桁仕様でも使用できるよう 予約番号の桁数をディップスイッチ(SW3-4)で
 * 可変できるようにする。桁数は、OFFの場合 2 で、ONの場合 3 となる。
 * 
 * リビジョン 1.3  1998/09/01  14:42:16  yonetsu
 * int tx_pre_rcu(int c1c2,int cmd,int pre_no) のプロトタイプを追加
 * 
 * リビジョン 1.2  1998/08/27  11:43:14  yonetsu
 * プロトタイプ struct mpspk *aux_baud_chg(int dev_no,struct mpspk *p) を
 * 追加
 * 
 * リビジョン 1.1  1998/08/26  18:29:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-1998, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#ifndef	_PCOM001_H_
#define	_PCOM001_H_

#define	PRECOM_EOT	0x04
#define	PRECOM_ENQ	0x05
#define	PRECOM_STX	0x02
#define	PRECOM_ETX	0x03
#define	PRECOM_ACK	0x06
#define	PRECOM_NAK	0x15

/*----- メールメッセージ -----*/
struct pcom_msg {
	int	priority;	/* 優先順位	*/
	int	cmd;		/* command	*/
	int	pre_no;		/* preset no	*/
};


/* ----- 予約通信のパケット構造 RCU-NHIC -----*/
struct prechg_txt2_rcu {
	unsigned char	pre_no[2];
	unsigned char	adrs[2];
	unsigned char	term;
};

struct prechg_txt3_rcu {
	unsigned char	pre_no[3];
	unsigned char	adrs[2];
	unsigned char	term;
};

int tx_pre_mail(int no,struct pcom_msg *pre);
void rcu_to_precom(int dev_no,int len,const unsigned char *buf,
		   const unsigned char *non_accpt_cmd,
		   const unsigned char *accpt_cmd );

void precom_to_rcu(int dev_no,int len,const unsigned char *buf,
		   const unsigned char *non_accpt_cmd,
		   const unsigned char *accpt_cmd );
void precom_setup(void);
int precom_tsk_setup(int aux_no);
struct mpspk *aux_baud_chg(int dev_no,struct mpspk *p);
struct scipk *con_baud_chg(int dev_no,struct scipk *p);
int tx_pre_mail(int no,struct pcom_msg *pre);
int tx_pre_rcu(int c1c2,int cmd,int pre_no);

#endif
