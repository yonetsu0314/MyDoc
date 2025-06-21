/*======================================================================
 * File name    : stat_eth.h
 * Original		: p:/dacs_i/mcu3/main/rcs/stat_eth.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/dmu_v7/bootload/rcs/stat_eth.h 1.4 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: stat_eth.h $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/17 20:30
 * set_vector() は不要なので消去する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ブートローダ
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/
#ifndef	_STAT_ETH_H_
#define	_STAT_ETH_H_

TASK tcpStreamSendServerTask(void);
TASK RunFTP(void);/*020806*/

#ifdef	STORAGE
const T_CTSK ctsk_sock  
	= { TA_HLNG, NULL, tcpStreamSendServerTask,  3+0*SOCKET_TSK_PRI, 1024, NULL, (B *)"socktask" };
const T_CTSK ctsk_ftp  = { TA_HLNG, NULL, RunFTP,  FTP_TSK_PRI, 1024, NULL, (B *)"ftptask" };/*020806*/
#else
extern const T_CTSK ctsk_sock;
extern const T_CTSK ctsk_ftp;
#endif

int SocketServer(void);
void FTPserver(void);
int		ttttt(void);

#endif
