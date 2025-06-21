/*----------------------------------------------------------------------*
 * File name	: sramctl.h	/ sram control data define		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/sram/RCS/sramctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: sramctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/sram/rcs/sramctl.h 1.1 1995/08/24 13:23:42 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(SRAMCTL_H)
#define	SRAMCTL_H

int		sram_open(int id, int mode);
int		sram_close(int id);
int		sram_seek(int id, int offset, int mode);
int		sram_read(int id, unsigned char *buf, int len);
int		sram_write(int id, unsigned char *buf, int len);
int		init_sram(int id);

#define	SRAM_DEV_MAX	2		/* SRAMの最大ﾃﾞﾊﾞｲｽ数	*/

#ifdef	STORAGE
int		sram_mode[] = {
			-1, -1		/* SRAMのｱｸｾｽ･ﾓｰﾄﾞ		*/
		};
#else
extern int		sram_mode[];
#endif

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#ifdef	STORAGE
unsigned int	sram_sem_id[SRAM_DEV_MAX] = {
		SRAM0_SEM_ID, SRAM1_SEM_ID
};
union aap	sram_sem[SRAM_DEV_MAX] = {
		0L			/* sramｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
unsigned char	sram_nest[SRAM_DEV_MAX] = {
		(unsigned char)0	/* RD,WR関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};
#else
extern unsigned int	sram_sem_id[SRAM_DEV_MAX];
extern union aap	sram_sem[SRAM_DEV_MAX];
extern unsigned char	sram_nest[SRAM_DEV_MAX];
#endif

#endif	/* SRAMCTL_H */
