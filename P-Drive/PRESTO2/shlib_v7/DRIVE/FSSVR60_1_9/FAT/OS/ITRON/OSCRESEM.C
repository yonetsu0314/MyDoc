/*======================================================================
 * File name    : oscresem.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/fat/os/itron/rcs/oscresem.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 *$Log: oscresem.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2006/03/22 16:00
 * Saver60 Ver.01.09 up
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
/* 関 数 名：FMR_osAcreSem                                                    */
/*         ：                                                                 */
/* パラメタ：IN:  F_WORD 指定セマフォID(ID番号自動割付けの場合、NONE)         */
/*         ：                                                                 */
/* 戻 り 値：F_INT 正常終了 DFMR_OK(ID番号自動割付けの場合、生成したセマフォID*/
/*         ：      異常終了 DFMR_NG                                           */
/*         ：                                                                 */
/* 概    要：セマフォ資源を生成する                                           */
/*         ：                                                                 */
/* 前提条件：                                                                 */
/*         ：                                                                 */
/* 作 成 日：2004.05.26                                                       */
/* 作 成 者：T.Tsuneyoshi                                                     */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */
/* 01.07 ヘダーコメント修正                              05/12/12 Y.Yoshizumi */
/******************************************************************************/

#include "fsbase.h"
#include "os.h"

#ifdef DFMR_SEMID_AUTO
F_INT FMR_osAcreSem(
)
{
	F_INT l_rtn;                        /* OSシステムコール戻り値             */
	T_CSEM l_csem;                      /* セマフォ生成情報                   */
	
	l_csem.sematr = TA_TPRI;
	l_csem.isemcnt = 1;
	l_csem.maxsem = 1;
	l_rtn = acre_sem(
		&l_csem
		);
#else /* DFMR_SEMID_AUTO */
F_INT FMR_osAcreSem(
    F_WORD semid                        /* 指定セマフォID                     */
)
{
	F_INT l_rtn;                        /* OSシステムコール戻り値             */
	T_CSEM l_csem;                      /* セマフォ生成情報                   */
	
	l_csem.sematr = TA_TPRI;
	l_csem.isemcnt = 1;
	l_csem.maxsem = 1;
	l_rtn = cre_sem(
		semid,
		&l_csem
		);
#endif /* DFMR_SEMID_AUTO */
	switch( l_rtn )
	{
	case E_RSATR:
	case E_PAR:
	case E_ID:
	case E_OBJ:
	case E_NOID:
		l_rtn = DFMR_NG;
		break;
	default:
		break;
	}
	return l_rtn;
}
/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/
