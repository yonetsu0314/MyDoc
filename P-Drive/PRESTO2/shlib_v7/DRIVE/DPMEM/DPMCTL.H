/*----------------------------------------------------------------------*
 * File name	: dpmctl.h	/ dpmem control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmctl.h 1.2 1993/12/10 21:09:52 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#if	!defined(DPMCTL_H)
#define	DPMCTL_H

int		dpm_open(int devno, int mode);
int		dpm_close(int devno);
int		dpm_read(int devno, unsigned char *buf, int len);
int		dpm_write(int devno, unsigned char *buf, int len);
int		init_dpm_read(int devno);
int		init_dpm_write(int devno);


#define	DPM_DEV_MAX	2		/* DP-SRAMの最大ﾃﾞﾊﾞｲｽ数	*/

#if	!defined(V_FUNC_T)
typedef	void	(*V_FUNC)();		/* voidを返す関数へのﾎﾟｲﾝﾀ	*/
#define	V_FUNC_T

typedef union	uci {
	int		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCI;
#endif

typedef struct	ucsinf {
	short		i;		/* wordでｱｸｾｽする場合		*/
	unsigned char	uc[2];		/* [0]:low-byte [1]:high-byte	*/
} UCSINF;

#ifdef	STORAGE
struct ucsinf	dpm_icu_inf[] = {
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1}
		};
V_FUNC const	dpm_int_hdr[] = {
			dpm0_int_hdr, dpm1_int_hdr,
		};
int		dpm_mode[] = {
			-1, -1		/* DPRAMのｱｸｾｽ･ﾓｰﾄﾞ		*/
		};
#else
extern struct ucsinf	dpm_icu_inf[];
extern V_FUNC const	dpm_int_hdr[];
extern int		dpm_mode[];
#endif

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

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
union aap	*dpm_rx_tsk[DPM_DEV_MAX] = {
		(void *)0, (void *)0	/* RXﾀｽｸAAの配列へのﾎﾟｲﾝﾀの配列	*/
};
long		dpm_poll_time = 2L;	/* ﾎﾟｰﾘﾝｸﾞ指定時間(ms)		*/
long		dpm_time_out = 1000L;	/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)		*/
union aap	dpm_ds = { 0L,0L };	/* I2NETCﾄﾞﾗｲﾊﾞｰのsegment情報	*/
#else
extern union aap	*dpm_rx_tsk[];
extern long		dpm_poll_time;
extern long		dpm_time_out;
extern union aap	dpm_ds;
#endif

#endif	/* DPMCTL_H */

