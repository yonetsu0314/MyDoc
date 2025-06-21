/*----------------------------------------------------------------------*
 * File name	: e2srdi.c	/ E2SROM DI control							*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/biosif/e2srom/RCS/e2srdi.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: e2srdi.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2005/01/12 ベクター番号をPIOからE2SROMに変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * シリアルＲＯＭＩ／Ｆ
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.							*/
/*		959-1 SHIMOMAGARI RITTO-CITY									*/
/*		SHIGA JAPAN														*/
/*		(077) 553-4141													*/
/*----------------------------------------------------------------------*/
/* 説明                                                  				*/
/*	E2SROMのＤＩをＰＩＯを介して制御する。								*/
/*----------------------------------------------------------------------*/
/* 引き数																*/
/*	int	: ﾃﾞﾊﾞｲｽ･ﾅﾝﾊﾞｰ													*/
/*	int	: ﾋﾞｯﾄ位置														*/
/*	int	: ｾｯﾄ:1 ﾘｾｯﾄ:0													*/
/*																		*/
/* 戻り値																*/
/*	int	: 0																*/
/*----------------------------------------------------------------------*/

#include	<biosif\iosvc.h>		/* Show biosif						*/
#include	<umachine.h>			/* C:\usr\local\SH\5_1_0\INCLUDE	*/

int
_eesrom_handle_di(
	int	devno,					/* device number	*/
	int	bit_position,			/* ﾋﾞｯﾄ位置			*/
	int	on_off					/* set:1,reset:0	*/
	)
{
	return ((int)trapa_svc(VECTOR_E2SROM, 0x1, devno, bit_position, on_off));
}
