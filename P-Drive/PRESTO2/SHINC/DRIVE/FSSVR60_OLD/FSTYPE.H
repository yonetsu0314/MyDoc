/*======================================================================
 * File name    : fstype.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/fstype.h 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fstype.h $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/17 9:00
 * Saver60 Ver.01.04 up
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
/* 関 数 名：FMR_Type.h                                                       */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：プロトタイプ宣言                                                 */
/*         ：                                                                 */
/* 作 成 日：2004.04.15                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 FMR_copyBackSector,FMR_copyForwardSector削除    05/02/09 Y.Yoshizumi */
/*       FMR_copySector追加                              05/02/09 Y.Yoshizumi */
/* 01.05 FMR_splitPath戻り値の型をF_WORDからF_SHORTに変更05/02/28 Y.Yoshizumi */
/******************************************************************************/

#ifndef __FMR_TYPE_H__
#define __FMR_TYPE_H__

/******************************************************************************/
/* 関数宣言:API                                                               */
/******************************************************************************/
/**************************************/
/* ファイルシステム構築               */
/**************************************/
/* ファイルシステムの初期化 */
int FMR_Init(
    void
);

/* ファイルシステムのフォーマット */
int FMR_Format(
    TFMR_FORMATINFO * formatinfo        /* フォーマット情報領域               */
);

/* マウント */
int FMR_Mount(
	char drvnum,                        /* 論理ドライブ番号                   */
	char * raminfo,                     /* RAM情報アドレス                    */
	unsigned long ramlen,               /* RAM領域サイズ                      */
	TFMR_MOUNTINFO * mountinfo          /* マウント情報領域アドレス           */
);

/* アンマウント */
int FMR_Unmount(
	char drvnum                         /* 論理ドライブ番号                   */
);

/**************************************/
/* ファイル操作                       */
/**************************************/
/* ファイルのオープン */
int FMR_Open ( 
	const char * path,                  /* 対象ファイルパス名                 */
	int flag,                           /* オープンフラグ                     */
	unsigned char atrb                  /* 属性                               */
);

/* ファイルのクローズ */
int FMR_Close ( 
    int filehandle                      /* ファイルハンドル番号               */
);

/* ファイルへのデータ書込み */
int FMR_Write ( 
	int filehandle,                     /* ファイルハンドル                   */
	void * writebuff,                   /* 書込みデータ格納領域               */
	unsigned int size                   /* 書込みサイズ                       */
);

/* ファイルからのデータ読込み */
int FMR_Read ( 
	int filehandle,                     /* ファイルハンドル番号               */
	void * readbuff,                    /* 読込みデータ格納領域               */
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
	const char * path,                  /* パス名                             */
	unsigned char attr                  /* 属性                               */
);

/* ファイル/ディレクトリ名称変更 */
int FMR_Rename ( 
	const char * oldpath,               /* 旧パス名                           */
	const char * newpath                /* 新パス名                           */
);

/**************************************/
/* ディレクトリ操作                   */
/**************************************/
/* ディレクトリの作成 */
int FMR_Mkdir ( 
	const char * path                   /* 作成するディレクトリのパス名       */
);

/* ディレクトリの削除 */
int FMR_Rmdir ( 
	const char * path                   /* 削除するディレクトリのパス名       */
);

/**************************************/
/* その他                             */
/**************************************/
/* ディレクトリ/ファイルの検索開始 */
long FMR_FindFirst ( 
	const char * path,                  /* パス名                             */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
);

/* ディレクトリ/ファイルの検索 */
int FMR_FindNext ( 
	long srchhandle,                    /* 検索ハンドル                       */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
);

/* ディレクトリ/ファイル検索終了 */
int FMR_FindClose ( 
	long srchhandle                     /* 検索ハンドル                       */
);

/* ファイル状態を取得する */
int FMR_Stat ( 
	const char * path,                  /* パス名                             */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
);

