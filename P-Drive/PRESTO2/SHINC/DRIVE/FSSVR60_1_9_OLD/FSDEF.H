/*======================================================================
 * File name    : fsdef.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/rcs/fsdef.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsdef.h $
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
/* 関 数 名：FMR_Def.h                                                        */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：定数定義                                                         */
/*         ：                                                                 */
/* 作 成 日：2004.04.15                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 FMR_copySector コピーモードフラグ追加           05/02/09 Y.Yoshizumi */
/*       DFMR_RWBUFFSIZE削除                             05/02/09 Y.Yoshizumi */
/******************************************************************************/

#ifndef __FMR_DEF_H__
#define __FMR_DEF_H__

/*------------------------------------*/
/*--- 型宣言 -------------------------*/
/*------------------------------------*/
typedef signed   short  F_BOOL;         /* 16bitブール値                      */
typedef signed   char   F_CHAR;         /* 符号付8bit値                       */
typedef unsigned char   F_BYTE;         /* 符号無し8bit値                     */
typedef signed   short  F_SHORT;        /* 符号付16bit値                      */
typedef unsigned short  F_WORD;         /* 符号無し16bit値                    */
typedef signed   int    F_INT;          /* 符号付16/32bit値(実装依存)         */
typedef unsigned int    F_UINT;         /* 符号無し16/32bit値(実装依存)       */
typedef signed   long   F_LONG;         /* 符号付32bit値                      */
typedef unsigned long   F_DWORD;        /* 符号無し32bit値                    */
typedef void F_VOID;                    /* void型                             */

/*------------------------------------*/
/*--- 汎用パラメータ -----------------*/
/*------------------------------------*/
#define DFMR_INITBYTE                0  /* 初期値0                            */
#define DFMR_TOPDRVNUM             'A'  /* ドライブ番号最小値                 */
#define DFMR_ENDDRVNUM             'Z'  /* ドライブ番号最大値                 */
#define DFMR_RAMINIT        0xFFFFFFFF  
                                   /* 論理ドライブRAMテーブルアドレス群初期値 */
#ifndef NULL
#define NULL                         0  /* NULL                               */
#endif /* NULL */
/*------------------------------------*/
/*--- 関数I/F ------------------------*/
/*------------------------------------*/
#define DFMR_SUC                     0  /* API正常終了                        */
#define DFMR_ERR                    -1  /* API異常終了                        */
#define DFMR_OK                      0  /* 内部処理正常終了                   */
#define DFMR_NG                 0xFFFF  /* 内部処理異常終了                   */
#define DFMR_LNG            0xFFFFFFFF  /* 内部処理異常終了(DWORD)            */
#define DFMR_LFN                     1  /* 名称チェック時、ロングファイル名   */

#define DFMR_MBR_OFF              0x80  /* MBR無し                            */

#define DFMR_O_RDONLY           0x0000  /* リードオンリー                     */
#define DFMR_O_WRONLY           0x0001  /* ライトオンリー                     */
#define DFMR_O_RDWR             0x0002  /* リード／ライト                     */
#define DFMR_O_APPEND           0x0008  /* アペンド                           */
#define DFMR_O_CREAT            0x0100  /* クリエイト                         */
#define DFMR_O_TRUNC            0x0200  /* トランケイト                       */
#define DFMR_O_EXCL             0x0400  /* エクスクルーシブ                   */

#define DFMR_SEEK_SET                0  /* ファイルの先頭                     */
#define DFMR_SEEK_CUR                1  /* ファイルポインタの現在位置         */
#define DFMR_SEEK_END                2  /* ファイルの終端                     */
#define DFMR_FLGON                   1  /* フラグON                           */
#define DFMR_FLGOFF                  0  /* フラグOFF                          */

