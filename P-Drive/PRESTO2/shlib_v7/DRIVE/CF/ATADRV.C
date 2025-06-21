/*======================================================================
 * File name    : atadrv.c
 * Original		: r:/c/astra-s/exp/rcs/atadrv.c 1.4
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/cf/RCS/atadrv.c 1.3 1970/01/01 00:00:00 chimura Exp $
 * $Log: atadrv.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2005/02/23
 * ファイルシステムsave60用にerase_sectを追加する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * PCMCIA_..._BASEｱﾄﾞﾚｽをcf_base...から取得
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*																		*/
/*		ファイル名		: ATADRV.C										*/
/* ==================================================================== */
/*		機能分類		: ＰＣＭＣＩＡ ＡＴＡドライバ					*/
/*	                        (Flash ATA/Compact Flash/ATA-Hard Disk)		*/
/*		作成日付		: 1998. 3. 9 < Mon >							*/
/*		作成者名		:												*/
/*																		*/
/*	履歴管理															*/
/* ------------+----------+-------------------------------------------- */
/*	日 付	   | 管理番号 | 担当者名及び変更内容						*/
/* ------------+----------+-------------------------------------------- */
/*			   |		  | 											*/
/* ------------+----------+-------------------------------------------- */
/*			   |		  | 											*/
/* ------------+----------+-------------------------------------------- */
/*																		*/
/************************************************************************/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <biosif\cf.h>
//#include </itron4/kernel.h>/*020726*/
#include <kernel.h>/*020726*/
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#include "drive\pcmtpl.h"
#include "drive\fs_ata.h"
#include "drive\atadrv.h"

	/*==========================================================
							構造体定義
	==========================================================*/
typedef struct {
	unsigned char 	*base;				/* ベースＩ／Ｏアドレス 	*/
	int cd;
} PCM_ATA_INFO;

	/*==========================================================
							静的変数定義
	==========================================================*/
static PCM_ATA_INFO atainfo;

	/*==========================================================
						プロトタイプ宣言
	==========================================================*/
static int read_secter( int sock, unsigned char *p, int sect, int cnt );


	/*---------------------------------------------------------
					カード挿入時の処理
	----------------------------------------------------------*/
static int insert_card( int sock, PCMEVT *evt )
{
	tuple_t tuple;
	cisparse_t parse;
	unsigned char buf[128];
	unsigned char *config;
	int mem;

	/* カードファンクションの取得 */
	tuple.Attributes = 0;
	tuple.DesiredTuple = CISTPL_FUNCID;
	tuple.TupleData = buf;
	tuple.TupleDataMax = 64;
	tuple.TupleOffset = 0;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -1;
	}
	/* 固定ディスクデバイス以外はこのドライバで処理できない */
	if( parse.funcid.func != CISTPL_FUNCID_FIXED ) {
		return -1;
	}
	tuple.DesiredTuple = CISTPL_FUNCE;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -1;
	}
	/* ＩＤＥインターフェイス以外はこのドライバで処理できない */
	if( parse.funce.type != CISTPL_FUNCE_IDE_IFACE ) {
		return -1;
	}
	/* コンフィギュレーション情報の取得 */
	tuple.DesiredTuple = CISTPL_CONFIG;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -1;
	}
//	config  = PCMCIA_ATTR_BASE;
	config  = (volatile unsigned char*)cf_base.attr;
	config += parse.config.base;

	/* コンフィギュレーションエントリの取得 */
	tuple.DesiredTuple = CISTPL_CFTABLE_ENTRY;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -1;
	}
	memset( &atainfo, 0, sizeof(PCM_ATA_INFO) );
	if( parse.cftable_entry.io.nwin ) {
		mem = 0;
//		atainfo.base  = PCMCIA_IO_BASE;
		atainfo.base  = (volatile unsigned char*)cf_base.io;
		atainfo.base += (unsigned short)parse.cftable_entry.io.win[0].base;
	}
	else {
		mem = 1;
//		atainfo.base  = PCMCIA_COMM_BASE;
		atainfo.base  = (volatile unsigned char*)cf_base.comm;
		atainfo.base += (unsigned short)parse.cftable_entry.mem.win[0].card_addr;
	}

	/* カードをリセットする */
	ResetCard( sock, config );

	/* コンフィギュレーションインデックスをセットする */
	*(config+CISREG_COR) = parse.cftable_entry.index;

	/* ８ビットアクセスモードにセットする */
	*(config+CISREG_CCSR) |= CCSR_IOIS8;

	/* ドライバＩＤの設定 */
	SetDriverID( sock, CISTPL_FUNCID_FIXED );
	atainfo.cd = 1;
	return 0;
}

	/*---------------------------------------------------------
					カードイジェクト時の処理
	----------------------------------------------------------*/