/* ファイルシステム情報取得 */
int FMR_Info ( 
	F_CHAR drvnum,                      /* ドライブ番号                       */
	TFMR_DRIVE_INFO * drvinfo           /* ドライブ状態格納領域               */
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
/* 関数宣言:共通関数                                                          */
/******************************************************************************/
/**************************************/
/* エントリ操作系                     */
/**************************************/
/* ディレクトリエントリ.ファイル名設定&登録 */
F_INT FMR_setDirEntry(
    TFMR_SEARCHINFO * toppos,           /* 検索終了先頭位置                   */
    TFMR_SEARCHINFO * bottompos,        /* 検索終了最終位置                   */
	TFMR_DIRENTRY *   entry,            /* ディレクトリエントリ作成領域       */
	F_CHAR *          filename,         /* 設定ファイル名                     */
	F_CHAR *          shortname,        /* 設定ショートファイル名             */
    F_WORD            flag,             /* 動作フラグ                         */
    F_CHAR            drvnum            /* 対象ドライブ番号                   */
);

/* ロングファイル名のエントリの登録 */
F_INT FMR_setLFNEntry(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    TFMR_DIRENTRY * entry,              /* エントリイメージ                   */
    F_CHAR * lfname,                    /* ロングファイル名                   */
    F_BYTE sumvalue,                    /* サム値                             */
    TFMR_SEARCHINFO * topslot           /* 先頭位置                           */
);

/* 指定エントリ削除 */
F_INT FMR_deleteDirEntry(
	F_DWORD targetsect,                 /* 削除対象エントリ登録セクタ番号     */
	F_DWORD targetlfnsect,              /* 削除対象最終LFNエントリ登録セクタ番号 */
	F_BYTE  targetpos,                  /* 削除対象エントリ登録位置           */
	F_BYTE  targetlfnpos,               /* 削除対象最終LFNエントリ登録位置    */
    F_BYTE  prsflg,                     /* 動作フラグ                         */
    F_CHAR  drvnum                      /* 対象ドライブ番号                   */
);

/* ロングファイル名のエントリを削除 */
F_INT FMR_deleteLFNEntry(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    TFMR_SEARCHINFO * topslot,          /* 先頭位置                           */
    TFMR_SEARCHINFO * btmslot           /* 最終位置                           */
);

/* パス名よりファイル/ディレクトリ名を取得 */
F_INT FMR_getCurrentDirCluster(
    F_CHAR    drvnum,                   /* 論理ドライブ番号                   */
    F_CHAR *  pathname,                 /* パス名                             */
    F_CHAR *  getlongname,              /* ロングファイル名用ワークエリア     */
    F_BYTE *  prsflg,                   /* FMR_searchEntry用動作フラグ        */
    TFMR_SEARCHINFO * toppos,           /* 検索開始/終了先頭位置              */
    F_CHAR **  pathlen,                 /* パス名先頭からの最終要素の位置     */
    F_DWORD * clstnum                   /* カレントクラスタ番号               */
);

/* ショートファイル名からサム値を取得 */
F_BYTE FMR_getChkSum(
    F_CHAR * filename                   /* ショートファイル名                 */
);

/* エントリの検索 */
F_INT FMR_searchEntry(
    F_CHAR drvnum,                      /* 対象ドライブ番号                   */
    TFMR_SEARCHINFO * toppos,           /* 検索開始/終了先頭位置              */
    TFMR_SEARCHINFO * bottompos,        /* 検索終了最終位置                   */
    F_CHAR * targetname,                /* 検索文字列                         */
    TFMR_DIRENTRY * entry,              /* 取得エントリ                       */
    F_CHAR * getlfn,                    /* 取得ロングファイル名               */
    F_BYTE targetlen,                   /* 検索文字列長                       */
    F_BYTE prsflg                       /* 動作フラグ                         */
);

/* エントリ検索位置を1件進める */
F_INT FMR_skipSearchPosition(
    F_CHAR drvnum,                      /* 論理ドライブ番号                   */
    F_BYTE fatbuffnum,                  /* FAT読込み領域番号                  */
    TFMR_SEARCHINFO * searchinfo        /* 検索セクタ位置                     */
);

/* 指定されたエントリをBigEndian用に変換する */
F_VOID FMR_setBigEndianEntry(
    TFMR_DIRENTRY * entry               /* 設定エントリポインタ               */
);

/* ディレクトリエントリ格納クラスタを拡張する */
F_INT FMR_expandEntry(
    F_CHAR  drvnum,                     /* 対象ドライブ番号                   */
    F_DWORD cdirclsnum,                 /* カレントディレクトリのクラスタ番号 */
    TFMR_SEARCHINFO * toppos            /* エントリ検索結果:先頭位置          */
);

/* ファイル状態を設定する */
F_VOID FMR_setFileInfo(
	TFMR_DIRENTRY *  entry,             /* 取得エントリ                       */
	F_CHAR *         getname,           /* 取得ロングファイル名               */
	TFMR_FILE_INFO * fileinfo           /* ファイル状態格納領域               */
);

/* ディレクトリエントリ初期設定 */
F_VOID FMR_initDirEntry(
	TFMR_DIRENTRY * dirptr,             /* エントリ作成用ポインタ             */
    F_DWORD         clstnum,            /* クラスタ番号                       */
    F_BYTE          dotcnt,             /* ドットディレクティブ数             */
    F_BYTE          attr                /* エントリ属性                       */
);

/**************************************/
/* FAT操作系                          */
/**************************************/
/* 空きクラスタ番号を取得 */
F_DWORD FMR_getEmptyCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD clstnum,                    /* 検索開始クラスタ番号               */
    F_BYTE fatarea                      /* 検索FAT読込み領域番号              */
);

