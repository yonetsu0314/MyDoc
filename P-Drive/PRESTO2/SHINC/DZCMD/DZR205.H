/*======================================================================
 * File name    : dzr205.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＡＦＶセル出力要求データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzr205.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr205.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: DZR205.Hv  1.2  94/02/18 18:12:12  ryoji
 *Log: P:/INCLUDE/DZCMD/RCS/DZR205.Hv
 * Revision 1.2  94/02/18 18:12:12  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZR205_H)
#define	DZR205_H

/*------------------------------------------------------*/
/*	AFV出力選択パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzr205t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	head_no;	/* ヘッド番号			*/
} DZR205T;

#if	defined(STORAGE)
struct dzr205t	dzr205s;
#else
#if	!defined(NOEXTERN)
extern struct dzr205t	dzr205s;
#endif
#endif

#endif
