/*======================================================================
 * File name    : s60fat.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/s60fat.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: s60fat.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/17 12:30
 * API変換定義fsapicnv.hのインクルードをはずす
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *			(c) Copyright 2002, ISHIDA CO., LTD.
 *			959-1 SHIMOMAGARI RITTO-CITY
 *			SHIGA JAPAN
 *			(077) 553-4141
 *======================================================================
 */
/******************************************************************************/
/* 関 数 名：S60FAT.H                                                         */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：SAVER60 FATファイルシステム ライブラリ用インクルードファイル     */
/* 作 成 日：2004.06.01                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#ifndef __S60FAT_H__
#define __S60FAT_H__
/******************************************************************************/
/* データ型定義                                                               */
/******************************************************************************/

typedef signed   short  F_BOOL;         /* 16bitブール値                      */
typedef signed   char   F_CHAR;         /* 符号付8bit値                       */
typedef unsigned char   F_BYTE;         /* 符号無し8bit値                     */
typedef signed   short  F_SHORT;        /* 符号付16bit値                      */
typedef unsigned short  F_WORD;         /* 符号無し16bit値                    */
typedef signed   int    F_INT;          /* 符号付16/32bit値(実装依存)         */
typedef unsigned int    F_UINT;         /* 符号無し16/32bit値(実装依存)       */
typedef signed   long   F_LONG;         /* 符号付32bit値                      */
typedef unsigned long   F_DWORD;        /* 符号無し32bit値                    */
typedef void            F_VOID;         /* void型                             */

/******************************************************************************/
/* 構造体定義                                                                 */
/******************************************************************************/

/* フォーマット情報 */
typedef struct tag_TFMR_FORMATINFO{
    F_BYTE driverid;                    /* ドライバID                         */
    F_BYTE chanel_unit;                 /* チャネル番号/ユニット番号          */
    F_WORD entry_cnt;                   /* ルートディレクトリエントリ数       */
    F_BYTE FATtype;                     /* FATタイプ                          */
    F_BYTE reserved[ 3 ];               /* 予約領域                           */
    F_CHAR * vol_name;                  /* ボリューム名                       */
}TFMR_FORMATINFO;

/* マウント情報 */
typedef struct tag_TFMR_MOUNTINFO{
    F_BYTE driverid;                    /* ドライバID                         */
    F_BYTE chanel_unit;                 /* チャネル番号/ユニット番号          */
    F_BYTE reserve[2];                  /* 予備                               */
}TFMR_MOUNTINFO;

/* ファイル状態構造体 */
#define DFMR_FNAMESZ                 8  /* ファイル名のサイズ                 */
#define DFMR_EXTSZ                   3  /* 拡張子のサイズ                     */
#define DFMR_MAXFILESZ             256  /* ファイル名サイズの最大値(256文字)  */

typedef struct tagTFMR_FILE_INFO{
    unsigned short crtdate;             /* 作成日付                           */
    unsigned short crttime;             /* 作成時刻                           */
    unsigned short lstaccdate;          /* 最終アクセス日付                   */
    unsigned short wrtdate;             /* 更新日付                           */
    unsigned short wrttime;             /* 更新時刻                           */
    unsigned char crtmsec;              /* 作成時刻(msec)                     */
    unsigned char attr;                 /* ファイル属性                       */
    unsigned long filesize;             /* ファイルサイズ                     */
    unsigned char sfn[ DFMR_FNAMESZ + DFMR_EXTSZ ];
                                        /* ファイル名                         */
    unsigned char reserved;             /* 予約領域                           */
    unsigned char lfn[ DFMR_MAXFILESZ ];/* ロングファイル名                   */
}TFMR_FILE_INFO;

/* ドライブ状態構造体 */
#define DFMR_VNAMESZ                11  /* ボリューム名サイズ                 */

typedef struct tagTFMR_DRIVE_INFO{
    unsigned char volume[DFMR_VNAMESZ ];/* ボリュームラベル                   */
    unsigned char mediatype;            /* メディアタイプ                     */
    unsigned char fattype;              /* FATタイプ                          */
    unsigned char reserved;             /* 予約領域                           */
    unsigned long totalsize;            /* 容量                               */
    unsigned long usedsize;             /* 使用領域                           */
    unsigned long emptyzise;            /* 空き領域                           */
}TFMR_DRIVE_INFO;

