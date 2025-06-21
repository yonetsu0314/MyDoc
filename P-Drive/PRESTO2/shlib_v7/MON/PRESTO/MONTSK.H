/*======================================================================
 * File name    : montsk.h
 * Original	: montsk.h Rev-2.1
 *----------------------------------------------------------------------
 * Function 	:
 *	モニター関係の関数プロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/wcu/main/dz_wcu2/rcs/montsk.h 1.1 1996/06/18 18:42:52 chimura Exp $
 *$Log: montsk.h $
 * Revision 1.1  1996/06/18  18:42:52  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/wcu/main/dz_wcu/rcs/montsk.h 1.1 1994/01/11 18:36:24 kabumoto
 *Log: montsk.h
 * リビジョン 1.1  1994/01/11  18:36:24  kabumoto
 *----------------------------------------------------------------------
 *
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

/*#define	_ADC_MODE	(0x1 << 1) bit 1: data convert use	*/


int		setup_mon_tsk(void);
int		set_sampl_sta_evf(void);
int		sta_stb_chk_tsk(void);

int		init_sampl_sta_evf(void);
int		del_sampl_sta_evf(void);

#if	!defined(P5385)
#define		setup_mon_tsk()		((void)0)
#define		set_sampl_sta_evf()	((void)0)
#define		sta_stb_chk_tsk()	((void)0)

#define		init_sampl_sta_evf()	((void)0)
#define		del_sampl_sta_evf()	((void)0)
#endif
