/*----------------------------------------------------------------------*
 * File name	: dpmsini.c	/ system initialize			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmsini.c 1.1 1993/10/20 20:11:58 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｾﾏﾌｫ、割り込みﾀｽｸ及びBIOSｺﾝﾃｷｽﾄの生成を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dev_id		: ﾃﾞﾊﾞｲｽIDﾅﾝﾊﾞｰ					*/
/*	*rx_tsk		: ｱｸｾｽ･ｱﾄﾞﾚｽの配列へのfarﾎﾟｲﾝﾀｰ			*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\icu.h>			/* Show bios		*/
#include	<biosif\dpmem.h>		/*	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\dpmdrv.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"dpmctxt.h"			/* Show current		*/
#include	"dpmctl.h"			/*	"		*/
#undef		STORAGE

int
dpm_sys_init(
	DPMID		dev_id,
	union aap	*rx_tsk
	)
{
	struct dpm_ctxt		*ctp;		/* pointer to contxt	*/
	const struct dpmpinf	*phy_inf;	/* & to physical info	*/
	int			no;		/* ｺﾝﾃｷｽﾄ･ﾅﾝﾊﾞｰ		*/
	int			err, j;

	dpm_ds.ifp = (int *)&dpm_ds.acadr;
	if((no = dev_id & (~DPM_DIR)) > DPM_DEV_MAX)		return(-1);
	if((dpm_rx_tsk[no] = rx_tsk)== NULL)			return(-1);
	ctp = &dpm_ctxt_tbl[no];
	if(err = init_dpm_read(no))				return(-1);
	if(err = init_dpm_write(no))				return(-1);
	if((dpm_icu_inf[no].i = (short)_icu_dev_hunt( dpm_int_hdr[no]))
	    == -1)						return(-1);
	/* エンディアンに関係無く uc[0]:下位 uc[1]:上位 */
	dpm_icu_inf[no].uc[0] = (unsigned char)(dpm_icu_inf[no].i & (short)0x00ff);
	dpm_icu_inf[no].uc[1] = (unsigned char)(dpm_icu_inf[no].i & (short)0xff00);
	ctp->ipfw = _get_ipfw( (int)dpm_icu_inf[no].uc[1]);
	if((phy_inf = _dpm_init(no)) == NULL)			return(-1);
	ctp->iobase = phy_inf->iobase;
	ctp->mem_type = phy_inf->mem_type;
	ctp->wrcmd = phy_inf->wrcmd;
	ctp->rdcmd = phy_inf->rdcmd;
	j = phy_inf->mem_size /2;
	if(dev_id & DPM_DIR)  ctp->wrtxt = j;
	else		      ctp->rdtxt = j;
	ctp->wrtxt_siz = ctp->rdtxt_siz = j;

	if(ctp->mem_type == BYTE_M)
		while(verify_putc((unsigned char)BUSY_M8, (void *)(ctp->iobase+ctp->rdcmd)));
	else if(ctp->mem_type == WORD_M)
		while(verify_puti((unsigned int)BUSY_M16, (void *)(ctp->iobase+ctp->rdcmd)));
	iodrv_open[__DPMEM_-1] = dpm_open;
	return(0);
}
