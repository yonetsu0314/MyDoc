/*======================================================================
 * File name    : fpgaio.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/dmu5562/RCS/fpgaio.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fpgaio.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 04.12.09 BIOS分離
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * FPGA-FIFOﾄﾞﾗｲﾊﾞ追加
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<biosif\fifo.h>
#include	"fpgaio.h"


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