/*------------------------------------*/
/*--- エラーコード -------------------*/
/*------------------------------------*/
#define DFMR_ENOENT             ( -2 )  /* 該当ファイル無し                   */
#define DFMR_EBADF              ( -9 )  /* ファイルアクセス異常               */
#define DFMR_EACCES            ( -13 )  /* モード不一致・directory            */
#define DFMR_EEXIST            ( -17 )  /* 既存ファイル有り                   */
#define DFMR_EINVAL            ( -22 )  /* イリーガルなモード                 */
#define DFMR_EMFILE            ( -24 )  /* オープンファイルオーバー           */
#define DFMR_EFBIG             ( -27 )  /* ファイルサイズオーバー             */
#define DFMR_ENOTEMPTY         ( -41 )  /* 対象ディレクトリ中身あり           */
#define DFMR_MULTIOPEN         ( -43 )  /* ２重オープン                       */
#define DFMR_NMERR             ( -50 )  /* 名称異常                           */
#define DFMR_ERRPARAM          ( -53 )  /* パラメータエラー                   */
#define DFMR_FATALERR          ( -54 )  /* FATAL ERROR                        */
#define DFMR_NOENTRY           ( -60 )  /* 対象エントリ無し                   */
#define DFMR_NODIR             ( -61 )  /* 対象上位ディレクトリ無し           */
#define DFMR_ENOEMPTYENTRY     ( -70 )  /* 未使用/無効エントリ領域なし        */
#define DFMR_MEMFULL           ( -71 )  /* メモリフル                         */
#define DFMR_ESEARCHFAIL       ( -72 )  /* 検索失敗                           */
#define DFMR_EIMPFORMAT        ( -73 )  /* フォーマット未完了                 */
#define DFMR_SAMENAME          ( -74 )  /* 同一エントリ名有り                 */
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
#define DFMR_FORMATERR        ( -273 )  /* フォーマット失敗                   */

#if 0
#define DFMR_EDEVERASE    0xFEFF        /* デバイス、消去失敗                 */
#define DFMR_EDEVWRITE    0xFEFE        /* デバイス、書込み失敗               */
#define DFMR_EDEVREAD     0xFEFD        /* デバイス、読込み失敗               */

#define DFMR_EFATEND      0x0EFF        /* 読込んだFATの終端まで検索完了      */
#define DFMN_ESEARCHFAIL                /* 検索対象エントリが存在しない       */

#define DFMR_ENODEV                     /* デバイスが存在しない               */
#define DFMR_ENOREADY                   /* デバイスが起動していない           */
#define DFMR_EDEVBUSY                   /* デバイスが使用中                   */
#endif

/*------------------------------------*/
/*--- Master Boot Record -------------*/
/*------------------------------------*/
#define DFMR_MBR_BSLSIZE           446  /* ブートストラップローダのサイズ     */

#define DFMR_PRTTBL_PARTMAX          4  /* 最大パーティション数               */
#define DFMR_PRTTBL_CHSSIZE          3  /* CHS形式セクタ番号サイズ            */
#define DFMR_PRTTBL_ACTFLGON      0x80  /* アクティブフラグON                 */
#define DFMR_PRTTBL_ACTFLGOFF     0x00  /* アクティブフラグOFF                */
#define DFMR_PRTTBL_TYPEF12       0x01  /* パーティションタイプ=FAT12         */
#define DFMR_PRTTBL_TYPEF16LOW    0x04  /* パーティションタイプ=FAT16         */
                                        /* 32MByte未満                        */
#define DFMR_PRTTBL_TYPEF16HI     0x06  /* パーティションタイプ=FAT16         */
                                        /* 32MByte以上                        */
#define DFMR_PRTTBL_TYPEF32CHS    0x0B  /* パーティションタイプ=FAT32(CHS)    */
#define DFMR_PRTTBL_TYPEF32LBA    0x0C  /* パーティションタイプ=FAT32(LBA)    */
#define DFMR_PRTTBL_TYPEF16LBA    0x0E  /* パーティションタイプ=FAT16(LBA)    */
#define DFMR_PRTTBL_LBATOPSECT  0x00000020 /* フォーマット時先頭セクタ番号    */

#define DFMR_PARTMINSIZE          4100  /* 最小パーティションサイズ           */
#define DFMR_FAT12MAXSIZE       131072  /* FAT12最大パーティションサイズ      */
#define DFMR_FAT16LOWMAXSIZE     32768  /* FAT16(32BYTE未満)最大パーティションサイズ      */
#define DFMR_FAT16MAXSIZE      2097152  /* FAT16最大パーティションサイズ      */
#define DFMR_FAT32MINSIZE        33300  /* FAT32最小パーティションサイズ      */

/*------------------------------------*/
/*--- Reserved Region BS,BPB,FSInfo --*/
/*------------------------------------*/
/* ブートシグネチャ */
#define DFMR_RSVRGN_SIGP510        510  /* 510Byte目シグネチャ位置            */
#define DFMR_RSVRGN_SIGV510       0x55  /* 510Byte目シグネチャ値              */
#define DFMR_RSVRGN_SIGP511        511  /* 511Byte目シグネチャ位置            */
#define DFMR_RSVRGN_SIGV511       0xAA  /* 511Byte目シグネチャ値              */

