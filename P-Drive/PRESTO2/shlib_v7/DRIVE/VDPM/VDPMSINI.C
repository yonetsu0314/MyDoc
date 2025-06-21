/*----------------------------------------------------------------------*
 * File name	: vdpmsini.c	/ system initialize			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpmsini.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: vdpmsini.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/13 11:15
 * リード、ライトの境界が奇数アドレスにならないようにする。
 * 奇数アドレスにワード、ダブルワードアクセスすると例外処理（アドレスエラー）
 * が発生する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/vdpm/rcs/vdpmsini.c 1.1 1995/09/16 14:29:33 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｾﾏﾌｫ、割り込みﾀｽｸ及びBIOSｺﾝﾃｷｽﾄの生成を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	dev_typ		: ﾃﾞﾊﾞｲｽ･ﾀｲﾌﾟ					*/
/*	*rx_tsk0	: dir0のｱｸｾｽ･ｱﾄﾞﾚｽの配列へのfarﾎﾟｲﾝﾀｰ		*/
/*	*rx_tsk1	: dir1のｱｸｾｽ･ｱﾄﾞﾚｽの配列へのfarﾎﾟｲﾝﾀｰ		*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ 正常終了				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<rom\handle.h>			/*	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\rxdefd.h>		/*	"		*/
#include	<drive\vdpmdrv.h>		/*	"		*/

#define		STORAGE				/* 確保/外部参照	*/
#include	"vdpmctxt.h"			/* Show current		*/
#include	"vdpmctl.h"			/*	"		*/
#undef		STORAGE

struct pinf {
	unsigned int	mem_size;		/* メモリーサイズ	*/
	unsigned int	wrcmd;			/* コマンドWRレジスタ	*/
	unsigned int	rdcmd;			/* コマンドRDレジスタ	*/
}
static ioinf[] = {
	0x0800, 0x07fe, 0x07ff,
	0x2000, 0x1ffc, 0x1ffe,
};

int
vdpm_sys_init(
	DPMTYP		dev_typ,
	union aap	*rx_tsk0,
	union aap	*rx_tsk1
	)
{
	struct vdpm_ctxt	*ctp[VDPMDIR_MAX];	/* pointer to contxt */
	const struct pinf	*phy_inf;		/*& to physical info */
	int			err, j;
	unsigned short		*p;

	vdpm_ds.ifp = (int *)&vdpm_ds.acadr;
	if((vdpm_rx_tsk[VDPMDIR0] = rx_tsk0)== NULL)		return(-1);
	if((vdpm_rx_tsk[VDPMDIR1] = rx_tsk1)== NULL)		return(-1);

	ctp[VDPMDIR0] = &vdpm_ctxt_tbl[VDPMDIR0];
	ctp[VDPMDIR1] = &vdpm_ctxt_tbl[VDPMDIR1];

	if(err = init_vdpm_read(VDPMDIR0^VDPM_DIR))		return(-1);
	if(err = init_vdpm_write(VDPMDIR0))			return(-1);

	phy_inf = &ioinf[dev_typ>>1];
	if((ctp[VDPMDIR0]->iobase = malloc((size_t)phy_inf->mem_size)) == NULL)
								return(-1);
	ctp[VDPMDIR1]->iobase = vdpm_ctxt_tbl[VDPMDIR0].iobase;

	ctp[VDPMDIR0]->mem_type = ctp[VDPMDIR1]->mem_type = dev_typ & WORD_M;

	j = phy_inf->mem_size/2 - (ctp[VDPMDIR0]->mem_type +1)*4;

	if(ctp[VDPMDIR0]->mem_type == BYTE_M){
		ctp[VDPMDIR0]->wrcmd = ctp[VDPMDIR1]->rdcmd = phy_inf->wrcmd;
		ctp[VDPMDIR0]->rdcmd = ctp[VDPMDIR1]->wrcmd = phy_inf->rdcmd;
	}else if(ctp[VDPMDIR0]->mem_type == WORD_M){
		ctp[VDPMDIR0]->wrcmd = ctp[VDPMDIR1]->rdcmd = phy_inf->wrcmd;
		ctp[VDPMDIR0]->rdcmd = ctp[VDPMDIR1]->wrcmd = phy_inf->rdcmd;
	}
	ctp[VDPMDIR0]->rdtxt = ctp[VDPMDIR1]->wrtxt = j;
	ctp[VDPMDIR0]->wrtxt_siz = ctp[VDPMDIR0]->rdtxt_siz = j;
	ctp[VDPMDIR1]->wrtxt_siz = ctp[VDPMDIR1]->rdtxt_siz = j;

	if(ctp[VDPMDIR0]->mem_type == BYTE_M){
		*(ctp[VDPMDIR0]->iobase+ctp[VDPMDIR0]->rdcmd) = (unsigned char)CMD_ACK;
		*(ctp[VDPMDIR1]->iobase+ctp[VDPMDIR1]->rdcmd) = (unsigned char)CMD_ACK;
	}else if(ctp[VDPMDIR0]->mem_type == WORD_M){
		p = (unsigned short *)(ctp[VDPMDIR0]->iobase+ctp[VDPMDIR0]->rdcmd);
		*p = (unsigned short)CMD_ACK;
		p = (unsigned short *)(ctp[VDPMDIR1]->iobase+ctp[VDPMDIR1]->rdcmd);
		*p = (unsigned short)CMD_ACK;
	}

	return(0);
}
