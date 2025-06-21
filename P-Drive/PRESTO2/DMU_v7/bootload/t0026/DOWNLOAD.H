/*======================================================================
 * File name    : download.h
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		プログラムダウンロード用関数プロトタイプ
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/download.h 2.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: download.h $
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * wcu_boot_cmdsendに関するプロトタイプ宣言を削除する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:00
 * ブートローダーの書き換えができるように関数を追加する
 * ダウンロード時のアドレスが合うようにcomWCU_F, comWCU_Lを変更する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/02/13 12:30
 * send_wcu_boot_cmd の追加
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/02/06 17:00
 * WCUブートローダー移行許可関数の追加に対応
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/22 21:00
 * comWCU_L のプロトタイプ宣言を追加する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/08 14:00
 * ダウンロード用関数のプロトタイプを変更する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * ダウンロード用関数のプロトタイプを変更する
 * 2004/01/07 16:00
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * ダウンロード用関数の追加に対応
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/05 16:00
 * ダウンロード用関数の追加に対応
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#ifndef	_DOWNLOAD_H_
#define	_DOWNLOAD_H_

void send_wcu_boot_cmd(void);
void send_WCU_command(char cmd);
int wait_WCU_ack(void);
int comWCU_N(char cmd);
int comWCU_K(void);
int comWCU_A(void);
int comWCU_F(char cmd, char fname[]);
int comWCU_J(char cmd);
int comWCU_L(char cmd);
int comWCU_U(void);
int comWCU_R(void);

#endif	/* _DOWNLOAD_H_ */
