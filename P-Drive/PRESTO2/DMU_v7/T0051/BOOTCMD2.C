/*======================================================================
 * File name    : bootcmd2.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/bootcmd2.c 3.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: bootcmd2.c $
 * リビジョン 3.3  1970/01/01  00:00:00  sakaguti
 * FLASHﾃｽﾄ･ﾌﾟﾛｸﾞﾗﾑ追加
 * 
 * リビジョン 3.2  1970/01/01  00:00:00  sakaguti
 * rsstr(04.12.7)追加、BTWRITERを削除 drive\flash.hを参照
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
#include <fsys.h>
#include <kernel.h>
#include "remmon.h"
#include "cmfhdl.h"
#include "download.h"
#include "bootcmds.h"
#include "tsk_lan.h"
#include "rsinout.h"
#include "init.h"
#include <drive\flash.h>
#include <sh7615\bootpara.h>
#include <drive\atadrv.h>
#include <sh7615\dmupara.h>
#include <drive\usnet.h>
#include <fcntl.h>				/* MS-C std */
#include <rom\xio.h>			/* Show include */

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
	ST_FILE	*fp;
	char	buf[120],*maxp,*minp;
	int		i,j;

	minp = (char *)0xffffffff;
	maxp = (char *)0x00000000;

	fp = fopen( (char*)fname, "r" );
	if( fp == NULL ) return(-1);
	rsstr("\n");
	for(j=0;;){
		if(fgets(buf, (short)120, fp)==NULL) break;
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
		++j;
		if((j%1000)==0){
			put8hex((int)maxp);
			rsstr("\r");
			dly_tsk((RELTIM)1);
		}
	}
	fclose(fp);
	rsstr("\nend");
	rsstr("\nbegin:"); put8hex((int)minp);
	rsstr("\n  end:"); put8hex((int)maxp);
	entryadrs = (int)minp;
	progsize = (int)(maxp - minp);
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

	ret = 0;
	rsstr("\n");
	set_fladr((UW)flashadr);
	set_mode(RESET_CMD);
	ret |= FlashX(flashadr,ramadr,(int)size,barcnt);
	set_mode(RESET_CMD);

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
comM(){
	static	int		fhcomm;
	static	char	tmp1[80];
	char	c;
	int		err, offset,len,no,dt;

	for(;;){
		rsstr("\nTEST>");
		if(getmes("", tmp1, 80)) break;
		c = tmp1[0];
		switch(c){
			case 'O':
				c = tmp1[1];
				switch(c){
					case 'F':
						rsstr("\nOpen Flash Memory");
						if((fhcomm = open( (unsigned char *)"from", O_RDWR)) == -1) {
							rsstr(" error ");
							return;
						}
						break;
					case 'E':
						rsstr("\nOpen E2rom");
						if((fhcomm = open( (unsigned char *)"e2rom", O_RDWR)) == -1) {
							rsstr(" error ");
							return;
						}
						break;
					case 'S':
						rsstr("\nOpen Sram");
						if((fhcomm = open( (unsigned char *)"sram", O_RDWR)) == -1) {
							rsstr(" error ");
							return;
						}
						break;
					default:
						break;
				}
				break;
			case 'S':
				rsstr("\nSeek");
				rsstr("\nOffset : ");
				offset=get4hex();
				if(chkesc()) return;
				if(chkrep()) return;
				if((err = seek( fhcomm, offset, 0)) == -1) {
					rsstr(" error ");
					return;
				}
				break;
			case 'R':
				rsstr("\nRead");
				rsstr("\nLength : ");
				len=get4hex();
				if(chkesc()) return;
				if(chkrep()) return;
				if(len>=80){
					rsstr(" len over.\n");
					break;
				}
				if((err = read( fhcomm, (unsigned char *)tmp1, len)) < 0) {
					rsstr(" error ");
					return;
				}
				tmp1[len]=(char)0;
				rsstr("\n");
				rsstr(tmp1);
				break;
			case 'W':
				rsstr("\nWrite");
				rsstr("\nString : ");
				if(getmes("", tmp1, 80)) break;
				if((err = write( fhcomm, (void *)tmp1, (int)strlen(tmp1))) < 0) {
					rsstr(" error ");
					return;
				}
				break;
			case 'C':
				rsstr("\nClose");
				if((err = close(fhcomm)) == -1) {
					rsstr(" error ");
					return;
				}
				break;
			case 'P':
				c = tmp1[1];
				switch(c){
					case 'D':
						rsstr("\nDipSW");
						rsstr("\nNo. : ");
						no=get4hex();
						if(chkesc()) return;
						if(chkrep()) return;
						rsstr("\nDipsw : ");
						put4hex((int)dipsw(no));
						break;
					case 'I':
						rsstr("\nParallel In");
						rsstr("\nPin : ");
						put4hex((int)pio_in());
						break;
					case 'O':
						rsstr("\nParallel Out");
						rsstr("\nData : ");
						dt=get4hex();
						if(chkesc()) return;
						if(chkrep()) return;
						pio_out((unsigned char)dt);
						break;
					default:
						break;
				}
				break;
			default:
				break;
		}
	}
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
	rsstr(" 04.12.7\n");
}


void
comT(){		/* ＣＦに書き込み */
	ST_FILE	*fp;
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

	fp = fopen( (char*)fname, "r" );
	if( fp != NULL ){
		fclose(fp);
		rsstr("\nFile exist. Over write?(Y or not):");
		if(rsin()!='Y') return;
	}

	comTfunc(memadr,sttofs,size,fname,0);
}


int
comTfunc(char *memadr, int sttofs, int size, char fname[],int barcnt){		/* CF Write */
	ST_FILE	*fp;
	char	buf[120],c;
	int		i,j,k,l,ofs;

	fp = fopen( (char*)fname, "w" );
	if( fp == NULL ) return(-1);
	rsstr("\n");

	mkmotstr(buf,fname,0,0,0);
	if(fputs(buf,fp)==EOF) goto err;
	for(l=0,k=0,i=0;i<size;i+=16){
		ofs = sttofs + i;
		j = size - i;
		if(j>16) j = 16;
		mkmotstr(buf,memadr,ofs,j,1);
		if(fputs(buf,fp)==EOF) goto err;
		++k;
		if(k > 1000){
			if(rschk()){
				c = rsin();
				if(c==(char)0x1b) return(-1);
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
	if(fputs(buf,fp)==EOF) goto err;
end:
//	fflush(fp);
	fclose(fp);
	return(0);
err:
	rsstr("\nCompact Flash Write Error.");
	fclose(fp);
	return(-1);
}