/* 検査ログ構造体 */
    /* 元ファイル名出力形式：ファイル名指定 */
typedef struct tagTFMR_SDLOGTBLSHT{
	F_CHAR pcename[ 11 ];               /* 修復ファイル名                     */
	F_CHAR errcode;                     /* エラーコード                       */
	F_DWORD orgtopclstnum;              /* 元ファイルの先頭クラスタ番号       */
	F_WORD orgclstnum;                  /* 元ファイルのクラスタ数             */
	F_CHAR orgname[ DFMR_FNAMESZ + DFMR_EXTSZ + 1 ];
	                                    /* 元ファイル名                       */
	                                    /* (ファイル+拡張子+'.')              */
	F_CHAR reserved[ 2 ];               /* 予約領域                           */
}TFMR_SDLOGTBLSHT;

    /* 元ファイル名出力形式：フルパス指定 */
typedef struct tagTFMR_SDLOGTBLLNG{
	F_CHAR pcename[ 11 ];               /* 修復ファイル名                     */
	F_CHAR errcode;                     /* エラーコード                       */
	F_DWORD orgtopclstnum;              /* 元ファイルの先頭クラスタ番号       */
	F_WORD orgclstnum;                  /* 元ファイルのクラスタ数             */
	F_CHAR orgname[ DFMR_MAXFILESZ ];   /* 元ファイル名(フルパス)             */
	F_CHAR reserved[ 2 ];               /* 予約領域                           */
}TFMR_SDLOGTBLLNG;

    /* 検査ログエラーコード */
#define DFMR_SD_LOGERRTAILOFF        1  /* ディレクトリクラスタリンク異常     */
#define DFMR_SD_LOGERRMULTI          2  /* ファイルクラスタリンク異常         */
#define DFMR_SD_LOGERRDIV            3  /* 不良クラスタ検出                   */
#define DFMR_SD_LOGERRMSIZE          4  /* ファイルサイズエラー               */
#define DFMR_SD_LOGERRTOPBAD         5  /* ディレクトリ先頭クラスタ不良       */
#define DFMR_SD_LOGERRTOPILL         6  /* 不正ディレクトリ                   */
                                        /* (先頭に｢.｣｢..｣が存在しない)        */
#define DFMR_SD_LOGERRLFNILL         7  /* 不正ロングファイル名               */
                                        /* (順序/サム値が不正)                */
#define DFMR_SD_LOGERRNOTOP          8  /* ディレクトリ先頭クラスタ異常       */
#define DFMR_SD_LOGERRRELINK         9  /* 再帰リンク検出                     */

/******************************************************************************/
/* 関数宣言                                                                   */
/******************************************************************************/
/******************************************************************************/
/* ファイルシステム構築                                                       */
/******************************************************************************/

/* ファイルシステムの初期化 */
int FMR_Init( 
    void
);

/* ファイルシステムのフォーマット */
int FMR_Format(
    TFMR_FORMATINFO * formatinfo        /* フォーマット情報領域アドレス       */
);

/* マウント */
int FMR_Mount(
	char drvnum,                        /* 論理ドライブ番号                   */
	char * raminfo,                     /* RAM領域アドレス                    */
	unsigned long ramlen,               /* RAM領域サイズ                      */
	TFMR_MOUNTINFO * mountinfo          /* マウント情報領域アドレス           */
);

/* アンマウント */
int FMR_Unmount(
	char drvnum                         /* 論理ドライブ番号                   */
);

/******************************************************************************/
/* ファイル操作                                                               */
/******************************************************************************/
/* ファイルのオープン */
int FMR_Open ( 
	const char * path,                  /* オープンするファイルのパス名       */
	int flag,                           /* オープンフラグ                     */
	unsigned char atrb                  /* ファイル属性                       */
);

/* ファイルのクローズ */
int FMR_Close ( 
    int filehandle                      /* ファイルハンドル                   */
);

/* ファイルへのデータ書込み */
int FMR_Write ( 
	int filehandle,                     /* ファイルハンドル                   */
	void * writebuff,                   /* 書込みデータ格納領域アドレス       */
	unsigned int size                   /* 書込みサイズ                       */
);

