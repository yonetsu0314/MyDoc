/*======================================================================
 * File name    : rxdef.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	RX116-SVC用ユーザ定義のシンボル等の定義
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/nz_wcu/wcu/rcs/rxdef.h 1.2.6308.4.6510.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: rxdef.h $
 * Revision 1.2.6308.4.6510.2  1970/01/01  00:00:00  chimura
 * 2003/09/13 09:00
 * ADCモニタプログラム用のイベントフラグIDを０から１に変更する。
 * NORTIでは、ID番号に０を認めないため。アプリに影響はない。
 * 
 * リビジョン 1.2.6308.4.6510.1  1970/01/01  00:00:00  chimura
 * 2003/07/24 13:30
 * PWM制御フィーダ仕様追加
 * 
 * リビジョン 1.2.6308.4  1970/01/01  00:00:00  kawamura
 * 2003/06/25  13:04
 * ペア機の仕様を追加する。
 * 
 * リビジョン 1.2.6308.3  1970/01/01  00:00:00  kawamura
 * 2003/05/07  14:33
 * 同時に動作しないタスクは同じタスクＩＤ番号を使用する。
 * 
 * リビジョン 1.2.6308.2  1970/01/01  00:00:00  chimura
 * 2003/04/15 12:15
 * タスクのID最大値を255から250に変更する。
 * タスクのスタックサイズを変更する。
 * 
 * リビジョン 1.2.6308.1  1970/01/01  00:00:00  chimura
 * 2003/02/04 19:00
 * タスク、フラグ、セマフォのIDを変更する。NORTi用暫定処理。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2000/04/14 14:15
 * タスクのスタックサイズを見直す。
 * 
 * リビジョン 1.1  1999/07/08  14:49:08  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/wcu/main/rz_wcu2/rcs/rxdef.h 2.2.4369.3 1999/04/30 15:19:30 kawamura Exp
 *log: rxdef.h
 * リビジョン 2.2.4369.3  1999/04/30  15:19:30  kawamura
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#ifndef		RXDEF_H
#define		RXDEF_H

#ifdef	acadr				/* ｱｸｾｽ･ｱﾄﾞﾚｽが定義されている?	*/
#ifdef	STORAGE
union aap	apl_ds = {		/* APLで使用するdata segment	*/
			0L		/* 初期値を持たせることが大事!	*/
		};
#else
extern const union aap	apl_ds;
#endif
#endif

/*--------------------------------------------------------------*/
/*	タスクIDの定義						*/
/*								*/
/*--------------------------------------------------------------*/

enum {
	APL_TSK_ID	= 180,		/* APLは180番以降を採番する	*/
/*-------------------------------------*/
/*  重複しないタスクのIDは同じにする   */
	DRAIN_TSK_ID0,
	DRAIN_TSK_ID1,
	DUMP_TSK_ID0	= DRAIN_TSK_ID0,
	DUMP_TSK_ID1	= DRAIN_TSK_ID1,
	MANCALC_TSK_ID0	= DRAIN_TSK_ID0,
	MANCALC_TSK_ID1	= DRAIN_TSK_ID1,
	PRODUCT_TSK_ID0	= DRAIN_TSK_ID0,
	PRODUCT_TSK_ID1	= DRAIN_TSK_ID1,
	SPAN_TSK_ID0	= DRAIN_TSK_ID0,
	SPAN_TSK_ID1	= DRAIN_TSK_ID1,
	TSTDRV_TSK_ID0	= DRAIN_TSK_ID0,
	TSTDRV_TSK_ID1	= DRAIN_TSK_ID1,
	ZERO_TSK_ID0	= DRAIN_TSK_ID0,
	ZERO_TSK_ID1	= DRAIN_TSK_ID1,
/*-------------------------------------*/
	ADCDIAG_TSK_ID,
	ADCINIT_TSK_ID,
	AFVRECALL_TSK_ID,
	ARCHIVE_TSK_ID,
	AT_ZERO_TSK_ID0,
	AT_ZERO_TSK_ID1,
	BKFWR_TSK_ID,
	CONRCV_TSK_ID,
	CONSND_TSK_ID,
	DUCDIAG_TSK_ID,
	DUCERR_TSK_ID0,
	DUCERR_TSK_ID1,
	LANRCV_TSK_ID,
	DUCRCV_TSK_ID0,
	DUCRCV_TSK_ID1,
	DUCSND_TSK_ID,
	ILKRCV_TSK_ID,
	INFEED_TSK_ID,
	LANMAP_TSK_ID0,
	LANMAP_TSK_ID1,
	LANMAP_TSK_ID2,
	LANMAP_TSK_ID3,
	LANMAP_TSK_ID4,
	LANMAP_TSK_ID5,
	LANMAP_TSK_ID6,
	LANMAP_TSK_ID7,
	RCURCV_TSK_ID,
	RCUSND_TSK_ID,
	RS485MAP_TSK_ID,
	SND_TSK_ID0,
	SND_TSK_ID1,
	SND_TSK_ID2,
	SND_TSK_ID3,
	SHIFT_TSK_ID0,
	SHIFT_TSK_ID1,
	RES_DETECT_TSK_ID,
/*-------------------------------------*/
	MONRCV_TSK_ID	= 245,
	MONSND_TSK_ID,
	MONAFV_TSK_ID,
	MONSTB_TSK_ID,
};


