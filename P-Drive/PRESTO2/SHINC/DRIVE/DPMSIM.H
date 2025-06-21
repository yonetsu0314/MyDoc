/*----------------------------------------------------------------------*
 * File name	: dpmsim.h	/ dpmem simulate driver			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/dpmsim.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dpmsim.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DPMSIM_H)
#define	DPMSIM_H

#define	DPM_SIM	(0x1 << 3)			/* bit 3 used		*/
#ifdef	_SIM
#if	(_SIM & DPM_SIM)

#define	dpm_sys_init(devid, rxtsk)\
	printf("\ndpm_sys_init(dev_id=%d, &rx_tsk= %p)",\
	devid, rxtsk)

#define	dpm_open(ch)\
	printf("\ndpm_open(channel=%d)", ch)

#define	dpm_dsp_cmdrd(cmd)\
	printf("\ndpm_dsp_cmdrd(&command= %p)", cmd)

#define	dpm_dsp_read(buf, max, offset)\
	printf("\ndpm_dsp_read(&buf= %p, max=%4d D, offset=%4d D)",\
		buf, max, offset)

#define	dpm_dsp_write(ret, buf, len)\
	printf("\ndpm_dsp_write(&retp= %p, &buf= %p, len=%4d D)",\
		ret, buf, len)

#endif
#endif

#endif	/* DPMSIM_H */

