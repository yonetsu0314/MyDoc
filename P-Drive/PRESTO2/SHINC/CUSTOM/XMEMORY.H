/*----------------------------------------------------------------------*
 * File_name	: xmemory.h	/ momery manipulete fucntion		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/custom/RCS/xmemory.h 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: xmemory.h $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/12 誤記修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/08/12 実際の引き数にプロトタイプにあわせる。
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

#if	!defined(XMEMORY_H)
#define	XMEMORY_H

/* function prototypes */

int		verify_bitrd( unsigned char, volatile unsigned char *);
int		verify_bitwr( unsigned char, int, volatile unsigned char *);
int	 	verify_getc( volatile unsigned char *);
int 		verify_putc( unsigned char, volatile unsigned char *);
long 		verify_geti( volatile unsigned short *);
int 		verify_puti( unsigned short, volatile unsigned short *);
void *		verify_fmemcpy( void *, void *, int);
void *		verify_blk_fmemcpy( void *, void *, int);

#endif	/* XMEMORY_H */

