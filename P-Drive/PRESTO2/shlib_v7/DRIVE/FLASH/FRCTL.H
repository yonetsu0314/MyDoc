/*----------------------------------------------------------------------*
 * File name	: frctl.h	/ flash rom control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/flash/rcs/frctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: frctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(FRCTL_H)
#define	FRCTL_H

int		flashromf_open(int no, int mode);
int		flashromf_close(int no);
int		flashromf_write(int no, unsigned char *buf, int len);
int		flashromf_read(int no, unsigned char *buf, int len);
int		flashromf_seek(int no, int offs, int org);

int		init_flashrom_tsk(void);

/*------------------------------------------------------*/
/*	FLASH ROMのR/W管理用ヘッダー部の定義		*/
/*							*/
/*------------------------------------------------------*/

typedef struct	frh {
/*-------------------------------------*/
	unsigned int	f_flag;		/* file flag (open/crose)	*/
#define	F_OPEN		(0x1 << 2)	/* file open bit assign		*/
#define	F_CLOSE		0		/* file crose 	"		*/
/*-------------------------------------*/
	unsigned int	frf_p;		/* flash rom file pointer	*/
/*-------------------------------------*/
	unsigned int	flag;		/* flash rom WR status flag	*/
#define	WR_ON		(0x1 << 0)	/* write bit assign		*/
#define	BUFF_FULL	(0x1 << 1)	/* buffer full bit assign	*/
/*-------------------------------------*/
	unsigned int	putp;		/* put pointer			*/
	unsigned int	getp;		/* get pointer			*/
	unsigned int	buf_max;	/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰのｻｲｽﾞ		*/
	unsigned char	*buff;		/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀｰ	*/
} FRH;


#ifdef	STORAGE
int		flashrom_mode = -1;
struct frh	flashrom_hd = {		/* define flash rom header	*/
			0, 0, 0, 0, 0, 0, (void *)0
};
int const	file_size[] = {
			1,		/* type 8K byte			*/
			2		/* type 32K byte		*/
};
#else
extern int		flashrom_mode;
extern struct frh	flashrom_hd;
extern int const	file_size[];
#endif


/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#define	FR_SEM_CNT	(unsigned int)1		/* ｾﾏﾌｫの資源数		*/

#ifdef	STORAGE
union aap	fr_sem;				/* ｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
union aap	frbuf_sem;			/* buffｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
long		fr_time_out = 2000;		/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)	*/
union aap	frwr_tsk;			/* WRﾀｽｸ･ｱｸｾｽ･ｱﾄﾞﾚｽ	*/
union aap	fr_ds = { 0L, 0L };			/* ﾄﾞﾗｲﾊﾞｰのseg情報	*/

#else
extern union aap	fr_sem;
extern union aap	frbuf_sem;
extern long		fr_time_out;
extern union aap	frwr_tsk;
extern union aap	fr_ds;
#endif

#endif	/* FRCTL_H */
