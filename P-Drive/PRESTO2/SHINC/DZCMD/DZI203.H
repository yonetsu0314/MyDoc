/*======================================================================
 * File name    : dzi203.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	`euWf[^\¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzi203.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzi203.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * úrW
 * 
 *----------------------------------------------------------------------
 *Header: DZI203.Hv  1.2  94/02/18 16:42:12  ryoji
 *Log: P:/INCLUDE/DZCMD/RCS/DZI203.Hv
 * Revision 1.2  94/02/18 16:42:12  ryoji
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZI203_H)
#define	DZI203_H

/*------------------------------------------------------*/
/*	AFVWp[^pPbg			*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzi203t {
	unsigned char	wh_no;		/* Y·éWHÔ		*/
	long		new_coeffi;	/* V`euW			*/
	long		old_coeffi;	/* `euW			*/
} DZI203T;

#if	defined(STORAGE)
struct dzi203t	dzi203s;
#else
#if	!defined(NOEXTERN)
extern struct dzi203t	dzi203s;
#endif
#endif

#endif
