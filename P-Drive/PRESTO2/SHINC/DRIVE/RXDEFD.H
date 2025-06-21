/*----------------------------------------------------------------------*
 * File name	: rxdefd.h	/ define rx116 svc related symbol	*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/RCS/rxdefd.h 1.5 1970/01/01 00:00:00 chimura Exp $
 *$Log: rxdefd.h $
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/12/29 16:00
 * ADCボード３、４用のIDを追加する。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * FIFO用ｾﾏﾌｫｱID追加
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ETHER用ID追加登録
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/02/04 19:00
 * タスク、フラグ、セマフォのID番号をNORTi用に変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/drive/rcs/rxdefd.h 2.8 1998/04/17 15:15:24 yonetsu
 *Log: rxdefd.h
 * リビジョン 2.8  1998/04/17  15:15:24  yonetsu
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(RXDEFD_H)
#define	RXDEFD_H

/*------------------------------------------------------*/
/*	イベント･フラグIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum {
	DRV_EVF_ID	= 10-1,		/* ﾄﾞﾗｲﾊﾞｰは10番以降を採番する	*/
/*-------------------------------------*/
	INC0TX_EVF_ID,			/* I2NETC#0 TX完了ﾌﾗｸﾞID	*/
	INC1TX_EVF_ID,			/* I2NETC#1 TX完了ﾌﾗｸﾞID	*/
	INC2TX_EVF_ID,			/* I2NETC#2 TX完了ﾌﾗｸﾞID	*/
/*-------------------------------------*/
	DADZC3_EVF_ID,			/* DADC3零点完了ﾌﾗｸﾞID		*/
	DADZC4_EVF_ID,			/* DADC4	"		*/
	DADRC3_EVF_ID,			/* DADC3ﾒﾓﾘｰ･ﾘｰﾄﾞ完了ﾌﾗｸﾞID	*/
	DADRC4_EVF_ID,			/* DADC4	"		*/
	DADSPC3_EVF_ID,			/* DADC3ｽﾊﾟﾝ完了ﾌﾗｸﾞID		*/
	DADSPC4_EVF_ID,			/* DADC4	"		*/
/*-------------------------------------*/
	DADZC1_EVF_ID,			/* DADC1零点完了ﾌﾗｸﾞID		*/
	DADZC2_EVF_ID,			/* DADC2	"		*/
	DADRC1_EVF_ID,			/* DADC1ﾒﾓﾘｰ･ﾘｰﾄﾞ完了ﾌﾗｸﾞID	*/
	DADRC2_EVF_ID,			/* DADC2	"		*/
	DADSPC1_EVF_ID,			/* DADC1ｽﾊﾟﾝ完了ﾌﾗｸﾞID		*/
	DADSPC2_EVF_ID,			/* DADC2	"		*/
	DADAFV1_EVF_ID,			/* DADC1AFV完了ﾌﾗｸﾞID		*/
	DADAFV2_EVF_ID,			/* DADC2	"		*/
	DADBOT1_EVF_ID,			/* DADC1ﾌﾞｰﾄ完了ﾌﾗｸﾞID		*/
	DADBOT2_EVF_ID,			/* DADC2	"		*/
	DADMW1_EVF_ID,			/* DADC1ﾒﾓﾘｰ･ﾗｲﾄ完了ﾌﾗｸﾞID	*/
	DADMW2_EVF_ID,			/* DADC2	"		*/
	DADSTAM1_EVF_ID,		/* DADC1ﾓﾆﾀ-･ｽﾀｰﾄ完了ﾌﾗｸﾞID	*/
	DADSTAM2_EVF_ID,		/* DADC2	"		*/
	DADSTPM1_EVF_ID,		/* DADC1ﾓﾆﾀ-･ｽﾄｯﾌﾟ完了ﾌﾗｸﾞID	*/
	DADSTPM2_EVF_ID,		/* DADC2	"		*/
/*-------------------------------------*/
	SPN_EVF_ID,			/* DSPｽﾊﾟﾝ完了ﾌﾗｸﾞID		*/
	CNV_EVF_ID,			/* DSPﾃﾞｰﾀ変換	"		*/
	AFV_EVF_ID,			/* DSPAFV調整完了ﾌﾗｸﾞID		*/
/*-------------------------------------*/
	ETHER_EVF_ID,			/* イーサネットイベントフラグID	*/
