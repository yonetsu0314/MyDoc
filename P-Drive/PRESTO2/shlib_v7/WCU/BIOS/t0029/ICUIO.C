/*----------------------------------------------------------------------*
 * File name	: icuio.c	/ icu io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/icuio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: icuio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include	<biosif\icu.h>			/*	"		*/

void		icu_io(void);
int		dis_icu(int, int);
int		ena_icu(int, int);
int		get_ipfw(short dev_no);
unsigned int	icu_dev_hnt(V_FUNC, struct vects *);

#pragma	inline_asm(icu_io)
static void icu_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_icu,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          icu_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
icu_exit:
          MOV.L       Lrsxxx,R0   ;icu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _icu_io_err

          .ALIGN 4
jt_icu:
          .DATA.L     _dis_icu		; r0 = 0
          .DATA.L     _ena_icu		;    = 1
          .DATA.L     _get_ipfw		;    = 2
          .DATA.L     _icu_dev_hnt	;    = 3
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
icu_io_err(void)
{
	return(-1);
}

int
dis_icu(
	int dev_no,
	int pattern
)
{
	return(0);
}

int
ena_icu(
	int dev_no,
	int pattern
)
{
	return(0);
}

int
get_ipfw(
	short dev_no
)
{
	return(0);
}

unsigned int
icu_dev_hnt(
	V_FUNC funcp,
	struct vects *vectp
)
{
	return((unsigned int)0);
}
