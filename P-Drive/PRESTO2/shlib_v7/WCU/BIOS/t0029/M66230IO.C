/*----------------------------------------------------------------------*
 * File name	: m66230io.c	/ m66230 scu io svc handler				*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/m66230io.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: m66230io.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto2/wcu_v7/bios/t0016/rcs/m66230io.c 1.3 1970/01/01
 *Log: m66230io.c
 * リビジョン 1.3  1970/01/01  00:00:00  chimura
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.							*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN				*
 *		PHONE (0775) 53-4141											*
 *----------------------------------------------------------------------*/
#include <drive\serial.h>
#include <intrpt.h>

#define	M66230_ADDR_BASE	(unsigned long)0x0b0010000  /* M66230 address   */
#define	OFFSET_DATA			(0)
#define	OFFSET_CMD			(1)

#define	M66230_DEV_MAX		(2)

#define	STP_1BIT			(0x00)
#define	STP_2BIT			(0x20)
#define	PARI_EVE			(0x18)
#define	PARI_ODD			(0x08)
#define	PARI_NON			(0x00)
#define	BAUD_SET			(0x02)	

#define	CMD1				(0x00)
#define	CMD3				(0x40)
#define	CMD4				(0x80)
#define	CMD6				(0xc0)

#define	TX_DIS				(0x00)
#define	TX_ENA				(0x01)
#define	RX_ENA				(0x02)
#define	ERR_CLR				(0x10)

void	m66230_io(void);
int		*m66230_io_init(int, int, int,int);
int		m66230_io_gst(int);
int		m66230_io_sst(int,int);
int		m66230_io_putc(int,int);
int		m66230_io_getc(int);
void	rsstr(char *);

#pragma	inline_asm(m66230_io)
static void m66230_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_m66230,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          m66230_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
m66230_exit:
          MOV.L       Lrsxxx,R0   ;m66230_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _m66230_io_err

          .ALIGN 4
jt_m66230:
          .DATA.L     _m66230_io_init  ; r0 = 0
          .DATA.L     _m66230_io_gst   ;    = 1
          .DATA.L     _m66230_io_sst   ;    = 2
          .DATA.L     _m66230_io_putc  ;    = 3
          .DATA.L     _m66230_io_getc  ;    = 4
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
          .DATA.L     0                ; none
}

int
m66230_io_err(void)
{
	return(0);
}

int *
m66230_io_init(
	int	ch,
	int	stp_len,			/* ｽﾄｯﾌﾟ･ﾋﾞｯﾄ数		*/
	int	pari_sel,			/* ﾊﾟﾘﾃｨｰ選択		*/
	int	baud				/* ﾎﾞｰﾚｲﾄ			*/
)
{
	unsigned char	*p = (unsigned char *)(M66230_ADDR_BASE + OFFSET_CMD);
	unsigned char	tmp = BAUD_SET;
	int				*rp = (int *)M66230_ADDR_BASE;
	ch = ch;

	disable_interrupt();

	*p = 0xa0;		/*初期設定*/

	if(stp_len == C1STPBIT){
		tmp |= STP_1BIT;
	} else {
		tmp |= STP_2BIT;
	}
	switch(pari_sel){
		case NO_PARI:
			tmp |= PARI_NON;
			break;
		case ODD_PARI:
			tmp |= PARI_ODD;
			break;
		case EVEN_PARI:
			tmp |= PARI_EVE;
			break;
		default:
			tmp |= PARI_NON;
			break;
	}
	*p = tmp;
	switch(baud){
		case C1200BPS:
			tmp = 255;
			break;
		case C2400BPS:
			tmp = 127;
			break;
		case C4800BPS:
			tmp = 63;
			break;
		case C9600BPS:
			tmp = 31;
			break;
		case C19KBPS:
			tmp = 15;
			break;
		case C38KBPS:
			tmp = 7;
			break;
		case C57KBPS:
			tmp = 3;
			break;
		default:
			tmp = 15;
			break;
	}
	*p = tmp;
	tmp = CMD3;
	*p = tmp;
	tmp = (unsigned char)(CMD4|RX_ENA|0x08);
	*p = tmp;
	enable_interrupt();
	return(rp);
}

int
m66230_io_gst(
	int	ch
)
{
	int				st;
	unsigned char	p1,p2;

	ch = ch;
rsstr("m66230 io gst\n\r");

	p1 = *(unsigned char *)(M66230_ADDR_BASE + OFFSET_CMD);
	p2 = *(unsigned char *)(M66230_ADDR_BASE + OFFSET_CMD);
	if(p1 & 0x80){
		st = (p1<<8 | p2);
	} else {
		st = (p2<<8 | p1);
	}
	return(st);
}

int
m66230_io_sst(
	int	ch,
	int	st
)
{

	unsigned char	*p = (unsigned char *)(M66230_ADDR_BASE + OFFSET_CMD);
	unsigned char	tmp;

	ch = ch;


	switch(st){
		case ERR_CLR:
			tmp = 0x90;
			break;
		case TX_ENA:
			tmp = 0x8b;
			break;
		case TX_DIS:
			tmp = 0x8a;
			break;
		default:
			*p = 0x8b;
			return(0x8b);
	}
	*p = tmp;
	return((int)tmp);
}

int
m66230_io_putc(
	int	ch,
	int	data
)
{
	unsigned char	*p = (unsigned char *)(M66230_ADDR_BASE + OFFSET_DATA);

	ch = ch;

	*p = (unsigned char)data;
	return(0);
}

int
m66230_io_getc(
	int	ch
)
{
	unsigned char	*p = (unsigned char *)(M66230_ADDR_BASE + OFFSET_DATA);
	unsigned char	tmp;

	ch = ch;

	tmp = *p;
	return((int)tmp);
}