/*-------------------------------------*/
	DADAFV3_EVF_ID,			/* DADC1AFV完了ﾌﾗｸﾞID		*/
	DADAFV4_EVF_ID,			/* DADC2	"		*/
	DADBOT3_EVF_ID,			/* DADC1ﾌﾞｰﾄ完了ﾌﾗｸﾞID		*/
	DADBOT4_EVF_ID,			/* DADC2	"		*/
	DADMW3_EVF_ID,			/* DADC1ﾒﾓﾘｰ･ﾗｲﾄ完了ﾌﾗｸﾞID	*/
	DADMW4_EVF_ID,			/* DADC2	"		*/
	DADSTAM3_EVF_ID,		/* DADC1ﾓﾆﾀ-･ｽﾀｰﾄ完了ﾌﾗｸﾞID	*/
	DADSTAM4_EVF_ID,		/* DADC2	"		*/
	DADSTPM3_EVF_ID,		/* DADC1ﾓﾆﾀ-･ｽﾄｯﾌﾟ完了ﾌﾗｸﾞID	*/
	DADSTPM4_EVF_ID,		/* DADC2	"		*/
/*-------------------------------------*/
/*	必ず最後に追加すること		*/
/*	50番以降はｱﾌﾟﾘで使用している	*/
};

/*------------------------------------------------------*/
/*	セマフォIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum  {
	DRV_SEM_ID	= 10-1,		/* ﾄﾞﾗｲﾊﾞｰは100番以降を採番する	*/
/*-------------------------------------*/
	SCURX1_SEM_ID,			/* SCU1 RXｾﾏﾌｫID		*/
	SCIF1RX_SEM_ID=SCURX1_SEM_ID,	/* SCIF1 RXｾﾏﾌｫID			*/
	SCURX2_SEM_ID,			/* SCU1 RXｾﾏﾌｫID		*/
	SCIF2RX_SEM_ID=SCURX2_SEM_ID,/* SCIF2 RXｾﾏﾌｫID			*/
	SCUTX1_SEM_ID,			/* SCU2 TXｾﾏﾌｫID		*/
	SCIF1TX_SEM_ID=SCUTX1_SEM_ID,	/* SCIF1 TXｾﾏﾌｫID			*/
	SCUTX2_SEM_ID,			/* SCU1 TXｾﾏﾌｫID		*/
	SCIF2TX_SEM_ID=SCUTX2_SEM_ID,	/* SCIF2 TXｾﾏﾌｫID			*/
/*-------------------------------------*/
	INC0RX_SEM_ID,			/* I2NETC#0 RXｾﾏﾌｫID		*/
	INC0TX_SEM_ID,			/* I2NETC#0 TXｾﾏﾌｫID		*/
	INC1RX_SEM_ID,			/* I2NETC#1 RXｾﾏﾌｫID		*/
	INC1TX_SEM_ID,			/* I2NETC#1 TXｾﾏﾌｫID		*/
	INC2RX_SEM_ID,			/* I2NETC#2 RXｾﾏﾌｫID		*/
	INC2TX_SEM_ID,			/* I2NETC#2 TXｾﾏﾌｫID		*/
	INC0TXB_SEM_ID,			/* I2NETC#0 TXﾊﾞｯﾌｧｾﾏﾌｫID	*/
	INC1TXB_SEM_ID,			/* I2NETC#1 TXﾊﾞｯﾌｧｾﾏﾌｫID	*/
	INC2TXB_SEM_ID,			/* I2NETC#2 TXﾊﾞｯﾌｧｾﾏﾌｫID	*/
/*-------------------------------------*/
	COM0_SEM_ID,			/* COM20020 #0 ｾﾏﾌｫID		*/
	COM1_SEM_ID,			/* COM20020 #1	"		*/
	COM0BUF_SEM_ID,			/* COM20020 #0 RXﾊﾞｯﾌｧ･ｾﾏﾌｫID	*/
	COM1BUF_SEM_ID,			/* COM20020 #1	"		*/
	COM0TX_SEM_ID,			/* COM20020 #0 TXｾﾏﾌｫID		*/
	COM1TX_SEM_ID,			/* COM20020 #1	"		*/
	COM2_SEM_ID,			/* COM20020 #2 ｾﾏﾌｫID		*/
	COM3_SEM_ID,			/* COM20020 #3	"		*/
	COM2BUF_SEM_ID,			/* COM20020 #2 RXﾊﾞｯﾌｧ･ｾﾏﾌｫID	*/
	COM3BUF_SEM_ID,			/* COM20020 #3	"		*/
	COM2TX_SEM_ID,			/* COM20020 #2 TXｾﾏﾌｫID		*/
	COM3TX_SEM_ID,			/* COM20020 #3	"		*/
