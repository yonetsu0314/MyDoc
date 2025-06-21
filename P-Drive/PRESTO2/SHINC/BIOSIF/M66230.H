/*----------------------------------------------------------------------*
 * File name	: m66230.h	/ m66230 bios prototype						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/m66230.h 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: m66230.h $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/02/03 TXEMPの定義を追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/02/01 TXDIS を定義する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 2004/12/7 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN							*/
/*		SHIGA JAPAN														*/
/*		(0775) 53-4141													*/
/*----------------------------------------------------------------------*/

#if	!defined(M66230_H)
#define	M66230_H

#define	RXBPE		(0x0001)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	RXBFULL		(0x0002)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	TXBEMP		(0x0004)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	TXEMP		(0x0800)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	ERR_FE		(0x0008)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	ERR_OE		(0x0010)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	ERR_PE		(0x0020)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/
#define	ERR_CRCE	(0x0040)				/*下位8bit ステータス1		*/
											/*上位8bit ステータス2		*/

#define	ALL_ERR		(ERR_FE | ERR_OE | ERR_PE | ERR_CRCE)
#define	ERR_CLR		(0x10)
#define	TXEN		(0x01)
#define	TXDIS		(0x00)

/*----------------------------------------------------------------------*/
int *	_m66230_init(int, int, int, int);
int		_m66230_gst(int);
int		_m66230_sst(int, int);
int		_m66230_putc(int, int);
int		_m66230_getc(int);

#endif
