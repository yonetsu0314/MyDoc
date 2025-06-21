/*======================================================================
 * File name    : fif001.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	FIFO受信タスク
 *----------------------------------------------------------------------
 *$Header: p:/presto2/dmu_v7/t0051/rcs/fif001.c 2.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fif001.c $
 * リビジョン 2.2  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 2.1  1970/01/01  00:00:00  kagatume
 * 2004/02/27 19:30
 * fif_rcv_tsk においてfifoバッファをクリアする処理を関数化する
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2004/01/06 14:30
 * プログラムの整理を行う
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#include <machine.h>
#include <string.h>
#include "kernel.h"


#include	<assert.h>			/* Show MSC\INC		*/
#include	<rxif\rx116.h>			/* Show XLIB\INC	*/

#include	"rxdef.h"			/* Show Current		*/
#include	"cmfhdl.h"			/* Show Current		*/
#include	"fifinout.h"			/* Show Current		*/

#define STORAGE
#include	"fif001.h"			/* Show Current		*/
#undef STORAGE

#define FIF_BUF_size  128*1024

int		chkfifbuflen(void);


int
chkfifbuflen(){
	return(((fif_buf_count-fif_buf_cntout)<(FIF_BUF_size/1024))?1:0);
}

/*----------------------------------------------------------------------*/
/*	fif_rcv_tsk							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	コンソールから受信したメッセージの解析を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	init_code	: ﾀｽｸに与える初期値				*/
/*									*/
/* 戻り値								*/
/*	void		: 無し						*/
/*----------------------------------------------------------------------*/
static void far
fif_rcv_tsk(
	int	ch_no
	)
{
	static unsigned char	buf[FIF_MES_MAX];	/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ		*/
	int		nRead,j,i = ch_no;	/* error code		*/

	fif_buf_clear();
	for(;;) {
		SLP_TSK();
		nRead = fif_gets( &buf[0], sizeof(buf));
		if(nRead > 0) {
			if((fif_buf_count-fif_buf_cntout)<(FIF_BUF_size-nRead)){
				if(fif_buf_in <= (fif_buf+FIF_BUF_size-nRead)){
					memcpy((void *)fif_buf_in,(void *)buf,(size_t)nRead);
					fif_buf_in += nRead;
					fif_buf_count += nRead;
					if(fif_buf_in >= fif_buf+FIF_BUF_size) fif_buf_in = fif_buf;
					continue;
				}
				else{
					i = FIF_BUF_size - (fif_buf_in - fif_buf);
					j = nRead - i;
					memcpy((void *)fif_buf_in,(void *)buf,(size_t)i);
					memcpy((void *)fif_buf,(void *)(buf+i),(size_t)j);
					fif_buf_in = fif_buf + j;
					fif_buf_count += nRead;
					continue;
				}
			}
			for(i=0;i<nRead;++i){
lll:
				if((fif_buf_count-fif_buf_cntout)<FIF_BUF_size){
					*fif_buf_in++ = buf[i];
					fif_buf_count++;
					if(fif_buf_in >= fif_buf+FIF_BUF_size) fif_buf_in = fif_buf;
				}
				else{
					fif_buf_ovfl = (unsigned char)1;
					for(;;){
						if(chkfifbuflen()) break;
						dly_tsk((RELTIM)100/MSEC);
					}
					fif_buf_ovfl = (unsigned char)0;
					goto lll;
				}
			}
		}
	}
}

/*----------------------------------------------------------------------*/
/*	setup_fif_rcv							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	コンソール受信タスクの生成及び起動を行う。			*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	ch_no	: 回線番号						*/
/*									*/
/* 戻り値								*/
/*	int	: 0	･･･ complete					*/
/*		  -1	･･･ error code					*/
/*----------------------------------------------------------------------*/

int
setup_fif_rcv(void)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/
	int		err;			/* error code		*/

	tsk_pkt.tsk_adr = fif_rcv_tsk;
	tsk_pkt.priority = FIFRCV_TSK_PRI;
	tsk_pkt.stk_size = FIF_RCV_TSK_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = apl_ds.acadr;

	err = CRE_TSK( &fif_rcv_tsk_aa.acadr, FIFRCV_TSK_ID, &tsk_pkt);
	assert( !err );
	err = STA_TSK( fif_rcv_tsk_aa.acadr, 0 );
	assert( !err );
	return(err);
}
