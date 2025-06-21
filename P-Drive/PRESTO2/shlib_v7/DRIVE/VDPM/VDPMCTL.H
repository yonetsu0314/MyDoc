/*----------------------------------------------------------------------*
 * File name	: vdpmctl.h	/ virtual dpmem control data define	*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/drive/vdpm/RCS/vdpmctl.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: vdpmctl.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/lib/drive/vdpm/rcs/vdpmctl.h 1.1 1995/09/16 14:29:14 chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(VDPMCTL_H)
#define	VDPMCTL_H

int			init_vdpm_read(int devno);
int			init_vdpm_write(int devno);


#define	DPM_DEV_MAX	2		/* DP-SRAMの最大channel数	*/


#ifdef	STORAGE
int	vdpm_mode[] = {-1, -1};		/* DPRAMのｱｸｾｽ･ﾓｰﾄﾞ		*/
#else
extern int	vdpm_mode[];
#endif

/*------------------------------------------------------*/
/*	リアル･タイムＯＳ関連の定義			*/
/*							*/
/*------------------------------------------------------*/

#ifdef	STORAGE
unsigned int	vdpm_sem_id[DPM_DEV_MAX] = {
		VDPM0TX_SEM_ID, VDPM0RX_SEM_ID
};
union aap	vdpm_sem[DPM_DEV_MAX] = {
		0L, 0L			/* DPMTX/RXｾﾏﾌｫのｱｸｾｽ･ｱﾄﾞﾚｽ	*/
};
unsigned char	vdpm_nest[DPM_DEV_MAX] = {
		(unsigned char)0	/* 関数呼出のﾈｽﾃｨﾝｸﾞ回数	*/
};
union aap	*vdpm_rx_tsk[DPM_DEV_MAX] = {
		(void *)0		/* RXﾀｽｸAAの配列へのﾎﾟｲﾝﾀの配列	*/
};
long		vdpm_poll_time = 2L;	/* ﾎﾟｰﾘﾝｸﾞ指定時間(ms)		*/
long		vdpm_time_out = 1000L;	/* ﾀｲﾑ･ｱｳﾄ指定時間(ms)		*/
union aap	vdpm_ds = { 0L, 0L };	/* ﾄﾞﾗｲﾊﾞｰのsegment情報		*/
#else
extern unsigned int	vdpm_sem_id[];
extern union aap	vdpm_sem[];
extern unsigned char	vdpm_nest[];
extern union aap	*vdpm_rx_tsk[];
extern long		vdpm_poll_time;
extern long		vdpm_time_out;
extern union aap	vdpm_ds;
#endif

#endif	/* VDPMCTL_H */
