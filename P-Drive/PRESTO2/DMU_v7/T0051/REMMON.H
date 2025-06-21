/*----------------------------------------------------------------------*
 * File name	:	remmon.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/t0051/rcs/remmon.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: remmon.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#undef	GLOBAL
#ifdef	STORAGE
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


void	remmon(void);
void	comQuest(void);