/*--------------------------------------------------------------*/
/*	タスク･プライオリティの定義				*/
/*								*/
/* 注:	ﾌﾟﾗｲｵﾘﾃｨはenumのトップが最も高く、以下記述順に		*/
/*	低くなる。						*/
/*	    enum {						*/
/*		TSK1_PRI,	･･･ APLの最高位			*/
/*		TSK2_PRI,		:			*/
/*		TSKn_PRI	･･･ APLの最低位			*/
/*	    }							*/
/*--------------------------------------------------------------*/

enum {
	APL_TSK_PRI	= 10-1,		/* APLは10番以降を採番する	*/
	MONSTB_CHK_PRI,			/* 秤安定ﾁｪｯｸ処理ﾀｽｸ		*/
	MONRCV_TSK_PRI,			/* ﾓﾆﾀｰ用受信処理ﾀｽｸ		*/
/*-------------------------------------*/
	ADCINIT_TSK_PRI		= 12,	/* ADC初期化タスク		*/
	AFVRECALL_TSK_PRI	= 13,	/* ＡＦＶリコールタスク		*/
	AT_ZERO_TSK_PRI		= 18,	/* 自動零点調整タスク		*/
	PRODUCT_TSK_PRI		= 20,	/* 運転タスク			*/
	PRODUCT_TSK_PRI_LOW	= 21,	/* 運転タスク(低速処理)		*/
	DRAIN_TSK_PRI		= 20,	/* 全排出タスク			*/
	ZERO_TSK_PRI		= 20,	/* 零点調整タスク		*/
	DUMP_TSK_PRI		= 20,	/* エラー解除タスク		*/
	SPAN_TSK_PRI		= 20,	/* スパン調整タスク		*/
	MANCALC_TSK_PRI		= 20,	/* 手動計算タスク		*/
	ARCHIVE_TSK_PRI		= 20,	/* 重量波形表示タスク		*/
	TSTDRV_TSK_PRI		= 20,	/* テスト駆動タスク		*/
	ILKRCV_TSK_PRI		= 21,	/* ICUからのメッセージ受信タスク*/
	RCURCV_TSK_PRI		= 21,	/* RCUからのメッセージ受信タスク*/
	LANRCV_TSK_PRI		= 22,	/* DUC(arcnet)メッセジ受信タスク*/
	DUCRCV_TSK_PRI		= 22,	/* DUCからのメッセージ受信タスク*/
	DUCSND_TSK_PRI		= 23,	/* DUCへのメッセージ送信タスク	*/
	DUCERR_TSK_PRI		= 23,	/* DUCエラー監視タスク		*/
	SHIFT_TSK_PRI		= 24,	/* 非同期供給タスク		*/
	INFEED_TSK_PRI		= 25,	/* 供給重量監視タスク		*/
	RES_DETECT_TSK_PRI	= 25,	/* 共振周期検出タスク		*/
	RS485MAP_TSK_PRI	= 26,	/* ネットワークのマップ処理ﾀｽｸ	*/
	LANMAP_TSK_PRI		= 26,	/* ネットワークのマップ処理ﾀｽｸ	*/
	RCUSND_TSK_PRI		= 27,	/* RCUへのメッセージ送信タスク	*/
	ADCDIAG_TSK_PRI		= 28,	/* ADC診断タスク		*/
	DUCDIAG_TSK_PRI		= 28,	/* DUC診断タスク		*/
	CONRCV_TSK_PRI		= 29,	/* CONからのメッセージ受信タスク*/
	CONSND_TSK_PRI		= 29,	/* CONへのメッセージ送信タスク	*/
	BKFWR_TSK_PRI		= 30,	/* ADfileの書き込みﾀｽｸ		*/
/*-------------------------------------*/
	MONAFV_TSK_PRI		= 31,	/* ﾓﾆﾀｰのAFV調整ﾀｽｸ		*/
	MONSND_TSK_PRI		= 31,	/* ﾓﾆﾀｰ用の送信ﾀｽｸ		*/
};

/*------------------------------------------------------*/
/*	イベント･フラグIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum {
	APL_EVF_ID	= 70,       	/* アプリは1番以降を採番する	*/
	RXTMOUT_ID,
/*-------------------------------------*/

/*-------------------------------------*/
	DRV_EVF_ID	= 10       	/* ﾄﾞﾗｲﾊﾞｰは10番以降を採番する	*/
