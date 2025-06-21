/*----------------------------------------------------------------------*
 * File name	: cfio.asm	/ cf io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/cfio.c 1.5 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: cfio.c $
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * 05.03.18 CD_CHKをBIOSに移動
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * cfio,fromio追加
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <biosif\cf.h>
#include <drive\pcmsock.h>
#include <sh7615\sh7615.h>
#include "dmu5562.h"

static struct cfpinf	phyxinf;	/* & to physical info	*/

void  cf_io(void);
void *cf_io_init(void);
int   cd_chk(int);

#pragma	inline_asm(cf_io)
static void cf_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_cf,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          cf_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
cf_exit:
          MOV.L       Lrsxxx,R0   ;cf_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _cf_io_err

          .ALIGN 4
jt_cf:
          .DATA.L     _cf_io_init		; r0 = 0
          .DATA.L     _cd_chk			; r0 = 1
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


int cf_io_err(){
	rsstr("*** cf_io err ***");
	return(0);
}

void *
cf_io_init(){
	phyxinf.comm = PCMCIA_COMM_BASE;
	phyxinf.attr = PCMCIA_ATTR_BASE;
	phyxinf.io   = PCMCIA_IO_BASE;
	return((void *)(&phyxinf));
}

/*----------------------------------------------------------------
					ＣＤ(Card Detect)信号の検査
----------------------------------------------------------------*/
int cd_chk( int psock )
{
	switch( psock ) {
		case 0:		return (PB.DR.BIT.B8)? 1 : 0;
		default:	return 0;
	}
}

