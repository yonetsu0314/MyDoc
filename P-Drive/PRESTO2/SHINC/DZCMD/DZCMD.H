/*======================================================================
 * File name    : dzcmd.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	コマンドサマリー
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzcmd.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzcmd.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
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
// CCW-DZ 拡張コマンドおよび拡張パラメータを定義する
*/

/* 通信テキストのフォーマット */
/**/
/*             ポインタ計算の起点                                       */
/*                ↓0                      p1                           */
/*�ｮ�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�｢�｢�｢�｢�ﾎ�｢�｢�ﾎ�｢�ｲ*/
/*�､LF�､DID �､SID �､CMD �､PTR0�､PARAMETER0�､PTR1�､PARAMETER1�､SUM �､CR�､*/
/*�､ 1�､  4 �､  4 �､  1 �､   4�､        n0�､   4�､        n1�､  2 �､ 1�､*/
/*�ｶ�｢�ﾖ�｢�｢�ﾖ�｢�｢�ﾖ�｢�｢�ﾖ�｢�｢�ﾖ�｢�｢�｢�｢�｢�ﾖ�｢�｢�ﾖ�｢�｢�｢�｢�｢�ﾖ�｢�｢�ﾖ�｢�ｺ*/
/*    �､                                  ↑                �､          */
/*    �､                               拡張パラメータ       �､          */
/*    �､                                                    �､          */
/*    �ｾ�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�｢�ﾆ          */
/*    �､SUM 計算範囲		    　                      �､          */

/* PTRn は次のパラメータの CMD を起点とした開始位置を示す。             */
/* 次のパラメータがない場合は 0 とする。                                */

#include	<custom\typedefs.h>		/* Show xlib		*/

typedef struct fmt_dzhd {			/* 先頭のヘッダー部	*/
	BYTE	srcno[4];			/* 転送元計量機number	*/
	BYTE	dstno[4];			/* 転送先	"	*/
} FMT_DZHD;

#define	PRMP	4				/* ポインターのサイズ	*/

#define		AFVADJ_CMD	('V')		/* ＡＦＶ調整コマンド	*/
#define		DIAGREQ_CMD	('U')		/* 秤診断データ要求	*/
#define		DZHOPSPEC_CMD	('j')		/* ＤＺホッパ駆動特性	*/
#define		AFVRECALL_CMD	('r')		/* ＡＦＶ係数呼び出し	*/
#define		COORD_CMD	('y')		/* 重心の座標データ	*/
#define		AFVCOE_CMD	('v')		/* ＡＦＶ係数コマンド 	*/
#define		DIAGDATA_CMD	('u')		/* 秤診断データ応答 	*/

/*------------------------------------------------------*/
/*	秤の計量特性拡張パラメータ			*/
/*	（AFV パラメータ）				*/
/* dzr009,dzr010					*/
/*------------------------------------------------------*/

struct fmt_wei_spec1 {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	afv_mode;		/* AFV mode (off/auto/on)	*/
	BYTE	filter;			/* filter			*/
	BYTE	span_wt;		/* スパン分銅			*/
	BYTE	cell_type;		/* cell type			*/
/*-------------------------------------*/
};

int		dz_scan_wei_spec1(struct fmt_wei_spec1 *buf);
void *		dz_print_wei_spec1(struct fmt_wei_spec1 *txt, struct dzr009t *pkt);

/*------------------------------------------------------*/
/*	ＡＦＶ調整テキストフォーマット			*/
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
/*	秤診断データ要求テキストフォーマット		*/
/*							*/
/* dzr205,dzr206					*/
/*------------------------------------------------------*/

struct fmt_diag_req {
	BYTE	cmd;			/* command = 'U'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	head_no[2];		/* ヘッド番号			*/
/*-------------------------------------*/
};

int		dz_scan_diag_req(struct fmt_diag_req *buf);
void *		dz_print_diag_req(struct fmt_diag_req *txt, struct dzr205t *pkt);

/*------------------------------------------------------*/
/*	ＡＦＶ係数呼び出しテキストフォーマット		*/
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
/*	ＤＺホッパの開閉特性のテキストフォーマット	*/
/*							*/
/* dzr209,dzr210					*/
/*------------------------------------------------------*/

typedef struct {			/*----- mdp_section_data -------*/
	BYTE	amp_n[4];		/* 最終開閉位置(回転区間)[%]	*/
	BYTE	sec_flag_power[4];	/* ｾｸｼｮﾝのﾊｰﾌﾊﾟﾜｰ設定		*/
	BYTE 	time_hold[4];		/* ﾎｰﾙﾄﾞ時間[ms]		*/
	BYTE	num_ss_start[4];	/* 始動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾃｯﾌﾟ数)	*/
	BYTE	num_ss_stop[4];		/* 制動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾃｯﾌﾟ数)	*/
	BYTE	max_pps[4];		/* 速度[pps]			*/
} P2093;

