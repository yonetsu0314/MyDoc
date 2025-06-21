/*======================================================================
 * File name    : rdscrtry.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/utility/common/RCS/rdscrtry.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: rdscrtry.c $
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
/* 関 数 名：FMR_readSectRetry                                                */
/*         ：                                                                 */
/* パラメタ：IN:  F_CHAR ドライブ番号                                         */
/*         ：     F_DWORD セクタ番号                                          */
/*         ：     F_BYTE R/Wバッファ番号                                      */
/*         ：                                                                 */
/* 戻 り 値：DFMR_SUC 正常終了                                                */
/*         ：(負の値) 異常終了                                                */
/*         ：                                                                 */
/* 概    要：対象セクタを読込む                                               */
/*         ：                                                                 */
/* 作 成 日：2004.09.02                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/
#include "fsbase.h"
#include "utility.h"

F_INT FMR_readSectRetry(
	F_CHAR drvnum,                      /* ドライブ番号                       */
	F_DWORD sectnum,                    /* セクタ番号                         */
	F_BYTE buffnum                      /* R/Wバッファ番号                    */
)
{
	TFMR_DRVIOPARAM l_drvioparam;       /* ドライバエントリI/F                */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	F_INT l_retrycnt;                   /* リトライカウンタ                   */
	F_WORD l_targetsctnum;              /* 対象セクタ数                       */
	
	/* パラメータ設定 */
	l_drvioparam.read.startsctnum = sectnum;    /* 読込み開始セクタ番号       */
	l_targetsctnum = 1;
	l_drvioparam.read.readsctnum = &l_targetsctnum; /* 読込みセクタ数         */
	l_drvioparam.read.readbuff          /* 読込みデータ格納領域               */
		= MFMR_RWBUFFTOP( drvnum, buffnum );
	                          
	for( l_retrycnt = 0; l_retrycnt < DFMR_USR_SDRETRY; l_retrycnt ++ )
	{
		l_drtn = MFMR_DRVENTRY( drvnum )(
			MFMR_CUNUM( drvnum ),       /* チャネル番号/ユニット番号          */
			DFMR_DRVFUNCREAD,           /* Func.番号=Read                     */
			&l_drvioparam               /* I/Oパラメータ                      */
			);
		if( l_targetsctnum != 1 )       /* 読込みセクタ数が1以外の場合は      */
		{                               /* オーバフローを起こしたと見なす     */
			l_drtn = DFMR_FATALERR;
			break;
		}
		if( l_drtn == DFMR_OK )
		{                               /* 正常終了の場合は処理終了           */
			break;
		}
		FMR_osRetryWait( );             /* ウェイト                           */
	}
	return l_drtn;
}

