/*----------------------------------------------------------------------*
 * File name	: scerint.c		/ sci Error interrupt handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scerint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scerint.c $
 * ���r�W���� 1.1  1970/01/01  00:00:00  yonetsu
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* ����                         		                         		*/
/*		SCI�G���[���荞�ݏ���											*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		sci_errint_hdr													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		void		: ����												*/
/*																		*/
/* �߂�l																*/
/*		void		: ����												*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		��M�G���[���荞�ݏ���											*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_errint_hdr)				/* SCI-ch0���荞�������	*/
void	sci0_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH0];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}


#pragma interrupt(sci1_errint_hdr)				/* SCI-ch1���荞�������	*/
void	sci1_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH1];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}

#pragma interrupt(sci2_errint_hdr)				/* SCI-ch2���荞�������	*/
void	sci2_errint_hdr(void)
{
	volatile struct st_sci*	p;
	unsigned char			c;

	p = sci_ctxt_tbl[CH2];
	c = p->SSR.BYTE & 0x38;						/* error flag read 		*/
	p->SSR.BIT.RDRF = 0;						/* RxRDY flag clear 	*/
	p->SSR.BYTE &= ~c;							/* Error IREQ reset 	*/
}
