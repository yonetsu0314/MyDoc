/*----------------------------------------------------------------------*
 * File name	: portio.asm	/ port io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/portio.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: portio.c $
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
 *----------------------------------------------------------------------*
 * Function								*
 *	SCU(UPD71051ｻﾌﾞｾｯﾄ)のI/Oﾊﾝﾄﾞﾗﾓｼﾞｭｰﾙ。BIOSのI/Fﾗｲﾌﾞﾗﾘｰより、	*
 *	    BX :機能番号(直接ﾃｰﾌﾞﾙ･ｼﾞｬﾝﾌﾟを行うため×2した値)		*
 *	    BP :ﾊﾟﾗﾒｰﾀへのﾎﾟｲﾝﾀ([bp+6]より第一ﾊﾟﾗﾒｰﾀ)			*
 *	が、設定されｴﾝﾄﾘする。						*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <biosif\pio.h>
#include "dmu5562.h"

void port_io(void);
int p_in(int);
int p_out(int,int);

#pragma	inline_asm(port_io)
static void port_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_port,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
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
          .DATA.L     _p_in				; r0 = 0
          .DATA.L     _p_out			; r0 = 0
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


int port_io_err(){
	rsstr("*** port_io err ***");
	return(0);
}

int p_in(
	int	port_ch				/* port channel		*/
){
	int		sw = 0;

//	rsstr("*** port_io in ***");
	switch(port_ch){
		case PIN_A:		//DIP SW 1 active bits
			sw |= (PB.DR.BIT.B1)?0x0010:0x0000;
			return(sw);
		case PIN_D:		//printer data inputs
			sw |= (PB.DR.BIT.B9)?0x0000:0x0001;	//BATTERY
			return(sw);
		case PIN_B:		//DIG SW no active bits
		case PIN_C:		//general inputs
		default:
			break;
	}
	return(0);
}

int p_out(
	int		ch,			/* channel			*/
	int		c			/* char bit data	*/
){
//	rsstr("*** port_io out ***");
	switch(ch){
		case POUT_B:		//LED
			PA.DR.BIT.B0 = (c&0x0001)?0:1;
			PA.DR.BIT.B1 = (c&0x0002)?0:1;
			PA.DR.BIT.B2 = (c&0x0004)?0:1;
			PA.DR.BIT.B4 = (c&0x0008)?0:1;
			break;
		case POUT_A:	//key select outputs
		case POUT_C:	//strobe online motor buzzer general outputs
		case POUT_D:	//printer data outputs
		default:
			break;
	}
	return(0);
}

