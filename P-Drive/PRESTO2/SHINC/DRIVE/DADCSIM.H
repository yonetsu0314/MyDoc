/*----------------------------------------------------------------------*
 * File name	: dadcsim.h	/ dadc simulate driver			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/dadcsim.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dadcsim.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DADCSIM_H)
#define	DADCSIM_H

#define	DADC_SIM	(0x1 << 1)		/* bit 1 used		*/
#ifdef	_SIM
#if	(_SIM & DADC_SIM)

#define	dad_sta_sampl(dad_id, dad_ind, dad_ary_ptn, xcnt)\
	printf("\ndad_sta_sampl(id=%1x, ind=%1x, ptn=%4lx, cnt=%2x)",\
		dad_id, dad_ind, dad_ary_ptn, xcnt)

#define	dad_stp_sampl(dad_id)\
	printf("\ndad_stp_sampl(id=%1x)",dad_id)

#define	dad_boot0(dad_id, dad_pk0)\
	printf("\ndad_boot0(id=%1x, &pkt=%4x)",dad_id, dad_pk0)

#define	dad_boot1(dad_id, dad_rp0, dad_pk1)\
	printf("\ndad_boot1(id=%1x, &ret= %4x, &pkt=%4x)",\
		dad_id, dad_rp0, dad_pk1)

#undef	dad_read_sampl_stat
#define	dad_read_sampl_stat(dad_id)\
	(printf("\ndad_sampl_status(id=%1x, DAD_RD)",\
		dad_id), DAD_MON_HALT)

#undef	dad_write_sampl_stat
#define	dad_write_sampl_stat(dad_id, sts)\
	(printf("\ndad_sampl_status(id=%1x, DAD_WR, stat=%1x)",\
		dad_id, sts))\

#define	is_dadck_compl(dad_id)\
	printf("\nis_dadck_compl(id=%1x)",dad_id)

#define	dad_sys_init(dad_id)\
	printf("\ndad_sys_init(id=%1x)",dad_id)

#define	dad_reset(dad_id)\
	printf("\ndad_reset(id=%1x)",dad_id)

#define	dad_mzero(dad_id, sbp, pbp, pad, pda)\
	printf("\ndad_mzero(id=%1x, rqp=%4x, &exp=%4x, &ad=%4x, &da=%4x)",\
		dad_id, sbp, pbp, pad, pda)

#define	dad_memrd(dad_id, store, admem, len)\
	printf("\ndad_memrd(id=%1x, &sto=%4x, &adm=%4x, byte=%4x)",\
		dad_id, store, admem, len)

#define	dad_memwr(dad_id, admem, load, len)\
	printf("\ndad_memwr(id=%1x, &adm=%4x, lod=%4x, byte=%4x)",\
		dad_id, admem, load, len)

#define	dad_error_ack(dad_id, command, ecode)\
	printf("\ndad_error_ack(id=%1x, cmd=%4x, code=%4x)",\
		dad_id, command, ecode)

#define	dad_block_read(dad_id, kcode, storep)\
	printf("\ndad_block_read(id=%1x, code=%4x, &store=%4x)",\
		dad_id, kcode, storep)

#define	dad_ptn_write(dad_id, kcode, dist, xptn, loadp)\
	printf("\ndad_ptn_write(id=%1x, code=%4x, dist=%4x, ptn=%8lx, &load=%4x)",\
		dad_id, kcode, dist, xptn, loadp)
#endif
#endif

#endif	/* DADCSIM_H */

