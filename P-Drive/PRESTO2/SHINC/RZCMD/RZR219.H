/*======================================================================
 * File name    : rzr219.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ce@nm^neeΤ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr219.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr219.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * ϊrW
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/RCS/rzr219.h 1.1 1995/07/19 18:44:37 hiroki
 *Log: rzr219.h
 * rW 1.1  1995/07/19  18:44:37  hiroki
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR219_H)
#define	RZR219_H


typedef struct rzr2191t {
	unsigned char 	on_time;	/* ce@nmΤ		*/
	unsigned char 	off_time;	/* ce@neeΤ		*/
} RZR2191T;

/*------------------------------------------------------*/
/*	@@ce@p[^pPbg		@@@@*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr219t {
	unsigned char	min_df_no;	/* Ε¬DFΤ			*/
	unsigned char	nof_df;		/* Y·ιCHΜDF		*/
	struct rzr2191t	df[8];		/* DF paramter[nof_df]		*/
} RZR219T;

#if	defined(STORAGE)
struct rzr219t	rzr219s;
#else
#if	!defined(NOEXTERN)
extern struct rzr219t	rzr219s;
#endif
#endif

#endif
