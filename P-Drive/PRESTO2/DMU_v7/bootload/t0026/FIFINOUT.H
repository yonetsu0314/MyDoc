/*======================================================================
 * File name    : fifinout.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		FIFO入出力関数プロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/fifinout.h 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fifinout.h $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * プログラム整理を行う
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/02/27 19:30
 * fif_buf_clear の追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/07 16:00
 * プロトタイプ宣言 get_fifstr の追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#ifndef	_FIFINOUT_H_
#define	_FIFINOUT_H_

#include	<stddef.h>	

#define ESC 	(0x1b)
#define EOF 	(-1)

void fif_buf_clear(void);
void fifoout(char c);
void fifolf(void);
void fifostr(char *str);
void fifostrb(char *str);
char fifochk(void);
char fifoin(void);
char *get_fifdata(void);
char *get_fifstr(void);


#endif	/* _FIFINOUT_H_ */
