/*======================================================================
 * File name    : rzr123.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（１）データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr123.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr123.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: rzr123.hv  1.1  94/05/27 09:36:58  fukutome
 *Log: p:/include/rzcmd/rcs/rzr123.hv
 * Revision 1.1  94/05/27 09:36:58  fukutome
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR123_H)
#define	RZR123_H

/*------------------------------------------------------*/
/*	優先参加回数パラメータパケット			*/
/*------------------------------------------------------*/

typedef struct rzr123t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	unsigned char	cnt[8];		/* 優先参加回数			*/
} RZR123T;

#if	defined(STORAGE)
struct rzr123t	rzr123s;
#else
#if	!defined(NOEXTERN)
extern struct rzr123t	rzr123s;
#endif
#endif

#endif
