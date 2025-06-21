/*----------------------------------------------------------------------*
 * File name	: mrt003.c	/ monitor text scan			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mrt003.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mrt003.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mrt003.c 1.2 1970/01/01
 *Log: mrt003.c
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
#include	<custom\custom.h>		/*	"		*/
//#include	<rzcmd\rzr009.h>		/* Show rzcmd		*/
#include	<drive\pdadcdrv.h>		/* Show drive		*/
#include	<drive\dadcsim.h>		/*	"		*/
#include	"adc100.h"			/* Show apl		*/
#include	"admtxt.h"			/* current		*/
#include	"pwbcfg.h"			/*	"		*/

static const enum mmsg	errt0[] = {
		ILLEGAL_PRM4,		/* ﾊﾟﾗﾒｰﾀ4の内容が不正		*/
		ILLEGAL_PRM5,		/* ﾊﾟﾗﾒｰﾀ5の内容が不正		*/
		ILLEGAL_PRM6,		/* ﾊﾟﾗﾒｰﾀ6の内容が不正		*/
		ILLEGAL_PRM7		/* ﾊﾟﾗﾒｰﾀ7の内容が不正		*/
};

static const enum mmsg	errt1[] = {
		ILLEGAL_NO_PRM4,	/* ﾊﾟﾗﾒｰﾀ4の数が合わない	*/
		ILLEGAL_NO_PRM5,	/* ﾊﾟﾗﾒｰﾀ5の数が合わない	*/
		ILLEGAL_NO_PRM6,	/* ﾊﾟﾗﾒｰﾀ6の数が合わない	*/
		ILLEGAL_NO_PRM7		/* ﾊﾟﾗﾒｰﾀ7の数が合わない	*/
};

/*----------------------------------------------------------------------*/
/*	mon_dad_cboot							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	ﾁｪﾝｼﾞ･ﾊﾟﾗﾒｰﾀ･ﾌﾞｰﾄ処理を行う。					*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	argc		: no. of argumunt				*/
/*	*argv[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

union usc {
	unsigned short	us;
	unsigned char	uc[2];
};

int
mon_dad_cboot(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	char		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	struct dadpk1	boot_pk;		/* arg parameter packet	*/
	struct dadrp0	cbrpk;			/* return param. packet	*/
	unsigned short	zsa[3];			/* 零点許容値		*/
	struct dadfip	fis[DAD_FIL_MAX];	/* ﾌｨﾙﾀ係数ﾊﾟﾗﾒｰﾀ･ﾊﾟｹｯﾄ	*/
	union usc	k;
	long		m;
	int		len, i, n, j;
	char		*cp, *pp, *p;

	if(argc != 9) {
		mon_text_snd( ILLEGAL_NO_PRM, (void **)argl);	return(-1);
	}
	if((len = strlen(argv[2])) != 1) {
		mon_text_snd( ILLEGAL_NO_PRM1, (void **)argl);	return(-1);
	}
	if((len = strlen(argv[3])) != 1) {
		mon_text_snd( ILLEGAL_NO_PRM2, (void **)argl);	return(-1);
	}
/*	if((len = strlen(argv[4])) != 34) {
		mon_text_snd( ILLEGAL_NO_PRM3, argl);		return(-1);
	}
*/
	memset((void *)&boot_pk, 0, sizeof(boot_pk));
	if(*argv[2] >= DAD_WMAX +'0') {
		mon_text_snd( ILLEGAL_PRM1, (void **)argl);	return(-1);
	}
	boot_pk.cell_t = (short)(*argv[2] & 0x0f);

	if(*argv[3] >= (DAD_CMAX >>8) +'0') {
		mon_text_snd( ILLEGAL_PRM2, (void **)argl);	return(-1);
	}
	boot_pk.cell_t |= ((*argv[3] & 0x0f) << 8);

