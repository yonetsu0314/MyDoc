/*======================================================================
 * File name    : prr0091.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動パラメータ1構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0091.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: prr0091.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr091.h 1.3 1997/12/08 18:34:43
 *Log: rzr091.h
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRR0091_H)
#define	PRR0091_H


/*------------------------------------------------------*/
/*	マルチ連動パラメータ1構造							*/
/*														*/
/*------------------------------------------------------*/

typedef struct prr00910t {
	unsigned my_node	:5 ;			/* 自ノード番号 0~4bit			*/
	unsigned chg_ch		:1 ;			/* チャンネル変換 5bit			*/
	unsigned ilk_inf	:2 ;			/* 連動形態 6~7bit				*/
	unsigned Dummy		:24;			/* 予備 						*/
} PRR00910T;
typedef struct prr0091t {
	unsigned char		ch_id;			/* channel number				*/
	unsigned char		neighbor_node;	/* 前段ノード番号				*/
	unsigned char		shift_cnt;		/* シフト数						*/
	unsigned char		retry_time;		/* リトライ時間					*/
	unsigned char		retry_span;		/* リトライ間隔					*/
	unsigned char		retry_cnt;		/* リトライ回数					*/
	unsigned char		master_slave;	/* マスター／スレーブ設定		*/
	unsigned char		ch_status;		/* チャンネル設定				*/
	struct prr00910t	dsw;			/* NHIC ディップスイッチ２状態	*/
} PRR0091T;

#if	defined(STORAGE)
struct prr0091t	prr0091s;
#else
#if	!defined(NOEXTERN)
extern struct prr0091t	prr0091s;
#endif
#endif

#endif
