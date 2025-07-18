/*======================================================================
 * File name    : rzr015.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	tB[_ì®Á«f[^\¢
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr015.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr015.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * úrW
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr015.h 2.2 1970/01/01 00:00:00 kawamura
 *Log: rzr015.h
 * rW 2.2  1970/01/01  00:00:00  kawamura
 * 2002/07/26  11:36
 * enumÉâtB[_(FEEDER_AF)ðÇÁB
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR015_H)
#define	RZR015_H

enum rzr0153e {				/* tB[_ÌíÞ		*/
	FEEDER_RF,			/* qe				*/
	FEEDER_DF,			/* ce				*/
	FEEDER_CF,			/* be				*/
	FEEDER_AF,			/* AF(assistant feeder)		*/
};

enum rzr0150e {				/* Ê§ä			*/
	PHASE_HALF,			/* ¼g				*/
	PHASE_ALL,			/* Sg				*/
};

enum rzr0151e {				/* õdqrvâ~		*/
	OPTSW_OFF,			/* Èµ				*/
	OPTSW_LLVL,			/* knvx			*/
	OPTSW_HLVL = 3,			/* ghfgx		*/
	OPTSW_LEDGE = 5,		/* knvx			*/
	OPTSW_HEDGE = 7,		/* ghfgx		*/
};

enum rzr0152e {				/* Vb^			*/
	SHUTR_OFF,			/* Èµ				*/
	SHUTR_LOW,			/* knv			*/
	SHUTR_HIGH = 3,			/* ghfg			*/
};

/*------------------------------------------------------*/
/*	tB[_ì®Á«p[^pPbg		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr015t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	fd_assign_ptn;	/* Hopper assign pattern
					   [nof_head/4]			*/
	enum rzr0153e	feeder_id;	/* feeder id			*/
	unsigned char	min_amp;	/* minimun aplitude		*/
	unsigned char	max_amp;	/* maximun aplitude		*/
	unsigned char	multiplier;	/* tB[_Ô{¦		*/
	enum rzr0150e	phase_type;	/* Ê				*/
	enum rzr0151e	opt_sw_ctrl;	/* õdqSWÉæéâ~[h	*/
	enum rzr0152e	shutter_ctrl;	/* Vb^A®[h		*/
	unsigned char	shutter_off_delay;/* Vb^â~xêÔ	*/
} RZR015T;

#if	defined(STORAGE)
struct rzr015t	rzr015s;
#else
#if	!defined(NOEXTERN)
extern struct rzr015t	rzr015s;
#endif
#endif

#endif
