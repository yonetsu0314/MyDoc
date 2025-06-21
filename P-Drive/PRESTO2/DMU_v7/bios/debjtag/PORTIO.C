/*----------------------------------------------------------------------*
 * File name	: portio.asm	/ port io svc handler					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/portio.c 1.4 1970/01/01 00:00:00 chimura Exp $
 *$Log: portio.c $
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2005/01/24 BIOSIFを経由する。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/14 20:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソフト割込み中は割込み禁止にする
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.							*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*
 *		PHONE (0775) 53-4141											*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <biosif\pio.h>
#include <sh7615\bootpara.h>
#include "dmu5562.h"

#define PIO_ADDR		((volatile unsigned char*)0x24400000)

void	port_io(void);
int		p_init(int);
int		p_out(int, int, unsigned char);
int		p_in(int, int);
int		p_bctl(int, int ,int);
void 	*p_info(int, int);
int		p_175_out(int, int, unsigned char *);

#pragma	inline_asm(port_io)
static void port_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_port,R2  ;arrangement Top Address
          ADD         R2,R0        ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          port_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
port_exit:
          MOV.L       Lrsxxx,R0   ;port_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _port_io_err

          .ALIGN 4
jt_port:
          .DATA.L     _p_init       ; r0 = 0
          .DATA.L     _p_out        ; r0 = 1
          .DATA.L     _p_in         ; r0 = 2
          .DATA.L     _p_bctl       ; r0 = 3
          .DATA.L     _p_info       ; r0 = 4
          .DATA.L     _p_175_out    ; r0 = 5
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


int port_io_err(){
	rsstr("*** port_io err ***");
	return(0);
}

int
p_init(
	int		devno
	)
{
	devno = devno;
	return(0);
}

int
p_out(
	int		dev_no,
	int		port_no,
	unsigned char	data
)
{
	if(dev_no){/*INT MASK REG*/
		switch(port_no){
			case 0:
				*INT_MASK_REG0 = (char)(0xf0 | data);	/* Disable Outernal Interrupt */
				break;
			case 1:
				*INT_MASK_REG1 = (char)(0xf0 | data);	/* Disable Outernal Interrupt */
				break;
			default:
				break;
		}
	} else {
		*PIO_ADDR = data;
		dev_no = dev_no; port_no = port_no;
	}
	return(0);
}


int
p_in(
	int		dev_no,
	int		port_no
)
{
	unsigned char a;

	if(dev_no){/*DIPSW*/
		switch(port_no){
			case 1:
				a = (unsigned char)(*DIPSW1_ADDR);
				a = (unsigned char)(~a);
				break;
			case 2:
				a = (unsigned char)(*DIPSW2_ADDR);
				a = (unsigned char)(~a);
				break;
			default:
				a = (unsigned char)((PB.DR.BIT.B1)|((PA.DR.BIT.B11)<<2));
				a = (unsigned char)((~a) & 0x05);
				break;
		}
	} else {
		a = (unsigned char)(*PIO_ADDR);
		a = (unsigned char)(~a);
	}
	return((int)a);
}

int
p_bctl(
	int	dev_no,
	int	bit_position,
	int	on_off
)
{
	dev_no = dev_no;
	bit_position = bit_position;
	on_off = on_off;
	return(0);
}

void *
p_info(
	int	dev_no,
	int	port_no
)
{
	dev_no = dev_no;
	port_no = port_no;
	return((void *)0);
}

int
p_175_out(
	int		on_off,
	int		data,
	unsigned char	*rdata
)
{
	on_off = on_off;
	data = data;
	rdata = rdata;
	return(0);
}
