/*======================================================================
 * File name    : sumfnc.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: サムチェック用関数
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/sumfnc.c 1.1 1997/05/28 13:09:51 yonetsu Exp $
 *$Log: sumfnc.c $
 * リビジョン 1.1  1997/05/28  13:09:51  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define _SUMFNC_C_

#include <stdio.h>	/* for NULL */
#include <string.h>
#include <custom\custom.h>
#include <rxif\rx116.h>			/* Show include	     */
#include "cmfhdl.h"


COMRV cpl_hex_ascii_sum(unsigned char *buf,int len,char *txrx)
{
	unsigned char sum;

	if(!strcmp(txrx,"tx")) {				/* 送信時 */
		if(len <= 0)	return COM_FAIL;
		sum = cksum_cpl(buf,len);
		buf[len] = hex_ascii(sum/16);
		buf[len+1] = hex_ascii(sum%16);
		buf[len+2] = '\0';
		return 2;	/* add length */
	} 
	else if(!strcmp(txrx,"rx")) {			/* 受信時 */
		if(len <= 0)	return len;	  	/* error */
		if(len <= 2)	return RCV_ERROR; 	/* to short */
		sum = (unsigned char)(ascii_hex(buf[len-2])<<4);
		sum |= (unsigned char)ascii_hex(buf[len-1]);		
		if(sum == cksum_cpl(buf,len-2)) {
			buf[len - 2] = '\0';
			return 2;			/* sub length */
		}
		else 	return RCV_ERROR;
	}
	return 0;
}

COMRV or30h_ascii_sum(unsigned char *buf,int len,char *txrx)
{
	unsigned char sum;

	if(!strcmp(txrx,"tx")) {				/* 送信時 */
		if(len <= 0)	return COM_FAIL;
		sum = (unsigned char)(0 - cksum_cpl(buf,len));
		buf[len] = (unsigned char)((sum/16) | '0');
		buf[len+1] = (unsigned char)((sum%16) | '0');
		buf[len+2] = '\0';
		return 2;	/* add length */
	} 
	else if(!strcmp(txrx,"rx")) {			/* 受信時 */
		if(len <= 0)	return len;	  	/* error */
		if(len <= 2)	return RCV_ERROR; 	/* to short */
		sum = (unsigned char)((buf[len-2] & 0x0f) << 4);
		sum |= (unsigned char)(buf[len-1] & 0x0f);		
 		sum += cksum_cpl(buf,len-2);
		if(!sum) {
			buf[len - 2] = '\0';
			return 2;			/* sub length */
		}
		else 	return RCV_ERROR;
	}
	return 0;
}
