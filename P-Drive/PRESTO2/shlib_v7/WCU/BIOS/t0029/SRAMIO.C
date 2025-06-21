/*----------------------------------------------------------------------*
 * File name	: sramio.asm	/ sram io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/sramio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <biosif\sram.h>

#define	SRAM_ADDR_BASE	(unsigned long)0x0cf00000/* SRAM Address	*/
#define	SRAM_SIZE	262143			/**/

static struct srampinf	phyxinf;		/* & to physical info	*/

void  sram_io(void);
void *sram_io_init(void);

#pragma	inline_asm(sram_io)
static void sram_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_sram,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          sram_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
sram_exit:
          MOV.L       Lrsxxx,R0   ;sram_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _sram_io_err

          .ALIGN 4
jt_sram:
          .DATA.L     _sram_io_init		; r0 = 0
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
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int
sram_io_err(void)
{
	return(0);
}

void *
sram_io_init(void)
{
	phyxinf.iobase = (unsigned char *)SRAM_ADDR_BASE;
	phyxinf.mem_size = (unsigned short)SRAM_SIZE;	//unsigned short なので実際の設定は 65535
	return((void *)(&phyxinf));
}
