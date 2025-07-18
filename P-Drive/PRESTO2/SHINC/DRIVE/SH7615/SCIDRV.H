/*----------------------------------------------------------------------*
 * File name	: scidrv.h		/ sci driver prototype					*
 * Original		: p:/h8inc/drive/rcs/scidrv.h
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/sh7615/RCS/scidrv.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: scidrv.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  sakaguti
 * �r�g�Q�p�r�b�h�h���C�o
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/

#include <kernel.h>


enum {
	SCIF1,				/* SCIF ch-1 number	*/
	SCIF2,				/* SCIF ch-2 number	*/
	SCIF_MAX
};

INTHDR sci1_errint_hdr(void);
INTHDR sci1_rxint_hdr(void);
INTHDR sci1_txint_hdr(void);
INTHDR sci2_errint_hdr(void);
INTHDR sci2_rxint_hdr(void);
INTHDR sci2_txint_hdr(void);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>													*/

typedef struct	scipk {
	int				char_len;					/* ��׸���				*/
	int				stp_len;					/* �į�ߥ�ޯĐ�			*/
	int				pari_sel;					/* ���è��I��			*/
	int				baud;						/* �ޯ�ڰ�				*/
	unsigned short	siz_txbuf;					/* ���M�ޯ̧�����		*/
	unsigned short	siz_rxbuf;					/* ��M�ޯ̧�����		*/
} SCIPK;

int		scif_sys_init(int, const struct scipk *, union aap *);
