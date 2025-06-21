/*======================================================================
 * File name    : ram_prot.h
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/ramdisk/RCS/ram_prot.h 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ram_prot.h $
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
/* ファイル名：RAM_Prot.h                                                   */
/* 概    要  ：プロタイプ宣言 & 変数領域宣言                                  */
/*           ：                                                               */
/* 作 成 日  ：2004.07.13                                                     */
/* 作 成 者  ：                                                               */
/*                                                                            */
/******************************************************************************/
/*** 変更履歴 *****************************************************************/
/*  履歴               変    更    内    容                日 付    修 正 者  */

/******************************************************************************/

#ifndef __RAM_PROT_H__
#define __RAM_PROT_H__

/************************************/
/*	プロトタイプ宣言 				*/
/************************************/

F_INT	RAM_main(			F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							F_BYTE					func_code,		/* I/Oファンクション番号	*/
							T_IO_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_Init(			F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_INIT_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_Mount(			F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_MOUNT_PARAM *			io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_Unmount(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_UNMOUNT_PARAM *		io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_ChanelInfo(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_CHANEL_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_DeviceInfo(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_DEVICE_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_UnitInfo(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_UNIT_INFO_PARAM *		io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_ReadWrite(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							F_BYTE					func_code,		/* I/Oファンクション番号	*/
							T_IO_PARAM * 			io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_DetectInfo(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_DETECT_INFO_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_PartCreate(		F_BYTE					chanel_unitno,	/* チャネル/ユニット番号	*/
							T_PART_CREATE_PARAM *	io_param	);	/* I/O パラメータアドレス	*/

F_INT	RAM_ReadMBR(		F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
							F_BYTE *				unit_cnt	);	/* ユニット数				*/

F_INT	RAM_ReadSector(		T_READ_PARAM *			io_param,		/* I/O パラメータアドレス	*/
						 	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
						 	F_BYTE					unitno		);	/* ユニット番号				*/

F_INT	RAM_WriteSector(	T_IO_PARAM *			io_param,		/* I/O パラメータアドレス	*/
						 	F_BYTE					chanelno,		/* チャネル番号				*/
							F_BYTE					deviceno,		/* デバイス番号				*/
						 	F_BYTE					unitno,			/* ユニット番号				*/
						 	F_BYTE					func_code	);	/* I/Oファンクション番号	*/

void	RAM_osGetTID(		F_DWORD *				tskid		);

void	RAM_SubMemSet(	F_BYTE *				dest,			/* 設定先ポインタ			*/
							F_BYTE					code,			/* 設定コード				*/
							F_WORD					count		);	/* 設定バイト数             */

void	RAM_SubMemCpy(	F_BYTE *				dest,			/* 設定先ポインタ			*/
							F_BYTE *				src,			/* 設定コード				*/
							F_WORD					count		);	/* 設定バイト数             */


/************************************/
/*	変数領域宣言	 				*/
/************************************/

/* ドライバデータ領域        */

extern 	T_DRIVER_INFO	RAM_drv_inf;

/* チャネルデータ領域        */

extern 	T_CHANEL_INFO	RAM_ch_inf[MAX_CHANEL];

/* デバイスデータ領域        */

extern 	T_DEVICE_INFO	RAM_dv_inf[MAX_DEVICE];

/* ユニットデータ領域        */

extern 	T_UNIT_INFO		RAM_un_inf[MAX_UNIT];

#endif /* __RAM_PROT_H__ */

/******************************************************************************/
/*      Unpublished Copyright (C) 2004      ウェスコム株式会社                */
/******************************************************************************/

