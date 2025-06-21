/*----------------------------------------------------------------------*
 * File name	: dspsim.h	/ dsp simulate driver			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/dspsim.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: dspsim.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(DSPSIM_H)
#define	DSPSIM_H

#define	DSP_SIM	(0x1 << 2)			/* bit 2 used		*/
#ifdef	_SIM
#if	(_SIM & DSP_SIM)

#define	dsp_sys_init(fun)\
	printf("\ndsp_sys_init(func=%p)",fun)

#define	dsp_iboot(head_no,dsp_pk0)\
	printf("\ndsp_iboot(&pkt=%p)", dsp_pk0);			\
	printf("\npkt->wei_max=%2d", *dsp_pk0.wei_max);			\
	printf("\tpkt->afv_max=%2d", *dsp_pk0.afv_max);			\
	printf("\tpkt->df_max=%2d\n", *dsp_pk0.df_max);			\
{									\
	int	i;							\
	for(i =0; i<16 ; i++)						\
		printf("pkt->spc_wei[%d]=%4x    ",			\
		i, *((*dsp_pk0.wei_spc)+i));				\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("pkt->spc_df[%d]=%4x     ",			\
		i, *((*dsp_pk0.df_spc)+i));				\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("pkt->afv_prm[%d]=%6ld   ",			\
		i, *((*dsp_pk0.afv_fprm)+i));				\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("pkt->x_wei[%d]=%4d      ",			\
		i, *((*dsp_pk0.x0_ary)+i));				\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("pkt->x_afv[%d]=%4d      ",			\
		i, *((*dsp_pk0.x1_ary)+i));				\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("pkt->y_wei[%d]=%4d      ",			\
		i, *((*dsp_pk0.y0_ary)+i));				\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("pkt->y_afv[%d]=%4d      ",			\
		i, *((*dsp_pk0.y1_ary)+i));				\
}

#define	dsp_cboot(dsp_pk1)\
	printf("\ndsp_cboot(&pkt=%p)", dsp_pk1);			\
	printf("\npkt->wei&cell=%2d", *dsp_pk1.code);			\
	printf("\tpkt->apn_mass=%2d\n", *dsp_pk1.spn_mas);		\
{									\
	int	i;							\
	for(i =0; i<9 ; i++)						\
		printf("pkt->zero_lmt[%d]=%4x    ",			\
		i, *((*dsp_pk1.zr_lmt)+i));				\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("pkt->zad_wei[%d]=%4x    ",			\
		i, *((*dsp_pk1.wei_zad)+i));				\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("pkt->zad_afv[%d]=%4x    ",			\
		i, *((*dsp_pk1.afv_zad)+i));				\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("pkt->zad_df[%d]=%4x     ",			\
		i, *((*dsp_pk1.df_zad)+i));				\
}

#define	dsp_zero(xptn, d4th, s4th, ret)\
	printf("\ndsp_zero(ptn=%p, &dat4th=%p, &sts4th=%p, &retp=%p)",	\
		xptn, d4th, s4th ,ret);					\
	printf("\nexe_ptn=%8lx\n", *xptn);				\
{									\
	int	i, j;							\
	for(i =0; i<24 ; i++)						\
		printf("dat_4th[%d]=%4x    ",				\
		i, *((*d4th)+i));					\
	printf("\n");							\
	for(i =0; i<24 ; i++)						\
		printf("sts_4th[%d]=%4x    ",				\
		i, *((*s4th)+i));					\
	printf("\n  -------<<< return parameter >>>----------  ");	\
	*ret.err_ptn = *xptn;						\
	printf("\nret->err_ptn=%8lx\n", *ret.err_ptn);			\
	j = 0xad00;					/* AD data    */\
	for(i =0; i<24 ; i++)						\
		*((*ret.zero_ad)+i) = j+i,		/* ad00-15    */\
		printf("ret->zero_ad[%d]=%4x    ",			\
		i, *((*ret.zero_ad)+i));				\
}

#define	dsp_span(xptn, d4th, s4th, ret)\
	printf("\ndsp_span(ptn=%p, &dat4th=%p, &sts4th=%p, &retp=%p)",	\
		xptn, d4th, s4th, ret);					\
	printf("\nexe_ptn=%8lx\n", *xptn);				\
{									\
	int	i, j;							\
	for(i =0; i<16 ; i++)						\
		printf("dat_4th[%d]=%4x    ",				\
		i, *((*d4th)+i));					\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("sts_4th[%d]=%4x    ",				\
		i, *((*s4th)+i));					\
	printf("\n  -------<<< return parameter >>>----------  ");	\
	*ret.err_ptn = *xptn;						\
	printf("\nret->err_ptn=%8lx\n", *ret.err_ptn);			\
	j = 0xcf00;					/* CoeFficent */\
	for(i =0; i<16 ; i++)						\
		*((*ret.sp_coef)+i) = j+i,		/* cf00-15    */\
		printf("ret->spn_coef[%d]=%4x    ",			\
		i, *((*ret.sp_coef)+i));				\
}

