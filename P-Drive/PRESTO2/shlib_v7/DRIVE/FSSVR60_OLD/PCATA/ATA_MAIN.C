/*======================================================================
 * File name    : ata_main.c
 * Original     :
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/ata_main.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 *$Log: ata_main.c $
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
/* ファイル名：PCATA_main.c                                                 */
/* 概    要  ：PCカードATAドライバ                                          */
/*           ：API I/F部                                                    */
/* 作 成 日  ：2004.03.19                                                   */
/* 作 成 者  ：                                                             */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/
/*  01.00 First Release									2004.03.19			*/
/****************************************************************************/

#include "ata_def.h"
#include "ata_srct.h"
#include "ata_prot.h"
#include "drive\atadrv.h"

/****************************************************************************/
/* 関 数 名：PCATA_main                                                     */
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
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_main(	F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					F_BYTE		func_code,				/* I/Oファンクション番号	*/
					T_IO_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/

/* I/Oファンクション番号による振分け */
	switch (func_code) {
		case IO_INIT:									/* 初期化					*/
			ret = PCATA_Init(chanel_unitno,(T_INIT_PARAM *)io_param);
			break;
		case IO_MOUNT:									/* マウント					*/
			ret = PCATA_Mount(chanel_unitno,(T_MOUNT_PARAM *)io_param);
			break;
		case IO_UNMOUNT:								/* アンマウント				*/
			ret = PCATA_Unmount(chanel_unitno,(T_UNMOUNT_PARAM *)io_param);
			break;
		case IO_CHANEL_INFO:							/* チャネル情報取得			*/
			ret = PCATA_ChanelInfo(chanel_unitno,(T_CHANEL_INFO_PARAM *)io_param);
			break;
		case IO_DEVICE_INFO:							/* デバイス情報取得			*/
			ret = PCATA_DeviceInfo(chanel_unitno,(T_DEVICE_INFO_PARAM *)io_param);
			break;
		case IO_UNIT_INFO:								/* ユニット情報取得			*/
			ret = PCATA_UnitInfo(chanel_unitno,(T_UNIT_INFO_PARAM *)io_param);
			break;
		case IO_READ:									/* データ読込み				*/
			ret = PCATA_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_WRITE:									/* データ書込み				*/
			ret = PCATA_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_ERASE:									/* データ消去				*/
			ret = PCATA_ReadWrite(chanel_unitno,func_code,io_param);
			break;
		case IO_DETECT_INFO:							/* メディア挿抜チェック		*/
			ret = PCATA_DetectInfo(chanel_unitno,(T_DETECT_INFO_PARAM *)io_param);
			break;
		case IO_PART_CREATE:							/* パーティション作成		*/
			ret = PCATA_PartCreate(chanel_unitno,(T_PART_CREATE_PARAM *)io_param);
			break;
		default:
			ret = ERR_PARAM;
	}

	return (ret);

}

/****************************************************************************/
/* 関 数 名：PCATA_Init                                                     */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_INIT_PARAM *  io_param    I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：初期化処理                                                     */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_Init(	F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					T_INIT_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	F_WORD		cnt;		/* ループカウンタ	*/

/* グローバル変数クリア */

	PCATA_SubMemSet((F_BYTE *)&PCATA_drv_inf, 0x00, sizeof(T_DRIVER_INFO));

	for ( cnt = 0; cnt < MAX_CHANEL; cnt++ )
	{
		PCATA_SubMemSet((F_BYTE *)&PCATA_ch_inf[cnt], 0x00, sizeof(T_CHANEL_INFO));
		PCATA_ch_inf[cnt].device_cnt = MAX_DEVICE;
		PCATA_ch_inf[cnt].chanel_status |= CS_INIT;	/* チャネル状態←初期化済み		*/
	}

	for ( cnt = 0; cnt < MAX_DEVICE; cnt++ )
	{
		PCATA_SubMemSet((F_BYTE *)&PCATA_dv_inf[cnt], 0x00, sizeof(T_DEVICE_INFO));
		PCATA_dv_inf[cnt].cnct_chanel = cnt;
	}

	for ( cnt = 0; cnt < MAX_UNIT; cnt++ )
	{
		PCATA_SubMemSet((F_BYTE *)&PCATA_un_inf[cnt], 0x00, sizeof(T_UNIT_INFO));
	}

	return ( DRV_OK );
}

