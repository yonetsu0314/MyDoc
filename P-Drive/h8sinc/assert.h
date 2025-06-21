/*----------------------------------------------------------------------*
 * File name	: assert.h		/ define the assert macro				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8inc/RCS/assert.h 1.2 1996/08/22 12:11:10 minaguchi Exp $
 *$Log: assert.h $
 * リビジョン 1.2  1996/08/22  12:11:10  minaguchi
 * assertマクロで、_assert関数を呼び出すときの引き数をキャストする。
 * 
 * リビジョン 1.1  1996/01/19  11:04:11  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* Purpose:																*/
/*		Defines the assert(exp) macro.									*/
/*		[ANSI/System V]													*/
/*----------------------------------------------------------------------*/

#if defined(_DLL) && !defined(_MT)
#error Cannot define _DLL without _MT
#endif

#ifdef _MT
#define _FAR_ _far
#else
#define _FAR_
#endif

#undef	assert

#ifdef NDEBUG

#define assert(exp)	((void)0)

#else

void _FAR_	 _assert(void _FAR_ *, void _FAR_ *, unsigned);
#define assert(exp) \
	( (exp) ? (void) 0 : _assert((void _FAR_ *)#exp, (void _FAR_ *)__FILE__, (unsigned)__LINE__) )

#endif /* NDEBUG */
