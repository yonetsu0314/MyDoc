/*----------------------------------------------------------------------*
 * File name	: e2sctxt.h		/ serial e2rom context header			*
 * Original	: p:/dacs_i/shdsplib/drive/e2srom/rcs/e2sctxt.h 1.1
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/e2srom/RCS/e2sctxt.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: e2sctxt.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2002, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

#include	<custom\typedefs.h>
#include	<biosif\eesrom.h>

int		e2s_rom_cmd_write(WORD cmd, .../* WORD adr, BYTE *dat */);
int		e2s_rom_chk_ready(void);

/*----------------------------------------------------------*/
/*	Addr情報を含むｺﾏﾝﾄﾞ体系[93C46(56)]						*/
/*----------------------------------------------------------*/
/*   B15  B14  B13  B12  B11  B10   B9   B8   B7	B6-0	*/
/*	| ST | x  | x  | A5 | A4 | A3 | A2 | A1 | A0 |	NULL  |	*/
/*	 "1"			(A6) (A5) (A4)  …   …   …  <shf_cnt>	*/
/*----------------------------------------------------------*/
/*	Addr情報を含まないｺﾏﾝﾄﾞ体系[93C46]						*/
/*----------------------------------------------------------*/
/*   B15  B14  B13  B12  B11   B10-0						*/
/*	| ST | x  | x  | x  | x  |	NULL  |						*/
/*	 "1"													*/
/*----------------------------------------------------------*/

#define	DATA		(1)						/* dataの有無				*/
#define	SHF_93C46	(7)						/* Addrｼﾌﾄ回数(64x16 bit)	*/
#define	SHF_93C56	(6)						/* Addrｼﾌﾄ回数(128x16 bit)	*/

#define	READ		(0xc000)				/* Read Command				*/
#define	WRITE		(0xa000+DATA)			/* Write Command			*/
#define	WRAL		(0x8800+DATA)			/* WRAL Command				*/
#define	ERASE		(0xe000)				/* Erase Command			*/
#define	ERAL		(0x9000)				/* ERAL Command				*/
#define	EWEN		(0x9800)				/* Write Enable Command		*/
#define	EWDS		(0x8000)				/* Write Disable Command	*/

#define	BYTEACCESS		(0)					/* e2s byte access			*/

/*----------------------------------------------*/
/*		ﾃﾞﾊﾞｲｽ情報の構造体						*/
/*												*/
/*----------------------------------------------*/

struct ctxt {
	struct eesp		fnc;					/* I/O操作関数				*/
	unsigned short	shf_cnt;				/* address shift count		*/
};

#ifdef	STORAGE
struct ctxt		e2sr_io;					/* EEPROM phygical 情報		*/
#else
extern struct ctxt	e2sr_io;
#endif
