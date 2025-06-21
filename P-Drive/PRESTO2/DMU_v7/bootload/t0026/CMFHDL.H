/*======================================================================
 * File name    : cmfhdl.h
 * Original		: p:/dacs_i/rcu/main/rcs/cmfhdl.h 1.1 CON0,1に対応
 *----------------------------------------------------------------------
 * Function 	:
 *		通信デバイスファイルアクセスモジュール
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/cmfhdl.h 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cmfhdl.h $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/02/19 21:30
 * DIPSW2の8番がONの場合は、con0からのシリアル出力を行わない
 * ようにするため、定義DIPSW_8_MASKを追加する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * FIFOの追加に対応する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef		_CMFHDL_H_
#define		_CMFHDL_H_

#define DIPSW_8_MASK	(0x80)	/* DIPSW2：8  ONの場合シリアル出力を行わない*/

/*--------------------------------------------------------------*/
/*	デバイスを選択する場合の列挙型定数							*/
/*																*/
/* 注:	入出力を行うデバイスの変更はenumのデバイス名の順序		*/
/*	を変更する。												*/
/*--------------------------------------------------------------*/

enum {
	CON0_DN,			/* 1: CON0 = "con0" SH7615 SCIF CH1	*/
	CON1_DN,			/* 2: CON1 = "con1" SH7615 SCIF	CH2	*/
	TTY0_DN,			/* 3: TTY0 = "tty0" MPSC#0 CH-A	*/
	TTY1_DN,			/* 4: TTY1 = "tty1" MPSC#0 CH-B */
	TTY2_DN,			/* 5: TTY2 = "tty2" MPSC#1 CH-A	*/
	TTY3_DN,			/* 6: TTY3 = "tty3" MPSC#1 CH-B */
	LAN_DN,				/* 7: LAN  = "lan"  ARCNET	*/
	FIF_DN,				/* 8: FIFO = "com0"  FIFO	*/
	COM_DN_MAX,
};

/*--------------------------------------------------------------*/
/*	ファイル･ハンドルを選択する場合の列挙型定数					*/
/*																*/
/*--------------------------------------------------------------*/

enum {
	CON0_FH,			/* CON0 SCIF channel	*/
	CON1_FH,			/* CON1 SCIF channel	*/
	TTY0_FH,			/* TTY0 channel			*/
	TTY1_FH,			/* TTY1 channel			*/
	TTY2_FH,			/* TTY2 channel			*/
	TTY3_FH,			/* TTY3 channel			*/
	LAN_FH,				/* LAN channel			*/
	FIF_FH,				/* FIFO channel			*/
	COM_FH_MAX,
};

/*----------------------------------------------------------------------*/
/*	<<< return value >>>												*/
enum comrv{
	RCV_ERROR	= -2,			/* 受信ｴﾗｰ			*/
	COM_FAIL,					/* 失敗				*/
	COM_COMPL					/* 成功				*/
};
typedef enum comrv	COMRV;

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int port_tbl_init(void);

COMRV con0_gets(unsigned char *buf, int max);
COMRV con0_puts(unsigned char *buf, int len);
COMRV con1_gets(unsigned char *buf, int max);
COMRV con1_puts(unsigned char *buf, int len);
COMRV tty0_gets(unsigned char *buf, int max);
COMRV tty0_puts(unsigned char *buf, int len);
COMRV tty1_gets(unsigned char *buf, int max);
COMRV tty1_puts(unsigned char *buf, int len);
COMRV tty2_gets(unsigned char *buf, int max);
COMRV tty2_puts(unsigned char *buf, int len);
COMRV tty3_gets(unsigned char *buf, int max);
COMRV tty3_puts(unsigned char *buf, int len);
COMRV lan_gets(unsigned char *buf, int max);
COMRV lan_puts(unsigned char *buf, int len);
COMRV fif_gets(unsigned char *buf, int max);
COMRV fif_puts(unsigned char *buf, int len);

#endif
