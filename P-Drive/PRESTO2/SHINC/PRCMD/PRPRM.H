/*======================================================================
 * File name	: prprm.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	パラメータ及び定数等
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prprm.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prprm.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzprm.h 1.2 1997/12/04 14:27:27 itami Exp
 *log: rzprm.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRPRM_H)
#define	PRPRM_H

enum prprm001e {			/* ボード番号			*/
	BORD_RCU,			/* ＲＣＵ			*/
	BORD_ICU,			/* ＩＣＵ			*/
	BORD_WCU,			/* ＷＣＵ			*/
	BORD_DUC,			/* ＤＵＣ			*/
	BORD_EXC,			/* ＥＸＣ			*/
	BORD_ADC,			/* ＡＤＣ			*/
	BORD_NHIC,			/* ＮＨＩＣ			*/
};

enum prprm002e {			/* データの種類			*/
	AC_VOLTAGE,			/* AC電源電圧			*/
	AC_FREQUENCY,			/* AC電源周波数			*/
	FDC_DSW_INFO,			/* FDC基板ﾃﾞｨｯﾌﾟｽｲｯﾁ情報	*/
	JAW_TIMING,			/* JAW CLOSE TIMING INFO	*/
	FEED_VALUE,			/* 投入重量			*/
	FEED_STD,			/* 投入重量の標準偏差		*/
};

enum prprm003e {			/* ホッパ状態			*/
	HSTS_SELE = 0x0001,		/* 選択				*/
	HSTS_STBL = 0x0002,		/* 安定				*/
	HSTS_EMPTY = 0x0004,		/* 空				*/
	HSTS_AZERO = 0x0008,		/* 自動零点			*/
	HSTS_ERR = 0x0010,		/* エラー			*/
	HSTS_OVSCL = 0x0020,		/* オーバースケール		*/
	HSTS_PFS = 0x0040,		/* プラスフルスケール		*/
	HSTS_MFS = 0x0080,		/* マイナスフルスケール		*/
	HSTS_4TH = 0x0100,		/* ４段目データ採用		*/
	HSTS_5TH = 0x0200,		/* ５段目データ採用		*/
};

enum prprm004e {			/* ホッパの種類			*/
	HOPPER_PH,			/* ＰＨ				*/
	HOPPER_WH,			/* ＷＨ				*/
	HOPPER_BH,			/* ＢＨ				*/
	HOPPER_DTH,			/* ＤＴＨ			*/
	HOPPER_TH,			/* ＴＨ				*/
	HOPPER_RS,			/* ＲＳ				*/
};
#endif
