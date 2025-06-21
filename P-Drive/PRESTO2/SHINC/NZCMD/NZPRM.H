/*======================================================================
 * File name    : nzprm.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	パラメータ及び定数等
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzprm.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzprm.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/RCS/nzprm.h 1.2 1970/01/01 00:00:00 itami
 *Log: nzprm.h
 * リビジョン 1.2  1970/01/01  00:00:00  itami
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(NZPRM_H)
#define	NZPRM_H

enum nzprm001e {			/* データの種類			*/
	AC_VOLTAGE,			/* AC電源電圧			*/
	AC_FREQUENCY,			/* AC電源周波数			*/
	FDC_DSW_INFO,			/* FDC基板ﾃﾞｨｯﾌﾟｽｲｯﾁ情報	*/
	JAW_TIMING,			/* JAW CLOSE TIMING INFO	*/
};

#endif
