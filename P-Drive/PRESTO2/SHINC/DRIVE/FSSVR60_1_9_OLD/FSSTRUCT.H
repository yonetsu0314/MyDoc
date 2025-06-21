/*======================================================================
 * File name    : fsstruct.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/rcs/fsstruct.h 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsstruct.h $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2006/03/22 15:00
 * Saver60 Ver.01.09 up
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/16 18:00
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
/* 関 数 名：FMR_Struct.h                                                     */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：構造体定義                                                       */
/*         ：                                                                 */
/* 作 成 日：2004.04.15                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 R/Wバッファ領域追加                             04/12/10 Y.Yoshizumi */
/*       RAMテーブルアドレス.R/Wバッファ名変更           04/12/10 Y.Yoshizumi */
/* 01.07 データ型修正                                    05/12/12 Y.Yoshizumi */
/******************************************************************************/

#ifndef __FMR_STRUCT_H__
#define __FMR_STRUCT_H__

/*------------------------------------*/
/*--- FAT論理構造関連 ----------------*/
/*------------------------------------*/
/* パーティションテーブル */
typedef struct tagTFMR_PARTITIONTABLE{
    F_BYTE activeflg;                   /* アクティブフラグ                   */
    F_BYTE chstopsect[ 3 ];             /* 開始セクタ(CHS)                    */
    F_BYTE partitiontype;               /* パーティションタイプ               */
    F_BYTE chsbottomsect[ 3 ];          /* 終了セクタ(CHS)                    */
    F_DWORD lbatopsect;                 /* 開始セクタ(LBA)                    */
    F_DWORD totalsectnum;               /* パーティション内の総セクタ数       */
}TFMR_PARTITIONTABLE;

/* マスターブートレコード */
typedef struct tagTFMR_MASTERBOOTRECORD{
    F_BYTE bootstraploader[ DFMR_MBR_BSLSIZE ];
                                        /* ブートストラップローダ             */
    F_BYTE partitiontbl[ sizeof( TFMR_PARTITIONTABLE ) * DFMR_PRTTBL_PARTMAX ];
                                        /* パーティションテーブル             */
    F_BYTE signature[ 2 ];              /* ブートシグネチャ                   */
}TFMR_MASTERBOOTRECORD;

/* ディレクトリエントリ */
typedef struct tagTFMR_DIRENTRY{
    F_BYTE name[ DFMR_FNAMESZ ];        /* ファイル名                         */
    F_BYTE extension[ DFMR_EXTSZ ];     /* 拡張子                             */
    F_BYTE attr;                        /* 属性                               */
    F_BYTE ntres;                       /* 予約領域                           */
    F_BYTE crttimetenth;                /* 作成時刻(msec)                     */
    F_WORD crttime;                     /* 作成時刻                           */
    F_WORD crtdate;                     /* 作成日付                           */
    F_WORD lstaccdate;                  /* アクセス日                         */
    F_WORD fstclushi;                   /* 開始クラスタ(上位)                 */
    F_WORD wrttime;                     /* 更新時刻                           */
    F_WORD wrtdate;                     /* 更新日付                           */
    F_WORD fstcluslo;                   /* 開始クラスタ(下位)                 */
    F_DWORD filesize;                   /* サイズ                             */
}TFMR_DIRENTRY;

/* LFNエントリ */
typedef struct tagTFMR_LFNENTRY{
    F_BYTE ord;                         /* 順序                               */
    F_BYTE name1[ DFMR_LFN_FNAME1SZ * sizeof( F_WORD ) ];
                                        /* LFNのファイル名サイズ(0～4文字)    */
    F_BYTE attr;                        /* 属性                               */
    F_BYTE type;                        /* 予約領域                           */
    F_BYTE chksum;                      /* サム値                             */
    F_BYTE name2[ DFMR_LFN_FNAME2SZ * sizeof( F_WORD ) ];
                                        /* LFNのファイル名サイズ(5～10文字)   */
    F_WORD fstcluslo;                   /* 開始クラスタ                       */
    F_BYTE name3[ DFMR_LFN_FNAME3SZ * sizeof( F_WORD ) ];
                                        /*LFNのファイル名サイズ(11～12文字)   */
}TFMR_LFNENTRY;
/*------------------------------------*/
/*--- ドライバI/F --------------------*/
/*------------------------------------*/
/* 初期化I/O */
typedef struct tagTFMR_DRVIOINIT{
    F_BYTE reserved1[ 4 ];              /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD reserved3;                  /* 予約領域                           */
    F_DWORD reserved4;                  /* 予約領域                           */
}TFMR_DRVIOINIT;

