/*----------------------------------------------------------------------*
 * File name	: comsini.c	/ arc-net system initialize		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/arcnet/RCS/comsini.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: comsini.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/_arcnet/rcs/comsini.c 1.1 1995/11/07 17:12:49 ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ARC-NETｺﾝﾄﾛｰﾗ用の環境設定。BIOSｺﾝﾃｷｽﾄの生成、及びﾊﾟﾗﾒｰﾀの	*/
/*	初期設定を行う。						*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	devno		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*prmp		: ﾊﾟﾗﾒｰﾀ･ｽﾄﾗｸﾄへのﾎﾟｲﾝﾀ				*/
/* 	*rx_tsk		: ｱｸｾｽ･ｱﾄﾞﾚｽへのﾎﾟｲﾝﾀ				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ success				*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<stdlib.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\typedefs.h>		/*	"		*/
#include	<rom\handle.h>			/*	"		*/
#include	<biosif\icu.h>			/* Show bios i/f	*/
#include	<biosif\com.h>			/*	"		*/
#include	<drive\iofsys.h>		/* Show drive		*/
#include	<drive\rxdefd.h>		/*	"		*/
#include	<drive\comdrva.h>		/* 	"		*/

#define		STORAGE
#include	"comctl.h"			/* Show current dir	*/
#include	"comctxt.h"			/*	"		*/
#undef		STORAGE
#include	"comrx.h"			/*	"		*/

#define	PACKET_MODE	LONG_PKT		/* Show com.h		*/

static const unsigned char	speed[] = {
		(unsigned char)0x08,
		(unsigned char)0x06,
		(unsigned char)0x04,
		(unsigned char)0x02,
		(unsigned char)0x00,
};

int
arcnet_sys_init(
	int			devno,
	const struct compk	*prmp,
	union aap		*rx_tsk
	)
{
	struct com_ctxt		*pc;		/* pointer to context	*/
	struct rxh		*ph;		/* pointer to ctl header*/
	const struct compinf	*phy_inf;	/* pnt to physical info	*/
	int			err, i;

	com_ds.ifp = (int *)&com_ds.acadr;
	if((com_rx_tsk[devno] = rx_tsk) == NULL)		return(-1);
	if(err = CRE_SEM( FIFO, &com_sem[devno].acadr,
			  com_sem_id[devno], (unsigned int)1))	return(-1);
	if(err = WAI_SEM( TIME_OUT, com_sem[devno].acadr,
			  (unsigned int)1, &com_time_out))	return(-1);
	if(err = CRE_SEM( FIFO, &com_tx_sem[devno].acadr,
			  com_tx_sem_id[devno], (unsigned int)1))return(-1);
	if(err = WAI_SEM( TIME_OUT, com_tx_sem[devno].acadr,
			  (unsigned int)1, &com_time_out))	return(-1);
	ph = &com_rxhd[devno];
	ph->buf_max = (unsigned short)(prmp->siz_buf-1);
	if((com_hd_tbl[devno].map_tbl = malloc((unsigned long)256)) == NULL)	return(-1);

	if((com_icu_inf[devno].i = (short)_icu_dev_hunt( com_int_hdr[devno]))
		 == -1)						return(-1);
	/* エンディアンに関係無く uc[0]:下位 uc[1]:上位 */
	com_icu_inf[devno].uc[0] = (unsigned char)(com_icu_inf[devno].i & (short)0x00ff);
	com_icu_inf[devno].uc[1] = (unsigned char)(com_icu_inf[devno].i & (short)0xff00);

	pc = &com_ctxt_tbl[devno];
	pc->ipfw = _get_ipfw( (int)com_icu_inf[devno].uc[1]);
	if((phy_inf = _arcnet_init( devno)) == NULL)		return(-1);
	pc->iobase = phy_inf->io_adr;
/*------------------------------------------------------*/
/*	ARC-NETｺﾝﾄﾛｰﾗの初期設定				*/
/*							*/
/*------------------------------------------------------*/

	ph->putp = ph->getp = RXPAGE_MIN;
	_arcnet_outpb(pc, INTMR, (unsigned char)ALL_MASK, (unsigned char)0);
	_arcnet_outp2(pc, CONFGR, (unsigned short)(((BCKPLN|ET12|NODEIDA)<<8)+0));
	_arcnet_outp2(pc, ADRHR, (unsigned short)((AUTOINC<<8)+0));
	for(i = 0; i < 2048; i++)  _arcnet_outp(pc, DATR, (unsigned char)0);
	_arcnet_outpb(pc, CONFGR, SUBAD, NODEIDA);
	_arcnet_outp(pc, NIDR, (BYTE)prmp->node_no);

	i = 50000;
	do {
		if(--i == 0)					return(-1);
		_arcnet_outp2(pc, ADRHR, (unsigned short)((RDDATA<<8)+0));
	} while(_arcnet_inp(pc, DATR) != CORE_STA);

	_arcnet_outp(pc, COMMR, CLRTX_PEND);
	_arcnet_outp(pc, COMMR, CLRRX_PEND);
	_arcnet_outpb(pc, CONFGR, SUBAD, SETUP);
	_arcnet_outp(pc, SETUPR, speed[prmp->baud]);
	_arcnet_outp(pc, COMMR, PACKET_MODE);
	_arcnet_outp(pc, COMMR, (unsigned char)(CLR_POR|CLR_RECON));
	_arcnet_outpb(pc, CONFGR, TXEN, TXEN);
	_arcnet_outp(pc, COMMR, (BYTE)(ENRX|PFNN(ph->putp)));

	tx_init_flag[devno] = 1;

	for(i=0; (i<100)&&(_arcnet_inp(pc, STATR) & RI) ; i++);
	pc->reg[INTMR] = (unsigned char)(RI|RECON);
	outp(pc->iobase+INTMR, pc->reg[INTMR]);
	iodrv_open[__ARCNET_-1] = arcnet_open;
	return(0);
}
