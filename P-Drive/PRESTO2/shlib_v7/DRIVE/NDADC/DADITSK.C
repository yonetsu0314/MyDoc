/*----------------------------------------------------------------------*
 * File name	: daditsk.c	/ dadc inturrupt task			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/ndadc/RCS/daditsk.c 1.5 1970/01/01 00:00:00 chimura Exp $
 *$Log: daditsk.c $
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/10/06 13:30
 * メールボックス処理をRX116コンバータライブラリーを使用せず
 * NORTI4の記述に変更する。
 * コンバータライブラリーの不具合対策。（CNV.LIBは後日変更する）
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/08/26 20:00
 * AFV調整コンプリートコマンド受信処理の不具合修正
 * 
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
 *Header: p:/lib/drive/ndadc/rcs/daditsk.c 1.1 1996/07/22 13:36:41 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ADCﾎﾞｰﾄﾞ(P-5350)からの割り込みﾀｽｸ。受信したﾃｷｽﾄのｾｰﾌﾞまたは	*/
/*	動作完了のｲﾍﾞﾝﾄ･ﾌﾗｸﾞのｾｯﾄを行う。				*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb	: DADC board number					*/
/*									*/
/* 戻り値								*/
/*	void	: 無し							*/
/*----------------------------------------------------------------------*/

#include	<assert.h>			/* ANSI std		*/
#include	<stddef.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<rxif\rx116.h>			/* Show xlib		*/
#include	<custom\xmemory.h>		/*	"		*/
#include	<biosif\dadc.h>			/* Show biosif		*/
#include	<drive\rxdefd.h>		/* Show drive		*/
#include	<drive\ndadcdrv.h>		/*	"		*/
#include	"dadctxt.h"			/* Show current		*/
#include	"dadctl.h"			/*	"		*/

