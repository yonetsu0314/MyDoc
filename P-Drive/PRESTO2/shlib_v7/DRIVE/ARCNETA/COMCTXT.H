/*----------------------------------------------------------------------*
 * File name	: comctxt.h	/ aec-net context header		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcneta/RCS/comctxt.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: comctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comctxt.h 1.1 1995/11/07 17:13:16 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(COMCTXT_H)
#define	COMCTXT_H

/*------------------------------------------------------*/
/*	ARC-NETコントローラのコンテキスト構造体		*/
/*							*/
/*------------------------------------------------------*/

#define	FI_COM	0x20				/* Finish interrupt	*/

#ifdef	STORAGE				/* before include "comctl.h"	*/
struct com_ctxt	com_ctxt_tbl[COM_DEV_MAX] = {
			0, 0,
			(unsigned char)0, (unsigned char)0,
			(unsigned char)0, (unsigned char)0,
			(unsigned char)0, (unsigned char)0,
			(unsigned char)0,
			(unsigned char)0
};
#else
extern struct com_ctxt	com_ctxt_tbl[];
#endif

#endif	/* COMCTXT_H */

