/*======================================================================
 * File name    : rzr091.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 * マルチ連動パラメータ1構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr091.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr091.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr091.h 1.3 1997/12/08 18:34:43 itami
 *Log: rzr091.h
 * リビジョン 1.3  1997/12/08  18:34:43  itami
 * ビットフィールドを構造体にする
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR091_H)
#define	RZR091_H


/*------------------------------------------------------*/
/*	マルチ連動パラメータ1構造			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr0910t {
	unsigned my_node	:5 ;		/* 自ノード番号 0~4bit		*/
	unsigned chg_ch		:1 ;		/* チャンネル変換 5bit		*/
	unsigned ilk_inf	:2 ;		/* 連動形態 6~7bit		*/
	unsigned Dummy		:8 ;		/* 予備 			*/
} RZR0910T;
typedef struct rzr091t {
	unsigned char	ch_id;			/* channel number		*/
	unsigned char	neighbor_node;		/* 前段ノード番号		*/
	unsigned char	shift_cnt;		/* シフト数			*/
	unsigned char	retry_time;		/* リトライ時間			*/
	unsigned char	retry_span;		/* リトライ間隔			*/
	unsigned char	retry_cnt;		/* リトライ回数			*/
	unsigned char	master_slave;		/* マスター／スレーブ設定	*/
	unsigned char	ch_status;		/* チャンネル設定		*/
	struct rzr0910t	dsw;			/* NHIC ディップスイッチ２状態	*/
} RZR091T;

#if	defined(STORAGE)
struct rzr091t	rzr091s;
#else
#if	!defined(NOEXTERN)
extern struct rzr091t	rzr091s;
#endif
#endif

#endif
