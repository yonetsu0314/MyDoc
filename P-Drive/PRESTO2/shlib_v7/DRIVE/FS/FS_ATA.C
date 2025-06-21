/*======================================================================
 * File name    : fs_ata.c
 * Original		: p:/dacs_i/shdsplib/drive/fs/RCS/fs_ata.c 1.4
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/fs/rcs/fs_ata.c 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: fs_ata.c $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/

/*******************************************************************
	Compact Flash/Flash ATA Driver
********************************************************************/

#include <stdio.h>
#include <string.h>
#include <sh7615\7615.h>
//#include </itron4/kernel.h>
#include <kernel.h>/*020726*/

#undef   feof

#include "fsys.h"
#include "drive\atadrv.h"/*020813*/
#include "drive\pcmsock.h"/*020813*/
#include "drive\fs_ata.h"/*020813*/

#define  IO_WIN	PCMCIA_IO_BASE

//#define  IDE_TOUT		  0xFFF
#define  IDE_TOUT		  0x7FFF	 /* Loop retry count *//*030305*/

/* Partition Entry */
typedef struct	PartEntry	{
	UB	BootableFlag;		/* bootable partition =0x80, other=0x00 */
	UB	StartHead;			/* starting head						*/
	UB	StartSector;		/* Bit0..5 are Start sector , 6..7 Cyl	*/
	UB	StartCyl;			/* Lower 8 Bits 						*/
	UB	SystemID;			/* File systems type					*/
							/* 0x00 = Unknown or unused entry		*/
							/* 0x01 = DOS 12BIT FAT 				*/
							/* 0x04 = DOS 16BIT FAT, Partition<32MB */
							/* 0x05 = DOS Extened partition 		*/
							/* 0x06 = DOS 16BIT FAT, Partition>32MB */
	UB	EndHead;			/* ending head number					*/
	UB	EndSector;			/* Ending Sector and hi bits of cyl 	*/
	UB	EndCyl; 			/* Low Bytes of Ending Cyl Number		*/
	UW	AbsBegin;			/* relative blocks to Logical first sec.*/
	UW	SectCount;			/* Total Sectors in partition			*/
}	PART_ENT;

static	UB	rd_sct_buf[512];		/* Sector Buffer for read		*/
UB			system_id;
UW			total_sect_count;		/* in partition 				*/
UW			logical_first_sect; 	/* not need +1, becayse of LBA	*/

ST_DISK		*dd;

static	const	UB	chla[2] 	= { CMD_IDE_CHS, CMD_IDE_LBA		};
static	const	UB	mast[2] 	= { CMD_IDE_MASTER, CMD_IDE_SLAVE	};

static ER read_sector_sub(UW sectno, UB *buf, UH num);
static ER get_devdat( UB chs, UB drv, UB *mode );

extern	volatile void	us_wait(void);		/* User Adjustable		*/

ID fsysid;

/****************************************************************************
	Delay Time : μ単位のDelay処理
*****************************************************************************/
volatile void	us_wait( void )		/* User Adjustable		*/
{
	short	j=1;
	while(--j);
}

/****************************************************************************
	Delay Time : μ単位のDelay処理
*****************************************************************************/
void u_wait( volatile long time )
{
	do {
		us_wait();
	} while(--time);
}

/*******************************************************************
	get_le_h
********************************************************************/
static UH get_le_h(VP p)
{
	UB *pp = (UB *)p + 1;
	UH d;

	d = *pp--; d <<= 8;
	d |= *pp;
	return d;
}

/*******************************************************************
	set_le_w
********************************************************************/
static void set_le_w(VP p, UW d)
{
	UB *pp = (UB *)p;

	*pp++ = (UB)d; d >>= 8;
	*pp++ = (UB)d; d >>= 8;
	*pp++ = (UB)d; d >>= 8;
	*pp   = (UB)d;
}