typedef struct {			/* actuator_type == ACT_SM	*/
	BYTE	phase_type;		/* 励磁モード	3:1-2相		*/
	BYTE	full_pow_tm[4];		/* 低速時フルパワー時間(4000)	*/
	BYTE	ignore_pulse[4];	/* スリット不感パルス数	 	*/
       	BYTE	cam_slit[2];		/* スリット停止パルス数 	*/
	BYTE	hold_mode;		/* 停止励磁条件			*/
	BYTE	brake_time[2];		/* ブレーキ時間	      [10msec]	*/
	BYTE	error_pulse[2];		/* エラー検出パルス数		*/
	BYTE	num_section[2];		/* 駆動ﾊﾟﾀｰﾝ構成数(ｾｸｼｮﾝ数）	*/
	BYTE	mode_flags[2];		/* 駆動ﾊﾟﾀｰﾝﾓｰﾄﾞﾌﾗｸﾞ		*/
	BYTE	start_pps[4];		/* 自起動単位ﾊﾟﾙｽ数[pps]	*/
	BYTE	fullstep[4];		/* 全ｽﾃｯﾌﾟ数			*/
	BYTE	pitch[4];		/* (予備)			*/
	P2093	msd[10];		/* msd[num_section個]		*/
} P2094;

typedef struct {			/* actuator_type == ACT_CB	*/
	BYTE	osc[4];			/* 基本ﾊﾟﾙｽ時間(1us単位)	*/
	BYTE	cl_drive_tm[4];		/* CL駆動時間			*/
 	BYTE	cl_on_delay[2];		/* CLオン遅延時間		*/
	BYTE	br_on_delay[2];		/* BRオン遅延時間		*/
	BYTE	br_time[2];		/* BRオン時間(10ms単位)		*/
} P2095;

typedef union {
	P2094	sm;			/* ステップ･モーター駆動用	*/
	P2095	cb;			/* クラッチ／ブレーキ駆動用	*/
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
	BYTE	stop_mode;		/* 停止条件			*/
	BYTE	actuator_type;		/* アクチュエータタイプ		*/
	PR2090	cpx;			/* 複合駆動パラメータ		*/
/*-------------------------------------*/
};

int		dz_scan_hop_spec(struct fmt_dzhop_spec *buf);
void *		dz_print_hop_spec(struct fmt_dzhop_spec *txt, struct dzr209t *pkt);

/*------------------------------------------------------*/
/*	ヘッドの重心の座標テキストフォーマット		*/
/*							*/
/* dzr211,dzr212					*/
/*------------------------------------------------------*/

struct fmt_afv_coord {
	BYTE	cmd;			/* command = 'y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ＡＤＣボード識別番号		*/
	BYTE	coord_x[20][4];		/* 計量器とAFVの重心のＸ座標	*/
	BYTE	coord_y[20][4];		/* 計量器とAFVの重心のＹ座標	*/
	BYTE	afv_coeffi[16][8];	/* ＡＦＶ係数			*/
/*-------------------------------------*/
};

int		dz_scan_afv_coord(struct fmt_afv_coord *buf);
void *		dz_print_afv_coord(struct fmt_afv_coord *txt, struct dzr211t *pkt);

/*------------------------------------------------------*/
/*	ＡＦＶ係数テキストフォーマット			*/
/*							*/
/* dzi203,dzi204					*/
/*------------------------------------------------------*/

struct fmt_afv_coeffi {
	BYTE	cmd;			/* command = 'v'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* 該当するWH番号		*/
	BYTE	new_coeffi[8];		/* 新ＡＦＶ係数			*/
	BYTE	old_coeffi[8];		/* 旧ＡＦＶ係数			*/
/*-------------------------------------*/
};

int		dz_scan_afv_coeffi(struct fmt_afv_coeffi *buf);
void *		dz_print_afv_coeffi(struct fmt_afv_coeffi *txt, struct dzi203t *pkt);

/*------------------------------------------------------*/
/*	秤診断データテキストフォーマット		*/
/*							*/
/* dzi205,dzi206					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[2];		/* ヘッドステータス		*/
} PI2050;

struct fmt_diag_data {
	BYTE	cmd;			/* command = 'u'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	PI2050	wh_3rd[32];		/* 3段目重量&ステータス[nof_wh]	*/
	PI2050	wh_4th[32];		/* 4段目重量&ステータス[nof_wh]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	PI2050	df[8];			/* WH重量&ステータス[nof_df]	*/
	BYTE	nof_afv;		/* ＡＦＶセル数			*/
	BYTE	afv_out[8][4];		/* ＡＦＶセル出力[nof_afv]	*/
/*-------------------------------------*/
};

int		dz_scan_diag_data(struct fmt_diag_data *buf);
void *		dz_print_diag_data(struct fmt_diag_data *txt, struct dzi205t *pkt);
#endif	/* __DZCMD_H__	*/
/*======================================================================*/
/*                        End of file                                   */
/*======================================================================*/
