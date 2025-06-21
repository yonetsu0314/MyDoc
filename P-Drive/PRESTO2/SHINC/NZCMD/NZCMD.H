/*======================================================================
 * File name    : nzcmd.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	コマンドサマリー
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/nzcmd/RCS/nzcmd.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: nzcmd.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: p:/include/nzcmd/rcs/nzcmd.h 1.5 1970/01/01 00:00:00 kawamura
 *Log: nzcmd.h
 * リビジョン 1.5  1970/01/01  00:00:00  kawamura
 * 2002/08/01  16:14
 *----------------------------------------------------------------------
 *		(c) Copyright 1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#ifndef	__NZCMD_H__
#define	__NZCMD_H__

/*
// CCW-NZ 拡張コマンドおよび拡張パラメータを定義する
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
/*    │SUM 計算範囲		    　                      │          */

/* PTRn は次のパラメータの CMD を起点とした開始位置を示す。             */
/* 次のパラメータがない場合は 0 とする。                                */

#include	<custom\typedefs.h>		/* Show xlib		*/

typedef struct fmt_nzhd {			/* 先頭のヘッダー部	*/
	BYTE	srcno[4];			/* 転送元計量機number	*/
	BYTE	dstno[4];			/* 転送先	"	*/
} FMT_NZHD;

#define	PRMP	4				/* ポインターのサイズ	*/

#ifndef	FD_SPEC_CMD
#define	FD_SPEC_CMD	('e')		/* フィーダ駆動特性設定	*/
#endif
#ifndef	INFO_REQ_CMD
#define	INFO_REQ_CMD	('.')		/* 各種データ要求	*/
#endif
#ifndef	INFO_CMD
#define	INFO_CMD	('/')		/* 各種データ応答	*/
#endif
#ifndef	DIAGDATA_CMD
#define	DIAGDATA_CMD	('u')		/* 秤診断データ応答 	*/
#endif
#ifndef	CBRSLT_CMD
#define	CBRSLT_CMD	('E')		/* 組み合わせ計算結果	*/
#endif
#ifndef	SCALE_CMD
#define	SCALE_CMD	('\\')		/* 秤データ応答	*/
#endif
#ifndef	P_RSLT_CMD
#define	P_RSLT_CMD	('O')		/* 親ヘッド計量結果	*/
#endif
#ifndef	LOGIC_REQ_CMD
#define	LOGIC_REQ_CMD	('0')		/* パラレルポートデータ要求	*/
#endif
#ifndef	LOGIC_CMD
#define	LOGIC_CMD	('1')		/* パラレルポートデータ応答	*/
#endif
#ifndef	ARC_REQ_CMD
#define	ARC_REQ_CMD	('2')		/* 重量波形データ要求	*/
#endif
#ifndef	ARC_CMD
#define	ARC_CMD		('3')		/* 重量波形データ応答	*/
#endif
#ifndef	RES_CYC_REQ_CMD
#define	RES_CYC_REQ_CMD	('4')		/* フィーダ共振周期要求	*/
#endif
#ifndef	RES_CYC_CMD
#define	RES_CYC_CMD	('5')		/* フィーダ共振周期応答	*/
#endif
#ifndef	RES_CYC_DET_CMD
#define	RES_CYC_DET_CMD	('6')		/* フィーダ共振周期検出開始	*/
#endif
#ifndef	RES_CYC_STP_CMD
#define	RES_CYC_STP_CMD	('7')		/* フィーダ共振周期検出中止	*/
#endif
#ifndef	DRV_CYC_PRM_CMD
#define	DRV_CYC_PRM_CMD	('8')		/* フィーダ駆動周期設定	*/
#endif
#ifndef	NAT_FRQ_PRM_CMD
#define	NAT_FRQ_PRM_CMD	('9')		/* フィーダ共振周期設定	*/
#endif

/*------------------------------------------------------*/
/*	秤診断データテキストフォーマット		*/
/*							*/
/* nzi001,nzi002					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[3];		/* ヘッドステータス		*/
} NI0010;

struct fmt_diag_data_nz {
	BYTE	cmd;			/* command = 'u'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	NI0010	wh_3rd[32];		/* 3段目重量&ステータス[nof_wh]	*/
	NI0010	wh_4th[32];		/* 4段目重量&ステータス[nof_wh]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	NI0010	df[8];			/* WH重量&ステータス[nof_df]	*/
	BYTE	nof_afv;		/* ＡＦＶセル数			*/
	BYTE	afv_out[8][4];		/* ＡＦＶセル出力[nof_afv]	*/
/*-------------------------------------*/
};

