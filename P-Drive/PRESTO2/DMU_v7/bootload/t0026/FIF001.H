/*======================================================================
 * File name    : fif001.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	fifへのテキスト送信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/fif001.h 2.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fif001.h $
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * プログラム整理を行う
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		_FIF001_H_
#define		_FIF001_H_

#define FIF_BUF_size  128*1024

#ifdef	STORAGE
union aap fif_rcv_tsk_aa;
char		fif_buf[FIF_BUF_size];
char		*fif_buf_in;
char		*fif_buf_out;
unsigned int	fif_buf_count;
unsigned int	fif_buf_cntout;
unsigned char	fif_buf_ovfl;
#else
extern union aap fif_rcv_tsk_aa;
extern char		fif_buf[FIF_BUF_size];
extern char		*fif_buf_in;
extern char		*fif_buf_out;
extern unsigned int	fif_buf_count;
extern unsigned int	fif_buf_cntout;
extern unsigned char	fif_buf_ovfl;
#endif

int setup_fif_rcv( void );
void fif_buf_clear(void);

#endif
