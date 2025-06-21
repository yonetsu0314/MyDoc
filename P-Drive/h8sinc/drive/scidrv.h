/*----------------------------------------------------------------------*
 * File name	: scidrv.h		/ sci driver prototype					*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/scidrv.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: scidrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/


void	sci0_rxint_hdr(void);
void	sci0_txint_hdr(void);
void	sci0_errint_hdr(void);
void	sci1_rxint_hdr(void);
void	sci1_txint_hdr(void);
void	sci1_errint_hdr(void);
void	sci2_rxint_hdr(void);
void	sci2_txint_hdr(void);
void	sci2_errint_hdr(void);

/*----------------------------------------------------------------------*/
/*	<<< argument 2 >>>													*/

typedef struct	scipk {
	int				char_len;					/* ｷｬﾗｸﾀ長				*/
	int				stp_len;					/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数			*/
	int				pari_sel;					/* ﾊﾟﾘﾃｨｰ選択			*/
	int				baud;						/* ﾋﾞｯﾄﾚｰﾄ				*/
	unsigned short	siz_txbuf;					/* 送信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
	unsigned short	siz_rxbuf;					/* 受信ﾊﾞｯﾌｧ･ｻｲｽﾞ		*/
} SCIPK;

int		sci_sys_init(int, const struct scipk *);
