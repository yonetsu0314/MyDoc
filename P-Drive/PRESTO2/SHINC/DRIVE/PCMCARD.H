/*======================================================================
 * File name    : pcmcard.h
 * Original		: r:/c/astra-s/exp/rcs/pcmcard.h 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shinc/drive/RCS/pcmcard.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmcard.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * 新規リビジョン
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*                                                                      */
/*      ファイル名      : PCMCARD.H                                     */
/* ==================================================================== */
/*      機能分類        : ＰＣＭＣＩＡカードサービス			        */
/*      作成日付        : 1997.12. 5 < Fri >                            */
/*      作成者名        :                                               */
/*                                                                      */
/*  履歴管理                                                            */
/* ------------+----------+-------------------------------------------- */
/*  日 付      | 管理番号 | 担当者名及び変更内容                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#ifndef __PCMCARD__
#define __PCMCARD__


	/*==========================================================
							定数宣言
	==========================================================*/
/* for SetPcmciaIntHandler */
#define LEVEL_INT 0
#define EDGE_INT  1

/* Return codes */
#define CS_SUCCESS				0x00
#define CS_BAD_ADAPTER			0x01
#define CS_BAD_ATTRIBUTE		0x02
#define CS_BAD_BASE				0x03
#define CS_BAD_EDC				0x04
#define CS_BAD_IRQ				0x06
#define CS_BAD_OFFSET			0x07
#define CS_BAD_PAGE				0x08
#define CS_READ_FAILURE 		0x09
#define CS_BAD_SIZE				0x0a
#define CS_BAD_SOCKET			0x0b
#define CS_BAD_TYPE				0x0d
#define CS_BAD_VCC				0x0e
#define CS_BAD_VPP				0x0f
#define CS_BAD_WINDOW			0x11
#define CS_WRITE_FAILURE		0x12
#define CS_NO_CARD				0x14
#define CS_UNSUPPORTED_FUNCTION 0x15
#define CS_UNSUPPORTED_MODE		0x16
#define CS_BAD_SPEED			0x17
#define CS_BUSY 				0x18
#define CS_GENERAL_FAILURE		0x19
#define CS_WRITE_PROTECTED		0x1a
#define CS_BAD_ARG_LENGTH		0x1b
#define CS_BAD_ARGS				0x1c
#define CS_CONFIGURATION_LOCKED 0x1d
#define CS_IN_USE				0x1e
#define CS_NO_MORE_ITEMS		0x1f
#define CS_OUT_OF_RESOURCE		0x20
#define CS_BAD_HANDLE			0x21
#define CS_BAD_TUPLE			0x40

/* Bits in IRQInfo1 field */
#define IRQ_MASK			0x0f
#define IRQ_NMI_ID			0x01
#define IRQ_IOCK_ID			0x02
#define IRQ_BERR_ID			0x04
#define IRQ_VEND_ID			0x08
#define IRQ_INFO2_VALID 	0x10
#define IRQ_LEVEL_ID		0x20
#define IRQ_PULSE_ID		0x40
#define IRQ_SHARE_ID		0x80

	/*==========================================================
							構造体宣言
	==========================================================*/
typedef struct {
	struct {
		unsigned long  last;		/* 前回イベント内容	*/
		unsigned long  event;		/* イベント内容		*/
	} sock[1];
} PCMEVT;

	/*==========================================================
						プロトタイプ宣言
	==========================================================*/
static void CardCtl( PCMEVT *evt );
void InitCardCtl( void );
void ExitCardCtl( void );
void ResetCard( int sock, unsigned char *config );
void SetPcmciaIntHandler( int sock, int mode, void (*fptr)( int no ) );
int SetUserCardHandler( void (*fptr)( int sock, int type, int event ) );
void SetDriverID( int sock, int id );
int GetDriverID( int sock );
int is_register_client_callback( void *client );
void intPCM( void );
#endif
