/*----------------------------------------------------------------------*
 * File name	: custom.h	/ custom function			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/custom/RCS/custom.h 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: custom.h $
 * rW 1.3  1970/01/01  00:00:00  chimura
 * 2003/01/27 10:00
 * _disableA_enable๐intrpt.hษฺฎ
 * 
 * rW 1.2  1970/01/01  00:00:00  syuji
 * 2002/12/16 16:30
 * QCQฬฤ่`h~B
 * 
 * rW 1.1  1970/01/01  00:00:00  syuji
 * ๚rW
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(CUSTOM_H)
#define	CUSTOM_H

#include	<machine.h>
/* ---> SHINC\INTRPT.H
#ifndef		_disable
#define		_disable( x )	local_disable( x )
#endif
#ifndef		_enable
#define		_enable( x )	local_enable( x )
#endif

#ifndef		disable_interrupt
#define		disable_interrupt()	set_imask(15);
#endif

#ifndef		enable_interrupt
#define		enable_interrupt()	set_imask(0);
#endif
*/
/*------------------------------------------------------*/
/*	f[^ฯท					*/
/*							*/
/*------------------------------------------------------*/

unsigned long	htol(char *);
unsigned short	htoi(char *);
unsigned char	htoc(char *);
void		ltoh(unsigned long, char *);
void		itoh(unsigned short, char *);
void		ctoh(unsigned char, char *);
void		ftoa(double, char *, int, int);
unsigned long	abp2bbp(unsigned char *, int);
int		bbp2abp(char *, char *, int);

#if	!defined(swapc)
void		swapc(char, char);
#define		swapc(x, y)	(y ^= x,  x ^= y,  y ^= x)
#endif

#if	!defined(swapi)
void		swapi(int, int);
#define		swapi(x, y)	(y ^= x,  x ^= y,  y ^= x)
#endif

#if	!defined(hex_ascii)
unsigned char	hex_ascii(unsigned char);
#define		hex_ascii(a)	(unsigned char)(((a)>9)?((a)+'7'):((a)+'0'))
#endif
#if	!defined(ascii_hex)
unsigned char	ascii_hex(unsigned char);
#define		ascii_hex(a)	(unsigned char)(((a)>'9')?((a)-'7'):((a)-'0'))
#endif

/*------------------------------------------------------*/
/*	`FbNฅTฬvZ				*/
/*							*/
/*------------------------------------------------------*/

unsigned char	check_bcc(unsigned char *, int);
unsigned char	cksum_alpha(unsigned char *, int);
unsigned char	cksum_cpl(unsigned char *, int);

/*------------------------------------------------------*/
/*	งไ					*/
/*------------------------------------------------------*/

void	local_disable(void);
void	local_enable(void);
void	local_disable_all(void);
void	local_enable_all(void);


/*------------------------------------------------------*/
/*	ปฬผ						*/
/*							*/
/*------------------------------------------------------*/

int	chrcmp(char *, char, int);
int	setarg(char *, int, char *);
void	*verify_blk_fmemcpy(void *, void *, int);

#endif	/* CUSTOM_H */

