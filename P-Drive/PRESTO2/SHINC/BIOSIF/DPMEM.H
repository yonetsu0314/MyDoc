/*----------------------------------------------------------------------*
 * File name	: dpmem.h	/ dpmem bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/dpmem.h 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmem.h $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/07/12 ADCボードの数を2->4に変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(DPMEM_H)
#define	DPMEM_H

/*	<<< argument 1 >>>						*/

enum dpmpid {
/*-----------	動作環境がWCUの場合	 -------------------------------*/
	WCU_ICU,			/* WCU to ICU			*/
	WCU_DSP,			/*	" DSP borad		*/
	WCU_DAD1,			/*	" DADC board 1		*/
	WCU_DAD2,			/*	"	     2		*/
	WCU_DAD3,			/*	"	     3		*/
	WCU_DAD4,			/*	"	     4		*/
/*-----------	動作環境がICUの場合	 -------------------------------*/
	ICU_WCU		= 0,		/* ICU to WCU			*/
	ICU_HIC,			/*	" HIC board		*/
	ICU_MLT,			/*	" MLT interlock		*/
};


/*	<<< return parameter >>>					*/

typedef struct dpmpinf {
	void		*iobase;	/* dpmem physical address	*/
	unsigned int	mem_size;	/* size of memory		*/
	int		mem_type;	/* access type			*/
	unsigned int	wrcmd;		/* command write register offset*/
	unsigned int	rdcmd;		/* command read register offset	*/
} DPMPINF;

const DPMPINF * 	_dpm_init(enum dpmpid);

#endif	/* DPMEM_H */

