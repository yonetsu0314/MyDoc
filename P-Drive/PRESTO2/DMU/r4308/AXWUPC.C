/*======================================================================
 * File name    : axwupc.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: AUX の受信タスクのウェイクアップをさせるキャラクター
 *		  の設定
 *                この関数は、dmps4.lib をリンクした時のみ使用可
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/axwupc.c 1.2 1998/09/01 13:03:25 yonetsu Exp $
 *$Log: axwupc.c $
 * リビジョン 1.2  1998/09/01  13:03:25  yonetsu
 * stat の初期化忘れを修正
 * 
 * リビジョン 1.1  1998/08/26  18:31:05  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-1998, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#define _AXWUPC_C_

#include	<string.h>
#include	<rxif\rx116.h>
#include	<drive\mpsdrva.h>
#include	"cmfhdl.h"

static char wupchar[2][16];

int set_wup_char(
	int	dev_no,
	unsigned char *wupc
	)
{
	int	stat = 0;

	switch(dev_no) {
	    case CON_DN0:	
	    	memcpy(wupchar[0], wupc, sizeof(wupchar[0]));
	    	break;
	    case CON_DN1:
	    	memcpy(wupchar[1], wupc, sizeof(wupchar[1]));
	    	break;
		default:
	    	stat = -1;
			break;
	}
	return stat;
}

int aux_wup_char_set(
	int dev_no,
	unsigned char *wupc
	)
{
	int	stat = 0;

	switch(dev_no) {
	    case CON_DN0:
	    case CON_DN1:
	    	break;
	    default:
	    	stat = -1;
			break;
	}
#if 1
	if(stat != -1)		return 	set_wup_char(dev_no,wupc);
#endif
	return stat;
}

int con0_text_check(
	char	*text,
	int		len
	)
{
	int	err = 0,i;
	static int	cnt;
	static char	rx_buff[RCU_MES_MAX];

	rx_buff[cnt] = text[0];
	cnt++;
	for(i=0; i<16; i++) {
		if(wupchar[0][i] == text[0] && wupchar[0][i]) {
			memcpy(text, rx_buff, (size_t)cnt);
			err = cnt;
			cnt = 0;
			return err;
		}
	}
	if(cnt >= RCU_MES_MAX)	cnt = 0;
	len = len;
	return err;
}

int con1_text_check(
	char	*text,
	int		len
	)
{
	int	err = 0,i;
	static int	cnt;
	static char	rx_buff[RCU_MES_MAX];

	rx_buff[cnt] = text[0];
	cnt++;
	for(i=0; i<16; i++) {
		if(wupchar[1][i] == text[0] && wupchar[1][i]) {
			memcpy(text, rx_buff, (size_t)cnt);
			err = cnt;
			cnt = 0;
			return err;
		}
	}
	if(cnt >= RCU_MES_MAX)	cnt = 0;
	len = len;
	return err;
}
