/*======================================================================
 * File name    : bootcmd3.c
 * Original		: p:/presto/dmu_v7/bootload/rcs/init.c 1.11
 *----------------------------------------------------------------------
 * Function 	:
 *		初期設定ルーチン（FOR SH7615）
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu_v7/bootload/rcs/bootcmd3.c 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootcmd3.c $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * ソフトウェア封印に対応するためnetwork設定のバックアップをFLASHから
 * E2ROMへ移動する
 * 
 * リビジョン 2.6  1970/01/01  00:00:00  kagatume
 * 2004/03/24 14:00
 * 不要なコマンド処理関数を削除する
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・BOOT_CHECK_ADDRへの状態文字列セット処理を削除する。
 * ・不要なコマンド処理関数を削除する。
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/09 18:00
 * LEDの表示パターン名を整理したことに対応する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/08 11:00
 * comGDfunc でのエラー発生時にLEDを点滅させるようにする
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/05 14:30
 * ・comG変更し、ブートローダーの書き換えができるようにする
 * ・comBに条件コンパイルを設定しブートライター時のワーニングをなくす
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/02 12:30
 * comGBfunc でのエラー発生時にLEDを点滅させるようにする
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * drive\usnet.hをｲﾝｸﾙｰﾄﾞ
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_resetをdmu_jump_to_resetに変更する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/26 18:00
 * FIFOチェック用関数comRWを追加
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/06 19:00
 * ワーニングをなくす
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
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
#include <stddef.h>
#include <string.h>
#include <kernel.h>
#include <biosif\rtc.h>
#include <sh7615\bootpara.h>
#include <drive\flash.h>
#include <sh7615\dmupara.h>
#include <dos_util.h>
#include <fsys.h>
#include <drive\usnet.h>
#include "cmfhdl.h"
#include "bootcmds.h"
#include "rsinout.h"
#include "tsk_lan.h"
#include "remmon.h"
#include "init.h"
#include "fifinout.h"
#include "ledset.h"


void
comB(){		/* ＩＰアドレスの設定 */
	char	*adr;
	int		ip,svr,gw,sbn;

	ip  = *((int *)ETH_IP_ADR);	svr = *((int *)ETH_SVR_ADR);
	gw  = *((int *)ETH_GW_ADR);	sbn = *((int *)ETH_SBN_ADR);

	rsstr("\nSet TCP/IP ADDRESS");

	rsstr("\nIP : ");	adr = (char *)ETH_IP_ADR;	putiphex(adr);	rsstr("\n   : ");
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;

	rsstr("\nSVR: ");	adr = (char *)ETH_SVR_ADR;	putiphex(adr);	rsstr("\n   : ");
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;

	rsstr("\nGW : ");	adr = (char *)ETH_GW_ADR;	putiphex(adr);	rsstr("\n   : ");
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;

	rsstr("\nSBN: ");	adr = (char *)ETH_SBN_ADR;	putiphex(adr);	rsstr("\n   : ");
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;

	rsstr("\nTCP/IP Setted !!!\n");

	*BOOT_SETSUM_ADDR = TcpAplyChecksum();
	write_toTcpipBackup();

	return;
err:
	*((int *)ETH_IP_ADR)  = ip ;	*((int *)ETH_SVR_ADR) = svr;
	*((int *)ETH_GW_ADR)  = gw ;	*((int *)ETH_SBN_ADR) = sbn;
	return;
}


void
putiphex(char *adr){
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
}


void
comG(){
	static	char	tmp1[80];
	char	c;
	unsigned long		i;

	for(;;){
		rsstr("\nDMU>");
		if(getmes("", tmp1, 80)) break;
		c = tmp1[0];
		switch(c){
			case 'D':
				if (comGDfunc()) set_ledmode(FUNC_ERROR);
				rsstr("\nRCU>END\n");
				return;
			case 'U':
				comGUfunc();
				rsstr("\nRCU>END\n");
				return;
			case 'B':
				if (comGBfunc()) set_ledmode(FUNC_ERROR);
				return;
			case '2':
				comG2func();
				return;
			case '3':
				comGZ3func();
				return;
			case 'M':
				tstMfunc();
				break;
			case 'E':
				tstEfunc();
				break;
			case 'F':
				tstFfunc();
				break;
			case 'O':
				tstOfunc();
				break;
			case 'R':
				tstRfunc();
				break;
			case 'C':
				tstCfunc();
				break;
			case 'A':
				tstAfunc();
				break;
			case 'H':
				closedir( Cur_DirPtr );
				Cur_DirPtr = NULL;
				break;
			case 'I':
				rsstr("CF Free Size = ");
				i = getfree((char)'C');
				put8hex((int)i);
				rsstr(" bytes.\n");
				break;
			case 'J':
				rsstr("CF Free Size = ");
				i = getfree((char)'E');
				put8hex((int)i);
				rsstr(" bytes.\n");
				break;
		    default:
				break;
		}
	}
}


