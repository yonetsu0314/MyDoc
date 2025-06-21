/*======================================================================
 * File name	: pri0005.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	単重更新結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0005.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0005.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzi005.hv  2.1  93/11/02 15:40:32  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzi005.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0005_H)
#define	PRI0005_H


/*------------------------------------------------------*/
/*	単重更新結果データのパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0005t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	unsigned short	weight[8];	/* 単重				*/
} PRI0005T;

#if	defined(STORAGE)
struct pri0005t	pri0005s;
#else
#if	!defined(NOEXTERN)
extern struct pri0005t	pri0005s;
#endif
#endif

#endif
