/*======================================================================
 * File name    : rzcmd.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	コマンドサマリー
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/rzcmd/RCS/rzcmd.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzcmd.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzcmd.h 2.95 1970/01/01 00:00:00 kawamura
 *Log: rzcmd.h
 * リビジョン 2.95  1970/01/01  00:00:00  kawamura
 * 2002/08/01  16:12
 * rzr203,rzr204,rzr419,rzr420を追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZCMD_H)
#define	RZCMD_H

/*
// CCW-RZ, DZ 拡張コマンドおよび拡張パラメータを定義する
*/

/* 通信テキストのフォーマット */
/**/
/*             ポインタ計算の起点                                       */
/*                ↓0                      p1                           */
/*┌─┬──┬──┬──┬──┬─────┬──┬─────┬──┬─┐*/
/*│LF│DID │SID │CMD │PTR0│PARAMETER0│PTR1│PARAMETER1│SUM │CR│*/
/*│ 1│  4 │  4 │  1 │   4│        n0│   4│        n1│  2 │ 1│*/
/*└─┴──┴──┴──┴──┴─────┴──┴─────┴──┴─┘*/
/*    │                                  ↑                │          */
/*    │                               拡張パラメータ       │          */
/*    │                                                    │          */
/*    ├──────────────────────────┤          */
/*    │SUM 計算範囲		                            │          */

/* PTRn は次のパラメータの CMD を起点とした開始位置を示す。             */
/* 次のパラメータがない場合は 0 とする。                                */

#include	<custom\typedefs.h>		/* Show xlib		*/

typedef struct fmt_rzhd {			/* 先頭のヘッダー部	*/
	BYTE	srcno[4];			/* 転送元計量機number	*/
	BYTE	dstno[4];			/* 転送先	"	*/
} FMT_RZHD;

#define	PRMP	4				/* ポインターのサイズ	*/

#define		MCH_SPEC_CMD	('w')
#define		INFD_SPEC_CMD	('x')
#define		BMIF_SPEC_CMD	('b')
#define		WEI_SPEC_CMD	('a')
#define		CBCAL_SPEC_CMD	('c')
#define		HOP_SPEC_CMD	('d')
#define		FD_SPEC_CMD	('e')
#define		TMG_PRM_CMD	('t')
#define		RFDF_PRM_CMD	('f')
#define		CF_PRM_CMD	('g')
#define		TGT_PRM_CMD	('T')
#define		PNUM_REQ_CMD	(',')
#define		SCL_REQ_CMD	('L')
#define		PW_ON_CMD	('W')
#define		PW_OFF_CMD	('X')
#define		ZERO_CMD	('Z')
#define		SPAN_CMD	('A')
#define		PROD_STRT_CMD	('R')
#define		PROD_STOP_CMD	('S')
#define		DRAIN_STRT_CMD	('G')
#define		DRAIN_STOP_CMD	('H')
#define		MAN_COMBI_CMD	('C')
#define		MAN_CYCLE_CMD	('M')
#define		ERREJ_CMD	('D')
#define		TDRV_STRT_CMD	('P')
#define		TDRV_STOP_CMD	('Q')
#define		CBRSLT_CMD	('E')
#define		RNWPW_CMD	('p')
#define		RNWCOMP_CMD	('q')
#define		PNUM_CMD	('Y')
#define		SCALE_CMD	('\\')
#define		COND_CMD	('N')
#define		WRNG_CMD	('F')
#define		ERROR_CMD	('I')
#define		RXCOMP_CMD	('=')
#define		ACTCOMP_CMD	('>')
#define		SYSERR_CMD	('?')
#define		AFDH_CMD	('h')
#define		AFDS_CMD	('s')
#define		MEAN_CMD	('m')
#define		PCUERR_CMD	('i')
#define		P_TARG_CMD	('o')
#define		P_RSLT_CMD	('O')
#define		EEPROMCLR_CMD	('z')
#define		CF_RSLT_CMD	('k')
#define		OPT_DEV_CMD	('@')
#define		USR_LEVEL_CMD	('l')
#define		METAL_TEXT_CMD	('{')
#define		METAL_ANS_CMD	('}')
#define		SAMPLE_DUMP_CMD	(':')
#define		DMY_WEIGHT_CMD	('$')
#define		DCA_PRM_CMD	('!')
#define		ATA_DEV_CMD	('"')
#define		DCA_MON_CMD	('(')
#define		ATA_MON_CMD	(')')
#define		NETMAP_REQ_CMD	('[')				/*ver3*/
#define		NETMAP_CMD	(']')				/*ver3*/
#define		CF2_PRM_CMD	('n')
#define		TIMER_PRM_CMD	('#')
#define		SHIFT_REQ_CMD	('%')
#define		SHIFT_MON_CMD	('&')
#define		SPC_REQ_CMD	('J')
#define		SPAN_COE_CMD	('K')
#define		PLC_WR_CMD	('\'')
#define		MLT_P1REQ_CMD	('_')
#define		MLT_P1_CMD	('-')
#define		MLT_P2_CMD	('^')
#define		MLT_EXT_CMD	('<')
#define		MLT_COND_CMD	('*')
#define		PRENUM_COMP_CMD	('+')
#define		SH_CTL_CMD	('|')

/*------------------------------------------------------*/
/*	計量機の構成仕様テキストフォーマット		*/
/*							*/
/* rzr003,rzr004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* hopper assign pattern
					   [nof_head/4]			*/
	BYTE	drv_type;		/* drive type			*/
	BYTE	direction;		/* 正不良、２方向識別子		*/
	BYTE	level;			/* 段数(WHからの位置)		*/
	BYTE	port;			/* EXCポート番号		*/
} P0;


struct fmt_mch_construct {
	BYTE	cmd;			/* command = 'w'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	head_construct;		/* radial/linear		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	model_id;		/* model ID			*/
	BYTE	nof_df;			/* number of DF			*/
	BYTE	df_assign_ptn[8][8];	/* DF loacate pattern
					   [nof_df][nof_head/4]		*/
	BYTE	nof_rs;			/* number of RS			*/
	P0	rs[8];			/* P0 parameter of RS		*/
	BYTE	nof_dth;		/* number of DTH		*/
	P0	dth[8];			/* P0 parameter	of DTH		*/
	BYTE	nof_th;			/* number of TH			*/
	P0	th[8];			/* P0 parameter	of TH		*/
	BYTE	nof_duc_net;		/* number of duc net		*/
	BYTE	duc_assign_ptn[2][8];	/* DUC network pattern
					   [nof_duc_net][nof_head/4]	*/
	BYTE	mlt_intlk_mode;		/* multi interlock mode		*/
	BYTE	shift_cnt[2];		/* shift count			*/
/*-------------------------------------*/
};

int	rz_scan_mch_construct( struct fmt_mch_construct *buf);
void *	rz_print_mch_construct( struct fmt_mch_construct *txt, struct rzr003t *pkt);


/*------------------------------------------------------*/
/*	供給機の制御仕様テキストフォーマット		*/
/*							*/
/* rzr005,rzr006					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	sig_assign_ptn[8];	/* infeed signal assign pattern
					   [nof_head/4]			*/
	BYTE	infd_detector;		/* level sensor type		*/
	BYTE	transient_int[2];	/* transient interrupt time	*/
	BYTE	feed_intrpt[2];		/* low product time		*/
} P1;

struct fmt_infd_spec {
	BYTE	cmd;			/* command = 'x'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sig;		/* number of infeed signal	*/
	P1	sig[8];			/* P1 paramter[nof_sig]		*/
	BYTE	nof_cf;			/* number of CF			*/
	BYTE	cf_assign_ptn[8][8];	/* CF assign pattern
					   [nof_cf][nof_head/4]		*/
	BYTE	pconst[4];		/* proportion constant		*/
	BYTE	iconst[4];		/* integral constant		*/
	BYTE	dconst[4];		/* differrential constant	*/
	BYTE	filter_sw[4];		/* filter switch		*/
	BYTE	filter_const[4];	/* filter constant		*/
/*-------------------------------------*/
};

int	rz_scan_infd_spec( struct fmt_infd_spec *buf);
void *	rz_print_infd_spec( struct fmt_infd_spec *txt, struct rzr005t *pkt);


/*------------------------------------------------------*/
/*	包装機との連動仕様テキストフォーマット		*/
/*							*/
/* rzr007,rzr008					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	bm_assign_ptn[8];	/* bag maker assign pattern
					   [nof_cf][nof_head/4]		*/
	BYTE	interface;		/* I/F type			*/
	BYTE	mul_dump_ini;		/* multi dump initiate mode	*/
	BYTE	mul_dump_conf;		/* multi dump confirm mode	*/
	BYTE	manu_dump_ini;		/* manual dump initiate mode	*/
	BYTE	manu_dump_conf;		/* manual dump confirm mode	*/
} P2;

struct fmt_bmif_spec {
	BYTE	cmd;			/* command = 'b'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_bm;			/* number of bag maker		*/
	P2	ch[2];			/* P2 paramter[nof_bm]		*/
/*-------------------------------------*/
};

int	rz_scan_bmif_spec(struct fmt_bmif_spec *buf);
void *	rz_print_bmif_spec( struct fmt_bmif_spec *txt, struct rzr007t *pkt);


/*------------------------------------------------------*/
/*	秤の計量特性テキストフォーマット		*/
/*							*/
/* rzr009,rzr010					*/
/*------------------------------------------------------*/

struct fmt_wei_spec {
	BYTE	cmd;			/* command = 'a'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	stable_count[2];	/* stable count			*/
	BYTE	zero_limit_cnt[4];	/* auto zero error count	*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
	BYTE	stable_cmp_cnt;		/* compare count		*/
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	range;			/* weigh range			*/
	BYTE	smpl_interval[2];	/* sampling interval		*/
	BYTE	iir_reset_cnt[2];	/* IIR filter reset count	*/
	BYTE	anticipation;		/* anticipate weighing		*/
/*-------------------------------------*/
};

int	rz_scan_wei_spec( struct fmt_wei_spec *buf);
void *	rz_print_wei_spec( struct fmt_wei_spec *txt, struct rzr009t *pkt);


/*------------------------------------------------------*/
/*	組み合わせ計算の特性テキストフォーマット	*/
/*							*/
/* rzr011,rzr012					*/
/*------------------------------------------------------*/

