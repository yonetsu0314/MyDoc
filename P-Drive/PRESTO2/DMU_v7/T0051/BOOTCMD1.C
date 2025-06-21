/*======================================================================
 * File name    : bootcmd1.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: ＣＦ、ＮＥＴによるブート・シーケンス実現
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/bootcmd1.c 3.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: bootcmd1.c $
 * リビジョン 3.3  1970/01/01  00:00:00  sakaguti
 * drive\flash.hを参照
 * 
 * リビジョン 3.2  1970/01/01  00:00:00  kagatume
 * 2004/5/13 10:00
 * FPGAプログラムダウンロード時にバージョン番号、プログラム名を
 * チェックするように変更する。
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/12 18:00
 * FPGAプログラムバージョン比較時のコメントを変更し統一する
 * 
 * リビジョン 2.12  1970/01/01  00:00:00  kagatume
 * 2004/03/18 12:30
 * インストール、アップロードするソフトはinfファイルによって判断
 * するように変更。
 * 
 * リビジョン 2.11  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・変数yourswは不要なので削除する。
 * ・BOOT_CHECK_ADDRへの状態文字列セット処理を削除する。
 * 
 * リビジョン 2.10  1970/01/01  00:00:00  kagatume
 * 2004/03/10 16:30
 * comGDfuncにおいてエラーが生じた時は、comGBfuncを実行し、
 * バックアップファイルの復旧を行う
 * 
 * リビジョン 2.9  1970/01/01  00:00:00  kagatume
 * 2004/03/09 18:30
 * バックアップファイル復帰中におけるLED表示パターンを新規に作成し、
 * 表示名を整理したことに対応
 * 
 * リビジョン 2.8  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * ・comGDfunc、comGBfuncにおいて状態文字列セット前にRESET_CMD
 *   をセットするようにする。
 * ・comGDfuncにおいてダウンロード終了後にLEDパターンを元に戻す処理を
 *   追加する
 * 
 * リビジョン 2.7  1970/01/01  00:00:00  kagatume
 * 2004/03/08 11:00
 * comGDfunc においてもLEDでダウンロード状況を把握できるようにする。
 * 
 * リビジョン 2.6  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:20
 * DIPSWがブートローダーモードの時はwebリモコンからダウンロード
 * ができないようにしエラーを返す
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:00
 * ダウンロード時のアドレスが正しくセットされるようにdownload.c の
 * 関数を変更したことに伴う変更
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/02 12:30
 * 前リビジョンの変更内容を整理する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/02 9:30
 * comGBfunc 実行時に状態をLEDパターンで表示できるようにする
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/02/27 19:30
 * スタートアドレス定義をsh7615\bootpara.h に移動する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/02/26 18:00
 * comGDfunc, comGBfuncの正常終了時においてBOOT_CHECK_ADDRに
 * "CF Check End"をセットするようにする
 * 
 * リビジョン 1.23  1970/01/01  00:00:00  kagatume
 * 2004/02/23 16:00
 * ・FPGAのソフトがプログラム名表示、バージョン表示に対応できて
 *   いないため、一旦、ダウンロード時と起動時においてそれらに関する
 *   項目をチェックしないようにする。
 * ・comGBfuncにおいてエラー時のコメント出力文字を修正する
 * 
 * リビジョン 1.22  1970/01/01  00:00:00  kagatume
 * 2004/02/20 11:00
 * ファイル削除時においてシリアル出力するコメントを変更する
 * 
 * リビジョン 1.21  1970/01/01  00:00:00  kagatume
 * 2004/02/20 10:00
 * comGDfunc内のコメントを変更する
 * comGBfuncにおいてエラー時にコメントをシリアル出力するようにする
 * 
 * リビジョン 1.20  1970/01/01  00:00:00  kagatume
 * 2004/02/19 17:10
 * プログラム名のチェックも行うようにcomGDfunc, comGBfunc
 * を変更する
 * 
 * リビジョン 1.19  1970/01/01  00:00:00  kagatume
 * 2004/02/17 21:00
 * comGUfunc においてsetBARの値を適切に割り当て直す
 * 
 * リビジョン 1.18  1970/01/01  00:00:00  kagatume
 * 2004/02/17 21:00
 * comGDfunc, comGUfunc, comGBfunc においてＦＰＧＡの
 * サム値チェックも行うようにする
 * 
 * リビジョン 1.17  1970/01/01  00:00:00  kagatume
 * 2004/02/17 19:00
 * comGUfunc のワーニングをなくす
 * 
 * リビジョン 1.16  1970/01/01  00:00:00  kagatume
 * 2004/02/17 19:00
 * comGBfuncにcomKfuncを追加する。
 * 初期状態（アプリ、FPGAがインストールされていない状態）の
 * DMUアプリダウンロードにおいてバージョン名の参照時に、
 * CPUが暴走しないようにするため。
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  kagatume
 * 2004/02/17 16:00
 * ＤＭＵアプリのアップロード中にBARコマンドを送信しないようにする
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  kagatume
 * 2004/02/17 9:00
 * comGDfunc においてWCUへのダウンロード後、WCUアプリを起動する
 * のをやめる
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  kagatume
 * 2004/02/12 12:30
 * comGUfunc実行中にWCUアプリ起動コマンドを送信しないようにする。
 * アップロード後、引き続きダウンロードを行うため。
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  kagatume
 * 2004/02/12 11:30
 * CFカードがない場合はダウンロードを行わないように
 * comGDfunc を変更する
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2004/01/22 20:30
 * Web リモコンからダウンロードできるように comGDfunc を変更する
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/01/15 19:30
 * comGUfunc を変更し、アップロードができるようにする
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/09 21:00
 * comGBfunc を修正する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/08 14:00
 * FPGAへのダウンロードに対応する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/07 16:00
 * fifo への移行に伴ってプログラムを整理する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/06 19:00
 * ワーニングをなくす
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/05 16:00
 * comGBfunc() を変更する
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
#include <stdio.h>				/* ANSI std	*/
#include <stdlib.h>				/* ANSI std	*/
#include <stddef.h>				/* ANSI std	*/
#include <string.h>				/*	"		*/
#include <ctype.h>				/*	"		*/
#include <custom\typedefs.h>	/*	"		*/
#include <dos_util.h>
#include <fsys.h>
#include <kernel.h>
#include "remmon.h"
#include "cmfhdl.h"
#include "download.h"
#include "rsinout.h"
#include "ledset.h"
#define STORAGE
#include "bootcmds.h"
#undef STORAGE
#include <sh7615\bootpara.h>
#include <sh7615\7615.h>
#include <drive\atadrv.h>
#include <drive\flash.h>