/*-------------------------------------*/
	MPS0RX_SEM_ID,			/* MPSC#0 CH-A RXｾﾏﾌｫID		*/
	MPS0TX_SEM_ID,			/* MPSC#0 CH-A TXｾﾏﾌｫID		*/
	MPS1RX_SEM_ID,			/* MPSC#0 CH-B RXｾﾏﾌｫID		*/
	MPS1TX_SEM_ID,			/* MPSC#0 CH-B TXｾﾏﾌｫID		*/
	MPS2RX_SEM_ID,			/* MPSC#1 CH-A RXｾﾏﾌｫID		*/
	MPS2TX_SEM_ID,			/* MPSC#1 CH-A TXｾﾏﾌｫID		*/
	MPS3RX_SEM_ID,			/* MPSC#1 CH-B RXｾﾏﾌｫID		*/
	MPS3TX_SEM_ID,			/* MPSC#1 CH-B TXｾﾏﾌｫID		*/
	MPS4RX_SEM_ID,			/* MPSC#2 CH-A RXｾﾏﾌｫID		*/
	MPS4TX_SEM_ID,			/* MPSC#2 CH-A TXｾﾏﾌｫID		*/
	MPS5RX_SEM_ID,			/* MPSC#2 CH-B RXｾﾏﾌｫID		*/
	MPS5TX_SEM_ID,			/* MPSC#2 CH-B TXｾﾏﾌｫID		*/
/*-------------------------------------*/
	AD1_SEM_ID,			/* ADC1ｾﾏﾌｫID			*/
	AD2_SEM_ID,			/* AD2C	 "			*/
/*-------------------------------------*/
	DAD1TX_SEM_ID,			/* DADC1-TXｾﾏﾌｫID		*/
	DAD1RX_SEM_ID,			/* DADC1-RXｾﾏﾌｫID		*/
	DAD2TX_SEM_ID,			/* DADC2-TX "			*/
	DAD2RX_SEM_ID,			/* DADC2-RX "			*/
/*-------------------------------------*/
	TAD1_SEM_ID,			/* TADC1ｾﾏﾌｫID			*/
	TAD2_SEM_ID,			/* TADC2ｾﾏﾌｫID			*/
/*-------------------------------------*/
	E2_SEM_ID,			/* E2ROMｾﾏﾌｫID			*/
	E2BUF_SEM_ID,			/* E2ROMﾘﾝｸﾞ･ﾊﾞｯﾌｧｾﾏﾌｫID	*/
/*-------------------------------------*/
	EXMCR_SEM_ID,			/* EX.RAMﾘﾝｸﾞ･ﾊﾞｯﾌｧｾﾏﾌｫID	*/
	EXFIL_SEM_ID,			/*   "   file処理用ｾﾏﾌｫID	*/
/*-------------------------------------*/
	DPM0TX_SEM_ID,			/* DPM#0-TXｾﾏﾌｫID		*/
	DPM0RX_SEM_ID,			/* DPM#0-RXｾﾏﾌｫID		*/
	DPM1TX_SEM_ID,			/* DPM#1-TXｾﾏﾌｫID		*/
	DPM1RX_SEM_ID,			/* DPM#1-TXｾﾏﾌｫID		*/
/*-------------------------------------*/
	DSPTX_SEM_ID,			/* DSP-WriteｾﾏﾌｫID		*/
	DSPRX_SEM_ID,			/* DSP-ReadﾏﾌｫID		*/
/*-------------------------------------*/
	VDPM0TX_SEM_ID,			/* Virtual DPM#0-TXｾﾏﾌｫID	*/
	VDPM0RX_SEM_ID,			/* 	"  DPM#0-RXｾﾏﾌｫID	*/
	VDPM1TX_SEM_ID,			/* 	"  DPM#1-TXｾﾏﾌｫID	*/
	VDPM1RX_SEM_ID,			/* 	"  DPM#1-TXｾﾏﾌｫID	*/
/*-------------------------------------*/
	SRAM0_SEM_ID,			/* SRAM #0ｾﾏﾌｫID		*/
	SRAM1_SEM_ID,			/* SRAM #1ｾﾏﾌｫID		*/