struct fmt_cbcal_spec {
	BYTE	cmd;			/* command = 'c'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	wh_active[8];		/* WH active head[nof_head/4]	*/
	BYTE	bh_active[8];		/* BH active head[nof_head/4]	*/
	BYTE	compensation[2];	/* compensation value		*/
	BYTE	renew_compensation;	/* renew compensation control	*/
	BYTE	calc_mode;		/* calculate control		*/
	BYTE	mix_mode;		/* mix calculate control	*/
	BYTE	over_scale;		/* over scale process control	*/
	BYTE	over_weight;		/* over weight process control	*/
	BYTE	hop_error;		/* hopper error process control	*/
	BYTE	az_error;		/* autozero error process control*/
	BYTE	err_rej_type;		/* error reject process control	*/
/*-------------------------------------*/
};

int	rz_scan_cbcal_spec( struct fmt_cbcal_spec *buf);
void *	rz_print_cbcal_spec( struct fmt_cbcal_spec *txt, struct rzr011t *pkt);


/*------------------------------------------------------*/
/*	ホッパの開閉特性のテキストフォーマット		*/
/*							*/
/* rzr013,rzr014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	pulse_mltp[2];		/* 基本パルス倍数		*/
	BYTE	repeat[2];	 	/* 繰り返し数			*/
} P3;

struct fmt_hop_spec {
	BYTE	cmd;			/* command = 'd'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern
					   [nof_head/4]			*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	actuator_type;		/* アクチュエータタイプ		*/
	BYTE	phase_type;		/* 励磁モード	3:1-2相		*/
	BYTE	stop_mode;		/* 停止条件			*/
	BYTE	speed[4];	 	/* 回転数 [単位パルス時間0.4us] */
	BYTE	ignore_pulse[4];	/* スリット不感パルス数	 	*/
       	BYTE	cam_slit[2];		/* スリット停止パルス数 	*/
  	BYTE	br_cl_delay[2];		/* BR-CL動作遅れパルス数	*/
	BYTE	cl_br_delay[2];		/* CL-BR動作遅れパルス数	*/
	BYTE	brake_time[2];		/* ブレーキ時間	      [10msec]	*/
	BYTE	error_pulse[2];		/* エラー検出パルス数		*/
	BYTE	nof_pattern[2];		/* パターン数			*/
	P3	ary[128];		/* P3 paramter[nof_pattern]	*/
/*-------------------------------------*/
};

int	rz_scan_hop_spec( struct fmt_hop_spec *buf);
void *	rz_print_hop_spec( struct fmt_hop_spec *txt, struct rzr013t *pkt);


/*------------------------------------------------------*/
/*	フィーダの駆動特性のテキストフォーマット	*/
/*							*/
/* rzr015,rzr016					*/
/*------------------------------------------------------*/

struct fmt_fd_spec {
	BYTE	cmd;			/* command = 'e'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	fd_assign_ptn[8];	/* Hopper assign pattern
					   [nof_head/4]			*/
	BYTE	feeder_id;		/* feeder id			*/
	BYTE	min_amp;		/* minimun aplitude		*/
	BYTE	max_amp;		/* maximun aplitude		*/
	BYTE	multiplier;		/* フィーダ時間倍率		*/
	BYTE	phase_type;		/* 位相				*/
	BYTE	opt_sw_ctrl;		/* 光電子SWによる停止モード	*/
	BYTE	shutter_ctrl;		/* シャッタ連動モード		*/
	BYTE	shutter_off_delay[2];	/* シャッタ停止遅れ時間		*/
/*-------------------------------------*/
};

int	rz_scan_fd_spec( struct fmt_fd_spec *buf);
void *	rz_print_fd_spec( struct fmt_fd_spec *txt, struct rzr015t *pkt);


/*------------------------------------------------------*/
/*	タイミングの設定テキストフォーマット		*/
/*							*/
/* rzr017,rzr018					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	delay[2];		/* RS遅れ時間[nof_rs]		*/
	BYTE	pool[2];		/* RSプール時間	    		*/
} P4;

typedef struct {
	BYTE	delay[2];		/* 遅れ時間[nof_rs]		*/
	BYTE	on[2];			/* オン時間	    		*/
} P41;

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern
					   [nof_head/4]			*/
	BYTE	wh_ph[2];		/* WH-PH遅れ時間		*/
	BYTE	ph_rf[2];		/* PH-RF遅れ時間		*/
	BYTE	wh1_bh[2];		/* WH1-MH遅れ時間		*/
	BYTE	bh_wh2[2];		/* MH-WH2遅れ時間		*/
	BYTE	stagger[2];		/* セクション内時差排出時間	*/
	BYTE	sect_stagger[2];	/* セクション間時差排出時間	*/
	BYTE	bh_on[2];		/* BHオン時間			*/
	BYTE	wh_on[2];		/* WHオン時間			*/
	BYTE	ph_on[2];		/* PHオン時間			*/
} P10;

struct fmt_tmg_parm {
	BYTE	cmd;			/* command = 't'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	ds_delay[2];		/* 排出信号遅れ時間		*/
	BYTE	ds_hold_tm[2];		/* dump confirm signal hold time*/
	BYTE	err_delay[2];		/* エラー信号遅れ時間		*/
	BYTE	es_hold_tm[2];		/* error signal hold time	*/
	BYTE	wh_delay[2];		/* WH遅れ時間			*/
	BYTE	nof_round;		/* ラウンディンググループ数	*/
	BYTE	round_base[8][2];	/* ラウンディング排出基本時間	*/
	BYTE	min_th_no;		/* 最小TH番号			*/
	BYTE	nof_th;			/* 該当するchのTH数		*/
	P41	th[8];			/* TH遅れ時間[nof_th]		*/
	BYTE	min_dth_no;		/* 最小DTH番号			*/
	BYTE	nof_dth;		/* 該当するchのDTH数		*/
	P41	dth[8];			/* DTH遅れ時間[nof_dth]		*/
	BYTE	min_rs_no;		/* 最小RS番号			*/
	BYTE	nof_rs;			/* 該当するchのRS数		*/
	P4	rs[8];			/* P4 paramter[nof_rs]		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P10	sect[8];		/* P4 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	rz_scan_tmg_parm( struct fmt_tmg_parm *buf);
void *	rz_print_tmg_parm( struct fmt_tmg_parm *txt, struct rzr017t *pkt);


/*------------------------------------------------------*/
/*	供給量の設定テキストフォーマット		*/
/*							*/
/* rzr019,rzr020					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	amp[2];			/* フィーダ振幅強度	 	*/
	BYTE	time[2];		/* フィーダ振幅時間		*/
} P5;

typedef struct {
	BYTE	targ[4];		/* 目標重量			*/
	BYTE	upper[2];		/* 上限偏差(%)			*/
	BYTE	lower[2];		/* 下限偏差(%)			*/
} P6;

struct fmt_rfdf_parm {
	BYTE	cmd;			/* command = 'f'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	backup;			/* backup			*/
	BYTE	afd_mode;		/* AFD mode			*/
	BYTE	min_rf_no[2];		/* 最小RF番号			*/
	BYTE	nof_rf[2];		/* 該当するchのRF数		*/
	P5	rf[32];			/* P5 paramter[nof_rf]		*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	P5	df[8];			/* P5 paramter[nof_df]		*/
	P6	lev[8];			/* P6 paramter[nof_df]		*/
/*-------------------------------------*/
};

int	rz_scan_rfdf_parm( struct fmt_rfdf_parm	*buf);
void *	rz_print_rfdf_parm( struct fmt_rfdf_parm *txt, struct rzr019t *pkt);


/*------------------------------------------------------*/
/*	CFの振幅設定テキストフォーマット		*/
/*							*/
/* rzr021,rzr022					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* 供給状態数			*/
	P5	at[8];			/* P5 paramter[nof_sig]		*/
} PR210;

struct fmt_cf_parm {
	BYTE	cmd;			/* command = 'g'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ctrl;			/* control			*/
	BYTE	nof_cf;			/* CF数				*/
	PR210	cf[8];			/* PR210 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	rz_scan_cf_parm( struct fmt_cf_parm *buf);
void *	rz_print_cf_parm( struct fmt_cf_parm	*txt, struct rzr021t *pkt);


/*------------------------------------------------------*/
/*	計量目標値の設定テキストフォーマット		*/
/*							*/
/* rzr023,rzr024					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern
					   [nof_head/4]			*/
	BYTE	target_wt[8];		/* 計量目標重量			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* 計量目標個数			*/
	BYTE	upper_cnt[2];		/* 上限個数			*/
	BYTE	lower_cnt[2];		/* 下限個数			*/
	BYTE	piece_wt[4];		/* 単重				*/
	BYTE	target_head[2];		/* 目標選択ヘッド数		*/
	BYTE	pw_ctrl;		/* 単重自動更新モード		*/
	BYTE	mix_ctrl;		/* 混合補正モード		*/
} P7;

struct fmt_wei_parm {
	BYTE	cmd;			/* command = 'T'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P7	sect[8];		/* P7 paramter[nof_sect]	*/
	BYTE	target_wt[8];		/* 計量目標重量			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* 計量目標個数			*/
	BYTE	upper_cnt[2];		/* 上限個数			*/
	BYTE	lower_cnt[2];		/* 下限個数			*/
	BYTE	speed[4];		/* 計量能力			*/
	BYTE	dump_cnt[2];		/* 排出回数			*/
	BYTE	avg_ctrl;		/* 平均値制御モード		*/
	BYTE	cyc_of_under[2];	/* 計量発生頻度			*/
/*-------------------------------------*/
};

int	rz_scan_wei_parm( struct fmt_wei_parm *buf);
void *	rz_print_wei_parm( struct fmt_wei_parm	*txt, struct rzr023t *pkt);

/*------------------------------------------------------*/
/*	プログラム番号の要求テキストフォーマット	*/
/*							*/
/* rzr025,rzr026					*/
/*------------------------------------------------------*/

struct fmt_req_prgno {
	BYTE	cmd;			/* command = ','		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dev_id;			/* デバイスID番号		*/
	BYTE	head_no[2];		/* ヘッド番号			*/
/*-------------------------------------*/
};

int	rz_scan_req_prgno( struct fmt_req_prgno	*buf);
void *	rz_print_req_prgno( struct fmt_req_prgno *txt, struct rzr025t *pkt);


/*------------------------------------------------------*/
/*	秤データの要求テキストフォーマット		*/
/*							*/
/* rzr027,rzr028					*/
/*------------------------------------------------------*/

struct fmt_req_weidat {
	BYTE	cmd;			/* command = 'L'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	head_no[2];		/* ヘッド番号			*/
/*-------------------------------------*/
};

int	rz_scan_req_weidat( struct fmt_req_weidat *buf);
void *	rz_print_req_weidat( struct fmt_req_weidat *txt, struct rzr027t *pkt);


