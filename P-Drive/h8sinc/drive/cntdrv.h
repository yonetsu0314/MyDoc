/*----------------------------------------------------------------------*
 * File name	: cntdrv.h		/ counter driver prototype				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/cntdrv.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cntdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
#if	!defined(_CNTDRV_H)
#define	_CNTDRV_H

void counter_int( void );

int counter_sys_init(
	int time_base,			/* 1 msec to 50 msec */
	int	no_of_counter		/* No. of counter	 */
);

int get_scnt( int counter_no );
int ena_scnt( int counter_no );
int dis_scnt( int counter_no );
#endif
