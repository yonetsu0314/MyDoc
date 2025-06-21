/*======================================================================
 * File name    : cmfhdl.c
 * Original		: p:/dacs_i/rcu/main/rcs/cmfhdl.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *		通信デバイスファイルアクセスモジュール
 *		関数定義
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/bootload/rcs/cmfhdl.c 4.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: cmfhdl.c $
 * リビジョン 4.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * ・電断対応ファイルシステム搭載
 * ・インストール時間短縮を行う
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/04/08 20:30
 * DIPSW2の8番がONの場合のみcon0からのシリアル出力を許可するように
 * 変更する。
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/02/19 21:30
 * DIPSW2の8番がONの場合は、con0からのシリアル出力を行わないようにする
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/02/04 13:30
 * len が0の時にwriteできるようにfif_puts を変更する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/06 9:00
 * FIFOの追加に対応する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2003/11/17 21:00
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
#define	_CMFHDL_C_
#include <fcntl.h>				/* MS-C std */
#include <rom\handle.h>			/* Show include */
#include <rxif\rx116.h>			/* Show include */
#include <rom\xio.h>			/* Show include */
#include <custom\custom.h>		/* Show include */
#include <drive\comdrv.h>		/* Show include */
#include <sh7615\bootprm.h>		/* Show include */

#include "cmfhdl.h"				/* Show current dir. */

static unsigned char *fname_tbl[] = {
		"con0",		/* CON0 file name */
		"con1",		/* CON1 file name */
		"tty0",		/* TTY0 file name */
		"tty1",		/* TTY1 file name */
		"tty2",		/* TTY2 file name */
		"tty3",		/* TTY3 file name */
		"lan",		/* LAN file name */
		"fifo0",	/* FIFO file name */
};

static int line_tbl[COM_FH_MAX] = {0};

#define con0_fh		(line_tbl[CON0_FH])
#define con1_fh		(line_tbl[CON1_FH])
#define tty0_fh		(line_tbl[TTY0_FH])
#define tty1_fh		(line_tbl[TTY1_FH])
#define tty2_fh		(line_tbl[TTY2_FH])
#define tty3_fh		(line_tbl[TTY3_FH])
#define lan_fh		(line_tbl[LAN_FH])
#define fif_fh		(line_tbl[FIF_FH])


