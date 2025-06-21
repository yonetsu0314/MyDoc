/*======================================================================
 * File name	: prcmd.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	コマンドサマリー
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/SHINC/PRCMD/RCS/prcmd.h 1.24 2015-05-28 10:55:50+09 itami Exp $
 *$Log: prcmd.h $
 *Revision 1.24  2015-05-28 10:55:50+09  itami
 *WHラウンディング タイミング拡張テキスト追加
 *(prr3017,prr3018)
 *
 *Revision 1.23  2014-06-26 20:51:52+09  hiroe
 *2014/06/26
 *第二のスパン構成の追加。prr1035
 *
 * リビジョン 1.22  1970/01/01  00:00:00  yonetsu
 * 2012/2/15 13:47
 * フィーダ駆動特性データ　拡張パラメータ追加
 * 
 * リビジョン 1.21  1970/01/01  00:00:00  ohtani
 * 2007/10/12 13:50:06
 * 計算結果拡張パラメータコマンドを追加する。(pri1011,pri1012)
 * 
 * リビジョン 1.20  1970/01/01  00:00:00  chimura
 * 2007/03/22
 * 4-20mA用拡張パラメータコマンドのソースと構造体の型の不一致を修正する。
 * (prr2005,prr2006)
 * 
 * リビジョン 1.19  1970/01/01  00:00:00  chimura
 * 2007/03/15 4-20mA用拡張パラメータコマンドを追加する。(prr2005,prr2006)
 * 
 * リビジョン 1.18  1970/01/01  00:00:00  itami
 * 2007/02/21 拡張パラメータコマンドを追加する。(prr0137,prr0138)
 * 
 * リビジョン 1.17  1970/01/01  00:00:00  itami
 * 2007/01/11 分散シャッター用コマンドを追加する。(prr0099)
 * 
 * リビジョン 1.16  1970/01/01  00:00:00  itami
 * 2006/10/26 特殊仕様データ用コマンドを追加する。(prr135)
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  kawamura
 * 2006/06/14  19:10
 * 供給振り分け装置ゲート位置拡張パラメータを追加する。（prr1005）
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  chimura
 * 2005/09/09
 * FSV用に状態コマンドを拡張する。（pri2013）
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  chimura
 * 2005/08/12
 * 拡張上下限値データ追加（prr5023）
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  ishidat
 * 2005/07/21 16:40
 * PLC書込みデータを追加する
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  ishidat
 * 2005/07/21 16:40
 * PLC書込みデータを追加する
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kawamura
 * 2005/07/09  10:55
 * 前回のリビジョンで、データのサイズを間違えていた不具合を修正する。
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kawamura
 * 2005/06/29  20:20
 * 補助組み合わせの参考重量データ送信を追加する。
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  itami
 * 2005/03/22 状態表示情報を追加する。
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kawamura
 * 2005/03/01  13:30
 * 特殊アベレージコントロールを追加する。
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  chimura
 * 2005/02/16 シフトデータ関係のコマンドを追加
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  chimura
 * 2005/02/14 14:00
 * マルチ連動関連のパラメータを追加する。
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kawamura
 * 2004/12/06  15:30
 * prr6023.hにＢＨ非同期供給のON/OFFを追加する。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kawamura
 * 2004/08/10  15:41
 * ＰＨセル関連及びセミオート関連のコマンドを追加する。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kawamura
 * 2004/05/08  16:00
 * 新ＡＦＤに関するコマンドを追加する。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kawamura
 * 2004/05/03  15:00
 * フリトレイATA関連のファイルを追加する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: p:/include/rzcmd/rcs/rzcmd.h 2.95 1970/01/01 00:00:00 kawamura Exp
 *log: rzcmd.h
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#ifndef	__PRCMD_H__
#define	__PRCMD_H__

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

/*#define		****_CMD	(' ')*/
#define		DCA_PRM_CMD	('!')
#define		ATA_DEV_CMD	('"')
#define		TIMER_PRM_CMD	('#')
#define		DMY_WEIGHT_CMD	('$')
#define		SHIFT_REQ_CMD	('%')
#define		SHIFT_MON_CMD	('&')
#define		PLC_WR_CMD	('\'')
#define		DCA_MON_CMD	('(')
#define		ATA_MON_CMD	(')')
#define		MLT_COND_CMD	('*')
#define		ADV_AFD_CMD	('+')
#define		PNUM_REQ_CMD	(',')
#define		MLT_P1_CMD	('-')
#define		INFO_REQ_CMD	('.')
#define		INFO_CMD	('/')
#define		LOGIC_REQ_CMD	('0')
#define		LOGIC_CMD	('1')
#define		ARC_REQ_CMD	('2')
#define		ARC_CMD		('3')
#define		RES_CYC_REQ_CMD	('4')
#define		RES_CYC_CMD	('5')
#define		RES_CYC_DET_CMD	('6')
#define		RES_CYC_STP_CMD	('7')
#define		DRV_CYC_PRM_CMD	('8')
#define		NAT_FRQ_PRM_CMD	('9')
#define		SAMPLE_DUMP_CMD	(':')
#define		RYIN_REQ_CMD	(';')
#define		MLT_EXT_CMD	('<')
#define		RXCOMP_CMD	('=')
#define		ACTCOMP_CMD	('>')
#define		SYSERR_CMD	('?')
#define		OPT_DEV_CMD	('@')
#define		SPAN_CMD	('A')
#define		SIG_ASSIGN_CMD	('B')
#define		MAN_COMBI_CMD	('C')
#define		ERREJ_CMD	('D')
#define		CBRSLT_CMD	('E')
#define		WRNG_CMD	('F')
#define		DRAIN_STRT_CMD	('G')
#define		DRAIN_STOP_CMD	('H')
#define		ERROR_CMD	('I')
#define		SPC_REQ_CMD	('J')
#define		SPAN_COE_CMD	('K')
#define		SCL_REQ_CMD	('L')
#define		MAN_CYCLE_CMD	('M')
#define		COND_CMD	('N')
#define		P_RSLT_CMD	('O')
#define		TDRV_STRT_CMD	('P')
#define		TDRV_STOP_CMD	('Q')
#define		PROD_STRT_CMD	('R')
#define		PROD_STOP_CMD	('S')
#define		TGT_PRM_CMD	('T')
#define		DIAGREQ_CMD	('U')
#define		AFVADJ_CMD	('V')
#define		PW_ON_CMD	('W')
#define		PW_OFF_CMD	('X')
#define		PNUM_CMD	('Y')
#define		ZERO_CMD	('Z')
#define		NETMAP_REQ_CMD	('[')
#define		SCALE_CMD	('\\')
#define		NETMAP_CMD	(']')
#define		MLT_P2_CMD	('^')
#define		MLT_P1REQ_CMD	('_')
/*#define		****_CMD	('`')*/
#define		WEI_SPEC_CMD	('a')
#define		BMIF_SPEC_CMD	('b')
#define		CBCAL_SPEC_CMD	('c')
#define		HOP_SPEC_CMD	('d')
#define		FD_SPEC_CMD	('e')
#define		RFDF_PRM_CMD	('f')
#define		CF_PRM_CMD	('g')
#define		AFDH_CMD	('h')
#define		PCUERR_CMD	('i')
#define		DZHOPSPEC_CMD	('j')
#define		CF_RSLT_CMD	('k')
#define		USR_LEVEL_CMD	('l')
#define		MEAN_CMD	('m')
/*#define		****_CMD	('n')*/
#define		P_TARG_CMD	('o')
#define		RNWPW_CMD	('p')
#define		RNWCOMP_CMD	('q')
#define		AFVRECALL_CMD	('r')
#define		AFDS_CMD	('s')
#define		TMG_PRM_CMD	('t')
#define		DIAGDATA_CMD	('u')
#define		AFVCOE_CMD	('v')
#define		MCH_SPEC_CMD	('w')
#define		INFD_SPEC_CMD	('x')
#define		COORD_CMD	('y')
#define		EEPROMCLR_CMD	('z')
#define		METAL_TEXT_CMD	('{')
#define		SH_CTL_CMD	('|')
#define		METAL_ANS_CMD	('}')
#define		RYIN_CMD	('~')


#define		HP_DRV_REQ_CMD	(0xa1)
#define		RYOUT_CMD	(0xa2)
#define		HP_DRV_CMD	(0xa3)
#define		HP_DRV_SET_CMD	(0xa4)
#define		FZERO_CMD	(0xa5)
#define		PRENUM_CMD	(0xa6)
#define		INFEED_CTL_CMD	(0xa7)
#define		DRAIN_ZERO_CMD	(0xa8)
#define		GOTO_BOOT_CMD	(0xa9)
#define		DSP_COND_CMD	(0xaa)
#define		SP_PARM_CMD	(0xab)
#define		EXT_PARM_CMD	(0xac)
/*#define		****_CMD	(0xad)*/
/*       		  ･     	      */
/*       		  ･     	      */
/*#define		****_CMD	(0xdf)*/


/*------------------------------------------------------*/
/*	計量機の構成仕様テキストフォーマット		*/
/*							*/
/* prr0003,prr0004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* hopper assign pattern	*/
	BYTE	drv_type;		/* drive type			*/
	BYTE	direction;		/* 正不良、２方向識別子		*/
	BYTE	level;			/* 段数(WHからの位置)		*/
	BYTE	port[2];		/* EXCポート番号		*/
} PR00030;

struct fmt_mch_construct {
	BYTE	cmd;			/* command = 'w'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	head_construct;		/* radial/linear		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	model_id;		/* model ID			*/
	BYTE	nof_df;			/* number of DF			*/
	BYTE	df_assign_ptn[8][8];	/* DF loacate pattern		*/
	BYTE	nof_af[2];		/* number of AF			*/
	BYTE	af_assign_ptn[32][8];	/* AF loacate pattern		*/
	BYTE	nof_rs[2];		/* number of RS			*/
	PR00030	rs[16];			/* PR00030 parameter of RS	*/
	BYTE	nof_dth[2];		/* number of DTH		*/
	PR00030	dth[16];		/* PR00030 parameter of DTH	*/
	BYTE	nof_th[2];		/* number of TH			*/
	PR00030	th[16];			/* PR00030 parameter of TH	*/
	BYTE	nof_duc_net;		/* number of duc net		*/
	BYTE	duc_assign_ptn[2][8];	/* DUC network pattern		*/
/*-------------------------------------*/
};

int	pr_scan_mch_construct( struct fmt_mch_construct *buf);
void *	pr_print_mch_construct( struct fmt_mch_construct *txt, struct prr0003t *pkt);


/*------------------------------------------------------*/
/*	供給機の制御仕様テキストフォーマット		*/
/*							*/
/* prr0005,prr0006					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	sig_assign_ptn[8];	/* infeed signal assign pattern	*/
	BYTE	infd_detector;		/* level sensor type		*/
	BYTE	transient_int[2];	/* transient interrupt time	*/
	BYTE	feed_intrpt[2];		/* low product time		*/
	BYTE	targ[4];		/* 目標重量			*/
	BYTE	upper[2];		/* 上限偏差(%)			*/
	BYTE	lower[2];		/* 下限偏差(%)			*/
} PR00050;

struct fmt_infd_spec {
	BYTE	cmd;			/* command = 'x'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sig;		/* number of infeed signal	*/
	PR00050	sig[8];			/* PR00050 paramter[nof_sig]	*/
	BYTE	nof_cf;			/* number of CF			*/
	BYTE	cf_assign_ptn[8][8];	/* CF assign pattern		*/
	BYTE	pconst[4];		/* proportion constant		*/
	BYTE	iconst[4];		/* integral constant		*/
	BYTE	dconst[4];		/* differrential constant	*/
	BYTE	filter_sw[4];		/* filter switch		*/
	BYTE	filter_const[4];	/* filter constant		*/
/*-------------------------------------*/
};

int	pr_scan_infd_spec( struct fmt_infd_spec *buf);
void *	pr_print_infd_spec( struct fmt_infd_spec *txt, struct prr0005t *pkt);


