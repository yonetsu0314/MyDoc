/*======================================================================
 * File name	: prr0075.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡパラメータコマンドデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0075.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0075.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzr075.h 2.2 1996/07/12 18:49:54 hiroki Exp
 *log: rzr075.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0075_H)
#define	PRR0075_H

enum prr00750e {			/* DCA の ON/OFF		*/
	DCA_OFF,			/* OFF				*/
	DCA_ON,				/* ON                      	*/
};

/*------------------------------------------------------*/
/*	ＤＣＡパラメータコマンド構造パラメータパケット  */
/*							*/
/*------------------------------------------------------*/
typedef struct 	prr0075t {
	enum prr00750e	dca_on_off;	/* DCA ON/OFF     		*/
	unsigned short	dca_cycle;	/* DCA cycle count  		*/
	unsigned char	dca_owf;	/* DCA OWF          		*/
	unsigned char	dca_sdm;	/* DCA SDM          		*/
} PRR0075T;

#if	defined(STORAGE)
struct prr0075t	prr0075s;
#else
#if	!defined(NOEXTERN)
extern struct prr0075t	prr0075s;
#endif
#endif

#endif
