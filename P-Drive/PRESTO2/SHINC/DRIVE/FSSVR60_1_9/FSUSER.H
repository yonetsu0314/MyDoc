/*======================================================================
 * File name    : fsuser.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/rcs/fsuser.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsuser.h $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
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
/* 関 数 名：FMR_User.h                                                       */
/* パラメタ：                                                                 */
/* 戻 り 値：                                                                 */
/* 概    要：ユーザ定義を設定する                                             */
/* 作 成 日：2004.07.15                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 DFMR_RWBUFFSIZE追加                             05/02/09 Y.Yoshizumi */
/* 01.09 ファイル&検索ハンドル排他制御用セマフォID追加   06/03/01 Y.Yoshizumi */
/******************************************************************************/

#ifndef __FMR_USER_H__
#define __FMR_USER_H__

/* ユーザ定義を記述する */
#define DFMR_RWBUFFSIZE               8 /* R/Wバッファサイズ                  */
#define DFMR_USR_MAXFILEHANDLE       32 /* 最大ファイルハンドル数             */
#define DFMR_USR_MAXSEARCHHANDLE      8 /* 最大検索ハンドル数                 */
#define DFMR_USR_MAXSECTSIZE        512 /* セクタサイズ                       */
#define DFMR_USR_MAXDRVNUM           26 /* 最大論理ドライブ数                 */
#define DFMR_USR_MAXDRIVER            2 /* 最大ドライバ数                     */
#define DFMR_USR_MAXCHANNEL           1 /* 最大チャネル数                     */
#define DFMR_USR_SEMID_HANDLE         0 /* ファイル&検索ハンドル用セマフォID  */
                                        /*   0=auto ID                        */

#define DFMR_USR_F32RDCLST            4 /* FAT32 ルートディレクトリクラスタ   */
                                        /* 検索許容の割合(1～100)             */
#if( ( DFMR_USR_F32RDCLST < 1 ) || ( 100 < DFMR_USR_F32RDCLST  ) )
#undef DFMR_USR_F32RDCLST               /* 先頭クラスタがBADクラスタの場合は  */
#define DFMR_USR_F32RDCLST            0 /* フォーマットを行わない             */
#endif


#define DFMR_ENDIAN_BIG                 /* Big Endian(Little Endian           */
                                        /* 指定時はコメントアウト)            */
#define DFMR_SEMID_AUTO                 /* セマフォID自動割付ON               */
                                        /* (自動割付OFFはコメントアウト)      */
#define DFMR_USR_SDRETRY              2 /* スキャンディスク時デバイスアクセス */
                                        /* リトライ回数                       */

#endif /* __FMR_USER_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2002      ウェスコム株式会社                */
/******************************************************************************/