/*------------------------------------------------------*/
/*	駆動系電源ONテキストフォーマット		*/
/*							*/
/* rzr029,rzr030					*/
/*------------------------------------------------------*/

struct fmt_power_on {
	BYTE	cmd;			/* command = 'W'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/

/*-------------------------------------*/
};

int		rz_scan_power_on( void * string);
#define		rz_scan_power_on()	((void)0)
void *		rz_print_power_on( void * string, void * struc);
#define		rz_print_power_on()	((void)0)


/*------------------------------------------------------*/
/*	駆動系電源OFFテキストフォーマット		*/
/*							*/
/* rzr031,rzr032					*/
/*------------------------------------------------------*/

struct fmt_power_off {
	BYTE	cmd;			/* command = 'X'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/

/*-------------------------------------*/
};

int		rz_scan_power_off( void * string);
#define		rz_scan_power_off()	((void)0)
void *		rz_print_power_off( void * string, void * struc);
#define		rz_print_power_off()	((void)0)


/*------------------------------------------------------*/
/*	排出零点調整テキストフォーマット		*/
/*							*/
/* rzr033,rzr034					*/
/*------------------------------------------------------*/

struct fmt_zero_adj {
	BYTE	cmd;			/* command = 'Z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* 調整ヘッドビットパターン	*/
	BYTE	adj_df_ptn[2];		/* 調整DFビットパターン		*/
/*-------------------------------------*/
};

int	rz_scan_zero_adj( struct fmt_zero_adj *buf);
void *	rz_print_zero_adj( struct fmt_zero_adj *txt, struct rzr033t *pkt);


/*------------------------------------------------------*/
/*	スパン校正テキストフォーマット			*/
/*							*/
/* rzr035,rzr036					*/
/*------------------------------------------------------*/

struct fmt_span_adj {
	BYTE	cmd;			/* command = 'A'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* 調整ヘッドビットパターン	*/
/*-------------------------------------*/
};

int	rz_scan_span_adj( struct fmt_span_adj *buf);
void *	rz_print_span_adj( struct fmt_span_adj *txt, struct rzr035t *pkt);


/*------------------------------------------------------*/
/*	運転開始テキストフォーマット			*/
/*							*/
/* rzr037,rzr038					*/
/*------------------------------------------------------*/

struct fmt_prod_run {
	BYTE	cmd;			/* command = 'R'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	infeed;			/* 供給機制御			*/
/*-------------------------------------*/
};

int	rz_scan_prod_run( struct fmt_prod_run *buf);
void *	rz_print_prod_run( struct fmt_prod_run *txt, struct rzr037t *pkt);


/*------------------------------------------------------*/
/*	運転停止テキストフォーマット			*/
/*							*/
/* rzr039,rzr040					*/
/*------------------------------------------------------*/

struct fmt_prod_stop {
	BYTE	cmd;			/* command = 'S'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_prod_stop( struct fmt_prod_stop *buf);
void *	rz_print_prod_stop( struct fmt_prod_stop *txt, struct rzr039t *pkt);


/*------------------------------------------------------*/
/*	全排出開始テキストフォーマット			*/
/*							*/
/* rzr041,rzr042					*/
/*------------------------------------------------------*/

struct fmt_drain_run {
	BYTE	cmd;			/* command = 'G'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	infeed;			/* 供給機制御モード		*/
	BYTE	auto_zero;		/* 自動零点制御モード		*/
/*-------------------------------------*/
};

int	rz_scan_drain_run( struct fmt_drain_run *buf);
void *	rz_print_drain_run( struct fmt_drain_run *txt, struct rzr041t *pkt);


/*------------------------------------------------------*/
/*	全排出停止テキストフォーマット			*/
/*							*/
/* rzr043,rzr044					*/
/*------------------------------------------------------*/

struct fmt_drain_stop {
	BYTE	cmd;			/* command = 'H'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_drain_stop( struct fmt_drain_stop *buf);
void *	rz_print_drain_stop( struct fmt_drain_stop *txt, struct rzr043t *pkt);


/*------------------------------------------------------*/
/*	テスト組み合わせ計算テキストフォーマット	*/
/*							*/
/* rzr045,rzr046					*/
/*------------------------------------------------------*/

struct fmt_test_calc {
	BYTE	cmd;			/* command = 'C'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_test_calc( struct fmt_test_calc *buf);
void *	rz_print_test_calc( struct fmt_test_calc *txt, struct rzr045t *pkt);


/*------------------------------------------------------*/
/*	テスト組み合わせ計算&排出テキストフォーマット	*/
/*							*/
/* rzr047,rzr048					*/
/*------------------------------------------------------*/

struct fmt_test_caldmp {
	BYTE	cmd;			/* command = 'M'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_test_caldmp( struct fmt_test_caldmp *buf);
void *	rz_print_test_caldmp( struct fmt_test_caldmp *txt, struct rzr047t *pkt);


/*------------------------------------------------------*/
/*	エラー解除テキストフォーマット			*/
/*							*/
/* rzr049,rzr050					*/
/*------------------------------------------------------*/

struct fmt_error_cancel {
	BYTE	cmd;			/* command = 'D'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_error_cancel( struct fmt_error_cancel *buf);
void *	rz_print_error_cancel( struct fmt_error_cancel *txt, struct rzr049t *pkt);


/*------------------------------------------------------*/
/*	テスト駆動開始テキストフォーマット		*/
/*							*/
/* rzr051,rzr052					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_run {
	BYTE	cmd;			/* command = 'P'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	rf_ptn[8];		/* RF pattern			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	wh2_ptn[8];		/* WH2 pattern(BH side)		*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[2];		/* RS pattern			*/
	BYTE	dth_ptn[2];		/* DTH pattern			*/
	BYTE	th_ptn[2];		/* TH pattern			*/
	BYTE	drv_cyc;		/* Drive cycle			*/
/*-------------------------------------*/
};

int	rz_scan_tstdrv_run( struct fmt_tstdrv_run *buf);
void *	rz_print_tstdrv_run( struct fmt_tstdrv_run *txt, struct rzr051t *pkt);


/*------------------------------------------------------*/
/*	テスト駆動停止テキストフォーマット		*/
/*							*/
/* rzr053,rzr054					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_stop {
	BYTE	cmd;			/* command = 'Q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_tstdrv_stop( struct fmt_tstdrv_stop *buf);
void *	rz_print_tstdrv_stop( struct fmt_tstdrv_stop *txt, struct rzr053t *pkt);

/*------------------------------------------------------*/
/*	組み合わせ計算結果テキストフォーマット		*/
/*							*/
/* rzi003,rzi004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	flag;			/* 演算結果フラグ		*/
	BYTE	weight[8];		/* 組み合わせ重量		*/
	BYTE	count[4];		/* 組み合わせ個数		*/
} P8;

struct fmt_cb_result {
	BYTE	cmd;			/* command = 'E'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P8	sect[8];		/* P8 paramter[nof_sect]	*/
	P8	ch;			/* P8   "			*/
	BYTE	slv_targ_wt[8];		/* スレーブ機目標重量		*/
	BYTE	slv_targ_cnt[4];	/* スレーブ機目標個数		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	BYTE	wh_status[32][2];	/* WHステータス[nof_wh]		*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	BYTE	bh_status[32][2];	/* WHステータス[nof_bh]		*/
/*-------------------------------------*/
};

int	rz_scan_cb_result( struct fmt_cb_result *buf);
void *	rz_print_cb_result( struct fmt_cb_result *txt, struct rzi003t *pkt);

/*------------------------------------------------------*/
/*	単重自動更新結果テキストフォーマット		*/
/*							*/
/* rzi005,rzi006					*/
/*------------------------------------------------------*/

struct fmt_piece_weight {
	BYTE	cmd;			/* command = 'p'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	BYTE	weight[8][4];		/* 単重[nof_sect]		*/
/*-------------------------------------*/
};

int	rz_scan_piece_weight( struct fmt_piece_weight *buf);
void *	rz_print_piece_weight( struct fmt_piece_weight *txt, struct rzi005t *pkt);


/*------------------------------------------------------*/
/*	補正値自動更新結果テキストフォーマット		*/
/*							*/
/* rzi007,rzi008					*/
/*------------------------------------------------------*/

struct fmt_cmpn_value {
	BYTE	cmd;			/* command = 'q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	cmpn_val[2];		/* 補正値			*/
/*-------------------------------------*/
};

int	rz_scan_cmpn_value( struct fmt_cmpn_value *buf);
void *	rz_print_cmpn_value( struct fmt_cmpn_value *txt, struct rzi007t *pkt);


/*------------------------------------------------------*/
/*	プログラム番号テキストフォーマット		*/
/*							*/
/* rzi009,rzi010					*/
/*------------------------------------------------------*/

struct fmt_prgno {
	BYTE	cmd;			/* command = 'Y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dev_id;			/* デバイスID番号		*/
	BYTE	head_no[2];		/* ヘッド番号			*/
	BYTE	rom_no[6];		/* プログラム番号		*/
	BYTE	ver_prefix[2];		/* バージョン番号		*/
	BYTE	ver_suffix[2];		/* レビジョン番号		*/
/*-------------------------------------*/
};

int	rz_scan_prgno( struct fmt_prgno *buf);
void *	rz_print_prgno( struct fmt_prgno *txt, struct rzi009t *pkt);


/*------------------------------------------------------*/
/*	秤データのテキストフォーマット			*/
/*							*/
/* rzi011,rzi012					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[2];		/* ヘッドステータス		*/
} P9;

struct fmt_head_weight {
	BYTE	cmd;			/* command = '\'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	P9	wh[32];			/* WH重量&ステータス[nof_wh]	*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	P9	bh[32];			/* WH重量&ステータス[nof_bh]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	P9	df[8];			/* WH重量&ステータス[nof_df]	*/
/*-------------------------------------*/
};

int	rz_scan_head_weight( struct fmt_head_weight *buf);
void *	rz_print_head_weight( struct fmt_head_weight *txt, struct rzi011t *pkt);


/*------------------------------------------------------*/
/*	本体の状態テキストフォーマット			*/
/*							*/
/* rzi013,rzi014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	condition;		/* 状態				*/
	BYTE	power_stat;		/* 駆動系電源状態		*/
	BYTE	infeed_stat;		/* 供給状態			*/
} P91;

struct fmt_condition {
	BYTE	cmd;			/* command = 'N'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	P91	ch[2];			/* P91 parameter of channel	*/
/*-------------------------------------*/
};

int	rz_scan_condition( struct fmt_condition	*buf);
void *	rz_print_condition( struct fmt_condition *txt, struct rzi013t *pkt);


/*------------------------------------------------------*/
/*	警報情報のテキストフォーマット			*/
/*							*/
/* rzi015,rzi016					*/
/*------------------------------------------------------*/

struct fmt_alarm {
	BYTE	cmd;			/* command = 'F'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	alarm;			/* 警報情報			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	afd[2];			/* AFD warning id		*/
/*-------------------------------------*/
};

int	rz_scan_alarm( struct fmt_alarm *buf);
void *	rz_print_alarm( struct fmt_alarm *txt, struct rzi015t *pkt);


/*------------------------------------------------------*/
/*	異常情報のテキストフォーマット			*/
/*							*/
/* rzi017,rzi018					*/
/*------------------------------------------------------*/

struct fmt_error {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	error;			/* 異常情報			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[2];		/* RS pattern			*/
	BYTE	dth_ptn[2];		/* DTH pattern			*/
	BYTE	th_ptn[2];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
/*-------------------------------------*/
};

int	rz_scan_error( struct fmt_error *buf);
void *	rz_print_error( struct fmt_error *txt, struct rzi017t *pkt);


/*------------------------------------------------------*/
/*	受信完了テキストフォーマット			*/
/*							*/
/* rzi019,rzi020					*/
/*------------------------------------------------------*/

struct fmt_echo {
	BYTE	cmd;			/* command = '='		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* 受信コマンド			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	rz_scan_echo( struct fmt_echo *buf);
void *	rz_print_echo( struct fmt_echo *txt, struct rzi019t *pkt);


/*------------------------------------------------------*/
/*	動作完了テキストフォーマット			*/
/*							*/
/* rzi021,rzi022					*/
/*------------------------------------------------------*/

struct fmt_complete {
	BYTE	cmd;			/* command = '>'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* 受信コマンド			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	rz_scan_complete( struct fmt_complete *buf);
void *	rz_print_complete( struct fmt_complete *txt, struct rzi021t *pkt);


/*------------------------------------------------------*/
/*	システムエラーテキストフォーマット		*/
/*							*/
/* rzi023,rzi024					*/
/*------------------------------------------------------*/

struct fmt_sys_fault {
	BYTE	cmd;			/* command = '?'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	len[2];			/* メッセージのバイト長		*/
	BYTE	mes_str[64];		/* メッセージ・ストリングス	*/
/*-------------------------------------*/
};

int	rz_scan_sys_fault( struct fmt_sys_fault *buf);
void *	rz_print_sys_fault( struct fmt_sys_fault *txt, struct rzi023t *pkt);

/*------------------------------------------------------*/
/*	ＡＦＤヘッド情報のテキストフォーマット		*/
/*							*/
/* rzr055,rzr056					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	trg[4];			/* 目標重量			*/
	BYTE	std[4];			/* 標準偏差			*/
} P92;

struct fmt_afd_head {
	BYTE	cmd;			/* command = 'h'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	P92	wh[32];			/* 目標＆標準偏差[nof_wh]	*/
/*-------------------------------------*/
};

int	rz_scan_afd_head( struct fmt_afd_head *buf);
void *	rz_print_afd_head( struct fmt_afd_head *txt, struct rzr055t *pkt);

/*------------------------------------------------------*/
/*	ＡＦＤセクション情報のテキストフォーマット	*/
/*							*/
/* rzr057,rzr058					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	ctrl_mode;		/* AFDモード			*/
	BYTE	ctrl_sw[4];		/* AFD動作ＳＷ			*/
	BYTE	start_delay[4];		/* 制御開始遅れサイクル数	*/
	BYTE	df_rf_ratio[2];		/* DF-RF振幅比			*/
	BYTE	target_head[2];		/* 目標選択ヘッド数		*/
	BYTE	amp_center[2];		/* RF振幅制御中心値		*/
	BYTE	amp_max[2];		/* 振幅絶対上限			*/
	BYTE	amp_min[2];		/* 振幅絶対下限			*/
	BYTE	amp_range[2];		/* 振幅制御範囲			*/
} P71;

struct fmt_afd_sect {
	BYTE	cmd;			/* command = 's'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P71	sect[8];		/* P7 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	rz_scan_afd_sect( struct fmt_afd_sect *buf);
void *	rz_print_afd_sect( struct fmt_afd_sect *txt, struct rzr057t *pkt);

/*------------------------------------------------------*/
/*	平均値制御結果テキストフォーマット		*/
/*							*/
/* rzi025,rzi026					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[8];		/* 組み合わせ重量		*/
	BYTE	count[8];		/* 組み合わせ個数		*/
} P81;

struct fmt_mean_result {
	BYTE	cmd;			/* command = 'm'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P81	sect[8];		/* P8 paramter[nof_sect]	*/
	P81	ch;			/* P8   "			*/
/*-------------------------------------*/
};

int	rz_scan_mean_result( struct fmt_mean_result *buf);
void *	rz_print_mean_result( struct fmt_mean_result *txt, struct rzi025t *pkt);

/*------------------------------------------------------*/
/*	周辺異常情報のテキストフォーマット		*/
/*							*/
/* rzi027,rzi028					*/
/*------------------------------------------------------*/

struct fmt_pcuerr {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	error;			/* 異常情報			*/
	BYTE	ptn[8];			/* pattern			*/
/*-------------------------------------*/
};

int	rz_scan_pcuerr( struct fmt_pcuerr *buf);
void *	rz_print_pcuerr( struct fmt_pcuerr *txt, struct rzi027t *pkt);

/*------------------------------------------------------*/
/*	親ヘッド計量目標値の設定テキストフォーマット	*/
/*							*/
/* rzr059,rzr060					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	no[2];			/* ヘッド番号			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	lower_wt[4];		/* 下限重量			*/
	BYTE	rf_ctrl;		/* ＲＦ駆動			*/
	BYTE	ph_fd[2];		/* PH-供給信号遅れ時間		*/
} PR591;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR591	phead[2];		/* PI591 paramter[nof_phead]	*/
	BYTE	on_off;			/* 親子計量する／しない		*/
	BYTE	dump;			/* 同時排出／交互排出		*/
	BYTE	calc;			/* 計算参加／不参加		*/
	BYTE	az_intvl[4];		/* 自動零点調整間隔		*/
	BYTE	over;			/* 上限以上時の処理		*/
	BYTE	under;			/* 下限未満時の処理		*/
} PR592;

