/*----------------------------------------------------------------------*
 * File name	: sramio.asm	/ sram io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/sramio.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sramio.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/17 14:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソフト割込み中は割込み禁止にする
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <biosif\sram.h>
#include <sh7615\sh7615.h>
#include "dmu5562.h"

static struct srampinf	phyxinf;	/* & to physical info	*/

void  sram_io(void);
void *sram_io_init(void);

#pragma	inline_asm(sram_io)
static void sram_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
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
          RTE
          NOP
sram_exit:
          MOV.L       Lrsxxx,R0   ;sram_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
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


int sram_io_err(){
	rsstr("*** sram_io err ***");
	return(0);
}

void *
sram_io_init(){
	phyxinf.iobase = (unsigned char *)0x02000100;
	phyxinf.mem_size = 65535;
// SRAMとして262143バイト確保しているが、ワーニング対応のため65535を代入
	return((void *)(&phyxinf));
}

