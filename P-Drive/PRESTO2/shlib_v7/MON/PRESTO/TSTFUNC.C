/*----------------------------------------------------------------------*
 * File name	: tstfunc.c	/ test driver & prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/tstfunc.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: tstfunc.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/tstfunc.c 1.1 1970/01/01
 *Log: tstfunc.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stddef.h>			/* ANSI std		*/
#include	<stdio.h>			/*	"		*/
#include	<memory.h>			/* MS-C			*/

#include	<drive\serial.h"		/* Show DRIVE directory	*/
#include	<drive\scudrv.h"		/*	"		*/
#include	<drive\ndadcdrv.h"		/*	"		*/
#include	<drive\dadcsim.h"		/*	"		*/
#include	"usrkint.h"			/*	"		*/
#include	"admtxt.h"			/*	"		*/

#define	STORAGE
#include	"adc100.h"
#include	"adc101.h"
#include	"adc102.h"
#include	"adc103.h"
#undef	STORAGE

#include	"p:\lib\biosif\include\scua.h"	/* Show BIOS I/F dir.V53*/
#include	"p:\lib\drive\pdadc\dadctxt.h"	/*			*/
#include	"p:\lib\drive\pdadc\dadctl.h"	/*			*/
#include	"p:\lib\drive\exram\exbctl.h"	/*			*/

#define	hard_int(\
		HANDLER			/* 割り込みﾊﾝﾄﾞﾗｰへのﾎﾟｲﾝﾀ    */\
	)								\
_asm {									\
_asm	cli								\
_asm	pushf								\
_asm	call	far ptr HANDLER						\
_asm	sti								\
}

static int	tmg[] = {1, 0x1f};
static int	cmd[] = {READ_TMG};
static short	sd[] = { 
	0,	0,	0,	0,	0,	0,	0,	0,	/*8*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*16*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*24*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*32*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*40*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*48*/
	0,	0,	0,	0,	0,	0,	0,	0,	/*56*/
	0,	0,	0,	0,	0,	0,	0,	0	/*64*/
};


/*------------------------------------------------------*/
/* Function :	sampling start				*/
/*	key "0" function				*/
/*------------------------------------------------------*/

static void
__func_0(void)
{
	struct dadc_ctxt	*contxt;
	char far		*p;

	contxt = &dadc_ctxt_tbl[DADC1];
	p = contxt->iobase;
	_fmemcpy(p+ S_PTR(contxt->adtxt), tmg, sizeof(tmg));
	_fmemcpy(p+ S_PTR(contxt->adcmd), cmd, sizeof(cmd));
	verify_puti(CMD_ACK, p+ S_PTR(contxt->hcmd));
	dad_sta_sampl(DADC1, DAD_ORG +DAD_1ST +DAD_2ND +DAD_3RD +DAD_4TH,
			0x998001, 0);
	verify_puti(CMD_ACK, p+ S_PTR(contxt->hcmd));
/*	dos_vector_swap(dadc_int_hdr);					*/
	printf("\nexecute: __func_0");
}

/*------------------------------------------------------*/
/* Function :	execute DADC int handler		*/
/*	key "1" function				*/
/*------------------------------------------------------*/

static void
__func_1(void)
{
	struct dadc_ctxt	*contxt;
	char far		*p;

	contxt = &dadc_ctxt_tbl[DADC1];
	p = contxt->iobase;
	_fmemcpy(p+ S_PTR(contxt->adcmd), cmd, sizeof(cmd));
	hard_int(dadc_int_hdr);
	printf("\nexecute: __func_1");
}

/*------------------------------------------------------*/
/* Function :	read ex-mem & text send			*/
/*	key "2" function				*/
/*------------------------------------------------------*/

static void
__func_2(void)
{
	unsigned char	buf[512];
	void		*argl[2];
	int		len, i;

	if((len = exbuf_read(0, buf, sizeof(buf))) == -1)
		printf("\nexbuf_read(): error");
	else if(len) {
		argl[0] = &buf[0];
		argl[1] = &len;
		mon_text_snd( DATA_SAMPL, argl);
/*		printf("\n");						*/
/*		for(i = 0 ; i < len ; i++)  printf("[%2x] ", buf[i]);	*/
	}
	printf("\nexecute: __func_2");
}

/*------------------------------------------------------*/
/* Function :	test dsp_zero				*/
/*	key "3" function				*/
/*------------------------------------------------------*/

