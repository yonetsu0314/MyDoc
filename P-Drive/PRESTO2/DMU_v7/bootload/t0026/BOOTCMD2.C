/*======================================================================
 * File name    : bootcmd2.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/bootcmd2.c 4.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootcmd2.c $
 * リビジョン 4.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 12:30
 * ・電断対応ファイルシステム搭載
 * ・インストール時間短縮を行う
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * ソフトウェア封印に対応するためnetwork設定のバックアップをFLASHから
 * E2ROMへ移動する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/24 14:00
 * comKを変更し、アプリにジャンプできるようにする
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・BOOT_CHECK_ADDRへの状態文字列セット処理を削除する。
 * ・不要なコマンド処理関数を削除する。
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/05/14:30
 * comJfuncに条件コンパイルを設定し、ブートライター時のワーニングをなくす
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:00
 * comLfuncにおいてシリアルバッファをクリアする処理を
 * 関数化する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/02/17 19:00
 * 初期状態（アプリがインストールされていない状態）において、
 * バージョン名を参照した時に、CPUが暴走しないようにcomN、
 * comJfuncを変更する。
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/02/12 10:30
 * comLfunc 先頭においてmotファイルのヘッダレコードを読み飛ばす
 * ことをやめる。transMOT で同様の処理を行っているため。
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * drive\usnet.hをｲﾝｸﾙｰﾄﾞ
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * dmupara.h をインクルードする
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_resetをdmu_jump_to_resetに変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 2002, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CITY
 *			SHIGA JAPAN
 *			(077) 553-4141
 *======================================================================
 */
#include <stdlib.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/
#include <custom\typedefs.h>	/*	"		*/
#include <rxif\rx116.h>			
#include <kernel.h>
#include "remmon.h"
#include "cmfhdl.h"
#include "download.h"
#include "bootcmds.h"
#include "tsk_lan.h"
#include "rsinout.h"
#include "cortsk.h"
#include "init.h"
#include <sh7615\bootprm.h>
#include <drive\atadrv.h>
#include <sh7615\dmupara.h>
#include <drive\flash.h>
#include <drive\usnet.h>
#include <drive\fssvr60\cnvfsys.h>