/*-------------------------------------*/
	RTC_SEM_ID,			/* RTC ｾﾏﾌｫID			*/
/*-------------------------------------*/
	FR_SEM_ID,			/* FLASH ROMｾﾏﾌｫID		*/
	FRBUF_SEM_ID,			/* FLASH OMﾘﾝｸﾞ･ﾊﾞｯﾌｧｾﾏﾌｫID	*/
	FIFRX1_SEM_ID,
	FIFRX2_SEM_ID,
	FIFTX1_SEM_ID,
	FIFTX2_SEM_ID,
/*-------------------------------------*/
	DAD3TX_SEM_ID,			/* DADC3-TXｾﾏﾌｫID		*/
	DAD3RX_SEM_ID,			/* DADC3-RXｾﾏﾌｫID		*/
	DAD4TX_SEM_ID,			/* DADC4-TX "			*/
	DAD4RX_SEM_ID,			/* DADC4-RX "			*/
/*	既に81個登録されています	*/

/*	<<< ROM libその他 >>>		*/

	OTHER_SEM_ID	= 80-1,		/* その他は80番以降を採番する	*/
/*-------------------------------------*/
	PRINT_SEM_ID,			/* printfのｾﾏﾌｫID		*/
	SPRINT_SEM_ID,			/* sprintfのｾﾏﾌｫID		*/
	SSCAN_SEM_ID,			/* sscanfのｾﾏﾌｫID		*/
	MCB_SEM_ID,			/* dmm systemのｾﾏﾌｫID		*/
/*-------------------------------------*/
/*-------------------------------------*/
/*	必ず最後に追加すること		*/
/*	100番以降はｱﾌﾟﾘで使用している	*/
};

/*------------------------------------------------------*/
/*	メールボックスIDの定義				*/
/*							*/
/*------------------------------------------------------*/

enum  {
	DRV_MBX_ID	= 10-1,		/* ﾄﾞﾗｲﾊﾞｰは10番以降を採番する	*/
/*-------------------------------------*/
	DAD1_MBX_ID,			/* DADC1ゼロ点完了メールBOX-ID	*/
	DAD2_MBX_ID,			/* DADC2	"		*/
	DADAFV_MBX_ID,			/* DADC AFV調整完了メールBOX-ID	*/
/*-------------------------------------*/
	TAD1_INI_MBX_ID,		/* イニシャルパラメータブート	*/
	TAD2_INI_MBX_ID,		/* 	"			*/
	TAD1_INF_MBX_ID,		/* フィルタ情報ブート		*/
	TAD2_INF_MBX_ID,		/* 	"			*/
	TAD1_WDT_MBX_ID,		/* 計量物データ時間ブート	*/
	TAD2_WDT_MBX_ID,		/* 	"			*/
	TAD1_ZAD_MBX_ID,		/* 零点ＡＤ値ブート		*/
	TAD2_ZAD_MBX_ID,		/* 	"			*/
	TAD1_MZA_MBX_ID,		/* 手動零点調整			*/
	TAD2_MZA_MBX_ID,		/* 	"			*/
	TAD1_SPA_MBX_ID,		/* スパン調整			*/
	TAD2_SPA_MBX_ID,		/* 	"			*/
	TAD1_AZA_MBX_ID,		/* 自動零点調整			*/
	TAD2_AZA_MBX_ID,		/* 	"			*/
	TAD1_AFV_MBX_ID,		/* AFV調整			*/
	TAD2_AFV_MBX_ID,		/* 	"			*/
	TAD1_BAL_MBX_ID,		/* バランス調整			*/
	TAD2_BAL_MBX_ID,		/* 	"			*/
	TAD1_SWT_MBX_ID,		/* 静止計量データ読み込み	*/
	TAD2_SWT_MBX_ID,		/* 	"			*/
	TAD1_ERR_MBX_ID,		/* エラーデータ読み込み		*/
	TAD2_ERR_MBX_ID,		/* 	"			*/
	TAD1_D3WT_MBX_ID,		/* 3rd駆動計量データ読み込み	*/
	TAD2_D3WT_MBX_ID,		/* 	"			*/
	TAD1_DYN_MBX_ID,		/* 動補正開始／応答		*/
	TAD2_DYN_MBX_ID,		/* 	"			*/
	TAD1_MRD_MBX_ID,		/* メモリリード			*/
	TAD2_MRD_MBX_ID,		/* 	"			*/
	TAD1_MWR_MBX_ID,		/* メモリライト			*/
	TAD2_MWR_MBX_ID,		/* 	"			*/
/*-------------------------------------*/
	DAD3_MBX_ID,			/* DADC3ゼロ点完了メールBOX-ID	*/
	DAD4_MBX_ID,			/* DADC4	"		*/
/*-------------------------------------*/
/*	必ず最後に追加すること		*/
/*	50番以降はｱﾌﾟﾘで使用している	*/
};

