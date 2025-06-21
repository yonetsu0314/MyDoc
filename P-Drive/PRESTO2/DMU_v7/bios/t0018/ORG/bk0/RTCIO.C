/*----------------------------------------------------------------------*
 * File name	: rtcio.c	/ rtc io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/rtcio.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rtcio.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/17 15:00
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
//#include <custom\typedefs.h>
#include "dmu5562.h"

typedef	unsigned char	BYTE ;		/* 8-bit data */
typedef	unsigned short	WORD ;		/* 16-bit data */
typedef	unsigned long	DWORD ;		/* 32-bit data */

struct st_rtc{
	unsigned char	SEC0;
	unsigned char	SEC1;
	unsigned char	MIN0;
	unsigned char	MIN1;
	unsigned char	HOUR0;
	unsigned char	HOUR1;
	unsigned char	DAY0;
	unsigned char	DAY1;
	unsigned char	MON0;
	unsigned char	MON1;
	unsigned char	YEAR0;
	unsigned char	YEAR1;
	unsigned char	WEEK;
	unsigned char	CTRL_D; 	//b0:HOLD,b1:BUSY,b2:IRQ FLG,b3:30sec ADJ
	unsigned char	CTRL_E; 	//b0:MASK,b1:INTRPT/STND,b2:t0,b3:t1
	unsigned char	CTRL_F;		//b0:RESET,b1:STOP,b2:24/12,b3:TEST
};

#define CLOCK  (*(volatile struct st_rtc   *)0x24800000) /* RTC62423  Address */

#define WEEK_VAL	(unsigned char)2		//;月曜日(mon:1)
#define YEAR1_VAL	(unsigned char)0		//;02年
#define YEAR0_VAL	(unsigned char)3
#define MON1_VAL	(unsigned char)0		//;1月
#define MON0_VAL	(unsigned char)4
#define DAY1_VAL	(unsigned char)0		//;1日
#define DAY0_VAL	(unsigned char)1
#define HOUR1_VAL	(unsigned char)0		//;0時
#define HOUR0_VAL	(unsigned char)0
#define MIN1_VAL	(unsigned char)0		//;0分
#define MIN0_VAL	(unsigned char)0
#define SEC1_VAL	(unsigned char)0		//;0秒
#define SEC0_VAL	(unsigned char)0

void rtc_io(void);
int rtc_io_init(void);
int rtc_io_sdate(int,int,int,int);
int rtc_io_gdate(int *,int *,int *,int *);
int rtc_io_stime(int,int,int);
int rtc_io_gtime(int *,int *,int *);

void	wait190(void);
int		stop_hold(void);

#pragma	inline_asm(rtc_io)
static void rtc_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
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
          RTE
          NOP
rtc_exit:
          MOV.L       Lrsxxx,R0   ;rtc_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
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


int rtc_io_err(){
	rsstr("*** rtc_io err ***");
	return(0);
}

int rtc_io_init(){
	unsigned char tmp;
//	rsstr("*** rtc_io init ***");

	if((tmp=(unsigned char)(CLOCK.CTRL_D & 0x01))!=0) goto init_go;
	if((tmp=(unsigned char)(CLOCK.CTRL_E & 0x08))==0) goto init_go;

	if((tmp=(unsigned char)(CLOCK.WEEK  & 0x0f)) >=  7) goto init_go;
	if((tmp=(unsigned char)(CLOCK.YEAR1 & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.YEAR0 & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.MON1  & 0x0f)) >=  2) goto init_go;
	if((tmp=(unsigned char)(CLOCK.MON0  & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.DAY1  & 0x0f)) >=  4) goto init_go;
	if((tmp=(unsigned char)(CLOCK.DAY0  & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.HOUR1 & 0x0f)) >=  3) goto init_go;
	if((tmp=(unsigned char)(CLOCK.HOUR0 & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.MIN1  & 0x0f)) >=  6) goto init_go;
	if((tmp=(unsigned char)(CLOCK.MIN0  & 0x0f)) >= 10) goto init_go;
	if((tmp=(unsigned char)(CLOCK.SEC1  & 0x0f)) >=  6) goto init_go;
	if((tmp=(unsigned char)(CLOCK.SEC0  & 0x0f)) >= 10) goto init_go;

	if((tmp=(unsigned char)(CLOCK.CTRL_F & 0x0f))==0x04) return(0);
init_go:
	CLOCK.CTRL_F = (unsigned char)0x05;
	CLOCK.CTRL_F = (unsigned char)0x06;
/*
 *Set initial time
 */
	CLOCK.WEEK = WEEK_VAL;
	CLOCK.YEAR1 = YEAR1_VAL;
	CLOCK.YEAR0 = YEAR0_VAL;
	CLOCK.MON1 = MON1_VAL;
	CLOCK.MON0 = MON0_VAL;
	CLOCK.DAY1 = DAY1_VAL;
	CLOCK.DAY0 = DAY0_VAL;
	CLOCK.HOUR1 = HOUR1_VAL;
	CLOCK.HOUR0 = HOUR0_VAL;
	CLOCK.MIN1 = MIN1_VAL;
	CLOCK.MIN0 = MIN0_VAL;
	CLOCK.SEC1 = SEC1_VAL;
	CLOCK.SEC0 = SEC0_VAL;

	CLOCK.CTRL_E = (unsigned char)0x08;			//;t1=1,t0=0,ITRPT/STND=0,MASK=0
	CLOCK.CTRL_D = (unsigned char)0x00;			//;HOLD = 0
	CLOCK.CTRL_F = (unsigned char)0x04;			//;TEST=0,24H,STOP=0,RESET=0
	return(0);
//	return(0xfffffcc0);
}