#ifndef SH3
#define SH3                 /* SH3 を定義してkernel.hをincludeしてください */
#endif
#include	<kernel.h>

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text);		/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
static void chg_ul_aft_read1(struct dades10 *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
static void chg_ul_aft_read2(struct dadsck *text);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */

typedef struct	msgfmt {			/* メッセージヘッダ	*/
	int		pri;			/* 優先度		*/
	int		size;			/* サイズ(バイト数)	*/
	int		ch;			/* channel		*/
	int		cmd;			/* command		*/
} MSGFMT;

#define	PARA		16
#define	BLK_SIZE	(unsigned int)((sizeof(MSGFMT)+(PARA-1))/PARA)

static union aap	mpl0;			/* メモリプール０のAA	*/
static union aap	dad_boot_evf[DAD_MAX];	/* boot完了ﾒｰﾙBOXのAA	*/
static union aap	dad_zc_mbx[DAD_MAX];	/* 零点完了ﾌﾗｸﾞのAA	*/
static union aap	dad_spn_evf[DAD_MAX];	/* ｽﾊﾟﾝ完了ﾌﾗｸﾞのAA	*/
static union aap	dad_afv_evf[DAD_MAX];	/* AFV完了ﾌﾗｸﾞのAA	*/
static union aap	dad_afv_mbx;		/*    "	  ﾒｰﾙBOXのAA	*/
static union aap	dad_mrd_evf[DAD_MAX];	/* ﾒﾓﾘｰ･ﾘｰﾄﾞ完了ﾌﾗｸﾞのAA*/
static union aap	dad_mwr_evf[DAD_MAX];	/* ﾒﾓﾘｰ･ﾗｲﾄ完了ﾌﾗｸﾞのAA	*/
static union aap	dad_stam_evf[DAD_MAX];	/* ﾓﾆﾀｰｽﾀｰﾄ完了ﾌﾗｸﾞのAA	*/
static union aap	dad_stpm_evf[DAD_MAX];	/* ﾓﾆﾀｰｽﾄｯﾌﾟ完了ﾌﾗｸﾞのAA*/

struct txt {				/* text format		*/
	short	len;			/* sizeof(struct txt)	*/
	short	cmd;			/* command		*/
/*	short	prm; */			/* parameter		*/
	long	prm;			/* parameter		*//* m-yamana はたして [2] でいいんだろうか？ */
};

typedef union mbuf {
	int		ibuf;
	unsigned short	sbuf[2];
	unsigned char	cbuf[4];
} TT_MBUF;

typedef struct ttmsg{
	void		*next;
	TT_MBUF		buf;
} TT_MSG;

static void
dad_int_tsk(
	int		pwb
	)
{
	struct dadctxt	*ctp;			/* ｺﾝﾃｷｽﾄへのfarﾎﾟｲﾝﾀｰ	*/
	unsigned int	ptn;
	int		err, cmd;
	long		time = 3000L;
	union aap	blk;
	struct msgfmt	msg;			/* message format	*/
	struct txt	*txtp;
	int		ui_yamana;
	long		afv_bp;
	TT_MSG		*tmsg;

	ctp = &dadc_ctxt_tbl[pwb];
	txtp = (struct txt *)(ctp->iobase + ctp->rdtxt);
	while(1) {
		err = SLP_TSK();
		assert(!err);
#ifdef	MAS_BIG_ENDIAN
		chg_ul_bef_write(txtp);	/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
#endif	/* MAS_BIG_ENDIAN */
#ifdef	MAS_P5561
		inverse_dadc((char *)txtp, (short)sizeof(txtp));
#endif
		switch(cmd = (int)txtp->cmd) {
		case EOF_INIT_BOOT0:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_boot_evf[pwb].acadr, (unsigned int)INIT_BOOT0_COMPL);
			assert(!err);
			break;
		case EOF_CHG_BOOT0:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_boot_evf[pwb].acadr, (unsigned int)CHG_BOOT0_COMPL);
			assert(!err);
			break;
		case EOF_SPAN:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_spn_evf[pwb].acadr, (unsigned int)EOF_SPAN);
			assert(!err);
			break;
		case EOF_AFV:
			afv_bp = txtp->prm;
#ifdef	MAS_P5561
			INVERSION(afv_bp);
#endif
			if(afv_bp) {
				err = get_mpl((ID)mpl0.acadr, 64, &tmsg);
//				err = GET_BLK(TIME_OUT, &blk.acadr, mpl0.acadr,
//						BLK_SIZE, &time);
				assert(!err);
				msg.pri  = 0;
				msg.size = sizeof(msg);
				msg.ch   = pwb;
				msg.cmd	 = EOF_AFV;
				memcpy( &tmsg->buf.cbuf[0], &msg, sizeof(msg) );
				err = snd_mbx( (ID)dad_afv_mbx.acadr, tmsg );
//				blk.ary[0] = 0;
//				memcpy( blk.vfp, (void *)&msg, sizeof(msg));
//				err = SND_MSG( dad_afv_mbx.acadr, blk.acadr );
				assert(!err);
			}
			else {
				err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
					       dad_afv_evf[pwb].acadr, (unsigned int)EOF_AFV);
				assert(!err);
			}
			break;
		case AFV_ERR:
			err = dad_dpm_read((DAD_ID)pwb, (void *)&dad_afv_err[pwb],
					   sizeof(dad_afv_err[pwb]),
					   (unsigned short)sizeof(txtp->cmd));
			assert(err>0);
#ifdef	MAS_BIG_ENDIAN
			chg_ul_aft_read1(&dad_afv_err[pwb]);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
			continue;
		case SELF_CHK:
			err = dad_dpm_read((DAD_ID)pwb, (void *)&dad_slf_chk[pwb],
					   sizeof(dad_slf_chk[pwb]),
					   (unsigned short)sizeof(txtp->cmd));
			assert(err>0);
#ifdef	MAS_BIG_ENDIAN
			chg_ul_aft_read2(&dad_slf_chk[pwb]);	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
#endif	/* MAS_BIG_ENDIAN */
			continue;
		case EOF_INIT_BOOT1:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_boot_evf[pwb].acadr, (unsigned int)INIT_BOOT1_COMPL);
			assert(!err);
			break;
		case EOF_CHG_BOOT1:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_boot_evf[pwb].acadr, (unsigned int)CHG_BOOT1_COMPL);
			assert(!err);
			break;
		case EOF_ZERO:
			err = get_mpl((ID)mpl0.acadr, 64, &tmsg);
//			err = GET_BLK(TIME_OUT, &blk.acadr, mpl0.acadr, BLK_SIZE, &time);
			assert(!err);
			msg.pri  = 0;
			msg.size = sizeof(msg);
			msg.ch   = pwb;
			msg.cmd	 = EOF_ZERO;
			memcpy( &tmsg->buf.cbuf[0], &msg, sizeof(msg) );
			err = snd_mbx( (ID)dad_zc_mbx[pwb].acadr, tmsg );