/* ファイルからのデータ読込み */
int FMR_Read ( 
	int filehandle,                     /* ファイルハンドル                   */
	void * readbuff,                    /* 読込みデータ格納領域アドレス       */
	unsigned int size                   /* 読込みサイズ                       */
);

/* ファイルポインタの移動 */
long FMR_Seek ( 
	int filehandle,                     /* ファイルハンドル                   */
	long offset,                        /* オフセット                         */
	int origin                          /* 初期位置                           */
);

/* ファイルポインタの取得 */
long FMR_Tell ( 
	int filehandle                      /* ファイルハンドル                   */
);

/* ファイルの削除 */
int FMR_Remove ( 
	const char * path                   /* 削除するファイルのパス名           */
);

/* ファイル属性を設定する */
int FMR_SetAtr ( 
	const char * path,                  /* 設定するファイルのパス名           */
	unsigned char attr                  /* ファイル属性                       */
);

/* ファイル/ディレクトリ名称変更 */
int FMR_Rename ( 
	const char * oldpath,               /* 旧パス名                           */
	const char * newpath                /* 新パス名                           */
);

/******************************************************************************/
/* ディレクトリ操作                                                           */
/******************************************************************************/
/* ディレクトリの作成 */
int FMR_Mkdir ( 
	const char * path                   /* 作成するディレクトリのパス名       */
);

/* ディレクトリの削除 */
int FMR_Rmdir ( 
	const char * path                   /* 削除するディレクトリのパス名       */
);

/******************************************************************************/
/* その他                                                                     */
/******************************************************************************/
/* ディレクトリ/ファイルの検索開始 */
long FMR_FindFirst ( 
	const char * path,                  /* 検索するパス名                     */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域アドレス       */
);

/* ディレクトリ/ファイルの検索 */
int FMR_FindNext ( 
	long srchhandle,                    /* 検索ハンドル                       */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域アドレス       */
);

/* ディレクトリ/ファイル検索終了 */
int FMR_FindClose ( 
	long srchhandle                     /* 検索ハンドル                       */
);

/* ファイル状態を取得する */
int FMR_Stat ( 
	const char * path,                  /* 取得するファイルのパス名           */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域アドレス       */
);

/* ファイルシステム情報取得 */
int FMR_Info ( 
	F_CHAR drvnum,                      /* ドライブ番号                       */
	TFMR_DRIVE_INFO * drvinfo           /* ドライブ状態格納領域アドレス        */
);

/* ファイルシステムを検査する */
int FMR_ScanDisk(
	char drvnum,                        /* ドライブ番号                       */
	unsigned char * ramaddr ,           /* RAM領域アドレス                    */
	unsigned long ramsize,              /* RAM領域サイズ                      */
	unsigned char * logarea,            /* 検査ログ領域アドレス               */
	unsigned long logsize,              /* 検査ログ領域サイズ                 */
	unsigned char logflg                /* 検査ログ動作フラグ                 */
);

/* fget, fput 2005/01/11追加(APIMISC.C) */
char *fmr_fgets(char *s, int n, int fhdl);
int fmr_fgetc(int fhdl);
int fmr_fputs(const char *s, int fhdl);
int fmr_fputc(int ch, int fhdl);

/******************************************************************************/
/* 定数定義                                                                   */
/******************************************************************************/

/*------------------------------------*/
/*--- ドライバID ---------------------*/
/*------------------------------------*/
#define DFMR_DRVID_PCATA          0x01  /* PCATAドライバ                      */
#define DFMR_DRVID_IDE            0x02  /* IDEドライバ                        */
#define DFMR_DRVID_RAM            0x03  /* RAMディスクドライバ                */

/*------------------------------------*/
/*--- FATタイプ ----------------------*/
/*------------------------------------*/
#define DFMR_FATTYPEDEP           0x00 /* FATタイプ:ユニット容量に依存        */
#define DFMR_FATTYPE12            0x01 /* FATタイプ:FAT12                     */
#define DFMR_FATTYPE16            0x02 /* FATタイプ:FAT16                     */
#define DFMR_FATTYPE32            0x03 /* FATタイプ:FAT32                     */

/*------------------------------------*/
/*--- MBR有無 ------------------------*/
/*------------------------------------*/
#define DFMR_MBROFF               0x80  /* MBR無し                            */

