/*----------------------------------------------------------------------*
 * File name	: vfd.h	/ vfd bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/vfd.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vfd.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/vfd.h 1.1 1995/08/25 09:26:21 ryoji
 *Log: vfd.h
 * リビジョン 1.1  1995/08/25  09:26:21  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(VFD_H)
#define	VFD_H

/*	<<< return parameter >>>					*/

typedef struct vfdpinf {
	void		*iobase;	/* vfd physical address	*/
	unsigned short	mem_size;	/* size of memory		*/
} VFDPINF;

const VFDPINF *	_vfd_init( int );

#endif	/* VFD_H */

