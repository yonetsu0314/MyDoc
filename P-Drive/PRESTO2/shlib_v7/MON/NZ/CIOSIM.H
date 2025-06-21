/*----------------------------------------------------------------------*/
/* File name	: ciosim.h	/ con-i/o simulate driver		*/
/*----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/ciosim.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: ciosim.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
/*----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/ciosim.h 1.1 1996/07/22 19:26:08
 *Log: ciosim.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(_CIO_SIM)
#define	_CIO_SIM

#define	con_puts( buff, byte)\
	printf("%s", buff)

#ifdef	stdin
#define	con_gets( buff, byte)\
	fgets( buff, byte, stdin)
#endif

#endif
