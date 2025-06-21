/*======================================================================
 * File name    : romno.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/romno.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: romno.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1996, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		ROMNO_H
#define		ROMNO_H


/*----------------------------------------------------------
  ＲＣＵと通信が、 arcnet の時の通信フォーマットの定義
------------------------------------------------------------*/

struct arc_rcu_pkt1 {	/* RCU - NHIC(MHIC) アークネットの通信のテキスト構造 	*/
	unsigned char	src;			/* ソースアドレス		*/
	unsigned char	dst;			/* ディスティネーションアドレス	*/
/*-- 上記は、内部処理用で、実際のテキストには現れない --*/
	unsigned char	src_memo[4];		/* ソースネーム			*/
	unsigned char	dst_memo[4];		/* ディスティネーションネーム	*/
	unsigned char	cmd;			/* コマンド			*/
	unsigned char	ext_ptr[4];		/* 拡張ポインター		*/
	unsigned char	param;			/* パラメータの先頭		*/
};

/*----------------------------------------------------------
		  プロトタイプ
------------------------------------------------------------*/

void rom_no_req(
	int dev_no,
	int len,
	const unsigned char *buf,
	const unsigned char *non_accpt_cmd,
	const unsigned char *accpt_cmd
);

char 	*get_lib_id(void);

#endif