/*****************************************************************************
*
*	関数名: int port_tbl_init(void)
*
*	機　能: 通信用portテーブルにファイル･ハンドルを設定する。
*
*	引き数:	なし
*
*	戻り値: int
*					 0	: complete
*					-1	: error code
*
******************************************************************************/
int port_tbl_init(void)
{
	con0_fh = open(fname_tbl[CON0_DN], O_RDWR);
	con1_fh = open(fname_tbl[CON1_DN], O_RDWR);
	tty0_fh = open(fname_tbl[TTY0_DN], O_RDWR);
	tty1_fh = open(fname_tbl[TTY1_DN], O_RDWR);
	tty2_fh = open(fname_tbl[TTY2_DN], O_RDWR);
	tty3_fh = open(fname_tbl[TTY3_DN], O_RDWR);
	fif_fh = open(fname_tbl[FIF_DN], O_RDWR);
	return(0);
}
/*****************************************************************************
*
*	関数名: COMRV con_gets(unsigned char *buf, int max)
*
*	機　能: CON0 からリードする。
*
*	引き数: unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV con0_gets(unsigned char *buf, int max)
{
	int	len = max;		/* length of text	*/
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(con0_fh, buf, max)) < 0) {	/* pre_com １文字受信*/
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV con0_puts(unsigned char *buf, int len)
*
*	機　能: テキストを CON0 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	:error code
*
******************************************************************************/
COMRV con0_puts(unsigned char *buf, int len)
{
	int err, tmp;

	/* DIPSW2：8  OFFの場合シリアル出力を行わない*/
	if ((tmp = *DIPSW2_ADDR) & DIPSW_8_MASK )/* DIPSW2：8  OFF */ return(COM_COMPL);
	if (len <= 0) return(COM_FAIL);
	if((err = write(con0_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV con1_gets(unsigned char *buf, int max)
*
*	機　能: CON1 からリードする。
*
*	引き数: unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV con1_gets(unsigned char *buf, int max)
{
	int	len = max;		/* length of text	*/
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(con1_fh, buf, max)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV con1_puts(unsigned char *buf, int len)
*
*	機　能: テキストを CON1 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	:error code
*
******************************************************************************/
COMRV con1_puts(unsigned char *buf, int len)
{
	int err;

	if (len <= 0)
		return(COM_FAIL);
	if((err = write(con1_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV tty0_gets(unsigned char *buf, int max)
*
*	機　能: TTY0 からリードする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV tty0_gets(unsigned char *buf, int max)
{
	int len = max;		/* length of text	*/
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(tty0_fh, buf, max)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV tty0_puts(unsigned char *buf, int len)
*
*	機　能: テキストを TTY0 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	: error code
*
******************************************************************************/
COMRV tty0_puts(unsigned char *buf, int len)
{
	int	err;

	if(len <= 0) return(COM_FAIL);
	if((err = write(tty0_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV tty1_gets(unsigned char *buf, int max)
*
*	機　能: TTY1 からリードする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV tty1_gets(unsigned char *buf, int max)
{
	int len = max;		/* length of text */
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(tty1_fh, buf, max)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV tty1_puts(unsigned char *buf, int len)
*
*	機　能: テキストを TTY1 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	: error code
*
******************************************************************************/
COMRV tty1_puts(unsigned char *buf, int len)
{
	int err;

	if(len <= 0) return(COM_FAIL);
	if((err = write(tty1_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV tty2_gets(unsigned char *buf, int max)
*
*	機　能: TTY2 からリードする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV tty2_gets(unsigned char *buf, int max)
{
	int len = max;		/* length of text	*/
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(tty2_fh, buf, max)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV tty2_puts(unsigned char *buf, int len)
*
*	機　能: テキストを TTY2 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	: error code
*
******************************************************************************/
COMRV tty2_puts(unsigned char *buf, int len)
{
	int	err;

	if(len <= 0) return(COM_FAIL);
	if((err = write(tty2_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV tty3_gets(unsigned char *buf, int max)
*
*	機　能: TTY3 からリードする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV tty3_gets(unsigned char *buf, int max)
{
	int len = max;		/* length of text */
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(tty3_fh, buf, max)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV tty3_puts(unsigned char *buf, int len)
*
*	機　能: テキストを TTY3 にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	: error code
*
******************************************************************************/
COMRV tty3_puts(unsigned char *buf, int len)
{
	int err;

	if(len <= 0) return(COM_FAIL);
	if((err = write(tty3_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV lan_gets(unsigned char *buf, int max)
*
*	機　能: LAN からリードする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV lan_gets(unsigned char *buf, int max)
{
	int len = max;		/* length of text */
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(lan_fh, buf, 0)) < 0) {
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV lan_puts(unsigned char *buf, int len)
*
*	機　能: テキストを LAN にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	: error code
*
******************************************************************************/
COMRV lan_puts(unsigned char *buf, int len)
{
	int err;

	if(len <= 0)
		return(COM_FAIL);
	if((err = write(lan_fh, buf, 0)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
/*****************************************************************************
*
*	関数名: COMRV fif_gets(unsigned char *buf, int max)
*
*	機　能: FIFO からリードする。
*
*	引き数: unsigned char *buf	  -  pointer to text buff
*			int max				  -  maximum sizeof buffer
*
*	戻り値: COMRV
*					len			: success
*					RCV_ERROR	: fail
*
******************************************************************************/
COMRV fif_gets(unsigned char *buf, int max)
{
	int	len = max;		/* length of text	*/
	/*** len = max; はコンパイルワーニングをなくす為のダミー記述 ***/

	if((len = read(fif_fh, buf, max)) < 0) {	/* pre_com １文字受信*/
		return(COM_FAIL);
	}
	return(len);
}
/*****************************************************************************
*
*	関数名: COMRV fif_puts(unsigned char *buf, int len)
*
*	機　能: テキストを FIFO にライトする。
*
*	引き数:	unsigned char *buf	  -  pointer to text buff
*			int len				  -  length of text
*
*	戻り値: COMRV
*					COM_COMPL	: complete
*					COM_FAIL	:error code
*
******************************************************************************/
COMRV fif_puts(unsigned char *buf, int len)
{
	int err;

//	if (len <= 0) return(COM_FAIL);
	if((err = write(fif_fh, buf, len)) < 0) {
		return(COM_FAIL);
	}
	return(COM_COMPL);
}
