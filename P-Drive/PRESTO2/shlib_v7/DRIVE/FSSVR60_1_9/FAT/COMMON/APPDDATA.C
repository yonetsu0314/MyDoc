/*======================================================================
 * File name    : appddata.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/appddata.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: appddata.c $
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
/* 関 数 名：FMR_appendData                                                   */
/*         ：                                                                 */
/* パラメタ：IN:  F_INT ファイルハンドル番号                                  */
/*         ：     F_DWORD 書込み開始セクタ番号                                */
/*         ：     F_BYTE * 書込みデータ格納領域                               */
/*         ：     F_DWORD 書込みサイズ                                        */
/*         ：     F_DWORD 検索開始クラスタ番号                                */
/*         ：                                                                 */
/*         ：OUT: F_DWORD * 新規取得先頭クラスタ番号                          */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 エラーコード                                      */
/*         ：                                                                 */
/* 概    要：データ追加書込みを行う                                           */
/*         ：                                                                 */
/* 注　　意：FAT読込み領域１は当処理内で使用                                  */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_appendData(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_DWORD startsectnum,               /* 書込み開始セクタ番号               */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD datasize,                   /* 書込みサイズ                       */
    F_DWORD srchstartclstnum,           /* 検索開始クラスタ番号               */
    F_DWORD * newtopclstnum             /* 新規取得先頭クラスタ番号           */
)
{
	F_DWORD l_flacsize;                 /* 終端部端数サイズ                   */
	F_BYTE * l_startptr;                /* 書込み開始位置ポインタ             */
	F_DWORD l_eoc;                      /* EOCコード                          */
	
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;

	F_DWORD l_modbps;                   /* 剰余算計算用(Byte/Sector)          */
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */

	F_DWORD l_newbtmclstnum;            /* 新規取得最終クラスタ番号           */

	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	F_WORD l_targetsectnum;             /* 書込みセクタ数                     */
	F_BYTE l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_fatareanum;                /* 最終FAT読込み領域番号              */

	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_drvnum = tfmr_filehandle[ handlenum ].drivenum;
	l_modbps = MFMR_BYTEPERSECT( l_drvnum ) - 1;
	l_modspc = MFMR_SECTPERCLST( l_drvnum ) - 1;
	
	l_flacsize = datasize & l_modbps;
	l_targetsectnum = ( F_WORD )( datasize / MFMR_BYTEPERSECT( l_drvnum ) );
	l_startptr = databuff;
	*( newtopclstnum ) = 0;             /* 新規取得最終クラスタ番号初期化     */
	l_fatareanum = DFMR_REFFATAREA0;    /* 最終FAT読込み領域番号初期化        */

	MFMR_GETEOC( MFMR_FATTYPE( l_drvnum ), l_eoc );
	

	/**************************************************************************/
	/* セクタ単位の書込みを行う                                               */
	/**************************************************************************/
	if( l_targetsectnum != 0 )
	{
		/**********************************************************************/
		/* セクタ単位の書込みが存在する場合                                   */
		/**********************************************************************/
		/* 連続書込み処理 */
		l_rtn = FMR_writeDataSectors(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			startsectnum,               /* 先頭セクタ番号                     */
			l_startptr,                 /* 書込みデータ格納領域               */
			l_targetsectnum,            /* 書込みセクタ数                     */
			srchstartclstnum,           /* 検索開始クラスタ番号               */
			DFMR_REFFATAREA0,           /* 最終FAT読込み領域番号              */
			newtopclstnum,              /* 新規取得先頭クラスタ番号           */
			&l_newbtmclstnum,           /* 新規取得最終クラスタ番号           */
			&l_fatareanum               /* 最終FAT読込み領域番号              */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/* 書込み開始位置を書込んだサイズ分進める */
		l_startptr += (l_targetsectnum * MFMR_BYTEPERSECT( l_drvnum ));
	}
	/**********************************************************************/
	/* 終端部端数の書込みを行う                                           */
	/**********************************************************************/
	if( l_flacsize != 0 )
	{
		/******************************************************************/
		/* 終端に端数の書込みデータが存在する場合                         */
		/******************************************************************/
		/* 端数を書込むセクタを求める */
		if ( l_targetsectnum != 0 )
		{
			l_work01 = tfmr_filehandle[ handlenum ].fptr;
			if( ( l_work01 != 0) &&
				( ( l_work01 % ( MFMR_SECTPERCLST( l_drvnum ) * MFMR_BYTEPERSECT( l_drvnum ) ) == 0 ) ) )
			{
				/******************************************************************/
				/* ファイルポインタがクラスタ終端に位置する場合                   */
				/******************************************************************/
				l_work01 = 0;			/* 書込み開始時のクラスタオフセットを０とする */
			}
			else
			{
				/******************************************************************/
				/* ファイルポインタがクラスタ終端以外に位置する場合               */
				/******************************************************************/
				l_work01 = startsectnum - MFMR_DATATOP( l_drvnum );
				l_work01 &= l_modspc;	/* 書込み開始時のクラスタオフセットをカレントセクタ番号より求める */
			}
			l_work02 = l_work01;
			l_work01 = MFMR_SECTPERCLST( l_drvnum ) - l_work01;	 /* 書込み開始時クラスタの残りセクタ数 */
			if ( l_targetsectnum >= l_work01 )
			{
				/* 連続書込みセクタ数≧書込み開始時クラスタの残りセクタ数*/
				l_work01 = l_targetsectnum - l_work01;
			}
			else
			{
				/* 連続書込みセクタ数<書込み開始時クラスタの残りセクタ数*/
				l_work02 += l_targetsectnum;
				l_work01 = l_work02;
			}
			l_work01 &= l_modspc;	/* 端数書込みクラスタオフセット */
			/* 連続書込み終了時クラスタと端数書込みクラスタオフセットよりセクタ番号を求める */
			l_work02 = FMR_getSectNum( l_newbtmclstnum, l_work01, l_drvnum );
		}
		else
		{
			l_work01 = MFMR_SECTPERCLST( l_drvnum );
			l_work02 = startsectnum;
			l_newbtmclstnum = startsectnum - MFMR_DATATOP( l_drvnum );
			l_newbtmclstnum /= MFMR_SECTPERCLST( l_drvnum );
			l_newbtmclstnum += 2;
		}
		/***********************************/
		/* R/Wバッファにイメージを作成する */
		/***********************************/
		/* R/Wバッファを全て0で埋める */
		FMR_memSet(
			MFMR_RWBUFFTOP( l_drvnum, 0 ),  /* R/Wバッファ先頭                */
			DFMR_INITBYTE,                  /* 初期値0                        */
			MFMR_BYTEPERSECT( l_drvnum )    /* セクタ当りのバイト数           */
			);
		/* R/Wバッファの先頭から端数サイズ分のデータを書込む */
		FMR_memCopy(
			MFMR_RWBUFFTOP( l_drvnum, 0 ),  /* R/Wバッファ先頭                */
			l_startptr,                 /* 書込み開始位置                     */
			l_flacsize                  /* 終端部端数サイズ                   */
			);
		if( l_work01 != 0 )
		{
			l_work01 = l_work02;        /* 検索開始クラスタ番号設定           */
		}
		l_drtn = FMR_writeEndData(
					&l_work01,          /* 書込み開始セクタ番号               */
					&l_newbtmclstnum,   /* 新規取得最終クラスタ番号           */
					newtopclstnum,      /* 新規取得先頭クラスタ番号           */
					&l_fatareanum,      /* 現在参照中のFAT読込み領域番号      */
					l_drvnum            /* 対象ドライブ番号                   */
					);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	/******************************************************************/
	/* 最終クラスタ番号のFATエントリにEOCを設定する                   */
	/******************************************************************/
	l_drtn = FMR_controlFatEntry(
		MFMR_DRVINFO( l_drvnum ),       /* 論理ドライブ構成情報アドレス       */
		MFMR_FATTABLE( l_drvnum, l_fatareanum ),
		                                /* FAT読込み領域アドレス              */
		l_newbtmclstnum,                /* クラスタ番号                       */
		&l_eoc,                         /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別：Write指定          */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	
	if( l_fatareanum != DFMR_REFFATAREA0 )
	{
		/******************************************************************/
		/* アクセス対象のFAT読込み領域が0以外の場合は                     */
		/* この関数内で更新を行う                                         */
		/******************************************************************/
		l_drtn = FMR_setFatSector(
			MFMR_DRVINFO( l_drvnum ),     /* 論理ドライブ構成情報アドレス   */
			MFMR_FATTABLE( l_drvnum, l_fatareanum ),
			                            /* FAT読込み領域アドレス          */
			l_fatareanum                /* FAT読込み領域番号              */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA1) = 0;
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