int	nz_scan_diag_data(struct fmt_diag_data_nz *buf);
void *	nz_print_diag_data(struct fmt_diag_data_nz *txt, struct nzi001t *pkt);

/*------------------------------------------------------*/
/*	組み合わせ計算結果テキストフォーマット		*/
/*							*/
/* nzi003,nzi004					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	flag;			/* 演算結果フラグ		*/
	BYTE	weight[8];		/* 組み合わせ重量		*/
	BYTE	count[4];		/* 組み合わせ個数		*/
} N8;

struct fmt_cb_result_nz {
	BYTE	cmd;			/* command = 'E'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	N8	sect[8];		/* N8 paramter[nof_sect]	*/
	N8	ch;			/* N8   "			*/
	BYTE	slv_targ_wt[8];		/* スレーブ機目標重量		*/
	BYTE	slv_targ_cnt[4];	/* スレーブ機目標個数		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	BYTE	wh_status[32][3];	/* WHステータス[nof_wh]		*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	BYTE	bh_status[32][3];	/* WHステータス[nof_bh]		*/
/*-------------------------------------*/
};

int	nz_scan_cb_result(struct fmt_cb_result_nz *buf);
void *	nz_print_cb_result(struct fmt_cb_result_nz *txt, struct nzi003t *pkt);

/*------------------------------------------------------*/
/*	秤データのテキストフォーマット			*/
/*							*/
/* nzi011,nzi012					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* ヘッド重量			*/
	BYTE	status[3];		/* ヘッドステータス		*/
} N9;

struct fmt_head_weight_nz {
	BYTE	cmd;			/* command = '\'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	min_wh_no[2];		/* 最小WH番号			*/
	BYTE	nof_wh[2];		/* 該当するchのWH数		*/
	N9	wh[32];			/* WH重量&ステータス[nof_wh]	*/
	BYTE	nof_bh[2];		/* 該当するchのBH数		*/
	N9	bh[32];			/* WH重量&ステータス[nof_bh]	*/
	BYTE	min_df_no;		/* 最小DF番号			*/
	BYTE	nof_df;			/* 該当するchのDF数		*/
	N9	df[8];			/* WH重量&ステータス[nof_df]	*/
/*-------------------------------------*/
};

int	nz_scan_head_weight(struct fmt_head_weight_nz *buf);
void *	nz_print_head_weight(struct fmt_head_weight_nz *txt, struct nzi011t *pkt);

/*------------------------------------------------------*/
/*	親ヘッド計算結果テキストフォーマット		*/
/*							*/
/* nzi029,nzi030					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	weight[4];		/* 重量				*/
	BYTE	wh_status[3];		/* WHステータス	 		*/
	BYTE	bh_status[3];		/* BHステータス 		*/
} NI291;

typedef struct {
	BYTE	nof_phead;		/* number of parent head	*/
	NI291	phead[2];		/* NI291 paramter[nof_phead]	*/
} NI292;

struct fmt_pandc_result_nz {
	BYTE	cmd;			/* command = 'O'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	ch_id;			/* channel number		*/
	BYTE	nof_head[2];		/* number of head		*/
	BYTE	nof_sect;		/* 該当するchのセクション数	*/
	NI292	sect[8];		/* NI292 paramter[nof_sect]	*/
/*-------------------------------------*/
};
int	nz_scan_pandc_result(struct fmt_pandc_result_nz *buf);
void *	nz_print_pandc_result(struct fmt_pandc_result_nz *txt, struct nzi029t *pkt);

/*------------------------------------------------------*/
/*	フィーダの駆動特性のテキストフォーマット	*/
/*							*/
/* nzr015,nzr016					*/
/*------------------------------------------------------*/

struct fmt_fd_spec_nz {
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
	BYTE	slow_start;		/* スロースタート		*/
/*-------------------------------------*/
};

int	nz_scan_fd_spec(struct fmt_fd_spec_nz *buf);
void *	nz_print_fd_spec(struct fmt_fd_spec_nz *txt, struct nzr015t *pkt);

/*------------------------------------------------------*/
/*	各種データの要求テキストフォーマット		*/
/*							*/
/* nzr025,nzr026					*/
/*------------------------------------------------------*/

struct fmt_req_info {
	BYTE	cmd;			/* command = '.'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	info_no[2];		/* データの種類			*/
	BYTE	info_desc[2];		/* 識別番号			*/
/*-------------------------------------*/
};