int rtc_io_sdate(
	int	year,
	int	month,
	int	day,
	int	weed
){
	if(stop_hold()!=0) return(-1);		//;stop clock

	CLOCK.WEEK = (unsigned char)weed;			//;曜日

	CLOCK.YEAR1 = (unsigned char)(year/10);
	CLOCK.YEAR0 = (unsigned char)(year%10);			//;年
	CLOCK.MON1 = (unsigned char)(month/10);
	CLOCK.MON0 = (unsigned char)(month%10);			//;月
	CLOCK.DAY1 = (unsigned char)(day/10);
	CLOCK.DAY0 = (unsigned char)(day%10);			//;日

	CLOCK.CTRL_E = (unsigned char)0x08;			//;t1=1,t0=0,ITRPT/STND=0,MASK=0
	CLOCK.CTRL_D = (unsigned char)0x00;			//;HOLD = 0
	CLOCK.CTRL_F = (unsigned char)0x04;			//;TEST=0,24H,STOP=0,RESET=0
	return(0);
}

int rtc_io_gdate(
	int	*year,
	int	*month,
	int	*day,
	int	*week
){
	if(stop_hold()!=0) return(-1);		//;stop clock

	*year  = (CLOCK.YEAR1 & 0x0f)*10 + (CLOCK.YEAR0 & 0x0f);			//;年
	*month = (CLOCK.MON1  & 0x0f)*10 + (CLOCK.MON0  & 0x0f);			//;月
	*day   = (CLOCK.DAY1  & 0x0f)*10 + (CLOCK.DAY0  & 0x0f);			//;日
	*week = CLOCK.WEEK & 0x0f;
	CLOCK.CTRL_D = (unsigned char)0x00;			//;HOLD = 0

	return(0);
}

int rtc_io_stime(
	int	hour,
	int	min,
	int	sec
){
	if(stop_hold()!=0) return(-1);		//;stop clock

	CLOCK.HOUR1 = (unsigned char)(hour/10);
	CLOCK.HOUR0 = (unsigned char)(hour%10);			//;時
	CLOCK.MIN1 = (unsigned char)(min/10);
	CLOCK.MIN0 = (unsigned char)(min%10);			//;分
	CLOCK.SEC1 = (unsigned char)(sec/10);
	CLOCK.SEC0 = (unsigned char)(sec%10);			//;秒

	CLOCK.CTRL_E = (unsigned char)0x08;			//;t1=1,t0=0,ITRPT/STND=0,MASK=0
	CLOCK.CTRL_D = (unsigned char)0x00;			//;HOLD = 0
	CLOCK.CTRL_F = (unsigned char)0x04;			//;TEST=0,24H,STOP=0,RESET=0
	return(0);
}

int rtc_io_gtime(
	int	*hour,
	int	*min,
	int	*sec
){
	if(stop_hold()!=0) return(-1);		//;stop clock

	*hour = (CLOCK.HOUR1 & 0x0f)*10 + (CLOCK.HOUR0 & 0x0f);			//;時
	*min  = (CLOCK.MIN1  & 0x0f)*10 + (CLOCK.MIN0  & 0x0f);			//;分
	*sec  = (CLOCK.SEC1  & 0x0f)*10 + (CLOCK.SEC0  & 0x0f);			//;秒

	CLOCK.CTRL_D = (unsigned char)0x00;			//;HOLD = 0
	return(0);
}


int
stop_hold(){
	int	cx;
	unsigned char tmp;

	cx = 10;						//;あきらめ回数
lp1:
	CLOCK.CTRL_D = (unsigned char)0x01;			//hold
	if((tmp=(unsigned char)(CLOCK.CTRL_D & 0x02))==0) return(0);			//;busy ?
	if((--cx)<=0) return(-1);
	CLOCK.CTRL_D = (unsigned char)0x00;			//hold = 0;
	wait190();
	goto lp1;
}


void
wait190(){
	int	i;
	int	a = 0;

//	for(i=0;i<600;++i) rstwdt();
	for(i=0;i<6000;++i) a++;
}


