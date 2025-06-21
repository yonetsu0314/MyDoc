/*----------------------------------------------------------------------*
 * File name	:	bootcmds.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bootload/rcs/bootcmds.h 5.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: bootcmds.h $
 * リビジョン 5.2  1970/01/01  00:00:00  kagatume
 * 2005/6/27 18:00
 * ・DMUBTNAMEをTOO26Aとする
 * ・スキャンディスク、フォーマット関数の追加
 * 
 * リビジョン 5.1  1970/01/01  00:00:00  kagatume
 * 2005/6/27 13:30
 * ・電断対応ファイルシステム搭載
 * ・インストール時間短縮を行う
 * ・不要なコード削除
 * 
 * リビジョン 4.1  1970/01/01  00:00:00  kagatume
 * 2004/05/26 14:30
 * DMUブートローダのプログラム番号をt0026、WCUブートローダのプログラム番号をt0025
 * に変更したことに対応する
 * 
 * リビジョン 3.1  1970/01/01  00:00:00  kagatume
 * 2004/05/24 18:00
 * network設定のバックアップをFLASHからE2ROMへ移動したことに伴って
 * 共用体の宣言を新規に設ける。
 * 
 * リビジョン 2.5  1970/01/01  00:00:00  kagatume
 * 2004/03/24 14:00
 * 不要なコマンド処理関数を削除する
 * 
 * リビジョン 2.4  1970/01/01  00:00:00  kagatume
 * 2004/03/19 17:00
 * comEfuncは不要なので削除する
 * 
 * リビジョン 2.3  1970/01/01  00:00:00  kagatume
 * 2004/03/16 12:00
 * 不要な変数、プロトタイプ宣言、定義を削除する。
 * 
 * リビジョン 2.2  1970/01/01  00:00:00  kagatume
 * 2004/03/10 16:30
 * recover_programのプロトタイプ宣言を追加する
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/03/05 16:00
 * ・アドレス設定定義の追加
 * ・ブートローダーの書き換え用プロトタイプ宣言、定義の変更追加
 * 
 * リビジョン 1.8  1970/01/01  00:00:00  sakaguti
 * trash.c用のﾌﾟﾛﾄﾀｲﾌﾟ追加
 * 
 * リビジョン 1.7  1970/01/01  00:00:00  kagatume
 * 2004/01/28 20:00
 * jump_to_reset のプロトタイプ宣言は不要なので削除する
 * 
 * リビジョン 1.6  1970/01/01  00:00:00  kagatume
 * 2004/01/26 18:00
 * comRW のプロトタイプ宣言を追加する
 * 
 * リビジョン 1.5  1970/01/01  00:00:00  kagatume
 * 2004/01/22 20:50
 * MEM2STT, MEM2STP の値を変更する
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kagatume
 * 2004/01/15 20:00
 * 使用されている文字MCUをCCWに置換する
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2004/01/14 13:45
 * ファイル名とコメントをＣＣＷ用に変更する
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
#define	BACKUP_ON

#define WCUBKNAME "E:\\WCUBKUP.MOT"
#define WCUBTWRNM "E:\\WCUBTWR.MOT"
#define WCUBTNAME "E:\\T0025.MOT"

#define DMUBKNAME "E:\\DMUBKUP.MOT"
#define DMUBTWRNM "E:\\DMUBTWR.MOT"
#ifdef HOUSOU
#define DMUBTNAME "E:\\R6581.MOT"
#else
#define DMUBTNAME "E:\\T0026A.MOT"
#endif

#define ROMSTT  (0x20000000)
#define MEM0STT (0x06000000)
#define MEM0STP (0x060fffff)
#define MEM1STT (0x06100000)
#define MEM1STP (0x061fffff)
#define MEM2STT (0x06200000)
#define MEM2STP (0x062fffff)
#define MEM3STT (0x06300000)
#define MEM3STP (0x063fffff)
#define MEM4STT (0x06400000)	/* CFからファイル一括読込みに使用するtemp領域 */

#define	OFSADDR1	(0x00000000)
#define	OFSADDR2	(0x01000000)
#define	OFSADDR3	(0x01010000)

