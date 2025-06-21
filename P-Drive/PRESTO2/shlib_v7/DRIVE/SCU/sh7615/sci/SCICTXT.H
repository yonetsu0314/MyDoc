/*----------------------------------------------------------------------*
 * File name	: scictxt.h		/ sci context header					*
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/sci/rcs/scictxt.h 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/scu/sh7615/sci/RCS/scictxt.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: scictxt.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/24 コメント整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

#include	<sh7615\7615.h>

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

#define		SCIF_CH1	0			/* SCIF Channel 1	*/
#define		SCIF_CH2	1			/* SCIF Channel 2	*/

/*------------------------------------------------------*/
/*		SCIのコンテキスト構造体							*/
/*														*/
/*------------------------------------------------------*/
typedef struct	scif_ctxt {
	unsigned short	ipfw;		/* ICU ipfw I/O address	*/
	unsigned short	io_base;	/* SCIFﾎﾟｰﾄ･ｱﾄﾞﾚｽ		*/
} SCIF_CTXT;

#ifdef	STORAGE
struct scif_ctxt	scif_ctxt_tbl[] = { 0L, 0L };
#else
extern struct scif_ctxt		scif_ctxt_tbl[];
#endif