/*----------------------------------------------------------------------*
 * File name	: sctxint.c		/ sci TX interrupt handler				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/sctxint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: sctxint.c $
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
/*		SCI���M���荞�ݏ���												*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		read_fifo														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*p		: ���M�Ǘ��pͯ�ނւ��߲��								*/
/*																		*/
/* �߂�l																*/
/*		int 	:	�ǂݏo�����L�����N�^ �i���̒l�j						*/
/*					�o�b�t�@����̎��� ���̒l							*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		���M�p�����O��o�b�t�@�̓��e���P�����ǂݏo���B					*/
/*		�֐����͊��荞�݋֎~��O��ɏ������s����B					*/
/*----------------------------------------------------------------------*/

static int
read_fifo(
	struct comh		*p
	)
{
	unsigned short	i;
	unsigned char	c;

	if((i = p->getp) == p->putp)					return(-1);
	c = *(p->buff+ i);
	p->getp = (++i & p->buf_max);
	return((int)c);
}

/*----------------------------------------------------------------------*/
/*		sci_txint_hdr													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		void		: ����												*/
/*																		*/
/* �߂�l																*/
/*		void		: ����												*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		���M���荞�݊֐�												*/
/*		�����O�o�b�t�@�ɏ������܂ꂽ�f�[�^�𑗐M����					*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_txint_hdr)				/* SCI-ch0���荞�������	*/
void	sci0_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH0];
	if((i = read_fifo(&sctx[CH0])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}


#pragma interrupt(sci1_txint_hdr)				/* SCI-ch1���荞�������	*/
void	sci1_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH1];
	if((i = read_fifo(&sctx[CH1])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}

#pragma interrupt(sci2_txint_hdr)				/* SCI-ch2���荞�������	*/
void	sci2_txint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH2];
	if((i = read_fifo(&sctx[CH2])) < 0)
		p->SCR.BIT.TIE = 0;						/* Tx interrupt disable */
	else {
		p->TDR = i;
		p->SSR.BIT.TDRE = 0;					/* Tx IREQ reset 		*/
	}
}
