/*======================================================================
 * File name	: prr0029.h
 * Original	:
 *----------------------------------------------------------------------
 * Function	:
 *	ＤＺホッパ駆動特性データ構造
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/prcmd/RCS/prr0029.h 1.1 1970/01/01 00:00:00 kawamura Exp $
 *$Log: prr0029.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kawamura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *header: dzr209.hv  1.8  94/03/16 13:58:12  kabumoto  Exp
 *log: P:/INCLUDE/DZCMD/RCS/dzr209.hv
 *----------------------------------------------------------------------
 *		(c) Copyright 2003, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#if	!defined(PRR0029_H)
#define	PRR0029_H

#ifdef	PRR0013_H			/* 既に定義されている場合	*/
typedef enum prprm004e	PRR00290E;
typedef enum prr00131e	PRR00291E;
typedef enum prr00132e	PRR00292E;
typedef enum prr00133e	PRR00293E;
#define	HOPPER_BOUNDARY	HOPPER_DTH

#else
enum prr00290e {			/* ホッパの種類			*/
	HOPPER_PH,			/* ＰＨ				*/
	HOPPER_WH,			/* ＷＨ				*/
	HOPPER_BH,			/* ＢＨ				*/
	HOPPER_BOUNDARY,		/* ＴＨとの境界			*/
};
typedef enum prr00290e	PRR00290E;

enum prr00291e {			/* アクチュエータ		*/
	ACT_SM,				/* ステッピングモータ		*/
	ACT_CB,				/* クラッチ＆ブレーキ		*/
};
typedef enum prr00291e	PRR00291E;

enum prr00292e {			/* 励磁タイプ			*/
	PHASE_1 = 1,			/* １相				*/
	PHASE_2,			/* ２相				*/
	PHASE_12,			/* １－２相			*/
	PHASE_W12,			/* Ｗ－１－２相			*/
};
typedef enum prr00292e	PRR00292E;

enum prr00293e {			/* 停止条件			*/
	STOP_NON,			/* なし				*/
	STOP_LGHT,			/* 入光				*/
	STOP_DARK,			/* 遮光				*/
	STOP_EDGE,			/* エッジ			*/
	STOP_PLS,			/* パルス			*/
	STOP_PLS_LGHT,			/* パルス＋入光			*/
	STOP_PLS_DARK,			/* パルス＋遮光			*/
	STOP_PLS_EDGE,			/* パルス＋エッジ		*/
};
typedef enum prr00293e	PRR00293E;
#endif

enum prr00294e {			/* 停止励磁条件			*/
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

#define	MODE_STOP		(0x03)	/* bit1,bit0			*/
#define	MODE_STOP_SLIT		(0x00)	/* bit1=0 bit0=0 ( 0)		*/
#define	MODE_STOP_PULS		(0x01)	/* bit1=0 bit0=1 ( 1)		*/
#define	FLAG_SLOWSTEP		(0x0c)	/* bit3,bit2			*/
#define	FLAG_SLOWSTEP_ON	(0x00)	/* bit3=0 bit2=0 ( 0)		*/
#define	FLAG_SLOWSTEP_OFF	(0x04)	/* bit3=1 bit2=1 ( 1)		*/
#define	FLAG_POWER		(0x30)	/* bit5,bit4			*/
#define	FLAG_POWER_FULL		(0x10)	/* bit5=0 bit4=1 ( 1)		*/
#define	FLAG_POWER_HALF		(0x30)	/* bit1=1 bit4=1 (-1)		*/
#define	FLAG_REV		(0xc0)	/* bit7,bit6			*/
#define	FLAG_REV_CCW		(0x40)	/* bit7=0 bit6=1 ( 1)		*/
#define	FLAG_REV_CW		(0xc0)	/* bit7=1 bit6=1 (-1)		*/



typedef struct	prr00293t {	/*--------- mdp_section_data -----------*/
	short	amp_n;		/* 最終開閉位置(回転区間)[%]		*/
	short	sec_flag_power;	/* ｾｸｼｮﾝのﾊｰﾌﾊﾟﾜｰ設定 (0:full -1:half)	*/
	short	time_hold;	/* ﾎｰﾙﾄﾞ時間[ms]			*/
	short	num_ss_start;	/* 始動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾛｰｽﾀｰﾄｽﾃｯﾌﾟ数)	*/
	short	num_ss_stop;	/* 制動ｽﾛｰｽﾃｯﾌﾟ区間(ｽﾛｰｽﾄｯﾌﾟｽﾃｯﾌﾟ数)	*/
	short	max_pps;	/* 速度[pps]				*/
} PRR00293T;

typedef struct	prr00294t {		/* actuator_type == ACT_SM	*/
	PRR00292E	phase_type;	/* 励磁モード	3:1-2相		*/
	unsigned short	full_pow_tm;	/* 低速時フルパワー時間(4000)	*/
	unsigned short	ignore_pulse;	/* スリット不感パルス数		*/
	unsigned char	cam_slit;	/* スリット停止パルス数		*/
	enum prr00294e	hold_mode;	/* 停止励磁条件			*/
	unsigned char	brake_time;	/* ブレーキ時間	[10msec]	*/
	unsigned char	error_pulse;	/* エラー検出パルス数		*/
	unsigned char	num_section;	/* 駆動ﾊﾟﾀｰﾝ構成数(ｾｸｼｮﾝ数）	*/
	unsigned char	mode_flags;	/* 駆動ﾊﾟﾀｰﾝﾓｰﾄﾞﾌﾗｸﾞ		*/
	short		start_pps;	/* 自起動単位ﾊﾟﾙｽ数[pps]	*/
	short		fullstep;	/* 全ｽﾃｯﾌﾟ数			*/
	short		pitch;		/* (予備)			*/
	PRR00293T	msd[10];
} PRR00294T;

/*------------------------------------------------------*/
/*	クラッチ／ブレーキ駆動用パラメータパケット	*/
/*							*/
/*------------------------------------------------------*/

typedef struct	prr00295t {		/* actuator_type == ACT_CB	*/
	unsigned short	osc;		/* 基本ﾊﾟﾙｽ時間(1us単位)	*/
	unsigned short	cl_drive_tm;	/* CL駆動時間			*/
	unsigned char	cl_on_delay;	/* CLオン遅延時間		*/
	unsigned char	br_on_delay;	/* BRオン遅延時間		*/
	unsigned char	br_time;	/* BRオン時間(10ms単位)		*/
} PRR00295T;

/*------------------------------------------------------*/
/*	ＤＺホッパ駆動特性パラメータパケット		*/
/*							*/
/*------------------------------------------------------*/

typedef union	prr00290t {
	struct prr00294t	sm;	/* ステップ･モーター駆動用	*/
	struct prr00295t	cb;	/* クラッチ／ブレーキ駆動用	*/
} PRR00290T;

typedef struct	prr0029t {
	unsigned char	nof_head;	/* number of head		*/
	unsigned long	hop_assign_ptn;	/* Hopper assign pattern	*/
	unsigned short	open_time;	/* open time			*/
	PRR00290E	hopper_id;	/* hopper id			*/
	unsigned char	hopper_no;	/* hopper number		*/
	PRR00293E	stop_mode;	/* 停止条件			*/
	PRR00291E	actuator_type;	/* アクチュエータタイプ		*/
	union prr00290t	cpx;		/* 複合駆動パラメータ		*/
} PRR0029T;

#if	defined(STORAGE)
struct prr0029t	prr0029s;
#else
#if	!defined(NOEXTERN)
extern struct prr0029t	prr0029s;
#endif
#endif

#endif
