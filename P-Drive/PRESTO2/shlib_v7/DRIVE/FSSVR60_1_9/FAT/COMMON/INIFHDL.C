/*======================================================================
 * File name    : inifhdl.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/inifhdl.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: inifhdl.c $
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
/* 関 数 名：FMR_initFileHandle                                               */
/*         ：                                                                 */
/* パラメタ：IN:  F_INT ファイルハンドル番号                                  */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：void                                                             */
/*         ：                                                                 */
/* 概    要：ファイルハンドルを初期化する                                     */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_initFileHandle(
    F_INT handle                        /* ファイルハンドル番号               */
)
{
	/* エントリイメージ初期化 */
	FMR_memSet(
		&tfmr_filehandle[ handle ].direntry,
		DFMR_INITBYTE,
		sizeof( TFMR_FILEHANDLE )
		);
	/* 他パラメータ初期化 */
	tfmr_filehandle[ handle ].entdirsectnum = 0xFFFFFFFF;
	tfmr_filehandle[ handle ].fptr = 0xFFFFFFFF;
	tfmr_filehandle[ handle ].crtsectnum = 0;
	tfmr_filehandle[ handle ].crtsectofst = 0xFFFF;
	tfmr_filehandle[ handle ].acsmode = 0;
	tfmr_filehandle[ handle ].entdirsectpos = 0xFF;
	tfmr_filehandle[ handle ].drivenum = 0;
	tfmr_filehandle[ handle ].driveindex = 0;

}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
