/*----------------------------------------------------------------------*
 * File name	:	rcutest.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/sh7615/RCS/rcutest.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: rcutest.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＳＨ２用インクルード
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include <machine.h>
#include "kernel.h"

#define	INTVEC (0x06000000)

#define	WAIT1S	3000	/* 1mS loop counter for external ROM */
#define	WAIT01S	300		/* 0.1mS loop counter for external ROM */

extern int RST_VECT;
extern int *_D_ROM, *_D_BGN, *_D_END, *_B_BGN, *_B_END;


#ifdef	GLOBAL_VALUE_DEFINE
#define	GLOBAL	
#else
#define	GLOBAL	extern
#endif

GLOBAL	int	rep;
GLOBAL	int	esc;
GLOBAL	char	*adrp;
GLOBAL	int	er[8],gr[8],gsr;
GLOBAL	int	gpc;
GLOBAL	char	*bkadr;

GLOBAL	int	ledcnt;


void	di(void);
void	ei(void);
int		err_code(int);
//void	assert(unsigned int);
void	rsinit(void);

void BscInitDRAM( void );
void CacheInit( void );
void INITINT( void );
void init_FMR(void);

INTHDR mpsc0_int_hdr(void);
INTHDR mpsc1_int_hdr(void);
INTHDR scu0_int_hdr(void);

INTHDR UBrk(void);
INTHDR GnrlIlgl(void);
INTHDR dummy(void);
INTHDR ADDerror(void);
INTHDR Slot(void);
INTHDR DMAerror(void);

void	ei(void);
void	di(void);

void	main(void);
void	debug(void);
void	remmon(void);

void	_INIT(void);
void	_INITSCT(void);
void	hardinit(void);
void	OOutDev0(char c);

void	hardinit(void);
void	rstwdt(void);
void	wait(int n);				/* wait 1mS * n */
void	wait0(int n);				/* wait 0.1mS * n */
void	wait1(void);				/* wait 1mS */

void	rsstr(char *str);
void	rsout(char c);

void	rslf(void);
int	get2hex(void);
int	get4hex(void);
int	get6hex(void);
int	get8hex(void);

void	put2hex(int);
void	put4hex(int);
void	put6hex(int);
void	put8hex(int);
int		atoi1(char);
void	itoa1(int);
char	rschk(void);
char	rsin(void);

int	chkin(char);
int	chkout(void);
int	chkrep(void);
int	chkesc(void);
int	strcomp(char *,char *);
void	regdsp(void);
void	lodbankrs(void);
void	clearram(void);

void	comA(void);
void	comB(void);
void	comC(void);
void	comD(void);
void	comE(void);
void	comF(void);
void	comG(void);
void	comH(void);
void	comI(void);
void	comJ(void);
void	comK(void);
void	comL(void);
void	comM(void);
void	comN(void);
void	comO(void);
void	comP(void);
void	comQ(void);
void	comR(void);
void	comS(void);
void	comT(void);
void	comU(void);
void	comV(void);
void	comW(void);
void	comX(void);
void	comY(void);
void	comZ(void);

