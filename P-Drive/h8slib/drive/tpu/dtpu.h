/*----------------------------------------------------------------------*
 * File name	: dtpu.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/tpu/RCS/dtpu.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: dtpu.h $
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

#ifdef STORAGE
void (*interrupt_func[TPU_DEV_MAX])(void) = { (void *)0 };
const long psc_tbl[] = { 10000L, 40000L, 160000L, 640000L };
const void *tpu_adrs[] = { 	TPU0_ADRS, TPU1_ADRS, TPU2_ADRS, TPU3_ADRS, TPU4_ADRS, TPU5_ADRS };
#else
extern void (*interrupt_func[])(void);
extern const long psc_tbl[];
extern const void *tpu_adrs[];
#endif

