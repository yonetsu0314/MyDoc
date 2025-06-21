/*======================================================================
 * File name	: prr0033.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	零点調整データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0033.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0033.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: rzr033.hv  2.1  93/11/02 15:42:20  ryoji  Exp
 *log: P:/INCLUDE/RZCMD/RCS/rzr033.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0033_H)
#define	PRR0033_H


/*------------------------------------------------------*/
/*	零点調整パラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0033t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	adj_head_ptn;	/* 調整ヘッドビットパターン	*/
	unsigned char	adj_df_ptn;	/* 調整DFビットパターン		*/
	unsigned char	init_zero_flag;	/* 0:通常零点flag 1:初期零点flag*/
	unsigned short	ad_coef;	/* AD係数			*/
	unsigned short	da_coef;	/* DA係数			*/
	unsigned short	range;		/* 秤量				*/
} PRR0033T;

#if	defined(STORAGE)
struct prr0033t	prr0033s;
#else
#if	!defined(NOEXTERN)
extern struct prr0033t	prr0033s;
#endif
#endif

#endif
