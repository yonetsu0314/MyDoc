/*======================================================================
 * File name    : fsdrvdef.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/fssvr60/RCS/fsdrvdef.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: fsdrvdef.h $
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
/* ファイル名：FMR_DRVDef.h                                                   */
/* 概    要  ：定数、構造体、プロタイプ定義                                   */
/*           ：                                                               */
/* 作 成 日  ：2004.03.19                                                     */
/* 作 成 者  ：                                                               */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/******************************************************************************/

#ifndef __FMR_DRVDef_H__
#define __FMR_DRVDef_H__

/************************************/
/*	定数値　定義	 				*/
/************************************/

/****** エラーコード ***************/

#define DFMR_DRV_OK				(  0 )			/* 正常終了					*/
#define DFMR_DRVERR_PARAM		( -512 )		/* パラメタエラー			*/
#define DFMR_DRVERR_NOTINIT		( -513 )		/* 未初期化エラー			*/
#define DFMR_DRVERR_NOCARD		( -514 )		/* カード抜きエラー			*/
#define DFMR_DRVERR_ILGALVS		( -515 )		/* VS1,2状態エラー			*/
#define DFMR_DRVERR_MRSHPC		( -516 )		/* MR-SHPC-01異常			*/
#define DFMR_DRVERR_PCCIS		( -517 )		/* PCカード CIS異常			*/
#define DFMR_DRVERR_PCREADYTMO	( -518 )		/* PCカードReady TimeOut	*/
#define DFMR_DRVERR_PCCARD		( -519 )		/* PCカード異常				*/
#define DFMR_DRVERR_TSLP		( -520 )		/* PCATA_osSleep異常終了	*/
#define DFMR_DRVERR_UNITOVER	( -521 )		/* 該当ユニット無し			*/
#define DFMR_DRVERR_NOTMOUNT	( -522 )		/* 未マウントエラー			*/
#define DFMR_DRVERR_DEFCARD		( -523 )		/* 別カード挿入				*/
#define DFMR_DRVERR_ILGCARD		( -524 )		/* カード挿抜発生			*/


/****** ドライバID ***************/

#define DFMR_DRVID_PCATA	0x01				/* PCATAドライバ			*/
#define DFMR_DRVID_IDE		0x02				/* IDEドライバ				*/
#define DFMR_DRVID_RAM		0x03				/* RAMディスクドライバ		*/

/****** メディアタイプ ***************/

#define DFMR_DRV_RMEDIA		0xf0				/* removable media			*/
#define DFMR_DRV_FMEDIA		0xf8				/* fixed media				*/

#endif /* __FMR_DRVDef_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/


