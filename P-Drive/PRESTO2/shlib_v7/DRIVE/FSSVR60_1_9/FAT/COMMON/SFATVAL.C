/*======================================================================
 * File name    : sfatval.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/rcs/sfatval.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sfatval.c $
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
/* 関 数 名：FMR_setFatValue                                                  */
/*         ：                                                                 */
/* パラメタ：IN: F_BYTE * FATバッファアドレス                                 */
/*         ：    F_BYTE FATタイプ                                             */
/*         ：    F_DWORD クラスタ番号                                         */
/*         ：    F_DWORD FATエントリ値                                        */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：VOID                                                             */
/*         ：                                                                 */
/* 概    要：FATエントリ値設定                                                */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_VOID FMR_setFatValue(
    F_BYTE * fatbuff,                   /* FATバッファアドレス                */
    F_BYTE fattype,                     /* FATタイプ                          */
    F_DWORD clstnum,                    /* クラスタ番号                       */
    F_DWORD fatentry                    /* FATエントリ値                      */
)
{
	T_ENDIAN l_endian;                  /* エンディアン調整用ワーク           */
	F_DWORD l_dentry;                   /* 32bit FATエントリ値                */
	F_WORD l_wwork;                     /* 16bit ワークエリア                 */
	F_WORD l_wentry;                    /* 16bit FATエントリ値                */
	
	if( fattype == DFMR_FATTYPE32 )
	{
		/**********************************************************************/
		/* FAT32の場合                                                        */
		/**********************************************************************/
		l_dentry = fatentry;
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
		l_endian.byteval[0] = *( fatbuff + 3 );
		l_endian.byteval[1] = *( fatbuff + 2 );
		l_endian.byteval[2] = *( fatbuff + 1 );
		l_endian.byteval[3] = *( fatbuff );
#else /* DFMR_ENDIAN_BIG */             /* Little Endian                      */
		l_endian.dwordval = *( ( F_DWORD * )fatbuff );
#endif /* DFMR_ENDIAN_BIG */
		l_dentry &= 0x0FFFFFFF;
		l_endian.dwordval &= 0xF0000000;
		
		l_endian.dwordval |= l_dentry;
		
#ifdef DFMR_ENDIAN_BIG                  /* Big Endian                         */
		*( fatbuff ) = l_endian.byteval[3];
		*( fatbuff + 1 ) = l_endian.byteval[2];
		*( fatbuff + 2 ) = l_endian.byteval[1];
		*( fatbuff + 3 ) = l_endian.byteval[0];
#else /* DFMR_ENDIAN_BIG */             /* Little Endian                      */
		*( ( F_DWORD * )fatbuff ) = l_endian.dwordval ;
#endif /* DFMR_ENDIAN_BIG */
		return;
	}
	else
	{
		/**********************************************************************/
		/* FAT12/16の場合                                                     */
		/**********************************************************************/
		l_wentry = ( F_WORD )fatentry;
		
		if( fattype == DFMR_FATTYPE12 )
		{
			/******************************************************************/
			/* FAT12の場合                                                    */
			/******************************************************************/
			l_wwork = ( F_WORD )( *( fatbuff + 1 ) );
			l_wwork = ( l_wwork << 8 ) | ( F_WORD )( *( fatbuff ) );
			
			if( ( clstnum & 0x0001 ) == 0x0001 )
			{
				l_wentry <<= 4;
				l_wwork &= 0x000F;
			}
			else
			{
				l_wentry &= 0x0FFF;
				l_wwork &= 0xF000;
				
			}
			l_wwork |= l_wentry;
		}
		else
		{
			l_wwork = ( F_WORD )( fatentry );
		}
		*( fatbuff ) = ( F_BYTE )( l_wwork & 0x00FF );
		*( fatbuff + 1 ) = ( F_BYTE )( ( l_wwork ) >> 8 );
		return;
	}
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
