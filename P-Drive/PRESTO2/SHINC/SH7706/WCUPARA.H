/*======================================================================
 * File name	: wcupara.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7706/RCS/wcupara.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: wcupara.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 新レビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

/* FPGA内割込みマスク・レジスタのＢＩＴ */
#define CONVINT (0x2000)
#define WFIFOINT (0x1000)
#define WPINT7   (0x0800)
#define WPINT6   (0x0400)
#define WPINT5   (0x0200)
#define WPINT4   (0x0100)
#define WPINT3   (0x0080)
#define WPINT2   (0x0040)
#define ADC2INT (0x0020)
#define ADC1INT (0x0010)
#define ADC3INT (0x0008)
#define ARC1INT (0x0004)
#define ARC0INT (0x0002)
#define M66230INT (0x0001)

int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);

void			int_mask(int);
void			wcu_jump_to_reset(void);

