/*----------------------------------------------------------------------*
 * File name	: e2rdrv.h	/ e2rom driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/e2srdrv.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2srdrv.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * e2s_romf_...()関数のプロトタイプ宣言を追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(E2SRDRV_H)
#define	E2SRDRV_H

int	e2s_romf_open(int no, int mode);
int	e2s_romf_close(int no);
int	e2s_romf_write(int no, unsigned short *buf, int len);
int	e2s_romf_read(int no, unsigned short *buf, int len);
int	e2s_romf_seek(int no, int offs, int org);
int	e2s_sys_init(unsigned short);

#endif	/* E2SRDRV_H */