/*------------------------------------*/
/*--- パーティションタイプ------------*/
/*------------------------------------*/
#define DFMR_PRTTBL_TYPEF12       0x01  /* パーティションタイプ=FAT12         */
#define DFMR_PRTTBL_TYPEF16LOW    0x04  /* パーティションタイプ=FAT16         */
                                        /* 32MByte未満                        */
#define DFMR_PRTTBL_TYPEF16HI     0x06  /* パーティションタイプ=FAT16         */
                                        /* 32MByte以上                        */
#define DFMR_PRTTBL_TYPEF32LBA    0x0C  /* パーティションタイプ=FAT32(LBA)    */
#define DFMR_PRTTBL_TYPEF16LBA    0x0E  /* パーティションタイプ=FAT16(LBA)    */

/*------------------------------------*/
/*--- アクセスモード -----------------*/
/*------------------------------------*/
#define DFMR_O_RDONLY           0x0000  /* リードオンリー                     */
#define DFMR_O_WRONLY           0x0001  /* ライトオンリー                     */
#define DFMR_O_RDWR             0x0002  /* リード／ライト                     */
#define DFMR_O_APPEND           0x0008  /* アペンド                           */
#define DFMR_O_CREAT            0x0100  /* クリエイト                         */
#define DFMR_O_TRUNC            0x0200  /* トランケイト                       */
#define DFMR_O_EXCL             0x0400  /* エクスクルーシブ                   */

/*------------------------------------*/
/*--- ファイル属性 -------------------*/
/*------------------------------------*/
#define DFMR_ATTR_RONLY           0x01  /* ファイルの属性(読込み専用)         */
#define DFMR_ATTR_HIDE            0x02  /* ファイルの属性(隠しファイル)       */
#define DFMR_ATTR_SYS             0x04  /* ファイルの属性(システムファイル)   */
#define DFMR_ATTR_VOLID           0x08  /* ファイルの属性(ボリュームID)       */
#define DFMR_ATTR_DIR             0x10  /* ファイルの属性(ディレクトリ)       */
#define DFMR_ATTR_ARC             0x20  /* ファイルの属性(アーカイブ)         */

/*------------------------------------*/
/*--- SEEK 初期位置 ------------------*/
/*------------------------------------*/
#define DFMR_SEEK_SET                0  /* ファイルの先頭                     */
#define DFMR_SEEK_CUR                1  /* ファイルポインタの現在位置         */
#define DFMR_SEEK_END                2  /* ファイルの終端                     */