struct fmt_pandc_parm {
	BYTE	cmd;			/* command = 'o'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR592	sect[8];		/* PR592 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	rz_scan_pandc_parm( struct fmt_pandc_parm *buf);
void *	rz_print_pandc_parm( struct fmt_pandc_parm *txt, struct rzr059t *pkt);

/*------------------------------------------------------*/
/*	親ヘッド計算結果テキストフォーマット		*/
/*							*/
/* rzi029,rzi030					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* 重量				*/
	BYTE	wh_status[2];		/* WHステータス	 		*/
	BYTE	bh_status[2];		/* BHステータス 		*/
} PI291;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PI291	phead[2];		/* PI291 paramter[nof_phead]	*/
} PI292;

struct fmt_pandc_result {
	BYTE	cmd;			/* command = 'O'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PI292	sect[8];		/* PI292 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	rz_scan_pandc_result( struct fmt_pandc_result *buf);
void *	rz_print_pandc_result( struct fmt_pandc_result *txt, struct rzi029t *pkt);

/*------------------------------------------------------*/
/*	ＥＥＰＲＯＭゼロクリアテキストフォーマット	*/
/*							*/
/* rzr061,rzr062					*/
/*------------------------------------------------------*/

struct fmt_eeprom_clr {
	BYTE	cmd;			/* command = 'z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/

/*-------------------------------------*/
};

int		rz_scan_eeprom_clr( void * string);
#define		rz_scan_eeprom_clr()	((void)0)
void *		rz_print_eeprom_clr( void * string, void * struc);
#define		rz_print_eeprom_clr()	((void)0)

/*------------------------------------------------------*/
/*	CF制御結果テキストフォーマット			*/
/*							*/
/* rzi031,rzi032					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* 供給状態数			*/
	P5	at[8];			/* P5 paramter[nof_sig]		*/
	BYTE	sig_stat[2];		/* 供給状態			*/
	BYTE	df_wt[4];		/* 合成分散重量			*/
} PI310;

struct fmt_cf_rslt {
	BYTE	cmd;			/* command = 'k'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	seq_no;			/* シーケンス番号		*/
	BYTE	nof_cf;			/* CF数				*/
	PI310	cf[8];			/* P0210 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	rz_scan_cf_rslt( struct fmt_cf_rslt *buf);
void *	rz_print_cf_rslt( struct fmt_cf_rslt *txt, struct rzi031t *pkt);

/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（１）			*/
/*		テキストフォーマット			*/
/*							*/
/* rzr123,rzr124					*/
/*------------------------------------------------------*/

struct fmt_prec_dump {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	BYTE	cnt[8][2];		/* 優先排出回数			*/
/*-------------------------------------*/
};

int	rz_scan_prec_dump(struct fmt_prec_dump *buf);
void *	rz_print_prec_dump(struct fmt_prec_dump	*txt, struct rzr123t *pkt);

/*------------------------------------------------------*/
/*	付加装置パラメータ				*/
/*		テキストフォーマット			*/
/*							*/
/* rzr063,rzr064					*/
/*------------------------------------------------------*/

struct updown_shoot_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dev_id[2];		/* device ID = 01 上下動シュート*/
	BYTE	nof_pos[2];		/* ポジション数			*/
	BYTE	err_detect_interval[2];	/* エラー検出時間（*10ms）	*/
	BYTE	position[2];		/* 設定位置			*/
/*-------------------------------------*/
};

int	rz_scan_updown_shoot(struct updown_shoot_parm *buf);
void *	rz_print_updown_shoot(struct updown_shoot_parm *txt, struct rzr063t *pkt);

/*------------------------------------------------------*/
/*	ユーザレベルのテキストフォーマット		*/
/*							*/
/* rzr065,rzr066					*/
/*------------------------------------------------------*/

struct fmt_usr_level {
	BYTE	cmd;			/* command = 'l'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	usr_level;		/* ユーザレベル			*/
/*-------------------------------------*/
};

int	rz_scan_usr_level( struct fmt_usr_level *buf);
void *	rz_print_usr_level( struct fmt_usr_level *txt, struct rzr065t *pkt);


/*------------------------------------------------------*/
/*    組み合わせ計算拡張パラメータテキストフォーマット  */
/*							*/
/* rzr111,rzr112					*/
/*------------------------------------------------------*/

struct fmt_approval {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	approval_flag;		/* approval flag		*/
	BYTE	vol_flag;		/* vol flag			*/
	BYTE	max_weight[4];		/* max. weight of approval	*/
	BYTE	min_weight[4];		/* min. weight of approval	*/
/*-------------------------------------*/
};

