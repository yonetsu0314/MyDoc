/*----------------------------------------------------------------------*
 * File name	: e2romio.c				/ e2rom io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/e2romio.c 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2romio.c $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * e2iohﾎﾟｲﾝﾀの誤ったｷｬｽﾄを削除
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
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <sh7615\sh7615.h>
#include <biosif\e2rom.h>
#include "dmu5562.h"
#define     RESET_CMD       (UH)0x0F000  /* reset command                */
#define     E2_OK            0

#define  E2ROM_ADDR_BASE (UW)0x201f8000  /* FLASH device standard address   */

void e2rom_io(void);
int e2rom_init(E2PHY **, E2WRT *);
int e2rom_pgwr(struct e2ioh  *);

#pragma	inline_asm(e2rom_io)
static void e2rom_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_e2rom,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          e2rom_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
e2rom_exit:
          MOV.L       Lrsxxx,R0   ;e2rom_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _e2rom_io_err

          .ALIGN 4
jt_e2rom:
          .DATA.L     _e2rom_init		; r0 = 0
          .DATA.L     _e2rom_pgwr		;    = 1
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


int e2rom_io_err(){
	rsstr("*** e2rom_io err ***");
	return(0);
}

int e2rom_init(E2PHY  **e2romphy, E2WRT  *intrvl){
//	rsstr("*** e2rom init ***");
	*e2romphy = (E2PHY *)E2ROM_ADDR_BASE;
	*intrvl = (E2WRT)1;
	set_fladr(E2ROM_ADDR_BASE);
	set_mode(RESET_CMD);
	return(0);
}

typedef union uci {		//saka: High,Low反転に注意
	unsigned char	c[2];			/* charの配列データ	*/
	short		i;			/* intデータ		*/
} UCI;

int e2rom_pgwr(struct e2ioh  *drvinfop){
	union uci		shrt;
	unsigned char	*bp;
	unsigned int	gofs,pofs,eofs,bufmax;
	short	*flashadr;
	int		retry = 5;

//	rsstr("*** e2rom pgwr ***");
start:
	if((--retry)==0) goto err;
	eofs = *(drvinfop->filep);
	bp   = (unsigned char *)drvinfop->e2buf;
	gofs = *(drvinfop->getp);
	pofs = *(drvinfop->putp);
	bufmax = drvinfop->szbuf;

	if(gofs==pofs) return(1);

	set_mode(RESET_CMD);
	flashadr = (short *)(E2ROM_ADDR_BASE + eofs);

	if((eofs%2)!=0){
		eofs -= 1;	eofs &= (short)0x7fff;
		flashadr = (short *)(E2ROM_ADDR_BASE + eofs);
		shrt.i = *flashadr;
		shrt.c[1] = *(bp + (gofs++));	gofs &= bufmax;
		if(flash_write(flashadr, shrt.i) != E2_OK){
			set_mode(RESET_CMD);
			if(sect_erase((UW)flashadr) != E2_OK) goto err;
			if(flash_write(flashadr, shrt.i) != E2_OK) goto err;
			goto start;
		}
		eofs += 2;	eofs &= (short)0x7fff;
		flashadr = (short *)(E2ROM_ADDR_BASE + eofs);
	}
	for(;;){
		shrt.i = (short)0xffff;
		shrt.c[0] = *(bp + (gofs++));	gofs &= bufmax;
		if(gofs == pofs) break;
		shrt.c[1] = *(bp + (gofs++));	gofs &= bufmax;
		if(gofs == pofs) break;
		if(flash_write(flashadr, shrt.i) != E2_OK){
			set_mode(RESET_CMD);
			if(sect_erase((UW)flashadr) != E2_OK) goto err;
			if(flash_write(flashadr, shrt.i) != E2_OK) goto err;
			goto start;
		}
		eofs += 2;	eofs &= (short)0x7fff;
		flashadr = (short *)(E2ROM_ADDR_BASE + eofs);
//		dly_tsk((RELTIM)1);
	}
	if(flash_write(flashadr, shrt.i) != E2_OK){
		set_mode(RESET_CMD);
		if(sect_erase((UW)flashadr) != E2_OK) goto err;
		if(flash_write(flashadr, shrt.i) != E2_OK) goto err;
		goto start;
	}
    set_mode(RESET_CMD);
	*(drvinfop->getp) = *(drvinfop->putp);
	return(1);			/* return semaphore number = 1 */
err:
    set_mode(RESET_CMD);
	*(drvinfop->getp) = *(drvinfop->putp);
	return(0);
}