int	nz_scan_req_info(struct fmt_req_info *buf);
void *	nz_print_req_info(struct fmt_req_info *txt, struct nzr025t *pkt);

/*------------------------------------------------------*/
/*	各種データの応答テキストフォーマット		*/
/*							*/
/* nzi009,nzi010					*/
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

int	nz_scan_info(struct fmt_info *buf);
void *	nz_print_info(struct fmt_info *txt, struct nzi009t *pkt);

/*------------------------------------------------------*/
/*	ロジック波形データの要求テキストフォーマット	*/
/*							*/
/* nzr017,nzr018					*/
/*------------------------------------------------------*/

struct fmt_logic_parm {
	BYTE	cmd;			/* command = '0'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	logic_intvl[4];		/* サンプリング周期		*/
	BYTE	unit_no;		/* リレーユニット番号		*/
/*-------------------------------------*/
};

int	nz_scan_logic_parm(struct fmt_logic_parm *buf);
void *	nz_print_logic_parm(struct fmt_logic_parm *txt, struct nzr017t *pkt);

/*------------------------------------------------------*/
/*	ロジック波形データの応答テキストフォーマット	*/
/*							*/
/* nzi005,nzi006					*/
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

int	nz_scan_logic(struct fmt_logic *buf);
void *	nz_print_logic(struct fmt_logic *txt, struct nzi005t *pkt);

/*------------------------------------------------------*/
/*	重量波形データのパラメータテキストフォーマット	*/
/*							*/
/* nzr019,nzr020					*/
/*------------------------------------------------------*/

struct fmt_archive_parm {
	BYTE	cmd;			/* command = '2'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* WH番号			*/
	BYTE	type;			/* 重量データのタイプ		*/
/*-------------------------------------*/
};

int	nz_scan_archive_parm(struct fmt_archive_parm *buf);
void *	nz_print_archive_parm(struct fmt_archive_parm *txt, struct nzr019t *pkt);

/*------------------------------------------------------*/
/*	重量波形データの応答テキストフォーマット	*/
/*							*/
/* nzi007,nzi008					*/
/*------------------------------------------------------*/
typedef struct {
	BYTE	weight[4];		/* 重量データ			*/
	BYTE	status;			/* 重量ステータス		*/
} NI0070;

struct fmt_archive {
	BYTE	cmd;			/* command = '3'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	wh_no[2];		/* WH番号			*/
	BYTE	index[4];		/* データのインデックス		*/
	BYTE	nof_data[2];		/* データ数			*/
	NI0070	data[32];		/* データ			*/
/*-------------------------------------*/
};

int	nz_scan_archive(struct fmt_archive *buf);
void *	nz_print_archive(struct fmt_archive *txt, struct nzi007t *pkt);

/*------------------------------------------------------*/
/*	秤の計量特性拡張パラメータ			*/
/*	（安定判定時間パラメータ）			*/
/* nzr009,nzr010					*/
/*------------------------------------------------------*/

typedef struct {
	BYTE	stable_time[4];		/* stable time constant		*/
} NR0090;

typedef struct {
	BYTE	nof_sect;		/* number of section		*/
	NR0090	sect[8];
} NR0091;

struct fmt_wei_spec1_nz {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_ch;			/* number of channel		*/
	NR0091	ch[2];			/* NR0090 parameter of channel	*/
/*-------------------------------------*/
};

int	nz_scan_wei_spec1(struct fmt_wei_spec1_nz *buf);
void *	nz_print_wei_spec1(struct fmt_wei_spec1_nz *txt, struct nzr009t *pkt);

/*------------------------------------------------------*/
/*	フィーダ共振周期応答テキストフォーマット	*/
/*							*/
/* nzi051,nzi052					*/
/*------------------------------------------------------*/

struct fmt_resonance {
	BYTE	cmd;			/* command = '5'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_res_cycle[32][4];	/* RFの共振周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_res_cycle[8][4];	/* DFの共振周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_res_cycle[8][4];	/* CFの共振周期			*/
/*-------------------------------------*/
};

int	nz_scan_resonance(struct fmt_resonance *buf);
void *	nz_print_resonance(struct fmt_resonance *txt, struct nzi051t *pkt);

/*------------------------------------------------------*/
/*	フィーダ共振周期検出開始テキストフォーマット	*/
/*							*/
/* nzr051,nzr052					*/
/*------------------------------------------------------*/

