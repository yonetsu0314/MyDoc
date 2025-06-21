/*======================================================================
 * File name    : pri0041.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	シフトデータ応答結果データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0041.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pri0041.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/presto2/shinc/rzcmd/RCS/rzi041.h 1.1 1970/01/01 00:00:00
 *Log: rzi041.h
 *----------------------------------------------------------------------
 *		(c) Copy	right 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(PRI0041_H)
#define	PRI0041_H


typedef struct pri00410t {
	unsigned char	sta_flag;	/* state flag			*/
	long		weight;		/* 重量				*/
	long		count;		/* 個数 			*/
	unsigned char	ser_no;		/* シリアル番号			*/
} PRI00410T;

/*------------------------------------------------------*/
/*	シフトデータ応答結果パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0041t {
	unsigned char		ch_id;		/* channel number	*/
	unsigned char		all_num;	/* all data num 	*/
	unsigned char		data_page;	/* data page          	*/
	unsigned char		data_num;	/* ページ内のデータ数	*/
	struct pri00410t	data[16];	/* 各データ内容		*/
} PRI0041T;

#if	defined(STORAGE)
struct pri0041t	pri0041s;
#else
#if	!defined(NOEXTERN)
extern struct pri0041t	pri0041s;
#endif
#endif

#endif
