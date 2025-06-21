/*======================================================================
 * File name    : rzr111.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	組合せ計算拡張パラメータデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr111.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr111.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr111.h 1.5 1998/07/30 15:03:56 hiroe
 *Log: rzr111.h
 * リビジョン 1.5  1998/07/30  15:03:56  hiroe
 * PTB_FLAG -> NMI_FLAG
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR111_H)
#define	RZR111_H

enum rzr1110e {				/* 検定フラグ	    */
        NO_FLAG		= 0x00,		/* 無検定 	    */
        NMI_FLAG	= 0x01,  	/* ＮＭＩ検定       */
        AUSTRIA_FLAG	= 0x02,		/* オーストリア検定 */
	HOLLAND_FLAG	= 0x04,		/* オランダ検定	    */
	FRANCE_FLAG	= 0x08,		/* フランス検定	    */
};

enum rzr1111e {				/* 容量フラグ */
        VOL_S_FLAG	= 0x01,		/* 標準容量　 */
        VOL_M_FLAG	= 0x02,		/* 中容量     */
	VOL_L_FLAG	= 0x04,		/* 大容量     */
	VOL_O_FLAG	= 0x08,		/* 特大容量　 */
};

/*------------------------------------------------------*/
/*	検定に必要なパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr111t {
	enum rzr1110e	approval_flag; 	/* approval flag		*/
	enum rzr1111e	vol_flag; 	/* vol flag     		*/
	unsigned short	max_weight;    	/* max. weight of approval	*/
	unsigned short	min_weight; 	/* min. weight of approval	*/
} RZR111T;

#if	defined(STORAGE)
struct rzr111t	rzr111s;
#else
#if	!defined(NOEXTERN)
extern struct rzr111t	rzr111s;
#endif
#endif

#endif
