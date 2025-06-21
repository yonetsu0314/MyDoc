/*----------------------------------------------------------------------*
 * File name	: mor002.c	/ monitor text interpret		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/nz/RCS/mor002.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mor002.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/wcu/main/nz_wcu/uti/rcs/mor002.c 1.1 1996/07/22 19:24:36
 *Log: mor002.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<string.h>			/* ANSI std		*/
#include	<ctype.h>			/*	"		*/
#include	<drive\ndadcdrv.h>		/* Show drive		*/
#include	"admtxt.h"			/* current		*/

#define		ARG_MAX		16	/* argcの最大			*/

static const char	*mon_cmdstr[] = {
		"RESET",		/* software reset		*/
		"START",		/* start sampling		*/
		"\x18",			/* stop sampling (CTL-X)	*/
		"IBOOT",		/* intial parameter boot	*/
		"CBOOT",		/* change parameter boot	*/
		"ZERO",			/* zero tracking		*/
		"READ",			/* memory read			*/
		"WRITE",		/* memory write			*/
		"DSPIBOOT",		/* DSP intial parameter boot	*/
		"DSPCBOOT",		/* DSP change parameter boot	*/
		"DSPZERO",		/* DSP zero tracking		*/
		"SPAN",			/* span adjustment		*/
		"AFV",			/* AFV adjustment		*/
		"DSPREAD",		/* DSP memory read		*/
		"DSPWRITE",		/* DSP memory write		*/
		"CHADC",		/* ADC board change		*/
		"SHOWMAP",		/* ARCNET show mapping		*/
		"WEIREAD",		/* weight rad			*/
};

/*----------------------------------------------------------------------*/
/*	mon_text_interpret						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	バッファのコマンドを解析し、コマンドに対応した処理を行う。	*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	buf[]		: 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ			*/
/*	byte		: ﾃｷｽﾄ長					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/
int     setarg(char *, int , char **);

int
mon_text_interpret(
	unsigned char	buf[],			/* 受信ﾃｷｽﾄ･ﾊﾞｯﾌｧ	*/
	int		byte			/* ﾃｷｽﾄ長		*/
	)
{
	int		argc;			/* no. of argumunt	*/
	char		*argv[ARG_MAX];		/* pointer to argument[]*/
	void		*argl[1];		/* 送信ﾃｷｽﾄの引き数ﾘｽﾄ	*/
	unsigned char	*p;
	int		len, cmd, i, j;

	len = strlen((char *)(p = buf));
	for(i =0; i < len ; i++) {
		*p = (unsigned char)toupper(*p);
		++p;
	}
	if((argc = setarg( (char *)buf, ARG_MAX, argv)) == -1)  goto err_retf;
	len = strlen(argv[1]);
	for(cmd = RESET; cmd < MON_CMD_MAX ; ++cmd) {
		if(len > (int)strlen(mon_cmdstr[cmd]))  continue;
		if(strncmp(argv[1], mon_cmdstr[cmd], (size_t)len) == 0)  goto found;
	}
err_retf:
	mon_text_snd( ILLEGAL_TXT_FMT, argl );			return(-1);

/*------------------------------------------------------*/
/*	コマンドに対するアクション			*/
/*							*/
/*------------------------------------------------------*/
found:
	switch(cmd) {
	case RESET:
		mon_dad_reset( argc, argv );
		break;

	case START:
		mon_dad_sta_sampl( argc, argv );
		break;

	case STOP:
		stp_afv_tsk();
		break;

	case IBOOT:
		mon_dad_iboot( argc, argv );
		break;

	case CBOOT:
		mon_dad_cboot( argc, argv );
		break;

	case ZERO:
		mon_dad_mzero( argc, argv );
		break;

	case READ:
		mon_dad_memrd( argc, argv );
		break;

	case WRITE:
		mon_dad_memwr( argc, argv );
		break;

	case DSPIBOOT:
		mon_dsp_iboot( argc, argv );
		break;

	case DSPCBOOT:
		mon_dsp_cboot( argc, argv );
		break;

	case DSPZERO:
		mon_dsp_zero( argc, argv );
		break;

	case SPAN:
		mon_dsp_span( argc, argv );
		break;

	case AFV:
		mon_dsp_sta_afv( argc, argv );
		break;

	case DSPREAD:
		mon_dsp_memrd( argc, argv );
		break;

	case DSPWRITE:
		mon_dsp_memwr( argc, argv );
		break;

	case CHADC:
		mon_chg_adc_board( argc, argv );
		break;

	case SHOWMAP:
		mon_arc_showmap( argc, argv );
		break;

	case WEIREAD:
		mon_dad_weird( argc, argv );
		break;

	default:
		mon_text_snd( MON_UNDEF_CMD, argl );
	}
	return(0);
}
