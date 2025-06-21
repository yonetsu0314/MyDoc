/*======================================================================
 * File name	: prr4023.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算仕様拡張パラメータデータ構造
 *	ＡＴＡ　Ｓａｍｐｌｅ  Ｓｉｚｅ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr4023.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr4023.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr423.h 1.2 1996/07/17 17:10:37 hiroki Exp
 *log: rzr423.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR4023_H)
#define	PRR4023_H

/*------------------------------------------------------*/
/*	ＡＴＡ　Ｓａｍｐｌｅ  Ｓｉｚｅ			*/
/*------------------------------------------------------*/

typedef struct prr4023t {
	unsigned short	sample_size;	/*ATA SAMPLE SIZE	*/
	unsigned char	preset_num;	/*プリセット番号　　	*/
} PRR4023T;

#if	defined(STORAGE)
struct prr4023t	prr4023s;
#else
#if	!defined(NOEXTERN)
extern struct prr4023t	prr4023s;
#endif
#endif

#endif
