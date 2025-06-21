/*----------------------------------------------------------------------*
 * File name	: mpsc.h	/ mpsc bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/mpsc.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: mpsc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/mpsc.h 1.2 1996/02/26 08:27:03 ryoji
 *Log: mpsc.h
 * リビジョン 1.2  1996/02/26  08:27:03  ryoji
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/


#if	!defined(MPSC_H)
#define	MPSC_H

/*------------------------------------------------------*/
/*	MPSC I/O register number define			*/
/*							*/
/*------------------------------------------------------*/

enum mpscreg {
	_CR0,				/* ﾚｼﾞｽﾀ選択機能 各種ﾄﾘｶﾞ･ｺﾏﾝﾄﾞ */
	_CR1,				/* 割り込み，ＤＭＡ　*/
	_CR2,
	_CR3,
	_CR4,
	_CR5,
	_CR6,
	_CR7,
	_CR8,
	_CR9,
	_CR10,
	_CR11,
	_CR12,
	_CR13,
	_CR14,
	_CR15,
	NO_OPE		= 0x0,		/* No Operation			*/
	RES_ES		= 0x10,		/* E/Sリセット			*/
	CH_RES		= 0x18,		/* Channel reset		*/
	RES_TX_INT	= 0x28,		/* TX割り込みリセット		*/
	MPSC_ERST	= 0x30,		/*				*/
	MPSC_EOI	= 0x38,		/*				*/
};
/*----------------------------------------------------------------------*/

/*	<<< control register 1 >>>	       				*/

#define	OV_RUN_SRXI	0x40		/* ｵｰﾊﾞｰﾗﾝ･ｴﾗｰを特別RX割り込みとする */
#define	FRXI_ENABLE	0x20		/* first Rx割り込みの許可	*/
#define	RX_INT_M1	0x8		/* RX割り込みmode1		*/
#define	RX_INT_M2	0x10		/* RX割り込みmode2		*/
#define	RX_INT_M3	0x18		/* RX割り込みmode3		*/
#define	FTXI_ENABLE	0x4		/* first Tx割り込みの許可	*/
#define	ENA_TX_INT	0x2		/* TX割り込み/DMAのenable	*/
#define	ENA_ES_INT	0x1		/* E/S割り込みenable		*/
#define	DIS_ES_INT	(~ENA_ES_INT)	/* E/S割り込みdisable		*/

/*	<<< control register 2 >>>	       				*/

#define	VEC_M0		0x0		/* vector mode 0		*/
#define	VEC_M1		0x80		/* vector mode 1		*/
#define	FIXED_VEC	0x0		/* vector内容の固定		*/
#define	AFFECTS_VEC	0x40		/* vector内容の変化		*/
#define	VEC_A1		0x0		/* vector type A-1		*/
#define	VEC_A2		0x8		/* vector type A-2		*/
#define	VEC_A3		0x10		/* vector type A-3		*/
#define	VEC_B1		0x18		/* vector type B-1		*/
#define	VEC_B2		0x20		/* vector type B-2		*/
#define	TXA_RXB		0x0		/* 優先順位 TxA > RxB		*/
#define	RXB_TXA		0x4		/* 優先順位 RxB > TxA		*/
#define	BOTH_INT	0x0		/* A,Bﾁｬﾈﾙ割り込みﾓｰﾄﾞ		*/
#define	ADMA_BINT	0x1		/* AﾁｬﾈﾙDMA､Bﾁｬﾈﾙ割り込みﾓｰﾄﾞ	*/
#define	BOTH_DMA	0x2		/* A,BﾁｬﾈﾙDMAﾓｰﾄﾞ		*/

/*	<<< control register 3 >>>	       				*/