//			blk.ary[0] = 0;
//			memcpy( blk.vfp, (void *)&msg, sizeof(msg));
//			err = SND_MSG( dad_zc_mbx[pwb].acadr, blk.acadr );
			assert(!err);
			break;
		case EOF_MEM_READ:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_mrd_evf[pwb].acadr, (unsigned int)EOF_MEM_READ);
			assert(!err);
			break;
		case EOF_MEM_WRITE:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_mwr_evf[pwb].acadr, (unsigned int)EOF_MEM_WRITE);
			assert(!err);
			break;
		case EOF_STA_MON:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_stam_evf[pwb].acadr, (unsigned int)EOF_STA_MON);
			assert(!err);
			break;
		case EOF_STP_MON:
			err = SET_FLG( (unsigned int)EVF_SET_REPL, &ptn,
				       dad_stpm_evf[pwb].acadr, (unsigned int)EOF_STP_MON);
			assert(!err);
			break;
		default:		/* undefine command		*/
			ui_yamana = (unsigned int)CMD_ACK;
#ifdef	MAS_P5561
			INVERSION(ui_yamana);
#endif
			verify_puti((unsigned short)ui_yamana, (unsigned short *)(ctp->iobase+ctp->rdcmd));
			break;
		}
	}
}

/*----------------------------------------------------------------------*/
/*	init_dadi_tsk							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: DADC board number				*/
/*									*/
/* 戻り値								*/
/*	int		: error						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	dad_int_tsk()関係の初期化を行う。				*/
/*----------------------------------------------------------------------*/

#ifdef	NDEBUG
#define	DEF_STK		0x100
#else
#define	DEF_STK		0x200
#endif

int
init_dadi_tsk(
	DAD_ID		pwb			/* DADC board number	*/
	)
{
	struct pktsk	tsk_pkt;		/* ﾀｽｸ生成用ﾊﾟｹｯﾄ	*/
	unsigned int	id[DAD_MAX];

	if(mpl0.vfp == NULL) {
/*		if(MPL_ADR( &mpl0.acadr, (unsigned int)0))	return(-1);*/
		if(MPL_ADR( &mpl0.acadr, (unsigned int)1))	return(-1);	/* m-yamana */
	}
	if(dad_afv_mbx.acadr == 0) {
		if(CRE_MBX((unsigned int)3, &dad_afv_mbx.acadr,
			(unsigned int)DADAFV_MBX_ID))		return(-1);
	}
	id[DADC1] = DADBOT1_EVF_ID;
	id[DADC2] = DADBOT2_EVF_ID;
	if(CRE_FLG( &dad_boot_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DAD1_MBX_ID;
	id[DADC2] = DAD2_MBX_ID;
	if(CRE_MBX((unsigned int)3, &dad_zc_mbx[pwb].acadr, id[pwb]))	return(-1);
	id[DADC1] = DADSPC1_EVF_ID;
	id[DADC2] = DADSPC2_EVF_ID;
	if(CRE_FLG( &dad_spn_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DADAFV1_EVF_ID;
	id[DADC2] = DADAFV2_EVF_ID;
	if(CRE_FLG( &dad_afv_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DADRC1_EVF_ID;
	id[DADC2] = DADRC2_EVF_ID;
	if(CRE_FLG( &dad_mrd_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DADMW1_EVF_ID;
	id[DADC2] = DADMW2_EVF_ID;
	if(CRE_FLG( &dad_mwr_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DADSTAM1_EVF_ID;
	id[DADC2] = DADSTAM2_EVF_ID;
	if(CRE_FLG( &dad_stam_evf[pwb].acadr, id[pwb]))		return(-1);
	id[DADC1] = DADSTPM1_EVF_ID;
	id[DADC2] = DADSTPM2_EVF_ID;
	if(CRE_FLG( &dad_stpm_evf[pwb].acadr, id[pwb]))		return(-1);

	tsk_pkt.tsk_adr = dad_int_tsk;
	tsk_pkt.priority = dadi_tsk_pri[pwb];
	tsk_pkt.stk_size = DEF_STK;
	tsk_pkt.option = 0;
	tsk_pkt.data_seg = dad_ds.acadr;
	if(CRE_TSK( &dadi_tsk[pwb].acadr,
		    dadi_tsk_id[pwb], &tsk_pkt))		return(-1);
	if(STA_TSK( dadi_tsk[pwb].acadr, (unsigned int)pwb))	return(-1);
	return(0);
}

/*----------------------------------------------------------------------*/
/*	wai_????_flg							*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: DADC board number				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ successful complete			*/
/*			  -1	･･･ error				*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｺﾏﾝﾄﾞ応答のﾌﾗｸﾞ待ちを行う。					*/
/*----------------------------------------------------------------------*/

int wai_span_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_spn_evf[pwb].acadr,
		    (unsigned int)EOF_SPAN, &dad_tmout))	return(-1);
	return(0);
}

int wai_afv_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_afv_evf[pwb].acadr,
		    (unsigned int)EOF_AFV, &dad_tmout))		return(-1);
	return(0);
}

int wai_afv_msg(DAD_ID *pwb)
{
	union aap	blk;
	struct msgfmt	msg;
	TT_MSG		*tmsg;

	if(rcv_mbx((ID)dad_afv_mbx.acadr, &tmsg))		return(-1);
	memcpy( &msg, &tmsg->buf.cbuf[0], sizeof(msg) );
	if(rel_mpl(1, tmsg))					return(-1);
//	if(RCV_MSG( ENDLESS, &blk.acadr, dad_afv_mbx.acadr,
//		    VOID_TIME))					return(-1);
//	blk.ary[0] = 0;
//	memcpy( (void *)&msg, blk.vfp, sizeof(msg));
//	if( REL_BLK( blk.acadr ))				return(-1);
	*pwb = msg.ch;
	return(0);
}

int wai_init_boot_flg(DAD_ID pwb, int bp)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_boot_evf[pwb].acadr,
		    (unsigned int)bp, &dad_tmout))		return(-1);
	return(0);
}

int wai_chg_boot_flg(DAD_ID pwb, int bp)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_boot_evf[pwb].acadr,
		    (unsigned int)bp, &dad_tmout))		return(-1);
	return(0);
}

