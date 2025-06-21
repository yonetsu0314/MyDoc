/*======================================================================
 * File name	: pri1029.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	補助組み合わせ候補重量データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri1029.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri1029.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2005/07/09  10:55
 * データのサイズを３２に変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI1029_H)
#define	PRI1029_H


typedef struct pri10290t {
	unsigned char	nof_data;	/* number of data		*/
	short		weight[32];	/* 重量				*/
} PRI10290T;

/*------------------------------------------------------*/
/*	補助組み合わせ候補重量データパラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri1029t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_sect;	/* 該当するchのセクション数	*/
	PRI10290T	sect[8];	/* セクション演算結果		*/
} PRI1029T;

#if	defined(STORAGE)
struct pri1029t	pri1029s;
#else
#if	!defined(NOEXTERN)
extern struct pri1029t	pri1029s;
#endif
#endif

#endif
