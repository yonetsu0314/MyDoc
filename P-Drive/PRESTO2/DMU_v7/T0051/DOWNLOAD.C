/*======================================================================
 * File name    : download.c
 * Original		: 
 *----------------------------------------------------------------------
 * Function 	:
 *		プログラムダウンロード用関数
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/download.c 2.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: download.c $
 * リビジョン 2.3  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * WCUブートローダー移行コマンドの送信許可設定処理は不要なので削除する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:00
 * ブートローダーの書き換えができるように関数を追加する
 * ダウンロード時のアドレスが合うようにcomWCU_F, comWCU_Lを変更する
 * 
 * リビジョン 1.13  1970/01/01  00:00:00  kagatume
 * 2004/02/13 12:30
 * WCUブートローダー移行コマンド送信処理をremmon.cから移動整理
 * 
 * リビジョン 1.12  1970/01/01  00:00:00  kagatume
 * 2004/02/06 17:00
 * WCUブートローダー移行コマンドの送信許可設定の追加
 * 
 * リビジョン 1.11  1970/01/01  00:00:00  kagatume
 * 2004/01/26 14:00
 * コメントの追加
 * 
 * リビジョン 1.10  1970/01/01  00:00:00  kagatume
 * 2004/01/22 21:00
 * Webリモコンからのダウンロードに対応する
 * 
 * リビジョン 1.9  1970/01/01  00:00:00  kagatume
 * 2004/01/09 21:00
 * comWCU_F の不要な記述を削除する
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  kagatume
 * 2004/01/08 19:00
 * comWCU_F を変更し、データ異常、サム値エラーに対応する
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/08 15:40
 * comWCU_F, send_WCU_data を変更する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/08 14:00
 * comWCU_F を変更する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/07 16:00
 * fifo への移行に伴ってプログラムを整理する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/06 14:30
 * send_WCU_data をfifo 用に変更する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/05 16:00
 * ダウンロード用関数の追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 1993, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *			SHIGA JAPAN
 *			(0775) 53-4141
 *======================================================================
 */
#define	_DOWNLOAD_C_

#include	<string.h>
#include	<kernel.h>
#include	<fsys.h>
#include	<sh7615\bootpara.h>
#include	"download.h"
#include	"cmfhdl.h"
#include	"bootcmds.h"
#include	"rsinout.h"
#include	"fifinout.h"

void		ctoh(unsigned char, char *);

/*---------------------------------------------------------------
				WCUブートローダー移行コマンド送信処理
----------------------------------------------------------------*/
#define	ICU_ADDR		(80)
#define	RCU_ADDR		(90)
#define	WCU_BOOT_CMD	(0xa9)

static int cal_checksum(char *text,int len)
{
	int i, sum = 0;

	for (i = 0; i < len; i++) {
		sum += *(text++);
	}
	return sum &= 0xff;
}
	
static void space_to_zero(char *text,int len)
{
	int i;

	for (i = 0; i < len; i++) {
		if (*text == '\0')
			break;
		else if (*text == ' ')
			*text = '0';
		text++;
	}
}

void send_wcu_boot_cmd(void)
{
	static char buf[20];
	int len, sum;

	buf[0] = RCU_ADDR;
	buf[1] = ICU_ADDR;
	
	buf[2] = 'R';
	buf[3] = 'C';
	buf[4] = 'U';
	buf[5] = '0';
	
	buf[6] = 'I';
	buf[7] = 'C';
	buf[8] = 'U';
	buf[9] = '0';
	
	buf[10] = WCU_BOOT_CMD;
	buf[11] = '0';
	buf[12] = '0';
	buf[13] = '0';
	buf[14] = '0';
	
	len = strlen(buf + 2);
	space_to_zero(buf + 2, len);
	sum = 0x100 - cal_checksum(buf + 2, len);
	ctoh((unsigned char)sum, buf + len + 2);
	buf[len+4] = '\0';
	fif_puts((unsigned char *)buf, 0);	/* WCUへテキスト送信 */
}


/*---------------------------------------------------------------
				WCUへのデータ送信関数
----------------------------------------------------------------*/
void send_WCU_data(char *buf)
{
	int len;
	
	len = strlen(buf);
	buf[len++] = (char)0x0d;
	buf[len++] = (char)0x0a;	/* for ASCII mode */
	buf[len]   = (char)0;
	fif_puts((unsigned char *)buf, len);		/* WCUへ送信 */
}


/*---------------------------------------------------------------
				WCUへのコマンド送信関数
----------------------------------------------------------------*/
void send_WCU_command(char cmd)
{
	fifoout('W');
	wait_WCU_ack();
	fifoout(cmd);
}


/*---------------------------------------------------------------
				WCUからの応答受信関数
----------------------------------------------------------------*/
int wait_WCU_ack(void)
{
	char ret;
	
	for (;;) {
		ret = fifoin();
		if (ret == '>')	return 0;		/* OK */
		if (ret == '!')	return -1;		/* ERROR */
	}
}


/*---------------------------------------------------------------
				WCUバージョン情報表示
----------------------------------------------------------------*/
int comWCU_N(char cmd)	
{		
	char	verstr[128];

	rsstr("\n");
	send_WCU_command(cmd);
	memcpy((void *)verstr, (void *)get_fifstr(), sizeof(verstr));
	pickversion(verstr);	
	return wait_WCU_ack();
}


/*---------------------------------------------------------------
				WCUアプリ起動
----------------------------------------------------------------*/
int comWCU_K(void)
{		
	send_WCU_command('K');	
	return wait_WCU_ack();
}


