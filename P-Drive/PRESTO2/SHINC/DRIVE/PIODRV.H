/*----------------------------------------------------------------------*
 * File name	: piodrv.h	/ pio driver prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/piodrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: piodrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PIODRV_H)
#define	PIODRV_H

#ifdef	P5280				/* HardwareがP5280の場合	*/
enum {
	MINTP3,				/* Master ICU INTP3		*/
	MINTP4,				/*   "    ICU INTP4			*/
	EXSIG_MAX
};

#else					/* それ以外のHardware		*/
enum {
	SINTP2,				/* Slave ICU INTP2		*/
	SINTP3,				/*   "   ICU INTP3		*/
	SINTP4,				/*   "   ICU INTP4		*/
	SINTP5,				/*   "   ICU INTP5		*/
	SINTP6,				/*   "   ICU INTP6		*/
	SINTP7,				/*   "   ICU INTP7		*/
	EXSIG_MAX
};

void		sig5_int_hdr(void);	/* signal 5 interrupt handler	*/
void		sig4_int_hdr(void);	/* signal 4 interrupt handler	*/
void		sig3_int_hdr(void);	/* signal 3 interrupt handler	*/
void		sig2_int_hdr(void);	/* signal 2 interrupt handler	*/
#endif

void		sig1_int_hdr(void);	/* signal 1 interrupt handler	*/
void		sig0_int_hdr(void);	/* signal 0 interrupt handler	*/

int		pio_sys_init(int, union aap *);
int		centro(int, unsigned char *);

#endif	/* PIODRV_H */

