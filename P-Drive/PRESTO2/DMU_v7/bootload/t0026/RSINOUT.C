/*======================================================================
 * File name    : rsinout.c
 * Original		: p:/presto/dmu_v7/bootload/rcs/init.c 1.11
 *----------------------------------------------------------------------
 * Function 	:
 *		初期設定ルーチン（FOR SH7615）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/rsinout.c 4.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rsinout.c $
 * リビジョン 4.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 14:00
 * ・電断対応ファイルシステム搭載
 * ・プログラム整理を行う
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/04/08 20:30
 * DIPSW2の8番がONの場合のみcon0からのシリアル出力を許可するように
 * 変更する。
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/19 17:00
 * wait_answerを追加する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/02/19 21:30
 * rsoutdirにおいてDIPSW2の8番がONの場合は、con0からのシリアル出力を
 * 行わないようにする
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * setStack削除(V7最適化に不適)。drive\usnet.hをｲﾝｸﾙｰﾄﾞ
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include <ctype.h>
#include <kernel.h>
#include <rxif\rx116.h>
#include <sh7615\bootprm.h>
#include <sh7615\sh7615.h>
#include <drive\usnet.h>
#include "cmfhdl.h"
#include "remmon.h"
#include "tsk_lan.h"
#include "init.h"
#include "rsinout.h"
#include "cortsk.h"


void
rsstrdt(char *str){
	rsstr232(str);
}

/****************************************************************/
/*			ｌｏｎｇ型をアスキー１６進コードに変換する			*/
/****************************************************************/
void *Bin_Asc16( long value, unsigned char *asc, size_t keta )
{
	unsigned char	i, hi, lo;
	unsigned char *	p = asc + keta;

	for( i=(unsigned char)0; i<8; i++, value >>= 8 ) {
		if( keta == 0 ) break;
		keta--;
		p--;
		lo = (unsigned char)((unsigned char)value & 15);
		*p = (unsigned char)( lo >= 10 ? ('A' + lo - 10) : ('0' + lo) );
		if( keta == 0 ) break;
		keta--;
		p--;
		hi = (unsigned char)((unsigned char)value >> 4);
		*p = (unsigned char)( hi >= 10 ? ('A' + hi - 10) : ('0' + hi) );
	}
	return asc;
}


/****************************************************************/
/*	アスキー１６進コードをｌｏｎｇ型に変換する(サイズ付き)		*/
/****************************************************************/
long Asc16_Bin2( unsigned char *asc, size_t len )
{
	long			l, m;
	unsigned short	i;
	unsigned char	c;
	unsigned char *	s = asc;

	len = (len<8)? len : 8;

	for( i=(unsigned short)0, l=m=(long)0; i < len && *s != 0; i++, s++ ) {
		c = *s;
		if( c >= '0' && c <= '9' ) {
			m = c - '0';
		}
		else if( c >= 'A' && c <= 'F' ) {
			m = c - 'A' + 10;
		}
		else if( c >= 'a' && c <= 'f' ) {
			m = c - 'a' + 10;
		}
		else {
			m = 0;
		}
		l <<= 4;
		l += m;
	}
	return l;
}


int
getmes(char *str, char *adr, int n){
	char	c;
	int	i,gemef;

	gemef=0;
	rsstr(str);
	for(i=0;;){
		c=rsin();
		if(c==ESC){
			adr[0]=(char)0;
			gemef=1;
			break;
		}
		if(c==0x0d){
			rsout(c);
			adr[i]=(char)0;
			break;
		}
		if(c==0x08){
			if(i==0) continue;
			--i;
			rsout(c);
			rsout((char)0x20);
			rsout((char)0x08);
			if(i==0) continue;
			c=adr[i-1];
			if((c>=0x81)&&(c<=0x9f)||(c>=0x0e0)&&(c<=0x0ee)){
				--i;
				rsout((char)0x08);
				rsout((char)0x20);
				rsout((char)0x08);
			}
			continue;
		}
		if((c>=0x81)&&(c<=0x9f)||(c>=0x0e0)&&(c<=0x0ee)){
			if(i>(n-2)){
				c=rsin();
				continue;
			}
			rsout(c);
			adr[i]=c;
			++i;
			c=rsin();
			rsout(c);
			adr[i]=c;
			++i;
			continue;
		}
		if(c==0){
			c=rsin();
			continue;
		}
		if(i>=(n-1)) continue;
		if((c<32)||(c>238)) continue;
		rsout(c);
		adr[i]=c;
		++i;
	}
	return gemef;
}


int htoi(char a){
	char	b;

	b = (char)(a & 0x0f0);
	a &= 0x0f;
	if(b == 0x30) return((int)a);
	if(b == 0x40) return((int)(a+9));
	return(-1);
}


int
chkin(c)
char	c;
{
	rep=esc=0;
	if(c==(char)0x0d){rep=1;rsout((char)0x0a);return(1);}
	if(c==(char)0x1b){esc=1;return(1);}
	if (('0' <= c)&&(c <= '9')) return(0);
	else if (('A' <= c)&&(c <= 'F')) return(0);
	esc=1;
	return(1);
}


int
chkout(){
	if(rep) return(1);
	if(esc) return(1);
	return(0);
}


int
chkrep(){
	if(rep) return(1);
	return(0);
}


int
chkesc(){
	if(esc) return(1);
	return(0);
}


void
put02hexaa(char *aa,int sum){
	int		k,ad;
	char	c;

	for (k=1;k>=0;--k){
		ad = sum;
		ad >>= 4*k;
		ad &= 0x0f;
		if(ad >= 10) c=(char)('A'+ad-10);
		else c = (char)('0' + ad);
		*aa++ = (char)c;
	}
	*aa++ = (char)0;
}


