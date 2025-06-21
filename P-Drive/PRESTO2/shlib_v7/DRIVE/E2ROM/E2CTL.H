/*----------------------------------------------------------------------*
 * File name	: e2ctl.h	/ e2rom control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2rom/RCS/e2ctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2ctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(E2CTL_H)
#define	E2CTL_H

int		e2romf_open(int no, int mode);
int		e2romf_close(int no);
int		e2romf_write(int no, unsigned char *buf, int len);
int		e2romf_read(int no, unsigned char *buf, int len);
int		e2romf_seek(int no, int offs, int org);

int		init_e2rom_tsk(void);

/*------------------------------------------------------*/
/*	EEPROMのR/W管理用ヘッダー部の定義		*/
/*							*/
/*------------------------------------------------------*/

typedef struct	eeh {
/*-------------------------------------*/
	unsigned int	f_flag;		/* file flag (open/crose)	*/
#define	F_OPEN		(0x1 << 2)	/* file open bit assign		*/
#define	F_CLOSE		0		/* file crose 	"		*/
/*-------------------------------------*/
	unsigned int	e2f_p;		/* e2rom file pointer		*/
/*-------------------------------------*/
	unsigned int	flag;		/* e2rom WR status flag		*/
#define	WR_ON		(0x1 << 0)	/* write bit assign		*/
#define	BUFF_FULL	(0x1 << 1)	/* buffer full bit assign	*/
/*-------------------------------------*/
	unsigned int	putp;		/* put pointer			*/
	unsigned int	getp;		/* get pointer			*/
	unsigned int	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰのｻｲｽﾞ		*/
	unsigned char	*buff;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀｰ	*/
} EEH;


#ifdef	STORAGE
int		e2rom_mode = -1;
struct eeh	e2rom_hd = {		/* define e2rom header		*/
			0, 0, 0, 0, 0, 0, (void *)0
};
int const	file_size[] = {
			1,		/* type 8K byte			*/
			2		/* type 32K byte		*/
};
#else
extern int		e2rom_mode;
extern struct eeh	e2rom_hd;
extern int const	file_size[];
#endif


/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#define	E2_SEM_CNT	(unsigned int)1		/* ｾﾏﾌｫの資源数		*/

#ifdef	STORAGE
union aap	e2_sem;				/* ｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
union aap	e2buf_sem;			/* buffｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
long		e2_time_out = 2000;		/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)	*/
union aap	e2wr_tsk;			/* WRﾀｽｸ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
union aap	e2r_ds = { 0L,0L };		/* ﾄﾞﾗｲﾊﾞｰのseg情報	*/

#else
extern union aap	e2_sem;
extern union aap	e2buf_sem;
extern long		e2_time_out;
extern union aap	e2wr_tsk;
extern union aap	e2r_ds;
#endif

#endif	/* E2CTL_H */