/*---------------------------------------------------------------
			Web-リモコンとのダウンロード・インターフェース
----------------------------------------------------------------*/
int recover_program(int err)
{
	int ret;
		
	char err_str[120], code[5];
	
	strcpy(err_str,"\nRCU>NG ");
	sprintf(code, "%d\n", err);
	strcat(err_str, code);

	/* バックアップファイルがあるかチェック */
	ret = chdir( (char*)DMUBKDIR );
	if(ret!=0) {
		rsstr(err_str);
		return -1;
	}
	
	rsstr("\nRCU>BACK 1\n");		/* 復旧開始 */
	rsstr("\nRCU>DISP Error!!\n");
	rsstr("\nRCU>DISP Recover from Backup File...\n");
	
	if (comGBfunc()) {		
		rsstr("\nRCU>BACK 3\n");	/* 復旧エラー */
		rsstr("\nRCU>DISP Program Recovery Failed.\n");
		rsstr(err_str);
	}
	
	rsstr("\nRCU>BACK 2\n");		/* 復旧終了*/
	rsstr("\nRCU>DISP Program Recovery Finished.\n");
	rsstr(err_str);
	return -1;
}

static int comGD_FPGA_download(void)
{
	char	buf[32];

	set_ledmode(FPGA_DOWNLOADING);

	rsstr("\n\nGet     FPGA Version...");
	if(comWCU_N('P')!=0){
		return recover_program(11);	/* FPGA didn't report version number */
	}

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		return recover_program(32);	/* DMU memory clear error */
	}
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		return recover_program(12);	/* WCU memory clear error */
	}

	rsstr("\nRCU>DISP 6:FPGA Program (RCU-CF -> DMU)\n");
	rsstr("\nRCU>SEND ");	rsstr(prgname[1]);	rsstr(".EXO");	rsstr("\n");
	if(comWCU_L('L')!=0){
		rscan();
		return recover_program(13);	/* FPGA Download error */
	}
	if(entryadrs != FPGA_START_CHECK){
		return recover_program(14);	/* FPGA Start Address Error */
	}
	rsstr("\nRCU>OK\n");
	setBAR(31);

	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[1]);
	strcat(buf,".EXO");
	if(tcpinput == 0){
		rsstr("\nRCU>DISP 7:FPGA Program ");	rsstr(buf+3);	rsstr(" (DMU -> DMU-CF)\n");
		if(comTfunc((char *)(MEM2STT-OFSADDR1),entryadrs,progsize,buf,0)!=0){
			return recover_program(15);	/* FPGA CF Write Error */
		}
	}
	setBAR(33);

	rsstr("\nRCU>DISP 8:FPGA Program ");	rsstr(buf+3);	rsstr(" (DMU -> FPGA)\n");
	if(comWCU_J('V')!=0){
		return recover_program(16);	/* FPGA Program Download Error */
	}
	setBAR(43);

	rsstr("\n\nGet     FPGA Version...");
	if(comWCU_N('P')!=0){
		return recover_program(11);	/* FPGA didn't report version number */
	}

	if(strcmp(prgname[1],tmpprogname)!=0){
		return recover_program(70);	/* FPGA download name check error */
	}
	if(strcmp(prgver[1],tmpversion)!=0){
		return recover_program(17);	/* FPGA download version check error */
	}
	if(strcmp(prgsum[1],tmpsumnum)!=0){
		return recover_program(18);	/* FPGA download sum check error */
	}

	return 0;
}

