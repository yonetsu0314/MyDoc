/*======================================================================
 * File name    : init.c
 * Original		: p:/dacs_i/rcu/main/rcs/init.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		初期設定ルーチン（FOR SH7615）
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu_v7/bootload/rcs/init.c 3.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: init.c $
 * リビジョン 3.2  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * ソフトウェア封印に対応するためnetwork設定のバックアップをFLASHから
 * E2ROMへ移動する
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/04/21 15:00
 * 関数wcu_turn_bootloaderにおいてWCUへのブートローダー移行コマンド送信前の
 * wait時間を増やす。WCUアプリで行っているfifoバッファのクリア処理前に
 * ブートローダー移行コマンドを送信してもWCUアプリで正常に受け取るとこができ
 * ないため。
 * 
 * リビジョン 2.8  1970/01/01  00:00:00  kagatume
 * 2004/03/24 14:00
 * aply_start_checkにおいてcomGBfuncエラー発生時はＬＥＤをエラーパターンに
 * 設定する。
 * 
 * リビジョン 2.7  1970/01/01  00:00:00  kagatume
 * 2004/03/18 14:00
 * サフィックスI,O,Qに相当するバージョン番号10、16、18は使用しないことに
 * 伴ってgetversionstrを変更する。
 * 
 * リビジョン 2.6  1970/01/01  00:00:00  kagatume
 * 2004/03/16 18:30
 * アプリ起動モードにおいてもdipswの設定によってデフォルトIPで
 * 立ち上げられるようにする。
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * ・BOOT_CHECK_ADDRへの状態文字列セット処理を削除する。
 * ・BOOT_WRITE_STATUSにブートローダー書き換え用ステータス文字列を
 *   セットするようにする。
 * ・APLY_START_STATUSにアプリスタートステータス文字列をセットするようにする。
 * ・アプリへのジャンプはマルチタスク状態になる前に行うようにする。
 *   それに伴って、setStkGoを削除し、ステータス文字列を新規に設ける。
 * ・IPアドレス設定に関する処理をイーサネット初期化前に行うようにする。
 * ・dipswの設定によってデフォルトIPで立ち上げられるようにする。
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/10 18:30
 * aply_start_checkにおいて状態文字列をシリアル表示する処理を
 * コメントアウトする。
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/09 16:00
 * ・aply_start_checkを整理する
 * ・start_aplyにTPUを初期化する処理を追加する
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/08 20:00
 * 条件コンパイル時のワーニングをなくす
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * 起動時のモード判別処理を整理する
 * 
 * リビジョン 1.18  1970/01/01  00:00:00  kagatume
 * 2004/02/17 19:00
 * DMUアプリのサム値エラーに対応できるようにエラーが生じたことを
 * 記憶する記述を追加する。エラーが生じている場合は、DMUのCFから
 * 正常なプログラムをダウンロードする。
 * 
 * リビジョン 1.17  1970/01/01  00:00:00  sakaguti
 * JTAGﾃﾞﾊﾞｯｸﾞ容易化のためｱﾌﾟﾘｼﾞｬﾝﾌﾟはBOOT_APLY_ADDRを用いる
 * 
 * リビジョン 1.16  1970/01/01  00:00:00  kagatume
 * 2004/02/06 17:00
 * 先リビジョンにおいてFIFO初期化後にブートローダー移行コマンドを
 * 送信するように変更
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  kagatume
 * 2004/02/06 12:10
 * DMUアプリからＷＤＴによるリセットがかかった場合は、
 * ＷＣＵをブートローダーに移行させるようにする
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  kagatume
 * 2004/01/27 13:30
 * アプリでＷＤＴによるリセットをかけた時には
 * ブートローダーを実行するようにINITを変更する
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2003/12/19 14:30
 * ＷＣＵブートローダーからの送信データを受信する処理を追加する
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2003/12/12 19:00
 * WCUとのシリアル通信用変数を追加する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2003/11/21 19:30
 * アプリへジャンプするようにinit()を変更する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2003/11/18 14:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  sakaguti
 * 電池電圧が高くてもｻﾑ値ﾁｪｯｸが通るように１０回ﾘﾄﾗｲする
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * ﾌﾞｰﾄﾛｰﾀﾞのｴﾗｰﾒｯｾｰｼﾞ追加
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * WDT状態の取得をやめ文字列コマンド追加
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * ライブラリと重複するwait()等を削除
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * bootpara.hをshinc\sh7615に変更
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 包装機用拡張
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */

#include <string.h>
#include <kernel.h>
#include <custom\typedefs.h>
#include <intrpt.h>
#include <sh7615\7615.h>
#include <sh7615\sh7615.h>
#include <sh7615\bootpara.h>
#include <sh7615\flash.h>
#include <drive\atadrv.h>
#include <sh7615\dmupara.h>
#include <usnets\local.h>