struct fmt_resonance_detect {
	BYTE	cmd;			/* command = '6'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	rf_detect_ptn[8];	/* RF共振周期検出パターン	*/
	BYTE	df_detect_ptn[8];	/* DF共振周期検出パターン	*/
	BYTE	cf_detect_ptn[8];	/* CF共振周期検出パターン	*/
/*-------------------------------------*/
};

int	nz_scan_res_detect(struct fmt_resonance_detect *buf);
void *	nz_print_res_detect(struct fmt_resonance_detect *txt, struct nzr051t *pkt);

/*------------------------------------------------------*/
/*	フィーダ駆動周期設定テキストフォーマット	*/
/*							*/
/* nzr053,nzr054					*/
/*------------------------------------------------------*/

struct fmt_drv_cycle {
	BYTE	cmd;			/* command = '8'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_drv_cycle[32][4];	/* RFの駆動周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_drv_cycle[8][4];	/* DFの駆動周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_drv_cycle[8][4];	/* CFの駆動周期			*/
/*-------------------------------------*/
};

int	nz_scan_drv_cycle(struct fmt_drv_cycle *buf);
void *	nz_print_drv_cycle(struct fmt_drv_cycle *txt, struct nzr053t *pkt);

/*------------------------------------------------------*/
/*	フィーダ共振周期設定テキストフォーマット	*/
/*							*/
/* nzr055,nzr056					*/
/*------------------------------------------------------*/

struct fmt_natural_freq {
	BYTE	cmd;			/* command = '9'		*/
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_rf[2];		/* RFの数			*/
	BYTE	rf_natural_freq[32][4];	/* RFの共振周期			*/
	BYTE	nof_df[2];		/* DFの数			*/
	BYTE	df_natural_freq[8][4];	/* DFの共振周期			*/
	BYTE	nof_cf[2];		/* CFの数			*/
	BYTE	cf_natural_freq[8][4];	/* CFの共振周期			*/
/*-------------------------------------*/
};

int	nz_scan_natural_freq(struct fmt_natural_freq *buf);
void *	nz_print_natural_freq(struct fmt_natural_freq *txt, struct nzr055t *pkt);

/*------------------------------------------------------*/
/*	補助フィーダ共振周期応答テキストフォーマット	*/
/*							*/
/* nzi151,nzi152					*/
/*------------------------------------------------------*/

struct fmt_af_resonance {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_res_cycle[32][4];	/* AFの共振周期			*/
/*-------------------------------------*/
};

int	nz_scan_af_resonance(struct fmt_af_resonance *buf);
void *	nz_print_af_resonance(struct fmt_af_resonance *txt, struct nzi151t *pkt);

/*------------------------------------------------------*/
/*	補助フィーダ共振周期検出開始テキストフォーマット*/
/*							*/
/* nzr151,nzr152					*/
/*------------------------------------------------------*/

struct fmt_af_resonance_detect {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	af_detect_ptn[8];	/* AF共振周期検出パターン	*/
/*-------------------------------------*/
};

int	nz_scan_af_res_detect(struct fmt_af_resonance_detect *buf);
void *	nz_print_af_res_detect(struct fmt_af_resonance_detect *txt, struct nzr151t *pkt);

/*------------------------------------------------------*/
/*	補助フィーダ駆動周期設定テキストフォーマット	*/
/*							*/
/* nzr153,nzr154					*/
/*------------------------------------------------------*/

struct fmt_af_drv_cycle {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_drv_cycle[32][4];	/* AFの駆動周期			*/
/*-------------------------------------*/
};

int	nz_scan_af_drv_cycle(struct fmt_af_drv_cycle *buf);
void *	nz_print_af_drv_cycle(struct fmt_af_drv_cycle *txt, struct nzr153t *pkt);

/*------------------------------------------------------*/
/*	補助フィーダ共振周期設定テキストフォーマット	*/
/*							*/
/* nzr155,nzr156					*/
/*------------------------------------------------------*/

struct fmt_af_natural_freq {
	BYTE	prm_pnt[4];		/* pointer to option parameter	*/
/*---------- parameter ----------------*/
	BYTE	nof_af[2];		/* AFの数			*/
	BYTE	af_natural_freq[32][4];	/* AFの共振周期			*/
/*-------------------------------------*/
};

int	nz_scan_af_natural_freq(struct fmt_af_natural_freq *buf);
void *	nz_print_af_natural_freq(struct fmt_af_natural_freq *txt, struct nzr155t *pkt);
#endif	/* __NZCMD_H__	*/