#define	RX_5CLEN	0x0		/* 5bit長受信ｷｬﾗｸﾀｰ		*/
#define	RX_6CLEN	0x80		/* 6bit長受信ｷｬﾗｸﾀｰ		*/
#define	RX_7CLEN	0x40		/* 7bit長受信ｷｬﾗｸﾀｰ		*/
#define	RX_8CLEN	0xc0		/* 8bit長受信ｷｬﾗｸﾀｰ		*/
#define	AUTO_ENABLE	0x20		/* CTS,DCD,RST端子受送信制御用	*/
#define	RX_ENABLE	0x1		/* RX動作開始			*/
#define	RX_DISABLE	(~RX_ENABLE)	/* RX動作停止			*/

/*	<<< control register 4 >>>	       				*/

#define	CLK16		0x40		/* clock rate /16		*/
#define	CLK32		0x80		/* clock rate /32		*/
#define	CLK64		0xc0		/* clock rate /64		*/
#define	SYNC_MONO	0x0		/* ASYNC mode			*/
#define	SYNC_BI		0x10		/* Bisync mode			*/
#define	SYNC_HDLC	0x20		/* HDLC mode			*/
#define	SYNC_EXT	0x30		/* External sync mode		*/
#define	STP_BIT0	0x0		/* stop bits 0HDLC		*/
#define	STP_BIT1	0x4		/* stop bits 1			*/
#define	STP_BIT1_5	0x8		/* stop bits 1.5		*/
#define	STP_BIT2	0xc		/* stop bits 2			*/
#define	PARI_EVEN	0x3		/* even parity			*/
#define	PARI_ODD	0x1		/* odd parity			*/
#define	PARI_NO		0x0		/* no parity			*/

/*	<<< control register 5 >>>	       				*/

#define	DTR_ON		0x80		/* DTR on			*/
#define	DTR_OFF		(~DTR_ON)	/* DTR off			*/
#define	RTS_ON		0x2		/* RTS on			*/
#define	RTS_OFF		(~RTS_ON)	/* RTS off			*/
#define	TX_5CLEN	0x0		/* 5bit長送信ｷｬﾗｸﾀｰ		*/
#define	TX_6CLEN	0x40		/* 6bit長送信ｷｬﾗｸﾀｰ		*/
#define	TX_7CLEN	0x20		/* 7bit長送信ｷｬﾗｸﾀｰ		*/
#define	TX_8CLEN	0x60		/* 8bit長送信ｷｬﾗｸﾀｰ		*/
#define	SEND_BREAK	0x10		/* send break機能		*/
#define	TX_ENABLE	0x8		/* TX動作開始			*/
#define	TX_DISABLE	(~TX_ENABLE)	/* TX動作停止			*/

/*	<<< control register 10 >>>	       				*/

#define	NRZ		0x0		/*				*/

/*	<<< control register 11 >>>	       				*/

#define	BREAK_EI	0x80		/* break E/S割り込み許可	*/
#define	TUE_EI		0x40		/* TUE E/S割り込み許可		*/
#define	CTS_EI		0x20		/* CTS E/S割り込み許可		*/
#define	SYNH_EI		0x10		/* SYNH E/S割り込み許可		*/
#define	DCD_EI		0x8		/* DCD E/S割り込み許可		*/
#define	AS_EI		0x4		/* AS E/S割り込み許可		*/
#define	ID_EI		0x2		/* ID E/S割り込み許可		*/
#define	BRG_EI		0x1		/* BRG E/S割り込み許可		*/
#define	ES_DISABLE	0x0		/* E/S割り込み禁止		*/

/*	<<< control register 12 >>>	       				*/

