/*----------------------------------------------------------------------*
 * File name	: softint.h	/ software interrupt header					*
 * Original		: p:/include/biosif/rcs/softint.h 1.1					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/softint.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: softint.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* Function                                                  		*/
/*	ｱｾﾝﾌﾞﾗのｿﾌﾄｳｪｱｰ割り込みをＣのマクロとして定義。以下の様に	*/
/*	記述するが、return文が無いことによる戻り値のﾐｽﾏｯﾁのﾜｰﾆﾝｸﾞ	*/
/*	は無視すること。						*/
/*									*/
/*	discription ･･･							*/
/*		#include	"softint.h"				*/
/*		int sample_io()						*/
/*		{							*/
/*			soft_int( 0x21,		･･･ ﾍﾞｸﾀｰ番号		*/
/*				  0x4 );	･･･ 機能番号		*/
/*		}							*/
/*----------------------------------------------------------------------*/

#define soft_int(VECTOR,NUM) _builtin_trapa_svc(VECTOR,NUM)

