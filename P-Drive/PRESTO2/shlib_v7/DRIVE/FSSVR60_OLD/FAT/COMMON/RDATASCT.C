/*======================================================================
 * File name    : rdatasct.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/rdatasct.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rdatasct.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/16 21:00
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
/* 関 数 名：FMR_readDataSector                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR    対象ドライブ番号                                  */
/*         ：     F_DWORD   読込みセクタ番号                                  */
/*         ：     F_BYTE    R/Wバッファスイッチ                               */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：指定R/Wバッファへ指定セクタ番号から１セクタ数読込む              */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/*       読込セクタが既に読込んである場合、正常終了      04/12/10 Y.Yoshizumi */
/*       読込セクタ番号をR/Wバッファ読込セクタ番号に保存 04/12/10 Y.Yoshizumi */
/*       エラー時R/Wバッファ読込セクタ番号初期化を追加   04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_readDataSector(
    F_CHAR  drvnum,                     /* 対象ドライブ番号                   */
    F_DWORD sectnum,                    /* 読込みセクタ番号                   */
    F_BYTE  buffsw                      /* R/Wバッファスイッチ                */
)
{
	TFMR_DEVIOREAD l_devioread;         /* ドライバI/F(Read)I/Oパラメータ     */
	F_INT l_drtn;                       /* ドライバI/F戻り値                  */
	F_WORD l_readsectcnt;               /* 読込みセクタ数                     */

	/* 読込み対象のセクタが既にR/Wバッファ上に読込んである場合、正常終了 */
	if ( MFMR_RWSECTNUM( drvnum, buffsw ) == sectnum ) {
		return DFMR_OK;
	}

	l_devioread.startsctnum = sectnum;  /* 読込み開始セクタ番号               */
	l_readsectcnt = 1;
	l_devioread.readsctnum = &l_readsectcnt; /* 読込みセクタ数                */
	l_devioread.readbuff = MFMR_RWBUFFTOP( drvnum, buffsw );
	                                    /* 読込みデータ格納領域               */
	/* 読込み対象のセクタを読込む */
	l_drtn = MFMR_DRVENTRY( drvnum )(
		MFMR_CUNUM( drvnum ),           /* チャネル/ユニット番号              */
		DFMR_DRVFUNCREAD,               /* I/O Func.コード：データ読込み      */
		&l_devioread                    /* I/Oパラメータ                      */
		);
	if( (l_drtn == DFMR_OK) && (l_readsectcnt != 1) ) 
	{
		MFMR_RWSECTNUM( drvnum, buffsw ) = 0xffffffff;
		return DFMR_FATALERR;
	}
	/* 読込んだセクタ番号を保存 */
	MFMR_RWSECTNUM( drvnum, buffsw ) = sectnum;
	return l_drtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
