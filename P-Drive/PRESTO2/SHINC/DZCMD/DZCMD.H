/*======================================================================
 * File name    : dzcmd.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ƒRƒ}ƒ“ƒhƒTƒ}ƒŠ[
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzcmd.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzcmd.h $
 * ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  chimura
 * ‰ŠúƒŠƒrƒWƒ‡ƒ“
 * 
 *----------------------------------------------------------------------
 *Header: dzcmd.hv  1.12  94/05/20 11:40:44  sato
 *Log: p:/include/dzcmd/rcs/dzcmd.hv
 * Revision 1.12  94/05/20 11:40:44  sato
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	__DZCMD_H__
#define	__DZCMD_H__

/*
// CCW-DZ Šg’£ƒRƒ}ƒ“ƒh‚¨‚æ‚ÑŠg’£ƒpƒ‰ƒ[ƒ^‚ð’è‹`‚·‚é
*/

/* ’ÊMƒeƒLƒXƒg‚ÌƒtƒH[ƒ}ƒbƒg */
/**/
/*             ƒ|ƒCƒ“ƒ^ŒvŽZ‚Ì‹N“_                                       */
/*                «0                      p1                           */
/*†®†¢†Î†¢†¢†Î†¢†¢†Î†¢†¢†Î†¢†¢†Î†¢†¢†¢†¢†¢†Î†¢†¢†Î†¢†¢†¢†¢†¢†Î†¢†¢†Î†¢†²*/
/*†¤LF†¤DID †¤SID †¤CMD †¤PTR0†¤PARAMETER0†¤PTR1†¤PARAMETER1†¤SUM †¤CR†¤*/
/*†¤ 1†¤  4 †¤  4 †¤  1 †¤   4†¤        n0†¤   4†¤        n1†¤  2 †¤ 1†¤*/
/*†¶†¢†Ö†¢†¢†Ö†¢†¢†Ö†¢†¢†Ö†¢†¢†Ö†¢†¢†¢†¢†¢†Ö†¢†¢†Ö†¢†¢†¢†¢†¢†Ö†¢†¢†Ö†¢†º*/
/*    †¤                                  ª                †¤          */
/*    †¤                               Šg’£ƒpƒ‰ƒ[ƒ^       †¤          */
/*    †¤                                                    †¤          */
/*    †¾†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†¢†Æ          */
/*    †¤SUM ŒvŽZ”ÍˆÍ		    @                      †¤          */

/* PTRn ‚ÍŽŸ‚Ìƒpƒ‰ƒ[ƒ^‚Ì CMD ‚ð‹N“_‚Æ‚µ‚½ŠJŽnˆÊ’u‚ðŽ¦‚·B             */
/* ŽŸ‚Ìƒpƒ‰ƒ[ƒ^‚ª‚È‚¢ê‡‚Í 0 ‚Æ‚·‚éB                                */

#include	<custom\typedefs.h>		/* Show xlib		*/

typedef struct fmt_dzhd {			/* æ“ª‚Ìƒwƒbƒ_[•”	*/
	BYTE	srcno[4];			/* “]‘—Œ³Œv—Ê‹@number	*/
	BYTE	dstno[4];			/* “]‘—æ	"	*/
} FMT_DZHD;

#define	PRMP	4				/* ƒ|ƒCƒ“ƒ^[‚ÌƒTƒCƒY	*/

#define		AFVADJ_CMD	('V')		/* ‚`‚e‚u’²®ƒRƒ}ƒ“ƒh	*/
#define		DIAGREQ_CMD	('U')		/* ”‰f’fƒf[ƒ^—v‹	*/
#define		DZHOPSPEC_CMD	('j')		/* ‚c‚yƒzƒbƒp‹ì“®“Á«	*/
#define		AFVRECALL_CMD	('r')		/* ‚`‚e‚uŒW”ŒÄ‚Ño‚µ	*/
#define		COORD_CMD	('y')		/* dS‚ÌÀ•Wƒf[ƒ^	*/
#define		AFVCOE_CMD	('v')		/* ‚`‚e‚uŒW”ƒRƒ}ƒ“ƒh 	*/
#define		DIAGDATA_CMD	('u')		/* ”‰f’fƒf[ƒ^‰ž“š 	*/

/*------------------------------------------------------*/
/*	”‰‚ÌŒv—Ê“Á«Šg’£ƒpƒ‰ƒ[ƒ^			*/
/*	iAFV ƒpƒ‰ƒ[ƒ^j				*/
/* dzr009,dzr010					*/
/*------------------------------------------------------*/

struct fmt_wei_spec1 {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	afv_mode;		/* AFV mode (off/auto/on)	*/
	BYTE	filter;			/* filter			*/
	BYTE	span_wt;		/* ƒXƒpƒ“•ª“º			*/
	BYTE	cell_type;		/* cell type			*/
/*-------------------------------------*/
};

