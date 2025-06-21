/*----------------------------------------------------------------------*
 * File name	: init.h	/ init bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/rcs/init.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: init.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 04.12.11 _init_jump_to_reset追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/05/12 12:10
 * プロトタイプ宣言_init_vectorの追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(INIT_H)
#define	INIT_H

int		_init_registers(void);
int		_init_section(void);
int		_init_vector(void);
void	_init_jump_to_reset(void);
#endif
