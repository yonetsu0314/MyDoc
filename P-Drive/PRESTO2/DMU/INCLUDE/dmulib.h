/*----------------------------------------------------------------------*
 * File name	: dmu.h						*
 *----------------------------------------------------------------------*
 *$Header$
 *$Log$
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DMULIB_H)
#define	DMULIB_H

int	chkctype(unsigned char c, int type);
int	dipsw(void);
int	dipsw2(void);
int	isDebugSW(void);

#endif	/* DMULIB_H */

