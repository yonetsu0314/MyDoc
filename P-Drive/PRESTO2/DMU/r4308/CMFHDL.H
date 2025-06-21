/*======================================================================
 * File name    : cmfhdl.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	通信デバイスファイルアクセスモジュール
 *	変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/cmfhdl.h 1.1.4102.2 1998/09/01 14:39:20 yonetsu Exp $
 *$Log: cmfhdl.h $
 * リビジョン 1.1.4102.2  1998/09/01  14:39:20  yonetsu
 * int aux_wup_char_set(int dev_no,unsigned char far *wupc ) のプロトタイプを
 * 追加。この関数は、dmps4.lib をリンクした時使用可能。受信タスクを起床させる
 * キャラクターを変更することが出来る。
 * 
 * リビジョン 1.1.4102.1  1997/05/28  13:31:52  yonetsu
 * プロトタイプの追加
 * 
 * リビジョン 1.1  1996/05/31  16:31:26  TGL
 * *** 空ログメッセージ ***
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		CMFHDL_H
#define		CMFHDL_H


/*--------------------------------------------------------------*/
/*	デバイスを選択する場合の列挙型定数							*/
/*--------------------------------------------------------------*/

enum {
	CON_DN0,			/* 0:  STD I/O  = "con"			*/
	CON_DN1,			/* 1:  STD I/O  = "con"			*/
	HST_DN,				/* 2:  HST      = "lan0"		*/
	HIC_DN,				/* 3:  HIC      = "lan1"		*/
	RCU_DN,				/* 4:  RCU      = "lan0"		*/
	ICU_DN,				/* 5:  ICU      = "lan1"		*/
	EXT_DN,				/* 6:  EXT      = "dpmem"		*/
	AUX0_DN,			/* 7:  AUX0     = "tty0"		*/
	AUX1_DN,			/* 8:  AUX1     = "tty1"		*/
	AUX2_DN,			/* 9:  AUX2     = "tty2"		*/
	AUX3_DN,			/* 10: AUX3     = "tty3"		*/
	AUX4_DN,			/* 11: AUX4     = "tty4"		*/
	AUX5_DN,			/* 12: AUX5     = "tty5"		*/
	TIM_DN,
	COM_DN_MAX
};

enum msgnn{
	MSG_EXC,				/* 例外処理用		*/
	INTER_NN,				/* node 1:default	*/
	MSG_MAX,
};

#ifdef	STORAGE
union aap	rxtsk[COM_DN_MAX];	/* 受信タスクのアクセスアドレス	*/
union aap	msgtsk[MSG_MAX];
#else
extern union aap	rxtsk[];
extern union aap	msgtsk[];
#endif

/*--------------------------------------------------------------*/
/*	受信タスクの受信バッファサイズ				*/
/*--------------------------------------------------------------*/

#define		ICU_MES_MAX	(256)
#define		HIC_MES_MAX	(256)
#define		RCU_MES_MAX	(512)
#define		HST_MES_MAX	(512)
#define		AUX_MES_MAX	(256)

/*--------------------------------------------------------------*/
/*	ファイル･ハンドルを選択する場合の列挙型定数		*/
/*								*/
/*--------------------------------------------------------------*/

enum {
	CON_FH0,			/* CON channel			*/
	CON_FH1,			/* CON channel			*/
	HST_FH,				/* ICU channel			*/
	HIC_FH,				/* HIC channel			*/
	RCU_FH,				/* RCU channel			*/
	ICU_FH,				/* HST channel			*/
	EXT_FH,				/* EXT channel			*/
	AUX_FH0,			/* AUX0 channel			*/
	AUX_FH1,			/* AUX1 channel			*/
	AUX_FH2,			/* AUX2 channel			*/
	AUX_FH3,			/* AUX3 channel			*/
	AUX_FH4,			/* AUX4 channel			*/
	AUX_FH5,			/* AUX5 channel			*/
	COM_FH_MAX,
};

/*----------------------------------------------------------------------*/
/*	<<< return value >>> */
enum comrv{
	COM_RETRY 	= -4,		/* リトライ			*/
	COM_ENODE,			/* ノード無し			*/
	RCV_ERROR,			/* 受信エラー			*/
	COM_DOWN	= RCV_ERROR,	/* 回線断			*/
	COM_FAIL,			/* 失敗				*/
	COM_COMPL			/* 成功				*/
};
typedef enum comrv	COMRV;

int port_tbl_init(void);
int aux_wup_char_set(int dev_no,unsigned char far *wupc ); /* dmps4.lib */
int con0_text_check(char *text, int len);
int con1_text_check(char *text, int len);
COMRV con0_gets(unsigned char *buf, int max);
COMRV con1_gets(unsigned char *buf, int max);
COMRV rcu_gets(unsigned char *buf, int max);
COMRV hst_gets(unsigned char *buf, int max);
COMRV icu_gets(unsigned char *buf, int max);
COMRV hic_gets(unsigned char *buf, int max);
COMRV aux0_gets(unsigned char *buf,int max);
COMRV aux1_gets(unsigned char *buf,int max);
COMRV aux2_gets(unsigned char *buf,int max);
COMRV aux3_gets(unsigned char *buf,int max);
COMRV aux4_gets(unsigned char *buf,int max);
COMRV aux5_gets(unsigned char *buf,int max);

COMRV con0_puts(unsigned char *buf, int len);
COMRV con1_puts(unsigned char *buf, int len);
COMRV rcu_puts(unsigned char *buf, int len);
COMRV hst_puts(unsigned char *buf, int len);
COMRV icu_puts(unsigned char *buf, int len);
COMRV hic_puts(unsigned char *buf, int len);
COMRV aux0_puts(unsigned char *buf,int len);
COMRV aux1_puts(unsigned char *buf,int len);
COMRV aux2_puts(unsigned char *buf,int len);
COMRV aux3_puts(unsigned char *buf,int len);
COMRV aux4_puts(unsigned char *buf,int len);
COMRV aux5_puts(unsigned char *buf,int len);

COMRV con0_read(unsigned char *buf, int max);
COMRV con1_read(unsigned char *buf, int max);
COMRV rcu_read(unsigned char *buf, int max);
COMRV hst_read(unsigned char *buf, int max);
COMRV icu_read(unsigned char *buf, int max);
COMRV hic_read(unsigned char *buf, int max);
COMRV aux0_read(unsigned char *buf,int max);
COMRV aux1_read(unsigned char *buf,int max);
COMRV aux2_read(unsigned char *buf,int max);
COMRV aux3_read(unsigned char *buf,int max);
COMRV aux4_read(unsigned char *buf,int max);
COMRV aux5_read(unsigned char *buf,int max);

COMRV con0_write(unsigned char *buf, int len);
COMRV con1_write(unsigned char *buf, int len);
COMRV rcu_write(unsigned char *buf, int len);
COMRV hst_write(unsigned char *buf, int len);
COMRV icu_write(unsigned char *buf, int len);
COMRV hic_write(unsigned char *buf, int len);
COMRV aux0_write(unsigned char *buf,int	len);
COMRV aux1_write(unsigned char *buf,int	len);
COMRV aux2_write(unsigned char *buf,int	len);
COMRV aux3_write(unsigned char *buf,int	len);
COMRV aux4_write(unsigned char *buf,int	len);
COMRV aux5_write(unsigned char *buf,int	len);

#endif