/*------------------------------------------------------*/
/*	タスクIDの定義					*/
/*							*/
/*------------------------------------------------------*/

enum {
	DRV_TSK_ID	= 5-1,		/* ﾄﾞﾗｲﾊﾞｰは5番以降を採番する	*/
/*-------------------------------------*/
	INCRXI_TSK0_ID,			/* I2NETC#0 RX割り込みﾀｽｸID	*/
	INCRXI_TSK1_ID,			/* I2NETC#1	"		*/
	INCRXI_TSK2_ID,			/* I2NETC#2 	"		*/
	INCTXI_TSK0_ID,			/* I2NETC#0 TX割り込みﾀｽｸID	*/
	INCTXI_TSK1_ID,			/* I2NETC#1	"		*/
	INCTXI_TSK2_ID,			/* I2NETC#2 	"		*/
/*-------------------------------------*/
	TMR_TSK_ID,			/* ﾀｲﾏｰ･ﾀｽｸID			*/
/*-------------------------------------*/
	MPS0_FLWRX_TSK_ID,		/* MPSC CH0 ﾌﾛｰ制御(受信)用 ﾀｽｸID  */
	MPS1_FLWRX_TSK_ID,		/* MPSC CH1       "                */
	MPS2_FLWRX_TSK_ID,		/* MPSC CH2       "                */
	MPS3_FLWRX_TSK_ID,		/* MPSC CH3       "                */
	MPS4_FLWRX_TSK_ID,		/* MPSC CH4       "                */
	MPS5_FLWRX_TSK_ID,		/* MPSC CH5       "                */
	MPS0_FLWTX_TSK_ID,		/* MPSC CH0 ﾌﾛｰ制御(送信)用 ﾀｽｸID  */
	MPS1_FLWTX_TSK_ID,              /* MPSC CH1       "                */
	MPS2_FLWTX_TSK_ID,              /* MPSC CH2       "                */
	MPS3_FLWTX_TSK_ID,              /* MPSC CH3       "                */
	MPS4_FLWTX_TSK_ID,              /* MPSC CH4       "                */
	MPS5_FLWTX_TSK_ID,              /* MPSC CH5       "                */
/*-------------------------------------*/
	ADI_TSK1_ID,			/* ADC1割り込みﾀｽｸID		*/
	ADI_TSK2_ID,			/* ADC2		"		*/
/*-------------------------------------*/
	DADI_TSK1_ID,			/* DADC1割り込みﾀｽｸID		*/
	DADI_TSK2_ID,			/* DADC2		"	*/
/*-------------------------------------*/
	TADI_TSK1_ID,			/* TADC1割り込みﾀｽｸID		*/
	TADI_TSK2_ID,			/* TADC2割り込みﾀｽｸID		*/
/*-------------------------------------*/
	E2R_TSK_ID,			/* E2ROM書き込みﾀｽｸID		*/
/*-------------------------------------*/
	DSP_TSK0_ID,			/* DSP割り込みﾀｽｸID(illegal)	*/
	DSP_TSK1_ID,			/* DSP割り込みﾀｽｸID(default)	*/
/*-------------------------------------*/
	COMRX_TSK0_ID,			/* ARC-NET#0割り込みﾀｽｸID	*/
	COMRX_TSK1_ID,			/*	" #1	"		*/
	COMMAP_TSK0_ID,			/* ARC-NET#0のMAP生成ﾀｽｸのID	*/
	COMMAP_TSK1_ID,			/*	" #1	"		*/
	COMRX_TSK2_ID,			/* ARC-NET#0割り込みﾀｽｸID	*/
	COMRX_TSK3_ID,			/*	" #1	"		*/
	COMMAP_TSK2_ID,			/* ARC-NET#0のMAP生成ﾀｽｸのID	*/
	COMMAP_TSK3_ID,			/*	" #1	"		*/
/*-------------------------------------*/
	FR_TSK_ID,			/* E2ROM書き込みﾀｽｸID		*/
/*-------------------------------------*/
	ETHERNET_TSK0_ID,
	ETHERNET_TSK1_ID,
	ETHERNET_TSK2_ID,
	ETHERNET_TSK3_ID,
	ETHERNET_TSK4_ID,
/*-------------------------------------*/
	DADI_TSK3_ID,			/* DADC3割り込みﾀｽｸID		*/
	DADI_TSK4_ID,			/* DADC4		"	*/
/*	必ず最後に追加すること		*/
/*	60番以降はｱﾌﾟﾘで使用している	*/
};

