/*======================================================================
 * File name    : sfatsect.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/common/RCS/sfatsect.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: sfatsect.c $
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
/* 関 数 名：FMR_setFatSector                                                 */
/*         ：                                                                 */
/* パラメタ：IN: TFMR_LOGDRV_INFO * 論理ドライブ構成情報                      */
/*         ：    TFMR_FATBUFF * FAT読込み領域                                 */
/*         ：    F_BYTE  FAT読込み領域番号                                    */
/*         ：                                                                 */
/*         ：OUT: なし                                                        */
/*         ：                                                                 */
/*         ：                                                                 */
/* 戻 り 値：正常終了  DFMR_SUC                                               */
/*         ：異常終了  DFMR_FATALERR                                          */
/*         ：                                                                 */
/* 概    要：FATセクタ書込み                                                  */
/*         ：                                                                 */
/* 作 成 日：2004.06.09                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/******************************************************************************/

#include "fsbase.h"

F_INT FMR_setFatSector(
    TFMR_LOGDRV_INFO * logdrv_info,     /* 論理ドライブ構成情報               */
    TFMR_FATBUFF * fatbuff,             /* FAT読込み領域                      */
    F_BYTE  fatarea                     /* FAT読込み領域番号                  */
)
{
	F_DWORD l_sectnum;                  /* セクタ番号ワーク                   */
	F_DWORD l_work01;                   /* ワークエリア                       */
	F_DWORD * l_source;                 /* コピー元アドレス                   */
	F_DWORD * l_dest;                   /* コピー先アドレス                   */

	TFMR_DRVIOPARAM l_drvioparam;       /* I/Oパラメータ                      */

	F_INT l_drtn;                       /* ドライバエントリ戻り値             */
	F_INT l_rtn;                        /* サブルーチン戻り値                 */
	F_INT l_error;                      /* エラー戻り値                       */

	F_WORD l_targetsectnum;             /* 対象セクタ数                       */
	F_WORD l_cnt2;                      /* コピーループカウンタ               */

	F_BYTE l_cnt1;                      /* ループカウンタ                     */
	F_BYTE l_drvnum;                    /* 論理ドライブ番号                   */
	
	l_drvnum = logdrv_info->drvnum;
	SetBackupFlg(l_drvnum);
	if( logdrv_info->fattype != DFMR_FATTYPE12 )
	{
		/**********************************************************************/
		/* FAT16/32の場合                                                     */
		/**********************************************************************/
		for( l_cnt1 = 0; l_cnt1 < logdrv_info->fatcnt; l_cnt1 ++ )
		{
			l_drvioparam.write.startsctnum = fatbuff->fatsectnum;
			l_drvioparam.write.startsctnum += l_cnt1 * logdrv_info->sectperfat;
			l_targetsectnum = 1;
			l_drvioparam.write.writesctnum = &l_targetsectnum;
			l_drvioparam.write.writebuff = &( fatbuff->fat[ 0 ] );
			l_drtn = ( logdrv_info->drventry )(
				logdrv_info->channelunitnum,
				DFMR_DRVFUNCWRITE,
				&l_drvioparam
				);
			if( ( l_drtn != DFMR_OK ) || 
				( l_targetsectnum != 1 ) )
			{
				if( l_cnt1 == 0 )
				{
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
					}
					else
					{
						l_rtn = DFMR_FATALERR;
					}
					l_error = l_rtn;
					goto ERROR;
				}
			}
		}
	}
	else
	{
		/**********************************************************************/
		/* FAT12の場合                                                        */
		/**********************************************************************/
		for( l_cnt1 = 0; l_cnt1 < logdrv_info->fatcnt; l_cnt1 ++ )
		{
			l_sectnum = fatbuff->fatsectnum;
			l_sectnum &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
			l_drvioparam.write.startsctnum = l_sectnum + l_cnt1 * logdrv_info->sectperfat;
			l_targetsectnum = 1;
			l_drvioparam.write.writesctnum = &l_targetsectnum;
			l_drvioparam.write.writebuff = &( fatbuff->fat[ 4 ] );
			l_drtn = ( logdrv_info->drventry )(
				logdrv_info->channelunitnum,
				DFMR_DRVFUNCWRITE,
				&l_drvioparam
				);
			if( ( l_drtn != DFMR_OK ) || 
				( l_targetsectnum != 1 ) )
			{
				if( l_cnt1 == 0 )
				{
					if( l_drtn != DFMR_OK )
					{
						l_rtn = l_drtn;
					}
					else
					{
						l_rtn = DFMR_FATALERR;
					}
					l_error = l_rtn;
					goto ERROR;
				}
			}
			if( ( fatbuff->fatsectnum & DFMR_FWDSECTUPDATE ) == DFMR_FWDSECTUPDATE )
			{
				/************************************/
				/* 前セクタ更新フラグがONの場合		*/
				/************************************/
				if( l_cnt1 == 0 )
				{
					if ( fatarea == DFMR_REFFATAREA0 )
					{
						l_work01 = MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA1);
						l_source = (F_DWORD *)&(MFMR_FATSECTAREA(l_drvnum, DFMR_REFFATAREA1, 4));
					}
					else
					{
						l_work01 = MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA0);
						l_source = (F_DWORD *)&(MFMR_FATSECTAREA(l_drvnum, DFMR_REFFATAREA0, 4));
					}
					l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
					if ( l_work01 == l_sectnum - 1 )
					{
						/****************************************************/
						/* 別FAT読込領域に前FATセクタを読込んでいる場合     */
						/****************************************************/
						l_dest = (F_DWORD *)&( fatbuff->fat[ 4 ] );
						for(l_cnt2=0;l_cnt2<(logdrv_info->bytepersect/sizeof(F_DWORD));l_cnt2++)
						{
							*l_dest++ = *l_source++;	/* コピー					*/
					    }
					}
					else
					{
						l_drtn = FMR_readFatSector(
							logdrv_info,/* 論理ドライブ構成情報               */
							fatbuff,    /* FAT読込領域                        */
							l_sectnum - 1,/* 読込FATセクタ番号                */
							4           /* FAT読込領域オフセット              */
							);
						if( l_drtn != DFMR_OK )
						{
							l_error = l_drtn;
							goto ERROR;
						}
					}
					l_work01 = l_sectnum - MFMR_FATTOP( l_drvnum );
					if ( (l_work01 % 0x00000003) == 0x00000001 )
					{
						fatbuff->fat[ logdrv_info->bytepersect + 3 ] &= 0x0F;
						fatbuff->fat[ 3 ] &= 0xF0;
						fatbuff->fat[ logdrv_info->bytepersect + 3 ] |= fatbuff->fat[ 3 ];
					}
					else if ( (l_work01 % 0x00000003) == 0x00000002 )
					{
						fatbuff->fat[ logdrv_info->bytepersect + 3 ] = fatbuff->fat[ 3 ];
					}
				}
				else                    /* FAT1以外はFAT1の内容をコピーする   */
				{
					l_drvioparam.read.startsctnum = l_sectnum - 1;
					l_targetsectnum = 1;
					l_drvioparam.read.readsctnum = &l_targetsectnum;
					l_drvioparam.read.readbuff = &( fatbuff->fat[ 4 ] );
					l_drtn = ( logdrv_info->drventry )(
						logdrv_info->channelunitnum,
						DFMR_DRVFUNCREAD,
						&l_drvioparam
					);
					if( ( l_drtn != DFMR_OK ) || 
						( l_targetsectnum != 1 ) )
					{
						if( l_cnt1 == 0 )
						{
							if( l_drtn != DFMR_OK )
							{
								l_rtn = l_drtn;
							}
							else
							{
								l_rtn = DFMR_FATALERR;
							}
							l_error = l_rtn;
							goto ERROR;
						}
					}
				}
				l_drvioparam.write.startsctnum = l_sectnum - 1;
				l_drvioparam.write.startsctnum += l_cnt1 * logdrv_info->sectperfat;
				l_targetsectnum = 1;
				l_drvioparam.write.writesctnum = &l_targetsectnum;
				l_drvioparam.write.writebuff = &( fatbuff->fat[ 4 ] );
				l_drtn = ( logdrv_info->drventry )(
					logdrv_info->channelunitnum,
					DFMR_DRVFUNCWRITE,
					&l_drvioparam
					);
				if( ( l_drtn != DFMR_OK ) || 
					( l_targetsectnum != 1 ) )
				{
					if( l_cnt1 == 0 )
					{
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
						}
						else
						{
							l_rtn = DFMR_FATALERR;
						}
						l_error = l_rtn;
						goto ERROR;
					}
				}
				if( l_cnt1 >= ( logdrv_info->fatcnt - 1 ) )
				{
					break;
				}
				/* 対象FATセクタを読込む */
				l_drvioparam.read.startsctnum = l_sectnum;
				l_targetsectnum = 1;
				l_drvioparam.read.readsctnum = &l_targetsectnum;
				l_drvioparam.read.readbuff = &( fatbuff->fat[ 4 ] );
				l_drtn = ( logdrv_info->drventry )(
					logdrv_info->channelunitnum,
					DFMR_DRVFUNCREAD,
					&l_drvioparam
					);
				if( ( l_drtn != DFMR_OK ) || 
					( l_targetsectnum != 1 ) )
				{
					if( l_cnt1 == 0 )
					{
						if( l_drtn != DFMR_OK )
						{
							l_rtn = l_drtn;
						}
						else
						{
							l_rtn = DFMR_FATALERR;
						}
						l_error = l_rtn;
						goto ERROR;
					}
				}
			}
		}
		if( ( fatbuff->fatsectnum & DFMR_FWDSECTUPDATE ) == DFMR_FWDSECTUPDATE )
		{
			/************************************/
			/* 前セクタ更新フラグがONの場合		*/
			/************************************/
			if ( fatarea == DFMR_REFFATAREA0 )
			{
				l_work01 = MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA1);
				l_dest = (F_DWORD *)&(MFMR_FATSECTAREA(l_drvnum, DFMR_REFFATAREA1, 4));
			}
			else
			{
				l_work01 = MFMR_FATSECTNUM(l_drvnum, DFMR_REFFATAREA0);
				l_dest = (F_DWORD *)&(MFMR_FATSECTAREA(l_drvnum, DFMR_REFFATAREA0, 4));
			}
			l_work01 &= ~(DFMR_FWDSECTUPDATE + DFMR_FWDSECTREAD);
										/* 前セクタ読込み・更新フラグOFF      */
			if ( l_work01 == l_sectnum - 1 )
			{
				/****************************************************/
				/* 別FAT読込領域にも前FATセクタを読込んでいる場合   */
				/****************************************************/
				l_source = (F_DWORD *)&( fatbuff->fat[ 4 ] );
				for(l_cnt2=0;l_cnt2<(logdrv_info->bytepersect/sizeof(F_DWORD));l_cnt2++)
				{
					*l_dest++ = *l_source++;	/* コピー					*/
			    }
			}
			l_drtn = FMR_readFatSector(
				logdrv_info,            /* 論理ドライブ構成情報               */
				fatbuff,                /* FAT読込領域                        */
				l_sectnum,              /* 読込FATセクタ番号                  */
				4                       /* FAT読込領域オフセット              */
				);
			if( l_drtn != DFMR_OK )
			{
				l_error = l_drtn;
				goto ERROR;
			}
		}
		fatbuff->fatsectnum &= ~DFMR_FWDSECTUPDATE;	/* 前セクタ更新フラグOFF */
	}
	ClearBackupFlg(l_drvnum);
	return DFMR_OK;

ERROR:
	/* 異常終了 */
	ClearBackupFlg(l_drvnum);
	return l_error;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

