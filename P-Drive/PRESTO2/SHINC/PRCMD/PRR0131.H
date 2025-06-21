/*======================================================================
 * File name	: prr0131.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	予約番号データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0131.h 1.2 1970/01/01 00:00:00 hiroe Exp $
 *$Log: prr0131.h $
 * リビジョン 1.2  1970/01/01  00:00:00  hiroe
 * 2004/03/11
 * defineを修正する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0131_H)
#define	PRR0131_H


/*--------------------------------------------------------------*/
/*	予約番号パラメータパケット				*/
/*								*/
/*--------------------------------------------------------------*/

typedef struct prr0131t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned short	preset_no;	/* preset number		*/
} PRR0131T;

#if	defined(STORAGE)
struct prr0131t	prr0131s;
#else
#if	!defined(NOEXTERN)
extern struct prr0131t	prr0131s;
#endif
#endif

#endif