static int comGD_WCU_download(void)
{
	char	buf[32];

	set_ledmode(WCU_DOWNLOADING);

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		return recover_program(21);	/* WCU didn't report version number */
	}

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		return recover_program(32);	/* DMU memory clear error */
	}
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		return recover_program(22);	/* WCU memory clear error */
	}

	rsstr("\nRCU>DISP 10:WCU Program (RCU-CF -> DMU)\n");
	rsstr("\nRCU>SEND ");	rsstr(prgname[2]);	rsstr(".MOT");	rsstr("\n");
	if(comWCU_L('O')!=0){
		rscan();
		return recover_program(23);	/* WCU Download error */
	}
	if(entryadrs != WCU_START_CHECK){
		return recover_program(24);	/* WCU Start Address Error */
	}
	rsstr("\nRCU>OK\n");

	setBAR(44);
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[2]);
	strcat(buf,".MOT");
	if(tcpinput == 0){
		rsstr("\nRCU>DISP 11:WCU Program ");	rsstr(buf+3);	rsstr(" (DMU -> DMU-CF)\n");
		if(comTfunc((char *)(MEM2STT-OFSADDR3),entryadrs,progsize,buf,0)!=0){
			return recover_program(25);	/* WCU CF Write Error */
		}
	}
	setBAR(46);

	rsstr("\nRCU>DISP 12:WCU Program ");	rsstr(buf+3);	rsstr(" (DMU -> WCU)\n");
	if(comWCU_J('J')!=0){
		return recover_program(26);	/* WCU Program Download Error */
	}
	setBAR(60);

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')){
		return recover_program(21);	/* WCU didn't report version number */
	}

	if(strcmp(prgname[2],tmpprogname)!=0){
		return recover_program(71);	/* WCU download name check error */
	}
	if(strcmp(prgver[2],tmpversion)!=0){
		return recover_program(27);	/* WCU download version check error */
	}
	if(strcmp(prgsum[2],tmpsumnum)!=0){
		return recover_program(28);	/* WCU download sum check error */
	}

	return 0;
}

