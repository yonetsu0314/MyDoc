/*----------------------------------------------------------------------*
 * File name	: dpmemio.asm	/ dpmem io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/dpmemio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmemio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <biosif\dpmem.h>

#define	ADC1_ADDR_BASE	(unsigned long)0x0b8000000	/* ADC0 Address	*/
#define	ADC2_ADDR_BASE	(unsigned long)0x0b8010000	/* ADC1 Address	*/

#define	BYTE_M		(0)
#define	WORD_M		(1)

#define	ADC_SIZE	(0x0ffc)
#define	WR_CMD		(0x0ffc)
#define	RD_CMD		(0x0ffe)

static struct dpmpinf	phyxinf[] = {
				(unsigned char *)ADC1_ADDR_BASE,
				(unsigned int)ADC_SIZE,
				BYTE_M,
				(unsigned int)WR_CMD,
				(unsigned int)RD_CMD,

				(unsigned char *)ADC2_ADDR_BASE,
				(unsigned int)ADC_SIZE,
				BYTE_M,
				(unsigned int)WR_CMD,
				(unsigned int)RD_CMD,
			};


void	dpmem_io(void);
void	*dpmem_io_init(int);

#pragma	inline_asm(dpmem_io)
static void dpmem_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_dpmem,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          dpmem_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
dpmem_exit:
          MOV.L       Lrsxxx,R0   ;dpmem_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _dpmem_io_err

          .ALIGN 4
jt_dpmem:
          .DATA.L     _dpmem_io_init		; r0 = 0
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
dpmem_io_err(void)
{
	return(0);
}

void *
dpmem_io_init(
	int	dev_no
)
{
	if(dev_no == WCU_DAD1){
		return((void *)(&phyxinf[0]));
	} else if(dev_no == WCU_DAD2){
		return((void *)(&phyxinf[1]));
	} else {
		return((void *)0);
	}
}

