/*======================================================================
 * File name    : dzr209.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＺホッパ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/dzcmd/RCS/dzr209.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dzr209.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *Header: dzr209.hv  1.8  94/03/16 13:58:12  kabumoto
 *Log: P:/INCLUDE/DZCMD/RCS/dzr209.hv
 * Revision 1.8  94/03/16 13:58:12  kabumoto
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(DZR209_H)
#define	DZR209_H

#ifdef	RZR013_H			/* 既にに定義されている場合	*/
typedef enum rzr0130e	DZR2090E;
typedef enum rzr0131e	DZR2091E;
typedef enum rzr0132e	DZR2092E;
typedef enum rzr0133e	DZR2093E;
#define	HOPPER_BOUNDARY	HOPPER_DTH

#else
enum dzr2090e {				/* ホッパの種類			*/
	HOPPER_PH,			/* ＰＨ				*/
	HOPPER_WH,			/* ＷＨ				*/
	HOPPER_BH,			/* ＢＨ				*/
	HOPPER_BOUNDARY,		/* ＴＨとの境界			*/
};
typedef enum dzr2090e	DZR2090E;

enum dzr2091e {				/* アクチュエータ		*/
	ACT_SM,				/* ステッピングモータ		*/
	ACT_CB,				/* クラッチ＆ブレーキ		*/
};
typedef enum dzr2091e	DZR2091E;

enum dzr2092e {				/* 励磁タイプ			*/
	PHASE_1 = 1,			/* １相				*/
	PHASE_2,			/* ２相				*/
	PHASE_12,			/* １－２相			*/
	PHASE_W12,			/* Ｗ－１－２相			*/
};
typedef enum dzr2092e	DZR2092E;

enum dzr2093e {				/* 停止条件			*/
	STOP_NON,			/* なし				*/
	STOP_LGHT,			/* 入光				*/
	STOP_DARK,			/* 遮光				*/
	STOP_EDGE,			/* エッジ			*/
	STOP_PLS,			/* パルス			*/
	STOP_PLS_LGHT,			/* パルス＋入光			*/
	STOP_PLS_DARK,			/* パルス＋遮光			*/
	STOP_PLS_EDGE,			/* パルス＋エッジ		*/
};
typedef enum dzr2093e	DZR2093E;
#endif

enum dzr2094e {				/* 停止励磁条件			*/
	HOLD_MODE0,			/* 全励磁ハーフパワー		*/
	HOLD_MODE1,			/* １相励磁ハーフパワー		*/
	HOLD_MODE2,			/* ２相励磁ハーフパワー		*/
	HOLD_MODE3,			/* 予約				*/
	HOLD_MODE4,			/* 全励磁ダブルハーフパワー	*/
	HOLD_MODE5,			/* １相励磁ダブルハーフパワー	*/
	HOLD_MODE6,			/* ２相励磁ダブルハーフパワー	*/
	HOLD_MODE7,			/* 全励磁フルパワー		*/
	HOLD_MODE8,			/* １相励磁フルパワー		*/
	HOLD_MODE9,			/* ２相励磁フルパワー		*/
};

/*------------------------------------------------------*/
/*	ステップ･モータ駆動用パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef struct dzr2091t {		/*------- mdp_mode_flags -------*/
	char	     mode_stop	   :2 ;	/* 0:slit  1:puls	bit 0,1	*/
	char	     flag_slowstep :2 ;	/* 0:on    1:off	bit 2,3	*/
	signed char  flag_power	   :2 ;	/* 1:full -1:half	bit 4,5 */
	signed char  flag_rev	   :2 ;	/* 1:ccw  -1:cw		bit 6,7	*/
} DZR2091T;

typedef struct	dzr2093t {	/*--------- mdp_section_data -----------*/
	int	amp_n;		/* 最終開閉位置(回転区間)[%]		*/
	int	sec_flag_power;	/* ｾｸｼｮﾝのﾊｰﾌﾊﾟﾜｰ設定 (0:full -1:half)	*/
	int 	time_hold;	/* ﾎｰﾙﾄﾞ時間[ms]			*/
	int	num_ss_start;	/* 始動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾛｰｽﾀｰﾄｽﾃｯﾌﾟ数)	*/
	int	num_ss_stop;	/* 制動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾛｰｽﾄｯﾌﾟｽﾃｯﾌﾟ数)	*/
	int	max_pps;	/* 速度[pps]				*/
} DZR2093T;

typedef struct	dzr2094t {		/* actuator_type == ACT_SM	*/
	DZR2092E	phase_type;	/* 励磁モード	3:1-2相		*/
	unsigned short	full_pow_tm;	/* 低速時フルパワー時間(4000)	*/
	unsigned short	ignore_pulse;	/* スリット不感パルス数	 	*/
       	unsigned char	cam_slit;	/* スリット停止パルス数 	*/
	enum dzr2094e	hold_mode;	/* 停止励磁条件			*/
	unsigned char	brake_time;	/* ブレーキ時間	      [10msec]	*/
	unsigned char	error_pulse;	/* エラー検出パルス数		*/
	unsigned char	num_section;	/* 駆動ﾊﾟﾀｰﾝ構成数(ｾｸｼｮﾝ数）	*/
	struct dzr2091t	mode_flags;	/* 駆動ﾊﾟﾀｰﾝﾓｰﾄﾞﾌﾗｸﾞ		*/
	int		start_pps;	/* 自起動単位ﾊﾟﾙｽ数[pps]	*/
	int		fullstep;	/* 全ｽﾃｯﾌﾟ数			*/
	int		pitch;		/* (予備)			*/
	struct dzr2093t	msd[10];
} DZR2094T;

/*------------------------------------------------------*/
/*	クラッチ／ブレーキ駆動用パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct	dzr2095t {		/* actuator_type == ACT_CB	*/
	unsigned short	osc;		/* 基本ﾊﾟﾙｽ時間(1us単位)	*/
	unsigned short	cl_drive_tm;	/* CL駆動時間			*/
 	unsigned char	cl_on_delay;	/* CLオン遅延時間		*/
	unsigned char	br_on_delay;	/* BRオン遅延時間		*/
	unsigned char	br_time;	/* BRオン時間(10ms単位)		*/
} DZR2095T;

/*------------------------------------------------------*/
/*	ＤＺホッパ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef union	dzr2090t {
	struct dzr2094t	sm;		/* ステップ･モーター駆動用	*/
	struct dzr2095t	cb;		/* クラッチ／ブレーキ駆動用	*/
} DZR2090T;

typedef struct	dzr209t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	hop_assign_ptn;	/* Hopper assign pattern
					   [nof_head/4]			*/
	unsigned short	open_time;	/* open time			*/
	DZR2090E	hopper_id;	/* hopper id			*/
	unsigned char	hopper_no;	/* hopper number		*/
	DZR2093E	stop_mode;	/* 停止条件			*/
	DZR2091E	actuator_type;	/* アクチュエータタイプ		*/
	union dzr2090t	cpx;		/* 複合駆動パラメータ		*/
} DZR209T;

#if	defined(STORAGE)
struct dzr209t	dzr209s;
#else
#if	!defined(NOEXTERN)
extern struct dzr209t	dzr209s;
#endif
#endif

#endif
