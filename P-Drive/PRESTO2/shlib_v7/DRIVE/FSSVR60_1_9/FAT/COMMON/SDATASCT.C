/*======================================================================
 * File name    : sdatasct.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/sdatasct.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sdatasct.c $
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
/* 関 数 名：FMR_setDataSector                                                */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR    対象ドライブ番号                                  */
/*         ：     F_DWORD   書込みセクタ番号                                  */
/*         ：     F_BYTE    R/Wバッファスイッチ                               */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_OK                                                */
/*         ：異常終了  DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：指定R/Wバッファから指定セクタ番号へ１セクタ書込む                */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/*       R/Wバッファ読込セクタ番号初期化を追加           04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_setDataSector(
    F_CHAR  drvnum,                     /* 対象ドライブ番号                   */
    F_DWORD sectnum,                    /* 書込みセクタ番号                   */
    F_BYTE  buffsw                      /* R/Wバッファスイッチ                */
)
{
	TFMR_DEVIOWRITE l_deviowrite;       /* ドライバI/F(Write)I/Oパラメータ    */
	F_INT l_drtn;                       /* ドライバI/F戻り値                  */
	F_WORD l_writesectcnt;              /* 書込みセクタ数                     */

	l_deviowrite.startsctnum = sectnum; /* 書込み開始セクタ番号               */
	l_writesectcnt = 1;
	l_deviowrite.writesctnum = &l_writesectcnt; /* 書込みセクタ数             */
	l_deviowrite.writebuff = MFMR_RWBUFFTOP( drvnum, buffsw );  /* 書込みデータ格納領域 */
	MFMR_RWSECTNUM( drvnum, 0 ) = 0xffffffff;
	MFMR_RWSECTNUM( drvnum, 1 ) = 0xffffffff;
	if( l_deviowrite.startsctnum == 0 )
	{
		return DFMR_FATALERR;
	}
	
	/* 書込み対象のセクタを書込む */
	l_drtn = MFMR_DRVENTRY( drvnum )(
		MFMR_CUNUM( drvnum ),           /* チャネル/ユニット番号              */
		DFMR_DRVFUNCWRITE,              /* I/O Func.コード：データ書込み      */
		&l_deviowrite                   /* I/Oパラメータ                      */
		);
	if( (l_drtn == DFMR_OK) && (l_writesectcnt != 1) ) 
	{
		return DFMR_FATALERR;
	}
	return l_drtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