int	rz_scan_approval( struct fmt_approval *buf);
void *	rz_print_approval( struct fmt_approval *txt, struct rzr111t *pkt);


/*------------------------------------------------------*/
/*	金属検出機テストのテキストフォーマット		*/
/*							*/
/* rzr067,rzr068					*/
/*------------------------------------------------------*/

struct fmt_metal_text {
	BYTE	cmd;			/* command = '{'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	function;		/* 0:テスト準備 1:テスト開始	*/
/*-------------------------------------*/
};

int	rz_scan_metal_text( struct fmt_metal_text *buf);
void *	rz_print_metal_text( struct fmt_metal_text *txt, struct rzr067t *pkt);


/*------------------------------------------------------*/
/*	金属検出機テスト結果のテキストフォーマット	*/
/*							*/
/* rzi033,rzi034					*/
/*------------------------------------------------------*/

struct fmt_metal_answer {
	BYTE	cmd;			/* command = '}'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	answer;			/* 0:成功 1:失敗 2:中止 3:SD中止*/
/*-------------------------------------*/
};

int	rz_scan_metal_answer( struct fmt_metal_answer *buf);
void *	rz_print_metal_answer( struct fmt_metal_answer *txt, struct rzi033t *pkt);

/*------------------------------------------------------*/
/*   本体状態情報拡張パラメータテキストフォーマット	*/
/*							*/
/* rzi113,rzr114					*/
/*------------------------------------------------------*/

struct fmt_df_lowproduct {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* DFの数			*/
	BYTE	infeed_stat[8];		/* 供給状態			*/
/*-------------------------------------*/
};

int	rz_scan_lowproduct(struct fmt_df_lowproduct *buf);
void *	rz_print_lowproduct(struct fmt_df_lowproduct *txt, struct rzi113t *pkt);

/*------------------------------------------------------*/
/*   供給量の設定拡張パラメータテキストフォーマット	*/
/*							*/
/* rzr119,rzr120					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	targ[4];		/* 目標重量			*/
	BYTE	upper[4];		/* 上限重量			*/
	BYTE	lower[4];		/* 下限重量			*/
} PR119;

struct fmt_dfspan {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* DFの数			*/
	PR119	spanwt[8];		/* レベル重量スパン[nof_df]	*/
/*-------------------------------------*/
};

int	rz_scan_dfspan(struct fmt_dfspan *buf);
void *	rz_print_dfspan(struct fmt_dfspan *txt, struct rzr119t *pkt);


/*------------------------------------------------------*/
/*	サブ異常情報のテキストフォーマット		*/
/*							*/
/* rzi117,rzi118					*/
/*------------------------------------------------------*/

struct fmt_error_sub {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	error[2];		/* 異常情報	part1		*/
	BYTE	error_ptn[8];		/* error pattern		*/
/*-------------------------------------*/
};

int	rz_scan_error_sub( struct fmt_error_sub *buf);
void *	rz_print_error_sub( struct fmt_error_sub *txt, struct rzi117t *pkt);


/*------------------------------------------------------*/
/*    組み合わせ計算拡張パラメータテキストフォーマット  */
/*							*/
/* rzr211,rzr212					*/
/*------------------------------------------------------*/

struct fmt_approval_sub {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	e1_ad[4];		/* e1 weight of approval */
/*-------------------------------------*/
};

int	rz_scan_approval_sub( struct fmt_approval_sub *buf);
void *	rz_print_approval_sub( struct fmt_approval_sub *txt, struct rzr211t *pkt);


/*------------------------------------------------------*/
/*	サンプル排出のテキストフォーマット		*/
/*							*/
/* rzr069,rzr070					*/
/*------------------------------------------------------*/

struct fmt_sample_dump {
	BYTE	cmd;			/* command = ':'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	sect_assign_ptn[8];	/* section  pattern		*/
/*-------------------------------------*/
};

int	rz_scan_sample_dump( struct fmt_sample_dump *buf);
void *	rz_print_sample_dump( struct fmt_sample_dump *txt, struct rzr069t *pkt);


/*------------------------------------------------------*/
/*    タイミング拡張パラメータテキストフォーマット  	*/
/*							*/
/* rzr117,rzr118					*/
/*------------------------------------------------------*/

struct fmt_rz600_tmg {
	BYTE	prm_pnt[4];		/* pointer o option parameter	*/
/*---------- parameter ----------------*/
	BYTE	cnv_delay[2];		/* 搬送コンベヤ遅れ時間		*/
	BYTE	cnv_hold_tm[2];		/* 搬送コンベヤオン時間		*/
	BYTE	srv_delay[2];		/* 排出用サーボ遅れ時間		*/
	BYTE	srv_hold_tm[2];		/* サーボ上限停止時間		*/
	BYTE	sv_delay[2];		/* 排出用シリンダ遅れ時間	*/
	BYTE	sv_hold_tm[2];		/* 排出用シリンダオン時間	*/
	BYTE	men1_delay[2];		/* 麺押し１遅れ時間		*/
	BYTE	men1_hold_tm[2];	/* 麺押し１オン時間		*/
	BYTE	men2_delay[2];		/* 麺押し２遅れ時間		*/
	BYTE	men2_hold_tm[2];	/* 麺押し２オン時間		*/
	BYTE	men3_delay[2];		/* 麺押し３遅れ時間		*/
	BYTE	men3_hold_tm[2];	/* 麺押し３オン時間		*/
	BYTE	men4_delay[2];		/* 麺押し４遅れ時間		*/
	BYTE	men4_hold_tm[2];	/* 麺押し４オン時間		*/
/*-------------------------------------*/
};

int	rz_scan_rz600_tmg( struct fmt_rz600_tmg *buf);
void *	rz_print_rz600_tmg( struct fmt_rz600_tmg *txt, struct rzr117t *pkt);

/*------------------------------------------------------*/
/*	ダミー計量開始テキストフォーマット		*/
/*							*/
/* rzr073,rzr074					*/
/*------------------------------------------------------*/

struct fmt_dummy_weight {
	BYTE	cmd;			/* command = '$'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_dummy_weight( struct fmt_dummy_weight *buf);
void *	rz_print_dummy_weight( struct fmt_dummy_weight *txt, struct rzr073t *pkt);


/*------------------------------------------------------*/
/*   供給機のON時間、OFF時間  テキストフォーマット	*/
/*							*/
/* rzr219,rzr220					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	on_time[2];		/* ＯＮ時間			*/
	BYTE	off_time[2];		/* ＯＦＦ時間			*/
} PR219;

struct fmt_df_time {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* DFの数			*/
	PR219	df[8];			/* df time[nof_df]		*/
/*-------------------------------------*/
};

int	rz_scan_df_time(struct fmt_df_time *buf);
void *	rz_print_df_time(struct fmt_df_time *txt, struct rzr219t *pkt);


/*------------------------------------------------------*/
/*  ＤＣＡパラメータコマンド          			*/
/*		  	テキストフォーマット            */
/* rzr075,rzr076					*/
/*------------------------------------------------------*/

struct fmt_dca_parm {
	BYTE	cmd;			/* command = '!'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_on_off;		/* DCA ON/OFF       		*/
	BYTE	dca_cycle[4];		/* DCA cycle count  		*/
	BYTE	dca_owf[2];		/* DCA OWF             		*/
	BYTE	dca_sdm[2];		/* DCA SDM          		*/
/*-------------------------------------*/
};

int	rz_scan_dca_parm(struct fmt_dca_parm *buf);
void *	rz_print_dca_parm(struct fmt_dca_parm *txt, struct rzr075t *pkt);


/*------------------------------------------------------*/
/*  	ＡＴＡ偏差      テキストフォーマット            */
/*                                                      */
/* rzr077,rzr078					*/
/*------------------------------------------------------*/

struct fmt_ata_dev {
	BYTE	cmd;			/* command = '"'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_dev[8];		/* ATA deviation 		*/
/*-------------------------------------*/
};

int	rz_scan_ata_dev(struct fmt_ata_dev *buf);
void *	rz_print_ata_dev(struct fmt_ata_dev *txt, struct rzr077t *pkt);


/*------------------------------------------------------*/
/*	ＤＣＡモニター情報  テキストフォーマット	*/
/*							*/
/* rzi037,rzi038					*/
/*------------------------------------------------------*/

struct fmt_dca_monitor {
	BYTE	cmd;			/* command = '('		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_sum_cnt[4];		/* DCAサンプルカウント値   	*/
	BYTE	bf_cbwt[8];		/* 再チェック前重量		*/
	BYTE	af_cbwt[8];		/* 再チェック後重量		*/
/*-------------------------------------*/
};

int	rz_scan_dca_monitor( struct fmt_dca_monitor *buf);
void *	rz_print_dca_monitor( struct fmt_dca_monitor *txt, struct rzi037t *pkt);


/*------------------------------------------------------*/
/*	ＡＴＡモニター情報  テキストフォーマット	*/
/*							*/
/* rzi039,rzi040					*/
/*------------------------------------------------------*/

struct fmt_ata_monitor {
	BYTE	cmd;			/* command = ')'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_enable;		/* ＡＴＡ  enable       	*/
	BYTE	ata_timer[4];		/* ＡＴＡタイマー値		*/
	BYTE	ata_mode;		/* 現在のＡＴＡモード		*/
	BYTE	ata_tgt[8];		/* ＡＴＡターゲット		*/
	BYTE	ata_def_time[4];	/* ATA default time		*/
	BYTE	ata_cwb[4];		/* ＣＷＢ			*/
/*-------------------------------------*/
};

int	rz_scan_ata_monitor( struct fmt_ata_monitor *buf);
void *	rz_print_ata_monitor( struct fmt_ata_monitor *txt, struct rzi039t *pkt);


/*<<<<<<<<<<<  Ver3  ===================================================*/
/*------------------------------------------------------*/
/*	プログラム番号の要求テキストフォーマット	*/
/*							*/
/* rzr025a,rzr026a					*/
/*------------------------------------------------------*/

struct fmt_req_prgno_v3 {
	BYTE	cmd;			/* command = ','		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no;			/* デバイス番号			*/
	BYTE	node_id[2];		/* ノード番号			*/
/*-------------------------------------*/
};

int	rz_scan_req_prgno_v3( struct fmt_req_prgno_v3 *buf);
void *	rz_print_req_prgno_v3( struct fmt_req_prgno_v3 *txt, struct rzr025t *pkt);


/*------------------------------------------------------*/
/*	プログラム番号テキストフォーマット		*/
/*							*/
/* rzi009a,rzi010a					*/
/*------------------------------------------------------*/

