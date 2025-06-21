/*======================================================================
 * File name    : ram_main.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/ramdisk/RCS/ram_main.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ram_main.c $
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
/****************************************************************************/
/* ファイル名：RAM_main.c                                                   */
/* 概    要  ：RAMディスクドライバ                                          */
/*           ：API I/F部                                                    */
/* 作 成 日  ：2004.07.13                                                   */
/* 作 成 者  ：                                                             */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
/*  01.00 First Release									2004.07.13			*/
/****************************************************************************/

#include "ram_def.h"
#include "ram_hw.h"
#include "ram_srct.h"
#include "ram_prot.h"

/****************************************************************************/
/* 関 数 名：RAM_main                                                       */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     F_BYTE        func_code     I/Oファンクション番号         */
/*         ：     T_IO_PARAM *  io_param      I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：API I/F エントリ                                               */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_main(	F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					F_BYTE		func_code,				/* I/Oファンクション番号	*/
					T_IO_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/

/* I/Oファンクション番号による振分け */
	switch (func_code) {
		case IO_INIT:									/* 初期化					*/
			ret = RAM_Init(chanel_unitno,(T_INIT_PARAM *)io_param);
			break;
		case IO_MOUNT:									/* マウント					*/
			ret = RAM_Mount(chanel_unitno,(T_MOUNT_PARAM *)io_param);
			break;
		case IO_UNMOUNT:								/* アンマウント				*/
			ret = RAM_Unmount(chanel_unitno,(T_UNMOUNT_PARAM *)io_param);
			break;
		case IO_CHANEL_INFO:							/* チャネル情報取得			*/
			ret = RAM_ChanelInfo(chanel_unitno,(T_CHANEL_INFO_PARAM *)io_param);
			break;
		case IO_DEVICE_INFO:							/* デバイス情報取得			*/
			ret = RAM_DeviceInfo(chanel_unitno,(T_DEVICE_INFO_PARAM *)io_param);
			break;
		case IO_UNIT_INFO:								/* ユニット情報取得			*/
			ret = RAM_UnitInfo(chanel_unitno,(T_UNIT_INFO_PARAM *)io_param);
			break;
		case IO_READ:									/* データ読込み				*/
			ret = RAM_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_WRITE:									/* データ書込み				*/
			ret = RAM_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_ERASE:									/* データ消去				*/
			ret = RAM_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_DETECT_INFO:							/* メディア挿抜チェック		*/
			ret = RAM_DetectInfo(chanel_unitno,(T_DETECT_INFO_PARAM *)io_param);
			break;
		case IO_PART_CREATE:							/* パーティション作成		*/
			ret = RAM_PartCreate(chanel_unitno,(T_PART_CREATE_PARAM *)io_param);
			break;
		default:
			ret = ERR_PARAM;
	}

	return (ret);

}

/****************************************************************************/
/* 関 数 名：RAM_Init                                                       */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_INIT_PARAM *  io_param    I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：初期化処理                                                     */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_Init(	F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					T_INIT_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	F_WORD		cnt;		/* ループカウンタ	*/

/* グローバル変数クリア */

	RAM_SubMemSet((F_BYTE *)&RAM_drv_inf, 0x00, sizeof(T_DRIVER_INFO));

	for ( cnt = 0; cnt < MAX_CHANEL; cnt++ )
	{
		RAM_SubMemSet((F_BYTE *)&RAM_ch_inf[cnt], 0x00, sizeof(T_CHANEL_INFO));
		RAM_ch_inf[cnt].memory_base = (F_BYTE *)RAM_START;	/* RAMディスク開始メモリアドレス設定*/
		RAM_ch_inf[cnt].memory_size = RAM_SIZE;	/* RAMディスクサイズ設定		*/
		RAM_ch_inf[cnt].device_cnt = 1;			/* 接続デバイス数は1固定		*/
		RAM_ch_inf[cnt].unit_cnt = 1;				/* 接続ユニット数は1固定		*/
		RAM_ch_inf[cnt].chanel_status |= CS_INIT;	/* チャネル状態←初期化済み		*/
	}

	for ( cnt = 0; cnt < MAX_DEVICE; cnt++ )
	{
		RAM_SubMemSet((F_BYTE *)&RAM_dv_inf[cnt], 0x00, sizeof(T_DEVICE_INFO));
		RAM_dv_inf[cnt].unit_cnt = 1;				/* 接続ユニット数は1固定		*/
		RAM_dv_inf[cnt].cnct_chanel = 0;			/* 接続チャネル番号は0固定		*/
	}

	for ( cnt = 0; cnt < MAX_UNIT; cnt++ )
	{
		RAM_SubMemSet((F_BYTE *)&RAM_un_inf[cnt], 0x00, sizeof(T_UNIT_INFO));
	}

	return ( DRV_OK );
}


