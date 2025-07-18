/*======================================================================
 * File name    : adrs.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: アドレス関係の設定、取得
 *----------------------------------------------------------------------
 * $Header: p:/nhic/main/ccw/rcs/adrs.c 1.4 1970/01/01 00:00:00 ryoji Exp $
 * $Log: adrs.c $
 * リビジョン 1.4  1970/01/01  00:00:00  ryoji
 * 2007/03/10 05:00:00
 * 予約３桁仕様でも使用できるよう 予約番号の桁数をディップスイッチ(SW3-4)で
 * 可変できるようにする。桁数は、OFFの場合 2 で、ONの場合 3 となる。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  ryoji
 * 2003/12/17 19:00:00
 * 包装機でも使用できるよう RCU のノード番号をディップスイッチ(SW3-3)で
 * 可変できるようにする。ノード番号は、OFFの場合 90 で、ONの場合 9 となる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  yonetsu
 * 2000/02/07 10:07
 * RCU と MHIC 間の MHIC のノード番号をディップスイッチで可変できるようにする。
 * ノード番号は、100 から 103 までとする。
 * 
 * リビジョン 1.1  1997/05/28  10:55:29  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#define	_ADRS_C_

#include	<rxif\rx116.h>
#include 	"swi001.h"
#include	"adrs.h"
#include	"txfnc_id.h"
#include	"fnctbl.h"
#include	"hif001.h"

static int hif_arc_master_adrs = HIF_ARC_MASTER_ADRS_DEFAULT;

int get_my_hif_adrs(void)
{
	static int init=1;
	static int adrs = 1;

	if(init) {
		init = 0;
		adrs = rd_sw(SW1);
		if(adrs == 0)				adrs = 1;
		else if(!is_arcnet(TX_EXT_STD_HOST))	adrs &= 0x3f;	/* async */
	}

	return adrs;
}

int is_double_mode(void)	/* HOST との通信が async の時 */
{
	static int init=1;
	static int mode = 0;

	if(init) {
		init = 0;
		mode = rd_sw(SW1) & 0x80;
	}

	return mode;
}


int get_rcu_my_adrs(void)
{
	static int init=1;
	static int adrs = RCU_MY_ADRS;

	if(init) {
		init = 0;
		adrs = (rd_sw(SW1) & 0x03) + RCU_MY_ADRS;
	}

	return adrs;
}

int get_rcu_adrs(void)
{
	if (rd_sw(SW1) & 0x04)	return RCU_ADRS_BM;
	else			return RCU_ADRS_CCW;
}

int get_pno_digit(void)
{
	static	int	pno_digit = -1;

	if ((pno_digit != 2) && (pno_digit != 3)) {	
		if (rd_sw(SW1) & 0x08)	return (pno_digit = 3);	/* 3桁 */
		else			return (pno_digit = 2);	/* 2桁 */
	} else {
		return pno_digit;
	}
}

int get_hif_arc_master_adrs(void)
{
	return hif_arc_master_adrs;
}

int set_hif_arc_master_adrs(int adrs)
{
	hif_arc_master_adrs = adrs;
	return 	hif_arc_master_adrs;
}