/* 格納位置オフセット */
#define DFMR_BSOFST_JMPBOOT          0  /* ブートコードへのジャンプ命令       */
#define DFMR_BSOFST_OEMNAME          3  /* フォーマットを行ったシステム名     */
#define DFMR_BPBOFST_BYTEPERSEC     11  /* 1セクタ当りのバイト数              */
#define DFMR_BPBOFST_SECPERCLUS     13  /* 1クラスタ当りのセクタ数            */
#define DFMR_BPBOFST_RSVDSECCNT     14  /* 予約領域が占めるセクタ数           */
#define DFMR_BPBOFST_NUMFATS        16  /* ボリュームに含まれるFAT数          */
#define DFMR_BPBOFST_ROOTENTCNT     17  /* ルートディレクトリに含まれる       */
                                        /* ディレクトリエントリ               */
#define DFMR_BPBOFST_TOTSEC16       19  /* ボリューム全体のセクタ数           */
#define DFMR_BPBOFST_MEDIA          21  /* メディアタイプ                     */
#define DFMR_BPBOFST_FATSZ16        22  /* 1つのFATが占めるセクタ数           */
#define DFMR_BPBOFST_SECPERTRK      24  /* INT13のための1トラック当りの       */
                                        /* セクタ数                           */
#define DFMR_BPBOFST_NUMHEADERS     26  /* INT13のためのヘッド数              */
#define DFMR_BPBOFST_HIDDSEC        28  /* ボリュームの前にあるセクタの数     */
#define DFMR_BPBOFST_TOTSEC32       32  /* ボリューム全体のセクタ数           */

#define DFMR_BSOFST_DRVNUM          36  /* INT13のドライブ番号                */
#define DFMR_BSOFST_RESERVED1       37  /* 予約領域                           */
#define DFMR_BSOFST_BOOTSIG         38  /* 拡張ブートシグネチャ               */
#define DFMR_BSOFST_VOLID           39  /* ボリュームシリアル番号             */
#define DFMR_BSOFST_VOLLAB          43  /* ボリュームラベル                   */
#define DFMR_BSOFST_FILESYSTYPE     54  /* FATタイプ                          */

#define DFMR_BPBOFST32_FATSZ32      36  /* 1つのFATが占めるセクタ数           */
#define DFMR_BPBOFST32_EXTFLGS      40  /* 動作中のFATを表すフラグ郡          */
#define DFMR_BPBOFST32_FSVER        42  /* FAT32のバージョン番号              */
#define DFMR_BPBOFST32_ROOTCLUS     44  /* ルートディレクトリの               */
                                        /* 先頭クラスタ番号                   */
#define DFMR_BPBOFST32_FSINFO       48  /* FSInfo構造体のセクタ番号           */
#define DFMR_BPBOFST32_BKBOOTSEC    50  /* ブートレコードのコピー             */
#define DFMR_BPBOFST32_RESERVED     52  /* 予約領域                           */
#define DFMR_BSOFST32_DRVNUM        64  /* INT13のドライブ番号                */
#define DFMR_BSOFST32_RESERVED1     65  /* 予約領域                           */
#define DFMR_BSOFST32_BOOTSIG       66  /* 拡張ブートシグネチャ               */
#define DFMR_BSOFST32_VOLID         67  /* ボリュームシリアル番号             */
#define DFMR_BSOFST32_VOLLAB        71  /* ボリュームラベル                   */
#define DFMR_BSOFST32_FILESYSTYPE   82  /* FATタイプ                          */

#define DFMR_FSIOFST_LEADSIG         0  /* リードシグネチャ                   */
#define DFMR_FSIOFST_RESERVED        4  /* 予約領域                           */
#define DFMR_FSIOFST_STRUCSIG      484  /* 構造シグネチャ                     */
#define DFMR_FSIOFST_FREECOUNT     488  /* 空きクラスタ数                     */
#define DFMR_FSIOFST_NXTFREE       492  /* 空きクラスタ番号                   */
#define DFMR_FSIOFST_RESERVED2     496  /* 予約領域                           */

