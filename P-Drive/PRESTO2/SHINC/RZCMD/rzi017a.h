/*======================================================================
 * File name    : rzi017a.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	Ver3用異常発生データ構造
 *----------------------------------------------------------------------
 *$Header: p:/PRESTO2/SHINC/RZCMD/RCS/rzi017a.h 1.4 2018-06-27 17:19:46+09 itami Exp $
 *$Log: rzi017a.h $
 *Revision 1.4  2018-06-27 17:19:46+09  itami
 *詰まりエラー、分散モーターエラー追加
 *
 *Revision 1.3  2017-04-28 14:13:25+09  itami
 *先端ＷＨエラー,センサー検知エラー追加
 *
 *Revision 1.2  2017-04-28 13:14:22+09  itami
 *コンベヤエラー追加
 *
 *Revision 1.1  2003-01-28 01:07:13+09  chimura
 *初期リビジョン
 *
 *----------------------------------------------------------------------
 *Header: p:/include/rzcmd/rcs/rzi017a.h 1.14 1970/01/01 00:00:00 itami
 *Log: rzi017a.h
 * リビジョン 1.14  1970/01/01  00:00:00  itami
 * 2002/04/18 先端ＰＨエラー追加
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#if	!defined(RZI017_H)
#define	RZI017_H


enum rzi0170e {				/* 異常内容			*/
	ERROR_ZERO,			/* 零点				*/
	ERROR_SPAN,			/* スパン			*/
	ERROR_WH,			/* ＷＨ				*/
	ERROR_PH,			/* ＰＨ				*/
	ERROR_BH,			/* ＢＨ				*/
	ERROR_RS,			/* ＲＳ				*/
	ERROR_DTH,			/* ＤＴＨ			*/
	ERROR_TH,			/* ＴＨ				*/
	ERROR_PW,			/* 単重				*/
	ERROR_OVSCL,			/* オーバースケール		*/
	ERROR_OVWGT,			/* 過量				*/
	ERROR_AFD,			/* AFD				*/
	ERROR_CHT,			/* 上下動シュート		*/
	ERROR_RCH,			/* 再チェックエラー（個数計量）	*/
	ERROR_NETWORK,			/* 通信エラー			*/
	ERROR_AD,			/* AD				*/
	ERROR_RAM,			/* RAM				*/
	ERROR_ROM,			/* ROM				*/
	ERROR_AUTO_DF,			/* 分散重量自動変更		*/
	ERROR_METAL,			/* 金属検出			*/
	ERROR_AIR,			/* エアー圧低下			*/
	ERROR_DOOR,			/* 扉開放			*/
	ERROR_DIS,			/* 排出物つまり			*/
	ERROR_ILK,			/* 連動エラー			*/
	ERROR_CUP,			/* カップエラー			*/
	ERROR_TRIP,			/* トリップエラー		*/
	ERROR_FATAL_ZERO,		/* 致命的零点エラー		*/
        ERROR_COUNT,                    /* 個数換算エラー               */
        ERROR_DIVERTER,                 /* 振り分け部エラー             */
        ERROR_EMERGENCY_STOP,          	/* 非常停止		        */
	ERROR_UNDWGT,			/* 軽量停止			*/
	ERROR_FDRV,			/* FDRVエラー			*/
	ERROR_TB,			/* TBエラー			*/
	ERROR_MB,			/* MBエラー			*/
	ERROR_BB,			/* BBエラー			*/
	ERROR_REJ,			/* 不良振り分け部エラー		*/
	ERROR_TPH,			/* 先端ＰＨエラー		*/
	ERROR_TWH,			/* 先端ＷＨエラー		*/
	ERROR_DETECT,			/* センサー検知エラー		*/
	ERROR_CONVEYOR,			/* コンベヤエラー		*/
	ERROR_JAM,			/* 詰まりエラー			*/
	ERROR_SMP,			/* サンプリング重量エラー	*/
	ERROR_SYNC,			/* 同期エラー			*/
	ERROR_DF_MOTOR,			/* 分散モーターエラー		*/
	ERROR_NON = 255,		/* 拡張側エラー			*/
};

enum rzi0171e {				/* ＤＵＣ通信エラー情報		*/
        ERR_D_CONNECT	= 0x01,  	/* 回線断エラー			*/
        ERR_D_NODE	= 0x02,		/* ノード無しエラー		*/
};

enum rzi0172e {				/* ＥＸＣ通信エラー情報		*/
        ERR_E_CONNECT	= 0x01,  	/* 回線断エラー			*/
        ERR_E_NODE	= 0x02,		/* ノード無しエラー		*/
};

enum rzi0173e {				/* ＤＵＣエラー情報		*/
        ERR_D_CALC	= 0x01,  	/* 駆動パターン計算エラー	*/
        ERR_D_RECEPT	= 0x02,		/* 駆動パラメータ未受信エラー	*/
        ERR_D_STM	= 0x04,  	/* 駆動エラー			*/
        ERR_D_POWER	= 0x08,		/* パワーエラー			*/
};

enum rzi0174e {				/* ＥＸＣエラー情報		*/
        ERR_E_STM	= 0x01,  	/* ｽﾃｯﾋﾟﾝｸﾞﾓｰﾀ異常停止エラー	*/
        ERR_E_OVERLAP	= 0x02,  	/* 動作オーバーラップエラー	*/
        ERR_E_RECEPT	= 0x04,		/* 駆動パラメータ未受信エラー	*/
        ERR_E_POWER	= 0x08,		/* パワーエラー			*/
};

enum rzi0175e {				/* ＦＤＲＶエラー情報		*/
        ERR_F_COMERR	= 0x01,  	/* 通信エラー			*/
        ERR_F_RECEPT	= 0x02,  	/* 駆動パラメータ未受信エラー	*/
        ERR_F_OVC	= 0x04,		/* 過電流エラー			*/
        ERR_F_POWER	= 0x08,		/* パワーエラー			*/
};

enum rzi0176e {				/* ＦＤＲＶボード番号情報	*/
        FDRV_BOARD_1	= 0x01,  	/* FDRVボード1			*/
        FDRV_BOARD_2	= 0x02,  	/* FDRVボード2			*/
        FDRV_BOARD_3	= 0x04,		/* FDRVボード3			*/
        FDRV_BOARD_4	= 0x08,		/* FDRVボード4			*/
};
/*------------------------------------------------------*/
/*	異常情報のパラメータパケット			*/
/*							*/
/*------------------------------------------------------*/

typedef struct rzi017t {
	unsigned char	ch_id;		/* channel number		*/
	unsigned char	nof_head;	/* number of head		*/
	enum rzi0170e	error;		/* 異常情報			*/
	unsigned long	ph_ptn;		/* PH pattern			*/
	unsigned long	wh_ptn;		/* WH pattern			*/
	unsigned long	bh_ptn;		/* BH pattern			*/
	unsigned char	rs_ptn;		/* RS pattern			*/
	unsigned char	dth_ptn;	/* DTH pattern			*/
	unsigned char	th_ptn;		/* TH pattern			*/
	unsigned char	df_ptn;		/* DF pattern			*/
	unsigned char	duc_inf;	/* DUC error information	*/
	unsigned char	exc_inf;	/* EXC error information	*/
} RZI017T;

#if	defined(STORAGE)
struct rzi017t	rzi017s;
#else
#if	!defined(NOEXTERN)
extern struct rzi017t	rzi017s;
#endif
#endif

#define		rz_scan_error	rz_scan_error_v3
#define		rz_print_error	rz_print_error_v3

#endif
