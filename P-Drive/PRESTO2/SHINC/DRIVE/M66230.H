/*----------------------------------------------------------------------*
 * File name	: m66230.h	/ m66230 driver prototype					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/rcs/m66230.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: m66230.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 04.12.09 新規追加
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.				*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/


#if	!defined(M66230DRV_H)
#define	M66230DRV_H

void	m66230_int_hdr0(void);
void	m66230_int_hdr1(void);

typedef struct	m66230pk {
	int				char_len;		/* ｷｬﾗｸﾀ長			*/
	int				stp_len;		/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int				pari_sel;		/* ﾊﾟﾘﾃｨｰ選択		*/
	int				baud;			/* ﾋﾞｯﾄﾚｰﾄ			*/
	unsigned short	siz_txbuf;		/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
	unsigned short	siz_rxbuf;		/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ	*/
} M66230PK;

int		m66230_sys_init(int, const struct m66230pk *, union aap *);

#endif	/* M66230DRV_H */

