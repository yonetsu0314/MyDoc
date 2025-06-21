/*----------------------------------------------------------------------*
 * File name	: piodrva.h	/ pio driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/piodrva.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: piodrva.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PIODRV_H)
#define	PIODRV

/*------------------------------------------------------*/
/*	PIOドライバーで管理可能な割り込み		*/
/*							*/
/*------------------------------------------------------*/

enum pioip {
 	SINTP0,				/* Signal Interrupt no.0	*/
 	SINTP1,				/*   "		       1	*/
 	SINTP2,				/*   "	 	       2	*/
	SINTP3,				/*   "		       3	*/
	SINTP4,				/*   "		       4	*/
	SINTP5,				/*   "		       5	*/
	SINTP6,				/*   "		       6	*/
	SINTP7,				/*   "		       7	*/
	EXSIG_MAX			/* 使用可能な割り込み最大数	*/
};


void		sig0_int_hdr(void);	/* signal 0 interrupt handler	*/
void		sig1_int_hdr(void);	/* signal 1 interrupt handler	*/
void		sig2_int_hdr(void);	/* signal 2 interrupt handler	*/
void		sig3_int_hdr(void);	/* signal 3 interrupt handler	*/
void		sig4_int_hdr(void);	/* signal 4 interrupt handler	*/
void		sig5_int_hdr(void);	/* signal 5 interrupt handler	*/
void		sig6_int_hdr(void);	/* signal 6 interrupt handler	*/
void		sig7_int_hdr(void);	/* signal 7 interrupt handler	*/

int		pio_sys_init(enum pioip, union aap *);
int		centro(int, unsigned char *);
int		pio_bit_write(int, int, int, int);
unsigned char	pio_rd_buf(int, int, int);

#endif
