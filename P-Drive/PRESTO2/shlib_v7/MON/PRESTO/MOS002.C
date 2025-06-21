/*----------------------------------------------------------------------*
 * File name	: mos002.c	/ monitor text set			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/mon/presto/RCS/mos002.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mos002.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto/shlib_v7/mon/src/rcs/mos002.c 1.1 1970/01/01
 *Log: mos002.c
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#include	<stdio.h>			/* ANSI std		*/
#include	<stddef.h>			/*	"		*/
#include	<string.h>			/*	"		*/
#include	"cmfhdl.h"			/* Show APL		*/
#include	"admtxt.h"			/* current		*/
#include	"ciosim.h"			/*	"		*/

static const char	*mon_msgstr[] = {
	"ADC reset ",			/* software reset		*/
	"ADC sampling start ",		/* start sampling		*/
	"ADC sampling stop ",		/* stop sampling		*/
	"ADC initial boot ",		/* intial parameter boot	*/
	"ADC change boot ",		/* change parameter boot	*/
	"ADC zero tracking ",		/* zero tracking		*/
	"ADC read ",			/* memory read			*/
	"ADC write ",			/* memory write			*/
	"DSP initial boot ",		/* DSP intial parameter boot	*/
	"DSP change boot ",		/* DSP change parameter boot	*/
	"DSP zero tracking ",		/* DSP zero tracking		*/
	"DSP span adjustment ",		/* DSP span adjustment		*/
	"DSP afv start ",		/* DSP afv start		*/
	"DSP afv stop ",		/* DSP afv stop			*/
	"DSP read ",			/* DSP memory read		*/
	"DSP write ",			/* DSP memory write		*/
	"CHange ADC board ",		/* ADC board change		*/
	"arcnet SHOW MAPping ",		/* ARCNET show mapping		*/
	"weight read ",			/* weight read			*/
};

static const char	*mon_errstr[] = {
	"communication ",		/* ｺﾐｭﾆｹｰｼｮﾝ･ｴﾗｰ		*/
	"illegal text format ",		/* ﾃｷｽﾄ･ｺﾏﾝﾄﾞのﾌｫｰﾏｯﾄが不正	*/
	"undefine command ",		/* 未定義のｺﾏﾝﾄﾞ		*/
	"illegal no of token ",		/* ﾄｰｸﾝの数が合わない		*/
	"illegal parameter1 ",		/* ﾊﾟﾗﾒｰﾀ1の内容が不正		*/
	"illegal parameter2 ",		/* ﾊﾟﾗﾒｰﾀ2の内容が不正		*/
	"illegal parameter3 ",		/* ﾊﾟﾗﾒｰﾀ3の内容が不正		*/
	"illegal parameter4 ",		/* ﾊﾟﾗﾒｰﾀ4の内容が不正		*/
	"illegal parameter5 ",		/* ﾊﾟﾗﾒｰﾀ5の内容が不正		*/
	"illegal parameter6 ",		/* ﾊﾟﾗﾒｰﾀ6の内容が不正		*/
	"illegal parameter7 ",		/* ﾊﾟﾗﾒｰﾀ7の内容が不正		*/
	"illegal length parameter1 ",	/* ﾊﾟﾗﾒｰﾀ1の数が合わない	*/
	"illegal length parameter2 ",	/* ﾊﾟﾗﾒｰﾀ2の数が合わない	*/
	"illegal length parameter3 ",	/* ﾊﾟﾗﾒｰﾀ3の数が合わない	*/
	"illegal length parameter4 ",	/* ﾊﾟﾗﾒｰﾀ4の数が合わない	*/
	"illegal length parameter5 ",	/* ﾊﾟﾗﾒｰﾀ5の数が合わない	*/
	"illegal length parameter6 ",	/* ﾊﾟﾗﾒｰﾀ6の数が合わない	*/
	"illegal length parameter7 ",	/* ﾊﾟﾗﾒｰﾀ7の数が合わない	*/
	"illegal ASII HEX code ",	/* ASCII HEXでないｺｰﾄﾞ		*/
	"error buffer read ",		/* ｻﾝﾌﾟﾘﾝｸﾞ･ﾊﾞｯﾌｧのﾘｰﾄﾞｴﾗｰ	*/
	"end data sampling. stat = ",	/* ｻﾝﾌﾟﾘﾝｸﾞの終了		*/
	"error ADC fuction ",		/* ADCの関数ｴﾗｰ			*/
	"error DSP function ",		/* DSPの関数ｴﾗｰ			*/
	"invalid 3RD filter ",		/* ADCの3段目ﾃﾞｰﾀが無効		*/
	"terminate AFV adjustment ",	/* AFV調整の終了		*/
	"terminate stable check ",	/* 安定ﾁｪｯｸの終了		*/
};

/*----------------------------------------------------------------------*/
/*	mon_text_snd							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	引き数のコマンドを解析し、テキストを作成し送信する。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	command		: ｺﾏﾝﾄﾞ						*/
/*	*arg_lst[]	: 引き数ﾊﾟﾗﾒｰﾀ･ﾎﾟｲﾝﾀの配列へのﾎﾟｲﾝﾀ		*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ complete				*/
/*			  -1	･･･ error code				*/
/*----------------------------------------------------------------------*/

