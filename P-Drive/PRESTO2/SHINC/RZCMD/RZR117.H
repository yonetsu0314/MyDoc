/*======================================================================
 * File name    : rzr117.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	^C~Of[^g£p[^f[^\’
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzr117.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr117.h $
 * rW 1.1  1970/01/01  00:00:00  chimura
 * ϊrW
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzr117.h 1.2 1995/05/15 17:50:09 sakagami
 *Log: rzr117.h
 * rW 1.2  1995/05/15  17:50:09  sakagami
 * T[{γΐβ~ΤΜΗΑ
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZR117_H)
#define	RZR117_H

/*------------------------------------------------------*/
/*	^C~Op[^pPbgig£ͺj		*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzr117t {
	unsigned char	cnv_delay;	/* ΐΊέΝήΤ  xκΤ@*/
	unsigned char	cnv_hold_tm;	/* ΐΊέΝήΤ@IΤ@*/
	unsigned char	srv_delay;	/* rop»°Ξή xκΤ  */
	unsigned char	srv_hold_tm;	/* »°Ξή@ γΐβ~Τ  */
	unsigned char	sv_delay;	/* ropΌΨέΐήxκΤ	*/
	unsigned char	sv_hold_tm;	/* ropΌΨέΐήIΤ@*/
	unsigned char	men1_delay;	/* Λ΅PxκΤ@@ */
	unsigned char	men1_hold_tm;	/* Λ΅PIΤ     */
	unsigned char	men2_delay;	/* Λ΅QxκΤ@@ */
	unsigned char	men2_hold_tm;	/* Λ΅QIΤ     */
	unsigned char	men3_delay;	/* Λ΅RxκΤi\υj*/
	unsigned char	men3_hold_tm;	/* Λ΅RIΤi\υj*/
	unsigned char	men4_delay;	/* Λ΅SxκΤi\υj*/
	unsigned char	men4_hold_tm;	/* Λ΅SIΤi\υj*/
} RZR117T;

#if	defined(STORAGE)
struct rzr117t	rzr117s;
#else
#if	!defined(NOEXTERN)
extern struct rzr117t	rzr117s;
#endif
#endif

#endif
