/*======================================================================
 * File name	: pri0021.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	動作完了データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0021.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0021.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzi021.hv  2.1  93/11/02 15:40:54  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzi021.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0021_H)
#define	PRI0021_H


/*------------------------------------------------------*/
/*	動作完了のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0021t {
	unsigned char	command;	/* 受信コマンド			*/
	unsigned char	ch_id;		/* channel number		*/
} PRI0021T;

#if	defined(STORAGE)
struct pri0021t	pri0021s;
#else
#if	!defined(NOEXTERN)
extern struct pri0021t	pri0021s;
#endif
#endif

#endif