#define	dsp_wei_cnvrt(xptn, d3rd, s3rd, ac, safv, d4th, s4th, ret)	\
	printf("\ndsp_wei_cnvrt(ptn=%p, &dat3rd=%p, &sts3rd=%p,"	\
		" &ac_afv=%p, &sts_afv=%p, &dat4th=%p, &sts4th=%p,"	\
		" &retp=%p)",						\
		xptn, d3rd, s3rd, ac, safv, d4th, s4th, ret);		\
	printf("\nexe_ptn=%8lx\n", *xptn);				\
{									\
	int	i, j;							\
	for(i =0; i<16 ; i++)						\
		printf("dat_3rd[%d]=%4x    ",				\
		i, *((*d4th)+i));					\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("sts_3rd[%d]=%4x    ",				\
		i, *((*s4th)+i));					\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("dat_afv[%d]=%4x    ",				\
		i, *((*ac)+i));						\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("sts_afv[%d]=%4x    ",				\
		i, *((*safv)+i));					\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("dat_4th[%d]=%4x    ",				\
		i, *((*d4th)+i));					\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("sts_4th[%d]=%4x    ",				\
		i, *((*s4th)+i));					\
	printf("\n  -------<<< return parameter >>>----------\n");	\
	j = 0xfd00;					/* Flag Data  */\
	for(i =0; i<16 ; i++)						\
		*(short *)((*ret.stat)+i) = j+i,	/* cf00-15    */\
		printf("ret->stat34[%d]=%4x    ",			\
		i, *((*ret.stat)+i));					\
	printf("\n");							\
	j = 0x3d00;					/* 3rd Data   */\
	for(i =0; i<16 ; i++)						\
		*((*ret.wd3rd)+i) = j+i,		/* 3d00-15    */\
		printf("ret->wei_3rd[%d]=%4x    ",			\
		i, *((*ret.wd3rd)+i));					\
	printf("\n");							\
	j = 0x4d00;					/* 4th Data   */\
	for(i =0; i<16 ; i++)						\
		*((*ret.wd4th)+i) = j+i,		/* 4d00-15    */\
		printf("ret->wei_4th[%d]=%4x    ",			\
		i, *((*ret.wd4th)+i));					\
}

#define	dsp_df_cnvrt(d4th, s4th, ret)\
	printf("\ndsp_df_cnvrt(&dat4th=%p, &sts4th=%p, &retp=%p)",	\
		d4th, s4th, ret);					\
{									\
	unsigned int	i, j;						\
	for(i =0; i<4 ; i++)						\
		printf("dat_4th[%d]=%4x    ",				\
		i, *((*d4th)+i));					\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("sts_4th[%d]=%4x    ",				\
		i, *((*s4th)+i));					\
	printf("\n  -------<<< return parameter >>>----------\n");	\
	j = 0xfd00;					/* Flag Data  */\
	for(i =0; i<4 ; i++)						\
		*((*ret.stat)+i) = j+i,			/* cf00-15    */\
		printf("ret->df_sts[%d]=%4x    ",			\
		i, *((*ret.stat)+i));					\
	printf("\n");							\
	j = 0x4d00;					/* 4th Data   */\
	for(i =0; i<4 ; i++)						\
		*((*ret.df4th)+i) = j+i,		/* 4d00-15    */\
		printf("ret->df_4th[%d]=%4x    ",			\
		i, *((*ret.df4th)+i));					\
}

#define	dsp_afv_adjust(xptn, d3rd, s3rd, ac, safv, ret)			\
	printf("\ndsp_afv_adjust(ptn=%p, &dat3rd=%p, &sts3rd=%p,"	\
		" &ac_afv=%p, &sts_afv=%p, &retp=%p)",			\
		xptn, d3rd, s3rd, ac, safv, ret);			\
	printf("\nexe_ptn=%8lx\n", *xptn);				\
{									\
	int	i, j;							\
	for(i =0; i<16 ; i++)						\
		printf("dat_3rd[%d]=%4x    ",				\
		i, *((*d3rd)+i));					\
	printf("\n");							\
	for(i =0; i<16 ; i++)						\
		printf("dts_3rd[%d]=%4x    ",				\
		i, *((*s3rd)+i));					\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("dat_afv[%d]=%4x    ",				\
		i, *((*ac)+i));						\
	printf("\n");							\
	for(i =0; i<4 ; i++)						\
		printf("sts_afv[%d]=%4x    ",				\
		i, *((*safv)+i));					\
	printf("\n  -------<<< return parameter >>>----------  ");	\
	*ret.err_ptn = *xptn;						\
	printf("\nret->err_ptn=%8lx\n", *ret.err_ptn);			\
	j = 0xcf00;					/* CoeFficent */\
	for(i =0; i<16 ; i++)						\
		*(long *)((*ret.afv_fprm)+i) = (long)(j+i),		\
		printf("ret->afv_fprm[%d]=%4x    ",			\
		i, *((*ret.afv_fprm)+i));				\
}

#define	dsp_memrd(store, admem, len)\
	len,printf("\ndsp_memrd(&sto=%p, &adm=%4x, byte=%4x)",\
		store, (short)admem, len)

#define	dsp_memwr(admem, load, len)\
	len,printf("\ndsp_memwr(&adm=%4x, lod=%p, byte=%4x)",\
		(short)admem, load, len)

#endif
#endif

#endif	/* DSPSIM_H */

