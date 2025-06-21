/*======================================================================
 * File name	: fpgafifo.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/dmu5562/RCS/fpgaio.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaio.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * FPGA-FIFOﾄﾞﾗｲﾊﾞ追加
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);