#include "remmon.h"
#include "cmfhdl.h"
#include "rsinout.h"
#include "init.h"
#include "download.h"
#include "bootcmds.h"
#include "ledset.h"

/*---------------------------------------------------------------
				アプリスタートチェック
----------------------------------------------------------------*/
static void wcu_turn_bootloader(void)
{
	dly_tsk(2500);			/* WCUアプリ起動待ち時間（WCUではブートローダーで起動するか、
								アプリを立ち上げるかの判断に１秒必要） */		
	send_wcu_boot_cmd();	/* WCUをWDTによりブートローダーへ移行させる */
	dly_tsk(1000);			/* WCUのブートローダー起動待ち時間 */
}

int aply_start_check(void)
{
	return -1;
}


/*---------------------------------------------------------------
				アプリスタート
----------------------------------------------------------------*/
void start_aply(void)
{
	int		i;
	struct	apl_info	*ap;
	V_FUNC			*vec;
	void			(*apl)();

	rsstrdir("Jump to Aply.\n");
	strcpy( APLY_START_STATUS, "       " ); /* 空白７文字をセット */

	ap = (struct apl_info *)(*((APLY*)BOOT_APLY_ADDR));
	vec = ap->vecter_adr;
	for(i = *(int *)ap->vect_no; i; i--) {
		*((V_FUNC *)INTVEC+i+RL1_P) = *vec++;
	}

	set_vbr((void **)INTVEC);	/* ＳＨ２のベクトル・テーブルに必須 */

	apl = ap->apl_start;
	apl();
	
	for(;;);
}


/*---------------------------------------------------------------
				チェックサムルーチン
----------------------------------------------------------------*/
int Aply_CheckSum(void)
{
	if(SysFromAplyChecksum() == (*((unsigned short *)APLY_SUM_ADDR))){
		rsstr("-OK Aply SumCheck.\n");
		if(TcpAplyChecksum() == (*BOOT_SETSUM_ADDR)){
			rsstr("-OK IP SumCheck.\n");
			/* IPアドレス等がアプリで変更されていれば、それをE2ROMにバックアップする */
			if(compare_TcpipBackup()){
				write_toTcpipBackup();
				rsstr("Update IP address.\n");
			}
		}
		else{	/* SRAMブートローダ情報のチェック・サムが合わない場合 */
			rsstr("-NG Bootload_Info SumCheck.\n");
			rsstr("Write back IP adress to SRAM from E2ROM backup.\n");
			read_fromTcpipBackup();		/* E2ROMのバックアップしたデータをSRAMに書き戻す */
			comKfunc((char *)DMU_START_CHECK);	/* SRAMへスタートアドレスを登録後、サム値書込み */
		}
	}
	else{
		rsstr("-NG Aply SumCheck.\n");
		set_SumCheck_NG();
		if(TcpAplyChecksum() != (*BOOT_SETSUM_ADDR)){
			rsstr("-NG IP SumCheck.\n");
			set_default_ipaddr();
		}
	}
	return 0;
}


/*----------------------------------------------------------------
				アプリ部分のチェックサム計算
----------------------------------------------------------------*/
unsigned short SysFromAplyChecksum( void )
{
	unsigned long size,sizemax;
	unsigned char *sta = (unsigned char*)APLY_START_ADDR;
	unsigned short sum = (unsigned short)0;

	/* アプリ部分 */
	sizemax = (unsigned long)(APLY_SUM_ADDR - APLY_START_ADDR);
	size = (unsigned long)(*((int *)APLY_SIZE_ADDR));
	if(size > sizemax) size = sizemax;
	while( size ) {
		sum += *sta;
		sta++;
		size--;
	}
	put4hex((int)sum);

	return sum;
}


/*---------------------------------------------------------------
				サムチェックNGの設定
----------------------------------------------------------------*/
static int SumCheck_NG = 0;
void set_SumCheck_NG(void)
{
	SumCheck_NG = 1;
}

void clear_SumCheck_NG(void)
{
	SumCheck_NG = 0;
}

int check_SumCheck_NG(void)
{
	return SumCheck_NG;
}


/*----------------------------------------------------------------
		ＳＲＡＭブートローダ情報のチェックサム計算
----------------------------------------------------------------*/
unsigned short TcpAplyChecksum( void )
{
	unsigned long size;
	unsigned char *sta = (unsigned char*)ETH_IP_ADR;
	unsigned short sum = (unsigned short)0;

	size = (unsigned long)((int)BOOT_SETSUM_ADDR - ETH_IP_ADR);
	while( size ) {
		sum += *sta;
		sta++;
		size--;
	}

	sta = (unsigned char*)ETH_HOST_NAME;
	size = (unsigned long)((int)BOOT_END_ADDR - ETH_HOST_NAME);
	while( size ) {
		sum += *sta;
		sta++;
		size--;
	}
	put4hex((int)sum);

	return sum;
}


