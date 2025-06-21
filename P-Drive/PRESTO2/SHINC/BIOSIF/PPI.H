/*----------------------------------------------------------------------*
 * File name	: ppi.h		/ pio bios prototype			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/ppi.h 1.5 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: ppi.h $
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 * _exc_in0,_exc_in1追加
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  kawamura
 * 2004/03/15  11:38
 * プロトタイプ宣言修正。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/01/27 21:00
 * プロトタイプ宣言修正
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(PPI_H)
#define	PPI_H

enum pioport {
	PORT_A,
	PORT_B,
	PORT_C,
};

#ifdef	P5280				/* HardwareがP5280の場合	*/

#define	PIO0	0
/*--------------------------------------------------------------*/
/* Function:							*/
/*	LED7をON/OFFする					*/
/*--------------------------------------------------------------*/

#define	_ctl_led7(			/* ret:  Success/0 or Err/-1	*/\
		on_off			/* arg1: ON/OFF data		*/\
		)			\
	_pio_bctl(4, on_off)

#define	_ctl_led6(on_off)	_pio_bctl(5, on_off)
#define	_ctl_led5(on_off)	_pio_bctl(6, on_off)
#define	_ctl_strobe(on_off)	_pio_bctl(PIO0, 7, on_off)

/*--------------------------------------------------------------*/
/* Function:							*/
/*	DIP switchのﾃﾞｰﾀを読み込む				*/
/*--------------------------------------------------------------*/

#define	_dip_sw				/* ret:  intｽｲｯﾁ･ﾃﾞｰﾀ		*/\
	_piob_rd

/*--------------------------------------------------------------*/
/* Function:							*/
/*	外部信号を読み込み、対象の信号をﾏｽｸする			*/
/*--------------------------------------------------------------*/

/*	<<< mask_bpは以下の定数を設定 >>>				*/

#define	RDY_SIG		0x08		/* レディ			*/
#define	INTLK_SIG1	0x04		/* 連動信号1			*/
#define	INTLK_SIG2	0x02		/* 	"  2			*/
#define	AUX_SIG		0x01		/* アクセサリー			*/

#define	_interlock(			/* ret:  Input data		*/\
		mask_bp			/* arg1: mask bit pattern	*/\
		)			\
		(_pio_rd(PIO0,PORT_C) & mask_bp)


#else					/* それ以外のHaredware		*/

enum pio_devch {
	PIO0,
	PIO1,
	PIO2,
	PIO3
};

/*--------------------------------------------------------------*/
/* Function:							*/
/*	外部出力信号0をRDする				*/
/*--------------------------------------------------------------*/

#define	_exc_in0(			/* ret:  Success/0 or Err/-1	*/\
		 )	                \
	_pio_bctlrd(PIO0)

#define	_exc_in1(			/* ret:  Success/0 or Err/-1	*/\
		 )	                \
	_pio_bctlrd(PIO1)

/*--------------------------------------------------------------*/
/* Function:							*/
/*	外部出力信号0をON/OFFする				*/
/*--------------------------------------------------------------*/

#define	_exc_out0(i,			/* ret:  Success/0 or Err/-1	*/\
		 on_off                 /* arg1: ON/OFF data		*/\
		 )	                \
	_pio_bctl(PIO0, i, on_off)

#define	_exc_out1(i,			/* ret:  Success/0 or Err/-1	*/\
		 on_off                 /* arg1: ON/OFF data		*/\
		 )	                \
	_pio_bctl(PIO1, i, on_off)

#define	_exc_out00(			/* ret:  Success/0 or Err/-1	*/\
		on_off			/* arg1: ON/OFF data		*/\
		)			\
	_pio_bctl(POI0, 0, on_off)

