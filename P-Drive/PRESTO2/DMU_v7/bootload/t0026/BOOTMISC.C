/*======================================================================
 * File name    : bootmisc.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	: 
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/bootmisc.c 3.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootmisc.c $
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * ・電断対応ファイルシステム搭載
 * ・インストール時間短縮を行う
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/19 17:00
 * ソフトの簡易ダウンロード・シーケンスcomEの作成
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・BOOT_CHECK_ADDRへの状態文字列セット処理を削除する。
 * ・BOOT_WRITE_STATUSにブートローダー書き換え用ステータス文字列を
 *   セットするようにする。
 * ・不要なコマンド処理関数を削除する。
 * ・不要な変数を削除する。
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/10 18:30
 * ブートローダー書き換え時のteraterm出力を変更する。
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/09 16:00
 * コメントを変更する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:00
 * ブートローダーの書き換えができるようにする
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_resetをdmu_jump_to_resetに変更する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/22 20:40
 * FROM_TRAN_BLOCK を FLASH_WRITE_BLOCK に変更する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
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
#include <string.h>				/* ANSI std	*/
#include <kernel.h>
#include <drive\atadrv.h>
#include <drive\flash.h>
#include <sh7615\bootprm.h>
#include <sh7615\dmupara.h>
#include "download.h"
#include "bootcmds.h"
#include "rsinout.h"
#include "ledset.h"


/*---------------------------------------------------------------
				ソフトの簡易ダウンロード・シーケンス
----------------------------------------------------------------*/
static int comE_FPGA_download(void)
{
	char	buf[32];

	set_ledmode(FPGA_DOWNLOADING);

	rsstr("\n\nGet     FPGA Version Number...");
	if(comWCU_N('P')!=0){
		rsstr("\nFPGA didn't report version number\n");
		return -1;
	}

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return -1;
	}
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return -1;
	}

	rsstr("\nSEND FPGA Program.\n");
	if(comWCU_L('L')!=0){
		rscan();
		rsstr("\nFPGA Download error\n");
		return -1;
	}
	if(entryadrs != FPGA_START_CHECK){
		rsstr("\nFPGA Start Address Error\n");
		return -1;
	}

	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	strcpy(buf,CFDIRNAME);
	strcat(buf,prgname[1]);
	strcat(buf,".EXO");

	if(comWCU_J('V')!=0){
		rsstr("\nFPGA Program Download Error\n");
		return -1;
	}

	rsstr("\n\nGet     FPGA Version Number...");
	if(comWCU_N('P')!=0){
		rsstr("\nFPGA didn't report version number\n");
		return -1;
	}

	return 0;
}

static int comE_WCU_download(void)
{
	set_ledmode(WCU_DOWNLOADING);

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		rsstr("\nWCU didn't report version number\n");
		return -1;
	}

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return -1;
	}
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return -1;
	}

	rsstr("\nSEND WCU Program.\n");
	if(comWCU_L('O')!=0){
		rscan();
		rsstr("\nWCU Download error\n");
		return -1;
	}
	if(entryadrs != WCU_START_CHECK){
		rsstr("\nWCU Start Address Error\n");
		return -1;
	}

	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	if(comWCU_J('J')!=0){
		rsstr("\nWCU Program Download Error\n");
		return -1;
	}

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')){
		rsstr("\nWCU didn't report version number\n");
		return -1;
	}

	return 0;
}

static int comE_DMU_download(void)
{	
	set_ledmode(DMU_DOWNLOADING);
	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return -1;
	}

	rsstr("\nSEND DMU Program.\n");
	if(comLfunc(MEM0STT,0)!=0){
		rscan();
		rsstr("\nDMU Download error\n");
		return -1;
	}
	if(entryadrs != DMU_START_CHECK){
		rsstr("\nDMU Start Address Error\n");
		return -1;
	}

	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	if(comJfunc((short *)APLY_START_ADDR,(short *)(MEM0STT+entryadrs)
		,(unsigned long)progsize,0)!=0){
		rsstr("\nDMU Program Download Error\n");
		return -1;
	}

	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */
	pickversion(versionstr);

	if(comKfunc((char *)DMU_START_CHECK)!=0){
		rsstr("\nFlash write error\n");
		return -1;
	}
}


