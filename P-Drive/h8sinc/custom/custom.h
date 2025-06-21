/*
 *==========================================================================
 *	Filename	: CUSTOM.H
 *--------------------------------------------------------------------------
 *	Function	: custom function
 *--------------------------------------------------------------------------
 *$Header: p:/h8sinc/custom/RCS/custom.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: custom.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *
 * このファイルは p:/h8inc/custom/RCS/custom.h 2.3 と同じ
 *--------------------------------------------------------------------------
 *			(c) Copyright 1995-2000, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *			TEL (077) 553-4141
 *==========================================================================
 */
#include	<machine.h>

/*--------------------------------------------------------------------------
 *	データ変換
 *--------------------------------------------------------------------------*/
long			htol(void *);
int				htoi(void *);
unsigned char	htoc(void *);
int				ltoh(long, void *);
int				itoh(int, void *);
int				ctoh(unsigned char, void *);
int				ftoa(double, void *, int, int);
unsigned long	abp2bbp(void *, int);
int				bbp2abp(void *, void *, int);

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
	#define			hex_ascii(a)	(unsigned char)(((a)>9)?((a)+'7'):((a)+'0'))
#endif
#if	!defined(ascii_hex)
	unsigned char	ascii_hex(unsigned char);
	#define		ascii_hex(a)	(unsigned char)(((a)>'9')?((a)-'7'):((a)-'0'))
#endif

/*--------------------------------------------------------------------------
 *	チェック･サムの計算
 *--------------------------------------------------------------------------*/
unsigned char	check_bcc(void *, int);
unsigned char	cksum_alpha(void *, int);
unsigned char	cksum_cpl(void *, int);

/*--------------------------------------------------------------------------
 *	割込制御
 *--------------------------------------------------------------------------*/
void			local_disable(void);
void			local_enable(void);
void			local_disable_all(void);
void			local_enable_all(void);

#if !defined(_disable_all)
	void	_disable_all(void);
	#define	_disable_all()		or_ccr((unsigned char)0xC0);
#endif
#if !defined(_enable_all)
	void	_enable_all(void);
	#define	_enable_all()		and_ccr((unsigned char)0x3F);
#endif
#if !defined(_disable)
	void	_disable(void);
	#define	_disable()			set_imask_ccr((unsigned char)1);
#endif
#if !defined(_enable)
	void	_enable(void);
	#define	_enable()			set_imask_ccr((unsigned char)0);
#endif
#if	!defined(disable_interrupt)
	void	disable_interrupt(unsigned char);
	#define	disable_interrupt(_i)\
			_i=get_imask_ccr();	set_imask_ccr((unsigned char)1);
#endif
#if	!defined(enable_interrupt)
	void	enable_interrupt(unsigned char);
	#define	enable_interrupt(_i)\
			if(_i==0){ set_imask_ccr((unsigned char)0); }
#endif

/*--------------------------------------------------------------------------
 *	その他
 *--------------------------------------------------------------------------*/
int				chrcmp(void *, int, int);
int				setarg(char *, int, char *);

/*==========================================================================*/
