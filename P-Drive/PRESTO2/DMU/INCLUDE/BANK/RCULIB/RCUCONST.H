/*======================================================================*/
/* File name	: RCUCONST.h											*/
/*----------------------------------------------------------------------*
 *$Header: /RCU32LIB/BANK/rculib/RCUCONST.H 3     02/04/01 10:16 Ryoji $
 *----------------------------------------------------------------------*/
/*original: p:/rcu/include/rculib/RCS/rcuconst.h 2.15                   */
/*----------------------------------------------------------------------*/
/*		(c) Copyright 1997 ISHIDA CO., LTD.								*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*/
/*		Phone +81 775 53-4141											*/
/*----------------------------------------------------------------------*/

#ifndef	__RCUCONST_H__
#define	__RCUCONST_H__

/*
 *----------------------------------------------------------------------
 *	共通定数の定義
 *----------------------------------------------------------------------
*/
#define	BELL		(0x07)
#define LF		(0x0a)
#define CR		(0x0d)
#define ESC		(0x1b)

/* #define INT_MIN 	0x8000*/		/* int型 最大値		*/
/* #define INT_MAX 	0x7fff*/		/* int型 最小値		*/
/* #define LONG_MAX 	0x7fffffffL*/	/* long型 最大値	*/

enum {ILLEGAL = -1};

enum {OFF, ON};
enum {NO, YES};

#ifndef	NULL
#define	NULL	((void*)0)
#endif

#define ISHIDA_NAME_MAX 	(24)		/* 計量物名最大長		*/
#define CODE_MAX	(16)		/* 計量物コード最大長		*/

#define	ROM_ID_MAX	(64)		/* 読み出し可能な ROM_ID の数	*/

/*
 *----------------------------------------------------------------------
 *	国名情報の定義
 *----------------------------------------------------------------------
*/
enum {UK = 1, US, JN};
enum{
	USA = 1,
	ENGLISH,
	JAPANESE,
	FRENCH,
	GERMAN,
	SPANISH,
	SWEDISH,
	DANISH,
	DUTCH,
	FINNISH,
	POLISH,
	GREEK,
	TURKISH,
	ITALIAN,
	PORTUGUESE,
	CHINESE,
	KOREAN,
	THAI,
	HUNGARY,
	NORWAY,
	RUSSIAN,
	SLOVAKIA,
	CZECHO,
};
/*
 *----------------------------------------------------------------------
 *	文字フォント
 *----------------------------------------------------------------------
*/
enum {
	FONT_0,				/* デフォルト	*/
	FONT_1,
	FONT_2,
	FONT_3,
	FONT_4,
	FONT_5,
	FONT_6,
	FONT_7,
};
/*					フォントの名前	大きさ（幅×高さ） */	#define	COURB12		FONT_0		/* 12*12    Courb12	8*14	*/
#define	STANDARD_FONT	FONT_0		/* 標準フォント			*/
#define	COURBO12I	FONT_1		/* 12*12 I  Courbo12	11*14	*/
#define	COURB14		FONT_2		/* 14*14    Courb14	11*16	*/
#define	COURBO14I	FONT_3		/* 14*14 I  Courbo14	12*16	*/
#define	COURB18		FONT_4		/* 18*18    Courb14	13*20	*/
#define	COURBO18I	FONT_5		/* 18*18 I  Courbo14	14*20	*/
#define	COURB24		FONT_6		/* 24*24    Courb24	17*25	*/
#define	COURBO24I	FONT_7		/* 24*24 I  Courbo24	19*25	*/

/* 表示フォントについて		1994.11.1	*/
/* FONT_0 を選択した場合、漢字 CGROM のフォントが使用される。（COURB12	*/
/* は選択できない） 文字の大きさは、漢字等の２バイト系の文字は 16×16 。*/
/* １バイト系の文字は 8×16 となる。  なおこれ以外のフォントを選択した	*/
/* 場合は、漢字などの２バイト系の文字は表示できない。			*/
/**/
/* 画面データファイルでフォントを選択する場合次の方法による。		*/
/* 1) 項目定義：							*/
/*	文字の非実行色でフォントの種類を指定する。			*/
/* 2) グラフィック文字（画面に埋め込まれている文字）：			*/
/*	描画直前にプログラム（font_select() 関数）で指定されている	*/
/*	フォントで表示される。						*/
/* FONT_0 以外では実際に表示される大きさは、画面上に定義した見かけの	*/
/* 大きさとは異なるため文字幅の定義などに注意が必要。			*/

/*
 *----------------------------------------------------------------------
 *	タイマー定数 （× 5 msec）
 *----------------------------------------------------------------------
*/
#define	TIME_600MSEC	(120)		/*  0.6 秒 	*/
#define	TIME_1SEC	(200)		/*  1   秒	*/
#define	TIME_2SEC	(400)		/*  2   秒	*/
#define	TIME_3SEC	(600)		/*  3   秒	*/
#define	TIME_5SEC	(1000)		/*  5   秒	*/
#define	TIME_10SEC	(2000)		/* 10   秒	*/
#define	TIME_30SEC	(6000)		/* 30   秒	*/
#define	TIME_50SEC	(9999)		/* 50   秒	*/
#define	TIME_1MIN	(12000)		/*  1   分	*/

#define	COM_ERR_DETECT_INTERVAL0	TIME_30SEC
#define	COM_ERR_DETECT_INTERVAL		TIME_5SEC