/* マウントI/O */
typedef struct tagTFMR_DRVIOMOUNT{
    F_BYTE mbrmode;                     /* MBRモード                          */
    F_BYTE parttype;                    /* パーティションタイプ               */
    F_BYTE sectsize;                    /* セクタサイズ                       */
    F_BYTE reserved1;                   /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD totalsectcnt;               /* ユニット全体のセクタ数             */
    F_DWORD sectnum;                    /* ユニット開始セクタ番号             */
}TFMR_DRVIOMOUNT;

/* アンマウントI/O */
typedef struct tagTFMR_DRVIOUNMOUNT{
    F_BYTE reserved1[ 4 ];              /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD reserved3;                  /* 予約領域                           */
    F_DWORD reserved4;                  /* 予約領域                           */
}TFMR_DRVIOUNMOUNT;

/* チャネル情報取得I/O */
typedef struct tagTFMR_DRVIOCHNLINFO{
    F_BYTE reserved1[ 4 ];              /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD reserved3;                  /* 予約領域                           */
    F_DWORD reserved4;                  /* 予約領域                           */
}TFMR_DRVIOCHNLINFO;

/* デバイス情報取得I/O */
typedef struct tagTFMR_DRVIODEVINFO{
    F_WORD sctpertrack;                 /* 1トラック当りのセクタ数            */
    F_WORD cylindernum;                 /* シリンダ数                         */
    F_WORD headnum;                     /* ヘッド数                           */
    F_WORD reserved1;                   /* 予約領域                           */
    F_DWORD totalsct;                   /* 合計セクタ数                       */
    F_DWORD reserved2;                  /* 予約領域                           */
}TFMR_DRVIODEVINFO;

/* ユニット情報取得I/O */
typedef struct tagTFMR_DRVIOUNITINFO{
    F_BYTE parttype;                    /* パーティションタイプ               */
    F_BYTE reserved1[ 3 ];              /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD sizesctnum;                 /* ユニット全体のセクタ数             */
    F_DWORD reserved3;                  /* 予約領域                           */
}TFMR_DRVIOUNITINFO;

/* データ読込みI/O */
typedef struct tagTFMR_DEVIOREAD{
    F_DWORD startsctnum;                /* 読込み開始セクタ番号               */
    F_WORD * readsctnum;                /* 読込みセクタ数                     */
    F_BYTE * readbuff;                  /* 読込みデータ格納領域               */
    F_DWORD reserved;                   /* 予約領域                           */
}TFMR_DEVIOREAD;

/* データ書込みI/O */
typedef struct tagTFMR_DEVIOWRITE{
    F_DWORD startsctnum;                /* 書込み開始セクタ番号               */
    F_WORD * writesctnum;               /* 書込みセクタ数                     */
    F_BYTE * writebuff;                 /* 書込みデータ格納領域               */
    F_DWORD reserved;                   /* 予約領域                           */
}TFMR_DEVIOWRITE;

/* データ消去I/O */
typedef struct tagTFMR_DEVIOERASE{
    F_DWORD startsctnum;                /* 消去開始セクタ番号                 */
    F_WORD * erasesctnum;               /* 消去セクタ数                       */
    F_BYTE erasedata;                   /* 消去データ                         */
    F_BYTE reserved1[ 3 ];              /* 予約領域                           */
    F_DWORD reserved2;                  /* 予約領域                           */
}TFMR_DEVIOERASE;

/* メディア挿抜チェックI/O */
typedef struct tagTFMR_DEVIOCHKINSERT{  /*                               */
    F_BYTE state;                       /* 挿抜状態                           */
    F_BYTE reserved1[ 3 ];              /* 予約領域                           */
    F_WORD reserved2[ 2 ];              /* 予約領域                           */
    F_DWORD reserted3;                  /* 予約領域                           */
    F_DWORD reserved4;                  /* 予約領域                           */
}TFMR_DEVIOCHKINSERT;                   /*                               */

