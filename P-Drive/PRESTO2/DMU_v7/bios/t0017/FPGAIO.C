/*----------------------------------------------------------------------*
 * File name	: fpgaio.asm	/ fpga io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/fpgaio.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaio.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04.12.09新規追加
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*
 * Function								*
 *	ＦＰＧＡ内で構成されているＦＩＦＯを制御する	*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <sh7615\bootpara.h>
#include "dmu5562.h"

#define FIFODATA ((volatile unsigned char *)0x24a00000)
#define FIFOSTTS ((volatile unsigned char *)0x24a00001)

void fpga_io(void);
int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);
void			int_mask(int mask);
void			pint_mask(int mask);


#pragma	inline_asm(fpga_io)
static void fpga_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_fpga,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          fpga_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
fpga_exit:
          MOV.L       Lrsxxx,R0   ;fpga_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _fpga_io_err

          .ALIGN 4
jt_fpga:
          .DATA.L     _fifo_input	; r0 = 0
          .DATA.L     _fifo_status	; r0 = 1
          .DATA.L     _fifo_output	; r0 = 2
          .DATA.L     _fifo_msktxrx	; r0 = 3
          .DATA.L     _fifo_init	; r0 = 4
          .DATA.L     _fifo_full	; r0 = 5
          .DATA.L     _int_mask     ; r0 = 6
          .DATA.L     _pint_mask    ; r0 = 7
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int fpga_io_err(){
	rsstr("*** fpga_io err ***");
	return(0);
}


void
int_mask(int mask){
	*INT_MASK_REG0 = (char)(0xf0 | mask);		/* Disable Outernal Interrupt */
}


void
pint_mask(int mask){
	*INT_MASK_REG1 = (char)(0xf0 | mask);		/* Disable Outernal Interrupt */
}


static void
wait88n(int m){				/* wait 88nS * m at Compile option = 1 */
	int	i;

	for(i=0;i<m;++i) ;
	return;
}


int
fifo_init(){
	wait88n(10000);
	fifo_input();	//ダミーリード
	return(0);
}


unsigned char
fifo_input(){
	unsigned char	c;

	c = (*FIFODATA);
	return(c);
}


unsigned char
fifo_status(){
	unsigned char	c;

	c = (*FIFOSTTS);
	return(c);
}


void
fifo_output( unsigned char Data8 ){
	*FIFODATA = Data8;
	return;
}


void
fifo_msktxrx(unsigned short flag){
	*FIFOSTTS = (unsigned char)flag;
}


int
fifo_full(){
	unsigned char	c;

	c = (*FIFOSTTS);
	return((int)c);
}


