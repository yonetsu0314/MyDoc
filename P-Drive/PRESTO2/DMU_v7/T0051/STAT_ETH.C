/*======================================================================
 * File name    : stat_eth.c
 * Original		: p:/dacs_i/mcu3/main/rcs/stat_eth.c 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		初期設定ルーチン（FOR SH7615）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/stat_eth.c 1.9 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: stat_eth.c $
 * リビジョン 1.9  1970/01/01  00:00:00  sakaguti
 * BTWRITERを削除
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/02/19 18:30
 * デバッグ情報入出力をcon1からcon0を通して行うように変更し、
 * プログラムを整理したことに対応
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  sakaguti
 * drive\usnet.hをｲﾝｸﾙｰﾄﾞ
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2003/12/12 19:00
 * WCUとのシリアル通信用変数を追加する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/21 12:00
 * MSEC を定義するためsh7615\bootpara.h をインクルードする
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/17 15:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
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
#include <sh7615\bootpara.h>
#include "rxdef.h"
#include <drive\eth_def.h>
#include <socket.h>
#include <drive\usnet.h>

#define STORAGE
#include "stat_eth.h"
#define	_TCP_SEND_8_
#include "tsk_lan.h"
#undef STORAGE

#include "utype.h"
#include "rsinout.h"


int
ttttt(){
	cre_tsk(SOCKET_TSK_ID,  &ctsk_sock);
	return sta_tsk(SOCKET_TSK_ID, 0);
}


#define MSGBUFSIZ 10240
/*==================================================================*/
/* ServerWork Task													*/
/* ホストからコマンドポート（制御コネクション）を通じてコマンドを	*/
/* 受信するタスク													*/
/* コマンド用ポートは元々のあったように常時受信を監視しており、		*/
/* ここで CTRL+x を受信すると内側のループを抜ける。					*/
/* コマンド用ポートの接続が切れるときはデータ用ポートも一緒に		*/
/* 切断される。														*/
/*------------------------------------------------------------------*/
LOCAL void tcpServerLoop(int sFd)
{
	int		i,j,nRead;
	static char	requestMsg[MSGBUFSIZ + 2];

	/* コマンドコネクションのソケット番号を格納 */
	CmdFd = sFd;		/* TCP/IPがオープンされている時のみ有効なデータが入る */
	rsstr("\nBootLoader from TCP/IP on DMU/SH2");
	rsstr("\n            by Ishida Co.,Ltd. 2003\n");
	rsstr("DMUAPL>");
	while ( (nRead = UsnetSocket_readsocket(sFd, requestMsg, MSGBUFSIZ) ) > 0) {
		if((int_buf0_count-int_buf0_cntout)<(INT_BUF_size-nRead)){
			if(int_buf0_in <= (int_buf0+INT_BUF_size-nRead)){
				memcpy((void *)int_buf0_in,(void *)requestMsg,(size_t)nRead);
				int_buf0_in += nRead;
				int_buf0_count += nRead;
				if(int_buf0_in >= int_buf0+INT_BUF_size) int_buf0_in = int_buf0;
				continue;
			}
			else{
				i = INT_BUF_size - (int_buf0_in - int_buf0);
				j = nRead - i;
				memcpy((void *)int_buf0_in,(void *)requestMsg,(size_t)i);
				memcpy((void *)int_buf0,(void *)(requestMsg+i),(size_t)j);
				int_buf0_in = int_buf0 + j;
				int_buf0_count += nRead;
				continue;
			}
		}
		for(i=0;i<nRead;++i){
lll:
			if((int_buf0_count-int_buf0_cntout)<INT_BUF_size){
				*int_buf0_in++ = requestMsg[i];
				int_buf0_count++;
				if(int_buf0_in >= int_buf0+INT_BUF_size) int_buf0_in = int_buf0;
			}
			else{
				int_buf0_ovfl = (unsigned char)1;
				for(;;){
					if(chkbuflen()) break;
					dly_tsk((RELTIM)100/MSEC);
				}
				int_buf0_ovfl = (unsigned char)0;
				goto lll;
			}
		}
	}
	CmdFd = -1;		/* TCP/IPがオープンされている時のみ有効なデータが入る */
	return;
}

