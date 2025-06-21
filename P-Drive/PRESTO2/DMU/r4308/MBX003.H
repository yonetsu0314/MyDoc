/*======================================================================
 * File name    : mbx003.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	汎用メイルボックス用関数のプロトタイプ。
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/mbx003.h 1.2 1998/05/15 10:20:40 yonetsu Exp $
 *$Log: mbx003.h $
 * リビジョン 1.2  1998/05/15  10:20:40  yonetsu
 * 関数 mbx_rcv_msg_clr のプロトタイプを追加
 * 
 * リビジョン 1.1  1997/05/28  15:40:34  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1996, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#ifndef		MBX003_H
#define		MBX003_H

enum blksz {
	BLK_SIZE1 = 1,
	BLK_SIZE2,
	BLK_SIZE3,
	BLK_SIZE4,
};

int mbx_snd_msg( void *, int, int, union aap, union aap);
int mbx_rcv_msg( void *, int, union aap);
int mbx_rcv_msg_t( void *, int, union aap, long);
void mbx_rcv_msg_clr(union aap);

#endif
