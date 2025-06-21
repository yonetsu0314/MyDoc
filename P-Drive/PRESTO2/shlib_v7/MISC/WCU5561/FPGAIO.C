/*======================================================================
 * File name    : fpgaio.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/wcu5561/rcs/fpgaio.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaio.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * int_maskをBIOS関数を用いて分離
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * int_mask()関数追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<sh7706\7706.h>
#include	<biosif\fifo.h>
#include	"fpgaio.h"


void
int_mask(int mask){
	_wcu_int_mask(mask);		/* Disable Outernal Interrupt */
}


int
fifo_init(){
	return(_fifo_init());
}


unsigned char
fifo_input(){
	return(_fifo_input());
}


unsigned char
fifo_status(){
	return(_fifo_status());
}


void
fifo_output( unsigned char Data8 ){
	_fifo_output(Data8);
}


void
fifo_msktxrx(unsigned short flag){
	_fifo_msktxrx(flag);
}


int
fifo_full(){
	return(_fifo_full());
}


