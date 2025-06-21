/*----------------------------------------------------------------------*
 * File name	: rtcctl.h	/ RTC control data define		*
 * Original	: p:/lib/drive/rtc/rcs/rtcctl.h 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/rtc/RCS/rtcctl.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rtcctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * リアルタイム・クロック
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY			*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#ifdef	STORAGE
unsigned int	rtc_sem_id 	= RTC_SEM_ID;
union aap		rtc_sem;				/* rtcｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
unsigned char	rtc_nest	= 0;			/* RD,WR関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
#else
extern unsigned int	rtc_sem_id;
extern union aap		rtc_sem;
extern unsigned char	rtc_nest;
#endif

