/*----------------------------------------------------------------------*
 * File name	: exmdrv.h	/ ex.ram driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/exmdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: exmdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(EXMDRV_H)
#define	EXMDRV_H

int		exm_sys_init(void);
int		exbuf_open(int, int);
int		exbuf_close(int);
int		exbuf_read(int, void *, int);
int		exbuf_write(int, void *, int);
int		exbuf_flush(int);

#endif	/* EXMDRV_H */

