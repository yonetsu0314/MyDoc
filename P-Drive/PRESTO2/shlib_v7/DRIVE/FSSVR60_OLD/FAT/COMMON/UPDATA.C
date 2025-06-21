/*======================================================================
 * File name    : updata.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/updata.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: updata.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2005/03/04 15:00
 * Saver60 Ver.01.05 up
 * 
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
/* 関 数 名：FMR_updateData                                                   */
/*         ：                                                                 */
/* パラメタ：IN:  F_INT ファイルハンドル番号                                  */
/*         ：     F_BYTE * 書込みデータ格納領域                               */
/*         ：     F_DWORD 書込みサイズ                                        */
/*         ：     F_DWORD 検索開始クラスタ番号                                */
/*         ：     F_DWORD 未更新最終クラスタ番号                              */
/*         ：                                                                 */
/*         ：I/O: F_DWORD * 新規取得先頭クラスタ番号                          */
/*         ：   IN: 上位で取得した空クラスタ番号,0なら空クラスタ未取得        */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK                                           */
/*         ：      異常終了 DFMR_FATALERR                                     */
/*         ：                                                                 */
/* 概    要：データ更新書込みを行う                                           */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.04 MFMR_FATRWBUFFマクロをMFMR_RWBUFFマクロへ変更   04/12/10 Y.Yoshizumi */
/*       MFMR_FATRWBUFFマクロをMFMR_RWBUFFTOPマクロへ変更04/12/10 Y.Yoshizumi */
/*       先端部収納範囲をセクタからRWバッファサイズへ変更05/02/09 Y.Yoshizumi */
/* 01.05 R/Wバッファサイズをユーザー定義値と             05/02/28 Y.Yoshizumi */
/*       セクタ数/クラスタを比較して決定する                                  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_updateData(
    F_INT handlenum,                    /* ファイルハンドル番号               */
    F_BYTE * databuff,                  /* 書込みデータ格納領域               */
    F_DWORD datasize,                   /* 書込みサイズ                       */
    F_DWORD srchstartclstnum,           /* 検索開始クラスタ番号               */
    F_DWORD fwdlnkclstnum,              /* 未更新最終クラスタ番号             */
    F_DWORD * newtopclstnum             /* 新規取得先頭クラスタ番号           */
)
{
	F_DWORD l_orgnewtopclstnum;         /* 起動時新規取得先頭クラスタ番号     */
	F_DWORD l_nowsectnum;               /* 現在アクセス中のセクタ番号         */
	F_DWORD l_orgsectnum;               /* 現在アクセス中の元セクタ番号       */
	F_DWORD l_srchstartclstnum;         /* 検索開始クラスタ番号               */
	
	F_DWORD l_topfracclstnum;           /* 先頭部端数クラスタ番号             */
	F_DWORD l_btmfatentry;              /* 書込み終了位置の後続クラスタ番号   */
	F_DWORD l_btmorgsectnum;            /* 書込み終了位置の元セクタ番号       */
	F_DWORD l_evacsectnum;              /* 開始時FAT読込み領域0退避用         */
	
	F_BYTE * l_writeptr;                /* 書込み開始位置                     */
	
	F_DWORD l_fwdsize;                  /* 先頭部端数サイズ                   */
	F_DWORD l_mdlsect;                  /* 連続書込みセクタ数                 */
	F_DWORD l_bcksize;                  /* 終端部端数サイズ                   */
	
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD l_work02;
	F_DWORD l_work03;
	F_DWORD l_work04;
	
	F_DWORD l_newtopclstnum;            /* 新規取得先頭クラスタ番号           */
	F_DWORD l_newbtmclstnum;            /* 新規取得最終クラスタ番号           */
	
	F_DWORD l_modbps;                   /* 剰余算計算用(Byte/Sector)          */
	F_DWORD l_modspc;                   /* 剰余算計算用(Sector/Cluster)       */

	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_drtn;                       /* ドライバエントリ戻り値             */

	F_BYTE l_drvnum;                    /* 論理ドライブ番号                   */
	F_BYTE l_fatareanum;                /* 現在参照中のFAT読込み領域番号      */
	F_BYTE l_workfatareanum;            /* ワークFAT読込み領域番号            */

	/**************************************************************************/
	/* 初期設定を行う                                                         */
	/**************************************************************************/
	l_orgnewtopclstnum = *( newtopclstnum );
	*( newtopclstnum ) = 0;

	l_drvnum = tfmr_filehandle[ handlenum ].drivenum;
	l_modbps = MFMR_BYTEPERSECT( l_drvnum ) - 1;
	l_modspc = MFMR_SECTPERCLST( l_drvnum ) - 1;
	l_fatareanum = DFMR_REFFATAREA0;
	l_evacsectnum = 0;
	l_topfracclstnum = 0;
	l_srchstartclstnum = srchstartclstnum;
	l_writeptr = databuff;
	l_newtopclstnum = 0;
	l_newbtmclstnum = 0;
	/************************************************************/
	/* 書込みサイズを先頭部端数/連続書込み部/終端部端数に分ける */
	/************************************************************/
	l_work01 = tfmr_filehandle[ handlenum ].fptr;
	if( DFMR_RWBUFFSIZE > MFMR_SECTPERCLST( l_drvnum ) )
	{
		/* R/Wバッファサイズがクラスタ当りのセクタ数を超える場合 */
		l_work02 = ( MFMR_BYTEPERSECT( l_drvnum ) * MFMR_SECTPERCLST( l_drvnum ) );
	}
	else
	{
		/* R/Wバッファサイズがクラスタ当りのセクタ数を超えない場合 */
		l_work02 = ( MFMR_BYTEPERSECT( l_drvnum ) * DFMR_RWBUFFSIZE );
	}
	l_work01 /= l_work02;
	l_work01 *= l_work02;
	l_work01 += l_work02;
	l_work01 -= tfmr_filehandle[ handlenum ].fptr;
	if( l_work01 >= datasize )
	{
		/*******************************************/
		/* 書込みデータがR/Wバッファ内に収まる場合 */
		/*******************************************/
		l_fwdsize = datasize;           /* 先頭部端数:全サイズ                */
		l_mdlsect = 0;                  /* 連続書込み部:なし                  */
		l_bcksize = 0;                  /* 終端部端数:なし                    */
	}
	else
	{
		/*********************************************/
		/* 書込みデータがR/Wバッファに収まらない場合 */
		/*********************************************/
		if( ( tfmr_filehandle[ handlenum ].fptr & l_modbps )
			== 0 )
		{
			/************************************/
			/* 書込み開始位置がセクタ先頭の場合 */
			/************************************/
			l_fwdsize = 0;              /* 先頭部はなし                       */
			l_mdlsect = datasize / MFMR_BYTEPERSECT( l_drvnum );
			                            /* 連続書込み部                       */
			l_bcksize = datasize & l_modbps;
			                            /* 終端部端数                         */
		}
		else
		{
			/**********************************/
			/* 書込み開始位置がセクタ先頭以外 */
			/**********************************/
			l_work01 = tfmr_filehandle[ handlenum ].fptr;
			l_work01 /= MFMR_BYTEPERSECT( l_drvnum );
			l_work01 *= MFMR_BYTEPERSECT( l_drvnum );
			l_work01 += MFMR_BYTEPERSECT( l_drvnum );
			l_work01 -= tfmr_filehandle[ handlenum ].fptr;
			l_fwdsize = l_work01;       /* 先頭部端数                         */
			l_work01 = datasize - l_work01;
			l_mdlsect = l_work01 / MFMR_BYTEPERSECT( l_drvnum );
			                            /* 連続書込み部                       */
			l_bcksize = l_work01 & l_modbps;
			                            /* 終端部端数                         */
		}
	}
	
	/* 書込み終了位置の後続クラスタ番号を求める */
	l_work01 = tfmr_filehandle[ handlenum ].fptr + datasize;
	if( l_work01 >= tfmr_filehandle[ handlenum ].direntry.filesize )
	{
		/**********************************************/
		/* 書込み後サイズが書込み前サイズを超える場合 */
		/**********************************************/
		MFMR_GETEOC( MFMR_FATTYPE( l_drvnum ), l_btmfatentry );
		                                /* 後続クラスタ番号はEOC              */
		l_btmorgsectnum = 0;            /* 書込み終了位置の元セクタ番号はなし */
	}
	else
	{
		/************************************************/
		/* 書込み後サイズが書込み前サイズを超えない場合 */
		/************************************************/
		l_work02 = (MFMR_BYTEPERSECT( l_drvnum ) * MFMR_SECTPERCLST( l_drvnum )) - 1;
		if ( (l_work01 & l_work02) == 0 )
		{
			/* クラスタ境界に書込み終了位置が位置する場合、１つ前にする。 */
			/* (FMR_setPointerでは、クラスタ境界の場合、次のセクタに位置させる為  */
			l_work04 = datasize;
			l_work04--;
		}
		else
		{
			l_work04 = datasize;
		}

		/* 現在のファイルポインタを保存 */
		l_work01 = tfmr_filehandle[ handlenum ].fptr;
		l_work02 = tfmr_filehandle[ handlenum ].crtsectnum;
		l_work03 = ( F_DWORD )tfmr_filehandle[ handlenum ].crtsectofst;
		
		/* ファイルポインタを書込み終了位置へ移動 */
		l_rtn = FMR_setPointer(
			handlenum,                  /* ファイルハンドル番号               */
			l_work04,                   /* オフセット                         */
			DFMR_SEEK_CUR               /* 初期位置                           */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		/* 書込み終了位置のセクタ番号を取得 */

		l_work04 = tfmr_filehandle[ handlenum ].crtsectnum;
		l_btmorgsectnum = l_work04;
		/* 書込み終了位置クラスタのFATエントリを取得 */
		l_work04 -= MFMR_DATATOP( l_drvnum );
		l_work04 /= MFMR_SECTPERCLST( l_drvnum );
		l_work04 += 2;                  /* 書込み終了位置のクラスタ番号       */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA1 ),
			                            /* FAT読込み領域アドレス              */
			                            /* (ポインタ移動後、必ず領域1に移動後 */
			                            /* カレントクラスタ番号を含むFATセクタ*/
			                            /* が読込まれている)                  */
			l_work04,                   /* クラスタ番号                       */
			&l_btmfatentry,             /* FATエントリ値                      */
			DFMR_FLGOFF                 /* Read/Write種別:Read                */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		/* ポインタを元に戻す */
		tfmr_filehandle[ handlenum ].fptr = l_work01;
		tfmr_filehandle[ handlenum ].crtsectnum = l_work02;
		tfmr_filehandle[ handlenum ].crtsectofst = ( F_WORD )l_work03;
	}
	/* 更新元先頭セクタを取得 */
	l_orgsectnum = tfmr_filehandle[ handlenum ].crtsectnum;
	
	/* 書込み先頭セクタのクラスタ内オフセットを求める */
	l_work03 = l_orgsectnum - MFMR_DATATOP( l_drvnum );
	l_work03 &= l_modspc;
	
	/**************************************************************************/
	/* 先頭部端数書込み処理                                                   */
	/**************************************************************************/
	if( ( l_work03 != 0 ) || ( l_fwdsize != 0 ) )
	{
		/************************************************************/
		/* 書込み開始位置がクラスタ先頭以外の場合                   */
		/************************************************************/
		if( l_orgnewtopclstnum == 0 )
		{
			/************************************************************/
			/* 上位で空クラスタ未取得の場合                             */
			/************************************************************/
			/* 空きクラスタ取得 */
			l_work01 = FMR_getEmptyCluster(
				l_drvnum,                   /* 対象論理ドライブ番号               */
				l_srchstartclstnum,           /* 検索開始クラスタ番号               */
				l_fatareanum                /* 検索FAT読込み領域( 0 )             */
				);
			if( l_work01 == DFMR_LNG )
			{
				/********************************************************/
				/* 空き領域が存在しない場合                             */
				/********************************************************/
				/* FAT0のセクタ番号を退避させる */
				l_evacsectnum = MFMR_FATSECTNUM( l_drvnum, DFMR_REFFATAREA0 );
				l_evacsectnum &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
				/* FATセクタ管理領域より空きクラスタを取得する */
				l_rtn = FMR_searchEmptyCluster(
					l_drvnum,               /* 対象論理ドライブ番号               */
					&l_work01               /* 取得空きクラスタ番号               */
					);
				if( l_rtn != DFMR_OK )
				{
					return l_rtn;
				}
				/* 取得した空きクラスタ番号を含むFATセクタを読込み領域0に読込む */
				l_drtn = FMR_getFatSector(
					MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報アドレス   */
					MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
					                            /* FAT読込み領域アドレス          */
					l_work01,                   /* クラスタ番号                   */
					&l_work02                   /* FATエントリ値                  */
					);
				if( l_drtn != DFMR_OK )
				{
					return l_drtn;
				}
			}
		}
		else
		{
			/************************************************************/
			/* 上位で空クラスタ取得済みの場合                           */
			/************************************************************/
			l_work01 = l_orgnewtopclstnum;
		}

		/* ここで取得したクラスタ番号を一旦新規取得の */
		/* 先頭/最終クラスタ番号とする                */
		l_newtopclstnum = l_work01;     /* 新規取得先頭クラスタ番号           */
		l_newbtmclstnum = l_work01;     /* 新規取得最終クラスタ番号           */
		/* 取得したクラスタ番号を新規取得クラスタ番号に設定する */
		*( newtopclstnum ) = l_work01;
		/* 先頭部端数クラスタ番号保存 */
		l_topfracclstnum = l_work01;
		/* 検索開始クラスタ番号を取得したクラスタ番号 + 1 とする */
		l_srchstartclstnum = l_work01 + 1;
		l_work01 -=2;
		l_work01 *= MFMR_SECTPERCLST( l_drvnum );
		l_work01 += MFMR_DATATOP( l_drvnum );
		l_nowsectnum = l_work01 + l_work03;
		
		/**********************************************************************/
		/* 書込み開始セクタより前の同クラスタ内セクタをコピーする             */
		/**********************************************************************/
		if( (l_mdlsect == 0) && (l_bcksize == 0) )
		{
			/* 書込みデータがR/Wバッファ内に収まる場合 */
			if( l_btmorgsectnum == 0 )
			{
				/* 書込み後サイズが書込み前サイズを超える場合 */
				l_work01 = DFMR_COPYSEC_CPYEND;
			}
			else
			{
				/* 書込み後サイズが書込み前サイズを超えない場合 */
				l_work01 = DFMR_COPYSEC_CPYALL;
			}
		}
		else
		{
			/* 書込みデータがR/Wバッファに収まらない場合 */
			l_work01 = DFMR_COPYSEC_FWDCPY;
		}
		l_rtn = FMR_copySector(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			l_work01,                   /* コピーモード                       */
			l_fwdsize,                  /* 書込みサイズ                       */
			handlenum,                  /* ファイルハンドル                   */
			l_orgsectnum,               /* コピー元セクタ番号                 */
			l_nowsectnum,               /* コピー先セクタ番号                 */
			l_writeptr                  /* 書込み開始位置                     */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		
		/**********************************************************************/
		/* 先頭部端数書込み処理                                               */
		/**********************************************************************/
		if( l_fwdsize != 0 )
		{
			l_writeptr += l_fwdsize;    /* 先頭部端数のサイズを更新           */
			
			/* 次に書込むセクタを設定する */
			if( l_work03 == ( F_DWORD )( MFMR_SECTPERCLST( l_drvnum ) - 1 ) )
			{
				/**************************************/
				/* 書込んだセクタがクラスタ終端の場合 */
				/**************************************/
				l_nowsectnum = 0;       /* 現在アクセス中のセクタは未定       */
			}
			else
			{
				/******************************************/
				/* 書込んだセクタがクラスタ終端でない場合 */
				/******************************************/
				if ( (l_mdlsect != 0) ||
					 (l_bcksize != 0) )
				{
					/* 連続書込みまたは終端部端数書込みがある場合 */
					l_nowsectnum ++;        /* 現在アクセス中のセクタを更新       */
				}
				else
				{
					l_nowsectnum = 0;       /* 現在アクセス中のセクタは未定       */
				}
			}
		}
	}
	else
	{
		/************************************************************/
		/* 書込み開始位置がクラスタ先頭の場合                       */
		/************************************************************/
		if( l_orgnewtopclstnum == 0 )
		{
			/************************************************************/
			/* 上位で空クラスタ未取得の場合                             */
			/************************************************************/
			l_nowsectnum = 0;           /* 現在アクセス中のセクタは未定       */
		}
		else
		{
			/************************************************************/
			/* 上位で空クラスタ取得済みの場合                           */
			/************************************************************/
			l_nowsectnum = ( l_orgnewtopclstnum - 2 ) * MFMR_SECTPERCLST( l_drvnum );
			l_nowsectnum += MFMR_DATATOP( l_drvnum );
		}
	}
	
	/**************************************************************************/
	/* 連続書込み処理                                                         */
	/**************************************************************************/
	if( l_mdlsect != 0 )
	{
		/************************************************************/
		/* 連続書込み対象のセクタが存在する場合                     */
		/************************************************************/
		l_workfatareanum = l_fatareanum;
		/* 連続書込みを行う */
		l_rtn = FMR_writeDataSectors(
			l_drvnum,                   /* 対象論理ドライブ番号               */
			l_nowsectnum,               /* 先頭セクタ番号                     */
			l_writeptr,                 /* 書込みデータ格納領域               */
			l_mdlsect,                  /* 書込みセクタ数                     */
			l_srchstartclstnum,         /* 検索開始クラスタ番号               */
			l_workfatareanum,           /* FAT読込み領域番号                  */
			&l_newtopclstnum,           /* 新規取得先頭クラスタ番号           */
			&l_newbtmclstnum,           /* 新規取得最終クラスタ番号           */
			&l_fatareanum               /* 最終FAT読込み領域番号              */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
		if( *( newtopclstnum ) == 0 )   /* 新規取得先頭クラスタ番号が         */
		{                               /* 未定の場合                         */
			*( newtopclstnum ) = l_newtopclstnum;  /* 連続書込みで取得した    */
		}                                          /* 先頭クラスタ番号を設定  */
		/* 検索開始クラスタ番号更新 */
		l_srchstartclstnum = l_newbtmclstnum + 1;
		
		/* 書込み開始位置を更新 */
		l_writeptr += l_mdlsect * MFMR_BYTEPERSECT( l_drvnum );
		
		/* 現在アクセス中のセクタ番号を更新 */
		l_work02 = l_nowsectnum + l_mdlsect;
		if( l_nowsectnum != 0 )
		{
			l_work02 -= MFMR_DATATOP( l_drvnum );
		}
		l_work02 &= l_modspc;
		
		if( l_work02 == 0 )
		{
			/**************************************************/
			/* 書込み終了位置のセクタ番号がクラスタ先頭の場合 */
			/**************************************************/
			l_nowsectnum = 0;           /* 現在アクセス中のセクタは未定       */
		}
		else
		{
			/******************************************************/
			/* 書込み終了位置のセクタ番号がクラスタ先頭以外の場合 */
			/******************************************************/
			l_work04 = ( l_newbtmclstnum - 2 ) * MFMR_SECTPERCLST( l_drvnum );
			l_work04 += MFMR_DATATOP( l_drvnum );
			l_nowsectnum = l_work04 + l_work02;
		}
	}
	
	/*-- MEMO --***************************************************************/
	/* この時点で先頭部端数書込み/連続書込みのどちらか、又は双方が行われて    */
	/* いるため、以下のパラメータは必ず設定されている                         */
	/* (｢終端部書込みのみ｣/｢いずれの書込み処理も行わない｣はありえない)        */
	/*  l_newtopclstnum : 新規取得先頭クラスタ番号                            */
	/*  l_newbtmclstnum : 新規取得最終クラスタ番号                            */
	/* ※｢セクタ先頭から1セクタ未満｣は｢先頭部端数書込み｣に含まれる            */
	/**************************************************************************/
	
	/* 現在アクセス中の元セクタを書込み終了位置に設定 */
	l_orgsectnum = l_btmorgsectnum;
	
	/**************************************************************************/
	/* 終端部端数書込み処理                                                   */
	/**************************************************************************/
	if( l_bcksize != 0 )
	{
		/************************************************************/
		/* 終端部端数が存在する場合                                 */
		/************************************************************/
		if( l_orgsectnum != 0 )
		{
			/*********************************************************/
			/* 元データが存在する場合はR/Wバッファに元セクタを読込む */
			/*********************************************************/
			/* パラメータ設定 */
			l_drtn = FMR_readDataSector(
				l_drvnum,
				l_orgsectnum,
				0
				);
			if( l_drtn != DFMR_OK )
			{
				return l_drtn;
			}
		}
		else
		{
			/****************************************************/
			/* 元データが存在しない場合はR/Wバッファを0で埋める */
			/****************************************************/
			FMR_memSet(
				MFMR_RWBUFFTOP( l_drvnum, 0 ),  /* R/Wバッファ                */
				DFMR_INITBYTE,                  /* 初期値=0                   */
				MFMR_BYTEPERSECT( l_drvnum )    /* 1セクタ分のサイズ          */
				);
		}
		/* R/Wバッファの先頭から端数サイズ分のデータを書込む */
		FMR_memCopy(
			MFMR_RWBUFFTOP( l_drvnum, 0 ),      /* R/Wバッファ先頭            */
			l_writeptr,                         /* 書込み開始位置             */
			l_bcksize                           /* 終端部端数サイズ           */
			);
		l_work02 = l_newtopclstnum;
		l_drtn = FMR_writeEndData(
					&l_nowsectnum,      /* 書込み開始セクタ番号               */
					&l_newbtmclstnum,   /* 新規取得最終クラスタ番号           */
					&l_work02,          /* 新規取得先頭クラスタ番号           */
					&l_fatareanum,      /* 現在参照中のFAT読込み領域番号      */
					l_drvnum            /* 対象ドライブ番号                   */
					);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	
	/**************************************************************************/
	/* 書込み終了位置より後ろのセクタのコピーを行う                           */
	/* (書込み終了位置より後ろセクタが存在しない場合は内部で判断)             */
	/**************************************************************************/
	if( (l_orgsectnum != 0) &&
		(l_nowsectnum != 0) )       /* 書込み終了位置より後続のセクタが       */
	{                               /* ある場合のみ                           */
		l_rtn = FMR_copySector(
			l_drvnum,               /* 対象論理ドライブ番号                   */
			DFMR_COPYSEC_BCKCPY,    /* コピーモード                           */
			0,                      /* 書込みサイズ                           */
			0,                      /* ファイルハンドル                       */
			l_orgsectnum,           /* コピー元セクタ番号                     */
			l_nowsectnum,           /* コピー先セクタ番号                     */
			0                       /* 書込み開始位置                         */
			);
		if( l_rtn != DFMR_OK )
		{
			return l_rtn;
		}
	}
	/**************************************************************************/
	/* 新規取得最終クラスタ番号のFATエントリに後続クラスタ番号を設定する      */
	/**************************************************************************/
	l_drtn = FMR_controlFatEntry(
		MFMR_DRVINFO( l_drvnum ),       /* 論理ドライブ構成情報アドレス       */
		MFMR_FATTABLE( l_drvnum, l_fatareanum ),
		                                /* FAT読込み領域アドレス              */
		l_newbtmclstnum,                /* クラスタ番号                       */
		&l_btmfatentry,                 /* FATエントリ値                      */
		DFMR_FLGON                      /* Read/Write種別：Write指定          */
		);
	if( l_drtn != DFMR_OK )
	{
		return l_drtn;
	}
	
	if( l_fatareanum != DFMR_REFFATAREA0 )
	{
		/********************************************************/
		/* アクセス対象のFAT読込み領域が0以外の場合             */
		/********************************************************/
		/* FATを書込む                          */
		l_drtn = FMR_setFatSector(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( l_drvnum, l_fatareanum ),
			                            /* FAT読込み領域アドレス              */
			l_fatareanum                /* FAT読込み領域番号                  */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA1) = 0;

	/**************************************************************************/
	/* 新規先頭クラスタを前のクラスタに設定                                   */
	/**************************************************************************/
	if ( ( l_newtopclstnum != l_topfracclstnum ) && 
		 ( l_topfracclstnum != 0 ) && 
		 ( 0 != l_newtopclstnum ) )
	{
		/*********************************************/
		/* 先頭部端数書込み/連続書込みが行われた場合 */
		/*********************************************/
		/* 連続書込み先頭クラスタ番号を先頭部端数書込み */
		/* クラスタのFATエントリに設定する              *//* 領域は必ず0 */
		l_drtn = FMR_controlFatEntry(
			MFMR_DRVINFO( l_drvnum ),   /* 論理ドライブ構成情報アドレス       */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                            /* FAT読込み領域アドレス              */
			l_topfracclstnum,           /* クラスタ番号                       */
			&l_newtopclstnum,           /* FATエントリ値                      */
			DFMR_FLGON                  /* Read/Write種別：Write指定          */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}

	if( l_evacsectnum != 0 )
	{
		/*************************************************************/
		/* 未更新最終クラスタ番号を含むFATセクタが退避されている場合 */
		/*************************************************************/
		/* FAT読込み領域0を上書きする */
		l_drtn = FMR_setFatSector(
			MFMR_DRVINFO( l_drvnum ),    /* 論理ドライブ構成情報           */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                             /*FAT読込み領域                  */
			DFMR_REFFATAREA0             /* FAT読込み領域番号             */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
		/* 未更新最終クラスタ番号を含むFATセクタを読込む */
		l_drtn = FMR_getFatSector(
			MFMR_DRVINFO( l_drvnum ),    /* 論理ドライブ構成情報アドレス   */
			MFMR_FATTABLE( l_drvnum, DFMR_REFFATAREA0 ),
			                             /* FAT読込み領域アドレス          */
			fwdlnkclstnum,               /* クラスタ番号                   */
			&l_work01                    /* FATエントリ値                  */
			);
		if( l_drtn != DFMR_OK )
		{
			return l_drtn;
		}
	}
	return DFMR_SUC;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

