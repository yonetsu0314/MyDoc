/*----------------------------------------------------------------------*
 * File name	: e2sromio.c	/ e2srom io svc handler					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/e2sromio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2sromio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.							*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*
 *		PHONE (0775) 53-4141											*
 *----------------------------------------------------------------------*/
#include "dmu5562.h"

void			e2srom_io(void);
void			e2s_handle_ce(int, int, int);
void			e2s_handle_di(int, int, int);
void			e2s_handle_sk(int, int, int);
unsigned char	e2s_handle_do(int, int);


#pragma	inline_asm(e2srom_io)
static void e2srom_io(void){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_e2srom,R2  ;arrangement Top Address
          ADD         R2,R0          ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          e2srom_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
e2srom_exit:
          MOV.L       Lrsxxx,R0     ;e2srom_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0        ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                       ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _e2srom_io_err

          .ALIGN 4
jt_e2srom:
          .DATA.L     _e2s_handle_ce ; r0 = 0
          .DATA.L     _e2s_handle_di ; r0 = 1
          .DATA.L     _e2s_handle_sk ; r0 = 2
          .DATA.L     _e2s_handle_do ; r0 = 3
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
          .DATA.L     0              ; none
}


int
e2srom_io_err(void)
{
	return(0);
}

void
e2s_handle_ce(int devno, int bit_pos, int on_off)	/* CE Bitのセット/リセットを行う	*/
{
	PB.DR.BIT.B0 = on_off;
	devno = devno;
	bit_pos = bit_pos;
}

/*----------------------------------------------------------------------*/
void
e2s_handle_di(int devno, int bit_pos, int on_off)	/* DI Bitのセット/リセットを行う	*/
{
	PA.DR.BIT.B10 = on_off;
	devno = devno;
	bit_pos = bit_pos;
}

/*----------------------------------------------------------------------*/
void
e2s_handle_sk(int devno, int bit_pos, int on_off)	/* SK Bitのセット/リセットを行う	*/
{

	PA.DR.BIT.B13 = on_off;
	devno = devno;
	bit_pos = bit_pos;
}

/*----------------------------------------------------------------------*/
unsigned char
e2s_handle_do(int devno, int port_ch)				/* DO Bitの入力を行い戻り値で返す	*/
{
	unsigned char	c;

	devno = devno;
	port_ch = port_ch;

	do {
		c = PA.DR.BIT.B12;
	} while(c != PA.DR.BIT.B12);	/* 念のための２度読み */
	return(c);
}