struct fmt_prgno_v3 {
	BYTE	cmd;			/* command = 'Y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no;			/* デバイス番号			*/
	BYTE	node_id[2];		/* ノード番号			*/
	BYTE	len[2];			/* ＩＤ文字列の長さ		*/
	BYTE	rom_id[128];		/* ROM ID文字列			*/
/*-------------------------------------*/
};

int	rz_scan_prgno_v3( struct fmt_prgno_v3 *buf);
void *	rz_print_prgno_v3( struct fmt_prgno_v3	*txt, struct rzi009t *pkt);


/*------------------------------------------------------*/
/*	本体の状態テキストフォーマット			*/
/*							*/
/* rzi013a,rzi014a					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	condition;		/* 状態				*/
	BYTE	power_stat;		/* 駆動系電源状態		*/
	BYTE	infeed_stat;		/* 供給状態			*/
} P91a;

struct fmt_condition_v3 {
	BYTE	cmd;			/* command = 'N'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	P91a	ch[2];			/* P91 parameter of channel	*/
	BYTE	df_infeed_stat[2];	/* 分散フィーダ供給状態		*/
/*-------------------------------------*/
};

int	rz_scan_condition_v3( struct fmt_condition_v3 *buf);
void *	rz_print_condition_v3( struct fmt_condition_v3 *txt, struct rzi013t *pkt);


/*------------------------------------------------------*/
/*	異常情報のテキストフォーマット			*/
/*							*/
/* rzi017a,rzi018a					*/
/*------------------------------------------------------*/

struct fmt_error_v3 {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	error[2];		/* 異常情報			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[2];		/* RS pattern			*/
	BYTE	dth_ptn[2];		/* DTH pattern			*/
	BYTE	th_ptn[2];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	duc_inf;		/* DUC error information	*/
	BYTE	exc_inf;		/* EXC error information	*/
/*-------------------------------------*/
};

int	rz_scan_error_v3( struct fmt_error_v3 *buf);
void *	rz_print_error_v3( struct fmt_error_v3	*txt, struct rzi017t *pkt);


/*------------------------------------------------------*/
/*	ネットワーク参加状態の要求テキストフォーマット	*/
/*							*/
/* rzr071,rzr072					*/
/*------------------------------------------------------*/

struct fmt_req_netmap {
	BYTE	cmd;			/* command = '['		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no[2];		/* デバイス番号			*/
/*-------------------------------------*/
};

int	rz_scan_req_netmap( struct fmt_req_netmap *buf);
void *	rz_print_req_netmap( struct fmt_req_netmap *txt, struct rzr071t *pkt);


/*------------------------------------------------------*/
/*	ネットワーク参加状況テキストフォーマット	*/
/*							*/
/* rzi035,rzi036					*/
/*------------------------------------------------------*/

struct fmt_netmap {
	BYTE	cmd;			/* command = ']'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no[2];		/* デバイス番号			*/
	BYTE	net_map[64];		/* 参加状態テーブル		*/
	BYTE	recon[4];		/* リコンフィグ発生回数		*/
/*-------------------------------------*/
};

int	rz_scan_netmap( struct fmt_netmap *buf);
void *	rz_print_netmap( struct fmt_netmap *txt, struct rzi035t *pkt);

/*=====================================================Ver3 >>>>>>>>>>>>*/

/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（２）			*/
/*			テキストフォーマット		*/
/*	アベコン・サイクル／レート			*/
/* rzr223,rzr224					*/
/*------------------------------------------------------*/

struct fmt_avg_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	avg_cycle[2];		/* average cycle  		*/
	BYTE	avg_ratio[2];		/* average ratio 		*/
/*-------------------------------------*/
};

int	rz_scan_avg_parm( struct fmt_avg_parm *buf);
void *	rz_print_avg_parm( struct fmt_avg_parm *txt, struct rzr223t *pkt);

/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（３）			*/
/*		テキストフォーマット			*/
/*	ＷＨ不良排出設定（各セクション）		*/
/* rzr323,rzr324					*/
/*------------------------------------------------------*/

struct fmt_err_div_wh{
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	BYTE	div[8][1];		/* 排出切り替えON/OFF[nof_sect]	*/
/*-------------------------------------*/
};

int	rz_scan_err_div_wh(struct fmt_err_div_wh *buf);
void *	rz_print_err_div_wh(struct fmt_err_div_wh *txt, struct rzr323t *pkt);

/*------------------------------------------------------*/
/*	CFの振幅設定テキストフォーマット２（最大１６）	*/
/*							*/
/* rzr079,rzr080					*/
/*------------------------------------------------------*/

struct fmt_cf2_parm {
	BYTE	cmd;			/* command = 'n'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ctrl;			/* control			*/
	BYTE	nof_cf[2];		/* CF数				*/
	PR210	cf[16];			/* PR210 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	rz_scan_cf2_parm( struct fmt_cf2_parm *buf);
void *	rz_print_cf2_parm( struct fmt_cf2_parm *txt, struct rzr079t *pkt);


/*------------------------------------------------------*/
/* 親ＢＨオーバースケール重量の設定テキストフォーマット	*/
/*							*/
/* rzr159,rzr160					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	bh_reject_wt[8];	/* ＢＨオーバースケール重量	*/
} PR1591;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR1591	phead[2];		/* PI591 paramter[nof_phead]	*/
} PR1592;

struct fmt_p_reject_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR1592	sect[8];		/* PR592 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	rz_scan_p_reject_parm( struct fmt_p_reject_parm *buf);
void *	rz_print_p_reject_parm( struct fmt_p_reject_parm *txt, struct rzr159t *pkt);

/*------------------------------------------------------*/
/*    組み合わせ計算拡張パラメータテキストフォーマット  */
/*    拡張（３）	ＭＡＸ  ＳＣＡＬＥ		*/
/* rzr311,rzr312					*/
/*------------------------------------------------------*/

struct fmt_max_scale {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	max_scale;		/* max scale          		*/
/*-------------------------------------*/
};

int	rz_scan_max_scale( struct fmt_max_scale *buf);
void *	rz_print_max_scale( struct fmt_max_scale *txt, struct rzr311t *pkt);

/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（４）			*/
/*		テキストフォーマット			*/
/*	ＡＴＡ  ＳＡＭＰＬＥ  ＳＩＺＥ			*/
/* rzr423,rzr424					*/
/*------------------------------------------------------*/

struct fmt_sample_size {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	sample_size[4];		/* ata sample size         	*/
	BYTE	preset_num[2];		/* プリセット番号         	*/
/*-------------------------------------*/
};

int	rz_scan_sample_size(struct fmt_sample_size *buf);
void *	rz_print_sample_size(struct fmt_sample_size *txt, struct rzr423t *pkt);

/*------------------------------------------------------*/
/*	組み合わせ計算結果テキストフォーマット（拡張１）*/
/*	回数カウント、排出フラグ			*/
/* rzi103,rzi104					*/
/*------------------------------------------------------*/
struct fmt_dump_cnt {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dump_cnt[2];		/* 回数計量カウント		*/
	BYTE	dump_flag;		/* 排出フラグ            	*/
/*-------------------------------------*/
};

int	rz_scan_dump_cnt( struct fmt_dump_cnt *buf);
void *	rz_print_dump_cnt( struct fmt_dump_cnt *txt, struct rzi103t *pkt);

/*------------------------------------------------------*/
/*   本体状態情報拡張パラメータテキストフォーマット	*/
/*	（拡張１）					*/
/* rzi113a,rzi114a					*/
/*------------------------------------------------------*/

struct fmt_ata_target {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	ata_target[2][8];	/* ata target			*/
/*-------------------------------------*/
};

int	rz_scan_ata_target(struct fmt_ata_target *buf);
void *	rz_print_ata_target(struct fmt_ata_target *txt,struct rzi113t *pkt);


/*------------------------------------------------------*/
/*	拡張タイミングの設定テキストフォーマット	*/
/*							*/
/* rzr217,rzr218					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	parent_delay[2];	/* 親ヘッド遅れ時間		*/
} P43;

typedef struct {
	BYTE	nof_parent;		/* 該当するchのセクション数	*/
	P43	pandc[2];		/* paramter[nof_parent]		*/
} P42;

struct fmt_parent_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	P42	sect[8];		/* P4 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	rz_scan_parent_tmg( struct fmt_parent_tmg *buf);
void *	rz_print_parent_tmg( struct fmt_parent_tmg *txt, struct rzr217t *pkt);

/*------------------------------------------------------*/
/*	タイマ設定用テキストフォーマット              	*/
/*							*/
/* rzr081,rzr082					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	port;			/* ポート番号      		*/
	BYTE	init[2];		/* 初期時間設定			*/
	BYTE	off[2];			/* ＯＦＦ時間設定		*/
	BYTE	on[2];			/* ＯＮ時間設定			*/
} PR810;

struct fmt_timer_parm {
	BYTE	cmd;			/* command = '#'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_port;		/* 該当するポート数		*/
	PR810	timer[16];		/* PR810 paramter[nof_port]	*/
/*-------------------------------------*/
};

int	rz_scan_timer_parm( struct fmt_timer_parm *buf);
void *	rz_print_timer_parm( struct fmt_timer_parm *txt, struct rzr081t *pkt);

/*------------------------------------------------------*/
/*	秤の計量特性テキストフォーマット（拡張１）	*/
/*							*/
/* rzr109,rzr110					*/
/*------------------------------------------------------*/

struct fmt_left_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	left_weight_cnt[4];	/* 付着重量許容値  */
/*-------------------------------------*/
};

int	rz_scan_left_weight( struct fmt_left_weight *buf);
void *	rz_print_left_weight( struct fmt_left_weight *txt, struct rzr109t *pkt);

/*------------------------------------------------------*/
/*    組み合わせ計算拡張パラメータテキストフォーマット  */
/*    拡張（４）	ホッパ取り替えビットパターン	*/
/* rzr411,rzr412					*/
/*------------------------------------------------------*/

struct fmt_wh_change {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_chg_ptn[8];		/* WH active Change head[nof_head/4]	*/
/*-------------------------------------*/
};

int	rz_scan_wh_change( struct fmt_wh_change *buf);
void *	rz_print_wh_change( struct fmt_wh_change *txt, struct rzr411t *pkt);

/*------------------------------------------------------*/
/*	ＷＨ用のラウンディングテキストフォーマット	*/
/*							*/
/* rzr317,rzr318					*/
/*------------------------------------------------------*/

struct fmt_round_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_round;		/* ラウンディンググループ数	*/
	BYTE	round_base[8][2];	/* ラウンディング排出基本時間	*/
