/*======================================================================
 * File name	: pri0017.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	異常発生データ構造
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/pri0017.h 1.25 2019-03-28 14:49:39+09 itami Exp $
 *$Log: pri0017.h $
 *Revision 1.25  2019-03-28 14:49:39+09  itami
 *供給機電源オフ,供給機連動エラー追加
 *
 *Revision 1.24  2019-01-07 16:33:11+09  itami
 *金検テストエラー追加
 *
 *Revision 1.23  2017-06-14 15:41:30+09  itami
 *運転不許可、ガス充填装置異常追加
 *
 *Revision 1.22  2017-02-06 11:04:47+09  satoruk
 *供給コンベヤエラー追加。
 *
 *Revision 1.21  2016-11-02 09:37:49+09  hiroe
 *2016/11/02
 *i-SFが未接続, i-SFへ送信不可, i-SF送信バッファフル, i-SFへ再送失敗, i-SFから応答なしの追加
 *
 *Revision 1.20  2016-08-26 16:53:59+09  hiroe
 *2016.08.26
 *i-STATION LINK 連動エラーの追加
 *
 *Revision 1.19  2016-06-08 17:46:33+09  hiroe
 *2016/06/08
 *スターホイールサーボエラー,スイング供給１サーボエラー,スイング供給２サーボエラー追加
 *
 *Revision 1.18  2016-04-07 14:04:05+09  hiroe
 *2016/04/07
 *RS駆動エラー
 *
 *Revision 1.17  2015-11-05 21:02:57+09  hiroe
 *2015/11/05
 *COM1予約交信受信エラー,COM1予約交信送信エラーの追加
 *
 *Revision 1.16  2015-10-21 18:28:38+09  hiroe
 *2015.10.21
 *COM予約交信受信エラーの追加
 *
 *Revision 1.15  2015-10-19 20:43:11+09  hiroe
 *2015.10.19
 *COM予約交信送信エラーの追加
 *
 *Revision 1.14  2015-09-08 10:59:13+09  itami
 *RS原点復帰エラー追加
 *
 *Revision 1.13  2014-12-09 14:10:35+09  itami
 *設定値エラー追加
 *
 *Revision 1.12  2014-10-17 15:14:46+09  itami
 *フレーム位置エラー追加
 *
 *Revision 1.11  2014-04-02 00:49:03+09  satoruk
 *2014/04/01
 *CTC用にインターフェイスエラーを追加する。
 *
 * Revision 1.10  1970/01/01  00:00:00  itami
 * 2014/02/06 スライドTH動作異常追加。
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  hiroe
 * 2013/03/12
 * CTC用にSHUTTERエラー、ABSエラー、N2CTLエラーを追加する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  itami
 * 2011/08/23 分散モーターエラー追加。
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  itami
 * 2008/08/20 同期エラー追加。
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  itami
 * 2007/10/15 サンプリング重量エラー
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kawamura
 * 2007/08/18  09:51
 * 詰まりエラーを追加する。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  ishidat
 * 2007/01/24 16:30
 * 自動零点タイムアウトエラー追加
 * ERROR_AUTO_ZERO_TIMEOUT
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  itami
 * 2005/08/12 センサー検知エラー追加。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/03/15 マルチ連動関連エラー追加　ERROR_MULT
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi017a.h 1.14 1970/01/01 00:00:00 itami Exp
 *log: rzi017a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0017_H)
#define	PRI0017_H


enum pri00170e {			/* 異常内容			*/
	ERROR_ZERO,			/* 零点				*/
	ERROR_SPAN,			/* スパン			*/
	ERROR_WH,			/* ＷＨ				*/
	ERROR_PH,			/* ＰＨ				*/
	ERROR_BH,			/* ＢＨ				*/
	ERROR_RS,			/* ＲＳ				*/
	ERROR_DTH,			/* ＤＴＨ			*/
	ERROR_TH,			/* ＴＨ				*/
	ERROR_PW,			/* 単重				*/
	ERROR_OVSCL,			/* オーバースケール		*/
	ERROR_OVWGT,			/* 過量				*/
	ERROR_AFD,			/* AFD				*/
	ERROR_CHT,			/* 上下動シュート		*/
	ERROR_RCH,			/* 再チェックエラー（個数計量）	*/
	ERROR_NETWORK,			/* 通信エラー			*/
	ERROR_AD,			/* AD				*/
	ERROR_RAM,			/* RAM				*/
	ERROR_ROM,			/* ROM				*/
	ERROR_AUTO_DF,			/* 分散重量自動変更		*/
	ERROR_METAL,			/* 金属検出			*/
	ERROR_AIR,			/* エアー圧低下			*/
	ERROR_DOOR,			/* 扉開放			*/
	ERROR_DIS,			/* 排出物つまり			*/
	ERROR_ILK,			/* 連動エラー			*/
	ERROR_CUP,			/* カップエラー			*/
	ERROR_TRIP,			/* トリップエラー		*/
	ERROR_FATAL_ZERO,		/* 致命的零点エラー		*/
	ERROR_COUNT,			/* 個数換算エラー		*/
	ERROR_DIVERTER,			/* 振り分け部エラー		*/
	ERROR_EMERGENCY_STOP,		/* 非常停止			*/
	ERROR_UNDWGT,			/* 軽量停止			*/
	ERROR_FDRV,			/* FDRVエラー			*/
	ERROR_TB,			/* TBエラー			*/
	ERROR_MB,			/* MBエラー			*/
	ERROR_BB,			/* BBエラー			*/
	ERROR_REJ,			/* 不良振り分け部エラー		*/
	ERROR_TPH,			/* 先端ＰＨエラー		*/
	ERROR_MULT,			/* マルチ連動関連エラー		*/
	ERROR_DETECT,			/* センサー検知エラー		*/
	ERROR_AUTO_ZERO_TIMEOUT,	/* 自動零点タイムアウトエラー	*/
	ERROR_JAM,			/* 詰まりエラー			*/
	ERROR_SMP,			/* サンプリング重量エラー	*/
	ERROR_SYNC,			/* 同期エラー			*/
	ERROR_DF_MOTOR,			/* 分散モーターエラー		*/
	ERROR_SHUTTER,			/* CTC用シャッターエラー	*/
	ERROR_ABS,			/* CTC用ABSエラー		*/
	ERROR_N2CTL,			/* CTC用N2CTLエラー		*/
	ERROR_POKER,			/* CTC用POKERエラー		*/
	ERROR_SLIDE_TH,			/* スライドTH動作異常		*/
	ERROR_CTC_IF,			/* CTC用インターフェイスエラー	*/
	ERROR_FRAME_POSITION,		/* フレーム位置エラー		*/
	ERROR_SETTING,			/* 設定値エラー			*/
	ERROR_RSRET,			/* RS原点復帰エラー		*/
	ERROR_COM_PRESND0,		/* COM0予約交信送信エラー */
	ERROR_COM_PRERCV0,		/* COM0予約交信受信エラー */
	ERROR_COM_PRESND1,		/* COM1予約交信送信エラー */
	ERROR_COM_PRERCV1,		/* COM1予約交信受信エラー */
	ERROR_RS_SRVERR,		/* RS駆動エラー */
	ERROR_STRWILLERR,		/* スターホイールサーボエラー */
	ERROR_SWING1_ERR,		/* スイング供給１サーボエラー */
	ERROR_SWING2_ERR,		/* スイング供給２サーボエラー */
	ERROR_ISLSND_ERR,		/* i-STATION LINK 連動エラー */
	ERROR_ISFCON_ERR,		/* i-SFが未接続              */
	ERROR_ISFSND_ERR,		/* i-SFへ送信不可            */
	ERROR_ISFFUL_ERR,		/* i-SF送信バッファフル      */
	ERROR_ISFRTY_ERR,		/* i-SFへ再送失敗            */
	ERROR_ISFRES_ERR,		/* i-SFから応答なし          */
	ERROR_INFD_CONV,		/* 供給コンベヤエラー        */
	ERROR_PROD_DIS,			/* 運転不許可                */
	ERROR_GAS_FILLER,		/* ガス充填装置異常          */
	ERROR_METAL_TEST,		/* 金属検出器テストエラー    */
	ERROR_IFD_PW_OFF,		/* 供給機電源オフ	     */
	ERROR_IFD_ILK,			/* 供給機連動エラー	     */

	ERROR_NON = 255,		/* 拡張側エラー			*/
};

