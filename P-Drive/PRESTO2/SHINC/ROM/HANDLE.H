/*----------------------------------------------------------------------*
 * File name	: handle.h						*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/rom/RCS/handle.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: handle.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * FIFOﾊﾝﾄﾞﾙ追加
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

#if	!defined(HANDLE_H)
#define	HANDLE_H

enum {
/*---------------------------------------------*/
	__FILE_STRUC_DEV,			/* ﾌｧｲﾙ構造を持つﾃﾞﾊﾞｲｽ	*/
	__E2ROM_,				/* EE-PROMﾃﾞﾊﾞｲｽ･ﾀｲﾌﾟ名	*/
	__SRAM_,				/* SRAM		"	*/
	__FLASHROM_,				/* FLAASH ROM	"	*/
	__EXRAM_,				/* EXT-MEM	"	*/
/*---------------------------------------------*/
	__CHR_STREAM_DEV = __EXRAM_,		/* charｽﾄﾘｰﾑ形のﾃﾞﾊﾞｲｽ	*/
	__DPMEM_,				/* DP-SRAMﾃﾞﾊﾞｲｽ･ﾀｲﾌﾟ名	*/
	__SCU_,					/* SCU 		"	*/
	__MPSC_,				/* MPSC		"	*/
	__ARCNET_,				/* ARCNET	"	*/
	__I2NET_,				/* I2NET	"	*/
	__ADARC_,				/* ADV-ARC	"	*/
	__PRN_,					/* PRINTER	"	*/
	__FIFO_,				/* FPGA-FIFO	"	*/
/*---------------------------------------------*/
	__KEY_MAX_,
};

#define	__FILE_KEY_MAX_	(__CHR_STREAM_DEV - __FILE_STRUC_DEV)


/*----------------------------------------------*/
/*	file name				*/
/*						*/
/*----------------------------------------------*/

/*	"e2rom"			EE-PROMのﾌｧｲﾙ･ﾈｰﾑ			*/
/*	"sram"			S-RAM		"			*/
/*	"from"			FLASH-ROM	"			*/
/*	"exram"			外部ﾒﾓﾘｰ	"			*/
/*	"dpmem"			ﾃﾞｭｱﾙ･ﾎﾟｰﾄ･ﾒﾓﾘｰ	"			*/
/*	"con"			SCU		"			*/
/*	"tty"			MPSC		"			*/
/*	"lan"			ARCNET		"			*/
/*	"inet"			I2NET		"			*/
/*	"com"			ADV-ARC		"			*/
/*	"prn"			PRINTER		"			*/
/*	"fifo"			FPGA-FIFO	"			*/

#endif	/* HANDLE_H */

