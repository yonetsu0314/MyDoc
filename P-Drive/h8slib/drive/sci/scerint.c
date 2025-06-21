/*----------------------------------------------------------------------*
 * File name	: scerint.c		/ sci Error interrupt handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scerint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scerint.c $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                         		                         		*/
/*		SCIエラー割り込み処理											*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		sci_errint_hdr													*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*		void		: 無し												*/
/*																		*/
/* 戻り値																*/
/*		void		: 無し												*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		受信エラー割り込み処理											*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_errint_hdr)				/* SCI-ch0割り込みﾊﾝﾄﾞﾗ	*/
void	sci0_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH0];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}


#pragma interrupt(sci1_errint_hdr)				/* SCI-ch1割り込みﾊﾝﾄﾞﾗ	*/
void	sci1_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH1];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}

#pragma interrupt(sci2_errint_hdr)				/* SCI-ch2割り込みﾊﾝﾄﾞﾗ	*/
void	sci2_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH2];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}
