/*----------------------------------------------------------------------*
 * File name	: pioctl.h	/ pio control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ppi/RCS/pioctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: pioctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

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
V_FUNC const	sig_int_hdr[] = {
			sig0_int_hdr, sig1_int_hdr, sig2_int_hdr,
			sig3_int_hdr, sig4_int_hdr, sig5_int_hdr
		};
struct ucsinf	sig_icu_inf[] = {
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1},
			{(short)-1,(unsigned char)-1,(unsigned char)-1}
		};
#else
extern V_FUNC const	sig_int_hdr[];
extern union uci	sig_icu_inf[];
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
union aap	*sig_int_tsk[EXSIG_MAX] = {
			(void *)0	/* INTﾀｽｸAAへのﾎﾟｲﾝﾀの配列	*/
		};
union aap	pio_ds = { 0L,0L };	/* I2NETCﾄﾞﾗｲﾊﾞｰのsegment情報	*/
#else
extern union aap	*sig_int_tsk[];
extern union aap	pio_ds;
#endif
