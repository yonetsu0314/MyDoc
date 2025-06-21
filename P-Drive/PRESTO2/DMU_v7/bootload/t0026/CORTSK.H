/*======================================================================
 * File name    : cortsk.h
 * Original		: p:/dacs_i/rcu/main/rcs/cortsk.h 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		CON へのテキスト受信タスクのプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/cortsk.h 2.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cortsk.h $
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * プログラム整理を行う
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef _CORTSK_H_
#define _CORTSK_H_

#define INT_BUF_size  128*1024

#ifdef	STORAGE
union aap con0_rcv_tsk_aa;
union aap con1_rcv_tsk_aa;
char		int_buf0[INT_BUF_size];
char		*int_buf0_in;
char		*int_buf0_out;
unsigned int	int_buf0_count;
unsigned int	int_buf0_cntout;
unsigned char	int_buf0_ovfl;
char		int_buf1[INT_BUF_size];
char		*int_buf1_in;
char		*int_buf1_out;
unsigned int	int_buf1_count;
unsigned int	int_buf1_cntout;
unsigned char	int_buf1_ovfl;
#else
extern union aap con0_rcv_tsk_aa;
extern union aap con1_rcv_tsk_aa;
extern char		int_buf0[INT_BUF_size];
extern char		*int_buf0_in;
extern char		*int_buf0_out;
extern unsigned int	int_buf0_count;
extern unsigned int	int_buf0_cntout;
extern unsigned char	int_buf0_ovfl;
extern char		int_buf1[INT_BUF_size];
extern char		*int_buf1_in;
extern char		*int_buf1_out;
extern unsigned int	int_buf1_count;
extern unsigned int	int_buf1_cntout;
extern unsigned char	int_buf1_ovfl;
#endif

/*
//	関数ﾌﾟﾛﾄﾀｲﾌﾟ
*/
int setup_con0_rcv_tsk(void);
int setup_con1_rcv_tsk(void);

int chk_buf0_len(void);
int chk_buf1_len(void);

void con0_buf_clear(void);
void con1_buf_clear(void);

#endif