/*==================================================================*/
/* 待機ソケットの生成												*/
/*------------------------------------------------------------------*/
LOCAL int tcpCreateSocket(int nPort)
{
	struct sockaddr_in	serverAddr;		/* server's socket address			*/
	int					sockAddrSize;	/* size of socket address structure	*/
	int					sFd;			/* socket file descriptor			*/

	/* set up the local address */

	sockAddrSize = sizeof(struct sockaddr_in);

	memset((void *)&serverAddr, 0, (size_t)sockAddrSize);
	serverAddr.sin_family = (short)AF_INET;
	serverAddr.sin_port = (unsigned short)htons((unsigned short)nPort);
	serverAddr.sin_addr.s_addr = (unsigned long)htonl((unsigned long)INADDR_ANY);

	/* create client's socket */

	if ((sFd = UsnetSocket_socket(AF_INET, SOCK_STREAM, 0)) == ERROR) {
		return (ERROR);
	}

	/* bind socket to local address */

	if (UsnetSocket_bind(sFd, (struct sockaddr_in *)&serverAddr, sockAddrSize) == ERROR) {
		UsnetSocket_closesocket(sFd);
		return (ERROR);
	}

	/* create queue for client connection requests */

	if (UsnetSocket_listen(sFd, SERVER_MAX_CONNECTIONS) == ERROR) {
		UsnetSocket_closesocket(sFd);
		return (ERROR);
	}

	return sFd;
}

/*==================================================================*/
/* DATA Send Server													*/
/* ホストからのデータポートのコネクションを監視するタスク			*/
/* このタスクは消滅することなくacceptを発行し、いったん接続されれば */
/* 切断されるまでテキストを読み捨てる。								*/
/* 切断されれば、readが終了するのでもう一度acceptに戻る。			*/
/*------------------------------------------------------------------*/
TASK tcpStreamSendServerTask()
{
//	tcpSendServerTask(DataFd0);
}

LOCAL int tcpSendServerTask(int sSvrFdDat)
{
	struct sockaddr_in	clientAddr;		/* client's socket address			*/
	int					sockAddrSize;	/* size of socket address structure	*/
	int					nRead;
	char				requestMsg[3];

	while(1) {
		sockAddrSize = sizeof(struct sockaddr_in);
		if ((DataFd = UsnetSocket_accept(sSvrFdDat, (struct sockaddr_in *)&clientAddr, &sockAddrSize)) == ERROR) {
			/* acceptが異常終了するときは待機ソケットが破棄されたとき等であろう */
			break;
		}
		while ( (nRead = UsnetSocket_readsocket(DataFd, requestMsg, 1) ) > 0);
		UsnetSocket_closesocket(DataFd);
		DataFd = -1;
	}
	return (ERROR);
}

/*==================================================================*/
/* Command Server													*/
/* メインのワークタスク												*/
/* まず、待機用のソケットは最初に二つ生成しておき、生成し終わったら	*/
/* データポート（SendServer）のacceptを実行するタスクを起こす。		*/
/* その後、コマンドポートのacceptを行う無限ループに入る。			*/
/*------------------------------------------------------------------*/
LOCAL int tcpServerTask(int nCmdPort, int nDatPort)
{
	struct sockaddr_in	clientAddr1;	/* client's socket address1 */
	int					sockAddrSize1;	/* size of socket address1 structure */
	int					sSvrFdCmd;		/* socket file descriptor for COMMAND Server */

	nDatPort = nDatPort;
	/* コマンド用ソケットを生成し指定ポートで回線を開く */
	if ((sSvrFdCmd = tcpCreateSocket(nCmdPort)) == ERROR) {
		return (ERROR);
	}

	while(1) {
		sockAddrSize1 = sizeof(struct sockaddr_in);
		if ((sCltFdCmd = UsnetSocket_accept(sSvrFdCmd, (struct sockaddr_in *)&clientAddr1, &sockAddrSize1)) == ERROR) {
			/* acceptが異常終了するときは待機ソケットが破棄されたとき等であろう */
			continue;
		}
		tcpServerLoop(sCltFdCmd);
		UsnetSocket_closesocket(sCltFdCmd);
	}
	return (ERROR);
}


/*==================================================================*/
/* Write To Cliente													*/
/*------------------------------------------------------------------*/
LOCAL int tcpServerWrite(int sFd, char *str)
{
	if(UsnetSocket_writesocket(sFd, str, (int)strlen(str)) == ERROR) {
		return ERROR;
	}
	return OK;
}

/*==================================================================*/
/* TCP Socket Create Task											*/
/* 引数		：使用ポート番号										*/
/* 戻り値	：Task Status											*/
/* 注意		：システム起動時に呼び出す								*/
/*------------------------------------------------------------------*/
LOCAL int SocketServerCreate(void)
{
	return tcpServerTask(BOOT_PORT, NULL_PORT);
}

int SocketServer(void)
{
	int st;
	st = SocketServerCreate();
	return st;
}

void UsnetFTPserver( void );
/*==================================================================*/
/* FTP Sever Task													*/
/* 引数		：														*/
/* 戻り値	：														*/
/* 注意		：														*/
/*------------------------------------------------------------------*/

void FTPserver(void){/*020806*/
	cre_tsk(FTP_TSK_ID,  &ctsk_ftp);
	sta_tsk(FTP_TSK_ID, 0);
}

TASK RunFTP(){/*020806*/
	UsnetFTPserver();	/*FTPサーバ 無限ループ*/
}
