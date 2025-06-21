/*----------------------------------------------------------------------*
 * File name	: dmm.c													*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/dmm.c 1.2 1996/03/06 08:51:41 ohtani Exp $
 *$Log: dmm.c $
 * リビジョン 1.2  1996/03/06  08:51:41  ohtani
 * malloc() : bug-fix list data structure
 * 
 * リビジョン 1.1  1996/01/19  09:43:27  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*	Function															*/
/*	========															*/
/*	This module contains the code for using dynamic memory in an		*/
/*	embedded application.  The size of the heap is limited by the size	*/
/*	of DGROUP in the small and medium models and 64KB in all other		*/
/*	models.  The size of the heap used for allocation is set by the		*/
/*	HEAPSIZE macro.														*/
/*																		*/
/*	The typical application of this code is to satisfy run-time library	*/
/*	routines which assume the availablility of dynamically allocated	*/
/*	memory. Since this typically requires a small amount of dynamically	*/
/*	allocated memory, this module initializes itself on the first call	*/
/*	to malloc().														*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>					/* Definition of NULL			*/
#include	<string.h>					/* Memory manipulation functions*/
#include	<custom\typedefs.h>			/* Common typedefs and macros 	*/
#include	<rom\xmcb.h>				/* define memory control block	*/


/*
//	Function:	free
//
//	This function is a ROMable replacement for the run-time library
//	version of this function.
*/

void	free(
	void	*pBlock					/* Handle to the block to be released */
	)
{
	MCBP		pPrev;				/* Previous referenced MCB */
	MCBP		pNext;				/* Next referenced MCB */
	MCBP		pMCB;				/* Current MCB in the free chain */

	/* Get a pointer to the block handle */
	pMCB = (MCBP) pBlock - 1 ;

	/* Check if returning a block ahead of the first free block */
	if (pFirst > pMCB)   {
		pNext = pFirst ;
		pFirst = pMCB ;
		pPrev = pMCB + pMCB->wLen ;

		/* Check if we can combine with the next block */
		if (pPrev == pNext)   {
			pMCB->wLen += pNext->wLen ;
			pMCB->pNext = pNext->pNext ;
		}
		else
			pMCB->pNext = pNext ;

		goto  escp;
	}

	/* Scan the free list to insert the returned block */
	for (pNext = pFirst; pNext != NULL && pNext < pMCB; pPrev = pNext, pNext = pNext->pNext)   {
		if (pNext + pNext->wLen == pMCB)   {
			/* Add the newly returned block to the front of the list */
			pNext->wLen += pMCB->wLen ;
			pMCB = pNext + pNext->wLen ;

			/* Check if it can also include the next block in the list */
			if (pMCB == pNext->pNext)   {
				pNext->wLen += pMCB->wLen ;
				pNext->pNext = pMCB->pNext ;
			}
			/* All done */
			goto  escp;
		}
	}

	/* Must add the returned block to the end of the free list */
	pPrev->pNext = pMCB ;
	pPrev = pMCB + pMCB->wLen ;

	/* Check if combined with the last block in the free list */
	if (pPrev == pNext)   {
		pMCB->wLen += pNext->wLen ;
		pMCB->pNext = pNext->pNext ;
	}
	else
		pMCB->pNext = pNext ;
escp:
	return ;
}

/*
//	Function:	malloc
//
//	This function is a ROMable replacement for the run-time library
//	version of this function.
*/

void	*malloc(
	size_t	wSize					/* Size of block to allocate */
	)
{
	MCBP		pPrev ;				/* Previous referenced MCB */
	MCBP		pMCB ;				/* Current MCB in the free chain */
	WORD		wBlocks ;			/* Size of the needed block in MCBs */
	void		*rmp = NULL;
	int		err =0;

	/* Check for a frivolous request */
	if (wSize == 0)								return(NULL);

	/* Self-initialization code */
	if (pFirst == NULL)   {
		pFirst = _DMMBuf ;
		pFirst->pNext = NULL ;
		pFirst->wLen = _no_MCB - 1 ;
	}

	/* Compute the size of the needed block in units of MCBs */
	wBlocks = (wSize - 1) / sizeof(MCB) + 2 ;

	/* Scan the free list for a free block meeting the size */
	for(pPrev = NULL, pMCB = pFirst ; pMCB != NULL ;
	    pPrev = pMCB, pMCB = pMCB->pNext)   {
		/* Check the size of the block */
		if (pMCB->wLen >= wBlocks)   {
			/* Check if an exact fit */
			if (pMCB->wLen > wBlocks)   {
				/* Remove the allocated block from the larger free block */
				pMCB->wLen -= wBlocks ;

				/* Create a new MCB for the split block */
				pMCB += pMCB->wLen ;
				pMCB->wLen = wBlocks ;
			}
			else   {
				if (pPrev == NULL)
					/* Move the free list pointer to the next free block */
					pFirst = pMCB->pNext ;
				else
					/* Unlink the block from the free list */
					pPrev->pNext = pMCB->pNext ;
			}

			/* Return the address of the data portion of the block */
			rmp = (pMCB + 1);
			break;
		}
	}
	return(rmp);
}

/*
//	Function:	calloc
//
//	This function is a ROMable replacement for the run-time library
//	version of this function.
*/

void	*calloc(
	size_t	wItems,					/* Number of items */
	size_t	wSize					/* Size of block to allocate */
	)
{
	DWORD		dwSize ;			/* Size of memory to allocate */
	char		*pBuf ;				/* Pointer to the buffer */

	/* Compute the size of the buffer to allocate */
	dwSize = (DWORD) wItems * wSize ;
	pBuf = (dwSize > 0xFFFF) ? NULL : malloc((size_t) dwSize) ;

	/* Clear the newly allocated buffer */
	if (pBuf != NULL)
		memset(pBuf, 0, (WORD) dwSize) ;

	return	pBuf ;
}

/*
//	Function:	realloc
//
//	This function is a ROMable replacement for the run-time library
//	version of this function.
*/

void	*realloc(
	void	*pOldBlk,				/* Handle to the original block */
	size_t	wNewSize				/* Size of new block to allocate */
	)
{
	WORD		wOldSize ;			/* Size of the original block */
 	MCBP		pOldMCB ;			/* Old MCB handle */
 	void		*pNewBlk ;			/* Pointer to the new block */

	/* Extract the original block length from the MCB */
	pOldMCB = (MCBP) pOldBlk - 1 ;
	wOldSize = pOldMCB->wLen * sizeof(MCB) ;  

	/* Allocate a new block and copy the original contents */
	if ((pNewBlk = malloc(wNewSize)) != NULL)   {
		memcpy(pNewBlk, pOldBlk, wOldSize < wNewSize ? wOldSize : wNewSize) ;
		free(pOldBlk) ;
	}

	return	pNewBlk ;
}