/* パラメータ */
#define DFMR_BSJMPBOOT00          0xEB  /* ジャンプ命令0Byte目                */
#define DFMR_BSJMPBOOT01          0x3C  /* ジャンプ命令1Byte目                */
#define DFMR_BSJMPBOOT02          0x90  /* ジャンプ命令2Byte目                */
#define DFMR_BSOEMNAME      "MSWIN4.1"  /* フォーマットしたシステム名         */
#define DFMR_BPBRSVDSECCNT12         1  /* Reserved Region が占めるセクタ数   */
                                        /* (FAT12/16)                         */
#define DFMR_BPBRSVDSECCNT32        32  /* Reserved Region が占めるセクタ数   */
                                        /* (FAT32)                            */
#define DFMR_BPBNUMFATS              2  /* ボリュームに含まれるFATの数        */
#define DFMR_BPBROOTENTCNT         512  /* ルートディレクトリに含まれる       */
                                        /* ディレクトリエントリ数             */
#define DFMR_BPBMEDIA             0xF8  /* メディアタイプ                     */

#define DFMR_BSDRVNUM             0x80  /* INT13のドライブ番号                */
#define DFMR_BSRESERVED1          0x00  /* 予約領域                           */
#define DFMR_BSBOOTSIG            0x29  /* 拡張ブートシグネチャ               */
#define DFMR_BSFILESYSTYPE12    "FAT12   "   /* FATタイプ(FAT12)              */
#define DFMR_BSFILESYSTYPE16    "FAT16   "   /* FATタイプ(FAT16)              */
#define DFMR_BSFILESYSTYPE32    "FAT32   "   /* FATタイプ(FAT32)              */

#define DFMR_ALLUPDATE          0x0000  /* FAT32 全てのFATが逐次更新          */
#define DFMR_TARGETUPDATE       0x0080  /* FAT32 指定したFATのみを更新        */
#define DFMR_BPBFSVER           0x0000  /* FAT32のバージョン番号              */
#define DFMR_BPBROOTCLUS    0x00000002  /* ルートディレクトリの               */
                                        /* 先頭クラスタ番号(FAT32)            */
#define DFMR_BPBFSINFO      0x00000001  /* FSInfo構造体のセクタ番号(FAT32)    */
#define DFMR_BPBBKBOOTSEC       0x0006  /* ブートレコードのコピー(FAT32)      */
#define DFMR_BPBRESERVED             0  /* 予約領域(FAT32)                    */

#define DFMR_FSILEADSIG0          0x41  /* リードシグネチャ0Byte目            */
#define DFMR_FSILEADSIG1          0x61  /* リードシグネチャ1Byte目            */
#define DFMR_FSILEADSIG2          0x52  /* リードシグネチャ2Byte目            */
#define DFMR_FSILEADSIG3          0x52  /* リードシグネチャ3Byte目            */
#define DFMR_FSIRESERVED1            0  /* 予約領域                           */
#define DFMR_FSISTRUCSIG0         0x61  /* 構造シグネチャ0Byte目              */
#define DFMR_FSISTRUCSIG1         0x41  /* 構造シグネチャ1Byte目              */
#define DFMR_FSISTRUCSIG2         0x72  /* 構造シグネチャ2Byte目              */
#define DFMR_FSISTRUCSIG3         0x72  /* 構造シグネチャ3Byte目              */
#define DFMR_FSICLSTINIT          0xFF  /* 空きクラスタ数/番号デフォルト値    */
#define DFMR_FSIRESERVED2            0  /* 予約領域                           */
#define DFMR_BPBVOLLAB   "NO NAME    "  /* ボリュームラベルデフォルト         */

/*------------------------------------*/
/*--- FAT ----------------------------*/
/*------------------------------------*/
#define DFMR_EOC12               0xFFF  /* EOC(FAT12)                         */
#define DFMR_EOC16              0xFFFF  /* EOC(FAT16)                         */
#define DFMR_EOC32          0x0FFFFFFF  /* EOC(FAT32)                         */
#define DFMR_EOC12MIN            0xFF8  /* EOC最小値(FAT12)                   */
#define DFMR_EOC16MIN           0xFFF8  /* EOC最小値(FAT16)                   */
#define DFMR_EOC32MIN       0x0FFFFFF8  /* EOC最小値(FAT32)                   */
#define DFMR_BAD12               0xFF7  /* バッドクラスタ(FAT12)              */
#define DFMR_BAD16              0xFFF7  /* バッドクラスタ(FAT16)              */
#define DFMR_BAD32          0x0FFFFFF7  /* バッドクラスタ(FAT32)              */
#define DFMR_NEWCLUS                 0  /* 未使用クラスタ                     */

