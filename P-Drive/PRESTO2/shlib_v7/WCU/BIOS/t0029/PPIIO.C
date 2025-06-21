/*----------------------------------------------------------------------*
 * File name	: ppiio.c	/ arcnet io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/ppiio.c 1.1.29.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: ppiio.c $
 * リビジョン 1.1.29.1  1970/01/01  00:00:00  kawamura
 * 2004/07/29  11:34
 * ディップスイッチが読み込める様にする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/13 11:00
 * pio_175_out の論理を反対にする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#define	P5561
#include <biosif\ppia.h>

#define	PIO0_ADDR_BASE	(unsigned long)0x0b0020001	/* PIO0 address		*/
#define	PIO1_ADDR_BASE	(unsigned long)0x0b0020002	/* PIO1 address		*/
#define	PIO2_ADDR_BASE	(unsigned long)0x0b0020000	/* PIO2 address		*/
							/* bit4 : TP2		*/
							/* bit3 : ADC_RES4	*/
							/* bit2 : ADC_RES3	*/
							/* bit1 : ADC_RES2	*/
							/* bit0 : ADC_RES1	*/
#define	PIO3_ADDR_BASE	(unsigned long)0x0b0020003	/* PIO2 address		*/
							/* bit0 : DRVPW		*/
#define	SW2_ADDR_BASE	(unsigned long)0x0b1000000	/* SW2  address		*/
#define	PIO_MAX		(4)

#define	INITDATA	(unsigned char)0x00

static struct pioinf	phyinf[PIO_DEV_MAX];


void	pio_io(void);
int	pio_io_init(int);
int	pio_io_out(int, int, unsigned char);
int	pio_io_in(int, int);
int	pio_io_bctl(int, int ,int);
void *	pio_icu_info(int, int);
int	pio_175_out(int, int, unsigned char *);

#pragma	inline_asm(pio_io)
static void pio_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_pio,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          pio_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
pio_exit:
          MOV.L       Lrsxxx,R0   ;pio_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _pio_io_err

          .ALIGN 4
jt_pio:
          .DATA.L     _pio_io_init  ; r0 = 0
          .DATA.L     _pio_io_out   ; r0 = 1
          .DATA.L     _pio_io_in    ; r0 = 2
          .DATA.L     _pio_io_bctl  ; r0 = 3
          .DATA.L     _pio_icu_info ; r0 = 4
          .DATA.L     _pio_175_out  ; r0 = 5
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int
pio_io_err(void)
{
	return(-1);
}

int
pio_io_init(
	int		devno
	)
{
	unsigned char	*p;
	int		err = 0;

	switch(devno){
		case PIO0:
			p = (unsigned char *)PIO0_ADDR_BASE;
			*p = INITDATA;
			break;
		case PIO1:
			p = (unsigned char *)PIO1_ADDR_BASE;
			*p = INITDATA;
			break;
		case PIO2:
			p = (unsigned char *)PIO2_ADDR_BASE;
			*p = INITDATA;
			break;
		case PIO3:
			p = (unsigned char *)PIO3_ADDR_BASE;
			*p = INITDATA;
			break;
		default:
			err = -1;
			break;
	}

	return(err);
}

int
pio_io_out(
	int		dev_no,
	int		port_no,
	unsigned char	data
)
{
	unsigned char	*p;
	int		err = 0;

	port_no = port_no;

	if(dev_no < PIO0 || dev_no >= PIO_DEV_MAX){
		return(-1);
	}

	switch(dev_no){
		case PIO0:
			p = (unsigned char *)PIO0_ADDR_BASE;
			*p = data;
			break;
		case PIO1:
			p = (unsigned char *)PIO1_ADDR_BASE;
			*p = data;
			break;
		case PIO2:
			p = (unsigned char *)PIO2_ADDR_BASE;
			*p = data;
			break;
		case PIO3:
			p = (unsigned char *)PIO3_ADDR_BASE;
			*p = data;
			break;
		default:
			break;
	}
	if(data != *p){
		err = -1;
	}
	return(err);
}

int
pio_io_in(
	int		dev_no,
	int		port_no
)
{
	 unsigned char	data;

	if(dev_no < PIO0 || dev_no >= PIO_DEV_MAX){
		return(-1);
	}
	switch(dev_no){
		case PIO0:
			data = *(unsigned char *)PIO0_ADDR_BASE;
			break;
		case PIO1:
			if(port_no) {
				data = *(unsigned char *)PIO1_ADDR_BASE;
			} else {
				data = *(unsigned char *)SW2_ADDR_BASE;
			}
			break;
		case PIO2:
			data = *(unsigned char *)PIO2_ADDR_BASE;
			break;
		case PIO3:
			data = *(unsigned char *)PIO3_ADDR_BASE;
			break;
		default:
			break;
	}

	return((int)~data);
}

int
pio_io_bctl(
	int	dev_no,
	int	bit_position,
	int	on_off
)
{
	static unsigned char	pdata[PIO_MAX];
	unsigned char		*p;
	int			i;

	if(dev_no < PIO0 || dev_no >= PIO_DEV_MAX){
		return(-1);
	}

	if(bit_position < 0 || bit_position > 7){
		return(-1);
	}

	switch(dev_no){
		case PIO0:
			i = PIO0;
			p = (unsigned char *)PIO0_ADDR_BASE;
			break;
		case PIO1:
			i = PIO1;
			p = (unsigned char *)PIO1_ADDR_BASE;
			break;
		case PIO2:
			i = PIO2;
			p = (unsigned char *)PIO2_ADDR_BASE;
			break;
		case PIO3:
			i = PIO3;
			p = (unsigned char *)PIO3_ADDR_BASE;
			break;
		default:
			return(-1);
			break;
	}
	
	switch(on_off){
		case 0:/*ON*/
			pdata[i] |= (unsigned char)(1<<bit_position);
			break;
		case 1:/*OFF*/
			pdata[i] &= (unsigned char)(~(1<<bit_position));
			break;
		default:
			return(-1);
			break;
	}
	
	*p = pdata[i];
	return((int)pdata[i]);
}

void *
pio_icu_info(
	int	dev_no,
	int	port_no
)
{
	port_no = port_no;
	if(dev_no < PIO0 || dev_no >= PIO_DEV_MAX){
		return((void*)0);
	}

	return((void *)&phyinf[dev_no]);

}

int
pio_175_out(
	int		on_off,
	int		data,
	unsigned char	*rdata
)
{
	unsigned char *p = (unsigned char *)PIO2_ADDR_BASE;

	switch(on_off){
		case 0:/*RESET*/
			*rdata |= (unsigned char)data;
			break;
		case 1:/*SET*/
			*rdata &= (unsigned char)(~(unsigned char)data);
			break;
		default:
			return(-1);
			break;
	}
	*p = *rdata;
	return(0);
}