/*------------------------------------------------------*/
/*	タスク･プライオリティの定義			*/
/*							*/
/* 注:	ﾌﾟﾗｲｵﾘﾃｨはenumのトップが最も高く、以下記述	*/
/*	順に低くなる。					*/
/*	    enum {					*/
/*		TSK1_PRI,  ･･･ ﾄﾞﾗｲﾊﾞｰでの最高位	*/
/*		TSK2_PRI,		:		*/
/*		TSKn_PRI   ･･･ ﾄﾞﾗｲﾊﾞｰでの最低位	*/
/*	    }						*/
/*------------------------------------------------------*/

enum {
	DRV_TSK_PRI	= 2-1,		/* ﾄﾞﾗｲﾊﾞｰは2番以降を採番する	*/
/*-------------------------------------*/
	INCRXI_TSK0_RRI	= 2,		/* I2NETC#0 RX INTﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	INCRXI_TSK1_PRI	= 2,		/* I2NETC#1	"		*/
	INCRXI_TSK2_PRI	= 2,		/* I2NETC#2 	"		*/
	INCTXI_TSK0_RRI	= 2,		/* I2NETC#0 TX INTﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	INCTXI_TSK1_PRI	= 2,		/* I2NETC#1	"		*/
	INCTXI_TSK2_PRI	= 2,		/* I2NETC#2 	"		*/
/*-------------------------------------*/
	DSPI_TSK0_PRI	= 2,		/* DSP0割り込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	DSPI_TSK1_PRI	= 2,		/* DSP1		"		*/
/*-------------------------------------*/
	DADI_TSK1_PRI	= 2,		/* DADC1割り込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	DADI_TSK2_PRI	= 2,		/* DADC2	"		*/
	DADI_TSK3_PRI	= 2,		/* DADC3	"		*/
	DADI_TSK4_PRI	= 2,		/* DADC4	"		*/
/*-------------------------------------*/
	COMRX_TSK_PRI	= 3,		/* ARC-NET#0RXﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
/*-------------------------------------*/
	E2R_TSK_PRI	= 4,		/* E2ROM書き込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
/*-------------------------------------*/
	FR_TSK_PRI	= 5,		/* FLASH ROM書き込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ*/
/*-------------------------------------*/
	ADI_TSK1_PRI	= 6,		/* ADC1割り込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	ADI_TSK2_PRI	= 6,		/* ADC2		"		*/
/*-------------------------------------*/
	TADI_TSK1_PRI	= 6,		/* TADC1割り込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
	TADI_TSK2_PRI	= 6,		/* TADC2割り込みﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
/*-------------------------------------*/
	TMR_TSK_PRI	= 7,		/* timer ﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ		*/
/*-------------------------------------*/
	MPS_FLW_TSK_PRI = 8,		/* MPSC ﾌﾛｰ制御 ﾌﾟﾗｲｵﾘﾃｨｰ	*/
/*-------------------------------------*/
	ETHER_NETTSK_PRI=8,		/* Ethernet NETﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/

/*	<<< ROM libその他 >>>		*/

	OTHER_TSK_PRI	= 9-1,		/* その他は9番以降を採番する	*/
/*-------------------------------------*/
	ABORT_TSK_PRI	= 9,		/* abortするﾀｽｸのﾌﾟﾗｲｵﾘﾃｨ	*/
/*-------------------------------------*/
	COMMAP_TSK_PRI	= 9,		/* ARC-MAPﾀｽｸのﾌﾟﾗｲｵﾘﾃｨ		*/
/*-------------------------------------*/
	ETHER_SERVTSK_PRI=9,	/* Ethernet SERVﾀｽｸ･ﾌﾟﾗｲｵﾘﾃｨ	*/
/*-------------------------------------*/
/*	10番以降はｱﾌﾟﾘで使用している	*/
};

#endif	/* RXDEFD_H */

