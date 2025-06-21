/*======================================================================
 * File name    : tagdef.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: タグの定義
 *----------------------------------------------------------------------
 * $Header: p:/nhic/main/ccw/rcs/tagdef.h 1.1.4102.2 1998/05/15 11:02:07 yonetsu Exp $
 * $Log: tagdef.h $
 * リビジョン 1.1.4102.2  1998/05/15  11:02:07  yonetsu
 * TAG4_C1,TAG4_C2 を追加
 * 
 * リビジョン 1.1.4102.1  1997/05/28  13:16:24  yonetsu
 * 定義の変更
 * 
 * リビジョン 1.1  1996/11/18  20:52:03  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef		TAGDEF_H
#define		TAGDEF_H

enum {
	/* Host Command */

	TAG_HST2_C1 = 0x11,	/* 0x11 */
	TAG_HST2_C2,		/* 0x12 */
	TAG_HST3_C1,		/* 0x13 */
	TAG_HST3_C2,		/* 0x14 */
	TAG_ETC2_C1,		/* 0x15 */
	TAG_ETC2_C2,		/* 0x16 */
	TAG_ETC3_C1,		/* 0x17 */
	TAG_ETC3_C2,		/* 0x18 */
	TAG_ETC4_C1,		/* 0x19 */
	TAG_ETC4_C2,		/* 0x1a */
	TAG_HST1_C1,		/* 0x1b 	ESC 	*/
	TAG_HST1_C2,		/* 0x1c 	ESC + 1	*/
	TAG_ETC1_C1,		/* 0x1d */
	TAG_ETC1_C2,		/* 0x1e */
};

#define	TAG1_C1		TAG_HST1_C1	/* std host */
#define TAG1_C2		TAG_HST1_C2

#define TAG2_C1		TAG_HST2_C1	/* dde host */
#define TAG2_C2		TAG_HST2_C2

#define TAG3_C1		TAG_ETC1_C1	
#define TAG3_C2		TAG_ETC1_C2

#define TAG4_C1		TAG_HST3_C1	/* modem host */
#define TAG4_C2		TAG_HST3_C2

#endif