static int comGD_DMU_download(void)
{
	char	buf[32];
	int		i;
	
	set_ledmode(DMU_DOWNLOADING);
	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		return recover_program(32);	/* DMU memory clear error */
	}

	setBAR(61);
	rsstr("\nRCU>DISP 14:DMU Program (RCU-CF -> DMU)\n");
	rsstr("\nRCU>SEND ");	rsstr(prgname[3]);	rsstr(".MOT");	rsstr("\n");
	i = LNMEMSIZ * 11;
	i = memsize[3]/i + 1;
	if(comLfunc(MEM0STT,i)!=0){
		rscan();
		return recover_program(33);	/* DMU Download error */
	}
	if(entryadrs != DMU_START_CHECK){
		return recover_program(34);	/* DMU Start Address Error */
	}
	rsstr("\nRCU>OK\n");
	setBAR(73);

	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[3]);
	strcat(buf,".MOT");
	if(tcpinput == 0){
		rsstr("\nRCU>DISP 15:DMU Program ");	rsstr(buf+3);	rsstr(" (DMU -> DMU-CF)\n");
		i = LNMEMSIZ * 17;
		i = memsize[3]/i + 1;
		if(comTfunc((char *)MEM0STT,entryadrs,progsize,buf,i)!=0){
			return recover_program(35);	/* DMU CF Write Error */
		}
	}
	setBAR(91);

	i = progsize/(7*2) + 1;
	rsstr("\nRCU>DISP 16:DMU Program ");	rsstr(buf+3);	rsstr(" (DMU -> DMU)\n");
	if(comJfunc((short *)APLY_START_ADDR,(short *)(MEM0STT+entryadrs)
		,(unsigned long)progsize,i)!=0){
		return recover_program(36);	/* DMU Program Download Error */
	}
	setBAR(98);

	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);

	if(strcmp(prgname[3],tmpprogname)!=0){
		return recover_program(72);	/* DMU download name check error */
	}
	if(strcmp(prgver[3],tmpversion)!=0){
		return recover_program(37);	/* DMU download version check error */
	}
	if(strcmp(prgsum[3],tmpsumnum)!=0){
		return recover_program(38);	/* DMU download sum check error */
	}

	if(comKfunc((char *)DMU_START_CHECK)!=0){
		return recover_program(39);	/* Flash write error */
	}
}


