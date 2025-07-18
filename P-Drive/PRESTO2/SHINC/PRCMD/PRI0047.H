/*======================================================================
 * File name	: pri0047.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	`euWf[^\¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri0047.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri0047.h $
 * rW 1.1  1970/01/01  00:00:00  kawamura
 * úrW
 * 
 *----------------------------------------------------------------------
 *header: DZI203.Hv  1.2  94/02/18 16:42:12  ryoji  Exp
 *log: P:/INCLUDE/DZCMD/RCS/DZI203.Hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI0047_H)
#define	PRI0047_H

/*------------------------------------------------------*/
/*	AFVWp[^pPbg			*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri0047t {
	unsigned char	wh_no;		/* Y·éWHÔ		*/
	long		new_coeffi;	/* V`euW			*/
	long		old_coeffi;	/* `euW			*/
} PRI0047T;

#if	defined(STORAGE)
struct pri0047t	pri0047s;
#else
#if	!defined(NOEXTERN)
extern struct pri0047t	pri0047s;
#endif
#endif

#endif