#define	TRXC_RXBRG	0x0		/* RxBRGクロックがTRxC端子から出力 */
#define	TRXC_TXBRG	0x80		/* TxBRGクロックがTRxC端子から出力 */
#define	DPLL_RXBRG	0x0		/* RxBRGクロックがDPLL回路のソース */
#define	DPLL_TXBRG	0x40		/* TxBRGクロックがDPLL回路のソース */
#define	TXBRG_EI	0x8		/* TxBRG割り込み許可		*/
#define	TXBRG_DI	(~TXBRG_EI)	/* TxBRG割り込み禁止		*/
#define	RXBRG_EI	0x4		/* RxBRG割り込み許可		*/
#define	RXBRG_DI	(~TXBRG_EI)	/* RxBRG割り込み禁止		*/
#define	TXBRG_REG	0x2		/* TxBRGレジスタセット		*/
#define	RXBRG_REG	0x1		/* RxBRGレジスタセット		*/

/*	<<< control register 14 >>>	       				*/

#define	SYS_CLK		0x4		/* システムクロック		*/
#define	RXBRG_ENA	0x2		/* RxBRG動作開始		*/
#define	RXBRG_DIS	(~RXBRG_ENA)	/* RxBRG動作停止		*/
#define	TXBRG_ENA	0x1		/* TxBRG動作開始		*/
#define	TXBRG_DIS	(~TXBRG_ENA)	/* TxBRG動作停止		*/

/*	<<< control register 15 >>>	       				*/

#define	XTAL		0x80		/* 水晶発振回路の使用		*/
#define	SRXC_INP	0x0		/* STRxC端子が受信用クロック	*/
#define	RXC_INP		0x20		/* TRxC端子が受信用クロック	*/
#define	RXBRG_CLK	0x40		/* RXBGRが受信用クロック	*/
#define	RX_DPLL_CLK	0x60		/* DPLLが送信用クロック		*/
#define	STXC_INP	0x0		/* STRxC端子が送信用クロック	*/
#define	TXC_INP		0x8		/* TRxC端子が送信用クロック	*/
#define	TXBRG_CLK	0x10		/* RXBGRが送信用クロック	*/
#define	TX_DPLL_CLK	0x18		/* DPLLが送信用クロック		*/
#define	TRXC_OUT	0x4		/* TRxC端子の入出力		*/
#define	SCR_XTAL	0x0		/* クリスタルがTRxCのソースクロック */
#define	SCR_TXC		0x1		/* TxCLKがTRxCのソースクロック  */
#define	SCR_BRG		0x2		/* BGRがTRxCのソースクロック	*/
#define	SCR_DPLL	0x3		/* DPLLがTRxCのソースクロック	*/

/*----------------------------------------------------------------------*/
enum mpssreg {
	_SR0,
	_SR1,				/* */
	_SR2B,
	_SR3,
	_SR4A,
	_SR8 		= _CR8,
	_SR9,
	_SR10,
	_SR11,				/* E/S割り込みﾏｽｸ･ﾋﾞｯﾄ･ｽﾃｰﾀｽ 	*/
	_SR12,                          /* 受信BRG設定値ﾛｳ･ﾊﾞｲﾄ		*/
	_SR13,                          /* 受信BRG設定値ﾊｲ･ﾊﾞｲﾄ		*/
	_SR14,                          /* 送信BRG設定値ﾛｳ･ﾊﾞｲﾄ		*/
	_SR15,                          /* 送信BRG設定値ﾊｲ･ﾊﾞｲﾄ		*/
};

/*	<<< status register 0 >>>	       				*/

#define	FRAM_ERR	0x40		/* ﾌﾚｰﾐﾝｸﾞ･ｴﾗｰの発生		*/
#define	OVRUN_ERR	0x20		/* ｵｰﾊﾞｰ･ﾗﾝ･ｴﾗｰの発生		*/
#define	PARI_ERR	0x10		/* ﾊﾟﾘﾃｨ･ｴﾗｰの発生		*/
#define	TXBUF_EMP	0x4		/* TXﾊﾞｯﾌｧｰ内のﾃﾞｰﾀの有無	*/
#define	RXCHR_AVL	0x1		/* RXﾊﾞｯﾌｧｰ内ﾃﾞｰﾀの有無		*/

/*	<<< status register 1 >>>	       				*/