/****************************************************************************/
/* 関 数 名：RAM_Mount                                                      */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_MOUNT_PARAM *  io_param   I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：マウント処理                                                   */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_Mount(F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					T_MOUNT_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	F_WORD		cnt;		/* ループカウンタ	*/
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		unit_cnt;	/* ユニット数	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

	deviceno = 0;									/* デバイス番号は0固定		*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= MAX_UNIT ) {
		return ( ERR_PARAM );
	}

	if ( io_param->mbr_mode > MOPAR_MBR_NON ) {
		return ( ERR_PARAM );
	}

/* 初期化済みチェック */

	if ( ((RAM_ch_inf[chanelno].chanel_status) & ~CS_CLEAR) != CS_INIT ) {
		return ( ERR_NOTINIT );
	}

/* 自タスクID取得 */

	RAM_osGetTID(&RAM_ch_inf[chanelno].task_id);

/* MBRモードならMBR読込み */

	if ( io_param->mbr_mode == MOPAR_MBR_EXIST ) {
		ret = RAM_ReadMBR(chanelno, deviceno, &unit_cnt);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
	}
	else
	{
/* MBRモード以外なら定数値設定 */
		RAM_ch_inf[chanelno].unit_cnt = 1;
		RAM_dv_inf[deviceno].unit_cnt = 1;
		RAM_un_inf[unitno].cnct_chanel = chanelno;
		RAM_un_inf[unitno].cnct_device = deviceno;
		RAM_un_inf[unitno].part_type = MBR_PT_FAT12;
		RAM_un_inf[unitno].sec_size = (SEC_SIZE >> 9);
		RAM_un_inf[unitno].total_sec_cnt = (RAM_SIZE >> 9);
		RAM_un_inf[unitno].start_sec_num = 0;
		unit_cnt = 1;
	}

/* チャネル状態、デバイス状態、ユニット状態←マウント済み,MBRモード設定				*/

	RAM_ch_inf[chanelno].chanel_status &= CS_CLEAR;
	RAM_ch_inf[chanelno].chanel_status |= CS_MOUNT;
	RAM_dv_inf[deviceno].device_status &= DS_CLEAR;
	RAM_dv_inf[deviceno].device_status |= DS_MOUNT;
	if ( io_param->mbr_mode == MOPAR_MBR_NON ) {
		RAM_dv_inf[deviceno].device_status |= DS_NONMBR;
	}
	else {
		RAM_dv_inf[deviceno].device_status &= ~DS_NONMBR;
	}
	if ( unitno >= unit_cnt ) {
		return ( ERR_UNITOVER );
	}

	RAM_un_inf[unitno].unit_status &= US_CLEAR;
	RAM_un_inf[unitno].unit_status |= US_MOUNT;
	RAM_un_inf[unitno].cnct_device = deviceno;

	io_param->part_type = RAM_un_inf[unitno].part_type;
	io_param->sec_size = RAM_un_inf[unitno].sec_size;
	io_param->total_sec_cnt = RAM_un_inf[unitno].total_sec_cnt;
	io_param->start_sec_num = RAM_un_inf[unitno].start_sec_num;

	return ( DRV_OK );
}

/****************************************************************************/
/* 関 数 名：RAM_Unmount                                                    */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_UNMOUNT_PARAM *  io_param I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：アンマウント処理                                               */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_Unmount(	F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_UNMOUNT_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= MAX_UNIT ) {
		return ( ERR_PARAM );
	}
	deviceno = RAM_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/


/* マウント済みチェック */

	if ( ((RAM_ch_inf[chanelno].chanel_status) & ~CS_CLEAR) != CS_MOUNT ) {
		return ( ERR_NOTINIT );
	}

