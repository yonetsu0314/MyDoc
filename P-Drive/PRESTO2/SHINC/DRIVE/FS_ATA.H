/*======================================================================
 * File name    : fs_ata.h
 * Original		: r:/c/astra-s/exp/rcs/fs_ata.h 1.3
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shinc/drive/RCS/fs_ata.h 1.2 1970/01/01 00:00:00 kagatume Exp $
 * $Log: fs_ata.h $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/01/06 21:00
 * 定義IDE_IDTFY_DRIVEを追加する
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 新規リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/

/****************************************************************************
	ATA DRIVER
*****************************************************************************/

/****************************************************************************
	ATA Control Register Assign
*****************************************************************************/
#define ATA_DATA_REG		0x00
#define ATA_ERROR_REG		0x01
#define ATA_SECCNT_REG		0x02
#define ATA_SECNUM_REG		0x03
#define ATA_CYLLOW_REG		0x04
#define ATA_CYLHI_REG		0x05
#define ATA_DRVHEAD_REG 	0x06
#define ATA_STATUS_REG		0x07
#define ATA_COMMAND_REG 	0x07
#define ATA_ALTSTA_REG		0x0e
#define ATA_DEVCTL_REG		0x0e
#define ATA_DRVADR_REG		0x0f

/****************************************************************************
	IDE Control Commands
*****************************************************************************/
#define  CMD_IDE_RON	0x0c
#define  CMD_IDE_ROFF	0x08
#define  CMD_INT_ON 	0x00
#define  CMD_INT_OFF	0x02

/****************************************************************************
	Alt. status register Data
*****************************************************************************/
#define  STS_IDE_BUSY  0x0080
#define  CHK_IDE_REDY  0x50
#define  CHK_IDE_SECT  0x58

/****************************************************************************
	Drive Head Register
*****************************************************************************/
#define  CMD_IDE_DHR	0xa0
#define  CMD_IDE_CHS	0x00
#define  CMD_IDE_LBA	0x40
#define  CMD_IDE_MASTER 0x00
#define  CMD_IDE_SLAVE	0x10

/****************************************************************************
	Function
*****************************************************************************/
#define  IDE_READ_SECTOR  0x20		/* Read sector		*/
#define  IDE_WRITE_SECTOR 0x30		/* Write sector 	*/
#define  IDE_IDTFY_DRIVE  0xec		/* Identify Drive	*/

