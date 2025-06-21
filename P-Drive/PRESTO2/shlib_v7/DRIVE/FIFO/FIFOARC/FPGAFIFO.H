/*======================================================================
 * File name	: fpgafifo.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fifo/fifoarc/rcs/fpgafifo.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgafifo.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期レビジョン
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

#define FIFOTXEND  (0x04)
#define FIFOTXFULL (0x02)
#define FIFORXDATA (0x01)

#define	FPGA_TX_MSK	(1)			/* 送信ﾏｽｸ		*/
#define	FPGA_RX_MSK	(2)			/* 受信ﾏｽｸ		*/

