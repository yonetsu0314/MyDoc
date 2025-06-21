/*======================================================================
 * File name    : fs_ram.c
 * Original		: p:/dacs_i/shdsplib/drive/fs/RCS/fs_ram.c 1.1
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/fs/rcs/fs_ram.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: fs_ram.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * dfsid.c追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/


#include <string.h>
#include "fsys.h"

int format_flag;

const ST_DIRENTRY Volume = 
{
	"RAMDISK    ",
	(ATTR_ARCH|ATTR_VOL),
	0,
    0,
    0,
    0,
    0,
};

static short format(ST_DISK *d)
{
	unsigned long *p, *end;

	/* fat 0 clear */
	p = (unsigned long *)d->addr;
	if( d->fatbits == FAT16 ) {
		*p++ = 0xf8ffffffL;
	}
	else {
		*p++ = 0xf9ffff00L;
	}
	end = (unsigned long *)( d->addr + ((d->rsc + d->sf * d->fn) * SECT_SZ));
	while (p != end)
		*p++ = 0L;

	/* Create volume Label */
	p = end;
	memcpy( p, &Volume, sizeof(ST_DIRENTRY) );

	/* root_dir 0 clear */
	p += sizeof(ST_DIRENTRY)/sizeof(long);
	end += (DIRENTRY_SZ * d->rde);
	while (p != end) {
		*p++ = 0L;
	}

	return R_OK;
}

static short identify(ST_DISK *d)
{
	int cluscnt;
	int sizefat;

	d->sc  = 1; 						/* Sectors per Cluster */
	d->rsc = 0; 						/* Reserved Sector Count */
	d->fn  = 2; 						/* Number of FATs */
	d->rde = 224;						/* Root Directry Entrys */
	d->sf  = 8; /*020725*/						/* Sectors per FAT */
	d->ts  = d->par / (SECT_SZ*d->sc);	/* Total Clusters */
	d->fatbits = FAT16; 				/* Set Fat Bits */

/* including from dos_util.c @im-7000 */
	/* ＦＡＴ当りのセクタ数の設定 */
	sizefat = (d->fatbits=FAT16)? 16:12;
	do {
		cluscnt = ( d->ts - d->rsc + d->sc - 1 ) / d->sc;
		d->sf = ( ( cluscnt * sizefat / 8 ) + SECT_SZ - 1 ) / SECT_SZ;

		cluscnt = ( d->ts - d->rsc - d->sf * d->fn ) / d->sc;
		d->sf = ( ( cluscnt * sizefat / 8 ) + SECT_SZ - 1 ) / SECT_SZ;
	}
	while( cluscnt > 65525 );

/* format!!  = fat 0 clear & root_dir 0 clear */
	if( format_flag ) {
		return format( d );
	}
	return R_OK;
}

static short hardware_init(ST_DISK *d)
{
#if 0	/* BPB領域はクリアしない */
	unsigned long *p, *end;

	/* BPB Clear */

	p = (unsigned long *)d->addr;
	end = (unsigned long *)(d->addr + SECT_SZ);
	while (p != end)
		*p++ = 0L;
#endif

	d->fatp = (char*)d->addr;
	return R_OK;
}

static short read_sector(ST_DISK *d, unsigned long sectno, unsigned char *buf, unsigned short num)
{
	if (sectno >= d->ts)
		return R_PARAM_ERR;

	memcpy(buf, (char *)(d->addr + sectno * SECT_SZ), SECT_SZ * num);

	return R_OK;
}

static short read_sector2(ST_DISK *d, unsigned long sectno, unsigned char **buf, unsigned short num)
{
	if (sectno >= d->ts)
		return R_PARAM_ERR;

	*buf = (unsigned char *)(d->addr + sectno * SECT_SZ);

	return R_OK;
}

static short write_sector(ST_DISK *d, unsigned long sectno, unsigned char *buf, unsigned short num)
{
	unsigned char *p;

	if (sectno >= d->ts)
		return R_PARAM_ERR;
	p = (unsigned char *)(d->addr + sectno * SECT_SZ);

	if (p != buf)
		memcpy(p, buf, SECT_SZ * num);
	return R_OK;
}

short ramdisk(ST_DISK *d, short fncd, unsigned long sectno, void * par, unsigned short num)
{
	short  ercd;
	switch (fncd)
	{
	case TFN_READ_SECTOR:
		ercd = read_sector(d, sectno, (unsigned char *)par, num);
		return ercd;
	case TFN_SECTOR_ADR:
		ercd = read_sector2(d, sectno, (unsigned char**)par, num);
		return ercd;
	case TFN_WRITE_SECTOR:
		ercd = write_sector(d, sectno, (unsigned char *)par, num);
		return ercd;
	case TFN_IDENTIFY:
	case TFN_MOUNT:
		ercd = identify(d);
		return ercd;
	case TFN_FORMAT:
		ercd = format(d);
		return ercd;
	case TFN_HARDWARE_INIT:
		ercd = hardware_init(d);
		return ercd;
	case TFN_HARDWARE_RESET:
		return R_OK;
	case TFN_UNMOUNT:
		return R_OK;
	case TFN_MEDIACHK:
		return R_OK;
	default:
		return R_NOT_SUPORT;
	}
}