static void
__func_3(void)
{
	struct dadd1	s3rd;
	struct dadd0	s4th;
	int		i;

	dad_ptn_write(DADC1, DAD3RD, 24, 0xff89f6, sd);
	printf("\ndad_ptn_write(DADC1, DAD3RD, 24, 0xff89f6, sd)");
	dad_ptn_write(DADC1, DAD4TH, 24, 0xff89f6, sd);
	printf("\ndad_ptn_write(DADC1, DAD4TH, 24, 0xff89f6, sd)");
/*
	dad_block_read(DADC1, DAD4TH, &s4th);
	for(i =0; i < sizeof(s4th.dat)/sizeof(s4th.dat[0]) ; i++)
		printf("dat[%d]= %4x    ", i, s4th.dat[i]);
	for(i =0; i < sizeof(s4th.sts)/sizeof(s4th.sts[0]) ; i++)
		printf("sts[%d]= %4x    ", i, s4th.sts[i]);
*/
}

/*------------------------------------------------------*/
/* Function :	test AFV adj				*/
/*	key "4" function				*/
/*------------------------------------------------------*/

static void
__func_4(void)
{
	struct dadd1	s3rd;
	struct dsprp4	ret;
	void		*argl[3];
	unsigned long	ptn[2];
	int		i;

/*	if(dad_ptn_write(DADC1, DAD3RD, 24, 0xf089f6, sd)== -1) {	*/
	if(dad_ptn_write(DADC1, DAD3RD, 24, 0xff89f6, sd)== -1) {
		printf("\ndad_ptn_write(DADC1, DAD3RD, 24, 0xff89f6, sd)");
								return;
	}
	if(dad_block_read(DADC1, DAD3RD, &s3rd)== -1) {
		printf("\ndad_block_read(DADC1, DAD3RD, &s3rd)");
								return;
	}
	ptn[0] = ptn[1] = 0x3ff;
	i = dsp_afv_adjust(&ptn[1], &s3rd.dat, &s3rd.sts,
			&(s3rd.ac+DAD_WEI_MAX), &(s3rd.sts+DAD_WEI_MAX),
			&ret);
	if(i == -1) {
		printf("\n!!! error  dsp_afv_adjust()");	return;
	}
	argl[0] = &ptn[0];
	argl[1] = &s3rd;
	argl[2] = &ret;
	mon_text_snd(DATA_AFV, argl);
}

/*------------------------------------------------------*/
/* Function :	test weigh data convert			*/
/*	key "5" function				*/
/*------------------------------------------------------*/

static void
__func_5(void)
{
	struct dadd1	s3rd;
	struct dadd0	s4th;
	struct dsprp2	ret;
	void		*argl[3];
	unsigned long	ptn[2];
	int	i, j;

	memset(&ret, 0, sizeof(ret));
/*	dad_ptn_write(DADC1, DAD3RD, 24, 0xff89f6, sd);		*/
	dad_ptn_write(DADC1, DAD3RD, 24, 0xf089f6, sd);
/*	dad_ptn_write(DADC1, DAD4TH, 24, 0xff89f6, sd);		*/
/*	dad_ptn_write(DADC1, DAD4TH, 24, ~0xff89f6, sd);	*/
	dad_ptn_write(DADC1, DAD4TH, 24, ~0x841248, sd);

	dad_block_read(DADC1, DAD3RD, &s3rd);
	dad_block_read(DADC1, DAD4TH, &s4th);

	ptn[0] = ptn[1] = 0x3ff;
	i = dsp_wei_cnvrt(&ptn[1], &s3rd.dat, &s3rd.sts,
			&(s3rd.ac+DAD_WEI_MAX), &(s3rd.sts+DAD_WEI_MAX),
			&s4th.dat, &s4th.sts, &ret);
	if(i == -1) {
		printf("\n!!! error  dsp_wei_cnvrt()");		return;
	}

	printf("\nexe_ptn=%8lx ::%8lx  ::%8lx", ptn[0], ptn[1], ptn[0]^ptn[1]);
	printf("\n  -------<<< return parameter >>>----------\n");
	for(i =0; i<16 ; i++)
		printf("ret.stat34[%d]=%4x    ", i, ret.stat[i].us);
	printf("\n");
	for(i =0; i<16 ; i++)
		printf("ret.wei_3rd[%d]=%4x    ", i, ret.wd3rd[i]);
	printf("\n");
	for(i =0; i<16 ; i++)
		printf("ret.wei_4th[%d]=%4x    ", i, ret.wd4th[i]);
}