/*------------------------------------------------------*/
/*	包装機との連動仕様テキストフォーマット		*/
/*							*/
/* prr0007,prr0008					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	bm_assign_ptn[8];	/* bag maker assign pattern	*/
	BYTE	interface;		/* I/F type			*/
	BYTE	mul_dump_ini;		/* multi dump initiate mode	*/
	BYTE	mul_dump_conf;		/* multi dump confirm mode	*/
	BYTE	manu_dump_ini;		/* manual dump initiate mode	*/
	BYTE	manu_dump_conf;		/* manual dump confirm mode	*/
} PR00070;

struct fmt_bmif_spec {
	BYTE	cmd;			/* command = 'b'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_bm;			/* number of bag maker		*/
	PR00070	ch[4];			/* PR00070 paramter[nof_bm]	*/
/*-------------------------------------*/
};

int	pr_scan_bmif_spec( struct fmt_bmif_spec *buf);
void *	pr_print_bmif_spec( struct fmt_bmif_spec *txt, struct prr0007t *pkt);


/*------------------------------------------------------*/
/*	秤の計量特性テキストフォーマット		*/
/*							*/
/* prr0009,prr0010					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	thin[4];		/* 間引き数			*/
	BYTE	dig[4];			/* 有効桁増加数			*/
	BYTE	tap[4];			/* タップ数			*/
	BYTE	coef[4];		/* フィルタ係数			*/
} PR00093;

typedef struct {
	BYTE	stable_count[2];	/* stable count			*/
	BYTE	zero_limit_cnt[4];	/* auto zero error count	*/
	BYTE	stable_cmp_cnt;		/* compare count		*/
	BYTE	range;			/* weigh range			*/
	BYTE	smpl_interval[2];	/* sampling interval		*/
	BYTE	iir_reset_cnt[2];	/* IIR filter reset count	*/
	BYTE	afv_mode;		/* AFV mode (off/auto/on)	*/
	BYTE	filter;			/* filter number		*/
	BYTE	span_wt;		/* スパン分銅			*/
	BYTE	cell_type;		/* cell type			*/
	BYTE	nof_stage;		/* フィルタステージ数		*/
	PR00093	dadfip[6];		/* フィルタパラメータ		*/
	BYTE	afv_wh_limit[4];	/* AFV調整WH限界値		*/
	BYTE	afv_afv_limit[4];	/* AFV調整AFV限界値		*/
	BYTE	afv_onoff_limit[4];	/* AFV補正ON/OFF限界値		*/
} PR00090;

typedef struct {
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	stick_weight[4];	/* stick weight			*/
} PR00091;

typedef struct {
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR00091	sect[8];		/* PR00091 paramter[nof_sect]	*/
} PR00092;

struct fmt_wei_spec {
	BYTE	cmd;			/* command = 'a'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_adc;		/* number of ADC board		*/
	PR00090	adc[4];			/* PR00090 paramter[nof_adc]	*/
	BYTE	nof_ch;			/* number of channel		*/
	PR00092	ch[4];			/* PR00092 paramter[nof_ch]	*/
};

int	pr_scan_wei_spec( struct fmt_wei_spec *buf);
void *	pr_print_wei_spec( struct fmt_wei_spec *txt, struct prr0009t *pkt);


/*------------------------------------------------------*/
/*	組み合わせ計算の特性テキストフォーマット	*/
/*							*/
/* prr0011,prr0012					*/
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
	BYTE	stable;			/* stable head process control	*/
	BYTE	re_check;		/* re check process control	*/
	BYTE	th_error;		/* TH process control		*/
	BYTE	nof_stop_head[2];	/* number of stop head		*/
	BYTE	nof_stop_cycle[2];	/* number of stop cycle		*/
	BYTE	err_rej_type;		/* error reject process control	*/
	BYTE	multi_cal;		/* multi cal. process control	*/
	BYTE	approval_flag;		/* approval flag		*/
	BYTE	max_weight[4];		/* max. weight of approval	*/
	BYTE	min_weight[4];		/* min. weight of approval	*/
/*-------------------------------------*/
};

int	pr_scan_cbcal_spec( struct fmt_cbcal_spec *buf);
void *	pr_print_cbcal_spec( struct fmt_cbcal_spec *txt, struct prr0011t *pkt);


/*------------------------------------------------------*/
/*	ホッパの開閉特性のテキストフォーマット		*/
/*							*/
/* prr0013,prr0014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	pulse_mltp[2];		/* 基本パルス倍数		*/
	BYTE	repeat[2];		/* 繰り返し数			*/
} PR00130;

struct fmt_hop_spec {
	BYTE	cmd;			/* command = 'd'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	actuator_type;		/* アクチュエータタイプ		*/
	BYTE	phase_type;		/* 励磁モード	3:1-2相		*/
	BYTE	stop_mode;		/* 停止条件			*/
	BYTE	speed[4];		/* 回転数 [単位パルス時間0.4us] */
	BYTE	ignore_pulse[4];	/* スリット不感パルス数		*/
	BYTE	cam_slit[2];		/* スリット停止パルス数		*/
	BYTE	br_cl_delay[2];		/* BR-CL動作遅れパルス数	*/
	BYTE	cl_br_delay[2];		/* CL-BR動作遅れパルス数	*/
	BYTE	brake_time[2];		/* ブレーキ時間[10msec]		*/
	BYTE	error_pulse[2];		/* エラー検出パルス数		*/
	BYTE	nof_pattern[2];		/* パターン数			*/
	PR00130	ary[128];		/* PR00130 paramter[nof_pattern]*/
/*-------------------------------------*/
};

int	pr_scan_hop_spec( struct fmt_hop_spec *buf);
void *	pr_print_hop_spec( struct fmt_hop_spec *txt, struct prr0013t *pkt);


/*------------------------------------------------------*/
/*	フィーダの駆動特性のテキストフォーマット	*/
/*							*/
/* prr0015,prr0016					*/
/*------------------------------------------------------*/

struct fmt_fd_spec {
	BYTE	cmd;			/* command = 'e'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	fd_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	feeder_id;		/* feeder id			*/
	BYTE	min_amp;		/* minimun aplitude		*/
	BYTE	max_amp;		/* maximun aplitude		*/
	BYTE	multiplier;		/* フィーダ時間倍率		*/
	BYTE	phase_type;		/* 位相				*/
	BYTE	opt_sw_ctrl;		/* 光電子SWによる停止モード	*/
	BYTE	shutter_ctrl;		/* シャッタ連動モード		*/
	BYTE	shutter_off_delay[2];	/* シャッタ停止遅れ時間		*/
	BYTE	slow_start;		/* スロースタート		*/
	BYTE	stop_ptn;		/* ストップパターン生成		*/
/*-------------------------------------*/
};

int	pr_scan_fd_spec( struct fmt_fd_spec *buf);
void *	pr_print_fd_spec( struct fmt_fd_spec *txt, struct prr0015t *pkt);


/*------------------------------------------------------*/
/*	タイミングの設定テキストフォーマット		*/
/*							*/
/* prr0017,prr0018					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	delay[2];		/* RS遅れ時間[nof_rs]		*/
	BYTE	pool[2];		/* RSプール時間			*/
} PR00170;

typedef struct {
	BYTE	delay[2];		/* 遅れ時間[nof_rs]		*/
	BYTE	on[2];			/* オン時間			*/
} PR00171;

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern	*/
	BYTE	wh_ph[2];		/* WH-PH遅れ時間		*/
	BYTE	ph_rf[2];		/* PH-RF遅れ時間		*/
	BYTE	wh1_bh[2];		/* WH1-MH遅れ時間		*/
	BYTE	bh_wh2[2];		/* MH-WH2遅れ時間		*/
	BYTE	stagger[2];		/* セクション内時差排出時間	*/
	BYTE	sect_stagger[2];	/* セクション間時差排出時間	*/
	BYTE	bh_on[2];		/* BHオン時間			*/
	BYTE	wh_on[2];		/* WHオン時間			*/
	BYTE	ph_on[2];		/* PHオン時間			*/
} PR00172;

struct fmt_tmg_parm {
	BYTE	cmd;			/* command = 't'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	ds_delay[2];		/* dump confirm signal delay	*/
	BYTE	ds_hold_tm[2];		/* dump confirm signal hold time*/
	BYTE	op1_delay[2];		/* option signal 1 delay	*/
	BYTE	op1_hold_tm[2];		/* option signal 1 hold time	*/
	BYTE	op2_delay[2];		/* option signal 2 delay	*/
	BYTE	op2_hold_tm[2];		/* option signal 2 hold time	*/
	BYTE	wh_delay[2];		/* WH遅れ時間			*/
	BYTE	nof_round;		/* ラウンディンググループ数	*/
	BYTE	round_base[8][2];	/* ラウンディング排出基本時間	*/
	BYTE	min_th_no[2];		/* 最小TH番号			*/
	BYTE	nof_th[2];		/* 該当するchのTH数		*/
	PR00171	th[16];			/* TH遅れ時間[nof_th]		*/
	BYTE	min_dth_no[2];		/* 最小DTH番号			*/
	BYTE	nof_dth[2];		/* 該当するchのDTH数		*/
	PR00171	dth[16];		/* DTH遅れ時間[nof_dth]		*/
	BYTE	min_rs_no[2];		/* 最小RS番号			*/
	BYTE	nof_rs[2];		/* 該当するchのRS数		*/
	PR00170	rs[16];			/* PR00170 paramter[nof_rs]	*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR00172	sect[8];		/* PR00172 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_tmg_parm( struct fmt_tmg_parm *buf);
void *	pr_print_tmg_parm( struct fmt_tmg_parm *txt, struct prr0017t *pkt);


/*------------------------------------------------------*/
/*	供給量の設定テキストフォーマット		*/
/*							*/
/* prr0019,prr0020					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	amp[2];			/* フィーダ振幅強度		*/
	BYTE	time[2];		/* フィーダ振幅時間		*/
} PR00190;

struct fmt_rfdf_parm {
	BYTE	cmd;			/* command = 'f'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	backup;			/* backup			*/
	BYTE	nof_rf[2];		/* 該当するchのRF数		*/
	PR00190	rf[32];			/* PR00190 paramter[nof_rf]	*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	PR00190	df[8];			/* PR00190 paramter[nof_df]	*/
	BYTE	nof_af[2];		/* 該当するchのAF数		*/
	PR00190	af[32];			/* PR00190 paramter[nof_af]	*/
/*-------------------------------------*/
};

int	pr_scan_rfdf_parm( struct fmt_rfdf_parm *buf);
void *	pr_print_rfdf_parm( struct fmt_rfdf_parm *txt, struct prr0019t *pkt);


/*------------------------------------------------------*/
/*	CFの振幅設定テキストフォーマット		*/
/*							*/
/* prr0021,prr0022					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* 供給状態数			*/
	PR00190	at[8];			/* PR00190 paramter[nof_sig]	*/
} PR00210;

struct fmt_cf_parm {
	BYTE	cmd;			/* command = 'g'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ctrl;			/* control			*/
	BYTE	nof_cf;			/* CF数				*/
	PR00210	cf[8];			/* PR00210 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	pr_scan_cf_parm( struct fmt_cf_parm *buf);
void *	pr_print_cf_parm( struct fmt_cf_parm *txt, struct prr0021t *pkt);


/*------------------------------------------------------*/
/*	ＡＦＤセクション情報のテキストフォーマット	*/
/*							*/
/* prr0057,prr0058					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	ctrl_mode[2];		/* AFDモード			*/
	BYTE	ctrl_sw[2];		/* AFD動作ＳＷ			*/
	BYTE	amp_max[2];		/* 振幅強度絶対上限		*/
	BYTE	amp_min[2];		/* 振幅強度絶対下限		*/
	BYTE	time_max[2];		/* 振幅時間絶対上限		*/
	BYTE	time_min[2];		/* 振幅時間絶対下限		*/
} PR00570;

typedef struct {
	BYTE	amp_max[2];		/* 振幅強度絶対上限		*/
	BYTE	amp_min[2];		/* 振幅強度絶対下限		*/
	BYTE	time_max[2];		/* 振幅時間絶対上限		*/
	BYTE	time_min[2];		/* 振幅時間絶対下限		*/
} PR00571;