/*------------------------------------------------------*/
/*	零点許容値のﾊﾟﾗﾒｰﾀ設定				*/
/*							*/
/*------------------------------------------------------*/

	n = 0;
	pp = cp = argv[4];
	while((cp = strchr( pp, PUNCTUATOR)) != NULL) {
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) != 4)  goto err_retp3n;
		for(i = sizeof(union usc)-1, p =pp; i >= 0 ; i--, p += 2) {
			if((j = htoc(p)) > 0xff)  goto err_retp3;
			k.uc[i] = (unsigned char)j;
		}
		zsa[n++] = k.us;
		pp = cp;
		if(n >= sizeof(zsa)/sizeof(short))  goto err_retp3;
	}
	if(n != (sizeof(zsa)/sizeof(short))-1) {
err_retp3:	mon_text_snd( ILLEGAL_PRM3, (void **)argl);	return(-1);
	}
	if((len = strlen(pp)) != 4) {
err_retp3n:	mon_text_snd( ILLEGAL_NO_PRM3, (void **)argl);	return(-1);
	}
	for(i = sizeof(union usc)-1, p =pp; i >= 0 ; i--, p += 2) {
		if((j = htoc(p)) > 0xff)  goto err_retp3;
		k.uc[i] = (unsigned char)j;
	}
	zsa[n] = k.us;
	boot_pk.z_targ = zsa[0];
	boot_pk.z_prm = (short *)&zsa[1];

/*------------------------------------------------------*/
/*	ﾌｨﾙﾀ係数のﾊﾟﾗﾒｰﾀ設定				*/
/*							*/
/*------------------------------------------------------*/

	for(n =0 ; n < DAD_FIL_MAX ; n++) {
		pp =cp =argv[n+5];
		boot_pk.fipp[n] = &fis[n];
		if((cp = strchr( pp, PUNCTUATOR)) == NULL)  goto err_ret0;
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) > 4)  goto err_ret1;
		if((j = atoi(pp)) <= 0)  goto err_ret0;
		fis[n].thin = (unsigned short)j;
		pp = cp;

		if((cp = strchr( pp, PUNCTUATOR)) == NULL)  goto err_ret0;
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) > 2)  goto err_ret1;
		if((j = atoi(pp)) < 0 || (j > 4))  goto err_ret0;
		fis[n].dig = (unsigned short)j;
		pp = cp;
		
		if((cp = strchr( pp, PUNCTUATOR)) == NULL)  goto err_ret0;
		*(cp++) = (char)'\0';
		if((len = strlen(pp)) > 4)  goto err_ret1;
		if((j = atoi(pp)) <= 0)  goto err_ret0;
		fis[n].tap = (unsigned short)j;
		pp = cp;
		
		if((cp = strchr( pp, '\0')) == NULL)  goto err_ret0;
		cp++;
		if((len = strlen(pp)) > 2)  goto err_ret1;
		if((j = atoi(pp)) <= 0)  goto err_ret0;
		fis[n].code = (unsigned short)j;
		pp = cp;
/*		
		fis[n].coef = &fic[n][0];
		i = 0;
		while((cp = strchr( pp, PUNCTUATOR)) != NULL) {
			*(cp++) = '\0';
			if((len = strlen(pp)) > 5)  goto err_ret1;
			if((m = atol(pp)) > USHRT_MAX)  goto err_ret0;
			fic[n][i++] = m;
			if(i >= DAD_TAP_MAX)  goto err_ret0;
			pp = cp;
		}
*/
		if((len = strlen(pp)) > 5) {
err_ret1:		mon_text_snd( errt1[n], (void **)argl);	return(-1);
		}
		if((m = atol(pp)) > USHRT_MAX) {
err_ret0:		mon_text_snd( errt0[n], (void **)argl);	return(-1);
		}
/*
		fic[n][i++] = m;
		fis[n].tap = i;
*/
	}
	if(dad_cboot1( pwbid, &cbrpk, &boot_pk) == -1) {
		mon_text_snd( CBOOT_FAIL, (void **)argl);	return(-1);
	}
	adcpub.f3rd_tmc = cbrpk.f3tmc;
	adcpub.f4th_tmc = cbrpk.f4tmc;
	adcpub.f5th_tmc = cbrpk.f5tmc;
//	rzr009s.stable_time = (unsigned short)(adcpub.lose_time+adcpub.f3rd_tmc);
	argl[0] = (void *)&adcpub.f3rd_tmc;
	argl[1] = (void *)&adcpub.f4th_tmc;
	argl[2] = (void *)&adcpub.f5th_tmc;
	return(mon_text_snd( CBOOT_SUCES, (void **)argl));
}
