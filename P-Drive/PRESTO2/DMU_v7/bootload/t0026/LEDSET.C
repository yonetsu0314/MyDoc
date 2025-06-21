/*======================================================================
 * File name    : ledset.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		ＬＥＤ制御用関数
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/ledset.c 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ledset.c $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * 電断対応ファイルシステム搭載
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/09 18:30
 * バックアップファイル復帰中におけるLED表示パターンを新規に作成し、
 * 表示名を整理する
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/09 17:00
 * エラー発生時のLEDパターンを修正する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * バックアップファイル作成中、復帰中時のLEDパターンを変更する
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/08 11:00
 * コメントを修正する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/02 9:30
 * LEDパターンのセット
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_LEDSET_C_

#include	<kernel.h>
#include	<sh7615\bootprm.h>
#include	<sh7615\7615.h>
#include	"ledset.h"

/*---------------------------------------------------------------
				ＬＥＤパターンセット
----------------------------------------------------------------*/
static int led_mode = 0;
void set_ledmode(int mode)
{
	led_mode = mode;
}

/*---------------------------------------------------------------
				ＬＥＤ制御タスク
----------------------------------------------------------------*/
TASK LedTask(void)
{
	int	i=0,j=1;
	unsigned short a;

	for(;;) {
		dly_tsk((RELTIM)100/MSEC);
		switch (led_mode) {
			case BOOTLOADER:				/* ブートローダー通常状態 */
				a = (unsigned short)(0x0001<<(i));
				PA.DR.BIT.B2 = (a&0x0001)?0:1;
				PA.DR.BIT.B0 = (a&0x0002)?0:1;
				PA.DR.BIT.B4 = (a&0x0004)?0:1;
				PA.DR.BIT.B1 = (a&0x0008)?0:1;
				i += j;
				if(i>=3) j = -1;
				if(i<=0) j = 1;
				break;
			case MAKE_BACKUP:				/* バックアップファイル作成中 */
				a = (unsigned short)(0x0001<<(i));
				PA.DR.BIT.B2 = 0;
				PA.DR.BIT.B0 = (a&0x0001)?0:1;
				PA.DR.BIT.B4 = (a&0x0002)?0:1;
				PA.DR.BIT.B1 = (a&0x0004)?0:1;
				i += j;
				if(i>=2) j = -1;
				if(i<=0) j = 1;
				break;
			case FPGA_DOWNLOADING:			/* FPGAダウンロード中 */
				PA.DR.BIT.B2 = ~PA.DR.BIT.B2;
				PA.DR.BIT.B0 = 1;
				PA.DR.BIT.B4 = 1;
				PA.DR.BIT.B1 = 1;
				break;
			case WCU_DOWNLOADING:			/* WCUダウンロード中 */
				PA.DR.BIT.B2 =0;
				PA.DR.BIT.B0 = ~PA.DR.BIT.B0;
				PA.DR.BIT.B4 = 1;
				PA.DR.BIT.B1 = 1;
				break;
			case DMU_DOWNLOADING:			/* DMUダウンロード中 */
				PA.DR.BIT.B2 = 0;
				PA.DR.BIT.B0 = 0;
				PA.DR.BIT.B4 = ~PA.DR.BIT.B4;
				PA.DR.BIT.B1 = 1;
				break;
			case FUNC_ERROR:				/* エラー発生時 */
				a = (unsigned short)~PA.DR.BIT.B2;
				PA.DR.BIT.B2 = a;
				PA.DR.BIT.B0 = a;
				PA.DR.BIT.B4 = a;
				PA.DR.BIT.B1 = a;
				break;
			case RET_BACKUP:				/* バックアップファイル復帰中 */
				dly_tsk((RELTIM)200/MSEC);
				a = (unsigned short)~PA.DR.BIT.B2;
				PA.DR.BIT.B2 = a;
				PA.DR.BIT.B0 = a;
				PA.DR.BIT.B4 = ~a;
				PA.DR.BIT.B1 = ~a;
				break;
			default:
				break;
		}
	}
}

