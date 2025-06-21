/*======================================================================
 * File name    : rzr075.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＣＡパラメータコマンドデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr075.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr075.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr075.h 2.2 1996/07/12 18:49:54 hiroki
 *Log: rzr075.h
 * リビジョン 2.2  1996/07/12  18:49:54  hiroki
 * dca_owf修正
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR075_H)
#define	RZR075_H

enum rzr0750e {				/* DCA の ON/OFF		*/
	DCA_OFF,			/* OFF				*/
	DCA_ON,				/* ON                      	*/
};

/*------------------------------------------------------*/
/*	ＤＣＡパラメータコマンド構造パラメータパケット  */
/*							*/
/*------------------------------------------------------*/
typedef struct 	rzr075t {
	enum rzr0750e	dca_on_off;	/* DCA ON/OFF     		*/
	unsigned int	dca_cycle;	/* DCA cycle count  		*/
	unsigned char	dca_owf;	/* DCA OWF          		*/
	unsigned char	dca_sdm;	/* DCA SDM          		*/
} RZR075T;

#if	defined(STORAGE)
struct rzr075t	rzr075s;
#else
#if	!defined(NOEXTERN)
extern struct rzr075t	rzr075s;
#endif
#endif

#endif
