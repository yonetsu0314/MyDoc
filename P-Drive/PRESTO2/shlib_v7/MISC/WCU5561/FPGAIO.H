/*======================================================================
 * File name	: fpgafifo.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/wcu5561/rcs/fpgaio.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaio.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * int_mask()関数追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

void			int_mask(int);

int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);

