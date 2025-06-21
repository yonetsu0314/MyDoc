/*----------------------------------------------------------------------*
 * File name	: e2romio.c	/ e2rom io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/e2romio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2romio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <biosif\e2rom.h>

#define	E2ROM_ADDR_BASE (unsigned long)0x0b0000000  /* E2ROM address   */
#define	E2ROM_PAGE	(32)

void e2rom_io(void);
int e2rom_init(E2PHY **, E2WRT *);
int e2rom_pgwr(struct e2ioh  *);

#pragma	inline_asm(e2rom_io)
static void e2rom_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
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
          RTS
          NOP
e2rom_exit:
          MOV.L       Lrsxxx,R0   ;e2rom_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
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


int
e2rom_io_err(void)
{
	return(0);
}

int
e2rom_init(
	E2PHY  **e2romphy,
	E2WRT  *intrvl
)
{
	*e2romphy = (E2PHY *)E2ROM_ADDR_BASE;
	*intrvl = (E2WRT)15;
	return(0);
}

int
e2rom_pgwr(
	struct e2ioh  *drvinfop
)
{
	unsigned char	*bp,*wp;
	unsigned int	getp, putp, eofp, bufmax;
	int		byte = 0;
	
	bp = (unsigned char *)drvinfop->e2buf;
	getp = *(drvinfop->getp);
	putp = *(drvinfop->putp);
	eofp = *(drvinfop->filep);
	bufmax = drvinfop->szbuf;

	wp = (unsigned char *)(E2ROM_ADDR_BASE + eofp);

	if(getp == putp){
		return(-1);
	}

	getp &= bufmax;

	for(;;){
		*(wp++) = *(bp + (getp++));
		eofp++;
		byte++;
		if(getp == putp){
			break;
		}
		if(byte == E2ROM_PAGE){
			break;
		}
	}
	*drvinfop->getp = getp;
	*drvinfop->filep = eofp;
	return(byte);
}
