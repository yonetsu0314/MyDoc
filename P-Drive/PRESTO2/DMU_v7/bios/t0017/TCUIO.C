/*----------------------------------------------------------------------*
 * File name	: tcuio.c	/ tcu io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/tcuio.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: tcuio.c $
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
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <sh7615\sh7615.h>
#include "dmu5562.h"

void tcu_io(void);
int tcu_io_init(void);
int tcu_io_write(void);
int tcu_io_read(void);
int tcu_io_stmod(void);
int tcu_io_bread(void);

#pragma	inline_asm(tcu_io)
static void tcu_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_tcu,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          tcu_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
tcu_exit:
          MOV.L       Lrsxxx,R0   ;tcu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _tcu_io_err

          .ALIGN 4
jt_tcu:
          .DATA.L     _tcu_io_init		; r0 = 0
          .DATA.L     _tcu_io_write	;    = 1
          .DATA.L     _tcu_io_read		;    = 2
          .DATA.L     _tcu_io_stmod	;    = 3
          .DATA.L     _tcu_io_bread	;    = 4
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


int tcu_io_err(){
	rsstr("*** tcu_io err ***");
	return(0);
}

int tcu_io_init(){
	rsstr("*** tcu_io init ***");
	return(int)(0xfffffcc0);
}

int tcu_io_write(){
	rsstr("*** tcu_sr write ***");
	return(0);
}

int tcu_io_read(){
	rsstr("*** tcu_cr read ***");
	return(0);
}

int tcu_io_stmod(){
	rsstr("*** tcu_data read ***");
	return(0);
}

int tcu_io_bread(){
	rsstr("*** tcu_data write ***");
	return(0);
}

