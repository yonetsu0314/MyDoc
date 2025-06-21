/*----------------------------------------------------------------------*
 * File name	: iofsys.h		/ I/O driver funtion table				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/iofsys.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: iofsys.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/

#ifdef	STORAGE
int		(*iodrv_open[__KEY_MAX_-1])(int, int)			= { (void *)0 };
int		(*iodrv_read[__KEY_MAX_-1])(int, unsigned char *, int)	= { (void *)0 };
int		(*iodrv_write[__KEY_MAX_-1])(int, unsigned char *, int)	= { (void *)0 };
int		(*iodrv_close[__KEY_MAX_-1])(int)				= { (void *)0 };
int		(*iodrv_seek[__FILE_KEY_MAX_])(int, int, int)	= { (void *)0 };
#else
extern int	(*iodrv_open[])(int, int);
extern int	(*iodrv_read[])(int, unsigned char *, int);
extern int	(*iodrv_write[])(int, unsigned char *, int);
extern int	(*iodrv_close[])(int);
extern int	(*iodrv_seek[])(int, int, int);
#endif
