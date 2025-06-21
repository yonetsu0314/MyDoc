/*----------------------------------------------------------------------*
 * File name	: e2sctl.h		/ serial e2rom control data define		*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2sctl.h 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2sctl.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2sctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

int		e2s_romf_open(int no, int mode);
int		e2s_romf_close(int no);
int		e2s_romf_write(int no, unsigned short *buf, int len);
//int		e2s_romf_write(int no, unsigned char *buf, int len);
int		e2s_rom_io_write(void);
int		e2s_romf_read(int no, unsigned short *buf, int len);
//int		e2s_romf_read(int no, unsigned char *buf, int len);
int		e2s_romf_seek(int no, int offs, int org);

/*------------------------------------------------------*/
/*		EEPROMのR/W管理用ヘッダー部の定義				*/
/*														*/
/*------------------------------------------------------*/

typedef struct	eeh {
/*-------------------------------------*/
	unsigned short	f_flag;				/* file flag (open/crose)	*/
#define	F_OPEN		(0x1 << 2)			/* file open bit assign		*/
#define	F_CLOSE		0					/* file crose 	"			*/
/*-------------------------------------*/
	unsigned short	e2f_p;				/* e2rom file pointer		*/
/*-------------------------------------*/
	unsigned short	flag;				/* e2rom WR status flag		*/
#define	WR_ON		(0x1 << 0)			/* write bit assign			*/
/*-------------------------------------*/
	unsigned short	putp;				/* put pointer				*/
	unsigned short	getp;				/* get pointer				*/
	unsigned short	buf_max;			/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰのｻｲｽﾞ		*/
	unsigned short	*buff;				/* ﾘﾝｸﾞ･ﾊﾞｯﾌｧｰへのﾎﾟｲﾝﾀｰ	*/
} EEH;


#ifdef	STORAGE
int			e2srom_mode = -1;
struct eeh	e2srom_hd = {				/* define e2rom header		*/
			(unsigned short)0, (unsigned short)0, (unsigned short)0,
			(unsigned short)0, (unsigned short)0, (unsigned short)0, (void *)0
};
int const	file_ssize[] = {
			1,							/* type 1K bit				*/
			2							/* type 2K bit				*/
};
#else
extern int			e2srom_mode;
extern struct eeh	e2srom_hd;
extern int const	file_ssize[];
#endif
