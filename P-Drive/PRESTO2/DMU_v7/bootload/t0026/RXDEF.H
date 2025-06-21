/*======================================================================
 * File name    : rxdef.h
 * Original		: p:/dacs_i/rcu/main/rcs/rxdef.h 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		NORT-i用ユーザ定義のシンボル等の定義
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/rxdef.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rxdef.h $
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
#ifndef		_RXDEF_H_
#define		_RXDEF_H_

#ifdef	acadr					/* ｱｸｾｽ･ｱﾄﾞﾚｽが定義されている?	*/
#ifdef	STORAGE
union aap	apl_ds = {			/* APLで使用するdata segment	*/
			0L					/* 初期値を持たせることが大事!	*/
		};
#else
extern const union aap	apl_ds;
#endif
#endif

	/*==========================================================
						ビットパターン定義
	==========================================================*/
#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BITALL	0xffff

/*--------------------------------------------------------------*/
/*		タスクIDの定義											*/
/*																*/
/*--------------------------------------------------------------*/

enum {
	APL_TSK_ID	= 60,				/* APLは60番以降を採番する	*/
/*-------------------------------------*/
	CON0_RCV_TSK_ID,
	CON0_SND_TSK_ID,
	CON1_RCV_TSK_ID,
	CON1_SND_TSK_ID,
	TTY0_RCV_TSK_ID,
	TTY0_SND_TSK_ID,
	TTY1_RCV_TSK_ID,
	TTY1_SND_TSK_ID,
	TTY2_RCV_TSK_ID,
	TTY2_SND_TSK_ID,
	TTY3_RCV_TSK_ID,
	TTY3_SND_TSK_ID,
	LAN_RCV_TSK_ID,
	LAN_SND_TSK_ID,
	KEY_RCV_TSK_ID,
	BKFWR_TSK_ID,
	KEYSCAN_TSK_ID,
	SOFTTIMER_TSK_ID,
	CYC25_TSK_ID,
	CYC100_TSK_ID,
	PRNOUT_TSK_ID,
	ETHER_TSK_ID,
	SOCKET_TSK_ID,
	FTP_TSK_ID,
	CF_TSK_ID,
	FIFRCV_TSK_ID
/*-------------------------------------*/
};

/*--------------------------------------------------------------*/
/*		タスク･プライオリティの定義								*/
/*																*/
/* 注:	ﾌﾟﾗｲｵﾘﾃｨはenumのトップが最も高く、以下記述順に			*/
/*		低くなる。												*/
/*	    enum {													*/
/*		TSK1_PRI,	･･･ APLの最高位								*/
/*		TSK2_PRI,		:										*/
/*		TSKn_PRI	･･･ APLの最低位								*/
/*	    }														*/
/*--------------------------------------------------------------*/

enum {
	APL_TSK_PRI	= 25-1,		/* APLは25番以降を採番する	*/
/*-------------------------------------*/
	SOFTTIMER_TSK_PRI= 2,
	KEYSCAN_TSK_PRI  = 6,
	KEY_RCV_TSK_PRI  = 6,
	CON0_RCV_TSK_PRI = 3,	/* melsec */
	CON0_SND_TSK_PRI = 10,
	CON1_RCV_TSK_PRI = 4,	/* pcu */
	CON1_SND_TSK_PRI = 10,
	TTY0_RCV_TSK_PRI = 6,	/* g_kcode */
	TTY0_SND_TSK_PRI = 10,
	TTY1_RCV_TSK_PRI = 10,	/* pcu */
	TTY1_SND_TSK_PRI = 10,
	TTY2_RCV_TSK_PRI = 5,	/* mds */
	TTY2_SND_TSK_PRI = 10,
	TTY3_RCV_TSK_PRI = 10,	/* melsec */
	TTY3_SND_TSK_PRI = 10,
	LAN_RCV_TSK_PRI  = 10,		/* hos */
	LAN_SND_TSK_PRI  = 10,
	BKFWR_TSK_PRI    = 10,
	CYC25_TSK_PRI    = 5,
	CYC100_TSK_PRI   = 2,
	PRNOUT_TSK_PRI   = 10,
	ETHERNET_TSK_PRI = 3,	/* melsec ether*/
	SOCKET_TSK_PRI   = 3,	/* melsec ether*/
	FTP_TSK_PRI=4,	/* ftp init task */
	CF_TSK_PRI=10,	/* detect CF insert */
	FIFRCV_TSK_PRI   = 2	/* melsec */
/*-------------------------------------*/
};

