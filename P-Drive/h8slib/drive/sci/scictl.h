/*----------------------------------------------------------------------*
 * File name	: scictl.h		/ sci control data define				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/sci/RCS/scictl.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scictl.h $
 * ���r�W���� 1.1  1970/01/01  00:00:00  yonetsu
 * �������r�W����
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993-2000, ISHIDA  CO., LTD.				*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

int		sci_open(int devno, int mode);
int		sci_close(int devno);
int		sci_rx(int devno, unsigned char *buf, int len);
int		sci_tx(int devno, unsigned char *buf, int len);
void	sci_io_tx(int devno, struct comh *p);


#define		LF		(10)					/* New line 	   :'\n'	*/
#define		CR		(13)					/* Carriage return :'\r'	*/
#define		STX		(2)						/* Start of Text			*/
#define		ETX		(3)						/* End of Text				*/
#define		BS		(8)						/* Backspace				*/
#define		HT		(9)						/* Horizontal Tabulation	*/
#define		ENQ		(5)						/* Enquiry					*/
#define		ACK		(6)						/* Acknowledge				*/
#define		NAK		(0x15)					/* Neagtive Acknowledge		*/
#define		XON		(0x13)					/* Device Control 3			*/
#define		XOFF	(0x11)					/* Device Control 1			*/
#define		CTLC	(0x03)					/* Control C				*/
#define		CTLS	(0x13)					/* Control S				*/
#define		CTLX	(0x18)					/* Control X				*/
#define		ESCP	(0x1b)					/* Escape					*/
#define		DEL		(0x7f)					/* Delete					*/

/*------------------------------------------------------*/
/*		����M�Ǘ��p�̍\���̒�`						*/
/*														*/
/*------------------------------------------------------*/

typedef struct	comh {
	unsigned short	putp;				/* �߯ĥ�߲��					*/
	unsigned short	getp;				/* �ޯĥ�߲��					*/
	unsigned short	lcnt;				/* ÷�ĥ�����(��M��p)			*/
	unsigned short	buf_max;			/* �ݸޥ�ޯ̧�̻���				*/
	unsigned char	*buff;				/* �ݸޥ�ޯ̧�ւ��߲��			*/
} COMH;


#ifdef	STORAGE
struct comh	scrx[] = {					/* SCI��RXͯ�ް��				*/
			0, 0, 0, 0, (void *)0,
			0, 0, 0, 0, (void *)0,
			0, 0, 0, 0, (void *)0
};
struct comh	sctx[] = {					/* SCI��TXͯ�ް��				*/
			0, 0, 0, 0, (void *)0,
			0, 0, 0, 0, (void *)0,
			0, 0, 0, 0, (void *)0
};
int			sci_mode = -1;				/* file access mode				*/
#else

extern struct comh	scrx[];
extern struct comh	sctx[];
extern int		sci_mode;
#endif
