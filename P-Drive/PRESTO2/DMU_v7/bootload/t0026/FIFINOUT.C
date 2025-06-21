
/*======================================================================
 * File name    : fifinout.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		FIFO入出力関数
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/fifinout.c 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fifinout.c $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * プログラム整理を行う
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/04 13:50
 * get_fifdata を修正する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/02/27 19:30
 * fifoバッファをクリアする関数を作成する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/07 16:00
 * get_fifstr を変更する
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
#define	_FIFINOUT_C_

#include	<kernel.h>
#include	<rxif\rx116.h>
#include	<sh7615\bootprm.h>
#include	"fifinout.h"
#include	"cmfhdl.h"
#include	"fif001.h"


void
fifoout(char c){
	char	s[2];

	s[0] = c;
	s[1] = (char)0;
	fif_puts((unsigned char *)s,1);
}


void
fifolf(){
	fifoout((char)0x0d);
	fifoout((char)0x0a);
}


void
fifostr(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			fifoout((char)0x0d);
		}
		fifoout(c);
		++i;
	}
}


void
fifostrb(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		fifoout(c);
		++i;
	}
}


char fifochk(){
	char	c;

	c = (char)((fif_buf_count-fif_buf_cntout)?1:0);
	return(c);
}

char fifoin(){
	char	c;

	for(;;){
		if(fif_buf_count-fif_buf_cntout){
			c = *fif_buf_out++;
			++fif_buf_cntout;
			if(fif_buf_out>=fif_buf+FIF_BUF_size)
						fif_buf_out=fif_buf;
			break;
		}
		dly_tsk((RELTIM)20/MSEC);
	}
	return(c);
}

char *get_fifdata(){
	static char	buf[128];
	int		c,i=0;

	for(;;){
		if(fifochk()){
			c  = (int)fifoin();
			if(c==(int)0x0d){
				buf[i++] = (char)0;
				continue;
			}
			if(c==(int)0x0a) break;
			buf[i++] = (char)c;
		}
	}
	return buf;
}

char *get_fifstr(){
	static char	buf[128];
	int		c,i=0;

	for(;;){
		if(fifochk()){
			c  = (int)fifoin();
			buf[i++] = (char)c;
			if(c==0)
				break;
		}
	}
	return buf;
}