/* ドライバI/O構造体群 */
typedef union tagTFMR_DRVIOPARAM{
    TFMR_DRVIOINIT init;                /* 初期化I/O                          */
    TFMR_DRVIOMOUNT mount;              /* マウントI/O                        */
    TFMR_DRVIOUNMOUNT unmount;          /* アンマウントI/O                    */
    TFMR_DRVIOCHNLINFO chnlinfo;        /* チャネル情報取得I/O                */
    TFMR_DRVIODEVINFO devinfo;          /* デバイス情報取得I/O                */
    TFMR_DRVIOUNITINFO unitinfo;        /* ユニット情報取得I/O                */
    TFMR_DEVIOREAD read;                /* 読込みI/O                          */
    TFMR_DEVIOWRITE write;              /* 書込みI/O                          */
    TFMR_DEVIOERASE erase;              /* 消去I/O                            */
    TFMR_DEVIOCHKINSERT chkinsert;      /* メディア挿抜チェックI/O            */
}TFMR_DRVIOPARAM;

/*------------------------------------*/
/*--- ファイルシステム管理/制御関連 --*/
/*------------------------------------*/
/* 論理ドライブ構成情報 */
typedef struct tagTFMR_LOGDRV_INFO{
    F_CHAR drvnum;                      /* 論理ドライブ番号                   */
    F_BYTE drvid;                       /* ドライバID                         */
    F_BYTE channelunitnum;              /* チャネル番号/ユニット番号          */
    F_BYTE sectperclst;                 /* クラスタ当りのセクタ数             */
    F_BYTE fattype;                     /* FATタイプ                          */
    F_BYTE fatcnt;                      /* FAT数                              */
    F_WORD bytepersect;                 /* セクタ当りのバイト数               */
    F_WORD direntcnt;                   /* ディレクトリエントリ数             */
    F_WORD semid;                       /* セマフォID                         */
    F_WORD actfatflg;                   /* アクティブFATフラグ                */
    F_WORD fsinfosect;                  /* FSInfoセクタ番号                   */
    F_INT ( * drventry )( F_BYTE, F_BYTE, F_VOID * );
                                        /* ドライバエントリアドレス           */
    F_DWORD partsectnum;                /* ユニット全体のセクタ数             */
    F_DWORD sectperfat;                 /* FAT1件当りのセクタ数               */
    F_DWORD fattopsect;                 /* FAT Regionの先頭セクタ番号         */
    F_DWORD roottopsect;                /* ルートディレクトリの先頭セクタ番号 */
    F_DWORD datatopsect;                /* Data Regionの先頭セクタ番号        */
    F_DWORD totalclstnum;               /* 総クラスタ数                       */
    F_DWORD volumeid;                   /* ボリュームID                       */
    F_BYTE volumename[ 11 ];            /* ボリューム名                       */
    F_BYTE reserved;                    /* 予備                               */
}TFMR_LOGDRV_INFO;

/* FAT読込み領域 */
typedef struct tagTFMR_FATBUFF{
    F_DWORD fatsectnum;                 /* FATセクタ番号                      */
    F_BYTE fat[ DFMR_USR_MAXSECTSIZE + 4];
                                        /* セクタ単位のFAT                    */
}TFMR_FATBUFF;

/* ワークエリア */
typedef struct tagTFMR_WORK_AREA{
    F_WORD reserved1;                   /* 予約領域                           */
    F_WORD reserved2;                   /* 予約領域                           */
}TFMR_WORK_AREA;

/* FATセクタ管理領域 */
typedef struct tagSECTADMINTBL{         /* セクタ管理テーブル                 */
    F_DWORD freeclstnum;                /* 空きクラスタ数                     */
    F_DWORD leastfreeclst;              /* セクタ内最小空きクラスタ番号       */
}SECTADMINTBL;

typedef struct tagTFMR_FATSECT_AREA{
    F_DWORD startfatsect;               /* 開始FATセクタ番号                  */
    SECTADMINTBL sectadmintbl[ DFMR_FATMNGSIZE ];
                                        /* セクタ管理テーブル                 */
}TFMR_FATSECT_AREA;

/* R/Wバッファ領域 */
typedef struct tagTFMR_RWBUFF{
    F_DWORD rwsectnum;                  /* R/Wセクタ番号                      */
    F_BYTE rwbuff[ DFMR_USR_MAXSECTSIZE ];
                                        /* セクタ単位のバッファ               */
}TFMR_RWBUFF;

/* ドライバ構成情報 */
typedef struct tagTFMR_DRVINFO{
    F_BYTE drvid;                       /* ドライバID                         */
    F_BYTE channelnum;                  /* チャネル数                         */
    F_BYTE mediatype;                   /* メディアタイプ                     */
    F_BYTE reserved;                    /* 予約領域                           */
    F_INT ( * drventry )( F_BYTE, F_BYTE, F_VOID * );
                                        /* ドライバエントリアドレス           */
    F_WORD semaphoreid[ DFMR_USR_MAXCHANNEL ];
                                        /* セマフォID                        */
}TFMR_DRVINFO;


