/*======================================================================
 * File name    : utility.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/rcs/utility.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: utility.h $
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
/* 関 数 名：FMR_Utility.h                                                    */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：ユーティリティ用各種定義値                                       */
/* 作 成 日：2004.09.13                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#ifndef __FMR_UTILITY_H__
#define __FMR_UTILITY_H__

#define DFMR_SD_BITONCHAR 3             /* Bitが存在するByteオフセット計算用  */
#define DFMR_SD_BITPOS 0x00000007       /* 対象Bit位置取得用マスク            */
#define DFMR_SD_TOPCLSTBITOFF 0xFC      /* クラスタ番号0,1のBit OFF           */

#define DFMR_SDBITALLON 0xFF            /* クラスタ使用状況領域初期値         */

#define DFMR_BITSIZE 8                  /* 1Byte当りのBit数                   */

#define DFMR_SD_CHKTBDCLST 1            /* BADクラスタチェック:未使用         */
#define DFMR_SD_CHKBADCLST 2            /* BADクラスタチェック:BADクラスタ    */

#define DFMR_SD_LOGMODEON   1           /* ロギング動作ON                     */
#define DFMR_SD_LOGFULLPATH 2           /* フルパス指定                       */
#define DFMR_SD_LOGWRTFILE  4           /* ファイルに出力                     */

#define DFMR_SD_LOGERRTAILOFF 1         /* リンク切れエラー                   */
#define DFMR_SD_LOGERRMULTI   2         /* 多重リンクエラー                   */
#define DFMR_SD_LOGERRDIV     3         /* BADクラスタ有り                    */
#define DFMR_SD_LOGERRMSIZE   4         /* サイズエラー                       */
#define DFMR_SD_LOGERRTOPBAD  5         /* 先頭クラスタがBADクラスタ          */
                                        /* (ディレクトリのみ)                 */
#define DFMR_SD_LOGERRTOPILL  6         /* 不正なディレクトリ                 */
                                        /* (先頭に｢.｣｢..｣が存在しない)        */
#define DFMR_SD_LOGERRLFNILL  7         /* 不正なLFNエントリ                  */
                                        /* (順序/サム値が不正)                */
#define DFMR_SD_LOGERRNOTOP   8         /* 先頭クラスタ不正                   */
                                        /* (未使用or2重リンク)                */
#define DFMR_SD_LOGERRRELINK  9         /* 再帰リンク                         */

#define DFMR_SD_LOGCNTINIT 0            /* ログカウンタ初期値                 */
#define DFMR_SD_RECURRENTMAX 2          /* 最大リンク許容数                   */
                                        /* (サイズより求めたクラスタ数の何倍  */
                                        /* までを許容するかを表す)            */

#define DFMR_SDDRVLEN 3                 /* パスのドライブ名部分の長さ         */
#define DFMR_SDDIRNAME "FOUND."         /* 断片ファイルディレクトリ名称       */
#define DFMR_SDDIRNAMELEN 9             /* 断片ファイルディレクトリ名称長     */
#define DFMR_SDDIRNAMECHARLEN 6         /* 同上文字列部分長                   */
#define DFMR_SDDIRMAX 1000              /* 断片ファイルディレクトリ最大数     */
#define DFMR_SDFILENAME "FILE0000.CHK"  /* 断片ファイル名称                   */
#define DFMR_SDFILEMAX 10000            /* 断片ファイル最大数                 */
#define DFMR_SDLOGNAME "REPORT.LOG"     /* ログファイル名称                   */
#define DFMR_SDLOGNAMELEN 10            /* ログファイル名称長                 */

/* エラーコード */
#define DFMR_SDILLEGALDIR  ( -300 )     /* 不正なディレクトリ                 */
                                        /* (先頭に｢.｣｢..｣が存在しない)        */
#define DFMR_SDILLEGALROOT ( -301 )     /* ルートディレクトリに読込み不能の   */
                                        /* セクタが存在する(FAT12/16のみ)     */
#define DFMR_SDROOTENTFULL ( -302 )     /* 断片ファイル作成不可(ルート直下に  */
                                        /* 空きエントリ位置が存在しない)      */
#define DFMR_SDSCRAPERR    ( -303 )     /* 断片ファイル作成時エラー発生       */
#define DFMR_SDLOGOUTERR   ( -304 )     /* ログファイル出力エラー             */
#define DFMR_SDSBROKENFILE ( -305 )     /* 対象ファイルが破損ファイル(内部用) */
#define DFMR_SDALLERRDIR   ( -306 )     /* データ部が全て破損している         */
                                        /* ディレクトリ(内部用)               */