/*---------------------------------------------------------------
				デフォルトIPアドレスのセット
----------------------------------------------------------------*/
void set_default_ipaddr(void)
{
#ifdef HOUSOU
	static unsigned char	 ip[IP_ADDRESS_LEN]	= {(unsigned char)192,(unsigned char)168,(unsigned char)0,(unsigned char)41};
#else
	static unsigned char	 ip[IP_ADDRESS_LEN]	= {(unsigned char)192,(unsigned char)168,(unsigned char)0,(unsigned char)11};
#endif
	static unsigned char	svr[IP_ADDRESS_LEN]	= {(unsigned char)192,(unsigned char)168,(unsigned char)0,(unsigned char)1};
	static unsigned char	 gw[IP_ADDRESS_LEN]	= {(unsigned char)192,(unsigned char)168,(unsigned char)0,(unsigned char)254};
	static unsigned char	sbn[IP_ADDRESS_LEN]	= {(unsigned char)255,(unsigned char)255,(unsigned char)255,(unsigned char)0};
	static unsigned char	host[USER_NAME_LEN]	= "host";
	static unsigned char	user[USER_NAME_LEN]	= "user";
	static unsigned char	pass[USER_NAME_LEN]	= "pass";

	memcpy( (void *)ETH_IP_ADR,    (void *)ip,   (size_t)IP_ADDRESS_LEN );
	memcpy( (void *)ETH_SVR_ADR,   (void *)svr,  (size_t)IP_ADDRESS_LEN );
	memcpy( (void *)ETH_GW_ADR,    (void *)gw,   (size_t)IP_ADDRESS_LEN );
	memcpy( (void *)ETH_SBN_ADR,   (void *)sbn,  (size_t)IP_ADDRESS_LEN );
	memcpy( (void *)ETH_HOST_NAME, (void *)host, (size_t)USER_NAME_LEN );
	memcpy( (void *)ETH_USER_NAME, (void *)user, (size_t)USER_NAME_LEN );
	memcpy( (void *)ETH_PASSWORD,  (void *)pass, (size_t)USER_NAME_LEN );
	
	rsstr("Set Default IP Address.\n");
}


/*---------------------------------------------------------------
				UsnetへのNETWORK設定の登録
----------------------------------------------------------------*/
void set_Tcpip_toUsnet(void)
{
	union macunion mac;
	int i;
	
	e2s_romf_open(0, 4);
	e2s_romf_seek(0,30,0);
	e2s_romf_read(0, mac.s, MAC_ADDRESS_LEN);
	e2s_romf_close(0);
	for(i=0;i<6;++i) mac_addr[i] = mac.b[i];
	
	memcpy( (void *)ip_addr,		(void *)ETH_IP_ADR,		(size_t)IP_ADDRESS_LEN );
	memcpy( (void *)svr_addr,		(void *)ETH_SVR_ADR,	(size_t)IP_ADDRESS_LEN );
	memcpy( (void *)gw_addr,		(void *)ETH_GW_ADR,		(size_t)IP_ADDRESS_LEN );
	memcpy( (void *)sbn_msk,		(void *)ETH_SBN_ADR,	(size_t)IP_ADDRESS_LEN );
	memcpy( (void *)hostname,		(void *)ETH_HOST_NAME,	(size_t)USERID_LEN );
	memcpy( (void *)Usnet_UserID,	(void *)ETH_USER_NAME,	(size_t)USERID_LEN );
	memcpy( (void *)Usnet_PassWd,	(void *)ETH_PASSWORD,	(size_t)PASSWD_LEN );
}


/*----------------------------------------------------------------
		NETWORK設定のバックアップ処理
----------------------------------------------------------------*/
#define TCP_SIZE1		((int)BOOT_APLY_ADDR - ETH_IP_ADR)//16
#define TCP_SIZE2		((int)BOOT_PACKAGE_NAME - ETH_HOST_NAME)//48
#define TCP_SIZE		((int)TCP_SIZE1 + TCP_SIZE2)//64
int compare_TcpipBackup(void)
{
	union netunion tcpip;

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,36,0);
	e2s_romf_read(0, tcpip.s, TCP_SIZE);
	e2s_romf_close(0);
	
	if (memcmp((void *)tcpip.b,	(void *)ETH_IP_ADR,	(size_t)TCP_SIZE1)) return -1;
	if (memcmp((void *)&tcpip.b[TCP_SIZE1],	(void *)ETH_HOST_NAME,	(size_t)TCP_SIZE2)) return -1;
	return 0;
}

