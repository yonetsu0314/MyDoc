/*----------------------------------------------------------------------*
 * File name	: xmcb.h	/ define MCB				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/rom/RCS/xmcb.h 1.2 1970/01/01 00:00:00 kawamura Exp $
 *$Log: xmcb.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2003/07/30  10:52
 * ワーニング対策
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

#if	!defined(XMCB_H)
#define	XMCB_H

/*
//	The following structure is the memory control block (MCB) used to
//	build a list of free blocks available for allocation.  Each MCB
//	contains two fields, a pointer to the next free block and the
//	size of this free block.
*/

typedef	struct	_MCB   {
	struct _MCB	*pNext ;	/* Pointer to the next free block */
	WORD		wLen ;		/* Size of this free block */
} MCB, *MCBP ;				/* Memory control block definition */

#ifdef	HEAPSIZE
#define	HEAPLEN	(HEAPSIZE /sizeof(MCB))	/* Heap size in MCBs */

const WORD	_no_MCB = (WORD)HEAPLEN;/* maximun no of MCBs */
MCBP		pFirst ;		/* Handle to the first free block */
MCB 		_DMMBuf[HEAPLEN] ;	/* Memory used for allocation */
#else

extern const WORD	_no_MCB ;
extern	MCBP		pFirst ;
extern	MCB 		_DMMBuf[] ;
#endif

#endif	/* XMCB_H */