/*---------------------------------------------------------------
				WCUメモリクリア
----------------------------------------------------------------*/
int comWCU_A(void)
{		
	send_WCU_command('A');
	return wait_WCU_ack();
}


/*---------------------------------------------------------------
				CFの内容をWCUへダウンロード
----------------------------------------------------------------*/
int comWCU_F(char cmd, char fname[]) 
{		
	ST_FILE	*fp;
	static char	buf[120];
	char	*maxp,*minp;
	int		ret=0;

	rsstr("\n");
	
	minp = (char *)0xffffffff;
	maxp = (char *)0x00000000;

	fp = fopen( (char*)fname, "r" );
	if( fp == NULL ) return(-1);
	
	send_WCU_command(cmd);	/* L:WCUブートローダー、FPGA用	M:WCUブートライター用	O:WCUアプリ用 */
	wait_WCU_ack();
	
	memset(buf, 0, sizeof(buf));
	for(;;) {
		if (!(buf[1]=='7' || buf[1]=='8' || buf[1]=='9')) {
			if(fgets(buf, (short)120, fp)==NULL) break;
			send_WCU_data(buf);
		}
	
		if (fifochk()) ret = fifoin();
		if (ret == '>') break;
		if (ret == '1') rsstr("\nIs Data strange,isn't it?\n");
		if (ret == '2') rsstr("\nCheck Sum Error !!\n");
		if (ret == '3') {	
			rsstr("\nTime out\n");
			fclose(fp);
			return(-1);
		}
	}
	fclose(fp);
	return 0;
}


/*---------------------------------------------------------------
				WCUのメモリからFLASHへブロック書き込み
----------------------------------------------------------------*/
int comWCU_J(char cmd)
{		
	send_WCU_command(cmd);
	return wait_WCU_ack();
}


/*---------------------------------------------------------------
				MOTファイル受信後、WCUへダウンロード
----------------------------------------------------------------*/
int comWCU_L(char cmd)
{		
	char	buf[120];
	int		i,j,ret,ofs,sttofs=0;

	if (cmd == 'L') sttofs = OFSADDR1;	/* WCUブートローダー、FPGA用 */
	if (cmd == 'M') sttofs = OFSADDR2;	/* WCUブートライター用 */
	if (cmd == 'O') sttofs = OFSADDR3;	/* WCUアプリ用 */
	
	if(comLfunc(MEM2STT-sttofs,0)!=0){
		return(-1);
	}
	
	send_WCU_command(cmd);
	wait_WCU_ack();
		
	for(i=0;i<progsize;i+=16){
		ofs = entryadrs + i;
		j = progsize - i;
		if(j>16) j = 16;
		mkmotstr(buf,(char *)MEM2STT-sttofs,ofs,j,1);
		fif_puts((unsigned char *)buf, (int)strlen(buf));		/* WCUへ送信 */
		
		if (fifochk()) ret = fifoin();
		if (ret == '>') return(-1);
		if (ret == '1') rsstr("\nIs Data strange,isn't it?\n");
		if (ret == '2') rsstr("\nCheck Sum Error !!\n");
		if (ret == '3') {	
			rsstr("\nTime out\n");
			return(-1);
		}
	}
	mkmotstr(buf,(char *)NULL,entryadrs,0,2);
	fif_puts((unsigned char *)buf, (int)strlen(buf));		/* WCUへ送信 */
	for(;;) {		
		if (fifochk()) ret = fifoin();
		if (ret == '>') break;
		if (ret == '1') rsstr("\nIs Data strange,isn't it?\n");
		if (ret == '2') rsstr("\nCheck Sum Error !!\n");
		if (ret == '3') {	
			rsstr("\nTime out\n");
			return(-1);
		}
	}
	return 0;
}


/*---------------------------------------------------------------
				WCUのメモリからDMUメモリへアプリ書き込み
----------------------------------------------------------------*/
static int wait_fifo_empty(void)
{
	for (;;) {		
		if (fifochk())	fifoin();	/* FIFOが空になるまで読む */
		else {
			dly_tsk((RELTIM)3000/MSEC);	/* FIFOが空になってから3秒間待つ */
			if (!fifochk()) break;	/* FIFOが空なら終了 */
		}
	}
	return -1;
}

int comWCU_U(void)
{
	char	buf[120], *maxp,*minp;
	int ret;
		
	minp = (char *)0xffffffff;
	maxp = (char *)0x00000000;
	
	fif_buf_clear();
	
	send_WCU_command('U');
	if (wait_WCU_ack()) {
		rsstr("\nNo WCU Aplication\n");
		return 1;
	}
	
	for (;;) {
		memcpy((void *)buf, (void *)get_fifdata(), sizeof(buf));
		ret = transMOT(buf, MEM2STT-OFSADDR3, &maxp, &minp);
		if (ret == -1) {
			rsstr("\nend");
			rsstr("\nbegin:"); put8hex((int)minp);
			rsstr("\n  end:"); put8hex((int)maxp);
			entryadrs = (int)minp;
			progsize = (int)(maxp - minp);
			break;
		}
		if (ret == 1) {		/* データ異常 */
			rsstr("\nIs Data strange,isn't it?\n");
			return wait_fifo_empty();
		}
		if (ret == 2) {		/* サム値エラー */
			rsstr("\nCheck Sum Error !!\n");
			return wait_fifo_empty();
		}
	}
	return 0;
}


/*---------------------------------------------------------------
				WCUのリセット実行
----------------------------------------------------------------*/
int comWCU_R(void)
{		
	send_WCU_command('R');
	return 0;
}