int		dz_scan_wei_spec1(struct fmt_wei_spec1 *buf);
void *		dz_print_wei_spec1(struct fmt_wei_spec1 *txt, struct dzr009t *pkt);

/*------------------------------------------------------*/
/*	‚`‚e‚u’²®ƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg			*/
/*							*/
/* dzr203,dzr204					*/
/*------------------------------------------------------*/

struct fmt_afv_adj {
	BYTE	cmd;			/* command = 'V'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	adj_mode[4];		/* adjust mode			*/
/*-------------------------------------*/
};

int		dz_scan_afv_adj(struct fmt_afv_adj *buf);
void *		dz_print_afv_adj(struct fmt_afv_adj *txt, struct dzr203t *pkt);

/*------------------------------------------------------*/
/*	”‰f’fƒf[ƒ^—v‹ƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg		*/
/*							*/
/* dzr205,dzr206					*/
/*------------------------------------------------------*/

struct fmt_diag_req {
	BYTE	cmd;			/* command = 'U'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	head_no[2];		/* ƒwƒbƒh”Ô†			*/
/*-------------------------------------*/
};

int		dz_scan_diag_req(struct fmt_diag_req *buf);
void *		dz_print_diag_req(struct fmt_diag_req *txt, struct dzr205t *pkt);

/*------------------------------------------------------*/
/*	‚`‚e‚uŒW”ŒÄ‚Ño‚µƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg		*/
/*							*/
/* dzr207,dzr208					*/
/*------------------------------------------------------*/

struct fmt_afv_recall {
	BYTE	cmd;			/* command = 'r'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/

/*-------------------------------------*/
};

int		dz_scan_afv_recall( void * string);
#define		dz_scan_afv_recall()	((void)0)
void *		dz_print_afv_recall( void * string, void * struc);
#define		dz_print_afv_recall()	((void)0)

/*------------------------------------------------------*/
/*	‚c‚yƒzƒbƒp‚ÌŠJ•Â“Á«‚ÌƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg	*/
/*							*/
/* dzr209,dzr210					*/
/*------------------------------------------------------*/

typedef struct {			/*----- mdp_section_data -------*/
	BYTE	amp_n[4];		/* ÅIŠJ•ÂˆÊ’u(‰ñ“]‹æŠÔ)[%]	*/
	BYTE	sec_flag_power[4];	/* ¾¸¼®Ý‚ÌÊ°ÌÊßÜ°Ý’è		*/
	BYTE 	time_hold[4];		/* Î°ÙÄÞŽžŠÔ[ms]		*/
	BYTE	num_ss_start[4];	/* Žn“®½Û°½Ã¯Ìß‹æŠÔ(½Ã¯Ìß”)	*/
	BYTE	num_ss_stop[4];		/* §“®½Û°½Ã¯Ìß‹æŠÔ(½Ã¯Ìß”)	*/
	BYTE	max_pps[4];		/* ‘¬“x[pps]			*/
} P2093;

typedef struct {			/* actuator_type == ACT_SM	*/
	BYTE	phase_type;		/* —ãŽ¥ƒ‚[ƒh	3:1-2‘Š		*/
	BYTE	full_pow_tm[4];		/* ’á‘¬Žžƒtƒ‹ƒpƒ[ŽžŠÔ(4000)	*/
	BYTE	ignore_pulse[4];	/* ƒXƒŠƒbƒg•sŠ´ƒpƒ‹ƒX”	 	*/
       	BYTE	cam_slit[2];		/* ƒXƒŠƒbƒg’âŽ~ƒpƒ‹ƒX” 	*/
	BYTE	hold_mode;		/* ’âŽ~—ãŽ¥ðŒ			*/
	BYTE	brake_time[2];		/* ƒuƒŒ[ƒLŽžŠÔ	      [10msec]	*/
	BYTE	error_pulse[2];		/* ƒGƒ‰[ŒŸoƒpƒ‹ƒX”		*/
	BYTE	num_section[2];		/* ‹ì“®ÊßÀ°Ý\¬”(¾¸¼®Ý”j	*/
	BYTE	mode_flags[2];		/* ‹ì“®ÊßÀ°ÝÓ°ÄÞÌ×¸Þ		*/
	BYTE	start_pps[4];		/* Ž©‹N“®’PˆÊÊßÙ½”[pps]	*/
	BYTE	fullstep[4];		/* ‘S½Ã¯Ìß”			*/
	BYTE	pitch[4];		/* (—\”õ)			*/
	P2093	msd[10];		/* msd[num_sectionŒÂ]		*/
} P2094;