/*-------------------------------------*/
};

int	rz_scan_round_tmg( struct fmt_round_tmg *buf);
void *	rz_print_round_tmg( struct fmt_round_tmg *txt, struct rzr317t *pkt);

/*------------------------------------------------------*/
/*  シフトデータ参照（要求）仕様テキストフォーマット    */
/*							*/
/* rzr083,rzr084					*/
/*------------------------------------------------------*/
struct fmt_shift_mon {
	BYTE	cmd;			/* command = '%'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	req_type;		/* reqest type   		*/
	BYTE	data_page[2];		/* data page          		*/
/*-------------------------------------*/
};

int	rz_scan_shift_mon( struct fmt_shift_mon *buf);
void *	rz_print_shift_mon( struct fmt_shift_mon *txt, struct rzr083t *pkt);

/*------------------------------------------------------*/
/*    シフトデータ応答結果データテキストフォーマット    */
/*							*/
/* rzi041,rzi042					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE		sta_flag;	/* state flag			*/
	BYTE		weight[8];	/* 重量				*/
	BYTE		count[8];	/* 個数 			*/
	BYTE         	ser_no[2];	/* シリアル番号			*/
} PI410;

struct fmt_shift_data {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	all_num[2];		/* all data num 		*/
	BYTE	data_page[2];		/* data page          		*/
	BYTE	data_num;		/* ページ内のデータ数		*/
	PI410 	data[16];		/* 各データ内容			*/

/*-------------------------------------*/
};

int	rz_scan_shift_data( struct fmt_shift_data *buf);
void *	rz_print_shift_data( struct fmt_shift_data *txt, struct rzi041t *pkt);


/*------------------------------------------------------*/
/*	スパン校正テキストフォーマット（拡張）		*/
/*							*/
/* rzr135,rzr136					*/
/*------------------------------------------------------*/

struct fmt_span_date {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	date[8];		/* 調整日付			*/
	BYTE	time[8];		/* 調整時間			*/
/*-------------------------------------*/
};

int	rz_scan_span_date( struct fmt_span_date *buf);
void *	rz_print_span_date( struct fmt_span_date *txt, struct rzr135t *pkt);


/*------------------------------------------------------*/
/*	スパン係数要求テキストフォーマット		*/
/*							*/
/* rzr085,rzr086					*/
/*------------------------------------------------------*/

struct fmt_span_call {
	BYTE	cmd;			/* command = 'J'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ＡＤＣボード識別番号		*/
/*-------------------------------------*/
};

int	rz_scan_span_call( struct fmt_span_call *buf);
void *	rz_print_span_call( struct fmt_span_call *txt, struct rzr085t *pkt);


/*------------------------------------------------------*/
/*	PLCデータ書込みテキストフォーマット		*/
/*							*/
/* rzr087,rzr088					*/
/*------------------------------------------------------*/

struct fmt_plc_wr {
	BYTE	cmd;			/* command = '''		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	plc_wr_data[25][4];	/* PLC write data		*/
	BYTE	dump_mode[4];		/* dump mode			*/
/*-------------------------------------*/
};

int	rz_scan_plc_data( struct fmt_plc_wr *buf);
void *	rz_print_plc_data( struct fmt_plc_wr *txt, struct rzr087t *pkt);


/*------------------------------------------------------*/
/*	スパン係数応答テキストフォーマット		*/
/*							*/
/* rzi043,rzi044					*/
/*------------------------------------------------------*/

struct fmt_span_coeffi {
	BYTE	cmd;			/* command = 'K'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ＡＤＣボード識別番号		*/
	BYTE	date[8];		/* 調整日付			*/
	BYTE	time[8];		/* 調整時間			*/
	BYTE	span_coeffi[16][4];	/* ＳＰＡＮ係数			*/
/*-------------------------------------*/
};

int	rz_scan_span_coeffi( struct fmt_span_coeffi *buf);
void *	rz_print_span_coeffi( struct fmt_span_coeffi *txt, struct rzi043t *pkt);

/*------------------------------------------------------*/
/*	セクション毎のAFD設定テキストフォーマット	*/
/*							*/
/* rzr319,rzr320					*/
/*------------------------------------------------------*/

struct fmt_sect_afd_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchに属するｾｸｼｮﾝ数	*/
	BYTE	afd_mode[8][1];		/* AFD mode			*/
/*-------------------------------------*/
};

int	rz_scan_sect_afd_parm( struct fmt_sect_afd_parm	*buf);
void *	rz_print_sect_afd_parm( struct fmt_sect_afd_parm *txt, struct rzr319t *pkt);

/*------------------------------------------------------*/
/* マルチ連動パラメータ1リクエストテキストフォーマット	*/
/*							*/
/* rzr089,rzr090					*/
/*------------------------------------------------------*/

struct fmt_mlt_p1req {
	BYTE	cmd;			/* command = '_'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_mlt_p1req( struct fmt_mlt_p1req *buf);
void *	rz_print_mlt_p1req( struct fmt_mlt_p1req *txt, struct rzr089t *pkt);

/*------------------------------------------------------*/
/* マルチ連動パラメータ1テキストフォーマット		*/
/*							*/
/* rzr091,rzr092					*/
/*------------------------------------------------------*/

struct fmt_mlt_parm1 {
	BYTE	cmd;			/* command = '-'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	neighbor_node[2];	/* 前段ノード番号		*/
	BYTE	shift_cnt[2];		/* シフト数			*/
	BYTE	retry_time[2];		/* リトライ時間			*/
	BYTE	retry_span[2];		/* リトライ間隔			*/
	BYTE	retry_cnt[2];		/* リトライ回数			*/
	BYTE	master_slave[2];	/* マスター／スレーブ設定	*/
	BYTE	ch_status[2];		/* チャンネル設定		*/
	BYTE	dsw[4];			/* NHIC DSW2 ビットフィールド	*/
/*-------------------------------------*/
};

int	rz_scan_mlt_parm1( struct fmt_mlt_parm1 *buf);
void *	rz_print_mlt_parm1( struct fmt_mlt_parm1 *txt, struct rzr091t *pkt);

/*------------------------------------------------------*/
/* マルチ連動パラメータ２テキストフォーマット		*/
/*							*/
/* rzr093,rzr094					*/
/*------------------------------------------------------*/

struct fmt_mlt_parm2 {
	BYTE	cmd;			/* command = '^'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	multi_onoff[2];		/* マルチ連動 ＯＮ／ＯＦＦ設定	*/
	BYTE	multi_enter[2];		/* マルチ連動 参加／不参加	*/
	BYTE	combi_delay[2];		/* 計量遅れ時間			*/
	BYTE	consec_mode[2];		/* 順次補正モード		*/
	BYTE	mlt_dump_mode[2];	/* 回数計量排出モード		*/
	BYTE	interface[2];		/* 包装機連動方式		*/
	BYTE	speed[2];		/* 自走時間			*/
	BYTE	conv_delay[2];		/* ｺﾝﾍﾞｱ起動信号遅れ時間	*/
	BYTE	conv_on_time[2];	/* ｺﾝﾍﾞｱ起動信号オン時間	*/
	BYTE	ds_delay[2];		/* 排出完了信号遅れ時間		*/
	BYTE	ds_on_time[2];		/* 排出完了信号オン時間		*/
	BYTE	ds_sht_cnt[2];		/* 排出完了信号シフト数		*/
	BYTE	mlt_dump_conf[2];	/* 回数計量排出完了信号出力ﾓｰﾄﾞ	*/
	BYTE	p_ds_delay[2];		/* 正量排出完了信号遅れ時間	*/
	BYTE	p_ds_on_time[2];	/* 正量排出完了信号オン時間	*/
	BYTE	p_ds_sht_cnt[2];	/* 正量排出完了信号シフト数	*/
/*-------------------------------------*/
};

int	rz_scan_mlt_parm2( struct fmt_mlt_parm2 *buf);
void *	rz_print_mlt_parm2( struct fmt_mlt_parm2 *txt, struct rzr093t *pkt);

/*------------------------------------------------------*/
/* マルチ連動拡張パラメータテキストフォーマット		*/
/*							*/
/* rzr095,rzr096					*/
/*------------------------------------------------------*/

struct fmt_mlt_ext {
	BYTE	cmd;			/* command = '<'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	s_cmd;			/* sub command			*/
	BYTE	p1[2];			/* parm1			*/
	BYTE	p2[2];			/* parm2			*/
	BYTE	p3[2];			/* parm3			*/
	BYTE	p4[2];			/* parm4			*/
/*-------------------------------------*/
};

int	rz_scan_mlt_ext( struct fmt_mlt_ext *buf);
void *	rz_print_mlt_ext( struct fmt_mlt_ext *txt, struct rzr095t *pkt);

/*------------------------------------------------------*/
/*	ＡＴＡモニター情報  テキストフォーマット	*/
/*	（拡張１）ステイル・ロックタイム・テキスト	*/
/*							*/
/* rzi139,rzi140					*/
/*------------------------------------------------------*/
struct fmt_stale_time {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stale_time[4];		/* ステイル・ロック・タイム     */
	BYTE	stale_mode;		/* 現在の状態                   */
	BYTE	stale_tmr[4];		/* ステイルタイマ、ドレインタイマ*/
	BYTE	stale_dcnt[2];		/* ダウンカウント値		*/
/*-------------------------------------*/
};

int	rz_scan_stale_time(struct fmt_stale_time *buf);
void *	rz_print_stale_time(struct fmt_stale_time *txt, struct rzi139t *pkt);

/*------------------------------------------------------*/
/* マルチ連動コンディションテキストフォーマット		*/
/*							*/
/* rzi045,rzi046					*/
/*------------------------------------------------------*/

typedef union {
	BYTE		c_data[32];	/* 汎用データ			*/
	BYTE		i_data[32];	/* 汎用データ			*/
	BYTE		l_data[32];	/* 汎用データ			*/
} PI450;

typedef struct {
	BYTE		data_cnt[2];	/* データバイト数		*/
	PI450		dt;		/* 汎用データ			*/
} PI451;

typedef struct {
	BYTE		disp_cnt[2];	/* データバイト数		*/
	BYTE		disp_str[128];	/* 表示データ			*/
} PI452;

typedef struct {
	BYTE		prn_cnt[2];	/* データバイト数		*/
	BYTE		prn_str[32];	/* 印字データ			*/
} PI453;

struct fmt_mlt_cond {
	BYTE	cmd;			/* command = '<'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	cond_inf[2];		/* 状態内容			*/
	BYTE	act_inf[2];		/* ＲＣＵ動作情報		*/
	BYTE	node_ptn[8];		/* error node pattern		*/
	BYTE	error_no[4];		/* エラー情報			*/
	PI451	ex_d;			/* 汎用データ			*/
	PI452	ds_d;			/* 表示データ			*/
	PI453	pr_d;			/* 印字データ			*/
/*-------------------------------------*/
};

int	rz_scan_mlt_cond( struct fmt_mlt_cond *buf);
void *	rz_print_mlt_cond( struct fmt_mlt_cond *txt, struct rzi045t *pkt);


/*------------------------------------------------------*/
/*	予約番号変更完了テキストフォーマット		*/
/*							*/
/* rzr097,rzr098					*/
/*------------------------------------------------------*/

struct fmt_prenum_comp {
	BYTE	cmd;			/* command = '+'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	rz_scan_prenum_comp( struct fmt_prenum_comp *buf);
void *	rz_print_prenum_comp( struct fmt_prenum_comp *txt, struct rzr097t *pkt);

/*------------------------------------------------------*/
/*	分散シャッターの設定テキストフォーマット	*/
/*							*/
/* rzr099,rzr100					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	on_off;			/* 分散シャッターオン・オフ	*/
	BYTE	fc_op_cnt[2];		/* 分散シャッター強制開カウント	*/
	BYTE	fc_op_time[2];		/* 分散シャッター強制開時間	*/
	BYTE	fc_cl_time[2];		/* 分散シャッター強制開時間	*/
	BYTE	mode;			/* 分散シャッターモード		*/
	BYTE	nof_sh[2];		/* 該当するDFのSH数		*/
	BYTE	min_sh_no[2];		/* 最小SH番号			*/
	BYTE	op_time[32][2];		/* 分散シャッター開時間		*/
} PR099;


struct fmt_dfsh_parm {
	BYTE	cmd;			/* command = '|'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	PR099	sh[8];			/* 分散シャッター情報		*/
/*-------------------------------------*/
};

int	rz_scan_sh_parm( struct fmt_dfsh_parm *buf);
void *	rz_print_sh_parm( struct fmt_dfsh_parm *txt, struct rzr099t *pkt);

/*------------------------------------------------------*/
/*	初期零点調整テキストフォーマット		*/
/*							*/
/* rzr133,rzr134					*/
/*------------------------------------------------------*/

struct fmt_init_zero_flag {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	init_zero_flag;		/* 初期零点フラグ		*/
	BYTE	ad_coef[4];		/* AD係数			*/
	BYTE	da_coef[4];		/* DA係数			*/
	BYTE	range[4];		/* 秤量				*/
/*-------------------------------------*/
};

int	rz_scan_init_zero_flag( struct fmt_init_zero_flag *buf);
void *	rz_print_init_zero_flag( struct fmt_init_zero_flag *txt, struct rzr133t *pkt);

/*------------------------------------------------------*/
/*	ＡＴＡ98モニター情報  テキストフォーマット	*/
/*	（拡張2）ステイル・ロックタイム・テキスト	*/
/*							*/
/* rzi239,rzi240					*/
/*------------------------------------------------------*/
struct fmt_stale98_time {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stale98_time[4];	/* ステイル・ロック・タイム     */
	BYTE	stale98_mode;		/* 現在の状態 (stale_mode)      */
	BYTE	stale98_tmr[4];		/* ステイルタイマ、ドレインタイマ*/
	BYTE	stale98_dcnt[2];	/* ダウンカウント値		*/
	BYTE	stale98_SDEnable;	/* ホストから設定されたStale Drain Enable */
	BYTE	stale98_drain_mode;	/* 現在の状態（Drain Mode)       */
	BYTE	stale98_drain_tmr[4];	/* STALE DRAIN CLEAR TIMEのｶｳﾝﾄﾀﾞｳﾝ*/
					/* ﾄﾞﾚｲﾝﾓｰﾄﾞがSTALE_DRAIN_START時有効*/
/*-------------------------------------*/
};

int	rz_scan_stale98_time(struct fmt_stale98_time *buf);
void *	rz_print_stale98_time(struct fmt_stale98_time *buf, struct rzi239t *pkt);

/*------------------------------------------------------*/
/* 拡張親ヘッドデータ構造				*/
/*							*/
/* rzr259,rzr260					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	fd_whbh;	/* WH->BH非同期供給する／しない		*/
	BYTE	phfd_on[2];	/* PH-供給信号オン時間			*/
	BYTE	ph_cv[2];	/* 親供給機への供給コンベア遅れ時間	*/
	BYTE	phcv_on[2];	/* 親投入機への供給コンベアオン時間	*/
} PR2590;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	BYTE	only_parent;		/* 親のみ計算する／しない	*/
	PR2590	phead[2];		/* P2590 paramter[nof_phead]	*/
} PR2591;