static int eject_card( int sock )
{
	if( GetDriverID( sock ) == CISTPL_FUNCID_FIXED ) {
		/* ドライバＩＤの設定 */
		SetDriverID( sock, 0 );
		atainfo.cd = 0;
	}
	return 0;
}
	/*==========================================================
			ＰＣＭＣＩＡ ＡＴＡドライバエントリ関数
	==========================================================*/
int pcmc_ata_driver( int sock, int type, PCMEVT *evt )
{
	switch( type ) {
		case SS_INSERTION:
			insert_card( sock, evt );
			break;
		case SS_EJECTION:
			eject_card( sock );
			break;
		default:
			break;
	}
	return 0;
}

	/*==========================================================
					ＡＴＡドライバが使用可能か
	==========================================================*/
int is_register_atadrv( void )
{
	return is_register_client_callback( pcmc_ata_driver );
}

	/*==========================================================
					Read I/O Window WORD
	==========================================================*/
UH  PCCard_io_inpw( UW addr )
{
	UH	data;

	*(UB*)&data       = port_in( atainfo.base + addr );
	*(((UB*)&data)+1) = port_in( atainfo.base + addr+1 );
	return	data;
}

	/*==========================================================
					Read I/O Window BYTE
	==========================================================*/
UB  PCCard_io_inpb( UW addr )
{
	UB	data;

	data = port_in( atainfo.base + addr );
	return	data;
}

	/*==========================================================
					Write I/O Window WORD
	==========================================================*/
void PCCard_io_outw( UW addr, UH data)
{
	port_out( atainfo.base + addr , *(UB*)&data );
	port_out( atainfo.base + addr+1 , *(((UB*)&data)+1) );
}

	/*==========================================================
					Write I/O Window BYTE
	==========================================================*/
void PCCard_io_outb( UW addr, UB data)
{
	port_out( atainfo.base + addr , data );
}

	/*==========================================================
							Init
	==========================================================*/
ER PCCard_init( void )
{
	if( atainfo.cd ) {
		return E_OK;
	}
	else {
		return E_OBJ;						/* Not Inserted 		*/
	}
}

	/*==========================================================
						Check Card
	==========================================================*/
ER  PCCard_check( void )
{
	return PCCard_init();
}

	/*==========================================================
						Card Power Off
	==========================================================*/
ER PCCard_end( void )
{
	return E_OK;
}

/****************************************************************************
	cf_read_sect
*****************************************************************************/
void  ata_read_sect( unsigned short *ptr )
{
#if 1
	unsigned char *buf;
	int	i;

	buf = (unsigned char*)ptr;
	for(i=0; i<512; i++)
	{
		*buf++ = port_in( atainfo.base + ATA_DATA_REG );
	}
#else
	/* ＤＭＡを使用するには非キャッシュエリアにバッファを配置する必要がある */
	dma1_burst_trans( atainfo.base + ATA_DATA_REG, ptr, 512, 0 );
#endif
}

/****************************************************************************
	write_sect
*****************************************************************************/
void  ata_write_sect( unsigned short *ptr )
{
#if 1
	unsigned char *buf;
	unsigned short i;

	buf = (unsigned char*)ptr;
	for(i=0; i<512; i++)
	{
		port_out( atainfo.base + ATA_DATA_REG , *buf++ );
		if((i%128)==0){
//			wait0(1);
			dly_tsk(1);
		}
	}
#else
	/* ＤＭＡを使用するには非キャッシュエリアにバッファを配置する必要がある */
	dma1_burst_trans( ptr, atainfo.base + ATA_DATA_REG, 512, 1 );
#endif
}

/****************************************************************************
	erase_sect
*****************************************************************************/
void  ata_erase_sect( unsigned char erase_data )
{
	unsigned short i;

	for(i=0; i<512; i++)
	{
		port_out( atainfo.base + ATA_DATA_REG , erase_data);
		if((i%128)==0){
			dly_tsk(1);
		}
	}
}

