/*----------------------------------------------------------------------*
 * File name	: iofsys.h	/ i/o driver funtion table		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/iofsys.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: iofsys.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(IOFSYS_H)
#define	IOFSYS_H

#ifdef	STORAGE
int	(*iodrv_open[__KEY_MAX_-1])(int, int) = { (void *)0 };
int	(*iodrv_read[__KEY_MAX_-1])(int, unsigned char *, int) = { (void *)0 };
int	(*iodrv_write[__KEY_MAX_-1])(int, unsigned char *, int) = { (void *)0 };
int	(*iodrv_close[__KEY_MAX_-1])(int) = { (void *)0 };
int	(*iodrv_seek[__FILE_KEY_MAX_])(int, int, int) = { (void *)0 };
#else
extern int	(*iodrv_open[])(int, int);
extern int	(*iodrv_read[])(int, unsigned char *, int);
extern int	(*iodrv_write[])(int, unsigned char *, int);
extern int	(*iodrv_close[])(int);
extern int	(*iodrv_seek[])(int, int, int);
#endif

#endif	/* IOFSYS_H */