struct fmt_pandc_ext_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR2591	sect[8];		/* PR2591 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	rz_scan_pandc_ext_parm( struct fmt_pandc_ext_parm *buf);
void *	rz_print_pandc_ext_parm( struct fmt_pandc_ext_parm *txt, struct rzr259t *pkt);

/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（５）			*/
/*		テキストフォーマット			*/
/*	拡張上下限値データ				*/
/* rzr523,rzr524					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	upper_wt[8];		/* 上限重量			*/
	BYTE	lower_wt[8];		/* 下限重量			*/
	BYTE	upper_cnt[4];		/* 上限個数			*/
	BYTE	lower_cnt[4];		/* 下限個数			*/
} PR5230;

struct fmt_ext_tgt {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR5230	sect[8];		/* PR5230 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	rz_scan_ext_tgt(struct fmt_ext_tgt *buf);
void *	rz_print_ext_tgt(struct fmt_ext_tgt *txt, struct rzr523t *pkt);

/*------------------------------------------------------*/
/*	拡張テスト駆動開始テキストフォーマット		*/
/*							*/
/* rzr151,rzr152					*/
/*------------------------------------------------------*/

struct fmt_ext_tstdrv_run {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	sht_ptn[8];		/* SHUTTER pattern		*/
	BYTE	sht_flg;		/* SHUTTER ctrl flag		*/
	BYTE	spa_ptn[8];		/* spare pattern		*/
	BYTE	spa_flg;		/* ctrl flag			*/
/*-------------------------------------*/
};

int	rz_scan_ext_tstdrv_run( struct fmt_ext_tstdrv_run *buf);
void *	rz_print_ext_tstdrv_run( struct fmt_ext_tstdrv_run *txt, struct rzr151t *pkt);

/*------------------------------------------------------*/
/*	拡張テスト秤データのテキストフォーマット	*/
/*							*/
/* rzi111,rzi112					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[2];		/* ヘッドステータス		*/
} PR111;

struct fmt_ph_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_ph_no[2];		/* 最小WH番号			*/
	BYTE	nof_ph[2];		/* 該当するchのWH数		*/
	PR111	ph[32];			/* PH重量&ステータス[nof_ph]	*/
/*-------------------------------------*/
};

int	rz_scan_ph_weight( struct fmt_ph_weight *buf);
void *	rz_print_ph_weight( struct fmt_ph_weight *txt, struct rzi111t *pkt);

/*------------------------------------------------------*/
/*	秤の計量特性拡張テキストフォーマット		*/
/*							*/
/* rzr209,rzr210					*/
/*------------------------------------------------------*/

struct fmt_ph_wei_spec {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
/*-------------------------------------*/
};

int	rz_scan_ph_wei_spec( struct fmt_ph_wei_spec *buf);
void *	rz_print_ph_wei_spec( struct fmt_ph_wei_spec *txt, struct rzr209t *pkt);

/*------------------------------------------------------*/
/* 計量機の構成仕様拡張テキストフォーマット		*/
/*							*/
/* rzr103,rzr104					*/
/*------------------------------------------------------*/

struct fmt_bucket_inf {
	BYTE	prm_pnt[4];		/* pointer to option parameter		*/
/*---------- parameter ----------------*/
	BYTE	nof_wh[2];		/* WH数					*/
	BYTE	nof_bb[2];		/* 下段計量バケット数			*/
	BYTE	nof_mb[2];		/* 中段計量バケット数			*/
	BYTE	nof_wherr[2];		/* WH-不良排出位置間バケット数		*/
	BYTE	nof_whwh[2];		/* WH間バケット数			*/
	BYTE	nof_whbb[2];		/* WH-下段計量バケット間バケット数	*/
	BYTE	nof_bbwh[2];		/* 下段計量バケット-WH間バケット数	*/
/*-------------------------------------*/
};

int	rz_scan_bucket_inf( struct fmt_bucket_inf *buf);
void *	rz_print_bucket_inf( struct fmt_bucket_inf *txt, struct rzr103t *pkt);

/*------------------------------------------------------*/
/*	マルチ連動パラメータ２拡張テキストフォーマット	*/
/*							*/
/* rzr193,rzr194					*/
/*------------------------------------------------------*/

struct fmt_mlt_parm2_ext {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_mch[2];		/* 計量機数			*/
	BYTE	node_id[32][2];		/* NHICノード番			*/
/*-------------------------------------*/
};

int	rz_scan_mlt_parm2_ext(struct fmt_mlt_parm2_ext *buf);
void *	rz_print_mlt_parm2_ext(struct fmt_mlt_parm2_ext *txt, struct rzr193t *pkt);

/*------------------------------------------------------*/
/*	計量機の構成仕様拡張テキストフォーマット	*/
/*							*/
/* rzr203,rzr204					*/
/*------------------------------------------------------*/

struct fmt_af_construct {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_af[2];		/* number of AF			*/
	BYTE	af_assign_ptn[32][8];	/* AF loacate pattern
					   [nof_af][nof_head/4]		*/
/*-------------------------------------*/
};

int	rz_scan_af_construct( struct fmt_af_construct *buf);
void *	rz_print_af_construct( struct fmt_af_construct	*txt, struct rzr203t *pkt);

/*------------------------------------------------------*/
/*	供給量の設定拡張テキストフォーマット		*/
/*							*/
/* rzr419,rzr420					*/
/*------------------------------------------------------*/

struct fmt_af_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	min_af_no[2];		/* 最小AF番号			*/
	BYTE	nof_af[2];		/* 該当するchのAF数		*/
	P5	af[32];			/* P5 paramter[nof_af]		*/
/*-------------------------------------*/
};

int	rz_scan_af_parm( struct fmt_af_parm *buf);
void *	rz_print_af_parm( struct fmt_af_parm *txt, struct rzr419t *pkt);

#endif	/* RZCMD_H */

/*----------------------------------------------------------------------*/
/*        		ＥＮＤ  ＯＦ ＦＩＬＥ				*/
/*----------------------------------------------------------------------*/
