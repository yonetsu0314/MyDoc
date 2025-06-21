/*----------------------------------------------------------------------*
 * File name	:	init.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/t0051/rcs/init.h 3.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: init.h $
 * リビジョン 3.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * network設定のバックアップをFLASHからE2ROMへ移動したことに伴って
 * 関数のプロトタイプ宣言を新規に設ける。
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/08 19:30
 * 起動時のモード判別処理を整理する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/02/17 19:00
 * ＤＭＵアプリのサム値エラーを示す関数を追加
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ｲﾝｸﾙｰﾄﾞ･ﾌｧｲﾙを整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ソースをわかりやすく整理
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

//#define	disable()		set_imask( 15 )

#define	INTVEC (0x06000000)

#ifndef	V_FUNC
#include <biosif\icu.h>			/* Show include	*/
#endif
//typedef void	(*V_FUNC)();

typedef struct	vectsx {			/* APL vector table		*/
	int	no;			/* 割り込みﾊﾝﾄﾞﾗの数		*/
	V_FUNC	fun[128];		/* 関数へのﾎﾟｲﾝﾀの配列		*/
} VECTSX;

/******************************/
/* Interrupt Table definition */
/******************************/
typedef struct	apl_info {
	void	*apl_start;
	void	*vect_no;
	V_FUNC	*vecter_adr;
};

extern const char prog_id[];
extern	int CmdFd;

extern int *_D_ROM, *_D_BGN, *_D_END, *_B_BGN, *_B_END;
extern void INITSCT(void);

void set_SumCheck_NG(void);
void clear_SumCheck_NG(void);
int check_SumCheck_NG(void);

unsigned short SysFromAplyChecksum(void);
unsigned short TcpAplyChecksum(void);
int aply_start_check(void);
void start_aply(void);
int Aply_CheckSum(void);
unsigned short SysFromAplyChecksum(void);
unsigned short TcpAplyChecksum(void);
void set_default_ipaddr(void);
int compare_TcpipBackup(void);
void set_Tcpip_toUsnet(void);
void read_fromTcpipBackup(void);
void write_toTcpipBackup(void);
void	getversionstr(char *verstr, char *idptr);
void	INIT(void);
void	INITSCT(void);
void main(void);


