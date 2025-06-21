/*======================================================================
 * File name	: pri0061.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	状態表示情報構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/rcs/pri0061.h 1.2 1970/01/01 00:00:00 itami Exp $
 *$Log: pri0061.h $
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 * 2005/09/28 補正計量中のメッセージ追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  itami
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-SHI SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0061_H)
#define	PRI0061_H


enum pri00610e {			/* 状態表示内容			*/
	INFO_MLT1,			/* MDMUネットワーク構築完了	*/
	INFO_MLT2,			/* ネットワーク構築未完		*/
	INFO_MLT3,			/* シフトデータクリア(RCU)	*/
	INFO_MLT4,			/* シフトデータクリア(SIG)	*/
	INFO_MLT5,			/* 終業終了(RCU)		*/
	INFO_MLT6,			/* 終業終了(SIG)		*/
	INFO_MLT7,			/* 終業開始(RCU)		*/
	INFO_MLT8,			/* 終業開始(SIG)		*/
	INFO_MLT9,			/* 連動にて補正計量中です	*/
};

/*------------------------------------------------------*/
/*	状態表示情報のパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0061t {
	unsigned char	ch_id;		/* channel number		*/
	enum pri00610e	info;		/* 状態表示情報			*/
	unsigned long	hp_ptn;		/* Hopper pattern		*/
} PRI0061T;

#if	defined(STORAGE)
struct pri0061t	pri0061s;
#else
#if	!defined(NOEXTERN)
extern struct pri0061t	pri0061s;
#endif
#endif

#endif
