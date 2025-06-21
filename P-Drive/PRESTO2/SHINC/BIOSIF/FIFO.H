/*----------------------------------------------------------------------*
 * File name	: init.h	/ init bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/rcs/fifo.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fifo.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * FPGAﾊﾞｰｼﾞｮﾝ確認関数追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 04.12.11 DMUのMASK関数新規追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04.12.09 新規追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(FIFO_H)
#define	FIFO_H

unsigned char	_fifo_input(void);
unsigned char	_fifo_status(void);
void			_fifo_output(unsigned char);
void			_fifo_msktxrx(unsigned short flag);
int				_fifo_init(void);
int				_fifo_full(void);
void			_wcu_int_mask(int);
void			_dmu_int_mask(int);
void			_dmu_pint_mask(int);
int				_make_FPGA_verstr(char *verstr);

#endif
