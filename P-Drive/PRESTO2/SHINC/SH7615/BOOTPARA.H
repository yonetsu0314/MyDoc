/*======================================================================
 * File name	: bootpara.h
 * Original 	: p:/dacs_i/rcu/main/rcs/cmfhdl.h 1.1 CON0,1に対応
 *----------------------------------------------------------------------
 * Function 	:
 *		ファイルシステムFSYS201用
 *		変数／定数定義とプロトタイプ宣言
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7615/RCS/bootpara.h 1.17 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootpara.h $
 * リビジョン 1.17  1970/01/01  00:00:00  kagatume
 * 2005/01/14 19:00
 * コメント”ファイルシステムFSYS201用”をプロトタイプ宣言InitSramDisk、
 * InitAtaDisk、InitFileSystemに追加する。
 * （本ファイルbootpara.hはファイルシステムFSYS201用とする。
 *   電断対応ファイルシステムsaver60を使用する時は、bootprm.h
 *   を使用する。）
 * 
 * リビジョン 1.16  1970/01/01  00:00:00  kagatume
 * 2004/03/16 11:00
 * ・BOOT_WRITE_STATUSのアドレス定義を変更する。
 * ・BOOT_END_ADDR、APLY_SET_SIZEの前リビジョンの変更を
 *   元に戻す。
 * 
 * リビジョン 1.15  1970/01/01  00:00:00  kagatume
 * 2004/03/16 9:00
 * ・BOOT_CHECK_ADDRの定義を削除し、APLY_START_STATUS、BOOT_WRITE_STATUSの
 *   定義を追加する。
 * ・APLY_JUMP_ADDRの定義を削除する。
 * ・BOOT_END_ADDRのアドレスを変更し、APLY_SET_SIZEを変更する。
 * ・MCU1_2_START_CHECKの定義を削除する。
 * ・mkmotstr、jump_to_reset、rscan、getmes、fromaplyfix、struprのプロトタイプ宣言を削除する。
 * ・変数upld、entryadrs、progsize、yourswの外部参照を削除する。
 * 
 * リビジョン 1.14  1970/01/01  00:00:00  kagatume
 * 2004/02/27 19:30
 * スタートアドレスチェック定義を追加する
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  kagatume
 * 2004/02/19 19:30
 * DMU基板のDIPSW1とDIPSW2のアドレスを追加する
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  kagatume
 * 2004/01/27 12:30
 * レジスタRSTCSRR の定義を追加する
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:40
 * RESET_CMD の定義を削除する
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2004/01/13:40
 * MCU3_START_CHECK を DMU_START_CHECK に変更する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  sakaguti
 * ﾌﾞｰﾄﾛｰﾀﾞ内部定義を削除
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * ﾃﾞﾊﾞｯｸﾞ用ｺﾏﾝﾄﾞ追加
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2003/11/21 19:30
 * アプリへのジャンプアドレス定義を追加する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2003/11/21 12:00
 * MSEC の定義を条件コンパイルに対応させる
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2003/11/17 12:00
 * プロトタイプ宣言 void InitAtaDisk( void ),
 * void InitFileSystem( void ) の追加
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2003/11/13 20:30
 * TCP/IP設定、SRAM初期化に関連した
 * プロトタイプ宣言を追加する。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 割込みﾏｽｸ定義追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * インタラプトのマスクとＡＣＫレジスタの変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＳＨ２用インクルード
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

void InitSramDisk( int flag );	/* ファイルシステムFSYS201用 */
void InitAtaDisk( void );		/* ファイルシステムFSYS201用 */
void InitFileSystem( void );	/* ファイルシステムFSYS201用 */

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

