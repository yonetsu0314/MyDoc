/*======================================================================
 * File name    : rzr063.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	付加装置拡張パラメータデータ構造
 *	上下動シュート
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr063.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr063.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr063.hv  1.1  94/06/03 10:59:52  fukutome
 *Log: p:/include/rzcmd/rcs/rzr063.hv
 * Revision 1.1  94/06/03 10:59:52  fukutome
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR063_H)
#define	RZR063_H

#define	DEV_UPDOWN_SHOOT	(1)

/*------------------------------------------------------*/
/*	付加装置パラメータパケット			*/
/*	（上下動シュート）				*/
/* rzr063,rzr064					*/
/*------------------------------------------------------*/

typedef struct rzr063t {
	unsigned char	dev_id;		/* device ID = 1 上下動シュート*/
	unsigned char	nof_pos;	/* ポジション数			*/
	unsigned char	err_detect_interval;	/* エラー検出時間（*10ms）*/
	unsigned char	position;	/* 設定位置			*/
} RZR063T;

#if	defined(STORAGE)
struct rzr063t	rzr063s;
#else
#if	!defined(NOEXTERN)
extern struct rzr063t	rzr063s;
#endif
#endif

#endif
