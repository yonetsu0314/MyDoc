/*======================================================================
 * File name    : ata_cmd.c
 * Original		: p:/dacs_i/shdsplib/drive/fs/RCS/fs_ata.c 1.4
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/fssvr60/pcata/RCS/ata_cmd.c 1.1 1970/01/01 00:00:00 kagatume Exp $
 * $Log: ata_cmd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/

/*******************************************************************
	Compact Flash/Flash ATA Driver
********************************************************************/

#include "ata_def.h"
#include "ata_srct.h"
#include "ata_prot.h"
#include "drive\atadrv.h"
#include "drive\fs_ata.h"

static	const	UB	chla[2] 	= { CMD_IDE_CHS, CMD_IDE_LBA		};
static	const	UB	mast[2] 	= { CMD_IDE_MASTER, CMD_IDE_SLAVE	};

static ER get_devdat( UB chs, UB drv, UB *mode );

/*******************************************************************
	ide_int
********************************************************************/
ER	  ide_int( UB set )
{

	UB	rst_cmd[2]	= { CMD_INT_ON, CMD_INT_OFF 		};
	if( 2 <= set ){
		return	E_PAR;
	}

	PCCard_io_outb( ATA_DEVCTL_REG , CMD_IDE_ROFF | rst_cmd[set] );

}

/*******************************************************************
	ide_busy
********************************************************************/
ER	  ide_busy( void )
{
	UW	tout;
	UH	stts;												/* Time Out Counter    */

	for( tout=0; tout<IDE_TOUT; tout++ ){					/* BUSY-off  Wait	   */
		stts =	PCCard_io_inpb( ATA_ALTSTA_REG );
		if( (stts & STS_IDE_BUSY) == 0x0000 ){
			return	E_OK;
		}
		dly_tsk(1);
	}
	return	E_TMOUT;
}

/*******************************************************************
	ide_check
********************************************************************/
ER	  ide_check( UH  check )
{
	UH	tout;										/* Time Out Counter    */
	int i;
	UH stts;

	for( tout=0; tout<IDE_TOUT; tout++ )
	{
		for(i=0;i<20;i++){
			stts =	PCCard_io_inpb( ATA_ALTSTA_REG );
			if( (stts & check) == check ){
				return	E_OK;
			}
		}
		dly_tsk(1);
	}
	return	E_TMOUT;
}

/*******************************************************************
	get_devdat
********************************************************************/
static ER	 get_devdat( UB chs, UB drv, UB *mode )
{
	if( 2 <= chs ){
		return E_PAR;
	}
	if( 2 <= drv ){
		return E_PAR;
	}
	*mode = (UB)(CMD_IDE_DHR | chla[chs] | mast[drv]);
	return	E_OK;
}

/*******************************************************************
	ide_cmd
********************************************************************/
ER	ide_cmd( UB chs, UB drv, UH cyl, UB head, UB f_sec, UB sec_cnt, UB cmd , UB fc )
{
	UB	tmp;
	UB	mode;

	if( E_OK != get_devdat( chs, drv, &mode ) )
		return	E_TMOUT;

	PCCard_io_outb( ATA_CYLLOW_REG , cyl & 0x00ff );			/* cylinder LO   */
	PCCard_io_outb( ATA_CYLHI_REG , ((cyl & 0xff00) >> 8) );	/* cylinder HI   */
	PCCard_io_outb( ATA_SECNUM_REG , (UB)f_sec );				/* start sector  */
	PCCard_io_outb( ATA_SECCNT_REG , (UB)sec_cnt );				/* sector num    */
	tmp = (UB)(head & 0x0f);
	PCCard_io_outb( ATA_DRVHEAD_REG , (mode | tmp) );			/* head number   */
	PCCard_io_outb( ATA_COMMAND_REG , cmd );					/* command       */
	return E_OK;
}