/* 論理ドライブRAMテーブルアドレス */
typedef struct tagTFMR_PART_RAMADDR{
    TFMR_LOGDRV_INFO * logdrvinfo;      /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff;             /* FAT読込み領域                      */
    TFMR_WORK_AREA * workarea;          /* ワークエリア                       */
    TFMR_FATSECT_AREA * fatsectarea;    /* FATセクタ管理領域                  */
    TFMR_RWBUFF * rwbuffarea;           /* R/Wバッファ領域                    */
}TFMR_PART_RAMADDR;

/* ファイルハンドル */
typedef struct tagTFMR_FILEHANDLE{
    TFMR_DIRENTRY direntry;             /* エントリイメージ                   */
    F_DWORD entdirsectnum;              /* エントリが登録されている           */
                                        /* セクタ番号                         */
    F_DWORD fptr;                       /* ファイルポインタ                   */
    F_DWORD crtsectnum;                 /* カレントセクタ番号                 */
    F_WORD crtsectofst;                 /* カレントセクタ内ポインタオフセット */
    F_WORD acsmode;                     /* ファイルアクセスモード             */
    F_BYTE entdirsectpos;               /* エントリが登録されている           */
                                        /* セクタ内位置                       */
    F_CHAR drivenum;                    /* 論理ドライブ番号                   */
    F_BYTE driveindex;                  /* 予約領域                           */
}TFMR_FILEHANDLE;

/* 検索ハンドル */
typedef struct tagTFMR_SEARCHHANDLE{
    F_DWORD sectornum;                  /* 検索開始セクタ番号                 */
    F_BYTE position;                    /* 検索開始セクタ内位置               */
    F_CHAR drivenum;                    /* 検索対象ドライブ番号               */
    F_WORD reserved;                    /* 予約領域                           */
    F_CHAR keyword[ DFMR_MAXFILESZ ];   /* キーワード                         */
}TFMR_SEARCHHANDLE;

/*------------------------------------*/
/*--- API I/F ------------------------*/
/*------------------------------------*/
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
typedef struct tagTFMR_FILE_INFO{
    F_WORD crtdate;                     /* 作成日付                           */
    F_WORD crttime;                     /* 作成時刻                           */
    F_WORD lstaccdate;                  /* 最終アクセス日付                   */
    F_WORD wrtdate;                     /* 更新日付                           */
    F_WORD wrttime;                     /* 更新時刻                           */
    F_BYTE crtmsec;                     /* 作成時刻(msec)                     */
    F_BYTE attr;                        /* ファイル属性                       */
    F_DWORD filesize;                   /* ファイルサイズ                     */
    F_BYTE sfn[ DFMR_FNAMESZ + DFMR_EXTSZ ];
                                        /* ファイル名                         */
    F_BYTE reserved;                    /* 予約領域                           */
    F_BYTE lfn[ DFMR_MAXFILESZ ];       /* ロングファイル名                   */
}TFMR_FILE_INFO;

/* ドライブ状態構造体 */
typedef struct tagTFMR_DRIVE_INFO{
    F_BYTE volume[DFMR_VNAMESZ ];       /* ボリュームラベル                   */
    F_BYTE mediatype;                   /* メディアタイプ                     */
    F_BYTE fattype;                     /* FATタイプ                          */
    F_BYTE reserved;                    /* 予約領域                           */
    F_DWORD totalsize;                  /* 容量                               */
    F_DWORD usedsize;                   /* 使用領域                           */
    F_DWORD emptyzise;                  /* 空き領域                           */
}TFMR_DRIVE_INFO;

/*------------------------------------*/
/*--- 内部使用 -----------------------*/
/*------------------------------------*/
/* エントリ検索用構造体 */
typedef struct tagTFMR_SEARCHINFO{
    F_DWORD sectnum;                    /* セクタ番号                         */
    F_BYTE slot;                        /* セクタ内位置                       */
    F_BYTE bufnum;                      /* 対象バッファ番号                   */
    F_WORD reserved;                    /* 予約領域                           */
}TFMR_SEARCHINFO;

/* エンディアン調整用共用体            */

typedef union tagT_ENDIAN {
	F_DWORD				dwordval;				/* ダブルワード値			*/
	F_WORD				wordval;				/* ワード値					*/
	F_BYTE				byteval[4];				/* バイト値					*/
} T_ENDIAN;

#endif /* __FMR_STRUCT_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

