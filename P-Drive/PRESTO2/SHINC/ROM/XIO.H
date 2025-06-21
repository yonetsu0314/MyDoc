/*----------------------------------------------------------------------*
 * File name	: xio.h		/ low-level file handling & I/O func.	*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/rom/RCS/xio.h 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: xio.h $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/07/24 16:30 前リビジョンの不具合修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/07/24 16:15
 * 実際の関数と引き数を合わす。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* Purpose:								*/
/*	This file contains the function declarations for the low-level	*/
/*	file handling and I/O functions.				*/
/*----------------------------------------------------------------------*/


#if	!defined(XIO_H)
#define	XIO_H

/* function prototypes */

int	close(int);
int	open(unsigned char *, int);
int	read(int, void *, int);
int	seek(int, int, int);
int	write(int, void *, int);

#endif	/* XIO_H */

