/*----------------------------------------------------------------------*
 * File name	: dpmdrv.h	/ dpmem driver prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/dpmdrv.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmdrv.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(DPMDRV_H)
#define	DPMDRV_H

void	dpm0_int_hdr(void);	/* DPM_ICU:DPM_WCU		*/
void	dpm1_int_hdr(void);	/* DPM_DSP:DPM_HIC		*/

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

#define	DPM_DIR		0x100		/* RD/WR textﾊﾞｯﾌｧの切り換え	*/

enum dpmid {
/*------  動作環境がWCUの場合  --------*/
	DPM_ICU,			/* WCU to ICU			*/

/*------  動作環境がICUの場合  --------*/
	DPM_WCU	= (0 +DPM_DIR),		/* ICU to WCU			*/
	DPM_HIC	= 1,			/*	" HIC board		*/

/*-------------------------------------*/
};
typedef enum dpmid	DPMID;

int		dpm_sys_init(DPMID, union aap *);

#endif	/* DPMDRV_H */