struct fmt_afd_sect {
	BYTE	cmd;			/* command = 's'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR00570	sect[8];		/* PR00570 paramter[nof_sect]	*/
	BYTE	nof_df;			/* 計量機のDF数			*/
	PR00571	df[8];			/* PR00571 paramter[nof_df]	*/
	BYTE	nof_af[2];		/* 計量機のAF数			*/
	PR00571	af[32];			/* PR00571 paramter[nof_df]	*/
/*-------------------------------------*/
};

int	pr_scan_afd_sect( struct fmt_afd_sect *buf);
void *	pr_print_afd_sect( struct fmt_afd_sect *txt, struct prr0057t *pkt);


/*------------------------------------------------------*/
/*	計量目標値の設定テキストフォーマット		*/
/*							*/
/* prr0023,prr0024					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	assign_ptn[8];		/* Section assign pattern	*/
	BYTE	target_wt[8];		/* 計量目標重量			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	upper_wt2[4];		/* 第２上限重量			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* 計量目標個数			*/
	BYTE	upper_cnt[2];		/* 上限個数			*/
	BYTE	upper_cnt2[2];		/* 第２上限個数			*/
	BYTE	lower_cnt[2];		/* 下限個数			*/
	BYTE	piece_wt[4];		/* 単重				*/
	BYTE	target_head[2];		/* 目標選択ヘッド数		*/
	BYTE	pw_ctrl;		/* 単重自動更新モード		*/
	BYTE	mix_ctrl;		/* 混合補正モード		*/
	BYTE	cnt[2];			/* 優先排出回数			*/
} PR00230;

struct fmt_wei_parm {
	BYTE	cmd;			/* command = 'T'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR00230	sect[8];		/* PR00230 paramter[nof_sect]	*/
	BYTE	target_wt[8];		/* 計量目標重量			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	upper_wt2[4];		/* 第２上限重量			*/
	BYTE	tne[4];			/* T.N.E			*/
	BYTE	target_cnt[4];		/* 計量目標個数			*/
	BYTE	upper_cnt[2];		/* 上限個数			*/
	BYTE	upper_cnt2[2];		/* 第２上限個数			*/
	BYTE	lower_cnt[2];		/* 下限個数			*/
	BYTE	speed[4];		/* 計量能力			*/
	BYTE	dump_cnt[2];		/* 排出回数			*/
	BYTE	avg_ctrl;		/* 平均値制御モード		*/
	BYTE	second_upper_cycle[4];	/* 第２上限排出サイクル		*/
	BYTE	cyc_of_under[2];	/* 計量発生頻度			*/
/*-------------------------------------*/
};

int	pr_scan_wei_parm( struct fmt_wei_parm *buf);
void *	pr_print_wei_parm( struct fmt_wei_parm *txt, struct prr0023t *pkt);


/*------------------------------------------------------*/
/*	親ヘッド計量目標値の設定テキストフォーマット	*/
/*							*/
/* prr0059,prr0060					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	no[2];			/* ヘッド番号			*/
	BYTE	upper_wt[4];		/* 上限重量			*/
	BYTE	lower_wt[4];		/* 下限重量			*/
	BYTE	rf_ctrl;		/* ＲＦ駆動			*/
	BYTE	ph_fd[2];		/* PH-供給信号遅れ時間		*/
} PR00590;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR00590	phead[2];		/* PR00590 paramter[nof_phead]	*/
	BYTE	on_off;			/* 親子計量する／しない		*/
	BYTE	dump;			/* 同時排出／交互排出		*/
	BYTE	calc;			/* 計算参加／不参加		*/
	BYTE	az_intvl[4];		/* 自動零点調整間隔		*/
	BYTE	over;			/* 上限以上時の処理		*/
	BYTE	under;			/* 下限未満時の処理		*/
} PR00591;