/* クラスタ番号、クラスタ内セクタ番号よりセクタ番号を求める */
F_DWORD FMR_getSectNum(
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_BYTE sectnum,                     /* クラスタ内セクタ番号               */
    F_BYTE drvnum                       /* ドライブ番号                       */
);

/* FATセクタ管理情報の構築 */
F_INT FMR_restoreFATInfo(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topsectofst                 /* 開始セクタオフセット               */
);

/* FATセクタ管理領域より空きクラスタを取得 */
F_INT FMR_searchEmptyCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD * emptyclustnum             /* 取得空きクラスタ番号               */
);

/* FATセクタ読込み */
F_INT FMR_getFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD * fatentry                  /* FATエントリ値                      */
);

/* FATセクタ読込み&ミラーリングFAT読込み */
F_INT FMR_readFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF     * fatbuff,         /* FAT読込領域                        */
    F_DWORD            sectnum,         /* 読込FATセクタ番号                  */
    F_WORD             fatbuffoffset    /* FAT読込領域オフセット              */
);

/* FATエントリの値取得/設定 */
F_INT FMR_controlFatEntry(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD * fatentry,                 /* FATエントリ値                      */
    F_BYTE ctrlflg                      /* Read/Write種別 OFF:取得,ON:設定    */
);

/* FATセクタ書込み */
F_INT FMR_setFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_BYTE  fatarea                     /* FAT読込み領域番号                  */
);

/* FATエントリ値取り出し */
F_DWORD FMR_getFatValue(
    F_BYTE * fatbuff,                   /* FATバッファアドレス                */
    F_BYTE fattype,                     /* FATタイプ                          */
    F_DWORD clstnum                     /* クラスタ番号                       */
);

/* FATエントリ値設定 */
F_VOID FMR_setFatValue(
    F_BYTE * fatbuff,                   /* FATバッファアドレス                */
    F_BYTE fattype,                     /* FATタイプ                          */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD fatentry                    /* FATエントリ値                      */
);

/* リンク対象クラスタに指定されたFATエントリ値を設定する */

F_INT FMR_linkNewCluster(
	F_DWORD   linkclstnum,              /* リンク対象クラスタ番号             */
	F_DWORD * fatentrynum,              /* FATエントリ設定値                  */
	F_BYTE  * fatareanum,               /* 現在参照中のFAT読込み領域番号      */
    F_CHAR    drvnum                    /* 対象ドライブ番号                   */
);

/**************************************/
/* データ操作:書込み関連              */
/**************************************/
/* データ追加書込み */
F_INT FMR_appendData(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_DWORD startsectnum,               /* 書込み開始セクタ番号               */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD datasize,                   /* 書込みサイズ                       */
    F_DWORD srchstartclstnum,           /* 検索開始クラスタ番号               */
    F_DWORD * newtopclstnum             /* 新規取得先頭クラスタ番号           */
);

/* データ更新書込み */
F_INT FMR_updateData(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD datasize,                   /* 書込みサイズ                       */
    F_DWORD srchstartclstnum,           /* 検索開始クラスタ番号               */
    F_DWORD fwdlnkclstnum,              /* 未更新最終クラスタ番号             */
    F_DWORD * newtopclstnum             /* 新規取得先頭クラスタ番号           */
);

/* クラスタ内セクタのコピー */
F_INT FMR_copySector(
    F_CHAR   drvnum,                    /* 対象論理ドライブ番号               */
    F_BYTE   copymode,                  /* コピーモード                       */
    F_WORD   writesize,                 /* 書込みサイズ                       */
    F_INT    handle,                    /* ファイルハンドル                   */
    F_DWORD  srcsectnum,                /* コピー元セクタ番号                 */
    F_DWORD  dstsectnum,                /* コピー先セクタ番号                 */
    F_BYTE * writeptr                   /* 書込み開始位置                     */
);

