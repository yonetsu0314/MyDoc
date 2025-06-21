/*----------------------------------------------------------------------*
 * File name	: mrt001.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mrt001.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt001.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mrt001.c 1.1 1996/07/22 19:24:29
 *Log: mrt001.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<limits.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	<stdlib.h>			/*	"		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"montsk.h"			/* Show apl		*/
#include	"cmfhdl.h"			/*	"		*/
#include	"admtxt.h"			/* current		*/
#include	"ciosim.h"			/*	"		*/
#include	"pwbcfg.h"			/*	"		*/

#define	ARGC_MAX	5			/* argcの最大		*/
#define	IND_MAX		10			/* ｽﾃｰｼﾞの指定範囲の最大*/
#define ARY_MAX		(DAD_WEI_MAX +DAD_AFV_MAX +DAD_DF_MAX)

enum {
	NOF_OPT,				/* no of option		*/
	_D,					/* -d			*/
	_S,					/* -s			*/
	OPT_MAX,
};

static const char	*opt_tbl[] = {
		"-D","-S"
};

/*----------------------------------------------------------------------*/
/*	check_option							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ｵﾌﾟｼｮﾝ･ﾊﾟﾗﾒｰﾀが、定義されている内容か否かをﾁｪｯｸする。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*opt_prm	: pointer to option parameter strings		*/
/*	*option		: pointer to option code			*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ ｵﾌﾟｼｮﾝ無し				*/
/*			  1	･･･ ｵﾌﾟｼｮﾝ有り				*/
/*----------------------------------------------------------------------*/

static int
check_option(
	char		*opt_prm,
	int		option[]
	)
{
	int		i, len, ret =0;

	len = strlen(opt_prm);
	for(i = 0; i< sizeof(opt_tbl)/sizeof(opt_tbl[0]); i++) {
		if(strncmp(opt_prm, opt_tbl[i], (size_t)len) == 0) {
			option[NOF_OPT]++;
			option[i+1] = i+1;
			ret = 1;
		}
	}
	return(ret);
}

/*----------------------------------------------------------------------*/
/*	mon_dad_sta_sampl						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾃﾞｰﾀのｻﾝﾌﾟﾘﾝｸﾞを開始する。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc		: no. of argumunt				*/
/*	*argv[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
mon_dad_sta_sampl(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	char		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	int		option[OPT_MAX];	/* option number	*/
	int		i, tok1 =0;
	unsigned int	j;
	unsigned long	m, xcnt =0;
	char		*pp, *cp, opt_prm[sizeof(opt_tbl)];
	short		xs =(short)0;
	DAD_ARY_PTN	ptn =0;

	strncpy(opt_prm, argv[2], sizeof(opt_prm));
	memset((void *)&option[NOF_OPT], 0, sizeof(option));
	pp = cp = &opt_prm[0];
	for(i =0; i < OPT_MAX; i++) {
		if((cp = strchr(pp, PUNCTUATOR)) != NULL) {
			*(cp++) = (char)'\0';
			tok1 |= check_option(pp, &option[0]);
			pp = cp;
			continue;
		}
		tok1 |= check_option(pp, &option[0]);
		break;
	}

	if((argc < tok1+3)||(argc > tok1+4)) {
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}
	pp = cp = argv[tok1+2];
	for(i =0; i < IND_MAX; i++) {
		if((cp = strchr( pp, PUNCTUATOR)) != NULL) {
			*(cp++) = (char)'\0';
			if((j= atoi(pp)) >= IND_MAX) {
				i = IND_MAX+1;
				break;
			}
			xs |= (0x1 << j);
			pp = cp;
			continue;
		}
		if((j= atoi(pp)) < IND_MAX)  xs |= (0x1 << j);
		else			     i = IND_MAX+1;
		break;
	}
	if(i > IND_MAX) {
		mon_text_snd( ILLEGAL_PRM1, (void **)argl);	return(-1);
	}
	pp = cp = argv[tok1+3];
	for(i =0; i < ARY_MAX; i++) {
		if((cp = strchr( pp, PUNCTUATOR)) != NULL) {
			*(cp++) = (char)'\0';
			if((j= atoi(pp)-1) >= ARY_MAX)  goto err_retp2;
			ptn |= (0x1L << j);
			pp = cp;
			continue;
		}
		if((j= atoi(pp)-1) >= ARY_MAX)  goto err_retp2;
		ptn |= (0x1L << j);
		break;
	}
	if(i > ARY_MAX) {
err_retp2:	mon_text_snd( ILLEGAL_PRM2, (void **)argl);	return(-1);
	}
	if(argc == (tok1+4)) {
		if((m = atol(argv[tok1+4])) > LONG_MAX) {
err_retp3:		mon_text_snd( ILLEGAL_PRM3, (void **)argl); return(-1);
		}
		xcnt = m;
	}
/*----------------------------------------------*/
/*	-Dオプション(データ変換)		*/
/*----------------------------------------------*/
/*	if(option[_D]) {
		xs |= (DAD_3RD|DAD_4TH|CNV_3RD|CNV_4TH);
		ptn |= 0x000f0000L;
	}
	if(dad_read_sampl_stat(pwbid) != DAD_MON_HALT) {
		mon_text_snd( START_UNEXE, argl);		return(0);
	}
*/
/*----------------------------------------------*/
/*	-Sオプション(同期スタート)		*/
/*----------------------------------------------*/
	if(option[_S]) {
		con_puts("Waiting for dump initiate",
			sizeof("Waiting for dump initiate"));
		if(init_sampl_sta_evf())  goto ret_fail;
	}

	if(xs &(DAD_ORG|DAD_1ST|DAD_2ND|DAD_3RD|DAD_4TH)) {
		if(dad_sta_mon( pwbid, (short)(xs &(DAD_ORG|DAD_1ST|DAD_2ND|DAD_3RD|
				DAD_4TH)), ptn, xcnt) == -1)  goto ret_fail;
	}
	if(xs >> 5) {
		if(dad_sta_dsp_mon( pwbid, (short)(xs>>5), ptn, xcnt) == -1) {
ret_fail:		mon_text_snd( START_FAIL, (void **)argl); return(-1);
		}
	}
	mon_save_sampl_stat((unsigned short)xs);
	del_sampl_sta_evf();
	return(mon_text_snd( START_SUCES, (void **)argl));
}