struct fmt_pandc_parm {
	BYTE	cmd;			/* command = 'o'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR00591	sect[8];		/* PR00591 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	pr_scan_pandc_parm( struct fmt_pandc_parm *buf);
void *	pr_print_pandc_parm( struct fmt_pandc_parm *txt, struct prr0059t *pkt);


/*------------------------------------------------------*/
/*	プログラム番号の要求テキストフォーマット	*/
/*							*/
/* prr0025,prr0026					*/
/*------------------------------------------------------*/

struct fmt_req_prgno {
	BYTE	cmd;			/* command = ','		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no;			/* デバイス番号			*/
	BYTE	node_id[2];		/* ノード番号			*/
/*-------------------------------------*/
};

int	pr_scan_req_prgno( struct fmt_req_prgno *buf);
void *	pr_print_req_prgno( struct fmt_req_prgno *txt, struct prr0025t *pkt);


/*------------------------------------------------------*/
/*	秤データの要求テキストフォーマット		*/
/*							*/
/* prr0027,prr0028					*/
/*------------------------------------------------------*/

struct fmt_req_weidat {
	BYTE	cmd;			/* command = 'L'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_req_weidat( struct fmt_req_weidat *buf);
void *	pr_print_req_weidat( struct fmt_req_weidat *txt, struct prr0027t *pkt);


/*------------------------------------------------------*/
/*	駆動系電源ONテキストフォーマット		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_power_on {
	BYTE	cmd;			/* command = 'W'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};

/*------------------------------------------------------*/
/*	駆動系電源OFFテキストフォーマット		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_power_off {
	BYTE	cmd;			/* command = 'X'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};

/*------------------------------------------------------*/
/*	排出零点調整テキストフォーマット		*/
/*							*/
/* prr0033,prr0034					*/
/*------------------------------------------------------*/

struct fmt_zero_adj {
	BYTE	cmd;			/* command = 'Z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* 調整ヘッドビットパターン	*/
	BYTE	adj_df_ptn[2];		/* 調整DFビットパターン		*/
	BYTE	init_zero_flag;		/* 初期零点フラグ		*/
	BYTE	ad_coef[4];		/* AD係数			*/
	BYTE	da_coef[4];		/* DA係数			*/
	BYTE	range[4];		/* 秤量				*/
/*-------------------------------------*/
};

int	pr_scan_zero_adj( struct fmt_zero_adj *buf);
void *	pr_print_zero_adj( struct fmt_zero_adj *txt, struct prr0033t *pkt);


/*------------------------------------------------------*/
/*	スパン校正テキストフォーマット			*/
/*							*/
/* prr0035,prr0036					*/
/*------------------------------------------------------*/

struct fmt_span_adj {
	BYTE	cmd;			/* command = 'A'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	adj_head_ptn[8];	/* 調整ヘッドビットパターン	*/
	BYTE	date[8];		/* 調整日付			*/
	BYTE	time[8];		/* 調整時間			*/
/*-------------------------------------*/
};

int	pr_scan_span_adj( struct fmt_span_adj *buf);
void *	pr_print_span_adj( struct fmt_span_adj *txt, struct prr0035t *pkt);


/*------------------------------------------------------*/
/*	運転開始テキストフォーマット			*/
/*							*/
/* prr0037,prr0038					*/
/*------------------------------------------------------*/

struct fmt_prod_run {
	BYTE	cmd;			/* command = 'R'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_prod_run( struct fmt_prod_run *buf);
void *	pr_print_prod_run( struct fmt_prod_run *txt, struct prr0037t *pkt);


/*------------------------------------------------------*/
/*	運転停止テキストフォーマット			*/
/*							*/
/* prr0039,prr0040					*/
/*------------------------------------------------------*/

struct fmt_prod_stop {
	BYTE	cmd;			/* command = 'S'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_prod_stop( struct fmt_prod_stop *buf);
void *	pr_print_prod_stop( struct fmt_prod_stop *txt, struct prr0039t *pkt);


/*------------------------------------------------------*/
/*	全排出開始テキストフォーマット			*/
/*							*/
/* prr0041,prr0042					*/
/*------------------------------------------------------*/

struct fmt_drain_run {
	BYTE	cmd;			/* command = 'G'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	head_ptn[8];		/* 排出ヘッドビットパターン	*/
/*-------------------------------------*/
};

int	pr_scan_drain_run( struct fmt_drain_run *buf);
void *	pr_print_drain_run( struct fmt_drain_run *txt, struct prr0041t *pkt);


/*------------------------------------------------------*/
/*	全排出停止テキストフォーマット			*/
/*							*/
/* prr0043,prr0044					*/
/*------------------------------------------------------*/

struct fmt_drain_stop {
	BYTE	cmd;			/* command = 'H'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_drain_stop( struct fmt_drain_stop *buf);
void *	pr_print_drain_stop( struct fmt_drain_stop *txt, struct prr0043t *pkt);


/*------------------------------------------------------*/
/*	テスト組み合わせ計算テキストフォーマット	*/
/*							*/
/* prr0045,prr0046					*/
/*------------------------------------------------------*/

struct fmt_test_calc {
	BYTE	cmd;			/* command = 'C'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_test_calc( struct fmt_test_calc *buf);
void *	pr_print_test_calc( struct fmt_test_calc *txt, struct prr0045t *pkt);


/*------------------------------------------------------*/
/*	テスト組み合わせ計算&排出テキストフォーマット	*/
/*							*/
/* prr0047,prr0048					*/
/*------------------------------------------------------*/

struct fmt_test_caldmp {
	BYTE	cmd;			/* command = 'M'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_test_caldmp( struct fmt_test_caldmp *buf);
void *	pr_print_test_caldmp( struct fmt_test_caldmp *txt, struct prr0047t *pkt);


/*------------------------------------------------------*/
/*	エラー解除テキストフォーマット			*/
/*							*/
/* prr0049,prr0050					*/
/*------------------------------------------------------*/

struct fmt_error_cancel {
	BYTE	cmd;			/* command = 'D'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_error_cancel( struct fmt_error_cancel *buf);
void *	pr_print_error_cancel( struct fmt_error_cancel *txt, struct prr0049t *pkt);


/*------------------------------------------------------*/
/*	テスト駆動開始テキストフォーマット		*/
/*							*/
/* prr0051,prr0052					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_run {
	BYTE	cmd;			/* command = 'P'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	cf_ptn[2];		/* CF pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	rf_ptn[8];		/* RF pattern			*/
	BYTE	sht_ptn[8];		/* SHUTTER pattern		*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	wh2_ptn[8];		/* WH2 pattern(BH side)		*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	spa_ptn[8];		/* spare pattern		*/
	BYTE	drv_cyc;		/* Drive cycle			*/
	BYTE	sht_flg;		/* SHUTTER control flag		*/
	BYTE	spa_flg;		/* control flag			*/
/*-------------------------------------*/
};

int	pr_scan_tstdrv_run( struct fmt_tstdrv_run *buf);
void *	pr_print_tstdrv_run( struct fmt_tstdrv_run *txt, struct prr0051t *pkt);


/*------------------------------------------------------*/
/*	テスト駆動停止テキストフォーマット		*/
/*							*/
/* prr0053,prr0054					*/
/*------------------------------------------------------*/

struct fmt_tstdrv_stop {
	BYTE	cmd;			/* command = 'Q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_tstdrv_stop( struct fmt_tstdrv_stop *buf);
void *	pr_print_tstdrv_stop( struct fmt_tstdrv_stop *txt, struct prr0053t *pkt);


/*------------------------------------------------------*/
/*	ＥＥＰＲＯＭゼロクリアテキストフォーマット	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_eeprom_clr {
	BYTE	cmd;			/* command = 'z'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*-------------------------------------*/
};


/*------------------------------------------------------*/
/*	組み合わせ計算結果テキストフォーマット		*/
/*							*/
/* pri0003,pri0004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	flag[2];		/* 演算結果フラグ		*/
	BYTE	weight[8];		/* 組み合わせ重量		*/
	BYTE	count[4];		/* 組み合わせ個数		*/
} PI00030;

struct fmt_cb_result {
	BYTE	cmd;			/* command = 'E'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PI00030	sect[8];		/* PI00030 paramter[nof_sect]	*/
	PI00030	ch;			/* PI00030   "			*/
	BYTE	multi_dump_seq_no[2];	/* 回数計量カウント		*/
	BYTE	slv_targ_wt[8];		/* スレーブ機目標重量		*/
	BYTE	slv_targ_cnt[4];	/* スレーブ機目標個数		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	BYTE	wh_status[32][3];	/* WHステータス[nof_wh]		*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	BYTE	bh_status[32][3];	/* WHステータス[nof_bh]		*/
/*-------------------------------------*/
};

int	pr_scan_cb_result( struct fmt_cb_result *buf);
void *	pr_print_cb_result( struct fmt_cb_result *txt, struct pri0003t *pkt);


/*------------------------------------------------------*/
/*	親ヘッド計算結果テキストフォーマット		*/
/*							*/
/* pri0029,pri0030					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* 重量				*/
	BYTE	wh_status[3];		/* WHステータス			*/
	BYTE	bh_status[3];		/* BHステータス			*/
} PI00290;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PI00290	phead[2];		/* PI00290 paramter[nof_phead]	*/
} PI00291;

struct fmt_pandc_result {
	BYTE	cmd;			/* command = 'O'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PI00291	sect[8];		/* PI00291 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_pandc_result( struct fmt_pandc_result *buf);
void *	pr_print_pandc_result( struct fmt_pandc_result *txt, struct pri0029t *pkt);


/*------------------------------------------------------*/
/*	単重自動更新結果テキストフォーマット		*/
/*							*/
/* pri0005,pri0006					*/
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

int	pr_scan_piece_weight( struct fmt_piece_weight *buf);
void *	pr_print_piece_weight( struct fmt_piece_weight *txt, struct pri0005t *pkt);


/*------------------------------------------------------*/
/*	補正値自動更新結果テキストフォーマット		*/
/*							*/
/* pri0007,pri0008					*/
/*------------------------------------------------------*/

struct fmt_cmpn_value {
	BYTE	cmd;			/* command = 'q'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	cmpn_val[2];		/* 補正値			*/
/*-------------------------------------*/
};

int	pr_scan_cmpn_value( struct fmt_cmpn_value *buf);
void *	pr_print_cmpn_value( struct fmt_cmpn_value *txt, struct pri0007t *pkt);


/*------------------------------------------------------*/
/*	CF制御結果テキストフォーマット			*/
/*							*/
/* pri0031,pri0032					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	nof_sig;		/* 供給状態数			*/
	PR00190	at[8];			/* paramter[nof_sig]		*/
	BYTE	sig_stat[2];		/* 供給状態			*/
	BYTE	df_wt[4];		/* 合成分散重量			*/
} PI00310;

struct fmt_cf_rslt {
	BYTE	cmd;			/* command = 'k'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	seq_no;			/* シーケンス番号		*/
	BYTE	nof_cf;			/* CF数				*/
	PI00310	cf[8];			/* PI00310 paramter[nof_cf]	*/
/*-------------------------------------*/
};

int	pr_scan_cf_rslt( struct fmt_cf_rslt *buf);
void *	pr_print_cf_rslt( struct fmt_cf_rslt *txt, struct pri0031t *pkt);


/*------------------------------------------------------*/
/*	プログラム番号テキストフォーマット		*/
/*							*/
/* pri0009,pri0010					*/
/*------------------------------------------------------*/

struct fmt_prgno {
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

int	pr_scan_prgno( struct fmt_prgno *buf);
void *	pr_print_prgno( struct fmt_prgno *txt, struct pri0009t *pkt);


/*------------------------------------------------------*/
/*	秤データのテキストフォーマット			*/
/*							*/
/* pri0011,pri0012					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[3];		/* ヘッドステータス		*/
} PI00110;

struct fmt_head_weight {
	BYTE	cmd;			/* command = '\'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	flag;			/* real data or cal. data	*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	PI00110	wh[32];			/* WH重量&ステータス[nof_wh]	*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	PI00110	bh[32];			/* WH重量&ステータス[nof_bh]	*/
	BYTE	nof_ph[2];		/* 該当するchのPH数		*/
	PI00110	ph[32];			/* WH重量&ステータス[nof_ph]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	PI00110	df[8];			/* WH重量&ステータス[nof_df]	*/
/*-------------------------------------*/
};

int	pr_scan_head_weight( struct fmt_head_weight *buf);
void *	pr_print_head_weight( struct fmt_head_weight *txt, struct pri0011t *pkt);


/*------------------------------------------------------*/
/*	本体の状態テキストフォーマット			*/
/*							*/
/* pri0013,pri0014					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	condition;		/* 状態				*/
	BYTE	power_stat;		/* 駆動系電源状態		*/
	BYTE	infeed_stat;		/* 供給状態			*/
} PI00130;

struct fmt_condition {
	BYTE	cmd;			/* command = 'N'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	PI00130	ch[4];			/* PI00130 parameter of channel	*/
	BYTE	df_infeed_stat[2];	/* 分散フィーダ供給状態		*/
/*-------------------------------------*/
};

int	pr_scan_condition( struct fmt_condition *buf);
void *	pr_print_condition( struct fmt_condition *txt, struct pri0013t *pkt);


/*------------------------------------------------------*/
/*	警報情報のテキストフォーマット			*/
/*							*/
/* pri0015,pri0016					*/
/*------------------------------------------------------*/

struct fmt_alarm {
	BYTE	cmd;			/* command = 'F'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	alarm[2];		/* 警報情報			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	afd[2];			/* AFD warning id		*/
/*-------------------------------------*/
};

int	pr_scan_alarm( struct fmt_alarm *buf);
void *	pr_print_alarm( struct fmt_alarm *txt, struct pri0015t *pkt);


/*------------------------------------------------------*/
/*	異常情報のテキストフォーマット			*/
/*							*/
/* pri0017,pri0018					*/
/*------------------------------------------------------*/

struct fmt_error {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	error[2];		/* 異常情報			*/
	BYTE	ph_ptn[8];		/* PH pattern			*/
	BYTE	wh_ptn[8];		/* WH pattern			*/
	BYTE	bh_ptn[8];		/* BH pattern			*/
	BYTE	rs_ptn[4];		/* RS pattern			*/
	BYTE	dth_ptn[4];		/* DTH pattern			*/
	BYTE	th_ptn[4];		/* TH pattern			*/
	BYTE	df_ptn[2];		/* DF pattern			*/
	BYTE	af_ptn[8];		/* AF pattern			*/
	BYTE	duc_inf;		/* DUC error information	*/
	BYTE	exc_inf;		/* EXC error information	*/
/*-------------------------------------*/
};

int	pr_scan_error( struct fmt_error *buf);
void *	pr_print_error( struct fmt_error *txt, struct pri0017t *pkt);


/*------------------------------------------------------*/
/*	受信完了テキストフォーマット			*/
/*							*/
/* pri0019,pri0020					*/
/*------------------------------------------------------*/

struct fmt_echo {
	BYTE	cmd;			/* command = '='		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* 受信コマンド			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	pr_scan_echo( struct fmt_echo *buf);
void *	pr_print_echo( struct fmt_echo *txt, struct pri0019t *pkt);


/*------------------------------------------------------*/
/*	動作完了テキストフォーマット			*/
/*							*/
/* pri0021,pri0022					*/
/*------------------------------------------------------*/

struct fmt_complete {
	BYTE	cmd;			/* command = '>'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	command;		/* 受信コマンド			*/
	BYTE	ch_id;			/* c1c2				*/
/*-------------------------------------*/
};

int	pr_scan_complete( struct fmt_complete *buf);
void *	pr_print_complete( struct fmt_complete *txt, struct pri0021t *pkt);


/*------------------------------------------------------*/
/*	システムエラーテキストフォーマット		*/
/*							*/
/* pri0023,pri0024					*/
/*------------------------------------------------------*/

struct fmt_sys_fault {
	BYTE	cmd;			/* command = '?'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	len[2];			/* メッセージのバイト長		*/
	BYTE	mes_str[64];		/* メッセージ・ストリングス	*/
/*-------------------------------------*/
};

int	pr_scan_sys_fault( struct fmt_sys_fault *buf);
void *	pr_print_sys_fault( struct fmt_sys_fault *txt, struct pri0023t *pkt);


/*------------------------------------------------------*/
/*	周辺異常情報のテキストフォーマット		*/
/*							*/
/* pri0027,pri0028					*/
/*------------------------------------------------------*/

struct fmt_pcuerr {
	BYTE	cmd;			/* command = 'I'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	error;			/* 異常情報			*/
	BYTE	ptn[8];			/* pattern			*/
/*-------------------------------------*/
};

int	pr_scan_pcuerr( struct fmt_pcuerr *buf);
void *	pr_print_pcuerr( struct fmt_pcuerr *txt, struct pri0027t *pkt);


/*------------------------------------------------------*/
/*	ネットワーク参加状態の要求テキストフォーマット	*/
/*							*/
/* prr0071,prr0072					*/
/*------------------------------------------------------*/

struct fmt_req_netmap {
	BYTE	cmd;			/* command = '['		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no[2];		/* デバイス番号			*/
/*-------------------------------------*/
};

int	pr_scan_req_netmap( struct fmt_req_netmap *buf);
void *	pr_print_req_netmap( struct fmt_req_netmap *txt, struct prr0071t *pkt);


/*------------------------------------------------------*/
/*	ネットワーク参加状況テキストフォーマット	*/
/*							*/
/* pri0035,pri0036					*/
/*------------------------------------------------------*/

struct fmt_netmap {
	BYTE	cmd;			/* command = ']'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_id;			/* ボードID番号			*/
	BYTE	dev_no[2];		/* デバイス番号			*/
	BYTE	net_map[32][2];		/* 参加状態テーブル		*/
	BYTE	recon[4];		/* リコンフィグ発生回数		*/
/*-------------------------------------*/
};

int	pr_scan_netmap( struct fmt_netmap *buf);
void *	pr_print_netmap( struct fmt_netmap *txt, struct pri0035t *pkt);


/*------------------------------------------------------*/
/*	ホッパの開閉特性のテキストフォーマット		*/
/*							*/
/* prr0029,prr0030					*/
/*------------------------------------------------------*/

typedef struct {			/*----- mdp_section_data -------*/
	BYTE	amp_n[4];		/* 最終開閉位置(回転区間)[%]	*/
	BYTE	sec_flag_power[4];	/* ｾｸｼｮﾝのﾊｰﾌﾊﾟﾜｰ設定		*/
	BYTE	time_hold[4];		/* ﾎｰﾙﾄﾞ時間[ms]		*/
	BYTE	num_ss_start[4];	/* 始動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾃｯﾌﾟ数)	*/
	BYTE	num_ss_stop[4];		/* 制動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾃｯﾌﾟ数)	*/
	BYTE	max_pps[4];		/* 速度[pps]			*/
} PR00290;

typedef struct {			/* actuator_type == ACT_SM	*/
	BYTE	phase_type;		/* 励磁モード	3:1-2相		*/
	BYTE	full_pow_tm[4];		/* 低速時フルパワー時間(4000)	*/
	BYTE	ignore_pulse[4];	/* スリット不感パルス数		*/
	BYTE	cam_slit[2];		/* スリット停止パルス数		*/
	BYTE	hold_mode;		/* 停止励磁条件			*/
	BYTE	brake_time[2];		/* ブレーキ時間[10msec]		*/
	BYTE	error_pulse[2];		/* エラー検出パルス数		*/
	BYTE	num_section[2];		/* 駆動ﾊﾟﾀｰﾝ構成数(ｾｸｼｮﾝ数）	*/
	BYTE	mode_flags[2];		/* 駆動ﾊﾟﾀｰﾝﾓｰﾄﾞﾌﾗｸﾞ		*/
	BYTE	start_pps[4];		/* 自起動単位ﾊﾟﾙｽ数[pps]	*/
	BYTE	fullstep[4];		/* 全ｽﾃｯﾌﾟ数			*/
	BYTE	pitch[4];		/* (予備)			*/
	PR00290	msd[10];		/* msd[num_section個]		*/
} PR00291;

typedef struct {			/* actuator_type == ACT_CB	*/
	BYTE	osc[4];			/* 基本ﾊﾟﾙｽ時間(1us単位)	*/
	BYTE	cl_drive_tm[4];		/* CL駆動時間			*/
	BYTE	cl_on_delay[2];		/* CLオン遅延時間		*/
	BYTE	br_on_delay[2];		/* BRオン遅延時間		*/
	BYTE	br_time[2];		/* BRオン時間(10ms単位)		*/
} PR00292;

typedef union {
	PR00291	sm;			/* ステップ･モーター駆動用	*/
	PR00292	cb;			/* クラッチ／ブレーキ駆動用	*/
} PR00293;

struct fmt_hop_spec2 {
	BYTE	cmd;			/* command = 'j'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	hop_assign_ptn[8];	/* Hopper assign pattern	*/
	BYTE	open_time[4];		/* open time			*/
	BYTE	hopper_id;		/* hopper id			*/
	BYTE	hopper_no;		/* hopper number		*/
	BYTE	stop_mode;		/* 停止条件			*/
	BYTE	actuator_type;		/* アクチュエータタイプ		*/
	PR00293	cpx;			/* 複合駆動パラメータ		*/
/*-------------------------------------*/
};

int	pr_scan_hop_spec2( struct fmt_hop_spec2 *buf);
void *	pr_print_hop_spec2( struct fmt_hop_spec2 *txt, struct prr0029t *pkt);


/*------------------------------------------------------*/
/*	秤診断データ要求テキストフォーマット		*/
/*							*/
/* prr0031,prr0032					*/
/*------------------------------------------------------*/

struct fmt_diag_req {
	BYTE	cmd;			/* command = 'U'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_diag_req( struct fmt_diag_req *buf);
void *	pr_print_diag_req( struct fmt_diag_req *txt, struct prr0031t *pkt);


/*------------------------------------------------------*/
/*	ＡＦＶ調整テキストフォーマット			*/
/*							*/
/* prr0055,prr0056					*/
/*------------------------------------------------------*/

struct fmt_afv_adj {
	BYTE	cmd;			/* command = 'V'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	adj_mode[4];		/* adjust mode			*/
/*-------------------------------------*/
};

int	pr_scan_afv_adj( struct fmt_afv_adj *buf);
void *	pr_print_afv_adj( struct fmt_afv_adj *txt, struct prr0055t *pkt);


/*------------------------------------------------------*/
/*	ＡＦＶ係数呼び出しテキストフォーマット		*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_afv_recall {
	BYTE	cmd;			/* command = 'r'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*-------------------------------------*/
};


/*------------------------------------------------------*/
/*	ヘッドの重心の座標テキストフォーマット		*/
/*							*/
/* prr0061,prr0062					*/
/*------------------------------------------------------*/

struct fmt_afv_coord {
	BYTE	cmd;			/* command = 'y'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ＡＤＣボード識別番号		*/
	BYTE	coord_x[28][4];		/* 計量器とAFVの重心のＸ座標	*/
	BYTE	coord_y[28][4];		/* 計量器とAFVの重心のＹ座標	*/
	BYTE	afv_coeffi[24][8];	/* ＡＦＶ係数			*/
/*-------------------------------------*/
};

int	pr_scan_afv_coord( struct fmt_afv_coord *buf);
void *	pr_print_afv_coord( struct fmt_afv_coord *txt, struct prr0061t *pkt);


/*------------------------------------------------------*/
/*	秤診断データテキストフォーマット		*/
/*							*/
/* pri0001,pri0002					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[3];		/* ヘッドステータス		*/
} PI00010;

struct fmt_diag_data {
	BYTE	cmd;			/* command = 'u'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	PI00010	wh_3rd[32];		/* 3段目重量&ステータス[nof_wh]	*/
	PI00010	wh_4th[32];		/* 4段目重量&ステータス[nof_wh]	*/
	PI00010	wh_5th[32];		/* 5段目重量&ステータス[nof_wh]	*/
	BYTE	nof_ph[2];		/* 該当するchのPH数		*/
	PI00010	ph_3rd[32];		/* 3段目重量&ステータス[nof_ph]	*/
	PI00010	ph_4th[32];		/* 4段目重量&ステータス[nof_ph]	*/
	PI00010	ph_5th[32];		/* 5段目重量&ステータス[nof_ph]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	PI00010	df[8];			/* WH重量&ステータス[nof_df]	*/
	BYTE	nof_afv[2];		/* ＡＦＶセル数			*/
	BYTE	afv_out[16][4];		/* ＡＦＶセル出力[nof_afv]	*/
/*-------------------------------------*/
};

int	pr_scan_diag_data( struct fmt_diag_data *buf);
void *	pr_print_diag_data( struct fmt_diag_data *txt, struct pri0001t *pkt);


/*------------------------------------------------------*/
/*	ＡＦＶ係数テキストフォーマット			*/
/*							*/
/* pri0047,pri0048					*/
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

int	pr_scan_afv_coeffi( struct fmt_afv_coeffi *buf);
void *	pr_print_afv_coeffi( struct fmt_afv_coeffi *txt, struct pri0047t *pkt);


/*------------------------------------------------------*/
/*	各種データの要求テキストフォーマット		*/
/*							*/
/* prr0101,prr0102					*/
/*------------------------------------------------------*/

struct fmt_req_info {
	BYTE	cmd;			/* command = '.'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	info_no[2];		/* データの種類			*/
	BYTE	info_desc[2];		/* 識別番号			*/
/*-------------------------------------*/
};

int	pr_scan_req_info( struct fmt_req_info *buf);
void *	pr_print_req_info( struct fmt_req_info *txt, struct prr0101t *pkt);

/*------------------------------------------------------*/
/*	各種データの応答テキストフォーマット		*/
/*							*/
/* pri0025,pri0026					*/
/*------------------------------------------------------*/

struct fmt_info {
	BYTE	cmd;			/* command = '/'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	info_no[2];		/* データの種類			*/
	BYTE	info_desc[2];		/* 識別番号			*/
	BYTE	nof_data[2];		/* データ数			*/
	BYTE	data[32][8];		/* データ			*/
/*-------------------------------------*/
};

int	pr_scan_info( struct fmt_info *buf);
void *	pr_print_info( struct fmt_info *txt, struct pri0025t *pkt);


/*------------------------------------------------------*/
/*	ロジック波形データの要求テキストフォーマット	*/
/*							*/
/* prr0103,prr0104					*/
/*------------------------------------------------------*/

struct fmt_logic_parm {
	BYTE	cmd;			/* command = '0'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	logic_intvl[4];		/* サンプリング周期		*/
	BYTE	unit_no;		/* リレーユニット番号		*/
/*-------------------------------------*/
};

int	pr_scan_logic_parm( struct fmt_logic_parm *buf);
void *	pr_print_logic_parm( struct fmt_logic_parm *txt, struct prr0103t *pkt);


/*------------------------------------------------------*/
/*	ロジック波形データの応答テキストフォーマット	*/
/*							*/
/* pri0049,pri0050					*/
/*------------------------------------------------------*/

struct fmt_logic {
	BYTE	cmd;			/* command = '1'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	unit_no;		/* データの種類			*/
	BYTE	nof_data[2];		/* データ数			*/
	BYTE	data[32][4];		/* データ			*/
/*-------------------------------------*/
};

int	pr_scan_logic( struct fmt_logic *buf);
void *	pr_print_logic( struct fmt_logic *txt, struct pri0049t *pkt);


/*------------------------------------------------------*/
/*	重量波形データのパラメータテキストフォーマット	*/
/*							*/
/* prr0105,prr0106					*/
/*------------------------------------------------------*/

struct fmt_archive_parm {
	BYTE	cmd;			/* command = '2'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* WH番号			*/
	BYTE	type;			/* 重量データのタイプ		*/
	BYTE	hopper_id;		/* WH/PH識別フラグ		*/
/*-------------------------------------*/
};

int	pr_scan_archive_parm( struct fmt_archive_parm *buf);
void *	pr_print_archive_parm( struct fmt_archive_parm *txt, struct prr0105t *pkt);


/*------------------------------------------------------*/
/*	重量波形データの応答テキストフォーマット	*/
/*							*/
/* pri0053,pri0054					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE	weight[4];		/* 重量データ			*/
	BYTE	status;			/* 重量ステータス		*/
} PI00530;

struct fmt_archive {
	BYTE	cmd;			/* command = '3'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* WH/PH識別フラグ		*/
	BYTE	wh_no[2];		/* WH番号			*/
	BYTE	index[4];		/* データのインデックス		*/
	BYTE	nof_data[2];		/* データ数			*/
	PI00530	data[32];		/* データ			*/
/*-------------------------------------*/
};

int	pr_scan_archive( struct fmt_archive *buf);
void *	pr_print_archive( struct fmt_archive *txt, struct pri0053t *pkt);


/*------------------------------------------------------*/
/*	フィーダ共振周期要求テキストフォーマット	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_res_cyc_req {
	BYTE	cmd;			/* command = '4'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};


/*------------------------------------------------------*/
/*	フィーダ共振周期応答テキストフォーマット	*/
/*							*/
/* pri0051,pri0052					*/
/*------------------------------------------------------*/

struct fmt_resonance {
	BYTE	cmd;			/* command = '5'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_res_cycle[32][4];	/* RFの共振周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_res_cycle[8][4];	/* DFの共振周期			*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_res_cycle[32][4];	/* AFの共振周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_res_cycle[8][4];	/* CFの共振周期			*/
/*-------------------------------------*/
};

int	pr_scan_resonance( struct fmt_resonance *buf);
void *	pr_print_resonance( struct fmt_resonance *txt, struct pri0051t *pkt);


/*------------------------------------------------------*/
/*	フィーダ共振周期検出開始テキストフォーマット	*/
/*							*/
/* prr0107,prrr0108					*/
/*------------------------------------------------------*/

struct fmt_resonance_detect {
	BYTE	cmd;			/* command = '6'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	rf_detect_ptn[8];	/* RF共振周期検出パターン	*/
	BYTE	df_detect_ptn[8];	/* DF共振周期検出パターン	*/
	BYTE	af_detect_ptn[8];	/* AF共振周期検出パターン	*/
	BYTE	cf_detect_ptn[8];	/* CF共振周期検出パターン	*/
/*-------------------------------------*/
};

int	pr_scan_res_detect( struct fmt_resonance_detect *buf);
void *	pr_print_res_detect( struct fmt_resonance_detect *txt, struct prr0107t *pkt);


/*------------------------------------------------------*/
/*	フィーダ共振周期要求テキストフォーマット	*/
/*							*/
/*							*/
/*------------------------------------------------------*/

struct fmt_res_cyc_stop {
	BYTE	cmd;			/* command = '7'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
};


/*------------------------------------------------------*/
/*	フィーダ駆動周期設定テキストフォーマット	*/
/*							*/
/* prr0109,prr0110					*/
/*------------------------------------------------------*/

struct fmt_drv_cycle {
	BYTE	cmd;			/* command = '8'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_drv_cycle[32][4];	/* RFの駆動周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_drv_cycle[8][4];	/* DFの駆動周期			*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_drv_cycle[32][4];	/* AFの駆動周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_drv_cycle[8][4];	/* CFの駆動周期			*/
/*-------------------------------------*/
};

int	pr_scan_drv_cycle( struct fmt_drv_cycle *buf);
void *	pr_print_drv_cycle( struct fmt_drv_cycle *txt, struct prr0109t *pkt);


/*------------------------------------------------------*/
/*	フィーダ共振周期設定テキストフォーマット	*/
/*							*/
/* prr0111,prr0112					*/
/*------------------------------------------------------*/

struct fmt_natural_freq {
	BYTE	cmd;			/* command = '9'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_natural_freq[32][4];	/* RFの共振周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_natural_freq[8][4];	/* DFの共振周期			*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_natural_freq[32][4];	/* AFの共振周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_natural_freq[8][4];	/* CFの共振周期			*/
/*-------------------------------------*/
};

int	pr_scan_natural_freq( struct fmt_natural_freq *buf);
void *	pr_print_natural_freq( struct fmt_natural_freq *txt, struct prr0111t *pkt);


/*------------------------------------------------------*/
/*	サンプル排出のテキストフォーマット		*/
/*							*/
/* prr0069,prr0070					*/
/*------------------------------------------------------*/

struct fmt_sample_dump {
	BYTE	cmd;			/* command = ':'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	sect_assign_ptn[2];	/* section pattern		*/
/*-------------------------------------*/
};

int	pr_scan_sample_dump( struct fmt_sample_dump *buf);
void *	pr_print_sample_dump( struct fmt_sample_dump *txt, struct prr0069t *pkt);


/*------------------------------------------------------*/
/*	スパン係数要求テキストフォーマット		*/
/*							*/
/* prr0085,prr0086					*/
/*------------------------------------------------------*/

struct fmt_span_call {
	BYTE	cmd;			/* command = 'J'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_no;			/* ＡＤＣボード識別番号		*/
	BYTE	range;			/* 秤量				*/
/*-------------------------------------*/
};

int	pr_scan_span_call( struct fmt_span_call *buf);
void *	pr_print_span_call( struct fmt_span_call *txt, struct prr0085t *pkt);


/*------------------------------------------------------*/
/*	スパン係数応答テキストフォーマット		*/
/*							*/
/* pri0043,pri0044					*/
/*------------------------------------------------------*/

struct fmt_span_coeffi {
	BYTE	cmd;			/* command = 'K'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	pwb_id;			/* ＡＤＣボード識別番号		*/
	BYTE	range;			/* 秤量				*/
	BYTE	date[8];		/* 調整日付			*/
	BYTE	time[8];		/* 調整時間			*/
	BYTE	span_coeffi[24][4];	/* ＳＰＡＮ係数			*/
/*-------------------------------------*/
};

int	pr_scan_span_coeffi( struct fmt_span_coeffi *buf);
void *	pr_print_span_coeffi( struct fmt_span_coeffi *txt, struct pri0043t *pkt);


/*------------------------------------------------------*/
/*	付加装置パラメータテキストフォーマット		*/
/*							*/
/* prr0063,prr0064					*/
/*------------------------------------------------------*/

struct updown_chute_parm {
	BYTE	cmd;			/* command = '@'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dev_no;			/* 装置番号			*/
	BYTE	nof_pos[2];		/* ポジション数			*/
	BYTE	err_detect_interval[2];	/* エラー検出時間（*10ms）	*/
	BYTE	position1[2];		/* 設定位置１			*/
	BYTE	position2[2];		/* 設定位置２			*/
/*-------------------------------------*/
};

int	pr_scan_updown_chute(struct updown_chute_parm *buf);
void *	pr_print_updown_chute(struct updown_chute_parm *txt, struct prr0063t *pkt);


/*------------------------------------------------------*/
/*	供給制御信号ON/OFFテキストフォーマット		*/
/*							*/
/* prr0113,prr0114					*/
/*------------------------------------------------------*/

struct fmt_infonoff {
	BYTE	cmd;			/* command = 'l'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	onoff_key;		/* infeed ON/OFF		*/
/*-------------------------------------*/
};

int	pr_scan_infonoff( struct fmt_infonoff *buf);
void *	pr_print_infonoff( struct fmt_infonoff *txt, struct prr0113t *pkt);


/*------------------------------------------------------*/
/*	全排出時の自動零点制御テキストフォーマット	*/
/*							*/
/* prr0115,prr0116					*/
/*------------------------------------------------------*/

struct fmt_drain_zero {
	BYTE	cmd;			/* command = 'm'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	drain_zero;		/* drain auto zero flag		*/
/*-------------------------------------*/
};

int	pr_scan_drain_zero( struct fmt_drain_zero *buf);
void *	pr_print_drain_zero( struct fmt_drain_zero *txt, struct prr0115t *pkt);


/*------------------------------------------------------*/
/*	信号出力先のアサインテキストフォーマット	*/
/*							*/
/* prr0119,prr0120					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE	sig_assign[2];		/* signal assign		*/
} PR01190;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	PR01190	phead[2];		/* parameter[nof_phead]		*/
} PR01191;

typedef struct {
	BYTE	nof_sect;		/* number of section		*/
	PR01191	sect[8];		/* parameter[nof_sect]		*/
} PR01192;

struct fmt_sig_assign {
	BYTE	cmd;			/* command = 'B'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_infsig;		/* number of infeed signal	*/
	BYTE	infsig_assign[8][2];	/* infeed signal assign		*/
	BYTE	lowp_sig_assign[8][2];	/* low product signal assign	*/
	BYTE	nof_df;			/* number of DF			*/
	BYTE	dfsync_sig_assign[8][2];/* DF sync. signal assign	*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	dumpsig_assign[4][2];	/* dump signal assign		*/
	BYTE	pr_dumpsig_assign[4][2];/* proper dump signal assign	*/
	BYTE	ng_dumpsig_assign[4][2];/* NG dump signal assign	*/
	BYTE	pr_readysig_assign[4][2];/* proper ready signal assign	*/
	BYTE	ng_readysig_assign[4][2];/* NG ready signal assign	*/
	BYTE	pr_each_dpsig_assign[4][2];/* proper each dump sig. assign*/
	BYTE	ng_each_dpsig_assign[4][2];/* NG each dump signal assign*/
	BYTE	pr_addsig_assign[4][2];	/* proper addition signal assign*/
	BYTE	ng_addsig_assign[4][2];	/* NG addition signal assign	*/
	BYTE	error_sig_assign[4][2];	/* error signal assign		*/
	BYTE	control_sig_assign[4][2];/* control signal assign	*/
	BYTE	prod_sig_assign[4][2];	/* production signal assign	*/
	BYTE	drain_sig_assign[4][2];	/* drain signal assign		*/
	BYTE	manuzero_sig_assign[4][2];/* manual zero adj. sig. assign*/
	BYTE	error_rej_sig_assign[4][2];/* error reject sig. assign	*/
	BYTE	lowp_stop_sig_assign[4][2];/* low product stop sig. assign*/
	BYTE	metal_det_sig_assign[4][2];/* metal detect signal assign*/
	PR01192	pandc[4];		/* parent parameter		*/
/*-------------------------------------*/
};

int	pr_scan_sig_assign_parm( struct fmt_sig_assign *buf);
void *	pr_print_sig_assign_parm( struct fmt_sig_assign *txt, struct prr0119t *pkt);


/*------------------------------------------------------*/
/*	リレー基板の出力チェックテキストフォーマット	*/
/*							*/
/* prr0121,prr0122					*/
/*------------------------------------------------------*/

struct fmt_ryout_check {
	BYTE	cmd;			/* command = '$'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* リレー基板番号		*/
	BYTE	drive_ptn[2];		/* 駆動リレーパターン		*/
	BYTE	drive_delay[2];		/* 駆動遅延時間[10msec]		*/
	BYTE	drive_time[2];		/* 駆動時間[10msec,0=OFF,255=ON]*/
/*-------------------------------------*/
};

int	pr_scan_ryout_check( struct fmt_ryout_check *buf);
void *	pr_print_ryout_check( struct fmt_ryout_check *txt, struct prr0121t *pkt);


/*------------------------------------------------------*/
/*	リレー基板の入力チェックテキストフォーマット	*/
/*							*/
/* prr0123,prr0124					*/
/*------------------------------------------------------*/

struct fmt_ryin_check {
	BYTE	cmd;			/* command = ';'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* リレー基板番号		*/
/*-------------------------------------*/
};

int	pr_scan_ryin_check( struct fmt_ryin_check *buf);
void *	pr_print_ryin_check( struct fmt_ryin_check *txt, struct prr0123t *pkt);


/*------------------------------------------------------*/
/*	リレー基板の入力状態応答テキストフォーマット	*/
/*							*/
/* pri0055,pri0056					*/
/*------------------------------------------------------*/

struct fmt_ryin {
	BYTE	cmd;			/* command = ';'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	brd_no;			/* リレー基板番号		*/
	BYTE	input_ptn[2];		/* 入力状態パターン		*/
/*-------------------------------------*/
};

int	pr_scan_ryin( struct fmt_ryin *buf);
void *	pr_print_ryin( struct fmt_ryin *txt, struct pri0055t *pkt);


/*------------------------------------------------------*/
/*	ホッパ駆動回数要求テキストフォーマット		*/
/*							*/
/* prr0125,prr0126					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_req {
	BYTE	cmd;			/* command = '#'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* ホッパ識別フラグ		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_req( struct fmt_hop_drive_req *buf);
void *	pr_print_hop_drive_req( struct fmt_hop_drive_req *txt, struct prr0125t *pkt);


/*------------------------------------------------------*/
/*	ホッパ駆動回数応答テキストフォーマット		*/
/*							*/
/* pri0057,pri0058					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_no {
	BYTE	cmd;			/* command = '%'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* ホッパ識別フラグ		*/
	BYTE	nof_hopper[2];		/* ホッパ数			*/
	BYTE	nof_times[32][8];	/* ホッパ駆動回数		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_no( struct fmt_hop_drive_no *buf);
void *	pr_print_hop_drive_no( struct fmt_hop_drive_no *txt, struct pri0057t *pkt);

/*------------------------------------------------------*/
/*	状態表示情報のテキストフォーマット		*/
/*							*/
/* pri0061,pri0062					*/
/*------------------------------------------------------*/

struct fmt_dspinf {
	BYTE	cmd;			/* command = 'ｪ'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	info[2];		/* 表示情報			*/
	BYTE	hp_ptn[8];		/* Hopper pattern		*/
/*-------------------------------------*/
};

int	pr_scan_dsp_info( struct fmt_dspinf *buf);
void *	pr_print_dsp_info( struct fmt_dspinf *txt, struct pri0061t *pkt);


/*------------------------------------------------------*/
/*	ホッパ駆動回数設定テキストフォーマット		*/
/*							*/
/* prr0127,pry0128					*/
/*------------------------------------------------------*/

struct fmt_hop_drive_set {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	hopper_id;		/* ホッパ識別フラグ		*/
	BYTE	hopper_no[2];		/* ホッパ番号			*/
	BYTE	nof_times[8];		/* ホッパ駆動回数		*/
/*-------------------------------------*/
};

int	pr_scan_hop_drive_set( struct fmt_hop_drive_set *buf);
void *	pr_print_hop_drive_set( struct fmt_hop_drive_set *txt, struct prr0127t *pkt);


/*--------------------------------------------------------------*/
/*	運転中にホッパを取り外しての零点テキストフォーマット	*/
/*								*/
/* prr0129,pry0130						*/
/*--------------------------------------------------------------*/

struct fmt_fzero {
	BYTE	cmd;			/* command = '\''		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_nonactive[8];	/* WH non active head	*/
	BYTE	ph_nonactive[8];	/* PH non active head	*/
/*-------------------------------------*/
};

int	pr_scan_fzero( struct fmt_fzero *buf);
void *	pr_print_fzero( struct fmt_fzero *txt, struct prr0129t *pkt);


/*------------------------------------------------------*/
/*	予約番号テキストフォーマット			*/
/*							*/
/* prr0131,prr0132					*/
/*------------------------------------------------------*/

struct fmt_preset_no {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	preset_no[4];		/* preset number		*/
/*-------------------------------------*/
};

int	pr_scan_preset_no( struct fmt_preset_no *buf);
void *	pr_print_preset_no( struct fmt_preset_no *txt, struct prr0131t *pkt);

/*------------------------------------------------------*/
/*	金属検出機テストのテキストフォーマット		*/
/*							*/
/* prr0067,prr0068					*/
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

int	pr_scan_metal_text( struct fmt_metal_text *buf);
void *	pr_print_metal_text( struct fmt_metal_text *txt, struct prr0067t *pkt);

/*------------------------------------------------------*/
/*	金属検出機テスト結果のテキストフォーマット	*/
/*							*/
/* pri0033,pri0034					*/
/*------------------------------------------------------*/

struct fmt_metal_answer {
	BYTE	cmd;			/* command = '}'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	answer;			/* 0:成功 1:失敗 2:中止 3:SD中止*/
/*-------------------------------------*/
};

int	pr_scan_metal_answer( struct fmt_metal_answer *buf);
void *	pr_print_metal_answer( struct fmt_metal_answer *txt, struct pri0033t *pkt);

/*----------------------------------------------------------------------*/
/*	親ヘッドの遅れ時間拡張タイミングの設定テキストフォーマット	*/
/*									*/
/* prr1017,prr1018							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	parent_delay[2];	/* 親ヘッド遅れ時間		*/
} PR10170;