int comGDfunc(void)
{
	ST_FILE	*fpw;
	char	buf[32],ret;
	int		reti,i,tmp;

	rsstr("\nRCU>DISP 1:Download Start. (RCU -> DMU)\n");
	setBAR(0);

	dly_tsk((RELTIM)1000);
	if(tmp = (FORCE_DOWNLOAD_ON)){		/* SW:ON  ブートローダーモード時*/
		return recover_program(73);	/* DMU DIPSW error */
	}
	
	if(PCCard_check()!=E_OK){	/* CFカードがない場合はダウンロードを行わない */
		return recover_program(40);	/* PCCard access error */
	}
	
	if(tcpinput == 0){
		/* ＣＦ内のプログラム・ファイルを/BOOTBAKにコピー */
		set_ledmode(MAKE_BACKUP);
		if(loadINF(CFINFNAME)<0) goto next0;
		reti = chdir( (char*)DMUBKDIR );
		if(reti==0) goto next0;			/* /BOOTBAKがすでにあれば作らない */
		rsstr("\nRCU>DISP 2:Backup Files. (DMU-CF)\n");
		ret = (char)mkdir( (char*)DMUBKDIR );
		if( ret ){
			return recover_program(60);	/* Make Directory Error */
		}
		strcpy(buf,DMUBKDIR); strcat(buf,"\\"); strcat(buf,BKNOW);
		fpw = fopen( (char*)buf, "w" );
		if( fpw == NULL ){
			return recover_program(62);	/* Can not Create File in CF */
		}
		if(fputs(buf,fpw)==EOF){
			return recover_program(64);	/* CF Write Error */
		}
		fclose(fpw);

		setBAR(1);
		if (prgname[1][0] != '\0') {
			strcpy(buf,prgname[1]);	strcat(buf,".EXO");
			if((tmp=filebkup(buf,0))>0) return recover_program(tmp);
		}
		if (prgname[2][0] != '\0') {
			strcpy(buf,prgname[2]);	strcat(buf,".MOT");
			if((tmp=filebkup(buf,0))>0) return recover_program(tmp);
		}
		setBAR(2);
		if (prgname[3][0] != '\0') {
			i = LNMEMSIZ * 24;
			i = memsize[3]/i + 1;
			strcpy(buf,prgname[3]);	strcat(buf,".MOT");
			if((tmp=filebkup(buf,i))>0) return recover_program(tmp);
		}
		setBAR(27);
		strcpy(buf,INFNAME);
		if((tmp=filebkup(buf,0))>0) return recover_program(tmp);

		strcpy(buf,DMUBKDIR); strcat(buf,"\\"); strcat(buf,BKNOW);
		remove( (char*)buf );

		if(loadINF(CFINFNAME)<0) goto next0;
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[1]); strcat(buf,".EXO");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove FPGAfile error.");
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[2]); strcat(buf,".MOT");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove WCUfile error.");
		setBAR(28);
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[3]); strcat(buf,".MOT");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove DMUfile error.");
		strcpy(buf,CFDIRNAME); strcat(buf,INFNAME);
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove INFfile error.");
	}

next0:
	setBAR(30);
	rsstr("\nRCU>DISP 4:PROGRAM.INF (RCU-CF -> DMU)\n");
	rsstr("\nRCU>SEND PROGRAM.INF\n");
	if(recvINF()<0){			/* SUM CHECK */
		return recover_program(1);	/* INF file Error */
	}
	else{
		rsstr("\nRCU>OK\n");
	}
	rsstr("\nRCU>DISP 5:PROGRAM.INF (DMU -> DMU-CF)\n");

	if (prgname[1][0] != '\0') {
		if (comGD_FPGA_download() != 0) return -1;
	}
	if (prgname[2][0] != '\0') {
		if (comGD_WCU_download() != 0) return -1;
	}
	if (prgname[3][0] != '\0') {
		if (comGD_DMU_download() != 0) return -1;
	}

	setBAR(99);
	if(tcpinput == 0){
		/* /BOOTBAKを消去する */
		rsstr("\nRCU>DISP 17:Remove Backup Files. (DMU-CF)\n");
		rmbkall();
	}
	rsstr("\nRCU>DISP 18:Download Finished. (RCU -> DMU)\n");
	setBAR(100);
	set_mode(RESET_CMD);
	set_ledmode(BOOTLOADER);
	return(0);
}


/*---------------------------------------------------------------
			Web-リモコンとのアップロード・インターフェース
----------------------------------------------------------------*/
static int comGU_FPGA_progcheck(void)
{
	rsstr("\n\nGet     FPGA Version...");
	if(comWCU_N('P')!=0){
		rsstr("\nRCU>NG 43\n");	/* FPGA Get Version error */
		return -1;
	}
	if(strcmp(prgname[1],tmpprogname)!=0) goto next1;
	if(strcmp(prgver[1],tmpversion)!=0) goto next1;
	if(strcmp(prgsum[1],tmpsumnum)!=0) goto next1;
	rsstr("No Difference.");
	return 0;
	
next1:
	rsstr("\nRCU>NG 44\n");		/* Do not match FPGA and INF file */
	return -1;
}

