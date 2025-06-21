/*----------------------------------------------------------------------*
 * File name	: comio.c	/ arcnet io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/comio.c 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: comio.c $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * ソフト割込み中は割込み禁止にする
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * outp2でWRパルスが３個でるバグを修正
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ＡＲＣのアクセス全てに200nSを追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <biosif\com.h>

#define	ARCNET0_ADDR_BASE	(unsigned long)0x24000000  /* ARCNET0 address	*/
#define	ARCNET1_ADDR_BASE	(unsigned long)0x24200000  /* ARCNET1 address	*/
#define	COM_MAX			(2)
#define	N200NS			3

void  com_io(void);
void *com_io_init(int);
int   com_io_outp(struct com_ctxt *, enum comreg, unsigned char);
int   com_io_outp2(struct com_ctxt *, enum comreg, unsigned short);
int   com_io_inp(struct com_ctxt *, enum comreg);
long  com_io_inp2(struct com_ctxt *, enum comreg);
int   com_io_outpb(struct com_ctxt *, enum comreg, unsigned char, unsigned char);

#pragma	inline_asm(com_io)
static void com_io(void){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_com,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          com_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
com_exit:
          MOV.L       Lrsxxx,R0   ;com_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _com_io_err

          .ALIGN 4
jt_com:
          .DATA.L     _com_io_init  ; r0 = 0
          .DATA.L     _com_io_outp  ; r0 = 1
          .DATA.L     _com_io_outp2 ; r0 = 2
          .DATA.L     _com_io_inp   ; r0 = 3
          .DATA.L     _com_io_inp2  ; r0 = 4
          .DATA.L     _com_io_outpb ; r0 = 5
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
com_io_err(void)
{
	return(0);
}


void
wait88n(int m){				/* wait 88nS * m at Compile option = 1 */
	int	iii;

	for(iii=0;iii<m;++iii) ;
}

static void
wait86(void)
{
//	wait0(1);
//	int	i = 205*4;
//	for(; i; i--);
	wait88n(971);
}

static	struct compinf	comphy[] ={
					ARCNET0_ADDR_BASE,
					ARCNET1_ADDR_BASE,
					0
				};
void *
com_io_init(
	int		devno
	)
{
	int	no;

	if(devno > COM_MAX){
		no = COM_MAX;
	} else {
		no = devno;
	}
	return((void *)&comphy[no]);
}

int
com_io_outp(
	struct com_ctxt *contxt,
	enum comreg	reg,
	unsigned char	data
)
{
	unsigned char	*p;
	int		err = 0;

	p = (unsigned char *)(contxt->iobase + reg);
	*p = data;
	wait88n(N200NS);

	switch(reg){
		case COMMR:
			wait86();
		case INTMR:
		case ADRHR:
		case ADRLR:
		case DATR:
		case RESERVED:
		case CONFGR:
			contxt->reg[reg] = data;
			break;
		case R7:
			break;
		default:
			break;
	}
	return(err);
}

int
com_io_outp2(
	struct com_ctxt *contxt,
	enum comreg	reg,
	unsigned short	data
)
{
	unsigned char	*p;
	int		err = 0;
	int		tmp = reg;

	p = (unsigned char *)(contxt->iobase + reg);
	*p = (unsigned char)(data >> 8);
	wait88n(N200NS);

	switch(reg){
		case COMMR:
			wait86();
		case INTMR:
		case ADRHR:
		case ADRLR:
		case DATR:
		case RESERVED:
		case CONFGR:
			contxt->reg[reg] = (unsigned char)(data >>8);
			break;		//saka
		case R7:
			p++;
			*p = (unsigned char)(data & 0x00ff);
			break;
		default:
			break;
	}

	p = (unsigned char *)(contxt->iobase + reg);
	*(++p) = (unsigned char)(data & 0x00ff);
	wait88n(N200NS);
	tmp++;
	switch(tmp){
		case COMMR:
			wait86();
		case INTMR:
		case ADRHR:
		case ADRLR:
		case DATR:
		case RESERVED:
		case CONFGR:
			contxt->reg[tmp] = (unsigned char)(data & 0x00ff);
			break;		//saka
		case R7:
			break;
		default:
			break;
	}

	return(err);
}

int
com_io_inp(
	struct com_ctxt *contxt,
	enum comreg	reg
)
{
	unsigned char	data;
	
	data = *(unsigned char *)(contxt->iobase + reg);
	wait88n(N200NS);

	return((int)data);
}

long
com_io_inp2(
	struct com_ctxt *contxt,
	enum comreg	reg
)
{
	unsigned char	data1,data2;

	data1 = *(unsigned char *)(contxt->iobase + reg);
	wait88n(N200NS);
	data2 = *(unsigned char *)(contxt->iobase + reg + 1);
	wait88n(N200NS);

	return((long)(data1<< 8 | data2));
}

int
com_io_outpb(
	struct com_ctxt *contxt,
	enum comreg	reg,
	unsigned char	data1,
	unsigned char	data2
)
{
	unsigned char	data;
	unsigned char	*p;

	p = (unsigned char *)(contxt->iobase + reg);

	data = (unsigned char)(~data1 & contxt->reg[reg]);
	data |= data2;
	*p = data;
	wait88n(N200NS);
	contxt->reg[reg] = data;
	return(0);
}
