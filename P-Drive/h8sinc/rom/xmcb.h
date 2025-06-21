/*----------------------------------------------------------------------*
 * File name	: xmcb.h	/ define MCB								*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/rom/RCS/xmcb.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: xmcb.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 * このファイルは p:/h8inc/rom/rcs/xmcb.h 1.1 と同じ
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*	Purpose:															*/
/*																		*/
/*	The following structure is the memory control block (MCB) used to	*/
/*	build a list of free blocks available for allocation.  Each MCB		*/
/*	contains two fields, a pointer to the next free block and the		*/
/*	size of this free block.											*/
/*----------------------------------------------------------------------*/

typedef	struct	_MCB   {
	struct _MCB	*pNext ;			/* Pointer to the next free block 	*/
	WORD		wLen ;				/* Size of this free block 			*/
} MCB, *MCBP ;						/* Memory control block definition 	*/

#ifdef	HEAPSIZE
#define	HEAPLEN	(HEAPSIZE /sizeof(MCB))			/* Heap size in MCBs 	*/

const WORD	_no_MCB = HEAPLEN;		/* maximun no of MCBs				*/
MCBP		pFirst ;				/* Handle to the first free block	*/
MCB 		_DMMBuf[HEAPLEN] ;		/* Memory used for allocation		*/
#else

extern const WORD	_no_MCB ;
extern	MCBP		pFirst ;
extern	MCB 		_DMMBuf[] ;
#endif