/****************************************************************************/
/* 関 数 名：PCATA_Mount                                                    */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_MOUNT_PARAM *  io_param   I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：マウント処理                                                   */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_Mount(F_BYTE		chanel_unitno,			/* チャネル/ユニット番号	*/
					T_MOUNT_PARAM * io_param	)			/* I/O パラメータアドレス	*/
{
	F_INT		ret;		/* サブモジュール戻り値	*/
	T_MBR		mbrbuffer;	/* MBRバッファ			*/
	F_WORD		cnt;		/* ループカウンタ	*/
	F_BYTE		chanelno;	/* チャネル番号	*/
	F_BYTE		unitno;		/* ユニット番号	*/
	F_BYTE		unit_cnt;	/* ユニット数	*/

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

/* 初期化済みチェック&ユニットマウント済みチェック */

	if ( (((PCATA_ch_inf[chanelno].chanel_status) & ~CS_CLEAR) != CS_INIT ) &&
		 (((PCATA_ch_inf[chanelno].chanel_status) & ~CS_CLEAR) != CS_MOUNT ) ) {
		return ( ERR_NOTINIT );
	}
	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) == US_MOUNT ) {
		return ( ERR_MOUNT );
	}
	
/* カード無しなら異常終了 */

	if ( PCCard_check() ) {
		return ( ERR_NOCARD );
	}

/* 自タスクID取得 */

	PCATA_osGetTID(&PCATA_ch_inf[chanelno].task_id);

	for ( cnt = 0; cnt < PCATA_ch_inf[chanelno].device_cnt; cnt++ )
	{
		if ( ((PCATA_dv_inf[cnt].device_status) & ~DS_CLEAR) != DS_MOUNT ) {

			/* IDENTIFY情報読込み */

			ret = PCATA_ReadIDTFY(chanelno, cnt);
			if ( ret != DRV_OK ) {
				return ( ret );
			}

			/* MBRモードならPartitionTable読込み */

			if ( io_param->mbr_mode == MOPAR_MBR_EXIST ) {
				ret = PCATA_ReadPTable(chanelno, cnt, &unit_cnt, &mbrbuffer);
				if ( ret != DRV_OK ) {
					return ( ret );
				}
				PCATA_ch_inf[chanelno].unit_cnt += unit_cnt;	/* 接続ユニット数設定	*/
				PCATA_dv_inf[cnt].unit_cnt = unit_cnt;		/* 接続ユニット数設定	*/
			}
			else {
				PCATA_ch_inf[chanelno].unit_cnt += 1;		/* 接続ユニット数は1固定*/
				PCATA_dv_inf[cnt].unit_cnt = 1;		/* 接続ユニット数は1固定*/
			}

			/* チャネル状態、デバイス状態、ユニット状態←マウント済み,MBRモード設定	*/

			PCATA_ch_inf[chanelno].chanel_status &= CS_CLEAR;
			PCATA_ch_inf[chanelno].chanel_status |= CS_MOUNT;
			PCATA_dv_inf[cnt].device_status &= DS_CLEAR;
			PCATA_dv_inf[cnt].device_status |= DS_MOUNT;
			PCATA_dv_inf[cnt].device_status &= ~DS_WAIT_INTR;
			if ( io_param->mbr_mode == MOPAR_MBR_NON ) {
				PCATA_dv_inf[cnt].device_status |= DS_NONMBR;
			}
			else {
				PCATA_dv_inf[cnt].device_status &= ~DS_NONMBR;
			}
		}
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_UNITOVER );
	}
	if ( PCATA_un_inf[unitno].part_type == MBR_PT_NONE ) {
		return ( ERR_UNITOVER );
	}

	PCATA_un_inf[unitno].unit_status &= US_CLEAR;
	PCATA_un_inf[unitno].unit_status |= US_MOUNT;
	unit_cnt = 0;
	for ( cnt = 0; cnt < PCATA_ch_inf[chanelno].device_cnt; cnt++ )
	{
		unit_cnt += PCATA_dv_inf[cnt].unit_cnt;
		if ( unitno < unit_cnt ) {
			PCATA_un_inf[unitno].cnct_device = cnt;
		}
	}

	io_param->part_type = PCATA_un_inf[unitno].part_type;
	io_param->sec_size = PCATA_un_inf[unitno].sec_size;
	io_param->total_sec_cnt = PCATA_un_inf[unitno].total_sec_cnt;
	io_param->start_sec_num = PCATA_un_inf[unitno].start_sec_num;

	return ( DRV_OK );
}

