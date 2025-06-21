/*======================================================================
 * File name    : setpoint.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/setpoint.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: setpoint.c $
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
/* 関 数 名：FMR_setPointer                                                   */
/*         ：                                                                 */
/* パラメタ：IN:  F_INT ファイルハンドル番号                                  */
/*         ：     F_LONG オフセット                                           */
/*         ：     F_INT 初期位置                                              */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_EINVAL                                       */
/*         ：                                                                 */
/* 概    要：ファイルポインタの移動を行う                                     */
/*         ：(FAT操作は全てFAT読込み領域1で行う)                              */
/*         ：                                                                 */
/* 作 成 日：2004.05.25                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_setPointer(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_LONG offset,                      /* オフセット                         */
    F_INT origin                        /* 初期位置                           */
)
{
	F_DWORD l_fptr;                     /* 移動後ポインタ位置                 */
	F_DWORD l_clstofst;                 /* 移動後クラスタオフセット           */
	F_DWORD l_clstnum;                  /* 検索用クラスタ番号                 */

	F_LONG l_work01;                    /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_cnt01;                    /* ループカウンタ                     */
	
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */
	F_DWORD l_modbps;                   /* 剰余算計算用(Byte/Sector)          */

	F_INT l_drtn;                       /* サブルーチン戻り値                 */
	F_WORD l_insctofst;                 /* 移動後セクタ内オフセット           */

	F_BYTE l_sctofst;                   /* 移動後セクタオフセット             */
	F_CHAR l_drvnum;                    /* 論理ドライブ番号                   */
	
	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_drvnum = tfmr_filehandle[ handlenum ].drivenum;  /* 論理ドライブ番号    */
	l_modspc = MFMR_SECTPERCLST( l_drvnum ) - 1;
	l_modbps = MFMR_BYTEPERSECT( l_drvnum ) - 1;
	if( tfmr_filehandle[ handlenum ].direntry.filesize == 0 )
	{
		return DFMR_OK;
	}
	/**************************************************************************/
	/* オフセット、初期位置より移動後のポインタ位置を取得                     */
	/**************************************************************************/
	switch( origin )
	{
	case DFMR_SEEK_SET:                 /* 先頭からの場合                     */
		l_work01 = offset;
		l_fptr = ( F_DWORD )offset;     /* 移動後ポインタ←オフセット         */
		break;
	case DFMR_SEEK_CUR:                 /* 現在値からの場合                   */
		l_work01 = offset + ( F_LONG )tfmr_filehandle[ handlenum ].fptr;
		l_fptr = ( F_DWORD )l_work01;   /* 移動後ポインタ←ポインタ+オフセット*/
		break;
	default: /* case DFMR_SEEK_END: */  /* 最終位置からの場合                 */
		l_work01 = offset
			+ ( F_LONG )tfmr_filehandle[ handlenum ].direntry.filesize;
		l_fptr = ( F_DWORD )l_work01;   /* 移動後ポインタ←サイズ+オフセット  */
		break;
	}
	
	if( ( l_work01 < 0 )
		|| ( tfmr_filehandle[ handlenum ].direntry.filesize < ( F_DWORD )l_work01 ) )
	{                                   /* 移動後ポインタが0～ファイルサイズ  */
	                                    /* ではない                           */
		return DFMR_EINVAL;
	}
	
	/**************************************************************************/
	/* 位置検索用パラメータ設定                                               */
	/**************************************************************************/
	/* 移動後クラスタオフセット設定 */
	l_clstofst = l_fptr / ( MFMR_SECTPERCLST( l_drvnum ) * MFMR_BYTEPERSECT( l_drvnum ) );
	/* 移動後セクタオフセット設定 */
	l_sctofst = ( F_BYTE )( ( l_fptr /  MFMR_BYTEPERSECT( l_drvnum ) ) & l_modspc );
	l_work02 = l_fptr;
	l_insctofst = ( F_WORD )(l_work02 & l_modbps);

	if( ( l_fptr == tfmr_filehandle[ handlenum ].direntry.filesize )
		&& ( ( l_fptr % ( MFMR_SECTPERCLST( l_drvnum ) * MFMR_BYTEPERSECT( l_drvnum ) ) == 0 ) ) )
	{                                   /* クラスタ境界の場合は1セクタ分戻す */
		l_clstofst -= 1;
		l_sctofst = ( F_BYTE )l_modspc;
		l_insctofst = ( F_WORD )(MFMR_BYTEPERSECT( l_drvnum ));
	}

	/* 検索用クラスタ番号 */
	l_clstnum = ( F_DWORD )tfmr_filehandle[ handlenum ].direntry.fstclushi;
	l_clstnum <<= 16;
	l_clstnum |= ( F_DWORD )tfmr_filehandle[ handlenum ].direntry.fstcluslo;

	if ( origin == DFMR_SEEK_CUR )
	{
		l_work02 = tfmr_filehandle[ handlenum ].fptr;
		l_work03 = l_work02 / ( MFMR_SECTPERCLST( l_drvnum ) * MFMR_BYTEPERSECT( l_drvnum ) );
		if ( l_work02 % ( MFMR_SECTPERCLST( l_drvnum ) * MFMR_BYTEPERSECT( l_drvnum ) ) == 0 )
		{
			/* ファイルポインタの現在値がクラスタ境界の場合	*/
			l_work02 = tfmr_filehandle[ handlenum ].crtsectnum;
			l_work02 -= MFMR_DATATOP( l_drvnum );
			if ( (l_work03 != 0) &&
				 (tfmr_filehandle[ handlenum ].crtsectofst == ( F_WORD )(MFMR_BYTEPERSECT( l_drvnum ))) )
			{
				l_work03 -= 1;
			}
		}
		l_work02 = tfmr_filehandle[ handlenum ].crtsectnum;
		l_work02 -= MFMR_DATATOP( l_drvnum );
		l_work02 /= MFMR_SECTPERCLST( l_drvnum );

		if ( l_work03 == l_clstofst )
		{
			l_clstnum = l_work02;
			l_clstnum += 2;
			/**************************************************************************/
			/* FAT読込み領域1に対象ファイルのデータ部の先頭クラスタを                 */
			/* 含むFATセクタを読込む                                                  */
			/**************************************************************************/
			l_drtn = FMR_getFatSector(
				MFMR_DRVINFO( l_drvnum ),       /* 論理ドライブ構成情報               */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),    /* FAT読込み領域      */
				l_clstnum,                      /* クラスタ番号                       */
				&l_work03                       /* FATエントリ値                      */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
			l_work02 *= MFMR_SECTPERCLST( l_drvnum );
			l_work02 += MFMR_DATATOP( l_drvnum );
			l_work02 += ( F_DWORD )l_sctofst;
			/* ファイルハンドルの設定を行う */
			tfmr_filehandle[ handlenum ].fptr = l_fptr; /* ファイルポインタ設定       */
			tfmr_filehandle[ handlenum ].crtsectnum = l_work02;
	                                            /* カレントセクタ番号設定     */
			tfmr_filehandle[ handlenum ].crtsectofst = ( F_WORD )l_insctofst;
	                                            /* カレントセクタ内位置設定   */
			return DFMR_OK;
		}
		if ( l_fptr >= tfmr_filehandle[ handlenum ].fptr )
		{
			l_clstnum = l_work02;
			l_clstnum += 2;
			l_clstofst -= l_work03;
		}
	}
	
	/**************************************************************************/
	/* FAT読込み領域1に対象ファイルのデータ部の先頭クラスタを                 */
	/* 含むFATセクタを読込む                                                  */
	/**************************************************************************/
	l_drtn = FMR_getFatSector(
		MFMR_DRVINFO( l_drvnum ),       /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),    /* FAT読込み領域      */
		l_clstnum,                      /* クラスタ番号                       */
		&l_work02                       /* FATエントリ値                      */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	l_work02 = l_clstnum;
	/**************************************************************************/
	/* 検索を行う                                                             */
	/**************************************************************************/
	l_cnt01 = 0;
	while( l_cnt01 < l_clstofst )
	{
		/* クラスタオフセット分クラスタ番号を取得する */
		
		/* 取得したFATエントリが次のクラスタ番号となる */
		l_clstnum = l_work02;
		
		/* FAT読込み領域より対象のFATエントリを求める */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),    /* FAT読込み領域  */
			l_clstnum,                  /* クラスタ番号                       */
			&l_work02,                  /* FATエントリ値                      */
			DFMR_FLGOFF                 /* エントリ取得                       */
			);
		if( l_drtn != DFMR_OK )
		{
			if( l_drtn == DFMR_NOFATENTRY )
			{
				/* 対象クラスタ番号が読込んでいるFATセクタ外の場合 */
				/* FAT読込み領域に対象クラスタ番号を含むFATセクタを読込む */
				l_drtn = FMR_getFatSector(
					MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報       */
					MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),
					                            /* FAT読込み領域              */
					l_clstnum,                  /* クラスタ番号               */
					&l_work02                   /* FATエントリ値              */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
			}
			else
			{
				return l_drtn;
			}
		}
		l_cnt01 ++;
	}
	/********************************************************************************************/
	/* whileループ終了時に、取得したFATエントリが、読込んでいるFATセクタ外の場合があるので      */
	/* 再度、取得したFATエントリ(l_work02)をチェックし、FATセクタ外なら、当該FATセクタを読込む。*/
	/********************************************************************************************/
	l_drtn = FMR_controlFatEntry(
		MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報               */
		MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),    /* FAT読込み領域  */
		l_work02,                   /* クラスタ番号                       */
		&l_cnt01,                   /* FATエントリ値                      */
		DFMR_FLGOFF                 /* エントリ取得                       */
		);
	if( l_drtn != DFMR_OK )
	{
		if( l_drtn == DFMR_NOFATENTRY )
		{
			/* 対象クラスタ番号が読込んでいるFATセクタ外の場合 */
			/* FAT読込み領域に対象クラスタ番号を含むFATセクタを読込む */
			l_drtn = FMR_getFatSector(
				MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報       */
				MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),
				                            /* FAT読込み領域              */
				l_work02,                   /* クラスタ番号               */
				&l_cnt01                    /* FATエントリ値              */
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		else
		{
			return l_drtn;
		}
	}
	l_work02 -= 2;
	l_work02 *= MFMR_SECTPERCLST( l_drvnum );
	l_work02 += ( F_DWORD )l_sctofst;
	l_work02 += MFMR_DATATOP( l_drvnum );
	/* ファイルハンドルの設定を行う */
	tfmr_filehandle[ handlenum ].fptr = l_fptr; /* ファイルポインタ設定       */
	tfmr_filehandle[ handlenum ].crtsectnum = l_work02;
	                                            /* カレントセクタ番号設定     */
	tfmr_filehandle[ handlenum ].crtsectofst = ( F_WORD )l_insctofst;
	                                            /* カレントセクタ内位置設定   */
	return DFMR_OK;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
