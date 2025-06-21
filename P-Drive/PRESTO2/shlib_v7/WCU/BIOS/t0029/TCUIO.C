/*----------------------------------------------------------------------*
 * File name	: tcuio.c	/ tcu io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/tcuio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tcuio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <sh7706\7706.h>

#define	C1200BPS	(0)
#define	C2400BPS	(1)
#define	C4800BPS	(2)
#define	C9600BPS	(3)
#define	C19KBPS		(4)

#define	T1MSEC		(10)
#define	T2MSEC		(11)
#define	T5MSEC		(12)
#define	T10MSEC		(13)

#define	CNTMODE		(0x100)

#define	TCH0		(0)
#define	TCH1		(1)
#define	TCH2		(2)

void tcu_io(void);
int tcu_io_init(int, int);
int tcu_io_write(int, int);
int tcu_io_read(int);
int tcu_io_stmod(int);
int tcu_io_bread(int);

#pragma	inline_asm(tcu_io)
static void tcu_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
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
          RTS
          NOP
tcu_exit:
          MOV.L       Lrsxxx,R0   ;tcu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _tcu_io_err

          .ALIGN 4
jt_tcu:
          .DATA.L     _tcu_io_init  ; r0 = 0
          .DATA.L     _tcu_io_write ;    = 1
          .DATA.L     _tcu_io_read  ;    = 2
          .DATA.L     _tcu_io_stmod ;    = 3
          .DATA.L     _tcu_io_bread ;    = 4
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
 tcu_io_err(void)
{
	return(0);
}

int
tcu_io_init(
	int	chno,
	int	mode
)
{
	switch(mode){
		case C1200BPS:
		case C2400BPS:
		case C4800BPS:
		case C9600BPS:
		case C19KBPS:
			break;
		case T1MSEC:
		case T2MSEC:
		case T5MSEC:
		case T10MSEC:
			break;
		case CNTMODE:
			break;
		default:
			break;
	}
	return(0);
}

int
tcu_io_write(
	int	ch,
	int	cnt
)
{
	switch(ch){
		case TCH0:
			TMU0.TCOR = cnt;
			break;
		case TCH1:
			TMU1.TCOR = cnt;
			break;
		case TCH2:
			TMU2.TCOR = cnt;
			break;
		default:
			break;
	}
	return(0);
}

int
tcu_io_read(
	int	ch
)
{
	int	cnt;

	switch(ch){
		case TCH0:
			cnt = TMU0.TCOR;
			break;
		case TCH1:
			cnt = TMU1.TCOR;
			break;
		case TCH2:
			cnt = TMU2.TCOR;
			break;
		default:
			cnt = -1;
			break;
	}
	return(cnt);
}

int
tcu_io_stmod(
	int	mode
)
{
	return(0);
}

int
tcu_io_bread(
	int	ch
)
{
	int	cnt;

	switch(ch){
		case TCH0:
			cnt = TMU0.TCR.WORD;
			break;
		case TCH1:
			cnt = TMU1.TCR.WORD;
			break;
		case TCH2:
			cnt = TMU2.TCR.WORD;
			break;
		default:
			cnt = -1;
			break;
	}
	return(cnt);
}