/*------------------------------------------------------*/
/* Function :	df data convert				*/
/*	key "6" function				*/
/*------------------------------------------------------*/

static void
__func_6(void)
{
	struct dadd0	s4th;
	struct dsprp3	ret;
	void		*argl[3];
	int	i, j;

	memset(&ret, 0, sizeof(ret));
	dad_ptn_write(DADC1, DAD4TH, 24, ~0x841248, sd);
	dad_block_read(DADC1, DAD4TH, &s4th);

	i = dsp_df_cnvrt( &(s4th.dat+20), &(s4th.sts+20), &ret);
	if(i == -1) {
		printf("\n!!! error  dsp_df_cnvrt()");		return;
	}

	printf("\n  -------<<< return parameter >>>----------\n");
	for(i =0; i<DSP_DF_MAX ; i++)
		printf("ret.stat[%d]=%4x    ", i, ret.stat[i]);
	printf("\n");
	for(i =0; i<DSP_DF_MAX ; i++)
		printf("ret.df_4th[%d]=%4x    ", i, ret.df4th[i]);
}

/*------------------------------------------------------*/
/* Function :						*/
/*	key "7" function				*/
/*------------------------------------------------------*/

#define	__func_7	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "8" function				*/
/*------------------------------------------------------*/

#define	__func_8	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "9" function				*/
/*------------------------------------------------------*/

static void
__func_9(void)
{
	printf("\nexecute: __func_9");
}

/*------------------------------------------------------*/
/* Function :						*/
/*	key "a" function				*/
/*------------------------------------------------------*/

static void
__func_a(void)
{
	printf("\nexecute: __func_a");
}
/*------------------------------------------------------*/
/* Function :						*/
/*	key "b" function				*/
/*------------------------------------------------------*/

#define	__func_b	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "c" function				*/
/*------------------------------------------------------*/

#define	__func_c	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "d" function				*/
/*------------------------------------------------------*/

#define	__func_d	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "e" function				*/
/*------------------------------------------------------*/

#define	__func_e	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "f" function				*/
/*------------------------------------------------------*/

#define	__func_f	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "g" function				*/
/*------------------------------------------------------*/

#define	__func_g	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "h" function				*/
/*------------------------------------------------------*/

#define	__func_h	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "i" function				*/
/*------------------------------------------------------*/

#define	__func_i	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "j" function				*/
/*------------------------------------------------------*/

#define	__func_j	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "k" function				*/
/*------------------------------------------------------*/

#define	__func_k	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "l" function				*/
/*------------------------------------------------------*/

#define	__func_l	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "m" function				*/
/*------------------------------------------------------*/

#define	__func_m	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "n" function				*/
/*------------------------------------------------------*/

#define	__func_n	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "o" function				*/
/*------------------------------------------------------*/

#define	__func_o	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "p" function				*/
/*------------------------------------------------------*/

#define	__func_p	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "q" function				*/
/*------------------------------------------------------*/

#define	__func_q	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "r" function				*/
/*------------------------------------------------------*/

#define	__func_r	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "s" function				*/
/*------------------------------------------------------*/

#define	__func_s	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "t" function				*/
/*------------------------------------------------------*/

#define	__func_t	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "u" function				*/
/*------------------------------------------------------*/

#define	__func_u	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "v" function				*/
/*------------------------------------------------------*/

#define	__func_v	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "w" function				*/
/*------------------------------------------------------*/

#define	__func_w	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "x" function				*/
/*------------------------------------------------------*/

#define	__func_x	NULL

/*------------------------------------------------------*/
/* Function :						*/
/*	key "y" function				*/
/*------------------------------------------------------*/

#define	__func_y	NULL

/*------------------------------------------------------*/
/* Function :	system init				*/
/*	key "z" function				*/
/*------------------------------------------------------*/

static void
__func_z(void)
{
	static int	con_fh;

	if(dad_sys_init(DADC1)) {
		printf("\nDADC System initialize fail");
		return;
	}
	if(exm_sys_init()) {
		printf("\nEXM System initialize fail");
		return;
	}
/*	if((con_fh = open("con", O_RDWR))== -1) {	*/
/*		printf("\nCON I/O initialize fail");	*/
/*		return;					*/
/*	}						*/
	printf("\nInitialze successful complete");
}

#define	STORAGE
#include	"functbl.h"
#undef	STORAGE
