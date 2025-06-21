/*======================================================================
 * File name	: wcupara.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7615/RCS/dmupara.h 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: dmupara.h $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * PIOのアドレスと関数追加
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * dipsw関数を追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * dmu_jump_to_reset()追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#define INT_MASK_REG0	((volatile char*)0x24900000)
#define INT_MASK_REG1	((volatile char*)0x24980000)
#define INT_ACK_REG0	((volatile char*)0x24700000)
#define INT_ACK_REG1	((volatile char*)0x24780000)
#define RSTCSRR			((volatile unsigned char*)0xfffffe83)
#define DIPSW1_ADDR		((volatile unsigned char*)0x24680000)
#define DIPSW2_ADDR		((volatile unsigned char*)0x24600000)
#define PIO_ADDR		((volatile unsigned char*)0x24400000)

#define ARCINT1 (0x08)
#define ARCINT0 (0x04)
#define FIFOINT (0x02)

#define PINT3 (0x08)
#define PINT2 (0x04)
#define PINT1 (0x02)
#define PINT0 (0x01)

int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);

void int_mask(int mask);
void pint_mask(int mask);
void dmu_jump_to_reset(void);
unsigned char dipsw(int no);
unsigned char pio_in(void);
void pio_out(unsigned char);