/* チャネル状態、デバイス状態、ユニット状態←マウント済み,MBRモード設定				*/

	RAM_ch_inf[chanelno].unit_cnt = 1;				/* 接続ユニット数は1固定		*/
	RAM_ch_inf[chanelno].device_cnt = 1;			/* 接続デバイス数は1固定		*/
	RAM_ch_inf[chanelno].chanel_status &= CS_CLEAR;
	RAM_ch_inf[chanelno].chanel_status |= CS_INIT;

	RAM_dv_inf[deviceno].unit_cnt = 1;
	RAM_dv_inf[deviceno].cnct_chanel = 0;			/* 接続チャネル番号は0固定		*/
	RAM_dv_inf[deviceno].device_status &= DS_CLEAR;
	RAM_dv_inf[deviceno].device_status |= DS_DEFAULT;

	RAM_un_inf[unitno].unit_status &= US_CLEAR;
	RAM_un_inf[unitno].unit_status |= US_DEFAULT;
	RAM_un_inf[unitno].cnct_chanel = 0;
	RAM_un_inf[unitno].cnct_device = 0;
	RAM_un_inf[unitno].part_type = 0;
	RAM_un_inf[unitno].sec_size = 0;
	RAM_un_inf[unitno].total_sec_cnt = 0;
	RAM_un_inf[unitno].start_sec_num = 0;

	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_ChanelInfo                                                 */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_CHANEL_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：チャネル情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_ChanelInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_CHANEL_INFO_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{

	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_DeviceInfo                                                 */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_DEVICE_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：デバイス情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_DeviceInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_DEVICE_INFO_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= RAM_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	if ( chanelno != RAM_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}

	deviceno = RAM_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((RAM_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	io_param->sec_track = RAM_dv_inf[deviceno].sec_track;
	io_param->cylinder_cnt = RAM_dv_inf[deviceno].cylinder_cnt;
	io_param->head_cnt = RAM_dv_inf[deviceno].head_cnt;
	io_param->total_sec_cnt = RAM_dv_inf[deviceno].total_sec;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_UnitInfo                                                 */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_UNIT_INFO_PARAM *  io_param I/O パラメータアドレス      */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：ユニット情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_UnitInfo(	F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_UNIT_INFO_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= RAM_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	if ( chanelno != RAM_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}

	deviceno = RAM_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((RAM_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	io_param->part_type = RAM_un_inf[unitno].part_type;
	io_param->sec_size = RAM_un_inf[unitno].sec_size;
	io_param->unit_status = RAM_un_inf[unitno].unit_status;
	io_param->total_sec_cnt = RAM_un_inf[unitno].total_sec_cnt;
	io_param->start_sec_num = RAM_un_inf[unitno].start_sec_num;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_ReadWrite                                                */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     F_BYTE        func_code     I/Oファンクション番号         */
/*         ：     T_IO_PARAM *  io_param      I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：データ読込み・書込み                                           */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_ReadWrite(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						F_BYTE		func_code,			/* I/Oファンクション番号	*/
						T_IO_PARAM * io_param	)		/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	F_WORD		cnt;		/* ループカウンタ	*/
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		unit_cnt;	/* ユニット数	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= RAM_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	if ( chanelno != RAM_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}

	if ( *(io_param->read_param.sec_cnt) == 0 ) {
		return ( DRV_OK );
	}
	deviceno = RAM_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((RAM_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

/* 自タスクID取得 */

	RAM_osGetTID(&RAM_ch_inf[chanelno].task_id);

/* Read or Write Sector */

	if ( func_code == IO_READ ) {
		ret = RAM_ReadSector((T_READ_PARAM *)io_param, chanelno, deviceno, unitno);
	}
	else {
		ret = RAM_WriteSector(io_param, chanelno, deviceno, unitno, func_code);
	}
	if( ret != DRV_OK ) {
		if ( ((RAM_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_PULL ) {
			return ( ERR_NOCARD );
		}
		if ( ((RAM_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_INSERT ) {
			return ( ERR_ILGCARD );
		}
		return ( ret );
	}

	RAM_ch_inf[chanelno].task_id = 0;
	return DRV_OK;							/* 正常終了						*/

}

/****************************************************************************/
/* 関 数 名：RAM_DetectInfo                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_DETECT_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：メディア挿抜チェック                                           */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_DetectInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_DETECT_INFO_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		deviceno;	/* デバイス番号	*/

/* パラメータチェック */

	chanelno = chanel_unitno & ARG_CHANEL;
	chanelno >>= 4;
	unitno = chanel_unitno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}

	if ( unitno >= RAM_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	if ( chanelno != RAM_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}

	deviceno = RAM_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((RAM_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	io_param->detect_status = PC_MOUNT;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：RAM_PartCreate                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_PART_CREATE_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：パーティション作成                                             */
/*         ：                                                               */
/* 作 成 日：2004.07.13                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	RAM_PartCreate(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_PART_CREATE_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	return DRV_OK;
}

/******************************************************************************/
/*      Unpublished Copyright (C) 2003      ウェスコム株式会社                */
/******************************************************************************/