#define CFDIRNAME	"E:\\"
#define CFINFNAME	"E:\\PROGRAM.INF"
#define INFNAME		"PROGRAM.INF"
#define BKNOW		"BROKEN"
#define DMUBKDIR	"E:\\BOOTBAK"
#define BKINFNAME	"E:\\BOOTBAK\\PROGRAM.INF"

#define MAXINFLINE	(5)
#define LNMEMSIZ	(0x0010)
#define	RDWR_SIZE	(128*1024)

union netunion{
	unsigned short s[35];
	unsigned char  b[70];
};
union macunion{
	unsigned short s[10];
	unsigned char  b[20];
};

#undef	GLOBAL	
#ifdef	STORAGE
#define	GLOBAL	
int		tcpinput = 0;
int		tcpinputOK = 0;
#else
#define	GLOBAL	extern
extern int		tcpinput;
extern int		tcpinputOK;
#endif

GLOBAL char	versionstr[128];

/*
	INF Information
	0:INF 1:FPGA 2:WCU 3:DMU 4:INFEND
*/
GLOBAL char	prgformat[MAXINFLINE][8];		/* INFファイルから読込んだ文字列 */
GLOBAL char	prgname[MAXINFLINE][16];
GLOBAL char	prgver[MAXINFLINE][8];
GLOBAL char	prgsum[MAXINFLINE][8];
GLOBAL char	prgsize[MAXINFLINE][8];
GLOBAL int		memsize[MAXINFLINE];

GLOBAL char	bkbuf[16][16];

GLOBAL char	tmpprogname[16];		/* CPUから確認した文字列 */
GLOBAL char	tmpversion[16];
GLOBAL char	tmpsumnum[8];
GLOBAL char	tmpsizenum[8];

GLOBAL int		entryadrs;
GLOBAL int		progsize;
GLOBAL int		entryaaa;
GLOBAL int		progsss;
GLOBAL int		barlen;

GLOBAL char	dirname[120];
GLOBAL char	rdwrbuf[RDWR_SIZE+1];	/* ファイルリード、ライト用バッファ */

void	comA(void);
int 	comAfunc(int *sttadr, int *stpadr, int dorg, int m, int inc);
void	comC(void);
int 	comCfunc(int *sttadr1, int *sttadr2, int size);
void	comF(void);
int 	comFfunc(int ofs, char fname[]);
void	comH(void);
int 	comHfunc(char *memadr, int sttofs, int size, char fname[]);
void	comJ(void);
int 	comJfunc(short *flashadr, short *ramadr, unsigned long size,int barcnt);
void	comK(void);
int 	comKfunc(char *adr);
void	comL(void);
int 	comLfunc(int ofs,int barcnt);
void	comN(void);
void	comT(void);
int 	comTfunc(char *memadr, int sttofs, int size, char fname[],int barcnt);

void	comB(void);
void	comG(void);
void	comO(void);
void	comP(void);
void	comQ(void);
void	comR(void);
void	comI(void);
int		comE(void);
void comRB(void);
void comRC(void);
void comRE(void);
void comRF(void);
void comRH(void);
void comRS(void);
void comIC(void);
void comIS(void);
void comIF(void);
int comBTwrite2(void);
int	comBTwrite3(void);
int comGZfunc(void);
int	comGYfunc(void);
int	comGZ2func(void);
int	comGZ3func(void);
int comG2func(void);
int	comGDfunc(void);
int	comGUfunc(void);
int	comGBfunc(void);
int recover_program(int err);
void putmachex(char *adr);
void putiphex(char *adr);

void	mkmotstr(char buf[], char mem[], int ofs, int len, int mode);
int		transMOT(char *buf, int ofs, char **maxpp, char **minpp);
int		calprogsize(int,int);
void	inxBAR(int dat);
void	dispBAR(int);
void	setBAR(int);
void	incBAR(int);
void	rscan(void);
int		filebkup(char fname[],int n);
int		filersm(char fname[]);
int		rmbkall(void);
int		loadINF(char *);
int		recvINF(void);
int		transINF(char *buf);
void	setccwinf(int n, char *buf);
void	setinfend(int n, char *buf);
char	*strupr(char *buf);
void	pickversion(char *);
int		upldcfasc(char fname[],int barcnt);
