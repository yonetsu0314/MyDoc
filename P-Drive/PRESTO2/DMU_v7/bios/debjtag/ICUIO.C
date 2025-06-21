/*----------------------------------------------------------------------*
 * File name	: icuio.c	/ icu io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/icuio.c 1.3 1970/01/01 00:00:00 kagatume Exp $
 *$Log: icuio.c $
 * リビジョン 1.3  1970/01/01  00:00:00  kagatume
 * 2003/11/14 17:00
 * コンパイラのバージョンアップ(Ver.7)に伴って、ワーニングをなくす
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * ソフト割込み中は割込み禁止にする
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＤＭＵ用ＢＩＯＳ
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include "dmu5562.h"
#include	<biosif\icu.h>			/*	"		*/

extern	char	sakastr[100];

void icu_io(void);
int dis_icu(int dev_no,int pattern);
int ena_icu(int dev_no,int pattern);
int get_ipfw(short dev_no);
unsigned int icu_dev_hnt(V_FUNC,struct vects *);

int getipradr(int dev_no);
unsigned int getiprpat(int dev_no);

#pragma	inline_asm(icu_io)
static void icu_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_icu,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          icu_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
icu_exit:
          MOV.L       Lrsxxx,R0   ;icu_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _icu_io_err

          .ALIGN 4
jt_icu:
          .DATA.L     _dis_icu		; r0 = 0
          .DATA.L     _ena_icu		;    = 1
          .DATA.L     _get_ipfw		;    = 2
          .DATA.L     _icu_dev_hnt	;    = 3
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
          .DATA.L     0             ; none
}


int icu_io_err(){
	rsstr("*** icu_io err ***");
	return(-1);
}

int ena_icu(int dev_no,int pattern){
//	rsstr("*** ena_icu ***");
	switch(dev_no){
		case 0:
			sfr_setw(getipradr(dev_no),(UH)pattern);
			break;
		default:
			break;
	}
	return(0);
}

int dis_icu(int dev_no,int pattern){
//	rsstr("*** dis_icu ***");
	switch(dev_no){
		case 0:
			sfr_clrw(getipradr(dev_no),pattern);
			break;
		default:
			break;
	}
	return(0);
}

unsigned int icu_dev_hnt(V_FUNC funcp,struct vects *vectp){
	unsigned int	i,pat;

	for(i=vectp->no;i;--i){
		if((int)funcp == (int)(vectp->fun)[i]) break;
	}
	pat=getiprpat((int)(i + RL1_P));
	return((pat<<16)|(i + RL1_P));		// device=0 vector=i pattern=pat
}

int get_ipfw(short dev_no){
	int		adr;

	adr=getipradr((int)dev_no);
	return(adr);
}

int
getipradr(int dev_no){
	int	adr;

	dev_no &= 0x00ff;		//for only device No.0
	switch(dev_no){
		case 64: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 65: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 66: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 67: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 68: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 69: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 70: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 71: adr = (int)(IO_BASE + INTC_IPRC); break;
		case 72: adr = (int)(IO_BASE + INTC_IPRA); break;
		case 73: adr = (int)(IO_BASE + INTC_IPRA); break;
		case 74: adr = (int)(IO_BASE + INTC_IPRA); break;
		case 75: adr = (int)(IO_BASE + INTC_IPRA); break;
		case 76: adr = (int)(IO_BASE + INTC_IPRB); break;
		case 77: adr = 0xffffffff; break;
		case 78: adr = (int)(IO_BASE + INTC_IPRB); break;
		case 79: adr = (int)(IO_BASE + INTC_IPRB); break;
		case 80: adr = (int)(IO_BASE + INTC_IPRB); break;
		case 81: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 82: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 83: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 84: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 85: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 86: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 87: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 88: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 89: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 90: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 91: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 92: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 93: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 94: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 95: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 96: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 97: adr = (int)(IO_BASE + INTC_IPRD); break;
		case 98: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 99: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 100: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 101: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 102: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 103: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 104: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 105: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 106: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 107: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 108: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 109: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 110: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 111: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 112: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 113: adr = (int)(IO_BASE + INTC_IPRE); break;
		case 114: adr = 0xffffffff; break;
		case 115: adr = 0xffffffff; break;
		case 116: adr = 0xffffffff; break;
		case 117: adr = 0xffffffff; break;
		case 118: adr = 0xffffffff; break;
		case 119: adr = 0xffffffff; break;
		case 120: adr = 0xffffffff; break;
		case 121: adr = 0xffffffff; break;
		case 122: adr = 0xffffffff; break;
		case 123: adr = 0xffffffff; break;
		case 124: adr = 0xffffffff; break;
		case 125: adr = 0xffffffff; break;
		case 126: adr = 0xffffffff; break;
		case 127: adr = 0xffffffff; break;
		default: adr = 0xffffffff; break;
	}
	return(adr);
}


