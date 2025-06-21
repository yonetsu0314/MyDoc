/*======================================================================
 * File name    : rzr033.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr033.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr033.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr033.hv  2.1  93/11/02 15:42:20  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr033.hv
 * Revision 2.1  93/11/02 15:42:20  ryoji
 * 第５版対応
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR033_H)
#define	RZR033_H


/*------------------------------------------------------*/
/*	零点調整パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr033t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	adj_head_ptn;	/* 調整ヘッドビットパターン	*/
	unsigned char	adj_df_ptn;	/* 調整DFビットパターン		*/
} RZR033T;

#if	defined(STORAGE)
struct rzr033t	rzr033s;
#else
#if	!defined(NOEXTERN)
extern struct rzr033t	rzr033s;
#endif
#endif

#endif