int wai_zero_flg(DAD_ID pwb)
{
	union aap	blk;
	struct msgfmt	msg;
	TT_MSG		*tmsg;

	if(rcv_mbx((ID)dad_zc_mbx[pwb].acadr, &tmsg))		return(-1);
	memcpy( &msg, &tmsg->buf.cbuf[0], sizeof(msg) );
	if(rel_mpl(1, tmsg))					return(-1);
//	if(RCV_MSG( TIME_OUT, &blk.acadr, dad_zc_mbx[pwb].acadr,
//			&dad_tmout))				return(-1);
//	blk.ary[0] = 0;
//	memcpy( (void *)&msg, blk.vfp, sizeof(msg));
//	if( REL_BLK( blk.acadr ))				return(-1);
	if((pwb != msg.ch)||(EOF_ZERO != msg.cmd))		return(-1);
	return(0);
}

int wai_mem_read_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_mrd_evf[pwb].acadr,
		    (unsigned int)EOF_MEM_READ, &dad_tmout))	return(-1);
	return(0);
}

int wai_mem_write_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_mwr_evf[pwb].acadr,
		    (unsigned int)EOF_MEM_WRITE, &dad_tmout))	return(-1);
	return(0);
}

int wai_sta_mon_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_stam_evf[pwb].acadr,
		    (unsigned int)EOF_STA_MON, &dad_tmout))	return(-1);
	return(0);
}

int wai_stp_mon_flg(DAD_ID pwb)
{
	unsigned int	ptn;

	if(WAI_FLG( TIME_OUT|EVF_AND|EVF_RES, &ptn, dad_stpm_evf[pwb].acadr,
		    (unsigned int)EOF_STP_MON, &dad_tmout))	return(-1);
	return(0);
}

#ifdef	MAS_BIG_ENDIAN
static void chg_ul_bef_write(struct txt *text)		/* 整数型変数の上位と下位を入れ替える。(書き込み直前) */
{
	CHG_UL_SHORT(text->len);
	CHG_UL_SHORT(text->cmd);
	return;
}
static void chg_ul_aft_read1(struct dades10 *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_ULONG(text->ptn);
	CHG_UL_SHORT(text->stat);
	return;
}
static void chg_ul_aft_read2(struct dadsck *text)	/* 整数型変数の上位と下位を入れ替える。(読み出し直後) */
{
	CHG_UL_SHORT(text->ref1);
	CHG_UL_SHORT(text->ref0);
	return;
}
#endif	/* MAS_BIG_ENDIAN */
