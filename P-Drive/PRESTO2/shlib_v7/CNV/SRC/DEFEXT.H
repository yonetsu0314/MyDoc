/*======================================================================
 * File name	: defext.h	/					*
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/defext.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: defext.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2005, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
*/
#if	!defined(DEFEXT_H)
#define	DEFEXT_H

typedef struct defext {
	void		(*tsk_adr)(int code);		/* 終了処理ルーチン	*/
	ID		tskid;				/* 定義するタスクＩＤ	*/
	int		init_code;			/* 初期値		*/
} DEFEXT;

#define	DEF_MAX		(64)

#ifdef	STORAGE
struct defext	def[DEF_MAX];
#else
extern struct defext	def[DEF_MAX];
#endif

#endif	/* DEFEXT_H */