int
mon_text_snd(
	enum mmsg	msg,			/* ﾒｯｾｰｼﾞ/ｺﾏﾝﾄﾞ		*/
	void		*argl[]			/* ﾊﾟﾗﾒｰﾀﾎﾟｲﾝﾀの[]への&	*/
	)
{
	int		len;			/* ﾃｷｽﾄ長		*/
	char		*p;			/* ﾃｷｽﾄの最後のｱﾄﾞﾚｽ	*/
	char		buf[TXBUF_MAX];		/* ﾒｯｾｰｼﾞ･ﾊﾞｯﾌｧ		*/

	switch(msg) {
	case DATA_SAMPL:
		strcpy( buf, "\n#");
		break;

	case DATA_AFV:
		strcpy( buf, "\n$");
		break;

	case RESET_SUCES:
	case START_SUCES:
	case STOP_SUCES:
	case IBOOT_SUCES:
	case CBOOT_SUCES:
	case ZERO_SUCES:
	case READ_SUCES:
	case WRITE_SUCES:
	case DSP_IBOOT_SUCES:
	case DSP_CBOOT_SUCES:
	case DSP_ZERO_SUCES:
	case SPAN_SUCES:
	case AFV_STA_SUCES:
	case AFV_STP_SUCES:
	case DSP_READ_SUCES:
	case DSP_WRITE_SUCES:
	case CHG_ADC_SUCES:
	case ARC_SHOWMAP_SUCES:
		strcpy( buf, mon_msgstr[msg- IDX_SUCES] );
		strcat( buf, "!successful complete" );
		break;
	case WEI_READ_SUCES:
		break;

	case RESET_FAIL:
	case START_FAIL:
	case STOP_FAIL:
	case IBOOT_FAIL:
	case CBOOT_FAIL:
	case ZERO_FAIL:
	case READ_FAIL:
	case WRITE_FAIL:
	case DSP_IBOOT_FAIL:
	case DSP_CBOOT_FAIL:
	case DSP_ZERO_FAIL:
	case SPAN_FAIL:
	case AFV_STA_FAIL:
	case AFV_STP_FAIL:
	case DSP_READ_FAIL:
	case DSP_WRITE_FAIL:
	case CHG_ADC_FAIL:
	case ARC_SHOWMAP_FAIL:
	case WEI_READ_FAIL:
		strcpy( buf, mon_msgstr[msg- IDX_FAIL] );
		strcat( buf, "!fail" );
		break;

	case RESET_UNEXE:
	case START_UNEXE:
	case STOP_UNEXE:
	case IBOOT_UNEXE:
	case CBOOT_UNEXE:
	case ZERO_UNEXE:
	case READ_UNEXE:
	case WRITE_UNEXE:
	case DSP_IBOOT_UNEXE:
	case DSP_CBOOT_UNEXE:
	case DSP_ZERO_UNEXE:
	case SPAN_UNEXE:
	case AFV_STA_UNEXE:
	case AFV_STP_UNEXE:
	case DSP_READ_UNEXE:
	case DSP_WRITE_UNEXE:
	case CHG_ADC_UNEXE:
		strcpy( buf, mon_msgstr[msg- IDX_UNEXE] );
		strcat( buf, "!unexecute" );
		break;

	case MON_COM_ERROR:
	case ILLEGAL_TXT_FMT:
	case MON_UNDEF_CMD:
	case ILLEGAL_NO_PRM:
	case ILLEGAL_PRM1:
	case ILLEGAL_PRM2:
	case ILLEGAL_PRM3:
	case ILLEGAL_PRM4:
	case ILLEGAL_PRM5:
	case ILLEGAL_PRM6:
	case ILLEGAL_PRM7:
	case ILLEGAL_NO_PRM1:
	case ILLEGAL_NO_PRM2:
	case ILLEGAL_NO_PRM3:
	case ILLEGAL_NO_PRM4:
	case ILLEGAL_NO_PRM5:
	case ILLEGAL_NO_PRM6:
	case ILLEGAL_NO_PRM7:
	case NOT_ASII_HEX:
	case ERR_RD_SAMPL:
	case TERM_DATA_SAMPL:
		strcpy( buf, mon_errstr[msg- IDX_ERROR] );
		break;

	default:
		return(-1);
	}

	p = &buf[len = strlen(buf)];
	switch(msg) {
	case DATA_SAMPL:
		if((p = mon_sampl_text(&buf[2], argl))== NULL)	return(-1);
		break;

	case DATA_AFV:
	case AFV_STA_SUCES:
		if((p = mon_afv_text(&buf[2], argl))== NULL)	return(-1);
		break;

	case ZERO_SUCES:
		if((p = mon_zero_text(buf, argl))== NULL)	return(-1);
		break;

	case READ_SUCES:
	case DSP_READ_SUCES:
		if((p = mon_memrd_text(buf, argl))== NULL)	return(-1);
		break;

	case TERM_DATA_SAMPL:
		if((p = mon_ter_sampl_text(buf, argl))== NULL)	return(-1);
		break;

	case CBOOT_SUCES:
		if((p = mon_cboot_text(buf, argl))== NULL)	return(-1);
		break;

	case DSP_ZERO_SUCES:
		if((p = mon_dsp_zero_text(buf, argl))== NULL)	return(-1);
		break;

	case SPAN_SUCES:
		if((p = mon_span_text(buf, argl))== NULL)	return(-1);
		break;

	case ARC_SHOWMAP_SUCES:
		if((p = mon_showmap_text(buf, argl))== NULL)	return(-1);
		break;
	case WEI_READ_SUCES:
		if((p = mon_wei_text(buf, argl))== NULL)	return(-1);
		*(p++) = (char)'\n';
		*p = (char)'\0';
		len = (p - &buf[0])* sizeof(unsigned char);
		return( con_puts( &buf[0], len));
	}
	*(p++) = (char)'\n';
	*(p++) = (char)'\r';
	*p = (char)'\0';
	len = (p - &buf[0])* sizeof(unsigned char);
	return( con_puts( &buf[0], len));
}
