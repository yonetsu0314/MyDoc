/*======================================================================
 * File name    : fifinout.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		FIFO入出力関数プロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/fifinout.h 2.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifinout.h $
 * リビジョン 2.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
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

#define FIF_BUF_size  128*1024

#undef	EXTERN
#ifdef _FIFINOUT_C_
#define	EXTERN
#else
#define	EXTERN	extern
#endif

EXTERN char		fif_buf[FIF_BUF_size];
EXTERN char		*fif_buf_in;
EXTERN char		*fif_buf_out;
EXTERN unsigned int	fif_buf_count;
EXTERN unsigned int	fif_buf_cntout;
EXTERN unsigned char	fif_buf_ovfl;

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