/*------------------------------------------------------*/
/*		イベント･フラグIDの定義							*/
/*														*/
/*------------------------------------------------------*/
enum{
	E_FLG_CTRL = 1,					/* 汎用制御用イベント			*/
	E_FLG_SCI,						/* 未使用			*/
	FLG_MAX
};


/*------------------------------------------------------*/
/*		メールボックスIDの定義							*/
/*														*/
/*------------------------------------------------------*/

enum {
	APL_MBX_ID	= 1,       		/* ｱﾌﾟﾘは1番以降を採番する	*/
/*-------------------------------------*/

/*-------------------------------------*/
	DRV_MBX_ID	= 10       	/* ﾄﾞﾗｲﾊﾞｰは10番以降を採番する	*/
};

/*------------------------------------------------------*/
/*		セマフォIDの定義								*/
/*														*/
/*------------------------------------------------------*/

enum  {
	APL_SEM_ID	= 1,       		/* ｱﾌﾟﾘは1番以降を採番する	*/
/*-------------------------------------*/
/*
//	RES_SEM_ID から RES_END_SEM_ID までは、
//	専用ﾓｼﾞｭｰﾙを使用して一元管理を行う為、予約する。
*/
	RES_SEM_ID,
	FIP_SEM_ID = RES_SEM_ID,
	RCD_SEM_ID,
	RES_END_SEM_ID,
	PRN_SEM_ID,
	FILE_SEM_ID,
/*-------------------------------------*/

/*-------------------------------------*/
	DRV_SEM_ID	= 10			/* ﾄﾞﾗｲﾊﾞｰは10番以降を採番する	*/
};

/*------------------------------------------------------*/
/*		フラグIDの定義									*/
/*														*/
/*------------------------------------------------------*/


/*------------------------------------------------------*/
/*		メッセージ送受バッファサイズの定義と			*/
/*		各タスクのユーザスタックサイズ					*/
/*------------------------------------------------------*/

#define RCU_MES_MAX			(512+8)
#define FIF_MES_MAX			(2048+8)

#ifdef	DEBUG
#define STK_SIZE			(512+512)
#else
#define STK_SIZE			(512)
#endif

#define  TTY0_RCV_TSK_STK              (    4096) + STK_SIZE
#define  TTY0_SND_TSK_STK              (    4096) + STK_SIZE
#define  TTY1_RCV_TSK_STK              (    4096) + STK_SIZE
#define  TTY1_SND_TSK_STK              (    4096) + STK_SIZE
#define  TTY2_RCV_TSK_STK              (    4096) + STK_SIZE
#define  TTY2_SND_TSK_STK              (    4096) + STK_SIZE
#define  TTY3_RCV_TSK_STK              (    4096) + STK_SIZE
#define  TTY3_SND_TSK_STK              (    4096) + STK_SIZE
#define  CON0_RCV_TSK_STK              (    4096) + STK_SIZE
#define  CON0_SND_TSK_STK              (    4096) + STK_SIZE
#define  CON1_RCV_TSK_STK              (    4096) + STK_SIZE
#define  CON1_SND_TSK_STK              (    4096) + STK_SIZE
#define  LAN_RCV_TSK_STK               (    4096) + STK_SIZE
#define  LAN_SND_TSK_STK               (    4096) + STK_SIZE
#define  BKF_WRITE_TSK_STK             (    4096) + STK_SIZE
#define  KEYSCAN_TSK_STK           	   (    4096) + STK_SIZE
#define  KEY_RCV_TSK_STK           	   (    4096) + STK_SIZE
#define  SOFTTIMER_TSK_STK         	   (    4096) + STK_SIZE
#define  CYC25_TSK_STK	         	   (    4096) + STK_SIZE
#define  CYC100_TSK_STK	         	   (    4096) + STK_SIZE
#define  PRNOUT_TSK_STK	         	   (    4096) + STK_SIZE
#define  FIF_RCV_TSK_STK               (    4096) + STK_SIZE

#endif