/* デバッグ用ＲＳ２３２Ｃ復帰改行文字送信 */
void
rslf(){
	rsout((char)0x0d);
	rsout((char)0x0a);
}


/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
rsstr(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			rsout((char)0x0d);
		}
		rsout(c);
		++i;
	}
}
/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
rsstrb(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		rsout(c);
		++i;
	}
}
/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
rsstr232(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			rsout232((char)0x0d);
		}
		rsout232(c);
		++i;
	}
}


void
rsout232(char c){
	char	s[2];

	s[0] = c;
	s[1] = (char)0;
//	con0_puts((unsigned char *)s,1);
}


void
rsout(char c){
	char	s[2];

	s[0] = c;
	s[1] = (char)0;
	if(CmdFd != -1) UsnetSocket_writesocket(CmdFd, s, 1);
	else{
		con0_puts((unsigned char *)s,1);
	}
}


/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
rsstrdir(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			rsoutdir((char)0x0d);
		}
		rsoutdir(c);
		++i;
	}
}


void
rsoutdir(char c){
	int tmp;
	
	/* DIPSW2：8  ONの場合のみシリアル出力を行う */
	if ( !((tmp = *DIPSW2_ADDR) & DIPSW_8_MASK) ) {	/* DIPSW2：8  ON */
		sfr_set(SCIF_SCSCR1,0x30);
		for(;;){
			if(tmp = ((sfr_inw(SCIF_SC1SSR1) & 0x0020)!=0)){
				sfr_out((unsigned)SCIF_SCFTDR1,(unsigned char)c);
				sfr_clrw(SCIF_SC1SSR1,0x0020);		/* TDFE */
				break;
			}
		}
	}
}


void
put4dir(i)
int	i;
{
	int	k,ad;

	for (k=3;k>=0;--k){
		ad=i;
		ad >>= 4*k;
		itoa1dir(ad);
	}
}


void
itoa1dir(i)
int	i;
{
	char	c;

	i &= 0x0f;
	if(i>=10) c=(char)('A'+i-10);
	else c=(char)('0'+i);
	rsoutdir(c);
}


char rschk(){
	char	c;

	c = (char)((int_buf0_count-int_buf0_cntout)?1:0);
	return(c);
}

char rsin(){
	char	c;

	for(;;){
		if(int_buf0_count-int_buf0_cntout){
			c = *int_buf0_out++;
			++int_buf0_cntout;
			if(int_buf0_out>=int_buf0+INT_BUF_size)
						int_buf0_out=int_buf0;
			break;
		}
		dly_tsk((RELTIM)20/MSEC);
	}
	return(c);
}


void
put2hex(i)
int	i;
{
	int	k,ad;

	for (k=1;k>=0;--k){
		ad=i;
		ad >>= 4*k;
		itoa1(ad);
	}
}


void
put4hex(i)
int	i;
{
	int	k,ad;

	for (k=3;k>=0;--k){
		ad=i;
		ad >>= 4*k;
		itoa1(ad);
	}
}


void
put6hex(l)
int	l;
{
	int	k,i;
	int	ad;

	for (k=5;k>=0;--k){
		ad=l;
		ad >>= 4*k;
		i = (int)ad;
		itoa1(i);
	}
}


void
put8hex(l)
int	l;
{
	int	k,i;
	int	ad;

	for (k=7;k>=0;--k){
		ad=l;
		ad >>= 4*k;
		i = (int)ad;
		itoa1(i);
	}
}


int
atoi1(c)
char	c;
{
	int	i;

	i = c & 0xff;
	if (('0' <= i)&&(i <= '9')) i -= '0';
	else if (('A' <= i)&&(i <= 'F')) i += (0x0a - 'A');
	else i = 0;
	return i;
}


void
itoa1(i)
int	i;
{
	char	c;

	i &= 0x0f;
	if(i>=10) c=(char)('A'+i-10);
	else c=(char)('0'+i);
	rsout(c);
}

int strcomp(char* s1,char* s2){
	if(s1==s2) return(0);
	while (*s1 == *s2++){
		if (*s1++ == '\0') return(0);
	}
	return(*s1 - *--s2);
}

int
get2hex(){
	int	k,ad;
	char	c;

	for (k=0,ad=0;k<2;++k){
		c = rsin();
		if(chkin(c)) return(0);
		rsout(c);
		ad <<= 4;
		ad += atoi1(c);
	}
	return(ad);
}


int
get4hex(){
	int	k,ad;
	char	c;

	for (k=0,ad=0;k<4;++k){
		c = rsin();
		if(chkin(c)) return(0);
		rsout(c);
		ad <<= 4;
		ad += atoi1(c);
	}
	return(ad);
}


int
get6hex(){
	int	k;
	int	ad;
	char	c;

	for (k=0,ad=0;k<6;++k){
		c = rsin();
		if(chkin(c)) return(0);
		rsout(c);
		ad <<= 4;
		ad += atoi1(c);
	}
	return(ad);
}


int
get8hex(){
	int	k;
	int	ad;
	char	c;

	for (k=0,ad=0;k<8;++k){
		c = rsin();
		if(chkin(c)) return(0);
		rsout(c);
		ad <<= 4;
		ad += atoi1(c);
	}
	return(ad);
}


int wait_answer(void)
{
	char c;
	
	for (;;) {
		c = rsin();
		rsout(c);
		if (tolower(c) == 'y')	return 0;
		if (tolower(c) == 'n')	return -1;
	}
}
