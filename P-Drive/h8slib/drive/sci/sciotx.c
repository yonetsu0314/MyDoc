/*----------------------------------------------------------------------*
 * File name	: sciotx.c		/ sci transmit control					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/drive/sci/rcs/sciotx.c 1.1 1996/01/18 21:50:03 kabumoto Exp $
 *$Log: sciotx.c $
 * ���r�W���� 1.1  1996/01/18  21:50:03  kabumoto
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/* ����  		                                                		*/
/*		���M������s���B���������Ƃ��āA���M���̔��f��SCI��SCR.BIT		*/
/*		.TIE == 1�ōs���B���M���荞�݋��^�֎~��SCR.BIT.TE���g�p		*/
/*		����ƁA�V���A���|�[�g�̏�Ԃ��ω����邽�߁A�v���ӁB			*/
/*		1.���M���܂��͑��M���f��ԂłȂ��A�����M�����ް��������		*/
/*	      SCI�̃g�����X�~�b�^�̃��f�B���m�F��A����SCI�ɃL�����N�^		*/
/*		  �̏������݂��s���B											*/
/*		2.�����łȂ��ꍇ�Areturn�i���M���荞�ݓ��ŏ��������j�B		*/
/*----------------------------------------------------------------------*/
/* ������																*/
/*		ch		: channel number										*/
/*		*p		: ͯ�ް�ւ��߲���										*/
/*																		*/
/* �߂�l																*/
/*		void	: ����													*/
/*----------------------------------------------------------------------*/

#include	<custom\custom.h>					/* show custom			*/
#include	"scictxt.h"
#include	"scictl.h"

void
sci_io_tx(
	int				ch,							/* channel no.			*/
	struct comh		*p							/* ͯ�ް�ւ��߲���		*/
	)
{
	volatile struct st_sci*	cp;
	unsigned char			f;

	cp = sci_ctxt_tbl[ch];
	f = cp->SCR.BIT.TIE;
	cp->SCR.BIT.TIE = 0;					/* ���M���荞��disable			*/
	if(!f) {
		if(cp->SSR.BIT.TDRE) {				/* ���M�o�b�t�@����ł������ꍇ */
			cp->TDR = *(p->buff + p->getp);	/* ���ڂP�o�C�g�ڂ��������� 	*/
			cp->SSR.BIT.TDRE = 0;			/* ���M�Z�b�g 					*/
			p->getp = ++p->getp & p->buf_max;
		}
	}
	cp->SCR.BIT.TIE = 1;					/* ���M���荞��enable			*/
}
