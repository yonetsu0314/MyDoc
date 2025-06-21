/*======================================================================
 * File name    : rzr423.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータ（４）データ構造
 *	ＡＴＡ　Ｓａｍｐｌｅ  Ｓｉｚｅ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr423.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr423.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr423.h 1.2 1996/07/17 17:10:37 hiroki
 *Log: rzr423.h
 * リビジョン 1.2  1996/07/17  17:10:37  hiroki
 * プリセット番号追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR423_H)
#define	RZR423_H

/*------------------------------------------------------*/
/*	ＡＴＡ　Ｓａｍｐｌｅ  Ｓｉｚｅ			*/
/*------------------------------------------------------*/

typedef struct rzr423t {
	unsigned short	sample_size;	/*ATA SAMPLE SIZE	*/
	unsigned char	preset_num;	/*プリセット番号　　	*/
} RZR423T;

#if	defined(STORAGE)
struct rzr423t	rzr423s;
#else
#if	!defined(NOEXTERN)
extern struct rzr423t	rzr423s;
#endif
#endif

#endif