typedef struct tagTFMR_SDLOGTBLSHT{     /* ログ構造体(フルパス非指定)         */
	F_CHAR pcename[ 11 ];               /* 断片ファイル名称                   */
	F_CHAR errcode;                     /* エラーコード                       */
	F_DWORD orgtopclstnum;              /* 元ファイルの先頭クラスタ番号       */
	F_WORD orgclstnum;                  /* 破損ファイルのクラスタ数           */
	F_CHAR orgname[ DFMR_FNAMESZ + DFMR_EXTSZ + 1 ];
	                                    /* 元ファイル名                       */
	                                    /* (ファイル+拡張子+'.')              */
	F_CHAR reserved[ 2 ];               /* 予約領域                           */
}TFMR_SDLOGTBLSHT;

typedef struct tagTFMR_SDLOGTBLLNG{     /* ログ構造体(フルパス指定)           */
	F_CHAR pcename[ 11 ];               /* 断片ファイル名称                   */
	F_CHAR errcode;                     /* エラーコード                       */
	F_DWORD orgtopclstnum;              /* 元ファイルの先頭クラスタ番号       */
	F_WORD orgclstnum;                  /* 破損ファイルのクラスタ数           */
	F_CHAR orgname[ DFMR_MAXFILESZ ];   /* 元ファイル名(フルパス)             */
	F_CHAR reserved[ 2 ];               /* 予約領域                           */
}TFMR_SDLOGTBLLNG;

typedef union tagTFMR_WORDWORK{         /* WORDサイズワークエリア             */
	F_BYTE bwork[ 2 ];
	F_WORD wwork;
}TFMR_WORDWORK;

typedef union tagTFMR_DWORDWORK{        /* DWORDサイズワークエリア            */
	F_BYTE bwork[ 4 ];
	F_WORD work[ 2 ];
	F_DWORD dwork;
	F_INT iwork;
}TFMR_DWORDWORK;

typedef struct tagTFMR_SDWORKAREA{      /* スキャンディスク用ワークエリア     */
	F_WORD logcnt;                      /* ログカウンタ                       */
	F_WORD logmax;                      /* ログ最大件数                       */
	F_BYTE mode;                        /* ロギング用動作フラグ               */
	                                    /* Bit0：ロギング動作のON/OFF指定     */
	                                    /*   ON ：ロギングを行う              */
	                                    /*   OFF：ロギングは行わない          */
	                                    /* Bit1：元ファイル名の設定方法       */
	                                    /*   ON ：元ファイルをフルパスで設定  */
	                                    /*   OFF：元ファイルのファイル名のみ  */
	                                    /* Bit2：ロギング結果の出力方法指定   */
	                                    /*   ON ：ScanDisk後にファイルに出力  */
	                                    /*   OFF：ファイルへの出力は行わない  */
	F_BYTE bwork;                       /* BYTEサイズワークエリア             */
	TFMR_WORDWORK wwork;                /* WORDサイズワークエリア             */
	TFMR_DWORDWORK dwork01;             /* DWORDサイズワークエリア            */
	TFMR_DWORDWORK dwork02;             /* DWORDサイズワークエリア            */
}TFMR_SDWORKAREA;

typedef union tagTFMR_ENTRY{            /* エントリ共用体                     */
	TFMR_DIRENTRY dir;                  /* ディレクトリエントリ               */
	TFMR_LFNENTRY lfn;                  /* LFNエントリ                        */
}TFMR_ENTRY;

/* Common */
F_BYTE FMR_framBitChk(                  /* クラスタ使用状況のBitの確認を行う  */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
);

F_VOID FMR_framBitOff(                  /* クラスタ使用状況のBitをOFFにする   */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
);

F_INT FMR_framChainClstBitOff(          /* チェインされたクラスタの使用状況を */
							            /* OFFにする                          */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD eoc,                        /* EOCコード                          */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
);

F_INT FMR_pickTbdClst(                  /* 未使用クラスタを摘出する           */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
);

F_INT FMR_chkBadClst(                   /* BADクラスタのチェックを行う        */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_BYTE buffnum,                     /* R/Wバッファ番号                    */
	F_BYTE dataflg                      /* 対象クラスタ判別フラグ             */
);

F_INT FMR_scanTbdClst(                  /* 未使用クラスタのBADクラスタ        */
                                        /* チェックを行う                     */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_BYTE * ramaddr                    /* クラスタ使用状況展開領域           */
);

F_INT FMR_scanFileData(                 /* ファイルデータのリンク切れ         */
	                                    /* /BADクラスタチェックを行う         */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD size,                       /* ファイルサイズ                     */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
);

F_INT FMR_scanEntry(                    /* エントリの検索を行う               */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	F_DWORD * topclstnum,               /* 先頭クラスタ番号                   */
	F_WORD * entrypathlen,              /* エントリ名設定先頭位置             */
	F_CHAR drvnum                       /* ドライブ番号                       */
);

F_INT FMR_scanEntryUnderRoot(           /* エントリの検索を行う               */
							            /* (ルートディレクトリ直下)           */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
);

