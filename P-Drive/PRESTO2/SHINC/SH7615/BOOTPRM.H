/*======================================================================
 * File name	: bootprm.h
 * Original 	: p:/presto/shinc/sh7615/rcs/bootpara.h 1.16
 *----------------------------------------------------------------------
 * Function 	:
 *		電断対応ファイルシステムsaver60用
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7615/RCS/bootprm.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootprm.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#ifdef MSEC
#undef	MSEC
#define MSEC        1             /* 1 msec */
#else
#define MSEC        1             /* 1 msec */
#endif

#define RL1_P		(64-1)

#define ESC 	(0x1b)
#define EOF 	(-1)

#define FORCE_DOWNLOAD_OFF (PB.DR.BIT.B1 != 0)
#define FORCE_DOWNLOAD_ON (PB.DR.BIT.B1 == 0)
#define INT_MASK_REG0	((volatile char*)0x24900000)
#define INT_MASK_REG1	((volatile char*)0x24980000)
#define INT_ACK_REG0	((volatile char*)0x24700000)
#define INT_ACK_REG1	((volatile char*)0x24780000)
#define RSTCSRR			((volatile unsigned char*)0xfffffe83)
#define DIPSW1_ADDR		((volatile unsigned char*)0x24680000)
#define DIPSW2_ADDR		((volatile unsigned char*)0x24600000)

#define ARCINT1 (0x08)
#define ARCINT0 (0x04)
#define FIFOINT (0x02)

#define PINT3 (0x08)
#define PINT2 (0x04)
#define PINT1 (0x02)
#define PINT0 (0x01)

typedef void (*APLY)(void);

/*
//	TCP/IP設定
*/
#define	IP_ADDRESS_LEN	4
#define	MAC_ADDRESS_LEN	6
#define	USER_NAME_LEN 16

/* ＳＲＡＭ上のブートローダ情報 */
#define APLY_START_STATUS	((volatile char *)0x02000000)	/* アプリスタートステータス文字列 */
#define BOOT_WRITE_STATUS	((volatile char *)0x02000008)	/* ブートローダー書き換えステータス文字列 */
#define BOOT_VERSION_ADDR	((char*)0x02000010)
#define BOOT_APLY_ADDR		((char*)0x02000030)	/* アプリ開始アドレス */
#define BOOT_SETSUM_ADDR	((unsigned short *)0x0200003e)	/* 02000020-020000ff */

#define ETH_IP_ADR	(0x02000020)				/* ＩＰアドレス */
#define ETH_SVR_ADR (0x02000024)				/* ＳＶＲアドレス */
#define ETH_GW_ADR	(0x02000028)				/* ＧＷアドレス */
#define ETH_SBN_ADR (0x0200002C)				/* ＳＢＮアドレス */

#define	ETH_HOST_NAME		(0x02000040)	/*	ＨＯＳＴ名	*/
#define	ETH_USER_NAME		(0x02000050)	/*	ユーザ名	*/
#define	ETH_PASSWORD		(0x02000060)	/*	パスワード	*/
#define	BOOT_PACKAGE_NAME	(0x02000070)	/*	Package INF file name	*/
#define	NONE8			(0x02000080)	/*		*/
#define	NONE9			(0x02000090)	/*		*/
#define	NONEA			(0x020000A0)	/*		*/
#define	NONEB			(0x020000B0)	/*		*/
#define	NONEC			(0x020000C0)	/*		*/
#define	NONED			(0x020000D0)	/*		*/
#define	NONEE			(0x020000E0)	/*		*/
#define	NONEF			(0x020000F0)	/*		*/

#define BOOT_END_ADDR		((char*)0x02000100)

/* フラッシュ用アプリ・チェックサム生成用変数 */
#define BOOT_START_ADDR 	(0x20000000)
#define APLY_START_ADDR 	(0x20200000)
#define APLY_SUM_ADDR		(0x203FC000)	/* unsigned short */
#define APLY_SIZE_ADDR	(0x203FC004)	/* unsigned int */
#define APLY_BOOT_ADDR	(0x203FC010)	/* */
#define APLY_END_ADDR		(0x20400000)
#define APLY_SET_SIZE		224			/* APLY_BOOT_ADDRからAPLY_SET_SIZE分は */
							/* ETH_IP_ADRからBOOT_END_ADDRと同じ */

#define FLCPSZ (APLY_BOOT_ADDR-APLY_SUM_ADDR+APLY_SET_SIZE)

#define PROGID_TBLNUM		128

/* スタートアドレスチェック定義 */
#define FPGA_START_CHECK	(0x00000000)
#define WCU_START_CHECK		(0x01010000)
#define DMU_START_CHECK		(0x200000)

#define FLASH_WRITE_BLOCK (0x2000)		/* for DMU */


void	set_mode(unsigned short cmd);

extern unsigned char	Usnet_UserID[13];	/*	"test"	*/
extern unsigned char	Usnet_PassWd[13];	/*	"usnet"	*/

extern	unsigned char	ip_addr[];
extern	unsigned char	svr_addr[];
extern	unsigned char	gw_addr[];
extern	unsigned char	sbn_msk[];
extern	unsigned char	hostname[];
extern	unsigned char	mac_addr[];

int	e2s_romf_open(int no, int mode);
int	e2s_romf_close(int no);
int	e2s_romf_write(int no, unsigned short *buf, int len);
int	e2s_romf_read(int no, unsigned short *buf, int len);
int	e2s_romf_seek(int no, int offs, int org);
int	e2s_sys_init(unsigned short);

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

void UsnetSetLocalIpAddress( char *buf );
void UsnetSetSubnetMask( char *buf );
void UsnetSetGwAddress( char *buf );
void UsnetSetRemoteIpAddress( char *buf );
void UsnetSetUserID( char *buf );
void UsnetSetPassWord( char *buf );
void UsnetSetMacAddress( char *buf );

void int_mask(int mask);
void pint_mask(int mask);