int comE(void)
{
	rsstr("\nDownload Start.\n");

	if(PCCard_check()==E_OK){	/* CFカードがある場合はダウンロードを行わない */
		rsstr("\nPCCard exist error\n");
		return -1;
	}
	
	rsstr("\nDownload FPGA program? (y or n)\n");
	if (wait_answer() == 0) {
		if (comE_FPGA_download() != 0) return -1;
	}
	rsstr("\nDownload WCU program? (y or n)\n");
	if (wait_answer() == 0) {
		if (comE_WCU_download() != 0) return -1;
	}
	rsstr("\nDownload DMU program? (y or n)\n");
	if (wait_answer() == 0) {
		if (comE_DMU_download() != 0) return -1;
	}

	rsstr("\nDownload Finished.\n");
	set_mode(RESET_CMD);
	set_ledmode(BOOTLOADER);
	return(0);
}


/*---------------------------------------------------------------
				WCUブートローダー書き換え処理
----------------------------------------------------------------*/
int comG2func(void)
{
	if (comGZ2func()) return -1;
	if (comBTwrite2()) return -1;
	if (comGZfunc()) return -1;
	return 0;
}


/*---------------------------------------------------------------
		WCUブートライターダウンロード処理
----------------------------------------------------------------*/
int comGZ2func(void)
{		
	int ret;
	
	ret = ret;
	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	rsstr("\nTurn WCU to Bootloader...");

	rsstr("\n\nGet     WCU Version...");
	if(comWCU_N('N')!=0){
		rsstr("\nWCU didn't report version number\n");
		return(-1);
	}
	
	/* ブートライターの読み込み */
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF WCU BTwriter ");	rsstr(WCUBTWRNM);	rsstr(" ...");
		if(comWCU_F('M', WCUBTWRNM)!=0) {
			rsstr("\nRead CF file erro\nr");
			return(-1);
		}
	}
	else{
		rsstr("\n\nClear Memory for Check DMU ...");
		if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
			rsstr("\nDMU memory clear error\n");
			return(-1);
		}
		rsstr("\n\nSend WCU BTwriter ");	rsstr(WCUBTWRNM);	rsstr(" ...");
		if(comWCU_L('M')!=0){
			rsstr("\nDownload error\n");
			rscan();
			return(-1);
		}
	}

#ifdef BACKUP_ON
	/* アプリのバックアップ */
	rsstr("\nBackup WCU  Aplication...");

	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return(-1);
	}

	ret = comWCU_U();
	if(ret==-1) {
		rsstr("\nBackup error\n");
		return(-1);
	}
	if(ret== 0) {
		if(tcpinput == 0){
			rsstr("\n\nWrite CF ");	rsstr(WCUBKNAME);	rsstr(" ...");
			if(comTfunc((char *)(MEM2STT-OFSADDR3),entryadrs,progsize,WCUBKNAME,0)!=0) return(-1);
		}
		else{
			rsstr("\n\nReceive ");	rsstr(WCUBKNAME);	rsstr(" ...");
			if(comHfunc((char *)(MEM2STT-OFSADDR3),entryadrs,progsize,WCUBKNAME)!=0);
			rsstr("\n\n=Backup File End=");
		}
	}
#endif
	
	/* ブートライターのダウンロード */
	rsstr("\n\nDownload  WCU BTwriter...");
	if(comWCU_J('D')!=0) {
		rsstr("\nBTwriter Program Download Error\n");
		return(-1);
	}
	
	/* ブートライターの起動 */
	if(comWCU_K()!=0) {
		rsstr("\nStart WCU BTwriter error\n");
		return(-1);
	}
	rsstr("\n\nStart WCU BTwriter.");
	return(0);
}


/*---------------------------------------------------------------
		WCUブートローダーダウンロード処理
----------------------------------------------------------------*/
int comBTwrite2(void)
{
	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	/* ブートローダーの読み込み */
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF WCU Bootloader ");	rsstr(WCUBTNAME);	rsstr(" ...");
		if(comWCU_F('L', WCUBTNAME)!=0) {
			rsstr("\nRead CF file error\n");
			return(-1);
		}
	}
	else{
		rsstr("\n\nClear Memory for Check DMU ...");
		if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
			rsstr("\nDMU memory clear error\n");
			return(-1);
		}	
		rsstr("\n\nSend WCU Bootloader ");	rsstr(WCUBTNAME);	rsstr(" ...");
		if(comWCU_L('L')!=0){
			rsstr("\nDownload error\n");
			rscan();
			return(-1);
		}
	}

	/* ブートローダーのダウンロード */
	rsstr("\n\nDownload  WCU Bootloader...");
	if(comWCU_J('B')!=0) {
		rsstr("\nBootloader Program Download Error\n");
		return(-1);
	}

	/* ブートローダーの起動 */	
	rsstr("\nStart WCU Bootloader.");
	if(comWCU_R()!=0) {
		rsstr("\nStart WCU Bootloader Error\n");
		return(-1);
	}
	return 0;
}


