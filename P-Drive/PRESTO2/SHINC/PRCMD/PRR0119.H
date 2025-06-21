/*======================================================================
 * File name	: prr0119.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	信号出力先のアサインデータ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0119.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0119.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0119_H)
#define	PRR0119_H

typedef struct prr01190t {
	unsigned char	sig_assign;		/* signal assign		*/
} PRR01190T;

typedef struct prr01191t {
	unsigned char	nof_phead;		/* number of parent head	*/
	struct prr01190t	phead[2];	/* parameter[nof_phead]		*/
} PRR01191T;

typedef struct prr01192t {
	unsigned char	nof_sect;		/* number of section		*/
	struct prr01191t	sect[8];	/* parameter[nof_sect]		*/
} PRR01192T;
/*------------------------------------------------------*/
/*	信号出力先のアサインパラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct prr0119t {
	unsigned char	nof_infsig;		/* number of infeed signal	*/
	unsigned char	infsig_assign[8];	/* infeed signal assign		*/
	unsigned char	lowp_sig_assign[8];	/* low product signal assign	*/
	unsigned char	nof_df;			/* number of DF			*/
	unsigned char	dfsync_sig_assign[8];	/* DF sync. signal assign	*/
	unsigned char	nof_ch;			/* number of channel		*/
	unsigned char	dumpsig_assign[4];	/* dump signal assign		*/
	unsigned char	pr_dumpsig_assign[4];	/* proper dump signal assign	*/
	unsigned char	ng_dumpsig_assign[4];	/* NG dump signal assign	*/
	unsigned char	pr_readysig_assign[4];	/* proper ready signal assign	*/
	unsigned char	ng_readysig_assign[4];	/* NG ready signal assign	*/
	unsigned char	pr_each_dpsig_assign[4];/* proper each dump sig. assign	*/
	unsigned char	ng_each_dpsig_assign[4];/* NG each dump signal assign	*/
	unsigned char	pr_addsig_assign[4];	/* proper addtion signal assign	*/
	unsigned char	ng_addsig_assign[4];	/* NG addtion signal assign	*/
	unsigned char	error_sig_assign[4];	/* error signal assign		*/
	unsigned char	control_sig_assign[4];	/* control signal assign	*/
	unsigned char	prod_sig_assign[4];	/* production signal assign	*/
	unsigned char	drain_sig_assign[4];	/* drain signal assign		*/
	unsigned char	manuzero_sig_assign[4];	/* manual zero adj. sig. assign	*/
	unsigned char	error_rej_sig_assign[4];/* error reject sig. assign	*/
	unsigned char	lowp_stop_sig_assign[4];/* low product stop sig. assign	*/
	unsigned char	metal_det_sig_assign[4];/* metal detect signal assign	*/
	struct prr01192t	pandc[4];	/* parent parameter		*/
} PRR0119T;

#if	defined(STORAGE)
struct prr0119t	prr0119s;
#else
#if	!defined(NOEXTERN)
extern struct prr0119t	prr0119s;
#endif
#endif

#endif