void
comO(){		/* DMUＦＬＡＳＨワード書き込み */
	short	*adri;
	int		i;

	rsout((char)'-');
	adri=(short *)get8hex();
	if(chkout()) return;
	set_fladr((UW)adri);
	for(;;){
		rslf();
		put8hex((int)adri);
		rsout((char)'=');put4hex((int)*adri);
		rsout((char)'-');
		i=get4hex();
		if(chkout()) return;
		set_mode(RESET_CMD);
		if(flash_write(adri, (short)i) != (short)0) rsstr("error\n");
		set_mode(RESET_CMD);
		++adri;
	}
}


void
comP(){		/* DMUブートローダ再起動 */
	UsnetSocket_closesocket(sCltFdCmd);

	set_mode(RESET_CMD);
	dmu_jump_to_reset();
}


void
comQ(){		/* DMUＦＬＡＳＨセクタ消去 */
	int		sect;
	long	rtcd;

	rsstr("\nSector ADDRESS : ");
	sect = get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	set_fladr((UW)sect);
	rsstr("\nSector erase: ");
	if((rtcd = sect_erase((unsigned long)sect)) != 0){
		if(rtcd == 2){
			rsstr("erase time-out\n");
		}
		else{
			rsstr("erase error\n");
		}
	}
	rsstr("complete\n");
}


void
comR(){
	char	c;

	rsstr("\nRTC Check");
	for(;;){
		rsstr("\n->");
		c = rsin();
		if(c==(char)0x1b) break;
		rsout(c);
		switch(c){
			case 'B':
				comRB();
				break;
			case 'C':
				comRC();
				break;
			case 'E':
				comRE();
				break;
			case 'F':
				comRF();
				break;
			case 'H':
				comRH();
				break;
			case 'S':
				comRS();
				break;
			case 'T':
				comRT();
				break;
			case 'U':
				comRU();
				break;
			case 'Q':
				comRQ();
				break;
			case 'W':
				comRW();
				break;
			case 'X':
				comRX();
				break;
			case 'Y':
				comRY();
				break;
			case 'Z':
				comRZ();
				break;
		    default:
				break;
		}
	}
}


void
comRB(){
	int		year,month,day,week;

	rsstr("\nSet Year : ");
	year=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nSet Month : ");
	month=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nSet Day : ");
	day=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nSet Week : ");
	week=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	_rtc_set_date(year,month,day,week);
	rsstr("\nRTC Setted !!!\n");
}


void
comRC(){
	_rtc_initialize();
	rsstr("\nRTC Resetted !!!\n");
}


void
comRE(){
	int		hour,min,sec;

	rsstr("\nSet Hour : ");
	hour=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nSet Min : ");
	min=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nSet Sec : ");
	sec=(int)get4hex();
	if(chkesc()) return;
	if(chkrep()) return;
	_rtc_set_time(hour,min,sec);
	rsstr("\nRTC Setted !!!\n");
}


void
comRF(){
	int	year,month,day,week;

	_rtc_get_date(&year,&month,&day,&week);
	rsstr("\nyear:");
	put4hex((int)year);
	rsstr("\nmonth:");
	put4hex((int)month);
	rsstr("\nday:");
	put4hex((int)day);
	rsstr("\nweek:");
	put4hex((int)week);
	rsstr("\n");
}


void
comRH(){
	int	hour,min,sec;

	_rtc_get_time(&hour,&min,&sec);
	rsstr("\nhour:");
	put4hex((int)hour);
	rsstr("\nmin:");
	put4hex((int)min);
	rsstr("\nsec:");
	put4hex((int)sec);
	rsstr("\n");
}


void
comRS(){		/* ＭＡＣアドレスの設定 */
	union macunion{
		unsigned short s[10];
		unsigned char  b[20];
	} mac;
	char	*adr;

	rsstr("\nSet MAC ADDRESS");

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,30,0);
	e2s_romf_read(0, mac.s, MAC_ADDRESS_LEN);
	e2s_romf_close(0);

	rsstr("\nMAC : ");	adr = (char *)mac.b;	putmachex(adr);	rsstr("\n    : ");
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;	rsout((char)':');
	*adr++ = (char)get2hex();	if(chkesc()) goto err;	if(chkrep()) goto err;

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,30,0);
	e2s_romf_write(0, mac.s, MAC_ADDRESS_LEN);
	e2s_romf_close(0);

	rsstr("\nTCP/IP Setted !!!\n");
err:
	return;
}


