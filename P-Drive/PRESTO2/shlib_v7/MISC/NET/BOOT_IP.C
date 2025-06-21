/*======================================================================
 * Filename : boot_ip.c
 * Original : p:/dacs_i/mcu3/main/rcs/boot_ip.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		SRAM上のブートローダ情報のネットワーク設定データへのアクセスモジュール
 *		(IPアドレス，サーバアドレス，ゲートウェイ，サブネットマスク)
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/net/RCS/boot_ip.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: boot_ip.c $
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

#define	_BOOT_IP_C_

#include <string.h>
#include "boot_ip.h"

void TcpAplyChecksum( void )
{
	unsigned long size;
	unsigned char *sta;
	unsigned short sum = (unsigned short)0;

	sta = (unsigned char*)ETH_IP_ADR;
	size = (unsigned long)((int)BOOT_SETSUM_ADDR - ETH_IP_ADR);
	while ( size ) {
		sum += *sta;
		sta++;
		size--;
	}
	sta = (unsigned char*)ETH_HOST_NAME;
	size = (unsigned long)((int)BOOT_END_ADDR - ETH_HOST_NAME);
	while ( size ) {
		sum += *sta;
		sta++;
		size--;
	}
	*BOOT_SETSUM_ADDR = sum;
	return;
}

/*	IPアドレス設定へのアクセス	*/
void read_boot_loader_ip_address( unsigned char *ip )
{
	memcpy( ip, (unsigned char*)ETH_IP_ADR, IP_ADDRESS_LEN );
	return;
}
void write_boot_loader_ip_address( unsigned char *ip )
{
	memcpy( (unsigned char*)ETH_IP_ADR, ip, IP_ADDRESS_LEN );
	TcpAplyChecksum();
	return;
}
/*	サブネットマスク設定へのアクセス	*/
void read_boot_loader_subnet_mask( unsigned char *sbn )
{
	memcpy( sbn, (unsigned char*)ETH_SBN_ADR, IP_ADDRESS_LEN );
	return;
}
void write_boot_loader_subnet_mask( unsigned char *sbn )
{
	memcpy( (unsigned char*)ETH_SBN_ADR, sbn, IP_ADDRESS_LEN );
	TcpAplyChecksum();
	return;
}
/*	ゲートウェイIPアドレス設定へのアクセス	*/
void read_boot_loader_gateway_ip( unsigned char *gw )
{
	memcpy( gw, (unsigned char*)ETH_GW_ADR, IP_ADDRESS_LEN );
	return;
}
void write_boot_loader_gateway_ip( unsigned char *gw )
{
	memcpy( (unsigned char*)ETH_GW_ADR, gw, IP_ADDRESS_LEN );
	TcpAplyChecksum();
	return;
}
/*	ホスト名設定へのアクセス	*/
void read_boot_loader_host_name( unsigned char *host )
{
	memcpy( host, (unsigned char*)ETH_HOST_NAME, USER_NAME_LEN );
	return;
}
void write_boot_loader_host_name( unsigned char *host )
{
	memcpy( (unsigned char*)ETH_HOST_NAME, host, USER_NAME_LEN );
	TcpAplyChecksum();
	return;
}
/*	ホストIPアドレス設定へのアクセス	*/
void read_boot_loader_host_ip_address( unsigned char *svr )
{
	memcpy( svr, (unsigned char*)ETH_SVR_ADR, IP_ADDRESS_LEN );
	return;
}
void write_boot_loader_host_ip_address( unsigned char *svr )
{
	memcpy( (unsigned char*)ETH_SVR_ADR, svr, IP_ADDRESS_LEN );
	TcpAplyChecksum();
	return;
}
/*	ユーザ名設定へのアクセス	*/
void read_boot_loader_user_name( unsigned char *user )
{
	memcpy( user, (unsigned char*)ETH_USER_NAME, USER_NAME_LEN );
	return;
}
void write_boot_loader_user_name( unsigned char *user )
{
	memcpy( (unsigned char*)ETH_USER_NAME, user, USER_NAME_LEN );
	TcpAplyChecksum();
	return;
}
/*	パスワード設定へのアクセス	*/
void read_boot_loader_passwd( unsigned char *pass )
{
	memcpy( pass, (unsigned char*)ETH_PASSWORD, USER_NAME_LEN );
	return;
}
void write_boot_loader_passwd( unsigned char *pass )
{
	memcpy( (unsigned char*)ETH_PASSWORD, pass, USER_NAME_LEN );
	TcpAplyChecksum();
	return;
}

/*	ＭＡＣアドレスへのアクセス	*/
void read_boot_loader_macaddr( unsigned char *macaddr )
{
	union macunion{
		unsigned short s[10];
		unsigned char  b[20];
	} mac;
	int		i;

	e2s_romf_open(0, 4);
	e2s_romf_seek(0,30,0);
	e2s_romf_read(0, mac.s, MAC_ADDRESS_LEN);
	e2s_romf_close(0);

	for(i=0;i<MAC_ADDRESS_LEN;++i) macaddr[i] = mac.b[i];

	return;
}

