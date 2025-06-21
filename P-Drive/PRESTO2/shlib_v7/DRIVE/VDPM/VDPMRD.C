/*----------------------------------------------------------------------*
 * File name	: vdpmrd.c	/ virtual dpmem text read		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpmrd.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: vdpmrd.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/03/13 11:15
 * 左辺のキャストをやめる。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/vdpm/rcs/vdpmrd.c 1.1 1995/09/16 14:29:30 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
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
#include	<custom\custom.h>			/* Show xlib		*/
#include	<drive\vdpmdrv.h>		/* Show drive		*/
#include	"vdpmctl.h"			/* Show current		*/
#include	"vdpmctxt.h"			/*	"		*/

int
vdpm_read(
	int		channel,		/* channel number	*/
	char		*buf,			/* string buffer pointer*/
	int		max			/* maxmum length	*/
	)
{
	struct vdpm_ctxt	*ctp;		/* pointer to context	*/
	unsigned char		*txtp;		/* poiter to text buff	*/
	int			len;		/* number of byte	*/
	unsigned char		cmd;
	int			i, err =0;
	unsigned short		*p;

	if(max == 0)						return(0);
	ctp = &vdpm_ctxt_tbl[channel];
	txtp = ctp->iobase +ctp->rdtxt;
	p = (unsigned short *)(ctp->iobase +ctp->rdtxt);
	if((len = *p)== 0 || (len+1 > max))
		len = -1;
	else {
		memcpy((void *)buf, (void *)(txtp+sizeof(short)), (size_t)len);
		if(ctp->mem_type == BYTE_M) {
			cmd = *(ctp->iobase + ctp->rdcmd);
			*(ctp->iobase + ctp->rdcmd) = (unsigned char)CMD_ACK;
		}
		else if(ctp->mem_type == WORD_M) {
			p = (unsigned short *)(ctp->iobase + ctp->rdcmd);
			cmd = (unsigned char)(*p);
			*p = (unsigned short)CMD_ACK;
		}
		buf[len] = cmd;
	}
	i = channel^VDPM_DIR;
	ADV_SIG_SEM(err, vdpm_nest[i], vdpm_sem[i].acadr);
	if(err)							return(err|INT_MIN);
	return(len);
}

/*----------------------------------------------------------------------*/
/*	init_vdpm_read							*/
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
init_vdpm_read(
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