F_INT FMR_deleteErrLFNEntry(            /* 対象の不正LFNエントリを削除する    */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	TFMR_SEARCHINFO * toplfnpos,        /* 削除対象先頭エントリ位置           */
	TFMR_SEARCHINFO * btmlfnpos,        /* 削除対象最終エントリ位置           */
	TFMR_SEARCHINFO * nowlfnpos         /* 現在アクセス中のエントリ位置       */
);

F_INT FMR_getFatSectorRetry(            /* FATセクタを読込む                  */
	TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
	TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
	F_DWORD clstnum,                    /* クラスタ番号                       */
	F_DWORD * fatentry                  /* FATエントリ値                      */
);

F_INT FMR_setFatSectorRetry(            /* FATセクタを書込む                  */
	F_CHAR drvnum,                      /* ドライブ番号                       */
    F_BYTE fatarea                      /* FAT読込み領域番号                  */
);

F_INT FMR_readSectRetry(                /* 対象セクタを読込む                 */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum,                    /* セクタ番号                         */
	F_BYTE buffnum                      /* R/Wバッファ番号                    */
);

F_INT FMR_writeSectRetry(               /* 対象セクタを書込む                 */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum,                    /* セクタ番号                         */
	F_BYTE buffnum                      /* R/Wバッファ番号                    */
);

F_VOID FMR_recErrLog(                   /* ロギングを行う                     */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 断片ファイルの先頭クラスタ番号     */
	F_WORD clstcnt,                     /* 断片ファイルのクラスタ数           */
	F_CHAR errcode,                     /* エラー種別                         */
	F_CHAR * orgpath,                   /* 元ファイル名                       */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	TFMR_SDWORKAREA * sdworkarea        /* スキャンディスク用ワークエリア     */
);

F_VOID FMR_recLFNErrLog(                /* 不正LFNのロギングを行う            */
	TFMR_LFNENTRY * l_entry,            /* 最終LFNエントリ                    */
	F_CHAR * entrypath,                 /* エントリ名格納領域                 */
	TFMR_SDWORKAREA * sdworkarea,       /* クラスタ使用状況展開領域           */
	F_BYTE * logarea,                   /* ログ出力領域                       */
	F_WORD * entrypathlen               /* エントリ名設定先頭位置             */
);

F_WORD FMR_getDirName(                  /* エントリの名称部より               */
	                                    /* ショートファイル名を取得する       */
	F_CHAR * entname,                   /* エントリの名称部                   */
	F_CHAR * pathname                   /* 名称を設定する領域                 */
);

F_DWORD FMR_getLastSectNum(             /* セクタが属するクラスタ内の         */
	                                    /* 最終セクタ番号を取得する           */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum                     /* セクタ番号                         */
);

F_DWORD FMR_getFileSize(                /* 対象のエントリよりファイルサイズを */
	                                    /* 取得する                           */
	TFMR_DIRENTRY * l_entry             /* ディレクトリエントリ               */
);

F_VOID FMR_setFileSize(                 /* 対象のエントリのファイルサイズを   */
					                    /* 設定する                           */
	TFMR_DIRENTRY * l_entry,            /* ディレクトリエントリ               */
	F_DWORD l_size                      /* ファイルサイズ                     */
);

F_DWORD FMR_getTopClstNum(              /* 対象のエントリより先頭クラスタ     */
	                                    /* 番号を取得する                     */
	TFMR_DIRENTRY * l_entry             /* ディレクトリエントリ               */
);

F_VOID FMR_setTopClstNum(               /* 対象のエントリの先頭クラスタ番号を */
	                                    /* 設定する                           */
	TFMR_DIRENTRY * l_entry,            /* ディレクトリエントリ               */
	F_DWORD topclstnum                  /* 先頭クラスタ番号                   */
);

F_VOID FMR_makeEntry(                   /* ディレクトリエントリのイメージを   */
					                    /* 作成する                           */
	TFMR_DIRENTRY * entry,              /* エントリイメージ作成領域           */
	F_CHAR * name,                      /* ファイル名称                       */
	F_BYTE attr,                        /* 属性                               */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_DWORD size                        /* ファイルサイズ                     */
);

F_INT FMR_makeScrapFile(                /* 断片ファイルを作成する             */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD topclstnum,                 /* 先頭クラスタ番号                   */
	F_BYTE * ramaddr,                   /* クラスタ使用状況展開領域           */
	F_BYTE * logarea                    /* ログ出力領域                       */
);

F_DWORD FMR_getClstNum(                 /* セクタ番号よりクラスタ番号を       */
					                    /* 取得する                           */
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum                     /* セクタ番号                         */
);

/* OS */
F_VOID FMR_osRetryWait(                 /* リトライ用ウェイトを行う           */
);
#endif /* __FMR_UTILITY_H__ */
