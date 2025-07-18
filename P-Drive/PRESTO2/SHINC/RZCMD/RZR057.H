/*======================================================================
 * File name    : rzr057.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	`ecZNVf[^\¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr057.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr057.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * úrW
 * 
 *----------------------------------------------------------------------
 *Header: rzr057.hv  1.1  93/11/02 17:16:20  ryoji
 *Log: P:/INCLUDE/RZCMD/RCS/rzr057.hv
 * Revision 1.1  93/11/02 17:16:20  ryoji
 * æTÅÎ
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR057_H)
#define	RZR057_H


typedef struct rzr0570t{
	unsigned char	ctrl_mode;	/* AFD[h			*/
	unsigned short	ctrl_sw;	/* AFD®ìrv			*/
	unsigned short	start_delay;	/* §äJnxêTCN	*/
	unsigned char	df_rf_ratio;	/* DF-RFUä			*/
	unsigned char	target_head;	/* ÚWIðwbh		*/
	unsigned char	amp_center;	/* RFU§äSl		*/
	unsigned char	amp_max;	/* UâÎãÀ			*/
	unsigned char	amp_min;	/* UâÎºÀ			*/
	unsigned char	amp_range;	/* U§äÍÍ			*/
} RZR0570T;

/*------------------------------------------------------*/
/*	`ecwbhf[^p[^pPbg		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr057t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	unsigned char	nof_sect;	/* Y·échÌZNV	*/
	struct rzr0570t	sect[8];	/* AFD paramter[nof_sect]	*/
} RZR057T;

#if	defined(STORAGE)
struct rzr057t	rzr057s;
#else
#if	!defined(NOEXTERN)
extern struct rzr057t	rzr057s;
#endif
#endif

#endif