/*---------------------------------------------------------------
		WCUアプリ復元処理
----------------------------------------------------------------*/
int comGZfunc(void)
{
	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	rsstr("\n\nRestore WCU Aplication...");

	/* WCUアプリの読み込み */
	rsstr("\n\nClear Memory for Check WCU ...");
	if(comWCU_A()!=0){
		rsstr("\nWCU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF WCUfile ");	rsstr(WCUBKNAME);	rsstr(" ...");
		if(comWCU_F('O', WCUBKNAME)!=0) {
			rsstr("\nRead CF file error\n");
			return(-1);
		}
	}
	else{
		rsstr("\n\nClear Memory for Check DMU ...");
		if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
			rsstr("\nDMU memory clear error\n");
			return(-1);
		}
		rsstr("\n\nSend WCUfile ");	rsstr(WCUBKNAME);	rsstr(" ...");
		if(comWCU_L('O')!=0){
			rsstr("\nDownload error\n");
			rscan();
			return(-1);
		}
	}

	/* WCUアプリのダウンロード */
	rsstr("\n\nDownload  WCU Aplication...");
	if(comWCU_J('J')!=0) {
		rsstr("\nWCU Program Download Error\n");
		return(-1);
	}
	
	/* WCUアプリの起動 */	
	rsstr("\nStart WCU Aplication.");
	if(comWCU_K()!=0) {
		rsstr("\nStart WCU Aplication error\n");
		return(-1);
	}
			
	rsstr("\n\nWCU BootLoader Download Complete.\n\n");
	rsstr("\nStart DMU Aplication.");
	
	/* 再起動 */	
	dly_tsk(100);
	set_mode(RESET_CMD);
	dmu_jump_to_reset();
	return(0);
}


/*---------------------------------------------------------------
		DMUブートライターダウンロード処理
----------------------------------------------------------------*/
int comGZ3func(void)
{		
	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	rsstr("\n\nGet     DMU Version...");
	comN();			/* BOOT_VERSION_ADDR,versionstr */

	/* ブートライターの読み込み */
	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF DMU BTwriter ");	rsstr(DMUBTWRNM);	rsstr(" ...");
		if(comFfunc(MEM0STT, DMUBTWRNM)!=0) {
			rsstr("\nRead CF file error\n");
			return(-1);
		}
	}
	else{
		rsstr("\n\nSend DMU BTwriter ");	rsstr(DMUBTWRNM);	rsstr(" ...");
		if(comLfunc(MEM0STT,0)!=0){
			rsstr("\nDMU Download error\n");
			rscan();
			return(-1);
		}
	}
	
	if(entryadrs != DMU_START_CHECK) {
		rsstr("\nFlash write error\n");
		return(-1);
	}
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);
	entryaaa = DMU_START_CHECK;
	progsss = *((int *)APLY_SIZE_ADDR);
	
#ifdef BACKUP_ON
	/* アプリのバックアップ */
	/* DMUアプリが書き込まれているかチェックする。なければアプリのバックアップは行わない。 */
	if(progsss>=(int)0x00100000 || progsss<=(int)0x00001000) {
		rsstr("\nNo DMU Aplication\n");
		goto next11;
	}
	rsstr("\n\nBackup DMU  Aplication...");
	if(tcpinput == 0){
		rsstr("\n\nWrite CF ");	rsstr(DMUBKNAME);	rsstr(" ...");
		if(comTfunc((char *)BOOT_START_ADDR,entryaaa,progsss,DMUBKNAME,0)!=0) return(-1);
	}
	else{
		rsstr("\n\nReceive ");	rsstr(DMUBKNAME);	rsstr(" ...");
		if(comHfunc((char *)BOOT_START_ADDR,entryaaa,progsss,DMUBKNAME)!=0);
		rsstr("\n\n=Backup File End=");
	}
#endif