/* 既存セクタの更新 */
F_INT FMR_updateDataSector(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD srcsectnum,                 /* 更新元セクタ番号                   */
    F_DWORD targetsectnum,              /* 更新先セクタ番号                   */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD startpos,                   /* 書込み開始位置                     */
    F_DWORD datasize                    /* 書込みサイズ                       */
);

/* 新規セクタ書込み処理 */
F_INT FMR_writeDataSectors(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topsectnum,                 /* 先頭セクタ番号                     */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD targetsectnum,              /* 書込みセクタ数                     */
    F_DWORD startclstnum,               /* 検索開始クラスタ番号               */
    F_BYTE  fatarea,                    /* FAT読込み領域番号                  */
    F_DWORD * newtopclstnum,            /* 新規取得先頭クラスタ番号           */
    F_DWORD * newbottomclstnum,         /* 新規取得最終クラスタ番号           */
    F_BYTE * lastfatarea                /* 最終FAT読込み領域番号              */
);

/* 指定R/Wバッファから指定セクタ番号へ１セクタ書込む */
F_INT FMR_setDataSector(
    F_CHAR  drvnum,                     /* 対象ドライブ番号                   */
    F_DWORD sectnum,                    /* 書込みセクタ番号                   */
    F_BYTE  buffsw                      /* R/Wバッファスイッチ                */
);

/* 書込みデータ後部端数部を書き込む */
F_INT FMR_writeEndData(
    F_DWORD * writesectnum,             /* 書込み開始セクタ番号               */
	F_DWORD * newbtmclstnum,            /* 新規取得最終クラスタ番号           */
	F_DWORD * newtopclstnum,            /* 新規取得先頭クラスタ番号           */
	F_BYTE  * fatareanum,               /* 現在参照中のFAT読込み領域番号      */
    F_CHAR    drvnum                    /* 対象ドライブ番号                   */
);

/**************************************/
/* データ操作:書込み関連以外          */
/**************************************/
/* データ部クラスタの削除 */
F_INT FMR_deleteDataCluster(
    F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
    F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
    F_DWORD endclstnum                  /* 終了クラスタ番号                   */
);

/* ファイルポインタの移動 */
F_INT FMR_setPointer(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_LONG offset,                      /* オフセット                         */
    F_INT origin                        /* 初期位置                           */
);

/* 指定R/Wバッファへ指定セクタ番号から１セクタ読込む */
F_INT FMR_readDataSector(
    F_CHAR   drvnum,                    /* 対象ドライブ番号                   */
    F_DWORD  sectnum,                   /* 読込みセクタ番号                   */
    F_BYTE   buffsw                     /* R/Wバッファスイッチ                */
);

/**************************************/
/* RAM領域操作                        */
/**************************************/
/* ファイルハンドルの初期化 */
F_VOID FMR_initFileHandle(
    F_INT handle                        /* ファイルハンドル番号               */
);

/* 検索ハンドルの初期化 */
F_VOID FMR_initSearchHandle(
    F_INT handle                        /* 検索ハンドル番号                   */
);

/* バッファのコピー */
F_VOID FMR_memCopy(
    F_VOID * deststr,                   /* コピー先バッファアドレス           */
    F_VOID * srcstr,                    /* コピー元バッファアドレス           */
    F_DWORD size                        /* コピーサイズ                       */
);

/* バッファの初期化 */
F_VOID FMR_memSet(
    F_VOID * buff,                      /* バッファアドレス                   */
    F_BYTE pattern,                     /* 初期化データ                       */
    F_DWORD size                        /* バッファサイズ                     */
);

/**************************************/
/* 文字列操作系                       */
/**************************************/
/* パス名よりファイル/ディレクトリ名を取得 */
F_SHORT FMR_splitPath(
    F_CHAR * pathname,                  /* パス名                             */
    F_CHAR * delimiter,                 /* 区切り記号                         */
    F_BYTE * prsflg                     /* FMR_searchEntry用動作フラグ        */
);