/*------------------------------------*/
/*--- Directory/LFN Entry ------------*/
/*------------------------------------*/
#define DFMR_FNAMESZ                 8  /* ファイル名のサイズ                 */
#define DFMR_EXTSZ                   3  /* 拡張子のサイズ                     */
#define DFMR_VNAMESZ                11  /* ボリューム名サイズ                 */
#define DFMR_ATTR_RONLY           0x01  /* ファイルの属性(読込み専用)         */
#define DFMR_ATTR_HIDE            0x02  /* ファイルの属性(隠しファイル)       */
#define DFMR_ATTR_SYS             0x04  /* ファイルの属性(システムファイル)   */
#define DFMR_ATTR_VOLID           0x08  /* ファイルの属性(ボリュームID)       */
#define DFMR_ATTR_DIR             0x10  /* ファイルの属性(ディレクトリ)       */
#define DFMR_ATTR_ARC             0x20  /* ファイルの属性(アーカイブ)         */
#define DFMR_LFN_LAST             0x40  /* 分割されたLFNの最終エントリ        */
                                        /* 判断フラグ（ON：最終エントリ)      */
#define DFMR_LFN_ORDMASK          0x3F  /* LFNの順序取得用マスク              */
#define DFMR_LFN_FNAME1SZ            5  /* LFNのファイル名サイズ(0～4文字)    */
#define DFMR_LFN_FNAME2SZ            6  /* LFNのファイル名サイズ(5～10文字)   */
#define DFMR_LFN_FNAME3SZ            2  /* LFNのファイル名サイズ(11～12文字)  */
#define DFMR_LFN_FNAMESZ            13  /* LFNの1エントリの最大文字数(13文字) */
#define DFMR_LFN_ATTR             0x0F  /* ファイルの属性(LFN)                */
                                        /* (RONLY | HIDE | SYS | VOLID )      */
#define DFMR_SPACE                0x20  /* スペース記号                       */
#define DFMR_MAXPASSSZ             259  /* パス名サイズの最大値(259文字)      */
#define DFMR_MAXFILESZ             256  /* ファイル名サイズの最大値(256文字)  */

#define DFMR_DELENTRY             0xE5  /* エントリ削除済み                   */
#define DFMR_TBDENTRY             0x00  /* エントリ未使用                     */
#define DFMR_UTFALIAS             0x05  /* UTF先頭文字0xE5のエイリアス        */

#define DFMR_LFN_ENTMAX             19  /* LFNの最大エントリ数 256/13 = 19    */
#define DFMR_LFN_MOD                 8  /* 最大LFNの最終エントリ文字数        */

/*------------------------------------*/
/*--- RAM Area -----------------------*/
/*------------------------------------*/
#define DFMR_RWFATNUM                2  /* FAT読込み領域数                    */

#define DFMR_FATMNGSIZE           256   /* FAT管理領域最大値                  */
#define DFMR_NOEMPTYCLST   0xFFFFFFFF   /* FATセクタ管理情報、空きクラスタなし*/

#define DFMR_FATTYPEDEP           0x00 /* FATタイプ:サイズに依存              */
#define DFMR_FATTYPE12            0x01 /* FATタイプ:FAT12                     */
#define DFMR_FATTYPE16            0x02 /* FATタイプ:FAT16                     */
#define DFMR_FATTYPE32            0x03 /* FATタイプ:FAT32                     */

#define DFMR_FAT12NUMMASK  0xA0000000  /* FAT12,FAT読込み領域のセクタマスク   */
#define DFMR_FAT12TOP2BYTE 0x40000000  /* FAT12,FAT読込み領域の先頭2Byteが前セクタ   */
#define DFMR_FAT12TOP1BYTE 0x80000000  /* FAT12,FAT読込み領域の先頭1Byteが前セクタ   */

#define DFMR_REFFATAREA0            0  /* 参照先FAT読込み領域 = 0             */
#define DFMR_REFFATAREA1            1  /* 参照先FAT読込み領域 = 1             */

