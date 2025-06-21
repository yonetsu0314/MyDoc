/*======================================================================*/
/* File name	: ICONVERT.C											*/
/* Original		: p:/vxworks/rcu/lib/line/rcs/lnbootln.c 1.1			*/
/*----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/net/RCS/iconvert.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: iconvert.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * TCP/IPのVxWorksI/F　SRAM設定等
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1999 ISHIDA CO., LTD.								*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*/
/*		Phone +81 775 53-4141											*/
/*----------------------------------------------------------------------*/
#include	<string.h>
#include	"usnets\net.h"
#include	"boot_ip.h"

char *UsnetGetLocalIpAddress( void );
char *UsnetGetSubnetMask( void );
char *UsnetGetLocalHostname( void );
char *UsnetGetGwAddress( void );
char *UsnetGetUserID( void );
void UsnetSetLocalHostname( char *buf );
void UsnetSetLocalIpAddress( char *buf );

/*----------------------------------------------------------------------*/

void ln_bootline_recall(void)
{
	return;		/* ダミー関数 */
}

void ln_bootline_save(void)
{
	return;		/* ダミー関数 */
}

void ln_bootline_setDefault(void)
{
	return;		/* ダミー関数 */
}

char **ln_system_information(void)
{
	static char sys_info[3][56] = {{"ishida"}, {"dummy"}, {"library"}};
	static char *sys_mess[3] = {sys_info[0], sys_info[1], sys_info[2]};

	return sys_mess;
}

/*----------------------------------------------------------------------*/

void ln_get_inetAddr(char *str)
{
//	strncpy(str, UsnetGetLocalIpAddress(), Iid_SZ);
//	*(str++) = '\0';
	read_boot_loader_ip_address((unsigned char *)str);
}

void ln_set_inetAddr(char *str)
{
//	UsnetSetLocalIpAddress(str);
	write_boot_loader_ip_address((unsigned char *)str);
}

/*----------------------------------------------------------------------*/

void ln_get_subnetAddr(char *str)
{
//	strncpy(str, UsnetGetSubnetMask(), Iid_SZ);
//	*(str++) = '\0';
	read_boot_loader_subnet_mask((unsigned char *)str);
}

void ln_set_subnetAddr(char *str)
{
//	UsnetSetSubnetMask(str);
	write_boot_loader_subnet_mask((unsigned char *)str);
}

/*----------------------------------------------------------------------*/

void ln_get_bootDev(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

void ln_set_bootDev(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/

void ln_get_hostName(char *str)
{
	strcpy(str, UsnetGetLocalHostname());
}

void ln_set_hostName(char *str)
{
	UsnetSetLocalHostname(str);
}

/*----------------------------------------------------------------------*/

void ln_get_targetName(char *str)
{
	strcpy(str, UsnetGetLocalHostname());	/* LOCAL HOST と同じにする */
}

void ln_set_targetName(char *str)
{
	UsnetSetLocalHostname(str);				/* LOCAL HOST と同じにする */
}

/*----------------------------------------------------------------------*/

void ln_get_backplaneAddr(char *str)
{
	str = str;
	return;
}

void ln_set_backplaneAddr(char *str)
{
	str = str;
	return;
}

/*----------------------------------------------------------------------*/

void ln_get_hostAddr(char *str)
{
	strncpy(str, UsnetGetLocalIpAddress(), Iid_SZ);
	*(str++) = '\0';
}

void ln_set_hostAddr(char *str)
{
	UsnetSetLocalIpAddress(str);
}

/*----------------------------------------------------------------------*/

void ln_get_gatewayAddr(char *str)
{
//	strncpy(str, UsnetGetGwAddress(), Iid_SZ);
//	*(str++) = '\0';
	read_boot_loader_gateway_ip((unsigned char *)str);
}

void ln_set_gatewayAddr(char *str)
{
//	UsnetSetGwAddress(str);
	write_boot_loader_gateway_ip((unsigned char *)str);
}

/*----------------------------------------------------------------------*/

void ln_get_bootFile(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

void ln_set_bootFile(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/

void ln_get_startupScript(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

void ln_set_startupScript(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/

void ln_get_userName(char *str)
{
	strcpy(str, UsnetGetLocalHostname());	/* LOCAL HOST と同じにする */
}

void ln_set_userName(char *str)
{
	UsnetSetLocalHostname(str);				/* LOCAL HOST と同じにする */
}

/*----------------------------------------------------------------------*/

void ln_get_ftpPasswd(char *str)
{
//	strcpy(str, UsnetGetUserID());
	read_boot_loader_passwd((unsigned char *)str);
}

void ln_set_ftpPasswd(char *str)
{
//	UsnetSetPassWord(str);
	write_boot_loader_passwd((unsigned char *)str);
}

/*----------------------------------------------------------------------*/

void ln_get_otherOptions(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

void ln_set_otherOptions(char *str)
{
	str = str;
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/

int ln_get_procNum(void)
{
	return(0);		/* ダミー関数 */
}

void ln_set_procNum(int num)
{
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/

int ln_get_configFlags(void)
{
	return(0);		/* ダミー関数 */
}

void ln_set_configFlags(int flg)
{
	return;		/* ダミー関数 */
}

/*----------------------------------------------------------------------*/
