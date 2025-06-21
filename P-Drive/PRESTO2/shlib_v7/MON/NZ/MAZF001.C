/*----------------------------------------------------------------------*
 * File name	: mazf001.c	/ monitor text interpret		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mazf001.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mazf001.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti0/rcs/mazf001.c 1.1 1970/01/01 00:00:00
 *Log: mazf001.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2001, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
#define		MAZF001_C
#include	<string.h>			/*	"		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

#define		F3RD	"3RD\0"

/*----------------------------------------------------------------------*/
/*	mon_dsp_az_f3rd							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	A/D値のみを操作する零点補正を行う。３段目フィルタでも操作可能。	*/
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
mon_dsp_az_f3rd(
	int		argc,			/* no. of argumunt	*/
	char		*argv[]			/* pointer to argument[]*/
	)
{
	void		*argl[3];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/

	if((argc != 2) && (argc != 3)) {
		mon_text_snd( ILLEGAL_NO_PRM, argl);		return(-1);
	}
	if(argc == 3) {
		if(strncmp(argv[3], F3RD, strlen(F3RD))) {
			mon_text_snd( ILLEGAL_NO_PRM2, argl);	return(-1);
		}
		argc--;
	}
	mon_dsp_zero( argc, argv);
}

/*----------------------------------------------------------------------*/
/*	mazf_dad_az							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	A/D値のみを操作する零点補正を行う。３段目フィルタでも操作可能。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	pwb		: ﾎﾞｰﾄﾞ･ﾅﾝﾊﾞｰ					*/
/*	ch_no		: ﾁｬﾈﾙ･ﾅﾝﾊﾞｰ					*/
/*	*zero_ad	: 零点ADﾃﾞｰﾀのｽﾄｱｰ･ﾎﾟｲﾝﾀｰ			*/
/*	*zero_da	: 零点DAﾃﾞｰﾀのｽﾄｱｰ･ﾎﾟｲﾝﾀｰ			*/
/*	*argv[]		: pointer to argument[]				*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/
int
mazf_dad_az(
	int		pwb,
	int		ch_no,
	short		*ad,
	short		*da,
	char		*argv[]			/* pointer to argument[]*/
	)
{
	if(strncmp( argv[3], F3RD, strlen(F3RD))) {
		return( dad_azero( pwb, ch_no, ad));
	} else {
		return( dad_az3rd( pwb, ch_no, ad));
	}
}