#define DFMR_ACTIVEFATMASK     0x000F  /* FAT32 アクティブFAT番号             */
/*------------------------------------*/
/*--- Driver I/F ---------------------*/
/*------------------------------------*/
#define DFMR_DRVMAXCHANNEL        0x0F  /* 最大チャネル番号                   */
#define DFMR_DRVMAXDEV            0x0F  /* 最大デバイス番号                   */
#define DFMR_DRVMAXUNIT           0x0F  /* 最大ユニット番号                   */

#define DFMR_DRVFUNCINIT          0x00  /* I/O Func.番号:初期化               */
#define DFMR_DRVFUNCMOUNT         0x01  /* I/O Func.番号:マウント             */
#define DFMR_DRVFUNCUNMOUNT       0x02  /* I/O Func.番号:アンマウント         */
#define DFMR_DRVFUNCCHANNEL       0x03  /* I/O Func.番号:チャネル情報取得     */
#define DFMR_DRVFUNCDEV           0x04  /* I/O Func.番号:デバイス情報取得     */
#define DFMR_DRVFUNCUNIT          0x05  /* I/O Func.番号:ユニット情報取得     */
#define DFMR_DRVFUNCREAD          0x06  /* I/O Func.番号:データ読込み         */
#define DFMR_DRVFUNCWRITE         0x07  /* I/O Func.番号:データ書込み         */
#define DFMR_DRVFUNCERASE         0x08  /* I/O Func.番号:データ消去           */
#define DFMR_DRVFUNCMEDIA         0x09  /* I/O Func.番号:メディア挿抜チェック */

/*------------------------------------*/
/*--- 内部関数用定数 -----------------*/
/*------------------------------------*/

/* エントリ検索関係 */

#define DFMR_SRCHLEN              0x1F  /* 検索処理フラグ：LFNエントリ数      */
#define DFMR_SRCHFLGSFN           0x01  /* エントリ検索:SFN指定ON             */
#define DFMR_SRCHFLGWC            0x20  /* エントリ検索:ワイルドカード指定ON  */
#define DFMR_SRCHFLGLFN           0x40  /* エントリ検索:LFN指定ON             */
#define DFMR_SRCHFLGCHK           0x80  /* エントリ検索:空エントリ検索時、    */
										/*       同一ファイル名チェックを行う */
#define DFMR_SRCHDIRTOPSLOT          2  /* ディレクトリ内検索先頭位置         */
#define DFMR_SRCHROOTTOPSLOT         0  /* ルートディレクトリ内検索先頭位置   */
#define DFMR_TOPCLSTNUM              2  /* 先頭クラスタ番号                   */
#define DFMR_FWDSECTREAD    0x80000000  /* 前セクタ読込みフラグ               */
#define DFMR_FWDSECTUPDATE  0x40000000  /* 前セクタ更新フラグ                 */
#define DFMR_MINSECTSIZE           512  /* 最小セクタサイズ                   */

/* FMR_setDirEntry関係 */
	/* 動作フラグ */

#define DFMR_SETFUNC_MKDIR      0x0100  /* Mkdir                              */
#define DFMR_SETFUNC_RENAMED    0x0200  /* Rename(Dir)                        */
#define DFMR_SETFUNC_RENAMEF    0x0400  /* Rename(File)                       */
#define DFMR_SETFUNC_OPEN       0x0800  /* Open                               */
#define DFMR_SETFUNC_SRCH       0x00FF  /* 検索処理フラグ                     */

/* FMR_deleteDirEntry関係 */
	/* 動作フラグ */

#define DFMR_DELFUNC_RMDIR      0x0100  /* Rmdir                              */
#define DFMR_DELFUNC_RENAME     0x0200  /* Rename                             */
#define DFMR_DELFUNC_REMOVE     0x0400  /* Remove                             */
#define DFMR_DELFUNC_SRCH       0x00FF  /* 検索処理フラグ                     */

/* FMR_copySector関係 */
	/* コピーモードフラグ */

#define DFMR_COPYSEC_FWDCPY          0  /* クラスタ先頭から書込み開始セクタまでコピー */
#define DFMR_COPYSEC_BCKCPY          1  /* 書込み終了セクタ+1からクラスタ終端までコピー*/
#define DFMR_COPYSEC_CPYEND          2  /* クラスタ先頭から書込み終了セクタまでコピー */
#define DFMR_COPYSEC_CPYALL          3  /* クラスタ全てコピー                         */

#endif /* __FMR_DEF_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
