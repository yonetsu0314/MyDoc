/*----------------------------------------------------------------------*
 * File name	: enhxmcb.h	/ define MCB	(Enhanced)			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/rom/rcs/enhxmcb.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(ENHXMCB_H)
#define	ENHXMCB_H

/*----------------------------------------------------------------------*/
/* 64KB以上のメモリサイズを取り扱えるようにxmcb.hを編集したソース        */
/*----------------------------------------------------------------------*/

/*
//	The following structure is the memory control block (MCB) used to
//	build a list of free blocks available for allocation.  Each MCB
//	contains two fields, a pointer to the next free block and the
//	size of this free block.
*/

typedef	struct	_MCB   {
	struct _MCB	*pNext ;	/* Pointer to the next free block */
	DWORD		dwLen ;		/* Size of this free block */
} MCB, *MCBP ;				/* Memory control block definition */

#ifdef	HEAPSIZE
#define	HEAPLEN	(HEAPSIZE /sizeof(MCB))	/* Heap size in MCBs */

const DWORD	_no_MCB = HEAPLEN;/* maximun no of MCBs */
MCBP		pFirst ;		/* Handle to the first free block */
MCB 		_DMMBuf[HEAPLEN] ;	/* Memory used for allocation */
#else

extern const DWORD	_no_MCB ;
extern	MCBP		pFirst ;
extern	MCB 		_DMMBuf[] ;
#endif

#endif	/* ENHXMCB_H */