static int comGU_WCU_progcheck(void)
{
	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		rsstr("\nRCU>NG 47\n");	/* WCU Get Version error */
		return(-1);
	}
	if(strcmp(prgname[2],tmpprogname)!=0) goto next4;
	if(strcmp(prgver[2],tmpversion)!=0) goto next4;
	if(strcmp(prgsum[2],tmpsumnum)!=0) goto next4;
	rsstr("No Difference.");
	return 0;
	
next4:
	rsstr("\nRCU>NG 48\n");		/* Do not match WCU and INF file */
	return(-1);
}

static int comGU_DMU_progcheck(void)
{
	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);
	if(strcmp(prgname[3],tmpprogname)!=0) goto next7;
	if(strcmp(prgver[3],tmpversion)!=0) goto next7;
	if(strcmp(prgsum[3],tmpsumnum)!=0) goto next7;
	rsstr("No Difference.");
	return 0;

next7:
	rsstr("\nRCU>NG 50\n");		/* Do not match DMU and INF file */
	return(-1);
}


int comGUfunc(void)
{
	char	buf[32];

	rsstr("\nRCU>DISP 100:Upload Start. (DMU-CF -> RCU)\n");
	setBAR(0);

	if(PCCard_check()!=E_OK){
		rsstr("\nRCU>NG 40\n");	/* PCCard access error */
		return(-1);
	}

	rsstr("\nRCU>DISP 101:Check Files. (DMU-CF)\n");
	if(loadINF(CFINFNAME)<0){
		rsstr("\nRCU>NG 41\n");	/* Load INF error */
		return(-1);
	}
	
	if (prgname[1][0] != '\0') {
		if (comGU_FPGA_progcheck() != 0) return -1;
	}
	if (prgname[2][0] != '\0') {
		if (comGU_WCU_progcheck() != 0) return -1;
	}
	if (prgname[3][0] != '\0') {
		if (comGU_DMU_progcheck() != 0) return -1;
	}

	setBAR(5);

	if(upldcfasc(CFINFNAME,0)<0) return(-1);

	setBAR(10);
	if (prgname[1][0] != '\0') {
		strcpy(buf,CFDIRNAME);	strcat(buf,prgname[1]);	strcat(buf,".EXO");
		if(upldcfasc(buf,0)<0) return(-1);
	}
	setBAR(40);
	if (prgname[2][0] != '\0') {
		strcpy(buf,CFDIRNAME);	strcat(buf,prgname[2]);	strcat(buf,".MOT");
		if(upldcfasc(buf,0)<0) return(-1);
	}
	setBAR(70);
	if (prgname[3][0] != '\0') {
	//	i = LNMEMSIZ * 16;
	//	i = memsize[3]/i + 1;
		strcpy(buf,CFDIRNAME);	strcat(buf,prgname[3]);	strcat(buf,".MOT");
	//	if(upldcfasc(buf,i)<0) return(-1);
		if(upldcfasc(buf,0)<0) return(-1);
	}
	
	rsstr("\nRCU>DISP 103:Upload Finished. (DMU-CF -> RCU)\n");
	setBAR(100);
	return(0);
}


/*---------------------------------------------------------------
			DMU起動時にＣＦのＩＮＦでサムチェック・ダウンロード
----------------------------------------------------------------*/
static int comGB_FPGA_download(void)
{
	char	buf[120];

	set_ledmode(FPGA_DOWNLOADING);

	rsstr("\n\nGet     FPGA Version...");
	if(comWCU_N('P')!=0){
		rsstr("\nFPGA didn't report version number\n");
		return(-1);
	}
	if(strcmp(prgname[1],tmpprogname)!=0) goto next1;
	if(strcmp(prgver[1],tmpversion)!=0) goto next1;
	if(strcmp(prgsum[1],tmpsumnum)!=0) goto next1;
	rsstr("\nNo Difference.");
	return 0;

next1:
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return(-1);
	}

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[1]);
	strcat(buf,".EXO");
	rsstr("\nRead CF FPGAfile ");	rsstr(buf);	rsstr(" ...");
	if(comWCU_F('L', buf)!=0) {
		rsstr("\nRead CF FPGAfile error\n");
		return(-1);
	}

	rsstr("\n\nDownload  FPGA Aplication...");
	if(comWCU_J('V')!=0){
		rsstr("\nFPGA Program Download Error\n");
		return(-1);
	}

	rsstr("\n\nGet     FPGA Version...");
	if(comWCU_N('P')!=0){
		rsstr("\nFPGA Get Version error\n");
		return(-1);
	}
	if(strcmp(prgname[1],tmpprogname)!=0){
		rsstr("\nFPGA download name check error\n");
		return(-1);
	}
	if(strcmp(prgver[1],tmpversion)!=0){
		rsstr("\nFPGA download version check error\n");
		return(-1);
	}
	if(strcmp(prgsum[1],tmpsumnum)!=0){
		rsstr("\nFPGA download sum check error\n");
		return(-1);
	}
	
	return 0;
}