void
putmachex(char *adr){
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
	rsout((char)':');
	put2hex((int)(*adr++));
}


void
comRT(){
	int		i;

	rsstr("1ms*X:");
	i=get4hex();
	if(chkesc()) return;
	for(;;){
		if(rschk()!=0) return;
		wait(i);
		rsout((char)'.');
		dly_tsk((RELTIM)1/MSEC);
	}
}


void
comRU(){
	int		i;

	rsstr("0.1ms*X:");
	i=get4hex();
	if(chkesc()) return;
	for(;;){
		if(rschk()!=0) return;
		wait0(i);
		rsout((char)'.');
		dly_tsk((RELTIM)1/MSEC);
	}
}


int
tstAfunc(){
	rsstr("\ncurrent dir is ");
	rsstr(dirname);
	return(0);
}


int
tstCfunc(){
	char	fname[120],ret;

	if(getmes("\nEnter Delete File Name : ", fname, 70)) return(-1);

	ret = (char)remove( (char*)fname );
	if( ret ){
		rsstr("\nremove file error.");
		return(-1);
	}
	rsstr("\nremove file success.");
	return(0);
}


int
tstEfunc(){
	char	fname[70],ret;

	if(getmes("\nEnter Remove Dir Name : ", fname, 70)) return(-1);

	ret = (char)rmdir( (char*)fname );
	if( ret ){
		rsstr("\ndir remove error.");
		return(-1);
	}
	rsstr("\ndir remove success.");
	return(0);
}


int
tstFfunc(){
	char	fname[120];

	if(getmes("\nEnter Make File Name : ", fname, 70)) return(-1);

	TestFileCreate( (char*)fname );
	return(0);
}


int
tstMfunc(){
	char	fname[70],ret;

	if(getmes("\nEnter Make Dir Name : ", fname, 70)) return(-1);

	ret = (char)mkdir( (char*)fname );
	if( ret ){
		rsstr("\ndir make error.");
		return(-1);
	}
	rsstr("\ndir make success.");
	return(0);
}


int
tstOfunc(){
	int		ret;

	if(getmes("\nEnter Change Dir Name : ", dirname, 15)) return(-1);

	ret = chdir( (char*)dirname );
	if( ret ){
		rsstr("\nchdir error.");
		return(-1);
	}
	rsstr("\nchdir to ");
	rsstr(dirname);
	return(0);
}


int
tstRfunc(){
	ST_DIRENTRY	*bp;
	int		a;
	unsigned char	uc;
	union llunion{
		unsigned long l;
		unsigned char  b[4];
	} ll;


	for(;;){
		bp = readdir( Cur_DirPtr );
		if( bp == NULL ) break;
		a = (int)bp->attr;
		if(a & 0x08){
			rsstr("\n#");
			rsstr((char *)bp->name);	rsout((char)'#');
			ll.l = (int)bp->size;
			uc = ll.b[0];	ll.b[0] = ll.b[3];	ll.b[3] = uc;
			uc = ll.b[1];	ll.b[1] = ll.b[2];	ll.b[2] = uc;
			put8hex((int)ll.l);
			continue;
		}
		else if(a & 0x10){
			rsstr("\n<");
			rsstr((char *)bp->name);
			rsout((char)'>');
			continue;
		}
		else{
			rsstr("\n ");
			rsstr((char *)bp->name);	rsout((char)' ');
			ll.l = (int)bp->size;
			uc = ll.b[0];	ll.b[0] = ll.b[3];	ll.b[3] = uc;
			uc = ll.b[1];	ll.b[1] = ll.b[2];	ll.b[2] = uc;
			put8hex((int)ll.l);
			continue;
		}
	}
	rsstr("\n");
	return(0);
}


void
comRW(){
	char	c;

	rsstr("\nFIFO Check");
	rsstr("\n--");
	for(;;){
		for(;;){
			if(fifochk()==0) break;
			c = fifoin();
			rsout(c);
		}
//		if(fifochk()){
//			c = fifoin();
//			rsout(c);
//		}
		if(rschk()){
			c = rsin();
			if(c==(char)0x00) continue;
			if(c==(char)0x1b) break;
			fifoout(c);
		}
//		dly_tsk((RELTIM)1/MSEC);
	}
}


void
comRX(){
	rsstr("\nfifo = ");
	put4hex((int)fifo_input());
}


void
comRQ(){
	rsstr("\nfifo status = ");
	put4hex((int)fifo_status());
}


void
comRY(){
	int		i;

	rsstr("\nwrite fifo : ");
	i=get2hex();
	fifo_output((unsigned char)i);
}


void
comRZ(){
	rsstr("\nfifo full = ");
	put4hex((int)fifo_full());
}