typedef struct {			/* actuator_type == ACT_CB	*/
	BYTE	osc[4];			/* Šî–{ÊßÙ½ŽžŠÔ(1us’PˆÊ)	*/
	BYTE	cl_drive_tm[4];		/* CL‹ì“®ŽžŠÔ			*/
 	BYTE	cl_on_delay[2];		/* CLƒIƒ“’x‰„ŽžŠÔ		*/
	BYTE	br_on_delay[2];		/* BRƒIƒ“’x‰„ŽžŠÔ		*/
	BYTE	br_time[2];		/* BRƒIƒ“ŽžŠÔ(10ms’PˆÊ)		*/
} P2095;

typedef union {
	P2094	sm;			/* ƒXƒeƒbƒv¥ƒ‚[ƒ^[‹ì“®—p	*/
	P2095	cb;			/* ƒNƒ‰ƒbƒ`^ƒuƒŒ[ƒL‹ì“®—p	*/
} PR2090;

struct fmt_dzhop_spec {
	BYTE	cmd;			/* command = 'j'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern
					   [nof_head/4]			*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	stop_mode;		/* ’âŽ~ðŒ			*/
	BYTE	actuator_type;		/* ƒAƒNƒ`ƒ…ƒG[ƒ^ƒ^ƒCƒv		*/
	PR2090	cpx;			/* •¡‡‹ì“®ƒpƒ‰ƒ[ƒ^		*/
/*-------------------------------------*/
};

int		dz_scan_hop_spec(struct fmt_dzhop_spec *buf);
void *		dz_print_hop_spec(struct fmt_dzhop_spec *txt, struct dzr209t *pkt);

/*------------------------------------------------------*/
/*	ƒwƒbƒh‚ÌdS‚ÌÀ•WƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg		*/
/*							*/
/* dzr211,dzr212					*/
/*------------------------------------------------------*/

struct fmt_afv_coord {
	BYTE	cmd;			/* command = 'y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ‚`‚c‚bƒ{[ƒhŽ¯•Ê”Ô†		*/
	BYTE	coord_x[20][4];		/* Œv—ÊŠí‚ÆAFV‚ÌdS‚Ì‚wÀ•W	*/
	BYTE	coord_y[20][4];		/* Œv—ÊŠí‚ÆAFV‚ÌdS‚Ì‚xÀ•W	*/
	BYTE	afv_coeffi[16][8];	/* ‚`‚e‚uŒW”			*/
/*-------------------------------------*/
};

int		dz_scan_afv_coord(struct fmt_afv_coord *buf);
void *		dz_print_afv_coord(struct fmt_afv_coord *txt, struct dzr211t *pkt);

/*------------------------------------------------------*/
/*	‚`‚e‚uŒW”ƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg			*/
/*							*/
/* dzi203,dzi204					*/
/*------------------------------------------------------*/

struct fmt_afv_coeffi {
	BYTE	cmd;			/* command = 'v'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* ŠY“–‚·‚éWH”Ô†		*/
	BYTE	new_coeffi[8];		/* V‚`‚e‚uŒW”			*/
	BYTE	old_coeffi[8];		/* ‹Œ‚`‚e‚uŒW”			*/
/*-------------------------------------*/
};

int		dz_scan_afv_coeffi(struct fmt_afv_coeffi *buf);
void *		dz_print_afv_coeffi(struct fmt_afv_coeffi *txt, struct dzi203t *pkt);

/*------------------------------------------------------*/
/*	”‰f’fƒf[ƒ^ƒeƒLƒXƒgƒtƒH[ƒ}ƒbƒg		*/
/*							*/
/* dzi205,dzi206					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ƒwƒbƒhd—Ê			*/
	BYTE	status[2];		/* ƒwƒbƒhƒXƒe[ƒ^ƒX		*/
} PI2050;

struct fmt_diag_data {
	BYTE	cmd;			/* command = 'u'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* Å¬WH”Ô†			*/
	BYTE	nof_wh[2];		/* ŠY“–‚·‚éch‚ÌWH”		*/
	PI2050	wh_3rd[32];		/* 3’i–Úd—Ê&ƒXƒe[ƒ^ƒX[nof_wh]	*/
	PI2050	wh_4th[32];		/* 4’i–Úd—Ê&ƒXƒe[ƒ^ƒX[nof_wh]	*/
	BYTE	min_df_no;		/* Å¬DF”Ô†			*/
	BYTE	nof_df;			/* ŠY“–‚·‚éch‚ÌDF”		*/
	PI2050	df[8];			/* WHd—Ê&ƒXƒe[ƒ^ƒX[nof_df]	*/
	BYTE	nof_afv;		/* ‚`‚e‚uƒZƒ‹”			*/
	BYTE	afv_out[8][4];		/* ‚`‚e‚uƒZƒ‹o—Í[nof_afv]	*/
/*-------------------------------------*/
};

int		dz_scan_diag_data(struct fmt_diag_data *buf);
void *		dz_print_diag_data(struct fmt_diag_data *txt, struct dzi205t *pkt);
#endif	/* __DZCMD_H__	*/
/*======================================================================*/
/*                        End of file                                   */
/*======================================================================*/
