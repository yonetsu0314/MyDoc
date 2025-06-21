/*----------------------------------------------------------------------*
 * File name	: tmrctl.h	/ timer control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/timer/RCS/tmrctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tmrctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* Modification history							*/
/*   	Version :    Date	:   Name				*/
/*   	v1.00   : 06-JUL-2002	: 					*/
/*				  / initial coding			*/
/*----------------------------------------------------------------------*/

#if	!defined(TMRCTL_H)
#define	TMRCTL_H

void	abort(void);
void	timer_dec( struct tmh *, unsigned short);
void	timer_chk( struct tmh *, unsigned long *);


#define	ITEM_NUM	6		/* ﾀｲﾏｰ毎の最大ｱｲﾃﾑ数		*/
#define MAX_TMR		32		/* 1ｱｲﾃﾑのﾀｲﾏｰの最大数		*/

/*--------------------------------------------------------------*/
/*	Software Timerの管理用構造体の定義			*/
/*								*/
/*--------------------------------------------------------------*/

typedef struct	tmh {
	int		no;	/* no of timer register		*/
	unsigned short	*pnt;	/* pointer to software timer	*/
} TMH;


#ifdef	STORAGE
struct tmh	tm5[ITEM_NUM] = 	/* 5ms timer header		*/
			{0, (void *)0};
struct tmh	tm10[ITEM_NUM] =	/* 10ms	   "			*/
			{0, (void *)0,};
unsigned char	item_n[] = 
			{(unsigned char)0, (unsigned char)0};		/* item number			*/
#else
extern struct tmh	tm5[];
extern struct tmh	tm10[];
extern unsigned char	item_n[];
#endif


/*--------------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義				*/
/*								*/
/*--------------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void		*vfp[2];	/* void型farポインタ	*/
	char		*cfp[2];	/* char		"	*/
	int		*ifp[2];	/* int型	"	*/
	long		*lfp[2];	/* long型	"	*/
	unsigned int	ary[2];		/* 16bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]			/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp	vfpp[1]			/* [1]void型farポインタ	*/
#define	cfp	cfpp[1]			/* [1]char	"	*/
#define	ifp	ifpp[1]			/* [1]int型	"	*/
#define	lfp	lfpp[1]			/* [1]long型	"	*/
#endif

#ifdef	STORAGE
union aap	tmr_tsk;			/* WRﾀｽｸ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
union aap	tmr_ds = { 0, 0};		/* ﾄﾞﾗｲﾊﾞｰのseg情報	*/

#else
extern union aap	tmr_tsk;
extern union aap	tmr_ds;
#endif

#endif	/* TMRCTL_H */
