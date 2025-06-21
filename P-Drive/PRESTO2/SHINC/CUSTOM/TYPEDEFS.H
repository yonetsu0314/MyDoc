/*----------------------------------------------------------------------*
	TYPEDEFS.H - General purpose definition file
	Copyright (C) 1990 Paradigm Systems.  All rights reserved.
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/custom/RCS/typedefs.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: typedefs.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/07/30 19:50
 * nortiのkernel.h 内でTRUE, FALSE が定義されているため、
 * BOOLEAN が条件コンパイルされるように変更する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*
	Function
	========
	This file contains the general purpose definitions common to the
	all Paradigm applications.  By defining synonyms for the physical
	data types to be manipulated, portability between memory models
	and machines is maximized.

	Note that this file follows the system include files and before
	any application include files.

	Variable Naming Conventions
	===========================
	The follow prefixes should be used to allow the type of a variable
	to be readily distinguishable:

			bVar			BYTE/BOOLEAN variable
			wVar			WORD variable (unsigned)
			dwVar			DWORD variable (unsigned)
			qwVar			QWORD variable (unsigned)
			fVar			An IEEE 32-bit float
			dVar			An IEEE 64-bit float
			pVar			Any pointer (near or far)
			fpVar			A far pointer
			npVar			A near pointer
			sVar			Structure
			szVar			ASCIIZ string

	The 'a' and 's' prefixes can be used with the above data types to
	distinguish staticlly declared arrays and structures.  For example,

			baVar
			sVar

	is a byte array and a structure.


	Date		Who	Action
	========	===	=========================================
	03/16/89	rwn	Created initial file
*/

#if	!defined(_TYPEDEFS)
#define	_TYPEDEFS

/*
	Define the types to be used to manipulate 8-, 16-, 32- and 64-bit
	data.
*/
#if	!defined(__TURBOC__)
typedef	unsigned char	BIT ;	/* Use for defining MSC bit fields */
#else
typedef	unsigned 	BIT ;	/* Use for defining Turbo C bit fields */
#endif	/* __TURBOC__ */

typedef	unsigned char	BYTE ;		/* 8-bit data */
typedef	unsigned short	WORD ;		/* 16-bit data */
typedef	unsigned long	DWORD ;		/* 32-bit data */
typedef	double		QWORD ;		/* 64-bit data */

typedef	float		FLOAT ;		/* 32-bit floating point data */
typedef	double		DOUBLE ;	/* 64-bit floating point data */

typedef	void *		PTR ;		/* Pointer to any data type */
typedef	BYTE *		BYTE_PTR ;	/* Pointer to 8-bit data */
typedef	WORD *		WORD_PTR ;	/* Pointer to 16-bit data */
typedef	DWORD *		DWORD_PTR ;	/* Pointer to 32-bit data */
typedef	QWORD *		QWORD_PTR ;	/* Pointer to 64-bit data */

/*
	Define the boolean data type
*/
#if	!( defined(FALSE) || defined(TRUE) )
typedef	enum { FALSE = 0, TRUE } BOOLEAN ;
#endif


/*
	Define some common macros that show up everywhere.  These are

		lowbyte(x)		Extract the low order byte of WORD x
		highbyte(x)		Extract the high order byte of WORD x
		dim(x)			Computes the dimension of an array
*/
#define	lowbyte(word)		((word) & 0xff)
#define	highbyte(word)		lowbyte((word) >> 8)
#define	dim(x)			(sizeof(x) / sizeof(x[0]))

#if	!defined(min)
#define max(a,b)	(((a) > (b)) ? (a) : (b))
#define min(a,b)	(((a) < (b)) ? (a) : (b))
#endif

/*
	This macro is used to install an interrupt vector entry directly.
*/
#if	defined(__TURBOC__)
#define	setvect(inum, addr) *(void interrupt far (far * far *)())((inum) * 4) = (addr)
#else	/* Must be Microsoft C */
#define MK_FP(seg,ofs)	((void far *) (((unsigned long)(seg) << 16) | (unsigned)(ofs)))
#define	setvect(inum, addr) *((DWORD_PTR) ((inum) * 4)) = ((DWORD) (addr))
#define	enable()	_enable()
#define	disable()	_disable()
#endif

#endif	/* _TYPEDEFS */