typedef struct {
	BYTE	nof_parent;		/* 該当するchのセクション数	*/
	PR10170	pandc[2];		/* paramter[nof_parent]		*/
} PR10171;

struct fmt_parent_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR10171	sect[8];		/* P4 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_parent_tmg( struct fmt_parent_tmg *buf);
void *	pr_print_parent_tmg( struct fmt_parent_tmg *txt, struct prr1017t *pkt);

/*----------------------------------------------------------------------*/
/*  ＤＣＡパラメータコマンドのテキストフォーマット			*/
/*									*/
/* prr0075,prr0076							*/
/*----------------------------------------------------------------------*/

struct fmt_dca_parm {
	BYTE	cmd;			/* command = '!'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_on_off;		/* DCA ON/OFF			*/
	BYTE	dca_cycle[4];		/* DCA cycle count		*/
	BYTE	dca_owf[2];		/* DCA OWF			*/
	BYTE	dca_sdm[2];		/* DCA SDM			*/
/*-------------------------------------*/
};

int	pr_scan_dca_parm( struct fmt_dca_parm *buf);
void *	pr_print_dca_parm( struct fmt_dca_parm *txt, struct prr0075t *pkt);


/*----------------------------------------------------------------------*/
/*  ＡＴＡ偏差のテキストフォーマット					*/
/*									*/
/* prr0077,prr0078							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_dev {
	BYTE	cmd;			/* command = '"'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_dev[8];		/* ATA deviation		*/
