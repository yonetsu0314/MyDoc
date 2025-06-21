/*----------------------------------------------------------------------*
 * File name	: cntsini.c		/ system initialize						*
 *----------------------------------------------------------------------*
 *$Header: p:/h8slib/drive/rsh/RCS/cntsini.c 1.2 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: cntsini.c $
 * リビジョン 1.2  1970/01/01  00:00:00  yonetsu
 * BIOS の INCLUDE フォルダーの変更
 * 
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
/*		リフレッシュコントローラを使用したカウンター					*/
/*----------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<bios2357\h8sbsc.h>
#include	<drive\cntdrv.h>

#define		STORAGE
#include	"cnt.h"

/*----------------------------------------------------------------------*/
/* システムカウンターの初期化											*/
/*																		*/
/* 引き数																*/
/*		time_base 	  : カウンターのインクリメント周期					*/
/*		no_of_counter :	使用するカウンターの数							*/
/* 戻り値																*/
/*		 0	･･･ 正常終了												*/
/*		 -1	･･･ 初期化失敗												*/
/*----------------------------------------------------------------------*/
static const long tbl[] = { 20L, 80L, 320L, 1280L, 5120L, 20480L, 40960L };

int counter_sys_init(
	int time_base,			/* 1 msec to 50 msec */
	int	no_of_counter		/* No. of counter	 */
	)
{
	int	i;
	
	BSC.DRAMCR.BIT.CMIE  	= 0;	/* CMF フラグによる割り込み禁止 	*/
	syscnt.enable_flag = 0;
	if((syscnt.cnt = calloc(no_of_counter,sizeof(int)))== NULL)		return(-1);
	if((syscnt.enable = calloc(no_of_counter,sizeof(int)))== NULL)	return(-1);

	BSC.DRAMCR.BYTE = 0;			/* Refresh Control 禁止 	*/
	BSC.RTCNT = 0;					/* Couter Clear				*/
	for(i=0; i<7 ; i++) {
		if((BASE_CLOCK * (long)time_base / tbl[i] ) < 250L ) {
			syscnt.enable_flag = no_of_counter;
			BSC.RTCOR = (unsigned char)((BASE_CLOCK * (long)time_base) / tbl[i]);
			BSC.DRAMCR.BIT.CKS = (unsigned char)(i+1);
			BSC.DRAMCR.BIT.CMIE  	= 1;	/* CMF フラグによる割り込み許可 	*/
			return 0;
		}
	}
	return -1;
}