#define	BRK_ABO		0x80		/* ﾌﾞﾚｰｸ/ｱﾎﾞｰﾄ･ｼｰｹﾝｽの検知	*/
#define	TX_UNDRUN	0x40		/* Tx underrun状態の検知	*/
#define	CTS		0x20		/* CTS入力のﾓﾆﾀｰ		*/
#define	SYNC_HUNT	0x10		/* SYNC入力/Huntﾌｪｰｽﾞのﾓﾆﾀｰ	*/
#define	DCD		0x8		/* DCD入力のﾓﾆﾀｰ		*/
#define	ALL_SENT	0x4		/* ﾄﾗﾝｽﾐｯﾀ内の全ｷｬﾗｸﾀの送出完了	*/
#define	BRG_ZERO	0x1		/* BRGｶｳﾝﾄがｾﾞﾛ			*/

/*	<<< status register 3 >>>	       				*/

#define	TXBRG_ZERO	0x10		/* TxBRGのｶｳﾝﾄ値ｾﾞﾛ		*/
#define	RXBRG_ZERO	0x8		/* RxBRGのｶｳﾝﾄ値ｾﾞﾛ		*/

/*	<<< status register 4 >>>	       				*/

#define	A_SR_PENDING	0x80		/* ch-A 特別受信割り込みの発生	*/
#define	B_SR_PENDING	0x40		/* ch-B 特別受信割り込みの発生	*/
#define	A_RX_PENDING	0x20		/* ch-A 受信割り込みの発生	*/
#define	A_TX_PENDING	0x10		/* ch-A 送信割り込みの発生	*/
#define	A_ES_PENDING	0x8		/* ch-A E/S割り込みの発生	*/
#define	B_RX_PENDING	0x4		/* ch-B 受信割り込みの発生	*/
#define	B_TX_PENDING	0x2		/* ch-B 送信割り込みの発生	*/
#define	B_ES_PENDING	0x1		/* ch-B E/S割り込みの発生	*/

/*----------------------------------------------------------------------*/

#define	setcr(x)	((x))
#define	rescr(x)	(((~(x))<<8) & 0xff00)
#define	SET_VOID	0		/* ｾｯﾄしない場合の設定		*/
#define	RES_VOID	0		/* ﾘｾｯﾄしない場合の設定		*/


typedef struct mpspinf {
	unsigned short	io_adr_ad;	/* id i/o address		*/
	unsigned short	io_adr_ac;	/* id i/o address		*/
	unsigned short	io_adr_bd;	/* id i/o address		*/
	unsigned short	io_adr_bc;	/* id i/o address		*/
	unsigned short	int_dma;	/* INT/DMA			*/
#define	INT_USE		0x1
#define	INTAK_USE	0x2
#define	DMAA_USE	0x4
#define	DMAB_USE	0x8
	unsigned short	modem_ctl;	/* modem control		*/
#define	RTSA_USE	0x1
#define	RTSB_USE	0x2
#define	CTSA_USE	0x4
#define	CTSB_USE	0x8
#define	DTRA_USE	0x10
#define	DTRB_USE	0x20
#define	DCDA_USE	0x40
#define	DCDB_USE	0x80
	unsigned short	clk_type;	/* clock type			*/
#define	XTALA_USE	0x1
#define	XTALB_USE	0x2
#define	STRCA_USE	0x4
#define	STRCB_USE	0x8
#define	SYNCA_USE	0x10
#define	SYNCB_USE	0x20
#define	TRXCA_USE	0x40
#define	TRXCB_USE	0x80
	unsigned long	sys_clk;	/* system clock			*/
} MPSPINF;

const MPSPINF *	_mpsc_init(int);

int	_mpsc_cr_write(const void *, enum mpscreg, int);
int	_mpsc_sr_read(const void *, enum mpssreg);
int	_mpsc_data_read(const void *);
int	_mpsc_data_write(const void *, unsigned char);

#endif	/* MPSC_H */