#if 0
enum {	CMD_UP_TIMER = 1,
	SCRSVR_TIMER,
	WARN_STOP_TIMER,
	TENKEY_UP_TIMER,
	REPEAT_UP_TIMER,
	ALARM_TIMER,
	COM_ERR_DETECT_TIMER,
	USR_TIMER1,
	USR_TIMER2,
	DUMMY10,
	CW_ZADJ_TMR1,	/*	CW_EX_USRTMR0,*/
	CW_ZADJ_TMR2,	/*	CW_EX_USRTMR1,*/
	CW_TMADJ_TMR,	/*	CW_EX_USRTMR2,*/
};
#endif

#define	SVRBSE_TIMER	ALARM_TIMER
#define	HELP_UP_TIMER	WARN_STOP_TIMER
#define	SCALEIN_TIMER	TENKEY_UP_TIMER

#define	KBUF_SIZE	(20)		/* key buffer size	*/
#define	EBUF_SIZE	(50)		/* error message buffer */

/* キーリピート間隔（x10ms）	*/
#define	DEFAULT_KEY_REPEAT	(20)
#define	RAPID_KEY_REPEAT	(20)

#define	CLICK_BELL_LEN		(50)

/*
 *----------------------------------------------------------------------
 *	機物の仕様定義				 v1.03
 *----------------------------------------------------------------------
*/
#define	H_MAX		(32)		/* 最大３２ヘッド		*/

enum {C1_, C2_, C_MAX};			/* Cn番号, チャンネル数		*/

enum {S1_, S2_, S3_, S4_, S5_, S6_, S7_, S8_,};		/* Sn番号	*/

enum {DF1_, DF2_, DF3_, DF4_, DF5_, DF6_, DF7_, DF8_,DF_MAX};	/* DFn番号*/

enum {CF1_, CF2_, CF3_, CF4_, CF5_, CF6_, CF7_, CF8_,CF_MAX};	/* CFn番号*/

enum {PH_, WH_, BH_, DTH_, TH_, RS_, HP_MAX, RF_, DF_,};

enum {P1_, P2_, P_MAX};			/* 親ヘッド番号		*/

#define	RS_MAX		(16)	/* CCW-R機の通信テキストバージョンアップに伴う修正 8->16 */
#define	DTH_MAX		(16)	/* CCW-R機の通信テキストバージョンアップに伴う修正 8->16 */
#define	TH_MAX		(16)	/* CCW-R機の通信テキストバージョンアップに伴う修正 8->16 */
#define	SECT_DIV_MAX	(8)	/* セクション分割パターン最大数	*/

/*----------------------------------------------------------------------*/
/*	重量データ換算		     	 				*/
/*----------------------------------------------------------------------*/
#define	KG_TO_G		(1000)
#define	LB_TO_OZ	(16)

enum {C1, C2, C1_C2, C_MIX};
/*----	 wcu_mode --*/
#define	M_STOP		(0x01)		/*  product sub command		*/
#define	M_PRODUCT	(0x02)		/* 		       		*/
#define	M_DRAIN		(0x04)		/* 		       		*/
#define	M_CALC		(0x08)		/* 		       		*/
#define	M_SCALEIN	(0x10)		/* 		       		*/
#define M_DUMP		(0x40)		/* 	 	       		*/
#define	M_INFEED	(0x80)		/* 		       		*/
#define	M_ZERO		(0x0100)	/*				*/
#define	M_SPAN		(0x0200)	/*				*/
#define	M_WARNING	(0x4000)	/*                              */
#define	M_ERROR		(0x8000)	/*                              */

/*--- rcu_status.user_level ------------------------------------------------*/

enum {
	USER_LEVEL0,
	USER_LEVEL1,
	USER_LEVEL2,
	USER_LEVEL3,
	USER_LEVEL4,
	USER_LEVEL_MAX
};

#define	OPERATOR_LEVEL		USER_LEVEL0	/* 作業レベル		*/
#define	SITE_ENGINEER_LEVEL	USER_LEVEL1	/* 管理レベル		*/
#define	INSTALLATION_LEVEL	USER_LEVEL2	/* 設置調整レベル	*/
#define	SERVICE_LEVEL		USER_LEVEL3	/* 保守サービスレベル	*/
#define	ONLINE_LEVEL		USER_LEVEL4	/* オンラインレベル	*/

/*--- rcu_status.host_flag -----------------------------------------------*/
#define ONLINE_OK	(0x0001)	/* ホストコンピュータ接続フラグ */
#define ONLINE		(0x0002)	/* オンラインフラグ		*/
#define WEI_DATA	(0x0010)	/* 重量要求コマンド受信フラグ	*/
#define ERROR_STAT	(0x0100)	/* エラー状態フラグ		*/

/*----------------------------------------------------------------------*/
/*	ポートアドレス		     	 				*/
/*----------------------------------------------------------------------*/
#define	DIP_SW	(0x0040)

#define	SW_1	(0x20)		/* Monitor Jump		[BIOS]	*/
#define	SW_2	(0x10)		/* BPS SELECT		[BIOS]	*/
#define	SW_3	(0x08)		/* FULL/HALF SELECT	[BIOS]	*/
#define	SW_4	(0x04)
#define	SW_5	(0x02)		/* DEBUG		[APL]	*/
#define	SW_6	(0x01)		/* LCD/PDP		[BIOS/APL] */

#define	DISPLAY_TYPE	SW_6
#define	LCD_COLOR	(0)
#define	DEBUG_MODE	SW_6

#endif		/* __RCUCONST_H__ */
/*======================================================================*/
/*====                   ***** END OF FILE *****                    ====*/
/*======================================================================*/
