/*----------------------------------------------------------------------*
 * File name	: vdpmwr.c	/ virtual dpmem text write		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpmwr.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: vdpmwr.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/13 11:15
 * 左辺のキャストをやめる。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/vdpm/rcs/vdpmwr.c 1.1 1995/09/16 14:29:35 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	バッファの内容はバイナリーのテキストを対象としている為、先	*/
/*	頭に管理用データ長のヘッダー部を付加し、デュアルポート･RAM	*/
/*	に書き込みを行う。						*/
/*									*/
/*	<<< text format >>>						*/
/*		+-------------------- // ---------------+-------+	*/
/*		|<---------       len       ----------->|(char)	|	*/
/*		|             CMD & PARAMETER		| NODE	|	*/
/*		+-------------------- // ---------------+-------+	*/
/*		buf[0] buf[2]	 buf[3]	･･･		 buf[len]	*/
/*									*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	channel		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*buf		: string buffer					*/
/*	len		: number of byte				*/
/*									*/
/* 戻り値								*/
/*	int		: n	･･･ 書き込んだﾊﾞｲﾄ数			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\custom.h>			/* Show xlib		*/
#include	<drive\vdpmdrv.h>		/* Show drive		*/
#include	"vdpmctl.h"			/* Show current		*/
#include	"vdpmctxt.h"			/*	"		*/

int
vdpm_write(
	int		channel,		/* channel number	*/
	char		*buf,			/* string buffer pointer*/
	int		len			/* number of byte	*/
	)
{
	struct vdpm_ctxt	*ctp;		/* pointer to context	*/
	unsigned char		*txtp;		/* poiter to text buff	*/
	int			err =0;
	unsigned short		*p;

	ctp = &vdpm_ctxt_tbl[channel];
	txtp = ctp->iobase +ctp->wrtxt;
	if(len +sizeof(short) > ctp->rdtxt_siz)			return(-1);
	ADV_WAI_SEM(err, vdpm_nest[channel], ENDLESS,
		    vdpm_sem[channel].acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);
	if(ctp ->mem_type == BYTE_M) {
		while(*(ctp->iobase + ctp->wrcmd) != CMD_ACK)
			if(err = HLT_TSK(&vdpm_poll_time))	return(err|INT_MIN);
		p = (unsigned short *)txtp;
		*p = (unsigned short)len;
		memcpy((void *)(txtp +sizeof(short)), (void *)buf, (size_t)len);
		*(ctp->iobase + ctp->wrcmd) = buf[len];
	}
	else if(ctp ->mem_type == WORD_M) {
		p = (unsigned short *)(ctp->iobase + ctp->wrcmd);
		while(*p != CMD_ACK) {
			if(err = HLT_TSK(&vdpm_poll_time))	return(err|INT_MIN);
		}
		p = (unsigned short *)txtp;
		*p = (unsigned short)len;
		memcpy((void *)(txtp +sizeof(len)), (void *)buf, (size_t)len);
		p = (unsigned short *)(ctp->iobase + ctp->wrcmd);
		*WD_PTR(ctp->iobase + ctp->wrcmd) = buf[len];
	}
	WUP_TSK(vdpm_rx_tsk[channel][buf[len]].acadr);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_vdpm_write							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	no	: チャネルナンバー					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	送信関係の処理の初期化を行う。					*/
/*----------------------------------------------------------------------*/

int
init_vdpm_write(
	int		no
	)
{
	int		err;

	if(vdpm_sem[no].acadr)					return(0);
	if(err = CRE_SEM( FIFO, &vdpm_sem[no].acadr,
			vdpm_sem_id[no], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, vdpm_sem[no].acadr,
			(unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