static int comGB_WCU_download(void)
{
	char	buf[120];

	set_ledmode(WCU_DOWNLOADING);

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		rsstr("\nWCU didn't report version number\n");
		return(-1);
	}
	if(strcmp(prgname[2],tmpprogname)!=0) goto next4;
	if(strcmp(prgver[2],tmpversion)!=0) goto next4;
	if(strcmp(prgsum[2],tmpsumnum)!=0) goto next4;
	rsstr("\nNo Difference.");
	goto next5;
	
next4:
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return(-1);
	}

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[2]);
	strcat(buf,".MOT");
	rsstr("\nRead CF WCUfile ");	rsstr(buf);	rsstr(" ...");
	if(comWCU_F('O', buf)!=0) {
		rsstr("\nRead CF WCUfile error\n");
		return(-1);
	}
	
	rsstr("\n\nDownload  WCU Aplication...");
	if(comWCU_J('J')!=0){
		rsstr("\nWCU Program Download Error\n");
		return(-1);
	}

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		rsstr("\nWCU Get Version error\n");
		return(-1);
	}
	if(strcmp(prgname[2],tmpprogname)!=0){
		rsstr("\nWCU download name check error\n");
		return(-1);
	}
	if(strcmp(prgver[2],tmpversion)!=0){
		rsstr("\nWCU download version check error\n");
		return(-1);
	}
	if(strcmp(prgsum[2],tmpsumnum)!=0){
		rsstr("\nWCU download sum check error\n");
		return(-1);
	}

next5:
	rsstr("\nStart WCU Aplication.");
	if(comWCU_K()!=0){
		rsstr("\nStart WCU Aplication error\n");
		return(-1);
	}
	return 0;
}

static int comGB_DMU_download(void)
{
	char	buf[120];

	set_ledmode(DMU_DOWNLOADING);

	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);
	if(strcmp(prgname[3],tmpprogname)!=0) goto next7;
	if(strcmp(prgver[3],tmpversion)!=0) goto next7;
	if(strcmp(prgsum[3],tmpsumnum)!=0) goto next7;
	rsstr("\nNo Difference.");
	return 0;
	
next7:
	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0) {
		rsstr("\nDMU memory clear error\n");
		return(-1);
	}
	
	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[3]);
	strcat(buf,".MOT");
	rsstr("\nRead CF DMUfile ");	rsstr(buf);	rsstr(" ...");
	if(comFfunc(MEM0STT, buf)!=0) {
		rsstr("\nRead CF DMUfile error\n");
		return(-1);
	}
	if(entryadrs != DMU_START_CHECK){
		rsstr("\nDMU Start Address Error\n");
		return(-1);
	}
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	rsstr("\n\nDownload  DMU Aplication...");
	if(comJfunc((short *)APLY_START_ADDR,(short *)(MEM0STT+entryadrs)
		,(unsigned long)progsize,0)!=0) {
		rsstr("\nDMU Program Download Error\n");
		return(-1);
	}
			
	/* BOOT_APLY_ADDRにアプリ開始アドレスをセットする */
	if(comKfunc((char *)DMU_START_CHECK)!=0){
		rsstr("\nFlash write error\n");
		return(-1);
	}
	
	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);
	if(strcmp(prgname[3],tmpprogname)!=0){
		rsstr("\nDMU download name check error\n");
		return(-1);
	}
	if(strcmp(prgver[3],tmpversion)!=0){
		rsstr("\nDMU download version check error\n");
		return(-1);
	}
	if(strcmp(prgsum[3],tmpsumnum)!=0){
		rsstr("\nDMU download sum check error\n");
		return(-1);
	}
	
	return 0;
}


