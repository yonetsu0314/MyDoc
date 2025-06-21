/*----------------------------------------------------------------------*
 * File name	: rtcio.c	/ rtc io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/rtcio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rtcio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/

void	rtc_io(void);
int	rtc_io_init(void);
int	rtc_io_sdate(int,int,int,int);
int	rtc_io_gdate(int *,int *,int *,int *);
int	rtc_io_stime(int,int,int);
int	rtc_io_gtime(int *,int *,int *);

#pragma	inline_asm(rtc_io)
static void rtc_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_rtc,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          rtc_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
rtc_exit:
          MOV.L       Lrsxxx,R0   ;rtc_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _rtc_io_err

          .ALIGN 4
jt_rtc:
          .DATA.L     _rtc_io_init		; r0 = 0
          .DATA.L     _rtc_io_sdate		;    = 1
          .DATA.L     _rtc_io_gdate		;    = 2
          .DATA.L     _rtc_io_stime		;    = 3
          .DATA.L     _rtc_io_gtime		;    = 4
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
rtc_io_err(void)
{
	return(0);
}

int
rtc_io_init(void)
{
	return((int)0xfffffec0);
}

int
rtc_io_sdate(
	int	year,
	int	month,
	int	day,
	int	weed
)
{
	return(0);
}

int
rtc_io_gdate(
	int	*year,
	int	*month,
	int	*day,
	int	*week
)
{
	return(0);
}

int
rtc_io_stime(
	int	hour,
	int	min,
	int	sec
)
{
	return(0);
}

int
rtc_io_gtime(
	int	*hour,
	int	*min,
	int	*sec
)
{
	return(0);
}