/*-------------------------------------*/
};
enum {
	RF_RXTMOUTFLG = 0x01,
	CF_RXTMOUTFLG = 0x02,
};

/*------------------------------------------------------*/
/*	メールボックスIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum {
	APL_MBX_ID	= 70,       	/* アプリは1番以降を採番する	*/
/*-------------------------------------*/
	BS_MBX_ID0,			/* 基本動作C1用			*/
	BS_MBX_ID1,			/* 基本動作C2用			*/
	LANMAP_MBX_ID,			/* マップ作成タスク用		*/
/*-------------------------------------*/
	DRV_MBX_ID	= 10       	/* ﾄﾞﾗｲﾊﾞｰは100番以降を採番する	*/
/*-------------------------------------*/
};

/*------------------------------------------------------*/
/*	セマフォIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum  {
	APL_SEM_ID	= 150,       	/* アプリは1番以降を採番する	*/
/*-------------------------------------*/
	HEAD_SEM_ID,			/* ヘッド管理セマフォ		*/
/*-------------------------------------*/
	ARC_SEM_ID,			/* arc_contact管理セマフォ	*/
/*-------------------------------------*/
	DRV_SEM_ID	= 10,		/* ﾄﾞﾗｲﾊﾞｰは100番以降を採番する	*/
/*-------------------------------------*/
	RCURCV_SEM_ID	= 190		/* WCUとICUの共有変数管理セマフォ*/
};

/*------------------------------------------------------*/
/*	フラグIDの定義					*/
/*							*/
/*------------------------------------------------------*/

enum {
/*-------------------------------------*/
	MON_SMPLSTA_EVF_ID = 1,		/* ｻﾌﾟﾘﾝｸﾞ･ｽﾀｰﾄの同期ﾌﾗｸﾞ	*/
/*-------------------------------------*/
};

/*------------------------------------------------------*/
/*	メッセージ送受バッファサイズの定義と		*/
/*	各タスクのユーザスタックサイズ			*/
/*------------------------------------------------------*/

#define	RCU_MES_MAX			(512+8)
#define	DUC_MES_MAX			(512+8)
#define	ICU_MES_MAX			(512+8)
#define	HIC_MES_MAX			(512+8)
#define	CON_MES_MAX			(512+8)

#ifdef	DEBUG
#define  STK_SIZE1                     	(512+512)
#define  STK_SIZE2                     	(1024+512+512)
#else
#define  STK_SIZE1                     	(512)
#define  STK_SIZE2                     	(1024+512)
#endif

#define  ADC_DIAG_TSK_STK              (    2048)+STK_SIZE2
#define  AFV_RECALL_TSK_STK            (    2048)+STK_SIZE2
#define  ARCHIVE_TSK_STK               (    2048)+STK_SIZE1
#define  AUTO_ZERO_TSK_STK             (    2048)+STK_SIZE2
#define  BKF_WRITE_TSK_STK             (    2048)+STK_SIZE1
#define  CON_RCV_TSK_STK               (    2048)+STK_SIZE1
#define  CON_SND_TSK_STK               (    2048)+STK_SIZE1
#define  DRAIN_TSK_STK                 (    4096)+STK_SIZE2
#define  DUCERR_MON_TSK_STK            (    2048)+STK_SIZE1
#define  DUC_DIAG_TSK_STK              (    2048)+STK_SIZE1
#define  DUC_RCV_TSK_STK               (    2048)+STK_SIZE1
#define  DUC_SND_TSK_STK               (    2048)+STK_SIZE1
#define  DUMP_TSK_STK                  (    4096)+STK_SIZE2
#define  ILK_RCV_TSK_STK               (    2048)+STK_SIZE1
#define  INFEED_MON_TSK_STK            (    2048)+STK_SIZE2
#define  LAN_MAP_TSK_STK               (    2048)+STK_SIZE1
#define  LAN_RCV_TSK_STK               (    2048)+STK_SIZE1
#define  MANCALC_TSK_STK               (    4096)+STK_SIZE2
#define  PRODUCT_TSK_STK               (    4096)+STK_SIZE2
#define  PSHIFT_TSK_STK                (    4096)+STK_SIZE2
#define  RCU_RCV_TSK_STK               (    4096)+STK_SIZE2
#define  RCU_SND_TSK_STK               (    2048)+STK_SIZE1
#define  RES_DETECT_TSK_STK            (    2048)+STK_SIZE2
#define  RS485_MAP_TSK_STK             (    2048)+STK_SIZE1
#define  SEND_TSK_STK                  (    2048)+STK_SIZE1
#define  SHIFT_TSK_STK                 (    4096)+STK_SIZE2
#define  SPAN_TSK_STK                  (    2048)+STK_SIZE2
#define  TSTDRV_TSK_STK                (    2048)+STK_SIZE1
#define  ZERO_TSK_STK                  (    2048)+STK_SIZE2

#endif
