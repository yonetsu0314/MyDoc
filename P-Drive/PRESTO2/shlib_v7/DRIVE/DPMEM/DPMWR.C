/*----------------------------------------------------------------------*
 * File name	: dpmwr.c	/ dpmem text write			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/dpmem/RCS/dpmwr.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmwr.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/dpmem/rcs/dpmwr.c 1.1 1993/10/20 20:07:00 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
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
#include	<custom\custom.h>		/*	"		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\dpmdrv.h>		/*	"		*/
#include	"dpmctl.h"			/* Show current		*/
#include	"dpmctxt.h"			/*	"		*/

static unsigned int	dpmtx_sem_id[DPM_DEV_MAX] = {
		DPM0TX_SEM_ID, DPM1TX_SEM_ID
};
static union aap	dpm_tx_sem[DPM_DEV_MAX] = {
		0L			/* DPMTXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
static unsigned char	dpm_tx_nest[DPM_DEV_MAX] = {
		(unsigned char)0	/* WR関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};

int
dpm_write(
	int		channel,		/* channel number	*/
	unsigned char	*buf,			/* string buffer pointer*/
	int		len			/* number of byte	*/
	)
{
	struct dpm_ctxt		*ctp;		/* pointer to context	*/
	unsigned char far	*txtp;		/* poiter to text buff	*/
	int			err =0;

	ctp = &dpm_ctxt_tbl[channel];
	txtp = ctp->iobase +ctp->wrtxt;
	if(len +sizeof(len) > ctp->rdtxt_siz)			return(-1);
	ADV_WAI_SEM(err, dpm_tx_nest[channel], ENDLESS,
		    dpm_tx_sem[channel].acadr, VOID_TIME);
	if(err)							return(err|INT_MIN);
	if(ctp ->mem_type == BYTE_M) {
		while(*(ctp->iobase+S_PTR(ctp->wrcmd)) != CMD_ACK)
			if(err = HLT_TSK(&dpm_poll_time))	return(err|INT_MIN);
		*WD_PTR(txtp) = (unsigned short)len;
		memcpy((void *)(txtp +sizeof(len)), (void *)buf, (unsigned long)len);
		while(verify_putc( buf[len],
				  (void *)(ctp->iobase +ctp->wrcmd)));
	}
	else if(ctp ->mem_type == WORD_M) {
		while(verify_geti((void *)(ctp->iobase+ctp->wrcmd)) != CMD_ACK) {
			if(err = HLT_TSK(&dpm_poll_time))	return(err|INT_MIN);
		}
		*WD_PTR(txtp) = (unsigned short)len;
		memcpy((void *)(txtp +sizeof(len)), (void *)buf, (unsigned long)len);
		while(verify_puti(*(unsigned int *)(buf+len),
				  (void *)(ctp->iobase +ctp->wrcmd)));
	}
	ADV_SIG_SEM(err, dpm_tx_nest[channel], dpm_tx_sem[channel].acadr);
	if(err)							return(err|INT_MIN);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_dpm_write							*/
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
init_dpm_write(
	int		no
	)
{
	int		err;

	if(err = CRE_SEM( FIFO, &dpm_tx_sem[no].acadr,
			dpmtx_sem_id[no], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( ENDLESS, dpm_tx_sem[no].acadr,
			(unsigned int)1, VOID_TIME))		return(-1);
	return(0);
}