void read_fromTcpipBackup(void)
{
	union netunion tcpip;

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,36,0);
	e2s_romf_read(0, tcpip.s, TCP_SIZE);
	e2s_romf_close(0);
	
	memcpy( (void *)ETH_IP_ADR,	(void *)tcpip.b,	(size_t)TCP_SIZE1 );
	memcpy( (void *)ETH_HOST_NAME,	(void *)&tcpip.b[TCP_SIZE1],	(size_t)TCP_SIZE2 );
}

void write_toTcpipBackup(void)
{
	union netunion tcpip;

	memcpy( (void *)tcpip.b,	(void *)ETH_IP_ADR,	(size_t)TCP_SIZE1 );
	memcpy( (void *)&tcpip.b[TCP_SIZE1],	(void *)ETH_HOST_NAME,	(size_t)TCP_SIZE2 );

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,36,0);
	e2s_romf_write(0, tcpip.s, TCP_SIZE);
	e2s_romf_close(0);
}


/*---------------------------------------------------------------
				バージョン文字列処理関数
----------------------------------------------------------------*/
static void LdaSetVersion( void )
{
	static	char	verstr[17];
	int		*iptr;
	char	*idptr;

	iptr = (int *)vector_strc.fun[0];
	iptr = (int *)(*iptr);
	idptr = (char *)(iptr);
	getversionstr(verstr, idptr);
	strcpy( BOOT_VERSION_ADDR, verstr );
}


void
getversionstr(char *verstr, char *idptr){
	static	char	tmp1[80],tmp2[20],tmp3[20];
	char	c;
	int		i,j,k;

	verstr[0] = (char)0;
	for(i=0;i<256;i++) if(idptr[i]==' ') break;	/* skip Header */
	if(i>=256) return;
	for(++i,j=0;j<80;++i,++j){
		c = idptr[i];
		tmp1[j] = c;						/* Load Filename */
		if(c==' '){
			tmp1[j] = (char)0;
			break;
		}
	}
	if(j>=80) return;
	for(;j>=0;j--){
		if(tmp1[j]=='.'){
			tmp1[j] = (char)0;
			continue;
		}
		if(tmp1[j]=='/') break;	/* search .../rcs/L****.c */
	}
	if(j<0) return;
	++j;
	if(strlen(tmp1+j)>=20) return;
	strcpy(tmp2,tmp1+j);					/* Load Filename */
	strcpy(tmp1,tmp2);
	for(++i,j=0;j<20;++i,++j){
		c = idptr[i];
		tmp3[j] = c;						/* Load Version */
		if(c==' '){
			tmp3[j] = (char)0;
			break;
		}
	}
	if(j>=20) return;
	for(i=0,k=0;i<20;++i){
		c = tmp3[i];
		if(c=='.') break;
		if(c==(char)0) break;
		k *= 10;
		k += c & 0x0f;
	}

	tmp2[0] = (char)(0x40 + k - 1);
	tmp2[1] = (char)0;
	if((k>=2)&&(k<=27)) strcat(tmp1,tmp2);
	tmp2[0] = (char)'_';
	if(k>=28) strcat(tmp1,tmp2);
	if(k<0) strcat(tmp1,tmp2);

	strcat(tmp1," ");
	strcat(tmp1,tmp3);
	for(i=0;i<16;++i){
		c = tmp1[i];
		verstr[i] = c;
		if(c==(char)0) break;
	}
	verstr[i] = (char)0;
}


/*****************************************************************************
* 初期設定
* キャッシュのライトスルー／ライトバックを設定します。
* キャッシュが有効の場合のみ有効です。また、キャッシュの設定とは別に
* SDRAMの設定には、ライトスルーでは、SingleWrite
* ライトバックでは、Burst Writeにします.
******************************************************************************/

void INIT(void)
{

	disable();
	
	INITSCT();     /* セクションの初期化ルーチンの呼び出し */

	int_mask(0x0f);
	pint_mask(0x0f);

	CmdFd = -1;		/* TCP/IPがオープンされている時のみ有効なデータが入る */

	LdaSetVersion();
	
	if(memcmp((const void *)APLY_START_STATUS,(const void *)"AplStat",(size_t)7)==0){
		start_aply(); /* マルチタスク開始前にアプリにジャンプするようにする */
	}
	
	main();         /* メインルーチンの呼び出し */
	
	rsstr232("Main Program Terminated !!!\n");
	for (;;) ;      /* 万一、main 関数終了したら、無限ループ */
}


/*****************************************************************************
* セクションの初期化
*
******************************************************************************/

void INITSCT(void)
{
	int *p, *q;

	/* 未初期化データ領域をゼロで初期化 */
	for (p = _B_BGN; p < _B_END; p++)
		*p = 0;

	/* 初期化データを ROM 上から RAM 上へコピー */
	for (p = _D_BGN, q = _D_ROM; p < _D_END; p++, q++)
		*p = *q;
}