int comGBfunc(void)
{
	ST_FILE	*fpr;
	char	buf[120],ret;
	int		reti;

	if(PCCard_check()!=E_OK){
		rsstr("\nPCCard access error\n");
		return(-1);
	}

	if(tcpinput == 0){
		/* /BOOTBAKがあればルートにコピーして復帰させる */
		reti = chdir( (char*)DMUBKDIR );
		if(reti!=0) goto next0;
		
		set_ledmode(RET_BACKUP);
		rsstr("\nCheck Backup Boot Files ...\n");
		strcpy(buf,DMUBKDIR); strcat(buf,"\\"); strcat(buf,BKNOW);
		fpr = fopen( (char*)buf, "r" );
		if( fpr != NULL ){		/* バックアップ途中で中断 */
			fclose(fpr);
			rsstr("\nBROKEN File exist\n");		
			rmbkall();
			goto next0;
		}
		strcpy(buf,DMUBKDIR);	strcat(buf,"\\");	strcat(buf,INFNAME);
		if(loadINF(buf)<0){		/* バックアップしたＩＮＦファイルのエラー */
			rsstr("\nBackup INF error\n");
			rmbkall();
			goto next0;
		}
		if(loadINF(CFINFNAME)<0) goto next00;
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[1]); strcat(buf,".EXO");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove FPGAfile error.");
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[2]); strcat(buf,".MOT");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove WCUfile error.");
		strcpy(buf,CFDIRNAME); strcat(buf,prgname[3]); strcat(buf,".MOT");
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove DMUfile error.");
		strcpy(buf,CFDIRNAME); strcat(buf,INFNAME);
		ret = (char)remove( (char*)buf );
		if( ret ) rsstr("\nremove INFfile error.");
next00:
		strcpy(buf,DMUBKDIR);	strcat(buf,"\\");	strcat(buf,INFNAME);
		if(loadINF(buf)<0){		/* バックアップしたＩＮＦファイルのエラー */
			rsstr("\nBackup INF error...\n");
			rmbkall();
			goto next0;
		}
		if (prgname[1][0] != '\0') {
			strcpy(buf,prgname[1]);	strcat(buf,".EXO");
			if(filersm(buf)<0) return(-1);
		}
		if (prgname[2][0] != '\0') {
			strcpy(buf,prgname[2]);	strcat(buf,".MOT");
			if(filersm(buf)<0) return(-1);
		}
		if (prgname[3][0] != '\0') {
			strcpy(buf,prgname[3]);	strcat(buf,".MOT");
			if(filersm(buf)<0) return(-1);
		}
		strcpy(buf,INFNAME);
		if(filersm(buf)<0) return(-1);
		rmbkall();
	}

next0:
	rsstr("\nLoad INF file from Compact Flash\n");
	if(loadINF(CFINFNAME)<0) {
		rsstr("\nLoad INF error\n");
		return(-1);
	}
	
	if (prgname[1][0] != '\0') {
		if (comGB_FPGA_download() != 0) return -1;
	}
	if (prgname[2][0] != '\0') {
		if (comGB_WCU_download() != 0) return -1;
	}
	if (prgname[3][0] != '\0') {
		if (comGB_DMU_download() != 0) return -1;
	}
	
	if(comKfunc((char *)DMU_START_CHECK)!=0){
		rsstr("\nFlash write error\n");
		return(-1);
	}

	rsstr("\nStart DMU Aplication.");
	set_mode(RESET_CMD);
	set_ledmode(BOOTLOADER);
	return(0);
}


