/*----------------------------------------------------------------------*
 * File name	: chrcmp.c	/ 					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/custom/src/RCS/chrcmp.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: chrcmp.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 2002, ISHIDA CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN		*/
/*		(077) 553-4141						*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*	buffで示されるバッファの指定バイト数すべてが、指定した		*/
/*	キャラクタ･データか否かをテストし、その結果を返す。		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	*buff		: ﾊﾞｯﾌｧへのﾎﾟｲﾝﾀ				*/
/*	chr		: 指定ｷｬﾗｸﾀ					*/
/*	len		: ﾊﾞｲﾄ数					*/
/*									*/
/* 戻り値								*/
/*	int		: 0	･･･ すべてchrと一致			*/
/*			  1	･･･ chr以外のデータ			*/
/*----------------------------------------------------------------------*/

int
chrcmp(
	char	*buff,
	char	chr,
	int	len
	)
{

	while( len-- ) {
		if(*buff != chr)	return 1;
		++buff;
	}
	return 0;
}