/*******************************************************************
	get_le_w
********************************************************************/
static UW get_le_w(VP p)
{
	UB *pp = (UB *)p + 3;
	UW d;

	d  = *pp--; d <<= 8;
	d |= *pp--; d <<= 8;
	d |= *pp--; d <<= 8;
	d |= *pp;
	return d;
}

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
	ER	ercd;

	for( tout=0; tout<IDE_TOUT; tout++ ){					/* BUSY-off  Wait	   */
		stts =	PCCard_io_inpb( ATA_ALTSTA_REG );
		if( (stts & STS_IDE_BUSY) == 0x0000 ){
			return	E_OK;
		}
//		wait0(1);
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
	ER	ercd;

	for( tout=0; tout<IDE_TOUT; tout++ )
	{
		for(i=0;i<20;i++){
			stts =	PCCard_io_inpb( ATA_ALTSTA_REG );
			if( (stts & check) == check ){
				return	E_OK;
			}
		}
//		wait0(1);
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
	ER	ercd;

	if( E_OK != get_devdat( chs, drv, &mode ) )
		return	E_TMOUT;

	PCCard_io_outb( ATA_CYLLOW_REG , cyl & 0x00ff );		  /* cylinder LO   */
	PCCard_io_outb( ATA_CYLHI_REG , ((cyl & 0xff00) >> 8) ); /* cylinder HI   */
	PCCard_io_outb( ATA_SECNUM_REG , (UB)f_sec );			  /* start sector  */
	PCCard_io_outb( ATA_SECCNT_REG , (UB)sec_cnt );		  /* sector num    */
	tmp = (UB)(head & 0x0f);
	PCCard_io_outb( ATA_DRVHEAD_REG , (mode | tmp) );		  /* head number   */
	PCCard_io_outb( ATA_COMMAND_REG , cmd );				  /* write command */
	return E_OK;
}

/****************************************************************************
	ide_swrite
*****************************************************************************/
ER	ide_swrite( UB chs, UB drv, UH cyl, UB head, UB f_sec, UB sec_cnt, UH *dbuf )
{
	ER	ercd;
	UH	*ptr;
	UH	cnt = 0;
	UH	i;
	UB	data_COR;

#ifdef USE_INT
	ide_int(0); 	 /* Card Ready interrupt ON */
	vcan_wup();
#endif

	if(E_OK != ide_cmd(chs, drv, cyl, head, f_sec, sec_cnt, IDE_WRITE_SECTOR, 0))
		return E_TMOUT;

	if(E_OK != ide_busy()) return E_TMOUT;

	ptr = (UH *)dbuf;

	if(sec_cnt == 0) cnt = 256;
	else			 cnt = sec_cnt;

	for(i=0; i<cnt; i++)
	{
		ata_write_sect(ptr);

#ifndef USE_INT
		if( E_OK != ide_busy() )
			return E_TMOUT;

#else
		fsysid = vget_tid();
		slp_tsk();
		data_COR = PCCard_io_inpb( ATA_STATUS_REG );
#endif

		ptr += 256;
	}

#ifdef USE_INT
	ide_int(1); 	 /* Card Ready interrupt OFF */
#endif

	return	E_OK;
}

/****************************************************************************
	ide_sread
*****************************************************************************/
ER	ide_sread( UB chs, UB drv, UH cyl, UB head, UB f_sec, UB sec_cnt, UH *dbuf )
{
	ER	ercd;
	UH	*ptr;
	UH	cnt = 0;
	UH	i;
	UB	data_COR;

#ifdef USE_INT
	ide_int(0);    /* Card Ready interrupt ON */
	vcan_wup();
#endif

	if(E_OK != ide_cmd(chs, drv, cyl, head, f_sec, sec_cnt, IDE_READ_SECTOR, 0))
		return	E_TMOUT;

#ifndef USE_INT
	if(E_OK != ide_busy()) return E_TMOUT;
#endif

	ptr = (UH *)dbuf;

	if(sec_cnt == 0) cnt = 256;
	else			 cnt = sec_cnt;

	for(i=0; i<cnt; i++)
	{

#ifndef USE_INT
		if(E_OK != ide_check(CHK_IDE_SECT)) return E_TMOUT;
#else
		fsysid = vget_tid();
		slp_tsk();
		data_COR = PCCard_io_inpb(ATA_STATUS_REG);
#endif
		ata_read_sect(ptr);
		if(E_OK != ide_busy()) return E_TMOUT;

		ptr += 256;
	}

#ifdef USE_INT
	ide_int(1); 	 /* Card Ready interrupt OFF */
#endif

	return	E_OK;
}

/****************************************************************************
	identify
*****************************************************************************/
static ER identify(ST_DISK *d)
{
	ER			ercd;
	PART_ENT	*pe;
	UH			*p;
	UH			tmp;
	UB			btmp;
	/*
	 * At first,Read the physical sector 0
	 * to get the information about partition.
	 * From this information,We will get
	 * the offset to Logical sector.
	 */
	
	
	if(E_OK != ide_busy()) return E_TMOUT;
	
	ercd = read_sector_sub((UW)0, rd_sct_buf, 1);		   /* LBA Block 0 read			   */
	if(ercd != E_OK) return ercd;

	pe = (PART_ENT *)&rd_sct_buf[0x1be];

	system_id = pe->SystemID;				/* format type					*/

	d->logic = get_le_w(&(pe->AbsBegin)); /* because we access by LBA			  */
	total_sect_count   = get_le_w(&(pe->SectCount));/* number of sectors in this partition	*/

	ercd = read_sector_sub(d->logic, rd_sct_buf, 1);
	if (ercd != E_OK) return ercd;

	d->sc  = rd_sct_buf[0x0d];				/* Sectors per Cluster					*/
	d->rsc = get_le_h(rd_sct_buf + 0x0e);	/* Reserved Sector Count				*/
	d->fn  = 2; 							/* Number of FATs ... 2(Same for ALL)	*/
	d->rde = get_le_h(rd_sct_buf + 0x11);	/* Directry Entry for ROOT Directry 	*/
	d->sf  = get_le_h(rd_sct_buf + 0x16);	/* Sectors per FAT						*/
	d->ts  = get_le_h(rd_sct_buf + 0x13);	/* Total Sectors						*/
	if(d->ts == 0) d->ts = get_le_w(rd_sct_buf + 0x20);
	if(rd_sct_buf[0x3a] == '2') d->fatbits = FAT12;
	else						d->fatbits = FAT16;

	return E_OK;
}

/****************************************************************************
	format
*****************************************************************************/
static ER format(ST_DISK *d)
{
	return E_NOSPT;
}

/****************************************************************************
	ref_drv
*****************************************************************************/
ER	ref_drv(ST_DISK *d)
{
	return PCCard_check();
}

/****************************************************************************
	hardware_init
*****************************************************************************/
static ER hardware_init(ST_DISK *d)
{
	ER	ercd;
	dd = d;
	return	E_OK;

}

/****************************************************************************
	read_sector_sub
*****************************************************************************/
static ER read_sector_sub(UW sectno, UB *buf, UH num)
{
	ER	ercd;
	UB	cyl_lo;
	UB	cyl_hi;
	UH	cyl;
	UB	secno;
	UB	head;

	secno  = (UB)(sectno	   & 0xff);
	cyl_lo = (UB)((sectno>>8)  & 0xff);
	cyl_hi = (UB)((sectno>>16) & 0xff);
	cyl    = ( cyl_hi << 8 ) | cyl_lo;
	head   = (UB)((sectno>>24) & 0x0f);
	ercd = ide_sread(1, 0 , cyl, head, secno, num, (UH *)buf);
	return ercd;
}

/****************************************************************************
	read_sector
*****************************************************************************/
static ER read_sector(ST_DISK *d, UW sectno, UB *buf, UH num)
{
	ER	ercd;
	if (sectno >= d->ts)
		return E_PAR;
	sectno	+= d->logic;
	ercd = read_sector_sub(sectno, buf, num);
	/* *buf = rd_sct_buf; */
	return ercd;
}

/****************************************************************************
	write_sector
*****************************************************************************/
static ER write_sector(ST_DISK *d, UW sectno, UB *buf, UH num)
{
	ER	ercd;
	UB	cyl_lo;
	UB	cyl_hi;
	UH	cyl;
	UB	secno;
	UB	head;

	if (sectno >= d->ts) return E_PAR;

	sectno += d->logic;
	secno  = (UB)(sectno	   & 0xff);
	cyl_lo = (UB)((sectno>>8)  & 0xff);
	cyl_hi = (UB)((sectno>>16) & 0xff);
	cyl    = ( cyl_hi << 8 ) | cyl_lo;
	head   = (UB)((sectno>>24) & 0x0f);
	ercd = ide_swrite( 1, 0 , cyl, head, secno, num, (UH *)buf );

	return ercd;
}

/****************************************************************************
	mount
*****************************************************************************/
static ER mount(ST_DISK *d)
{
	ER		ercd;
	int 	i;

	ercd = PCCard_init();							/* Card Initialize			*/
	if(ercd != E_OK) return ercd;

	ercd = identify(d);
	if(ercd != E_OK) return ercd;

	d->dsk_flg |= DISK_MOUNT;

	return E_OK;
}

/****************************************************************************
	unmount
*****************************************************************************/
static ER unmount(ST_DISK *d)
{
	ER	ercd;

	ercd = PCCard_end();
	d->dsk_flg &= ~DISK_MOUNT;
	return ercd;
}

/****************************************************************************
	flash_ATA
*****************************************************************************/
ER flash_ATA(ST_DISK *d, FN fncd, UW sectno, VP par, UH snum)
{
	ER	ercd;

	switch (fncd)
	{
	case TFN_READ_SECTOR:
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_READING );
		ercd = read_sector(d, sectno, (UB *)par, snum);
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_STOPPED );
		return ercd;
	case TFN_WRITE_SECTOR:
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_WRITING );
		ercd = write_sector(d, sectno, (UB *)par, snum);
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_STOPPED );
		return ercd;
	case TFN_IDENTIFY:
		ercd = identify(d);
		return ercd;
	case TFN_FORMAT:
		ercd = format(d);
		return ercd;
	case TFN_HARDWARE_INIT:
		ercd = hardware_init(d);
		return ercd;
	case TFN_HARDWARE_RESET:
		return E_OK;
	case TFN_MOUNT:
		ercd = mount(d);
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_MOUNTED );
		return ercd;
	case TFN_UNMOUNT:
		ercd = unmount(d);
		if ( d->callback != (DISK_CALLBACK)NULL )
			d->callback( d, TFN_DISK_UNMOUNTED );
		return ercd;
	case TFN_MEDIACHK:
		ercd = ref_drv(d);
		return ercd;
	default:
		return E_NOSPT;
	}
}