enum pri00171e {			/* ＤＵＣ通信エラー情報		*/
	ERR_D_CONNECT	= 0x01,		/* 回線断エラー			*/
	ERR_D_NODE	= 0x02,		/* ノード無しエラー		*/
};

enum pri00172e {			/* ＥＸＣ通信エラー情報		*/
	ERR_E_CONNECT	= 0x01,		/* 回線断エラー			*/
	ERR_E_NODE	= 0x02,		/* ノード無しエラー		*/
};

enum pri00173e {			/* ＤＵＣエラー情報		*/
	ERR_D_CALC	= 0x01,		/* 駆動パターン計算エラー	*/
	ERR_D_RECEPT	= 0x02,		/* 駆動パラメータ未受信エラー	*/
	ERR_D_STM	= 0x04,		/* 駆動エラー			*/
	ERR_D_POWER	= 0x08,		/* パワーエラー			*/
};

enum pri00174e {			/* ＥＸＣエラー情報		*/
	ERR_E_STM	= 0x01,		/* ｽﾃｯﾋﾟﾝｸﾞﾓｰﾀ異常停止エラー	*/
	ERR_E_OVERLAP	= 0x02,		/* 動作オーバーラップエラー	*/
	ERR_E_RECEPT	= 0x04,		/* 駆動パラメータ未受信エラー	*/
	ERR_E_POWER	= 0x08,		/* パワーエラー			*/
};

enum pri00175e {			/* ＦＤＲＶエラー情報		*/
	ERR_F_COMERR	= 0x01,		/* 通信エラー			*/
	ERR_F_RECEPT	= 0x02,		/* 駆動パラメータ未受信エラー	*/
	ERR_F_OVC	= 0x04,		/* 過電流エラー			*/
	ERR_F_POWER	= 0x08,		/* パワーエラー			*/
};

enum pri00176e {			/* ＦＤＲＶボード番号情報	*/
	FDRV_BOARD_1	= 0x01,		/* FDRVボード1			*/
	FDRV_BOARD_2	= 0x02,		/* FDRVボード2			*/
	FDRV_BOARD_3	= 0x04,		/* FDRVボード3			*/
	FDRV_BOARD_4	= 0x08,		/* FDRVボード4			*/
};
/*------------------------------------------------------*/
/*	異常情報のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0017t {
	unsigned char	ch_id;		/* channel number		*/
	enum pri00170e	error;		/* 異常情報			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned short	rs_ptn;		/* RS pattern			*/
	unsigned short	dth_ptn;	/* DTH pattern			*/
	unsigned short	th_ptn;		/* TH pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned long	af_ptn;		/* AF pattern			*/
	unsigned char	duc_inf;	/* DUC error information	*/
	unsigned char	exc_inf;	/* EXC error information	*/
} PRI0017T;

#if	defined(STORAGE)
struct pri0017t	pri0017s;
#else
#if	!defined(NOEXTERN)
extern struct pri0017t	pri0017s;
#endif
#endif

#endif