/* ロングファイル名よりショートファイル名を取得 */
F_INT FMR_getShortName(
	F_CHAR drvnum,                      /* 対象論理ドライブ番号               */
	F_CHAR * targetname,                /* 変換前文字列                       */
	F_WORD targetlen,                   /* 変換前文字列長                     */
	F_BYTE targetentry,                 /* 対象エントリ種別                   */
	F_CHAR * outstring,                 /* 変換後文字列                       */
	F_DWORD cclstnum                    /* カレントクラスタ番号               */
);

/* ASCII/Shift-JIS → Unicodeの変換 */
F_VOID FMR_exchgSJISToUnicode(
	F_CHAR * srcstr,                    /* 変換前文字列                       */
	F_WORD * buff,                      /* 変換後文字列                       */
	F_WORD num                          /* 変換文字数                         */
);

/* Unicode → ASCII/Shift-JISの変換 */
F_VOID FMR_exchgUnicodeToSJIS(
	F_WORD * srcstr,                    /* 変換前文字列                       */
	F_CHAR * buff,                      /* 変換後文字列                       */
	F_WORD num                          /* 変換文字数                         */
);

/* 文字列の比較 */
F_INT FMR_compare(
    F_CHAR * srcstr,                    /* 比較元文字列                       */
    F_CHAR * dststr,                    /* 比較先文字列                       */
    F_BYTE srclen,                      /* 比較元文字列数                     */
    F_BYTE dstlen,                      /* 比較先文字列数                     */
    F_BYTE wcflg                        /* ワイルドカードフラグ               */
);

/* 小文字を大文字に変換 */
F_VOID FMR_smallToCapital(
	F_CHAR * srcstr,                    /* 変換前文字列                       */
	F_CHAR * buff,                      /* 変換後文字列                       */
	F_WORD num                          /* 変換文字数                         */
);

/**************************************/
/* APIサブルーチン                    */
/**************************************/
/* Mkdir */
F_INT FMR_subMkdir ( 
    F_CHAR * path,                      /* パス名                             */
    F_CHAR   drvnum                     /* 論理ドライブ番号                   */
);

/* Open */
F_INT FMR_subOpen ( 
	F_CHAR * path,                      /* 対象ファイルパス名                 */
	F_INT    flag,                      /* オープンフラグ                     */
	F_BYTE   atrb,                      /* 属性                               */
    F_CHAR   drvnum                     /* 論理ドライブ番号                   */
);

/* Write */
F_INT FMR_subWrite (
	F_INT filehandle,                   /* ファイルハンドル                   */
	F_VOID * writebuff,                 /* 書込みデータ格納領域               */
	F_UINT size                         /* 書込みサイズ                       */
);

/******************************************************************************/
/* 関数宣言:OS部                                                              */
/******************************************************************************/
/* 日付時刻を取得 */
F_VOID FMR_osGetTime(
    F_WORD * setday,                    /* 取得年月日設定領域                 */
    F_WORD * settime,                   /* 取得時分秒設定領域                 */
    F_BYTE * setmtime                   /* 取得ミリ秒設定領域                 */
);

/* セマフォ資源を獲得 */
F_INT FMR_osWaiSem(
    F_WORD semid                        /* セマフォID                         */
);

/* セマフォ資源を返却 */
F_INT FMR_osSigSem(
    F_WORD semid                        /* セマフォID                         */
);

/* セマフォ資源を生成 */
#ifdef DFMR_SEMID_AUTO
F_INT FMR_osAcreSem(                    /* セマフォID自動生成用               */
);
#else /* DFMR_SEMID_AUTO */
F_INT FMR_osAcreSem(                    /* セマフォID指定用                   */
    F_WORD semid                        /* 指定セマフォID                     */
);
#endif /* DFMR_SEMID_AUTO */

/******************************************************************************/
/* グローバル変数                                                             */
/******************************************************************************/
extern TFMR_PART_RAMADDR tfmr_part_ramaddr[ DFMR_USR_MAXDRVNUM ];
                                        /* 論理ドライブRAMテーブルアドレス    */
extern TFMR_FILEHANDLE tfmr_filehandle[ DFMR_USR_MAXFILEHANDLE ];
                                        /* ファイルハンドル                   */
extern TFMR_SEARCHHANDLE tfmr_searchhandle[ DFMR_USR_MAXSEARCHHANDLE ];
                                        /* 検索ハンドル                       */
extern TFMR_DRVINFO tfmr_drvinfo[ DFMR_USR_MAXDRIVER ];
                                        /* ドライバ構成情報                   */
#endif /* __FMR_TYPE_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