unsigned int
getiprpat(int dev_no){
	unsigned int	pat;

	dev_no &= 0x00ff;		//for only device No.0
	pat = 0;
	switch(dev_no){
		case 64: pat = 0xf000; break;
		case 65: pat = 0x0f00; break;
		case 66: pat = 0x00f0; break;
		case 67: pat = 0x000f; break;
		case 68: pat = 0xf000; break;
		case 69: pat = 0x0f00; break;
		case 70: pat = 0x00f0; break;
		case 71: pat = 0x000f; break;
		case 72: pat = 0x0f00; break;
		case 73: pat = 0x0f00; break;
		case 74: pat = 0x00f0; break;
		case 75: pat = 0x00f0; break;
		case 76: pat = 0xf000; break;
		case 77: pat = 0x0000; break;
		case 78: pat = 0x0f00; break;
		case 79: pat = 0x0f00; break;
		case 80: pat = 0x0f00; break;
		case 81: pat = 0xf000; break;
		case 82: pat = 0xf000; break;
		case 83: pat = 0xf000; break;
		case 84: pat = 0xf000; break;
		case 85: pat = 0xf000; break;
		case 86: pat = 0x0f00; break;
		case 87: pat = 0x0f00; break;
		case 88: pat = 0x0f00; break;
		case 89: pat = 0x0f00; break;
		case 90: pat = 0x00f0; break;
		case 91: pat = 0x00f0; break;
		case 92: pat = 0x00f0; break;
		case 93: pat = 0x00f0; break;
		case 94: pat = 0x000f; break;
		case 95: pat = 0x000f; break;
		case 96: pat = 0x000f; break;
		case 97: pat = 0x000f; break;
		case 98: pat = 0xf000; break;
		case 99: pat = 0xf000; break;
		case 100: pat = 0xf000; break;
		case 101: pat = 0xf000; break;
		case 102: pat = 0x0f00; break;
		case 103: pat = 0x0f00; break;
		case 104: pat = 0x0f00; break;
		case 105: pat = 0x0f00; break;
		case 106: pat = 0x00f0; break;
		case 107: pat = 0x00f0; break;
		case 108: pat = 0x00f0; break;
		case 109: pat = 0x00f0; break;
		case 110: pat = 0x000f; break;
		case 111: pat = 0x000f; break;
		case 112: pat = 0x000f; break;
		case 113: pat = 0x000f; break;
		case 114: pat = 0x0000; break;
		case 115: pat = 0x0000; break;
		case 116: pat = 0x0000; break;
		case 117: pat = 0x0000; break;
		case 118: pat = 0x0000; break;
		case 119: pat = 0x0000; break;
		case 120: pat = 0x0000; break;
		case 121: pat = 0x0000; break;
		case 122: pat = 0x0000; break;
		case 123: pat = 0x0000; break;
		case 124: pat = 0x0000; break;
		case 125: pat = 0x0000; break;
		case 126: pat = 0x0000; break;
		case 127: pat = 0x0000; break;
		default:  pat = 0x0000; break;
	}
	return(pat);
}

