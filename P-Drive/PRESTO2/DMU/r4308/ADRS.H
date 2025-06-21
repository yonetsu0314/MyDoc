/*======================================================================
 * File name    : adrs.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: アドレス関係のプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/adrs.h 1.3 1970/01/01 00:00:00 ryoji Exp $
 *$Log: adrs.h $
 * リビジョン 1.3  1970/01/01  00:00:00  ryoji
 * 2007/03/10 05:00:00
 * 予約３桁仕様でも使用できるよう 予約番号の桁数をディップスイッチ(SW3-4)で
 * 可変できるようにする。桁数は、OFFの場合 2 で、ONの場合 3 となる。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  ryoji
 * 2003/12/17 19:00:00
 * 包装機でも使用できるよう RCU のノード番号をディップスイッチ(SW3-3)で
 * 可変できるようにする。ノード番号は、OFFの場合 90 で、ONの場合 9 となる。
 * 
 * リビジョン 1.1  1997/05/28  13:10:12  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef	_ADRS_H_
#define	_ADRS_H_

#define	RCU_MY_ADRS	100
#define RCU_ADRS_CCW	90
#define RCU_ADRS_BM		9
#define ICU_ADRS	80

int get_my_hif_adrs(void);
int get_rcu_my_adrs(void);
int get_rcu_adrs(void);
int get_pno_digit(void);
int is_double_mode(void);	/* HOST との通信が async の時 */
int get_hif_arc_master_adrs(void);
int set_hif_arc_master_adrs(int adrs);

#endif