void
comA(){		/* DMUメモリ・テスト */
	int	*sttadr,*stpadr;
	int	dorg,m,inc;

	rsstr("\nStart ADDRESS : ");
	sttadr=(int *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nStop  ADDRESS : ");
	stpadr=(int *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nWRITE DATA  : ");
	dorg=get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nMASK  DATA  : ");
	m=get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nINC   DATA  : ");
	inc=get8hex();
	if(chkesc()) return;
	if(chkrep()) return;

	comAfunc(sttadr,stpadr,dorg,m,inc);
}


int
comAfunc(int *sttadr, int *stpadr, int dorg, int m, int inc){
	int	*adr;
	int	d;

	rsstr("\n");
	if(sttadr==stpadr){
		rsstr("incorrect address\n");
		return(-1);
	}
	for(adr=sttadr,d=dorg;adr<=stpadr;++adr,d+=inc){
		*adr = d;
		if((((int)adr)&0x0000ffff)==0){
			put8hex((int)adr);
			rsstr("\r");
		}
	}
	rsstr("\n");
	for(adr=sttadr,d=dorg;adr<=stpadr;++adr,d+=inc){
		if(((*adr)&m)!=(d&m)){
			rsstr("\nERROR ON ");
			put4hex((int)adr);
			rsstr(" WRITE=");
			put8hex(d);
			rsstr(" READ=");
			put8hex(*adr);
			return(-1);
		}
		if((((int)adr)&0x0000ffff)==0){
			put8hex((int)adr);
			rsstr("\r");
		}
	}
	rsstr("\n");
	return(0);
}


void
comC(){		/* DMUメモリ比較 */
	int		*sttadr1,*sttadr2;
	int		size;

	rsstr("\nStart ADDRESS 1 : ");
	sttadr1 = (int *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nStart ADDRESS 2 : ");
	sttadr2 = (int *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\n           SIZE : ");
	size    =   (int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;

	comCfunc(sttadr1,sttadr2,size);
}


int
comCfunc(int *sttadr1, int *sttadr2, int size){
	int		i;

	rsstr("\n");
	for(i=0;i<size/4;++i,++sttadr1,++sttadr2){
		if((*sttadr1)!=(*sttadr2)){
			rsstr("\nDIFFERENT DATA ON ADDRESS 1 : ");
			put8hex((int)sttadr1);
			rsstr(" DATA1=");
			put8hex(*sttadr1);
			rsstr(" DATA2=");
			put8hex(*sttadr2);
			return(-1);
		}
		if((((int)sttadr1)&0x0000ffff)==0){
			put8hex((int)sttadr1);
			rsstr("\r");
		}
	}
	rsstr("\n");
	return(0);
}


void
comF(){		/* ＣＦの読出し */
	char	fname[70];
	int		ofs;

	rsout((char)'-');
	ofs=(int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	if(getmes("\nEnter File Name : ", fname, 70)) return;

	comFfunc(ofs, fname);
}


int
comFfunc(int ofs, char fname[]){		/* ＣＦから読出し */
	TFMR_FILE_INFO	fileinfo;
	int		rdhdl,i,j,k,ret;
	static char ch;
	char	buf[120],*maxp,*minp,*data;

	rsstr("\n");

	minp = (char *)0xffffffff;
	maxp = (char *)0x00000000;
	
	rdhdl = IFS_fopen(fname, "r");
	if( rdhdl<0 ) return(-1);

	IFS_stat(fname,&fileinfo);
	ret = IFS_read(rdhdl,(void *)MEM4STT,(unsigned int)fileinfo.filesize);
	if(ret<=0) {
		IFS_fclose(rdhdl);
		return(-1);
	}

	data = (char *)MEM4STT;
	for(j=0;;){
		for(k=0; k<120; k++){
			ch = *(data++);
			if(ch==0x0d){
				buf[k] = (char)0;
				data++;
				break;
			}
			buf[k] = ch;
		}
		i = transMOT(buf, ofs, &maxp, &minp);
		if(i==1){
			rsstr("\nIs Data strange,isn't it?\n");
			IFS_fclose(rdhdl);
			return(-1);
		}
		if(i==2){
			rsstr("\nCheck Sum Error !!\n");
			IFS_fclose(rdhdl);
			return(-1);
		}
		if(i < 0) break;
		++j;
		if((j%1000)==0){
			put8hex((int)maxp);
			rsstr("\r");
			dly_tsk((RELTIM)1);
		}
	}
	IFS_fclose(rdhdl);
	rsstr("\nend");
	rsstr("\nbegin:"); put8hex((int)minp);
	rsstr("\n  end:"); put8hex((int)maxp);
	entryadrs = (int)minp;
	progsize = (int)(maxp - minp);

	/* プログラムサム値計算を正常に行うためプログラム書込み完了位置からMEM3STPまでをffで埋める */
	if(comAfunc((int *)(MEM2STT+progsize),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return -1;
	}
	return(0);
}


void
comH(){		/* Ｓフォーマット・ファイルの送信 */
	char	fname[70],*memadr;
	int		size,sttofs;

	rsstr("\nSDRAM ADDRESS : ");
	memadr = (char *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nENTRY ADDRESS : ");
	sttofs = (int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\n         SIZE : ");
	size    =   (int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	if(getmes("\nEnter Header Name : ", fname, 70)) return;

	comHfunc(memadr,sttofs,size,fname);
}


int
comHfunc(char *memadr, int sttofs, int size, char fname[]){
	char	buf[120],c;
	int		i,j,ofs;

	rsstr("\nPush CR to START !!!");
	if(rsin()!=(char)0x0d) return(-1);

	mkmotstr(buf,fname,0,0,0);
	rsstrb(buf);
	for(i=0;i<size;i+=16){
		if(rschk()){
			c = rsin();
			if(c==(char)0x1b) return(-1);
		}
		ofs = sttofs + i;
		j = size - i;
		if(j>16) j = 16;
		mkmotstr(buf,memadr,ofs,j,1);
		rsstrb(buf);
	}
	mkmotstr(buf,(char *)NULL,sttofs,0,2);
	rsstrb(buf);
	return(0);
}


void
comJ(){		/* DMUＦＬＡＳＨ書き込み */
	short	*flashadr,*ramadr;
	unsigned long size;

	rsstr("\nFlash ROM Programming ...");
	rsstr("\nStart FLASH ADDRESS : ");
	flashadr=(short *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nStart  DRAM ADDRESS : ");
	ramadr=(short *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\n            SIZE    : ");
	size=(int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;

	comJfunc(flashadr,ramadr,size,0);
}


int
comJfunc(short *flashadr, short *ramadr, unsigned long size,int barcnt){
	int		ret;
#ifndef BTWRITER
	unsigned char *sta;
	unsigned short sum,sss;
#endif

	ret = 0;
	rsstr("\n");
	set_fladr((UW)flashadr);
	set_mode(RESET_CMD);
	ret |= FlashX(flashadr,ramadr,(int)size,barcnt);
	set_mode(RESET_CMD);

#ifndef BTWRITER
	set_fladr((unsigned int)APLY_SUM_ADDR);
	sect_erase((unsigned long)APLY_SUM_ADDR);

	set_fladr((unsigned int)APLY_SIZE_ADDR);
	set_mode(RESET_CMD);
	sss = (unsigned short)(size >> 16);
	ret |= flash_write((short *)APLY_SIZE_ADDR, (short)sss);
	sss = (unsigned short)(size & 0x0ffff);
	ret |= flash_write((short *)(APLY_SIZE_ADDR+2), (short)sss);

	set_mode(RESET_CMD);
	sta = (unsigned char*)APLY_START_ADDR;
	sum = (unsigned short)0;
	/* アプリ部分 */
//	size = (unsigned long)(APLY_END_ADDR - APLY_START_ADDR);
	while( size ) {
		sum += *sta;
		sta++;
		size--;
	}

	set_fladr((unsigned int)APLY_SUM_ADDR);
	set_mode(RESET_CMD);
	ret |= flash_write((short *)APLY_SUM_ADDR, (short)sum);
	set_mode(RESET_CMD);
#endif
	if (check_SumCheck_NG()) {	/* サム値エラーが生じていた場合はフラグをクリアする */
		clear_SumCheck_NG();
	}
	return(ret);
}


void
comK(){		/* DMUアプリの実行 */
	char	*adr;

	rsstr("\nWDT Start Address : ");
	adr=(char *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;

	comKfunc(adr);

	UsnetSocket_closesocket(sCltFdCmd);

	set_mode(RESET_CMD);
	strcpy( APLY_START_STATUS, "AplStat" );
	dmu_jump_to_reset();
}


int
comKfunc(char *adr){
	APLY	func;
	int		ret;

	ret = 0;
	func = (void *)adr;
	*((APLY*)BOOT_APLY_ADDR) = func;
	*BOOT_SETSUM_ADDR = (unsigned short)TcpAplyChecksum();
	return(ret);
}


void
comL(){		/* Ｓファイルの受信 */
	int		ofs;

	rsout((char)'-');
	ofs=(int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;

	rsstr("\nPleas send Motorola S format data\n");
	if(comLfunc(ofs,0)!=0) rscan();
}


int
comLfunc(int ofs,int barcnt){
	char	buf[120],*maxp,*minp;
	int		c,i,j,k;

	minp = (char *)0xffffffff;
	maxp = (char *)0x00000000;

	con0_buf_clear();
	
	for(i=0;;){
		if(rschk()) break;
		dly_tsk((RELTIM)1/MSEC);
		++i;
		if(i>60000) break;
	}
	if(i>60000){
		rsstr("\nTime out\n");
		return(-1);
	}

	rsstr("\nSTART\n");
	for(k=0;;){
		for(i=0,j=0;;){
			if(rschk()){
				c  = rsin();
				if(c==(int)0x0a) continue;
				if(c==(int)0x0d){
					buf[j++] = (char)0;
					break;
				}
				buf[j++] = (char)c;
				i = 0;
				continue;
			}
			dly_tsk((RELTIM)1/MSEC);
			++i;
			if(i>5000) break;
		}
		if(i>5000){
			rsstr("\nTime out\n");
			return(-1);
		}

		if(barcnt!=0){
			++k;
			if(k > barcnt){
				incBAR(1);
				k = 0;
			}
		}

		i = transMOT(buf, ofs, &maxp, &minp);

		if(i==1){
			rsstr("\nIs Data strange,isn't it?\n");
			return(-1);
		}
		if(i==2){
			rsstr("\nCheck Sum Error !!\n");
			return(-1);
		}
		if(i < 0) break;
	}
	rsstr("\nend");
	rsstr("\nbegin:"); put8hex((int)minp);
	rsstr("\n  end:"); put8hex((int)maxp);
	entryadrs = (int)minp;
	progsize = (int)(maxp - minp);
	return(0);
}


void
comN(){		/* DMUバージョン情報表示 */
	static	char	tmp[17],verstr[128];
	char	*idptr;
	int		*iptr;
	struct	apl_info		*ap;

//	rsstr("\nBOOT : ");
	strcpy(verstr,BOOT_VERSION_ADDR);
	strcat(verstr," ");

//	rsstr("\nAPLY : ");

	if (check_SumCheck_NG()) {
		strcat(verstr, "FFFFF F.F FFFF FFFFFF"); /* サム値エラーの場合はFFを返す */
		rsstr("\n");
		rsstr(verstr);
		strcpy(versionstr,verstr);
	}
	else {
		ap = (struct apl_info *)(*((APLY*)BOOT_APLY_ADDR));
		iptr = (int *)ap->vecter_adr;
		iptr = (int *)(*iptr);
		idptr = (char *)(iptr);
		getversionstr(tmp, idptr);
		strcat(verstr,tmp);
		strcat(verstr," ");

		Bin_Asc16( (long)(*((unsigned short *)APLY_SUM_ADDR)), (unsigned char *)(&tmp[0]), (size_t)4 );
		tmp[4] = (char)0;
		strcat(verstr,tmp);
		strcat(verstr," ");

		Bin_Asc16( (long)(*((int *)APLY_SIZE_ADDR)), (unsigned char *)(&tmp[0]), (size_t)6 );
		tmp[6] = (char)0;
		strcat(verstr,tmp);

		rsstr("\n");
		rsstr(verstr);
		strcpy(versionstr,verstr);
	}
}


void
comT(){		/* ＣＦに書き込み */
	int		rdhdl;
	char	fname[70],*memadr;
	int		size,sttofs;

	rsstr("\nSDRAM ADDRESS : ");
	memadr = (char *)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\nENTRY ADDRESS : ");
	sttofs = (int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	rsstr("\n         SIZE : ");
	size    =   (int)get8hex();
	if(chkesc()) return;
	if(chkrep()) return;
	if(getmes("\nEnter File Name : ", fname, 70)) return;

	rdhdl = IFS_fopen(fname, "r");
	if( rdhdl>=0 ){
		IFS_fclose(rdhdl);
		rsstr("\nFile exist. Over write?(Y or not):");
		if(rsin()!='Y') return;
	}

	comTfunc(memadr,sttofs,size,fname,0);
}


int
comTfunc(char *memadr, int sttofs, int size, char fname[],int barcnt){		/* CF Write */
	char	buf[120],c;
	int		wrhdl,i,j,k,l,ofs,datasize=0;

	wrhdl = IFS_fopen(fname, "w");
	if( wrhdl<0) return(-1);
	rsstr("\n");

	mkmotstr(buf,fname,0,0,0);
	if(IFS_write(wrhdl,(void *)buf,(unsigned int)strlen(buf))<0) goto err;
	for(l=0,k=0,i=0;i<size;i+=16){
		ofs = sttofs + i;
		j = size - i;
		if(j>16) j = 16;
		mkmotstr(buf,memadr,ofs,j,1);
		strcpy(rdwrbuf+datasize,buf);
		datasize += strlen(buf);
		if ( (datasize > (RDWR_SIZE-120)) || ((size-i) <= 16) ) {
			if(IFS_write(wrhdl,(void *)rdwrbuf,(unsigned int)datasize)<0) goto err;
			datasize = 0;
		}
		++k;
		if(k > 1000){
			if(rschk()){
				c = rsin();
				if(c==(char)0x1b) {
					IFS_fclose(wrhdl);
					return(-1);
				}
			}
			put8hex(ofs);
			rsstr("\r");
			dly_tsk((RELTIM)1);
			k=0;
		}
		if(barcnt!=0){
			++l;
			if(l > barcnt){
				incBAR(1);
				l = 0;
			}
		}
	}
	mkmotstr(buf,(char *)NULL,sttofs,0,2);
	if(IFS_write(wrhdl,(void *)buf,(unsigned int)strlen(buf))<0) goto err;
end:
	IFS_fclose(wrhdl);
	return(0);
err:
	rsstr("\nCompact Flash Write Error.");
	IFS_fclose(wrhdl);
	return(-1);
}