/****************************************************************************/
/* 関 数 名：PCATA_Unmount                                                  */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_UNMOUNT_PARAM *  io_param I/O パラメータアドレス        */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：アンマウント処理                                               */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_Unmount(	F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_UNMOUNT_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_WORD		cnt;		/* ループカウンタ	*/
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
	if ( chanelno != PCATA_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_UNITOVER );
	}

	deviceno = PCATA_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

/* カード無しなら異常終了 */

	if ( PCCard_check() ) {
		return ( ERR_NOCARD );
	}

/* チャネル状態、デバイス状態、ユニット状態←初期化済み				*/

	PCATA_un_inf[unitno].unit_status &= US_CLEAR;
	PCATA_un_inf[unitno].unit_status |= US_DEFAULT;
	PCATA_un_inf[unitno].cnct_chanel = 0;
	PCATA_un_inf[unitno].cnct_device = 0;
	PCATA_un_inf[unitno].part_type = 0;
	PCATA_un_inf[unitno].sec_size = 0;
	PCATA_un_inf[unitno].total_sec_cnt = 0;
	PCATA_un_inf[unitno].start_sec_num = 0;
	for ( cnt = 0; cnt < PCATA_dv_inf[deviceno].unit_cnt; cnt++ )
	{
		if ( ((PCATA_un_inf[cnt].unit_status) & ~US_CLEAR) == US_MOUNT ) {
			break;
		}
	}
	if ( cnt >= PCATA_dv_inf[deviceno].unit_cnt ) {
		PCATA_ch_inf[chanelno].unit_cnt -= PCATA_dv_inf[deviceno].unit_cnt;
		if ( PCATA_ch_inf[chanelno].unit_cnt <= 0 ) {
			PCATA_ch_inf[chanelno].chanel_status &= CS_CLEAR;
			PCATA_ch_inf[chanelno].chanel_status |= CS_INIT;
		}

		PCATA_dv_inf[deviceno].device_status &= DS_CLEAR;
		PCATA_dv_inf[deviceno].device_status |= DS_DEFAULT;
		PCATA_dv_inf[deviceno].device_status &= ~DS_WAIT_INTR;
		PCATA_dv_inf[deviceno].unit_cnt = 0;
	}
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_ChanelInfo                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_CHANEL_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：チャネル情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_ChanelInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
						T_CHANEL_INFO_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_DeviceInfo                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_DEVICE_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：デバイス情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_DeviceInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
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
	if ( chanelno != PCATA_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_UNITOVER );
	}

	deviceno = PCATA_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	io_param->sec_track = PCATA_dv_inf[deviceno].sec_track;
	io_param->cylinder_cnt = PCATA_dv_inf[deviceno].cylinder_cnt;
	io_param->head_cnt = PCATA_dv_inf[deviceno].head_cnt;
	io_param->total_sec_cnt = PCATA_dv_inf[deviceno].total_sec;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_UnitInfo                                                 */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_UNIT_INFO_PARAM *  io_param I/O パラメータアドレス      */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：ユニット情報取得                                               */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_UnitInfo(	F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
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
	if ( chanelno != PCATA_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	deviceno = PCATA_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	io_param->part_type = PCATA_un_inf[unitno].part_type;
	io_param->sec_size = PCATA_un_inf[unitno].sec_size;
	io_param->unit_status = PCATA_un_inf[unitno].unit_status;
	io_param->total_sec_cnt = PCATA_un_inf[unitno].total_sec_cnt;
	io_param->start_sec_num = PCATA_un_inf[unitno].start_sec_num;
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_ReadWrite                                                */
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
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_ReadWrite(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
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
	if ( chanelno != PCATA_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}
	if ( *(io_param->read_param.sec_cnt) == 0 ) {
		return ( DRV_OK );
	}

	deviceno = PCATA_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

/* カード無しor別カード挿入なら異常終了 */

	if ( ((PCATA_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_PULL ) {
		return ( ERR_NOCARD );
	}
	if ( ((PCATA_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_INSERT ) {
		return ( ERR_ILGCARD );
	}

/* 自タスクID取得 */

	PCATA_osGetTID(&PCATA_ch_inf[chanelno].task_id);

/* Read or Write Sector */

	if ( func_code == IO_READ ) {
		ret = PCATA_ReadSector((T_READ_PARAM *)io_param, PCREADY_TIMEOUT, chanelno, deviceno, unitno);
	}
	else {
		ret = PCATA_WriteSector(io_param, PCREADY_TIMEOUT, chanelno, deviceno, unitno, func_code);
	}
	if( ret != DRV_OK ) {
		if ( ((PCATA_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_PULL ) {
			return ( ERR_NOCARD );
		}
		if ( ((PCATA_dv_inf[deviceno].device_status) & ~DS_CLEAR) == DS_INSERT ) {
			return ( ERR_ILGCARD );
		}
		return ( ret );
	}

	PCATA_ch_inf[chanelno].task_id = 0;
	return DRV_OK;							/* 正常終了						*/

}

/****************************************************************************/
/* 関 数 名：PCATA_DetectInfo                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_unitno チャネル/ユニット番号         */
/*         ：     T_DETECT_INFO_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：メディア挿抜チェック                                           */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_DetectInfo(F_BYTE		chanel_unitno,		/* チャネル/ユニット番号	*/
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
	if ( chanelno != PCATA_un_inf[unitno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( unitno >= PCATA_ch_inf[chanelno].unit_cnt ) {
		return ( ERR_PARAM );
	}

	deviceno = PCATA_un_inf[unitno].cnct_device;		/* デバイス番号設定			*/

/* マウント済みチェック */

	if ( ((PCATA_un_inf[unitno].unit_status) & ~US_CLEAR) != US_MOUNT ) {
		return ( ERR_NOTMOUNT );
	}

	if ( (PCATA_dv_inf[deviceno].device_status & ~DS_CLEAR) == DS_MOUNT ) {
		io_param->detect_status = PC_MOUNT;
	} else if ( (PCATA_dv_inf[deviceno].device_status & ~DS_CLEAR) == DS_PULL ) {
		io_param->detect_status = PC_NOTMOUNT;
	} else {
		io_param->detect_status = PC_DEFCARD;
	}
	return DRV_OK;
}

/****************************************************************************/
/* 関 数 名：PCATA_PartCreate                                               */
/*         ：                                                               */
/* パラメタ：IN   F_BYTE        chanel_deviceno チャネル/デバイス番号       */
/*         ：     T_PART_CREATE_PARAM *  io_param I/O パラメータアドレス    */
/*         ：                                                               */
/* 戻 り 値：0     ：正常終了                                               */
/*         ：負の値：異常終了                                               */
/*         ：                                                               */
/* 概    要：パーティション作成                                             */
/*         ：                                                               */
/* 作 成 日：2004.03.19                                                     */
/* 作 成 者：                                                               */
/*                                                                          */
/****************************************************************************/
/*** 変更履歴 ***************************************************************/
/*  履歴               変    更    内    容                日 付   修 正 者 */
/****************************************************************************/

F_INT	PCATA_PartCreate(F_BYTE		chanel_deviceno,		/* チャネル/デバイス番号	*/
						T_PART_CREATE_PARAM * io_param	)	/* I/O パラメータアドレス	*/
{
	F_INT		ret;				/* サブモジュール戻り値		*/
	F_DWORD		work;
	F_DWORD		work1;
	T_MBR		mbrbuffer;			/* MBRバッファ				*/
	F_WORD *	bufptr;				/* バッファポインタ			*/
	F_SHORT		cnt;				/* ループカウンタ			*/
	F_BYTE		chanelno;			/* チャネル番号				*/
	F_BYTE		deviceno;			/* デバイス番号				*/

/* パラメータチェック */

	chanelno = chanel_deviceno & ARG_CHANEL;
	chanelno >>= 4;
	deviceno = chanel_deviceno & ARG_UNIT;
	
	if ( chanelno >= MAX_CHANEL ) {
		return ( ERR_PARAM );
	}
	if ( chanelno != PCATA_dv_inf[deviceno].cnct_chanel ) {
		return ( ERR_PARAM );
	}
	if ( deviceno >= PCATA_ch_inf[chanelno].device_cnt ) {
		return ( ERR_PARAM );
	}
	if ( (io_param->mode == PART_MODE_PRIME) ||
		 (io_param->mode == PART_MODE_CREXPART) ) {
		if ( io_param->part_size == 0 ) {
			return ( ERR_PARAM );
		}
	}
	if ( io_param->mode == PART_MODE_DELPART ) {
		if ( io_param->part_no == 0 ) {
			return ( ERR_PARAM );
		}
	}

/* 初期化済みチェック&ユニットマウント済みチェック */

	if ( ((PCATA_ch_inf[chanelno].chanel_status) & ~CS_CLEAR) != CS_INIT ) {
		return ( ERR_NOTINIT );
	}

/* カード無しなら異常終了 */

	if ( PCCard_check() ) {
		return ( ERR_NOCARD );
	}

/* 自タスクID取得 */

	PCATA_osGetTID(&PCATA_ch_inf[chanelno].task_id);

/* IDENTIFY情報読込み */

	ret = PCATA_ReadIDTFY(chanelno, deviceno);
	if ( ret != DRV_OK ) {
		return ( ret );
	}

/* MBR 読込 */

	ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, 0);
	if ( ret != DRV_OK ) {
		return ( ret );
	}
	if ( *(F_WORD *)(mbrbuffer.signature) != MBR_SIGNATURE ) {
		return ( ERR_PCCARD );
	}
	if ( io_param->mode == PART_MODE_PRIME ) {
		if ( io_param->part_size > (PCATA_dv_inf[deviceno].total_sec -
									PCATA_dv_inf[deviceno].sec_track) ) {
			return ( ERR_PARAM );
		}
		mbrbuffer.part_table[ 0 ].active_flg = 0x00;
		PCATA_SubLBAtoCHS(PCATA_dv_inf[deviceno].sec_track,
						  mbrbuffer.part_table[ 0 ].top_chs, deviceno);
		mbrbuffer.part_table[ 0 ].part_type = io_param->part_type;
		work = io_param->part_size + PCATA_dv_inf[deviceno].sec_track - 1;
		PCATA_SubLBAtoCHS(work, mbrbuffer.part_table[ 0 ].end_chs, deviceno);
		mbrbuffer.part_table[ 0 ].top_lba[ 0 ] =
							(F_BYTE)PCATA_dv_inf[deviceno].sec_track;
		mbrbuffer.part_table[ 0 ].top_lba[ 1 ] =
							(F_BYTE)(PCATA_dv_inf[deviceno].sec_track >> 8);
		mbrbuffer.part_table[ 0 ].top_lba[ 2 ] = 0;
		mbrbuffer.part_table[ 0 ].top_lba[ 3 ] = 0;
		mbrbuffer.part_table[ 0 ].sct_size[ 0 ] =
							(F_BYTE)io_param->part_size;
		mbrbuffer.part_table[ 0 ].sct_size[ 1 ] = 
							(F_BYTE)(io_param->part_size >> 8);
		mbrbuffer.part_table[ 0 ].sct_size[ 2 ] = 
							(F_BYTE)(io_param->part_size >> 16);
		mbrbuffer.part_table[ 0 ].sct_size[ 3 ] = 
							(F_BYTE)(io_param->part_size >> 24);
		bufptr = (F_WORD *)&mbrbuffer.part_table[ 1 ];
		for( cnt = 0; cnt < ( MBR_PARTSIZE * 3 ); cnt++ ) {
			*bufptr = 0;
			bufptr++;
		}
		ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, 0);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
		/* BPB Clear */
		bufptr = (F_WORD *)&mbrbuffer;
		for( cnt = 0; cnt < (SEC_SIZE/sizeof(F_WORD)); cnt++ ) {
			*bufptr = 0xf6f6;
			bufptr++;
		}
		work = PCATA_dv_inf[deviceno].sec_track;
		ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, work);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
	}
	else if ( io_param->mode == PART_MODE_CREXPART ) {
		if ( mbrbuffer.part_table[ 0 ].part_type == MBR_PT_NONE ) {
			return ( ERR_NOPRIMARY );	/* 基本領域無し							*/
		}
		if ( mbrbuffer.part_table[ 1 ].part_type == MBR_PT_NONE ) {
			/* 拡張領域がない場合	*/
			mbrbuffer.part_table[ 1 ].active_flg = 0x00;
			work = 0;
			work1 = 0;
			for( cnt = 3; cnt >= 0; cnt-- ) {
				work = (work << 8) + mbrbuffer.part_table[ 0 ].sct_size[cnt];
			}
			for( cnt = 3; cnt >= 0; cnt-- ) {
				work1 = (work1 << 8) + mbrbuffer.part_table[ 0 ].top_lba[cnt];
			}
			work += work1;			/* 基本領域.セクタサイズ＋基本領域.開始セクタ	*/
			mbrbuffer.part_table[ 1 ].top_lba[ 0 ] = (F_BYTE)work;
			mbrbuffer.part_table[ 1 ].top_lba[ 1 ] = (F_BYTE)(work >> 8);
			mbrbuffer.part_table[ 1 ].top_lba[ 2 ] = (F_BYTE)(work >> 16);
			mbrbuffer.part_table[ 1 ].top_lba[ 3 ] = (F_BYTE)(work >> 24);
			mbrbuffer.part_table[ 1 ].part_type = MBR_PT_EXTEND2;
			PCATA_SubLBAtoCHS(work, mbrbuffer.part_table[ 1 ].top_chs, deviceno);
			work1 = PCATA_dv_inf[deviceno].total_sec - work;
			mbrbuffer.part_table[ 1 ].sct_size[ 0 ] = (F_BYTE)work1;
			mbrbuffer.part_table[ 1 ].sct_size[ 1 ] = (F_BYTE)(work1 >> 8);
			mbrbuffer.part_table[ 1 ].sct_size[ 2 ] = (F_BYTE)(work1 >> 16);
			mbrbuffer.part_table[ 1 ].sct_size[ 3 ] = (F_BYTE)(work1 >> 24);
			work1 += work;
			work1--;
			PCATA_SubLBAtoCHS(work1, mbrbuffer.part_table[ 1 ].end_chs, deviceno);
			bufptr = (F_WORD *)&mbrbuffer.part_table[ 2 ];
			for( cnt = 0; cnt < ( MBR_PARTSIZE * 2 ); cnt++ ) {
				*bufptr = 0;
				bufptr++;
			}
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, 0);
			if ( ret != DRV_OK ) {
				return ( ret );
			}
			/* 新規EBR作成	*/
			bufptr = (F_WORD *)&mbrbuffer;
			for (cnt = 0; cnt < TRANS_COUNT; cnt++ ) {
				*bufptr = 0;
				bufptr++;
			}
			mbrbuffer.part_table[ 0 ].active_flg = 0x00;
			work += PCATA_dv_inf[deviceno].sec_track;
			PCATA_SubLBAtoCHS(work, mbrbuffer.part_table[ 0 ].top_chs, deviceno);
			mbrbuffer.part_table[ 0 ].part_type = io_param->part_type;
			work1 = io_param->part_size;
			work1 += work;
			work1--;
			PCATA_SubLBAtoCHS(work1, mbrbuffer.part_table[ 0 ].end_chs, deviceno);
			mbrbuffer.part_table[ 0 ].top_lba[ 0 ] =
								(F_BYTE)PCATA_dv_inf[deviceno].sec_track;
			mbrbuffer.part_table[ 0 ].top_lba[ 1 ] =
								(F_BYTE)(PCATA_dv_inf[deviceno].sec_track >> 8);
			mbrbuffer.part_table[ 0 ].top_lba[ 2 ] = 0;
			mbrbuffer.part_table[ 0 ].top_lba[ 3 ] = 0;
			mbrbuffer.part_table[ 0 ].sct_size[ 0 ] =
								(F_BYTE)io_param->part_size;
			mbrbuffer.part_table[ 0 ].sct_size[ 1 ] = 
								(F_BYTE)(io_param->part_size >> 8);
			mbrbuffer.part_table[ 0 ].sct_size[ 2 ] = 
								(F_BYTE)(io_param->part_size >> 16);
			mbrbuffer.part_table[ 0 ].sct_size[ 3 ] = 
								(F_BYTE)(io_param->part_size >> 24);
			bufptr = (F_WORD *)mbrbuffer.signature;
			*bufptr = MBR_SIGNATURE;
			work -= PCATA_dv_inf[deviceno].sec_track;
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, work);
			if ( ret != DRV_OK ) {
				return ( ret );
			}
			/* BPB Clear */
			bufptr = (F_WORD *)&mbrbuffer;
			for( cnt = 0; cnt < (SEC_SIZE/sizeof(F_WORD)); cnt++ ) {
				*bufptr = 0xf6f6;
				bufptr++;
			}
			work += PCATA_dv_inf[deviceno].sec_track;
			ret = PCATA_WriteBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, work);
			if ( ret != DRV_OK ) {
				return ( ret );
			}
		}
		else {
			if ( ( mbrbuffer.part_table[ 1 ].part_type != MBR_PT_EXTEND1 )	&&
				 ( mbrbuffer.part_table[ 1 ].part_type != MBR_PT_EXTEND2 ) ) {
				return ( ERR_NOEXTEND );	/* 拡張領域無し							*/
			}
			work = 0;
			work1 = 0;
			for( cnt = 3; cnt >= 0; cnt-- ) {
				work = (work << 8) + mbrbuffer.part_table[ 1 ].sct_size[cnt];
			}
			for( cnt = 3; cnt >= 0; cnt-- ) {
				work1 = (work1 << 8) + mbrbuffer.part_table[ 1 ].top_lba[cnt];
			}
			ret = PCATA_ReadBR(chanelno, deviceno, (F_WORD *)&mbrbuffer, work1);
			if ( ret != DRV_OK ) {
				return ( ret );
			}
			if ( *(F_WORD *)(mbrbuffer.signature) != MBR_SIGNATURE ) {
				return ( ERR_PCCARD );
			}
			ret = PCATA_MakeExPTbl(chanelno, deviceno, &mbrbuffer, work1, &work, 0, io_param);
			if ( ret != DRV_OK ) {
				return ( ret );
			}
		}
	}
	else if ( io_param->mode == PART_MODE_DELPART ) {
		if ( mbrbuffer.part_table[ 0 ].part_type == MBR_PT_NONE ) {
			return ( ERR_NOPRIMARY );	/* 基本領域無し							*/
		}
		if ( ( mbrbuffer.part_table[ 1 ].part_type != MBR_PT_EXTEND1 )	&&
			 ( mbrbuffer.part_table[ 1 ].part_type != MBR_PT_EXTEND2 ) ) {
			return ( ERR_NOEXTEND );	/* 拡張領域無し							*/
		}
		work = 0;
		for( cnt = 3; cnt >= 0; cnt-- ) {
			work = (work << 8) + mbrbuffer.part_table[ 1 ].top_lba[cnt];
		}
		ret = PCATA_DeleteExPTbl(chanelno, deviceno, &mbrbuffer, work, 1, 0, io_param);
		if ( ret != DRV_OK ) {
			return ( ret );
		}
	}
	return DRV_OK;
}

/******************************************************************************/
/*      Unpublished Copyright (C) 2003      ウェスコム株式会社                */
/******************************************************************************/