next11:
	/* ブートライターのダウンロード */
	rsstr("\n\nDownload  DMU BTwriter...");
	if(comJfunc((short *)APLY_START_ADDR,(short *)(MEM0STT+entryadrs)
		,(unsigned long)progsize,0)!=0) {
		rsstr("\nDMU BTwriter Download Error\n");
		return(-1);
	}
	
	if(comKfunc((char *)DMU_START_CHECK)!=0) {
		rsstr("\nFlash write error\n");
		return(-1);
	}

	/* ブートライターの起動 */		
	rsstr("\nStart DMU BTwriter.");
	set_mode(RESET_CMD);
	strcpy( BOOT_WRITE_STATUS, "BTWrite" );
	dmu_jump_to_reset();
	return(0);
}


/*---------------------------------------------------------------
		DMUブートローダーダウンロード処理
----------------------------------------------------------------*/
int comBTwrite3(void)
{
	strcpy( BOOT_WRITE_STATUS, "       " ); /* 空白７文字をセット */

	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	/* ブートローダーの読み込み */
	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF DMU Bootloader ");	rsstr(DMUBTNAME);	rsstr(" ...");
		if(comFfunc(MEM2STT, DMUBTNAME)!=0) {
			rsstr("\nRead CF file error\n");
			return(-1);
		}
	}
	else{
		rsstr("\n\nSend DMU Bootloader ");	rsstr(DMUBTNAME);	rsstr(" ...");
		if(comLfunc(MEM2STT,0)!=0){
			rsstr("\nDownload error\n");
			rscan();
			return(-1);
		}
	}
	if(entryadrs != (int)OFSADDR1) {
		rsstr("\nFlash write error\n");
		return(-1);
	}
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	/* ブートローダーのダウンロード */
	rsstr("\n\nDownload  DMU Bootloader...");
	if(comJfunc((short *)BOOT_START_ADDR,(short *)(MEM2STT+entryadrs)
		,(unsigned long)progsize,0)!=0) {
		rsstr("\nDMU Bootloader Download Error\n");
		return(-1);
	}
		
	/* ブートローダーの起動 */		
	rsstr("\nStart DMU Bootloader.");
	set_mode(RESET_CMD);
	strcpy( BOOT_WRITE_STATUS, "ResmApl" );
	dmu_jump_to_reset();
}


/*---------------------------------------------------------------
		DMUアプリ復元処理
----------------------------------------------------------------*/
int comGYfunc(void)
{
	strcpy( BOOT_WRITE_STATUS, "       " ); /* 空白７文字をセット */

	dly_tsk((RELTIM)1000);
	if(PCCard_check()!=E_OK) tcpinput = 1;
	else tcpinput = tcpinputOK;

	rsstr("\n\nRestore DMU Aplication...");

	/* DMUアプリの読み込み */
	rsstr("\n\nClear Memory for Check DMU ...");
	if(comAfunc((int *)(MEM2STT),(int *)(MEM3STP),(int)0xffffffff,(int)0xffffffff,0)!=0){
		rsstr("\nDMU memory clear error\n");
		return(-1);
	}

	if(tcpinput == 0){
		rsstr("\n\nRead CF DMUfile ");	rsstr(DMUBKNAME);	rsstr(" ...");
		if(comFfunc(MEM0STT, DMUBKNAME)!=0) {
			rsstr("\nRead CF file error\n");
			return(-1);
		}
	}
	else{
		rsstr("\n\nSend DMUfile ");	rsstr(DMUBKNAME);	rsstr(" ...");
		if(comLfunc(MEM0STT,0)!=0){
			rsstr("\nDownload error\n");
			rscan();
			return(-1);
		}
	}
	if(entryadrs != DMU_START_CHECK) {
		rsstr("\nFlash write error\n");
		return(-1);
	}
	progsize = calprogsize(progsize,FLASH_WRITE_BLOCK);

	/* DMUアプリのダウンロード */
	rsstr("\n\nDownload  DMU Aplication...");
	if(comJfunc((short *)APLY_START_ADDR,(short *)(MEM0STT+entryadrs)
		,(unsigned long)progsize,0)!=0) {
		rsstr("\nDMU Aplication Download Error\n");
		return(-1);
	}
	
	if(comKfunc((char *)DMU_START_CHECK)!=0) {
		rsstr("\nFlash write error\n");
		return(-1);
	}
	rsstr("\n\nDMU BootLoader Download Complete.\n\n");

	/* DMUアプリの起動 */		
	rsstr("\nStart DMU Aplication.");
	dly_tsk(100);
	set_mode(RESET_CMD);
	dmu_jump_to_reset();
	return(0);
}


