/*======================================================================
 * File name	: prprm.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	p[^yÑè
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prprm.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prprm.h $
 * rW 1.1  1970/01/01  00:00:00  kawamura
 * úrW
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzprm.h 1.2 1997/12/04 14:27:27 itami Exp
 *log: rzprm.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRPRM_H)
#define	PRPRM_H

enum prprm001e {			/* {[hÔ			*/
	BORD_RCU,			/* qbt			*/
	BORD_ICU,			/* hbt			*/
	BORD_WCU,			/* vbt			*/
	BORD_DUC,			/* ctb			*/
	BORD_EXC,			/* dwb			*/
	BORD_ADC,			/* `cb			*/
	BORD_NHIC,			/* mghb			*/
};

enum prprm002e {			/* f[^ÌíÞ			*/
	AC_VOLTAGE,			/* ACd¹d³			*/
	AC_FREQUENCY,			/* ACd¹üg			*/
	FDC_DSW_INFO,			/* FDCîÂÃÞ¨¯Ìß½²¯Áîñ	*/
	JAW_TIMING,			/* JAW CLOSE TIMING INFO	*/
	FEED_VALUE,			/* üdÊ			*/
	FEED_STD,			/* üdÊÌWÎ·		*/
};

enum prprm003e {			/* zbpóÔ			*/
	HSTS_SELE = 0x0001,		/* Ið				*/
	HSTS_STBL = 0x0002,		/* Àè				*/
	HSTS_EMPTY = 0x0004,		/* ó				*/
	HSTS_AZERO = 0x0008,		/* ©®ë_			*/
	HSTS_ERR = 0x0010,		/* G[			*/
	HSTS_OVSCL = 0x0020,		/* I[o[XP[		*/
	HSTS_PFS = 0x0040,		/* vXtXP[		*/
	HSTS_MFS = 0x0080,		/* }CiXtXP[		*/
	HSTS_4TH = 0x0100,		/* SiÚf[^Ìp		*/
	HSTS_5TH = 0x0200,		/* TiÚf[^Ìp		*/
};

enum prprm004e {			/* zbpÌíÞ			*/
	HOPPER_PH,			/* og				*/
	HOPPER_WH,			/* vg				*/
	HOPPER_BH,			/* ag				*/
	HOPPER_DTH,			/* csg			*/
	HOPPER_TH,			/* sg				*/
	HOPPER_RS,			/* qr				*/
};
#endif
