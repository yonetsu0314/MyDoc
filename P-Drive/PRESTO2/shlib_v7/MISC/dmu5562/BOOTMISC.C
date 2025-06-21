/*----------------------------------------------------------------------*
 * File name	: bootmisc.c		/ WDT & WAIT & DEBOUT				*
 * Original	: p:/dacs_i/mcu3/bios2/src/rcs/eesprom.c 1.3				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/dmu5562/RCS/bootmisc.c 1.7 1970/01/01 00:00:00 chimura Exp $
 *$Log: bootmisc.c $
 * リビジョン 1.7  1970/01/01  00:00:00  chimura
 * 2005/01/24 dipsw、pio関係の関数は、biosifの関数をコールするように変更
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * wdtをinitのBIOSへ、int_mask等をFPGAのBIOSへ分離
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * pio関数を追加
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * DIPSW3の未使用を０出力にする
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * DIPSW関数でONの時1出力にする
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * DIPSW関数を追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * bootmisc.c追加
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 2004, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CITY							*/
/*				SHIGA JAPAN												*/
/*				(077) 553-4141											*/
/*----------------------------------------------------------------------*/

#include	<sh7615\7615.h>
#include	<sh7615\RCUTEST.h>
#include	<sh7615\dmupara.h>
#include	<sh7615\bootpara.h>
#include	<sh7615\debutil.h>
#include	<sh7615\sh7615.h>
#include	<intrpt.h>
#include	<biosif\fifo.h>
#include	<biosif\init.h>
#include	<biosif\ppi.h>


unsigned char
dipsw(int no){
	int	a;

	a = _pio_rd(1, no);
	return((unsigned char)a);
}


unsigned char
pio_in(){
	unsigned char a;

	a = _pio_rd(0, 0);
	return((unsigned char)a);
}


void
pio_out(unsigned char a){
	_pio_wr(0, 0, a);
}


void
debout(char c){
	sfr_set(SCIF_SCSCR1,0x30);
	for(;;){
		if((sfr_inw(SCIF_SC1SSR1) & 0x0020)!=0){
			sfr_out((unsigned)SCIF_SCFTDR1,(unsigned char)c);
			sfr_clrw(SCIF_SC1SSR1,0x0020);		/* TDFE */
			break;
		}
	}
}


void
dmu_jump_to_reset(){		/* ＷＤＴによる再起動 */
	_init_jump_to_reset();
}


void
int_mask(int mask){
	_dmu_int_mask(mask);
}


void
pint_mask(int mask){
	_dmu_pint_mask(mask);
}


/****************************************************/
/* rstwdt():WDTのｶｳﾝﾀを0にﾘｾｯﾄする関数				*/
/*	この関数は、以下のwait時間待ち関数において		*/
/*	時間待ちしている間、WDTをﾘｾｯﾄする関数である。	*/
/*	wait関数は、いろいろな関数で使用している為、	*/
/*	この関数の物理ｱﾄﾞﾚｽのｱｸｾｽをBIOS分離すると		*/
/*	wait時間が変わってしまうため、変更せずに		*/
/*	このままとする。他のCPUに移植の際は、BIOS		*/
/*	分離して、時間調整（WAIT1S,WAIT01S）する事。	*/
/****************************************************/
void
rstwdt(){
	*WTCNT = (unsigned short)0x5a00;		/* Rest Watch Dog Timer */
}


void
wait(int n){				/* wait 1mS * n */
	int	a,i;

	a=0;
www1:
	++a;
	if(a>n)	return;
	for(i=0;i<WAIT1S;++i) rstwdt();
	goto	www1;
}


void
wait0(int n){				/* wait 0.1mS * n */
	int	a,i;

	a=0;
www1:
	++a;
	if(a>n)	return;
	for(i=0;i<WAIT01S;++i) rstwdt();
	goto	www1;
}


void
wait1(){				/* wait 1mS */
	wait(1);
}


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


