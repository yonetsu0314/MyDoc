/*---------------------------------------------------------------------
 * Original	: p:/dacs_i/shdsplib/drive/sh7615/ether/rcs/lan_init.c 1.5
 * $Header: p:/presto2/shlib_v7/drive/sh7615/ether/rcs/lan_init.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 * $Log: lan_init.c $
 * ƒŠƒrƒWƒ‡ƒ“ 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/17 17:00
 * InitLan() ‚Ì’Ç‰Á
 * 
 * ƒŠƒrƒWƒ‡ƒ“ 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

#include <kernel.h>

#include "drive\eth_def.h"
#include <socket.h>

#define ETH_IP_ADR  ((unsigned char *)0x02000020)	/* ‚h‚oƒAƒhƒŒƒX */
#define ETH_SVR_ADR ((unsigned char *)0x02000024)	/* ‚r‚u‚qƒAƒhƒŒƒX */
#define ETH_GW_ADR  ((unsigned char *)0x02000028)	/* ‚f‚vƒAƒhƒŒƒX */
#define ETH_SBN_ADR ((unsigned char *)0x0200002C)	/* ‚r‚a‚mƒAƒhƒŒƒX */

unsigned char	ip_addr[8]	= {192,168,10,2};
unsigned char	svr_addr[8]	= {192,168,10,1};
unsigned char	gw_addr[8]	= {192,168,10,254};
unsigned char	sbn_msk[8]	= {255,255,0,0};
unsigned char	hostname[16]	= "host";
unsigned char	mac_addr[8]	= {0xff,0xff,0xff,0xff,0xff,0xff};

int InitLanSystem( void )
{
	int		ret = 0;

//	if((ip_addr[0]==192)&&(ip_addr[1]==168)&&(ip_addr[2]==10)&&(ip_addr[3]==2)){
//		UsnetSetLocalIpAddress(ETH_IP_ADR);
//		UsnetSetRemoteIpAddress(ETH_SVR_ADR);
//		UsnetSetRemoteHostname(hostname);
//		UsnetSetMacAddress(mac_addr);
//		UsnetSetGwAddress(ETH_GW_ADR);
//		UsnetSetSubnetMask(ETH_SBN_ADR);
//	}
//	else{
		UsnetSetLocalIpAddress(ip_addr);
		UsnetSetRemoteIpAddress(svr_addr);
		UsnetSetRemoteHostname(hostname);
		UsnetSetMacAddress(mac_addr);
		UsnetSetGwAddress(gw_addr);
		UsnetSetSubnetMask(sbn_msk);
//	}

	ret = UsnetTerm();
	ret = UsnetInit();

	return ret;
}

int InitLan( void )
{
	int		ret = 0;

	ret = UsnetTerm();
	ret = UsnetInit();

	return ret;
}
