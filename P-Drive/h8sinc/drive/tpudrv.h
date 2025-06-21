/*----------------------------------------------------------------------*
 * File name	: dtpu.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/drive/RCS/tpudrv.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: tpudrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2000, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*		TCU ドライバー用ヘッダーファイル								*/
/*----------------------------------------------------------------------*/

#include	<bios2357\h8stpu.h>

/*----------------------------------------------------------------------*/
/* タイマーの初期化														*/
/*																		*/
/* 引き数																*/
/*		TPU 番号 0-5													*/
/*		割り込み周期													*/
/*		割り込み処理関数												*/
/* 戻り値																*/
/*		 0	･･･ 正常終了												*/
/*		 -1	･･･ 初期化失敗												*/
/*----------------------------------------------------------------------*/

#define BASE_CLOCK	196608

enum {
	TPU_DEV0,
	TPU_DEV1,
	TPU_DEV2,
	TPU_DEV3,
	TPU_DEV4,
	TPU_DEV5,
	TPU_DEV_MAX
};

#ifdef STORAGE
void (*interrupt_func[TPU_DEV_MAX])(void) = { (void *)0 };
const long psc_tbl[] = { 10000L, 40000L, 1600000L, 640000L };
const void *tpu_adrs[] = { 	TPU0_ADRS, TPU1_ADRS, TPU2_ADRS, TPU3_ADRS, TPU4_ADRS, TPU5_ADRS };
#else
extern void (*interrupt_func[])(void);
extern const long psc_tbl[];
extern const void *tpu_adrs[];
#endif

#ifndef	TPUINT_C
void	tgi0a_int_hdr(void);
void	tgi1a_int_hdr(void);
void	tgi2a_int_hdr(void);
void	tgi3a_int_hdr(void);
void	tgi4a_int_hdr(void);
void	tgi5a_int_hdr(void);
#endif

int tpu_tmr_sys_init(int tpu_no,unsigned short ival_t, void (*i_func)(void));
int tpu_tmr_ival_chg(int tpu_no,unsigned short	ival_t);