#define	_exc_out01(on_off)	_pio_bctl(PIO0, 1, on_off)
#define	_exc_out02(on_off)	_pio_bctl(PIO0, 2, on_off)
#define	_exc_out03(on_off)	_pio_bctl(PIO0, 3, on_off)
#define	_exc_out04(on_off)	_pio_bctl(PIO0, 4, on_off)
#define	_exc_out05(on_off)	_pio_bctl(PIO0, 5, on_off)
#define	_exc_out06(on_off)	_pio_bctl(PIO0, 6, on_off)
#define	_exc_out07(on_off)	_pio_bctl(PIO0, 7, on_off)
#define	_exc_out10(on_off)	_pio_bctl(PIO1, 0, on_off)
#define	_exc_out11(on_off)	_pio_bctl(PIO1, 1, on_off)
#define	_exc_out12(on_off)	_pio_bctl(PIO1, 2, on_off)
#define	_exc_out13(on_off)	_pio_bctl(PIO1, 3, on_off)
#define	_exc_out14(on_off)	_pio_bctl(PIO1, 4, on_off)
#define	_exc_out15(on_off)	_pio_bctl(PIO1, 5, on_off)
#define	_exc_out16(on_off)	_pio_bctl(PIO1, 6, on_off)

#ifdef	P5561
#define	_exc_out17(on_off)	_pio_bctl(PIO3, 0, on_off)
#else
#define	_exc_out17(on_off)	_pio_bctl(PIO1, 7, on_off)
#endif

/*--------------------------------------------------------------*/
/* Function:							*/
/*	外部信号を読み込み、対象の信号をﾏｽｸする			*/
/*--------------------------------------------------------------*/

/*	<<< mask_bpは以下の定数を設定 >>>				*/

#define	INTLK_SIG1	0x04		/* 連動信号1			*/
#define	INTLK_SIG2	0x02		/* 	"  2			*/

#define	_interlock(			/* ret:  Input data		*/\
		mask_bp			/* arg1: mask bit pattern	*/\
		)			\
		(_pio_rd(PIO0,PORT_B) & mask_bp)
#endif


int	_pio_init(int);
int	_pio_wr(int, int, unsigned char);
unsigned char	_pio_rd(int, int);
int	_pio_bctl(int, int, int);
int	_pio_bctlrd(int);

/*----------------------------------------------------------------------*/
/*	<<< return parameter >>>					*/

typedef struct pioinf {
	unsigned short	level_port;	/* 割り込みのﾚﾍﾞﾙ入力ﾎﾟｰﾄ	*/
	unsigned short	xor_mask_bp;	/* 論理反転&ﾏｽｸ･ﾋﾞｯﾄﾊﾟﾀｰﾝ	*/
} PIOINF;

const PIOINF *_pio_icu_port(int, int);	/* P-5374 ICU 専用		*/

/*----------------------------------------------------------------------*/
/*	<<< argument 1 >>>						*/

enum piosr {
	PIO_RES,
	PIO_SET,
};

/*	<<< argument 2 >>>						*/
#ifdef	P5554
	enum p175 {
		PIO_AUX		= 0x1,		/* AUX port		*/
		DSP_RESET	= 0x2,		/* DSP		"	*/
		DADC1_RESET	= 0x4,		/* DADC1 board reset	*/
		DADC2_RESET	= 0x8,		/*   " 2	"	*/
	};
#else
	#ifdef	P5561 
		enum p175 {
			DADC1_RESET	= 0x1,	/* DADC1 board reset	*/
			DADC2_RESET	= 0x2,	/*   " 2	"	*/
			DADC3_RESET	= 0x4,	/*   " 3	"	*/
			DADC4_RESET	= 0x8,	/*   " 4	"	*/
			PIO_AUX		= 0x10,	/* AUX port		*/
		};
	#else
		enum p175 {
			DADC1_RESET	= 0x2,	/* DADC1 board reset	*/
			DADC2_RESET	= 0x1,	/*   " 2	"	*/
			DSP_RESET	= 0x4,	/* DSP		"	*/
			PIO_AUX		= 0x8,	/* AUX port		*/
		};
	#endif
#endif

int	_pio175_wr(enum piosr, enum p175);/* P-5348 WCU 専用	*/

#endif	/* PPI_H */

