/*----------------------------------------------------------------------*
 * File name	: scidrv.h		/ sci driver prototype					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/scidrv.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scidrv.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  yonetsu
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/


void	sci0_rxint_hdr(void);
void	sci0_txint_hdr(void);
void	sci0_errint_hdr(void);
void	sci1_rxint_hdr(void);
void	sci1_txint_hdr(void);
void	sci1_errint_hdr(void);
void	sci2_rxint_hdr(void);
void	sci2_txint_hdr(void);
void	sci2_errint_hdr(void);

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

int		sci_sys_init(int, const struct scipk *);
