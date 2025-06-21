/*----------------------------------------------------------------------*
 * File name	: scictxt.h		/ sci context header					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scictxt.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scictxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

#include	<bios2357\h8ssci.h>

#define		TIE_BP		0x80
#define		RIE_BP		0x40
#define		TXE_BP		0x20
#define		RXE_BP		0x10
#define		TDRE_BP		0x80
#define		RDRF_BP		0x40
#define		RERF_BP		0x38
#define		RRXF_BP		(RDRF|RERF)

#define		BIT8		0x00
#define		BIT7		0x40
#define		STOP2		0x08
#define		STOP1		0x00
#define		NON			0x00
#define		EVEN		0x20
#define		ODD			0x30

#define		CH0		0		/* SCI Channel 0	*/
#define		CH1		1		/* SCI Channel 1	*/
#define		CH2		2		/* SCI Channel 2	*/

/*------------------------------------------------------*/
/*		SCIのコンテキスト構造体							*/
/*														*/
/*------------------------------------------------------*/

#ifdef	STORAGE

volatile struct st_sci* const sci_ctxt_tbl[] = { SCI0ADRS, SCI1ADRS, SCI2ADRS };
#else

extern volatile struct st_sci* const sci_ctxt_tbl[];
#endif
