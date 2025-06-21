/*----------------------------------------------------------------------*
 * File name	: dpmrd.c	/ dpmem text read			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmrd.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmrd.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmrd.c 1.1 1993/10/20 20:05:36 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信バッファの内容を、指定バイト数または、テキスト単位でユ	*/
/*	ーザー･バッファにコピーする。 バッファの内容はバイナリーテ	*/
/*	キストを対象としているため、先頭に管理用ヘッダー部としてデ	*/
/*	ータ長(int)が付加されているが、テキストからは除去され、戻り	*/
/*	値として返される。						*/
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
/*	max 		: maximum length				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ ｽﾄﾘﾝｸﾞ無し				*/
/*			  n	･･･ character length			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\dpmdrv.h>		/*	"		*/
#include	"dpmctl.h"			/* Show current		*/
#include	"dpmctxt.h"			/*	"		*/

static unsigned int	dpmrx_sem_id[DPM_DEV_MAX] = {
		DPM0RX_SEM_ID, DPM1RX_SEM_ID
};
static union aap	dpm_rx_sem[DPM_DEV_MAX] = {
		0L			/* DPMRXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
static unsigned char	dpm_rx_nest[DPM_DEV_MAX] = {
		(unsigned char)0	/* RD関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};

int
dpm_read(
	int		channel,		/* channel number	*/
	unsigned char	*buf,			/* string buffer pointer*/
	int		max			/* maxmum length	*/
	)
{
	struct dpm_ctxt		*ctp;		/* pointer to context	*/
	unsigned char far	*txtp;		/* poiter to text buff	*/
	int			len;		/* number of byte	*/
	unsigned char		cmd;
	int			err =0;

	if(max == 0)						return(0);
	ctp = &dpm_ctxt_tbl[channel];
	txtp = ctp->iobase +ctp->rdtxt;
	ADV_WAI_SEM(err, dpm_rx_nest[channel], ENDLESS,
		    dpm_rx_sem[channel].acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);
	if(((len = *WD_PTR(txtp))== 0) || (len+1 > max))
		len = -1;
	else {
		memcpy((void *)buf, (void *)(txtp+sizeof(len)), (unsigned long)len);
		if(ctp->mem_type == BYTE_M) {
			cmd = (unsigned char)verify_getc((void *)(ctp->iobase+ctp->rdcmd));
			while(verify_putc((unsigned char)CMD_ACK,
					(void *)(ctp->iobase+ctp->rdcmd)));
		}
		else if(ctp->mem_type == WORD_M) {
			cmd = (unsigned char)verify_geti((void *)(ctp->iobase+ctp->rdcmd));
			while(verify_puti((unsigned int)CMD_ACK,
					(void *)(ctp->iobase+ctp->rdcmd)));
		}
		buf[len] = cmd;
	}
	ADV_SIG_SEM(err, dpm_rx_nest[channel], dpm_rx_sem[channel].acadr);
	if(err)							return(err|INT_MIN);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_dpm_read							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	no	: チャネルナンバー					*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ 成功					*/
/*		  -1	･･･ error					*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	受信関係の処理の初期化を行う。					*/
/*----------------------------------------------------------------------*/

int
init_dpm_read(
	int		no
	)
{
	int		err;

	if(err = CRE_SEM( FIFO, &dpm_rx_sem[no].acadr,
			dpmrx_sem_id[no], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, dpm_rx_sem[no].acadr,
			(unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
