/*======================================================================
 * Filename : boot_ip.h
 * Original : p:/dacs_i/mcu3/main/rcs/boot_ip.h 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *		SRAM上のブートローダ情報のネットワーク設定データへのアクセスモジュール
 *		(IPアドレス，サーバアドレス，ゲートウェイ，サブネットマスク)
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/net/RCS/boot_ip.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: boot_ip.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * MACｱﾄﾞﾚｽ関数を追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * TCP/IPのVxWorksI/F　SRAM設定等
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 2003, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CITY
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */

#ifndef	_BOOT_IP_H_
#define	_BOOT_IP_H_

/*
//	TCP/IP設定
*/
#define	IP_ADDRESS_LEN	4
#define	MAC_ADDRESS_LEN	6
#define	USER_NAME_LEN 16

/* ＳＲＡＭ上のブートローダ情報のアドレス */
#define BOOT_SETSUM_ADDR	((unsigned short *)0x0200003e)	/* 200020-20003d */

#define ETH_IP_ADR		(0x02000020)	/*	ＩＰアドレス	*/
#define ETH_SVR_ADR 	(0x02000024)	/*	ＳＶＲアドレス	*/
#define ETH_GW_ADR		(0x02000028)	/*	ＧＷアドレス	*/
#define ETH_SBN_ADR 	(0x0200002C)	/*	ＳＢＮアドレス	*/

#define	ETH_HOST_NAME	(0x02000040)	/*	ＨＯＳＴ名		*/
#define	ETH_USER_NAME	(0x02000050)	/*	ユーザ名		*/
#define	ETH_PASSWORD	(0x02000060)	/*	パスワード		*/

#define BOOT_END_ADDR		((char*)0x02000100)

void read_boot_loader_ip_address( unsigned char * );
void read_boot_loader_subnet_mask( unsigned char * );
void read_boot_loader_gateway_ip( unsigned char * );
void read_boot_loader_host_name( unsigned char * );
void read_boot_loader_host_ip_address( unsigned char * );
void read_boot_loader_user_name( unsigned char * );
void read_boot_loader_passwd( unsigned char * );
void read_boot_loader_macaddr( unsigned char *macaddr );

void write_boot_loader_ip_address( unsigned char * );
void write_boot_loader_subnet_mask( unsigned char * );
void write_boot_loader_gateway_ip( unsigned char * );
void write_boot_loader_host_name( unsigned char * );
void write_boot_loader_host_ip_address( unsigned char * );
void write_boot_loader_user_name( unsigned char * );
void write_boot_loader_passwd( unsigned char * );

int	e2s_romf_open(int no, int mode);
int	e2s_romf_close(int no);
int	e2s_romf_write(int no, unsigned short *buf, int len);
int	e2s_romf_read(int no, unsigned short *buf, int len);
int	e2s_romf_seek(int no, int offs, int org);

#endif