/*-------------------------------------*/
};

int	pr_scan_ata_dev( struct fmt_ata_dev *buf);
void *	pr_print_ata_dev( struct fmt_ata_dev *txt, struct prr0077t *pkt);


/*----------------------------------------------------------------------*/
/*	ＤＣＡモニター情報のテキストフォーマット			*/
/*									*/
/* pri0037,pri0038							*/
/*----------------------------------------------------------------------*/

struct fmt_dca_monitor {
	BYTE	cmd;			/* command = '('		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	dca_sum_cnt[4];		/* DCAサンプルカウント値	*/
	BYTE	bf_cbwt[8];		/* 再チェック前重量		*/
	BYTE	af_cbwt[8];		/* 再チェック後重量		*/
/*-------------------------------------*/
};

int	pr_scan_dca_monitor( struct fmt_dca_monitor *buf);
void *	pr_print_dca_monitor( struct fmt_dca_monitor *txt, struct pri0037t *pkt);


/*----------------------------------------------------------------------*/
/*	ＡＴＡモニター情報のテキストフォーマット			*/
/*									*/
/* pri0039,pri0040							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_monitor {
	BYTE	cmd;			/* command = ')'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	ata_enable;		/* ＡＴＡ  enable		*/
	BYTE	ata_timer[4];		/* ＡＴＡタイマー値		*/
	BYTE	ata_mode;		/* 現在のＡＴＡモード		*/
	BYTE	ata_tgt[8];		/* ＡＴＡターゲット		*/
	BYTE	ata_def_time[4];	/* ATA default time		*/
	BYTE	ata_cwb[4];		/* ＣＷＢ			*/
