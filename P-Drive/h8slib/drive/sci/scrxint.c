/*----------------------------------------------------------------------*
 * File name	: scrxint.c		/ sci RX interrupt handler				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scrxint.c 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scrxint.c $
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
/*		SCI��M���荞�ݏ���												*/
/*----------------------------------------------------------------------*/

#include	"scictxt.h"
#include	"scictl.h"

/*----------------------------------------------------------------------*/
/*		write_fifo														*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		*p		: ��M�Ǘ��pͯ�ނւ��߲��								*/
/*		c		: ��M�ް�												*/
/*																		*/
/* �߂�l																*/
/*		int		: N	�c	text��M����									*/
/*				  0	�c	text�������Ă��Ȃ�								*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		��M�f�[�^�������O�o�b�t�@�ɏ������ށB�֐����͊��荞�݋֎~		*/
/*		��Ԃ�O��ɏ������s����B									*/
/*----------------------------------------------------------------------*/

static int
write_fifo(
	struct comh		*p,
	unsigned char	c
	)
{
	unsigned char		*rp;				/* ring buffer�ւ��߲���	*/
	unsigned short		i, j, max;

	rp = p->buff;
	i = p->putp;
	max = p->buf_max;
	switch(c)	{
		case DEL:
		case BS:
				if(i != p->getp) {
					i = --i & max;
					if(*(rp+i) != CR)  p->putp = i;
				}
				break;
		case CTLX:
				while(i != p->getp) {
					i = --i & max;
					if(*(rp+ i) != CR) {
						p->putp = i;  break;
					}
				}
				*(rp+ i) = c;
				p->putp = ++i & max;
				if(i == p->getp) {
					p->putp = ++i & max;
					*(rp+ i) = CR;					return(++p->lcnt);
				}
				c = CR;
		case CR:
		case LF:
		case HT:
		default:
				*(rp+ i) = c;
				p->putp = ++i & max;
				if(c == CR)							return(++p->lcnt);
				else {
					if(i == p->getp) {
						p->putp = ++i & max;
						*(rp+ i) = CR;				return(++p->lcnt);
					}
				}
				break;
	}
	return(0);
}

/*----------------------------------------------------------------------*/
/*		sci_rxint_hdr													*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		void		: ����												*/
/*																		*/
/* �߂�l																*/
/*		void		: ����												*/
/*----------------------------------------------------------------------*/
/* ����                                                  				*/
/*		��M���荞�݊֐�												*/
/*		��M�f�[�^�������O�o�b�t�@�ɏ�������							*/
/*----------------------------------------------------------------------*/

#pragma interrupt(sci0_rxint_hdr)				/* SCI-ch0���荞�������	*/
void	sci0_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH0];
	i = write_fifo(&scrx[CH0], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}


#pragma interrupt(sci1_rxint_hdr)				/* SCI-ch1���荞�������	*/
void	sci1_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH1];
	i = write_fifo(&scrx[CH1], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}

#pragma interrupt(sci2_rxint_hdr)				/* SCI-ch2���荞�������	*/
void	sci2_rxint_hdr(void)
{
	volatile struct st_sci*	p;
	int						i;

	p = sci_ctxt_tbl[CH2];
	i = write_fifo(&scrx[CH2], p->RDR);
	p->SSR.BIT.RDRF = 0;						/* Rx IREQ reset 		*/
}