/*------------------------------------*/
/*--- エラーコード -------------------*/
/*------------------------------------*/
#define DFMR_SUC                 ( 0 )  /* API正常終了                        */
#define DFMR_ENOENT             ( -2 )  /* 該当ファイル無し                   */
#define DFMR_EBADF              ( -9 )  /* ファイルアクセス異常               */
#define DFMR_EACCES            ( -13 )  /* オープンモード不一致               */
#define DFMR_EEXIST            ( -17 )  /* 既存ファイル有り                   */
#define DFMR_EINVAL            ( -22 )  /* 不正な値                           */
#define DFMR_EMFILE            ( -24 )  /* オープンファイル数オーバー         */
#define DFMR_EFBIG             ( -27 )  /* ファイルサイズオーバー             */
#define DFMR_ENOTEMPTY         ( -41 )  /* 対象ディレクトリ中身あり           */
#define DFMR_MULTIOPEN         ( -43 )  /* ２重オープン                       */
#define DFMR_NMERR             ( -50 )  /* 名称異常                           */
#define DFMR_ERRPARAM          ( -53 )  /* パラメータエラー                   */
#define DFMR_FATALERR          ( -54 )  /* FATAL ERROR                        */
#define DFMR_NOENTRY           ( -60 )  /* 対象エントリ無し                   */
#define DFMR_NODIR             ( -61 )  /* 対象上位ディレクトリ無し           */
#define DFMR_ENOEMPTYENTRY     ( -70 )  /* 未使用/無効エントリ領域なし        */
#define DFMR_MEMFULL           ( -71 )  /* 空容量無し                         */
#define DFMR_ESEARCHFAIL       ( -72 )  /* 検索失敗                           */
#define DFMR_EIMPFORMAT        ( -73 )  /* フォーマット未完了                 */
#define DFMR_CLSTNUMERR       ( -256 )  /* クラスタ番号異常                   */
#define DFMR_NOFATENTRY       ( -257 )  /* FATエントリなし                    */
#define DFMR_DRVINITERR       ( -258 )  /* ドライバ初期化エラー               */
#define DFMR_CRESENERR        ( -259 )  /* セマフォ生成エラー                 */
#define DFMR_NODRV            ( -260 )  /* 対象ドライバなし                   */
#define DFMR_CHANNELERR       ( -261 )  /* チャネル番号エラー                 */
#define DFMR_PARTCNTERR       ( -262 )  /* パーティション数エラー             */
#define DFMR_PARTTYPERR       ( -263 )  /* パーティションタイプエラー         */
#define DFMR_WAISENERR        ( -264 )  /* セマフォ獲得エラー                 */
#define DFMR_SIGSENERR        ( -265 )  /* セマフォ返却エラー                 */
#define DFMR_FATTYPEERR       ( -266 )  /* FATタイプエラー                    */
#define DFMR_ENTRYCNTERR      ( -267 )  /* エントリ数エラー                   */
#define DFMR_RAMSIZEERR       ( -268 )  /* RAM領域サイズエラー                */
#define DFMR_FATERR           ( -269 )  /* FATサイズエラー                    */
#define DFMR_DRVNUMERR        ( -270 )  /* 論理ドライブ番号エラー             */
#define DFMR_DRVCONNECTERR    ( -271 )  /* 論理ドライブ接続済みエラー         */
#define DFMR_DRVNOCONNECTERR  ( -272 )  /* 論理ドライブ未接続エラー           */
#define DFMR_FORMATERR        ( -273 )  /* ルートディレクトリ作成エラー       */
#define DFMR_SDILLEGALDIR     ( -300 )  /* 不正なディレクトリ                 */
                                        /* (先頭に｢.｣｢..｣が存在しない)        */
#define DFMR_SDILLEGALROOT    ( -301 )  /* ルートディレクトリに読込み不能の   */
                                        /* セクタが存在する(FAT12/16のみ)     */
#define DFMR_SDROOTENTFULL    ( -302 )  /* 修復ファイル作成不可(ルート直下に  */
                                        /* 空きエントリ位置が存在しない)      */
#define DFMR_SDSCRAPERR       ( -303 )  /* 修復ファイル作成エラー             */
#define DFMR_SDLOGOUTERR      ( -304 )  /* 検査ログファイル出力エラー         */
#define DFMR_SDALLERRDIR      ( -306 )  /* ルートディレクトリが全て破損       */

/* ドライバエラーコード */

#define DFMR_DRVERR_PARAM     ( -512 )  /* パラメタエラー                     */
#define DFMR_DRVERR_NOTINIT   ( -513 )  /* 未初期化エラー                     */
#define DFMR_DRVERR_NOCARD    ( -514 )  /* カード抜きエラー                   */
#define DFMR_DRVERR_ILGALVS   ( -515 )  /* VS1,2状態エラー                    */
#define DFMR_DRVERR_MRSHPC    ( -516 )  /* MR-SHPC-01異常                     */
#define DFMR_DRVERR_PCCIS     ( -517 )  /* PCカード CIS異常                   */
#define DFMR_DRVERR_PCREADYTMO ( -518 ) /* PCカードReady TimeOut              */
#define DFMR_DRVERR_PCCARD    ( -519 )  /* PCカード異常                       */
#define DFMR_DRVERR_TSLP      ( -520 )  /* PCATA_osSleep異常終了              */
#define DFMR_DRVERR_UNITOVER  ( -521 )  /* 該当ユニット無し                   */
#define DFMR_DRVERR_NOTMOUNT  ( -522 )  /* 未マウントエラー                   */
#define DFMR_DRVERR_DEFCARD   ( -523 )  /* 別カード挿入                       */
#define DFMR_DRVERR_ILGCARD   ( -524 )  /* カード挿抜発生                     */
#define DFMR_DRERR_MOUNT      ( -525 )  /* マウント済みエラー                 */
#define DFMR_DRERR_NOPRIMARY  ( -526 )  /* 基本領域無し                       */
#define DFMR_DRERR_NOEXTEND   ( -527 )  /* 拡張領域無し                       */

#endif /* __S60FAT_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