/*-------------------------------------*/
};

int	pr_scan_ata_monitor( struct fmt_ata_monitor *buf);
void *	pr_print_ata_monitor( struct fmt_ata_monitor *txt, struct pri0039t *pkt);


/*----------------------------------------------------------------------*/
/*    組み合わせ計算拡張パラメータテキストフォーマット			*/
/*    ＭＡＸ  ＳＣＡＬＥ						*/
/* prr3011,prr3012							*/
/*----------------------------------------------------------------------*/

struct fmt_max_scale {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	max_scale;		/* max scale			*/
/*-------------------------------------*/
};

int	pr_scan_max_scale( struct fmt_max_scale *buf);
void *	pr_print_max_scale( struct fmt_max_scale *txt, struct prr3011t *pkt);


/*----------------------------------------------------------------------*/
/*   本体状態情報拡張パラメータテキストフォーマット			*/
/*									*/
/* pri1013,pri1014							*/
/*----------------------------------------------------------------------*/

struct fmt_ata_target {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	ata_target[4][8];	/* ata target			*/
/*-------------------------------------*/
};

int	pr_scan_ata_target(struct fmt_ata_target *buf);
void *	pr_print_ata_target(struct fmt_ata_target *txt,struct pri1013t *pkt);


/*----------------------------------------------------------------------*/
/*	ＡＴＡ98モニター情報  テキストフォーマット			*/
/*	ステイル・ロックタイム・テキスト				*/
/*									*/
/* pri2039,pri2040							*/
/*----------------------------------------------------------------------*/
struct fmt_stale98_time {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	stale98_time[4];	/* ステイル・ロック・タイム	*/
	BYTE	stale98_mode;		/* 現在の状態 (stale_mode)	*/
	BYTE	stale98_tmr[4];		/* ステイルタイマ、ドレインタイマ*/
	BYTE	stale98_dcnt[2];	/* ダウンカウント値		*/
	BYTE	stale98_SDEnable;	/* ホストから設定されたStale Drain Enable */
	BYTE	stale98_drain_mode;	/* 現在の状態（Drain Mode)	*/
	BYTE	stale98_drain_tmr[4];	/* STALE DRAIN CLEAR TIMEのｶｳﾝﾄﾀﾞｳﾝ*/
					/* ﾄﾞﾚｲﾝﾓｰﾄﾞがSTALE_DRAIN_START時有効*/
/*-------------------------------------*/
};

int	pr_scan_stale98_time( struct fmt_stale98_time *buf);
void *	pr_print_stale98_time( struct fmt_stale98_time *txt, struct pri2039t *pkt);


/*----------------------------------------------------------------------*/
/*	計量目標値拡張パラメータテキストフォーマット			*/
/*	ＡＴＡ  ＳＡＭＰＬＥ  ＳＩＺＥ					*/
/* prr4023,prr4024							*/
/*----------------------------------------------------------------------*/

struct fmt_sample_size {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	sample_size[4];		/* ata sample size		*/
	BYTE	preset_num[2];		/* プリセット番号		*/
/*-------------------------------------*/
};

int	pr_scan_sample_size( struct fmt_sample_size *buf);
void *	pr_print_sample_size( struct fmt_sample_size *txt, struct prr4023t *pkt);


/*------------------------------------------------------*/
/*	新ＡＦＤのテキストフォーマット			*/
/*							*/
/* prr0133,prr0134					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	tbl[15][2];		/* fuzzy table			*/
	BYTE	zero_d_num[2];		/* 空検出時操作量		*/
	BYTE	p_os_c_num[2];		/* オーバースケール検出時操作量	*/
	BYTE	trb_off_head[2];	/* ヘッド数低下停止ヘッド数	*/
	BYTE	cln_req_sw[2];		/* 清掃要求検知			*/
	BYTE	good_effi[4];		/* 高稼働率判定値		*/
} PR01330;

typedef struct {
	BYTE	nof_sect;	/* 該当するchのセクション数		*/
	PR01330	sect[8];	/* AFD parameter[nof_sect]		*/
} PR01331;

typedef struct {
	BYTE	df_fuzzy_gain[4];	/* ファジーテーブル操作ゲイン		*/
	BYTE	df_l_detect[4];		/* 分散操作許容サイクル数		*/
	BYTE	df_ow_ctrl[2];		/* 供給過大状態時間操作			*/
	BYTE	dfw_on_off[2];		/* 分散重量制御ON/OFF			*/
	BYTE	df_e_detect1[2];	/* 分散操作許容空ヘッド数（バラツキ小）	*/
	BYTE	df_e_detect2[2];	/* 分散操作許容空ヘッド数（バラツキ大）	*/
	BYTE	df_w_max[4];		/* 分散重量制御最大値			*/
	BYTE	df_w_min[4];		/* 分散重量制御最小値			*/
	BYTE	low_wt[2];		/* AFD制御停止分散重量			*/
} PR01332;

struct fmt_adv_afd {
	BYTE	cmd;			/* command = '+'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channnel		*/
	PR01331	ch[4];			/* channel parameter		*/
	BYTE	nof_infeed;		/* number of infeed		*/
	PR01332	ifd[8];			/* PR01332 paramter[nof_infeed]	*/
/*-------------------------------------*/
};

int	pr_scan_adv_afd( struct fmt_adv_afd *buf);
void *	pr_print_adv_afd( struct fmt_adv_afd *txt, struct prr0133t *pkt);


/*----------------------------------------------------------------------*/
/*    ＰＨセルデータテキストフォーマット				*/
/*									*/
/* prr2009,prr2010							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	empty_weight[4];	/* empty weight			*/
	BYTE	stick_weight[4];	/* stick weight			*/
	BYTE	ph_weight_check;	/* PHの重量監視			*/
} PR20091;

typedef struct {
	BYTE	stable_time[4];		/* stable time constant		*/
	BYTE	zero_interval[2];	/* auto zero interval		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR20091	sect[8];		/* PR00091 paramter[nof_sect]	*/
} PR20090;

struct fmt_ph_cell {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ph_span_adj;		/* PH span adjustment		*/
	BYTE	nof_ch;			/* number of channnel		*/
	PR20090	ch[4];			/* channel parameter		*/
/*-------------------------------------*/
};

int	pr_scan_ph_cell( struct fmt_ph_cell *buf);
void *	pr_print_ph_cell( struct fmt_ph_cell *txt, struct prr2009t *pkt);


/*----------------------------------------------------------------------*/
/*    セミオート機計量データテキストフォーマット			*/
/*									*/
/* prr6023,prr6024							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	wh_async_supply;	/* WHへの非同期供給		*/
	BYTE	only_bh_cal;		/* BHのみで組み合わせ計算	*/
	BYTE	bh_async_supply;	/* BHへの非同期供給		*/
} PR60230;

struct fmt_semi_auto {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	dump_dir;		/* dump direction		*/
	BYTE	conv_rev;		/* 集合コンベヤ正逆転動作	*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR60230	sect[8];		/* PR60230 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_semi_auto( struct fmt_semi_auto *buf);
void *	pr_print_semi_auto( struct fmt_semi_auto *txt, struct prr6023t *pkt);

/*----------------------------------------------------------------------*/
/* マルチ連動コンディションテキストフォーマット				*/
/*									*/
/* pri0045,pri0046							*/
/*----------------------------------------------------------------------*/

typedef union {
	BYTE		c_data[32];	/* 汎用データ			*/
	BYTE		i_data[32];	/* 汎用データ			*/
	BYTE		l_data[32];	/* 汎用データ			*/
} PI00450;

typedef struct {
	BYTE		data_cnt[2];	/* データバイト数		*/
	PI00450		dt;		/* 汎用データ			*/
} PI00451;

typedef struct {
	BYTE		disp_cnt[2];	/* データバイト数		*/
	BYTE		disp_str[128];	/* 表示データ			*/
} PI00452;

typedef struct {
	BYTE		prn_cnt[2];	/* データバイト数		*/
	BYTE		prn_str[32];	/* 印字データ			*/
} PI00453;

struct fmt_mlt_cond {
	BYTE	cmd;			/* command = '<'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	cond_inf[2];		/* 状態内容			*/
	BYTE	act_inf[2];		/* ＲＣＵ動作情報		*/
	BYTE	node_ptn[8];		/* error node pattern		*/
	BYTE	error_no[4];		/* エラー情報			*/
	PI00451	ex_d;			/* 汎用データ			*/
	PI00452	ds_d;			/* 表示データ			*/
	PI00453	pr_d;			/* 印字データ			*/
/*-------------------------------------*/
};

int	pr_scan_mlt_cond( struct fmt_mlt_cond *buf);
void *	pr_print_mlt_cond( struct fmt_mlt_cond *txt, struct pri0045t *pkt);

/*----------------------------------------------------------------------*/
/* マルチ連動パラメータ1リクエストテキストフォーマット			*/
/*									*/
/* prr0089,prr0090							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_p1req {
	BYTE	cmd;			/* command = '_'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
/*-------------------------------------*/
};

int	pr_scan_mlt_p1req( struct fmt_mlt_p1req *buf);
void *	pr_print_mlt_p1req( struct fmt_mlt_p1req *txt, struct prr0089t *pkt);

