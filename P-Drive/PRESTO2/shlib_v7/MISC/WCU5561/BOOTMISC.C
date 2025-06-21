/*======================================================================
 * File name    : bootmisc.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/wcu5561/rcs/bootmisc.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: bootmisc.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * wcu_jump_to_resetをBIOS分離
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * debstr(),debout()を追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * bootmisc.c追加
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<sh7706\7706.h>
#include	<sh7706\wcupara.h>
#include	<sh7706\debutil.h>
#include	<intrpt.h>
#include	<biosif\init.h>


void
debout(char c){
	SCI.SCSCR.BIT.TIE = (unsigned char)0;
	for(;;){
		if(SCI.SCSSR.BIT.TDRE != 0){
			SCI.SCTDR = (unsigned char)c;
			SCI.SCSSR.BIT.TDRE = (unsigned char)0;
			break;
		}
	}
}


void
wcu_jump_to_reset(){		/* ＷＤＴによる再起動 */
	_init_jump_to_reset();
}


/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
debstr(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			debout((char)0x0d);
		}
		debout(c);
		++i;
	}
}


