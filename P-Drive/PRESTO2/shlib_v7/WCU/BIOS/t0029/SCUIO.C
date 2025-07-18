/*----------------------------------------------------------------------*
 * File name	: scuio.c	/ scu io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/scuio.c 1.3 1970/01/01 00:00:00 chimura Exp $
 *$Log: scuio.c $
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 * 2008/01/07
 * SCU 偶数／奇数パリティが反対であった不具合修正。（マニュアルの誤記）
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2007/12/19
 * SCU 2400bpsで動作しない不具合修正。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
/*#include <biosif\scua.h>*/
#include <intrpt.h>
#include <sh7706\7706.h>

#define P5561
#define	SCU_DEV_MAX	(2)

#define	C1200BPS	(0)
#define	C2400BPS	(1)
#define	C4800BPS	(2)
#define	C9600BPS	(3)
#define	C19KBPS		(4)
#define	C38KBPS		(5)
#define	C57KBPS		(6)
#define	C115KBPS	(7)
#define	SCU_IREQ_PRI	((unsigned short)5)

void	scu_io(void);
int	*scu_io_init(int, int, int,int);
int	scu_io_scm(int,int);
int	scu_io_sst(int);
int	scu_io_putc(int,int);
int	scu_io_baud(int,int);

static unsigned char baudrate[SCU_DEV_MAX];
static unsigned char cke[SCU_DEV_MAX];


#pragma	inline_asm(scu_io)
static void scu_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_scu,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          scu_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
scu_exit:
          MOV.L       Lrsxxx,R0   ;scu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _scu_io_err

          .ALIGN 4
jt_scu:
          .DATA.L     _scu_io_init  ; r0 = 0
          .DATA.L     _scu_io_scm   ;    = 1
          .DATA.L     _scu_io_sst   ;    = 2
          .DATA.L     _scu_io_putc  ;    = 3
          .DATA.L     _scu_io_baud  ;    = 4
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int
scu_io_err(void)
{
	return(0);
}

int *
scu_io_init(
	int	ch,
	int	char_len,			/* ｷｬﾗｸﾀ長		*/
	int	stp_len,			/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int	pari_sel			/* ﾊﾟﾘﾃｨｰ選択		*/
)
{
	int	*p;

	disable_interrupt();
	if(ch == 0){
		SCI.SCSCR.BYTE = (unsigned char)0;
		SCI.SCSCR.BIT.CKE = cke[ch];
		SCI.SCBRR = baudrate[ch];
		SCI.SCSMR.BIT.CHR = (unsigned char)(char_len ^ 0x01);
		if(pari_sel){
			SCI.SCSMR.BIT._PE = (unsigned char)1;
			SCI.SCSMR.BIT.OE  = (unsigned char)(pari_sel & 0x01);
		} else {
			SCI.SCSMR.BIT._PE = (unsigned char)0;
		}
		SCI.SCSMR.BIT.STOP = (unsigned char)stp_len;
		SCI.SCSSR.BIT.RDRF = (unsigned char)0;
		SCI.SCSSR.BIT.PER = (unsigned char)0;
		SCI.SCSCR.BIT.RIE  = (unsigned char)1;
		SCI.SCSCR.BIT.TE   = (unsigned char)1;
		SCI.SCSCR.BIT.RE   = (unsigned char)1;
		p = (int *)&SCI;
		INTC.IPRB.BIT._SCI = SCU_IREQ_PRI;
	} else {
		SCIF.SCSCR.BYTE = (unsigned char)0;
		SCIF.SCSCR.BIT.CKE = cke[ch];
		SCIF.SCBRR = baudrate[ch];
		SCIF.SCSMR.BIT.CHR = (unsigned char)(char_len ^ 0x01);
		if(pari_sel){
			SCIF.SCSMR.BIT._PE = (unsigned char)1;
			SCIF.SCSMR.BIT.OE  = (unsigned char)(pari_sel & 0x01);
		} else {
			SCIF.SCSMR.BIT._PE = (unsigned char)0;
		}
		SCIF.SCSMR.BIT.STOP = (unsigned char)stp_len;
		SCIF.SCSSR.BIT.RDF   = (unsigned char)0;
		SCIF.SCSCR.BIT.RIE  = (unsigned char)1;
		SCIF.SCSCR.BIT.TE   = (unsigned char)1;
		SCIF.SCSCR.BIT.RE   = (unsigned char)1;
		p = (int *)&SCIF;
		INTX.IPRE.BIT._SCIF = SCU_IREQ_PRI;
	}
	enable_interrupt();
	return(p);
}

int
scu_io_scm(
	int	ch,
	int	command
)
{
	if(ch == 0){
		if(command & 1){
			SCI.SCSCR.BIT.TE = (unsigned char)1;
		} else {
			SCI.SCSCR.BIT.TE = (unsigned char)0;
		}
		if(command & 2){
			SCI.SCSCR.BIT.RE = (unsigned char)1;
		} else {
			SCI.SCSCR.BIT.RE = (unsigned char)0;
		}
	} else {
		if(command & 1){
			SCIF.SCSCR.BIT.TE = (unsigned char)1;
		} else {
			SCIF.SCSCR.BIT.TE = (unsigned char)0;
		}
		if(command & 2){
			SCIF.SCSCR.BIT.RE = (unsigned char)1;
		} else {
			SCIF.SCSCR.BIT.RE = (unsigned char)0;
		}
	}
	return(0);
}

int
scu_io_sst(
	int	ch
)
{
	int	data;

	if(ch == 0){
		data = SCI.SCSSR.BYTE;
	} else {
		data = SCIF.SCSSR.WORD;
	}
	return(data);
}

int
scu_io_putc(
	int	ch,
	int	data
)
{
	if(ch == 0){
		SCI.SCSCR.BYTE |= 0x70;
		SCI.SCSCR.BIT.TIE = (unsigned char)0;
		if(SCI.SCSSR.BIT.TDRE){
			SCI.SCTDR = (unsigned char)data;
			SCI.SCSSR.BIT.TDRE = (unsigned char)0;
		}
		SCI.SCSCR.BIT.TIE = (unsigned char)1;
	} else {
		SCIF.SCSCR.BYTE |= 0x70;
		SCIF.SCFTDR = (unsigned char)data;
		SCIF.SCSSR.BIT.TDFE = (unsigned short)0;
		SCIF.SCSCR.BIT.TIE = (unsigned short)1;
	}
	return(0);
}

int
scu_io_baud(
	int	ch,
	int	baud
)
{
#ifdef P5561
	switch(baud){
		case C1200BPS:
			cke[ch] = (unsigned char)1;
			baudrate[ch] = (unsigned char)127;
			break;
		case C2400BPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)255;
			break;
		case C4800BPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)127;
			break;
		case C9600BPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)63;
			break;
		case C19KBPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)31;
			break;
		case C38KBPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)15;
			break;
		case C57KBPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)10;
			break;
		case C115KBPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)4;
			break;
		default:
			break;
	}
#else
	switch(baud){
		case C1200BPS:
			cke[ch] = (unsigned char)1;
			baudrate[ch] = (unsigned char)95;
			break;
		case C2400BPS:
			cke[ch] = (unsigned char)1;
			baudrate[ch] = (unsigned char)191;
			break;
		case C4800BPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)95;
			break;
		case C9600BPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)47;
			break;
		case C19KBPS:
			cke[ch] = (unsigned char)0;
			baudrate[ch] = (unsigned char)23;
			break;
		default:
			break;
	}
#endif
	return(0);
}
