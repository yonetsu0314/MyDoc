/*----------------------------------------------------------------------*
 * File name	: dadsini.c	/ system initialize			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/pdadc/RCS/dadsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/drive/ndadc/rcs/dadsini.c 1.3 1970/01/01
 *Log: dadsini.c
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2003/08/12 ワーニング削除
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  m-yamana
 * 2003/06/30  15:50
 * ・基板(P5561/P5554)に対応させた。
 *   コンパイル定数 MAS_P5561 を定義することでP5561(坂口氏作成)に、
 *   定義しないことでP5554(村田氏作成)に対応させた。
 *   
 * ・エンディアン(ビッグ/リトル)に対応させた。
 *   コンパイル定数 MAS_BIG_ENDIAN を定義することでビッグエンディアンに、
 *   定義しないことでリトルエンディアンに対応させた。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/ndadc/rcs/dadsini.c 1.1 1996/07/22 13:37:18 chimura
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
/*	pwb		: ﾎﾞｰﾄﾞ･ﾅﾝﾊﾞｰ					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\icu.h>			/* Show bios		*/
#include	<biosif\dadc.h>			/*	"		*/
#include	<biosif\dpmem.h>		/*	"		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\pdadcdrv.h>		/*	"		*/
#define		STORAGE				/* 確保/外部参照	*/
#include	"dadctxt.h"			/* Show current		*/
#include	"dadctl.h"			/*	"		*/
#undef		STORAGE

static const enum dpmpid idno[] = {
		WCU_DAD1, WCU_DAD2,		/* dual port mem id no.	*/
		WCU_DAD3, WCU_DAD4		/* dual port mem id no.	*/
};

int
dad_sys_init(					/* stack = 16byte	*/
	DAD_ID		pwb
	)
{
	struct dadctxt		*ctp;		/* pnt to DADC context	*/
	const struct dadpinf	*dad_inf;	/* pnt to physical info	*/
	const struct dpmpinf	*phy_inf;	/* pnt to physical info	*/
	short syamana;
	int   iyamana;

	dad_ds.ifp = (int *)&dad_ds.acadr;
	if(init_dad_dpm_read(pwb))				return(-1);
	if(init_dad_dpm_write(pwb))				return(-1);
	if((dad_icu_inf[pwb].i =
	    (short)_icu_dev_hunt( dad_int_hdr[pwb])) == -1)	return(-1);
	/* エンディアンに関係無く uc[0]:下位 uc[1]:上位 */
	dad_icu_inf[pwb].uc[0] = (unsigned char)(dad_icu_inf[pwb].i & (short)0x00ff);
	dad_icu_inf[pwb].uc[1] = (unsigned char)(dad_icu_inf[pwb].i & (short)0xff00);
	dadc_id_io.ipfw = _get_ipfw( (int)dad_icu_inf[pwb].uc[1]);
/*
	if((dad_inf = _dadc_init( pwb)) == NULL)		return(-1);
	dadc_id_io.io_adr[pwb] = dad_inf->io_adr;
	dadc_id_io.ptn[pwb].us = dad_inf->ptn.us;
*/
	if((phy_inf = _dpm_init( idno[pwb])) == NULL)		return(-1);	/* [_dpm_init() p:/presto/shlib/biosif/dpmem/RCS/dpminit.c] -> [trapa_svc()] -> [p:/presto/bios/RCS/dpmemio.c] */
	ctp = &dadc_ctxt_tbl[pwb];
	ctp->iobase = phy_inf->iobase;
	ctp->mem_type = phy_inf->mem_type;
	ctp->wrcmd = phy_inf->wrcmd;
	ctp->rdcmd = phy_inf->rdcmd;
	ctp->wrtxt = 0;
	ctp->wrtxt_siz = ((phy_inf->mem_size/2)+1023)/1024*1024;
	ctp->rdtxt = ctp->wrtxt_siz;
	ctp->rdtxt_siz = phy_inf->mem_size-ctp->wrtxt_siz;
	if(ctp->wrtxt_siz < ctp->rdtxt_siz)			return(-1);

	syamana = (short)BUSY_M16;
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_SHORT(syamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(syamana);
#endif
	do {
		iyamana = verify_puti((unsigned short)syamana, (unsigned short *)(ctp->iobase + ctp->rdcmd));
	} while(iyamana);

/*	if(init_dadi_tsk(pwb))					return(-1);*/
	iyamana = init_dadi_tsk(pwb);
	if(iyamana) {
		return(-1);
	}

	syamana = (short)CMD_ACK;
#ifdef	MAS_BIG_ENDIAN
	CHG_UL_SHORT(syamana);
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
	INVERSION(syamana);
#endif
	do {
		iyamana = verify_puti((unsigned short)syamana, (unsigned short *)(ctp->iobase + ctp->rdcmd));
	} while(iyamana);

	return(0);
}
