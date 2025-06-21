/*======================================================================
 * File name	: pri1013.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ó‘Ôƒf[ƒ^Šg’£ƒpƒ‰ƒ[ƒ^\‘¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/pri1013.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: pri1013.h $
 * ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  kawamura
 * ‰ŠúƒŠƒrƒWƒ‡ƒ“
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzi113a.h 1.1 1996/07/26 11:23:49 hiroki Exp
 *log: rzi113a.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2004, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRI1013_H)
#define	PRI1013_H
/*------------------------------------------------------*/
/*	‚`‚s‚`@‚s‚`‚q‚f‚d‚s				*/
/*							*/
/*------------------------------------------------------*/

typedef struct pri1013t {
	unsigned char	nof_ch;		/* number of channel		*/
	unsigned long	ata_target[4];	/* ata target			*/
} PRI1013T;

#if	defined(STORAGE)
struct pri1013t	pri1013s;
#else
#if	!defined(NOEXTERN)
extern struct pri1013t	pri1013s;
#endif
#endif

#endif
