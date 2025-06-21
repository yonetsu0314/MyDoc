/*----------------------------------------------------------------------*
 * File name	: eesrom.h		/ serial EE-PROM context header	*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/eesrom.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: eesrom.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/RCS/eesrom.h 1.1 1999/08/26 17:54:27 chimura
 *Log: eesrom.h
 * リビジョン 1.1  1999/08/26  17:54:27  chimura
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1993, ISHIDA  CO., LTD.			*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/
/* 説明									*/
/*	3線式Serial EE-PROMに接続されている、I/Oに対するアクセス	*/
/*	手段を提供するヘッダ。EE-PROM以外のI/Oハードウェア依存部	*/
/*	は全てコーラー側で吸収する。					*/
/*									*/
/*	【出力する関数】						*/
/*	out_handle_xx(ON/OFF);	ON	: 1	… アクティブ		*/
/*				OFF	: 0	… インアクティブ	*/
/*	【入力する関数】						*/
/*	dt = in_handle_xx();	dt	: 入力データ(0/1)		*/
/*----------------------------------------------------------------------*/

#if	!defined(EESROM_H)
#define		EESROM_H
#include	<custom\typedefs.h>

struct eesp {
	void (*out_handle_ce)(int,int,int);	/* eeprom CEの操作関数	*/
	void (*out_handle_di)(int,int,int);	/* eeprom DIの操作関数	*/
	void (*out_handle_sk)(int,int,int);	/* eeprom SKの操作関数	*/
	BYTE (*in_handle_do)(int,int);		/* eeprom DOの操作関数	*/
};

/*void	_ees_rom_init(struct eesp *ioh);*/
void	_eesrom_handle_ce(int devno, int bit_pos, int on_off);
void	_eesrom_handle_di(int devno, int bit_pos, int on_off);
void	_eesrom_handle_sk(int devno, int bit_pos, int on_off);
BYTE	_eesrom_handle_do(int devno, int port_ch);
#endif
