/*----------------------------------------------------------------------*
 * File name	: typedefs.h		/ General purpose definition file	*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/custom/RCS/typedefs.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: typedefs.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 * このファイルは p:/h8inc/custom/RCS/typedefs.h 1.3 と同じ
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993-2001, ISHIDA  CO., LTD.				*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/*	Function															*/
/*	========															*/
/*	This file contains the general purpose definitions common to the	*/
/*	all Paradigm applications.  By defining synonyms for the physical	*/
/*	data types to be manipulated, portability between memory models		*/
/*	and machines is maximized.											*/
/*																		*/
/*	Note that this file follows the system include files and before		*/
/*	any application include files.										*/
/*																		*/
/*	Variable Naming Conventions											*/
/*	===========================											*/
/*	The follow prefixes should be used to allow the type of a variable	*/
/*	to be readily distinguishable:										*/
/*																		*/
/*			bVar			BYTE/BOOLEAN variable						*/
/*			wVar			WORD variable (unsigned)					*/
/*			dwVar			DWORD variable (unsigned)					*/
/*			qwVar			QWORD variable (unsigned)					*/
/*			fVar			An IEEE 32-bit float						*/
/*			dVar			An IEEE 64-bit float						*/
/*			pVar			Any pointer (near or far)					*/
/*			sVar			Structure									*/
/*			szVar			ASCIIZ string								*/
/*																		*/
/*	The 'a' and 's' prefixes can be used with the above data types to	*/
/*	distinguish staticlly declared arrays and structures.  For example,	*/
/*																		*/
/*			baVar														*/
/*			sVar														*/
/*																		*/
/*	is a byte array and a structure.									*/
/*----------------------------------------------------------------------*/

#if	!defined(_TYPEDEFS)
#define	_TYPEDEFS

/*
//	Define the types to be used to manipulate 8-, 16-, 32- and 64-bit
//	data.
*/

typedef	unsigned char	BYTE ;		/* 8-bit data */
typedef	unsigned short	WORD ;		/* 16-bit data */
typedef	unsigned long	DWORD ;		/* 32-bit data */
typedef	double			QWORD ;		/* 64-bit data */

typedef	float		FLOAT ;			/* 32-bit floating point data */
typedef	double		DOUBLE ;		/* 64-bit floating point data */

typedef	void *	PTR ;				/* Pointer to any data type */
typedef	BYTE *	BYTE_PTR ;			/* Pointer to 8-bit data */
typedef	WORD *	WORD_PTR ;			/* Pointer to 16-bit data */
typedef	DWORD *	DWORD_PTR ;			/* Pointer to 32-bit data */
typedef	QWORD *	QWORD_PTR ;			/* Pointer to 64-bit data */

/*
//	Define a number of implementation dependent values
*/
#define		BYTE_MIN		(0u)
#define		BYTE_MAX		(255u)
#define		WORD_MIN		(0u)
#define		WORD_MAX		(65535u)
#define		DWORD_MIN		(0u)
#define		DWORD_MAX		(4294967295u)

/*
//	Define the boolean data type
*/
typedef	enum { FALSE = 0, TRUE } BOOLEAN ;


/*
//	Define some common macros that show up everywhere.  These are
//
//		lowbyte(x)		Extract the low order byte of WORD x
//		highbyte(x)		Extract the high order byte of WORD x
//		dim(x)			Computes the dimension of an array
*/
#define	lowbyte(word)		((word) & 0xff)
#define	highbyte(word)		lowbyte((word) >> 8)
#define	dim(x)				(sizeof(x) / sizeof(x[0]))

#if	!defined(min)
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif


#endif	/* _TYPEDEFS */
