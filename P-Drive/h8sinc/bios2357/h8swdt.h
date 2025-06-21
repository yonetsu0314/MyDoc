/*----------------------------------------------------------------------*
 * File name	: h8swdt.h		/ H8S WDT context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8swdt.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8swdt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SWDT_H)
#define		H8SWDT_H

#define WDT_TCSR    (*(volatile unsigned char *)0xFFFFBC) /* WDT */
#define WDT_TCNT    (*(volatile unsigned char *)0xFFFFBC) /* WDT */
#define WDT_RSTCSR   (*(volatile unsigned char *)0xFFFFBE) /* WDT */

#endif