/*----------------------------------------------------------------------*/
/* マルチ連動パラメータ1テキストフォーマット				*/
/*									*/
/* prr0091,prr0092							*/
/*----------------------------------------------------------------------*/

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

int	pr_scan_mlt_parm1( struct fmt_mlt_parm1 *buf);
void *	pr_print_mlt_parm1( struct fmt_mlt_parm1 *txt, struct prr0091t *pkt);

/*----------------------------------------------------------------------*/
/* マルチ連動パラメータ２テキストフォーマット				*/
/*									*/
/* prr0093,prr0094							*/
/*----------------------------------------------------------------------*/

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

int	pr_scan_mlt_parm2( struct fmt_mlt_parm2 *buf);
void *	pr_print_mlt_parm2( struct fmt_mlt_parm2 *txt, struct prr0093t *pkt);

/*----------------------------------------------------------------------*/
/* マルチ連動拡張パラメータテキストフォーマット				*/
/*									*/
/* prr0095,prr0096							*/
/*----------------------------------------------------------------------*/

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

int	pr_scan_mlt_ext( struct fmt_mlt_ext *buf);
void *	pr_print_mlt_ext( struct fmt_mlt_ext *txt, struct prr0095t *pkt);

/*----------------------------------------------------------------------*/
/*	マルチ連動パラメータ２拡張テキストフォーマット			*/
/*									*/
/* prr1093,prr1094							*/
/*----------------------------------------------------------------------*/

struct fmt_mlt_parm2_ext {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_mch[2];		/* 計量機数			*/
	BYTE	node_id[32][2];		/* NHICノード番			*/
/*-------------------------------------*/
};

int	pr_scan_mlt_parm2_ext( struct fmt_mlt_parm2_ext *buf);
void *	pr_print_mlt_parm2_ext( struct fmt_mlt_parm2_ext *txt, struct prr1093t *pkt);

/*----------------------------------------------------------------------*/
/*    シフトデータ応答結果データテキストフォーマット    		*/
/*									*/
/* pri0041,pri0042							*/
/*----------------------------------------------------------------------*/
typedef struct {
	BYTE		sta_flag;	/* state flag			*/
	BYTE		weight[8];	/* 重量				*/
	BYTE		count[8];	/* 個数 			*/
	BYTE         	ser_no[2];	/* シリアル番号			*/
} PI00410;

struct fmt_shift_data {
	BYTE	cmd;			/* command = '&'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	all_num[2];		/* all data num 		*/
	BYTE	data_page[2];		/* data page          		*/
	BYTE	data_num;		/* ページ内のデータ数		*/
	PI00410 data[16];		/* 各データ内容			*/

/*-------------------------------------*/
};

int	pr_scan_shift_data( struct fmt_shift_data *buf);
void *	pr_print_shift_data( struct fmt_shift_data *txt, struct pri0041t *pkt);

/*----------------------------------------------------------------------*/
/*  シフトデータ参照（要求）仕様テキストフォーマット    		*/
/*									*/
/* prr0083,prr0084							*/
/*----------------------------------------------------------------------*/
struct fmt_shift_mon {
	BYTE	cmd;			/* command = '%'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	req_type;		/* reqest type   		*/
	BYTE	data_page[2];		/* data page          		*/
/*-------------------------------------*/
};

int	pr_scan_shift_mon( struct fmt_shift_mon *buf);
void *	pr_print_shift_mon( struct fmt_shift_mon *txt, struct prr0083t *pkt);


/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（２）			*/
/*			テキストフォーマット		*/
/*	アベコン・サイクル／レシオ／リセットカウント	*/
/* prr2023,prr2024					*/
/*------------------------------------------------------*/

struct fmt_avg_parm {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	avg_cycle[2];		/* average cycle		*/
	BYTE	avg_ratio[2];		/* average ratio		*/
	BYTE	avg_reset_count[4];	/* average reset count		*/
/*-------------------------------------*/
};

int	pr_scan_avg_parm( struct fmt_avg_parm *buf);
void *	pr_print_avg_parm( struct fmt_avg_parm *txt, struct prr2023t *pkt);


/*----------------------------------------------------------------------*/
/*	補助組み合わせ候補重量データ構造テキストフォーマット		*/
/*									*/
/* pri1029,pri1030							*/
/*----------------------------------------------------------------------*/

typedef struct {
	BYTE	nof_data;		/* number of data		*/
	BYTE	weight[32][4];		/* reference weight		*/
} PR10290;

struct fmt_ref_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR10290	sect[8];		/* paramter[nof_sect]		*/
/*-------------------------------------*/
};

int	pr_scan_ref_weight( struct fmt_ref_weight *buf);
void *	pr_print_ref_weight( struct fmt_ref_weight *txt, struct pri1029t *pkt);


/*------------------------------------------------------*/
/*	PLCデータ書込みテキストフォーマット		*/
/*							*/
/* prr0087,prr0088					*/
/*------------------------------------------------------*/

struct fmt_plc_wr {
	BYTE	cmd;			/* command = '''		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	plc_wr_data[25][4];	/* PLC write data		*/
	BYTE	dump_mode[4];		/* dump mode			*/
/*-------------------------------------*/
};

int		pr_scan_plc_data( struct fmt_plc_wr *buf);
void *		pr_print_plc_data( struct fmt_plc_wr *txt, struct prr0087t *pkt);


/*------------------------------------------------------*/
/*	計量目標値拡張パラメータ（５）			*/
/*		テキストフォーマット			*/
/*	拡張上下限値データ				*/
/* prr5023,prr5024					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	upper_wt[8];		/* 上限重量			*/
	BYTE	lower_wt[8];		/* 下限重量			*/
	BYTE	upper_cnt[4];		/* 上限個数			*/
	BYTE	lower_cnt[4];		/* 下限個数			*/
} PR50230;

struct fmt_ext_tgt {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	PR50230	sect[8];		/* PR50230 paramter[nof_sect]	*/
/*-------------------------------------*/
};

int	pr_scan_ext_tgt(struct fmt_ext_tgt *buf);
void *	pr_print_ext_tgt(struct fmt_ext_tgt *txt, struct prr5023t *pkt);


/*----------------------------------------------------------------------*/
/*   本体状態情報拡張パラメータテキストフォーマット			*/
/*									*/
/* pri2013,pri2014							*/
/*----------------------------------------------------------------------*/

struct fmt_fsv_condition {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	BYTE	cnd[4];			/* FSV condition		*/
/*-------------------------------------*/
};

int	pr_scan_fsv_condition(struct fmt_fsv_condition *buf);
void *	pr_print_fsv_condition(struct fmt_fsv_condition *txt,struct pri2013t *pkt);


/*----------------------------------------------------------------------*/
/*	供給振り分け装置ゲート位置拡張パラメータテキストフォーマット	*/
/*									*/
/* prr1005,prr1006							*/
/*----------------------------------------------------------------------*/

struct fmt_gate_position {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channnel		*/
	BYTE	gate_position[4];	/* 供給振り分け装置ゲート位置	*/
/*-------------------------------------*/
};

int	pr_scan_gate_position( struct fmt_gate_position *buf);
void *	pr_print_gate_position( struct fmt_gate_position *txt, struct prr1005t *pkt);

/*------------------------------------------------------*/
/*	特殊仕様データの設定テキストフォーマット	*/
/*							*/
/* prr0135,prr0136					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	spec_no[2];		/* 仕様番号			*/
	BYTE	nof_data;		/* データ数			*/
	BYTE	parm[8][8];		/* データ			*/
} PR01350;

struct fmt_sp_parm {
	BYTE	cmd;			/* command = 'T'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_parm[2];		/* number of paramter		*/
	PR01350	sp_parm[16];		/* PR01350 sp_parm[nof_parm]	*/
/*-------------------------------------*/
};

int	pr_scan_sp_parm( struct fmt_sp_parm *buf);
void *	pr_print_sp_parm( struct fmt_sp_parm *txt, struct prr0135t *pkt);

/*------------------------------------------------------*/
/*	分散シャッターの設定テキストフォーマット	*/
/*							*/
/* prr0099,prr0100					*/
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
} PR0990;


struct fmt_dfsh_parm {
	BYTE	cmd;			/* command = '|'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	PR0990	sh[8];			/* 分散シャッター情報		*/
/*-------------------------------------*/
};

int	pr_scan_sh_parm( struct fmt_dfsh_parm *buf);
void *	pr_print_sh_parm( struct fmt_dfsh_parm *txt, struct prr0099t *pkt);

/*------------------------------------------------------*/
/*	拡張パラメータのテキストフォーマット		*/
/*							*/
/* prr0137,prr0138					*/
/*------------------------------------------------------*/

struct fmt_ext_parm {
	BYTE	cmd;			/* command = 0xac		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	sub_cmd[2];		/* sub command			*/
	BYTE	nof_parm[2];		/* number of paramter		*/
	BYTE	parm[40][8];		/* データ			*/
/*-------------------------------------*/
};

int	pr_scan_ext_parm( struct fmt_ext_parm *buf);
void *	pr_print_ext_parm( struct fmt_ext_parm *txt, struct prr0137t *pkt);

/*----------------------------------------------------------------------*/
/*	4-20mA制御用拡張パラメータテキストフォーマット			*/
/*									*/
/* prr2005,prr2006							*/
/*----------------------------------------------------------------------*/

struct fmt_max_df_weight {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_sig[2];		/* 供給機数			*/
	BYTE	max_df_weight[8][4];	/* 4-20mA用最大分散重量		*/
/*-------------------------------------*/
};

int	pr_scan_max_df_weight( struct fmt_max_df_weight *buf);
void *	pr_print_max_df_weight( struct fmt_max_df_weight *txt, struct prr2005t *pkt);

/*----------------------------------------------------------------------*/
/*   計算結果拡張パラメータテキストフォーマット				*/
/*									*/
/* pri1011,pri1012							*/
/*----------------------------------------------------------------------*/

struct fmt_cb_caltm {
	BYTE	prm_pnt[4];		/* pointer to option parameter		*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel			*/
	BYTE	cb_caltm[4][8];		/* time of combinated calcuration	*/
/*-------------------------------------*/
};

int	pr_scan_cb_caltm(struct fmt_cb_caltm *buf);
void *	pr_print_cb_caltm(struct fmt_cb_caltm *txt, struct pri1011t *pkt);

/*----------------------------------------------------------------------*/
/*   フィーダ駆動特性データ構造 拡張パラメータテキストフォーマット 	*/
/*									*/
/* prr1015,prr1016							*/
/*----------------------------------------------------------------------*/
typedef struct  {
	BYTE	Ap[4];
	BYTE	Aon[4];
	BYTE	Bp[4];
	BYTE	Bon[4];
} PRR10150;

typedef struct {
	PRR10150 start;
	PRR10150 stop;
} PRR10151;

struct fmt_fd_spec_edge {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	PRR10151	mess[4];
/*-------------------------------------*/
};

int	pr_scan_fd_spec_edge( struct fmt_fd_spec_edge *buf);
void *	pr_print_fd_spec_edge( struct fmt_fd_spec_edge *txt, struct prr1015t *pkt);


/*----------------------------------------------------------------------*/
/*	第二スパン構成拡張パラメータテキストフォーマット	*/
/*									*/
/* prr1035,prr1036							*/
/*----------------------------------------------------------------------*/

struct fmt_span2_adj {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	span_wt2[4];	/* 第二スパン重量	*/
/*-------------------------------------*/
};

int	pr_scan_span2_adj( struct fmt_span2_adj *buf);
void *	pr_print_span2_adj( struct fmt_span2_adj *txt, struct prr1035t *pkt);


/*------------------------------------------------------*/
/*	ＷＨ用のラウンディングテキストフォーマット	*/
/*							*/
/* prr3017,prr3018					*/
/*------------------------------------------------------*/

struct fmt_round_tmg {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_round;		/* ラウンディンググループ数	*/
	BYTE	round_base[8][2];	/* ラウンディング排出基本時間	*/
/*-------------------------------------*/
};

int	pr_scan_round_tmg( struct fmt_round_tmg *buf);
void *	pr_print_round_tmg( struct fmt_round_tmg *txt, struct prr3017t *pkt);

#endif	/* __PRCMD_H__	*/
