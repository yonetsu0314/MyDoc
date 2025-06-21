/*----------------------------------------------------------------------*
 * File name	: fpgaio.c	/ fpga fifo io svc handler					*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/fpgaio.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: fpgaio.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/presto2/wcu_v7/bios/t0016/rcs/fpgaio.c 1.5 1970/01/01
 *Log: fpgaio.c
 * リビジョン 1.5  1970/01/01  00:00:00  sakaguti
 *----------------------------------------------------------------------*
 *		(C) Copyright 2004, ISHIDA CO., LTD.							*
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN						*
 *		PHONE (077) 553-4141											*
 *----------------------------------------------------------------------*/
#include	<string.h>		
#include	<stdlib.h>		
#include	<sh7706\7706.h>
//#include <biosif\fifo.h>

#define WCU_INT_MASK	((unsigned short *)0xA80000f4)
#define WCU_FIFO_MASK	((unsigned short *)0xA80000fa)

#define FPGA_PROG_ADDR_L		(0xA80000E0)
#define FPGA_PROG_ADDR_H		(0xA80000E2)
#define FPGA_VERSION_ADDR		(0xA80000E4)

void	fpga_io(void);

int				fifo_init(void);
unsigned char	fifo_input(void);
unsigned char	fifo_status(void);
void			fifo_output(unsigned char);
int				fifo_full(void);
void			fifo_msktxrx(unsigned short flag);
void			int_mask(int);
void			pint_mask(int);
int				make_FPGA_verstr(char *verstr);
void			*Bin_Asc16( long value, unsigned char *asc, size_t keta );
void			set_suffix(char *verstr, char *vertmp);


#pragma	inline_asm(fpga_io)
static void fpga_io(void){
          STS.L       PR,@-R15
;         MOV.L       #H'000000F0,R1
;         LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_fpga,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          fpga_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTS
          NOP
fpga_exit:
          MOV.L       Lrsxxx,R0   ;fpga_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTS                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _fpga_io_err

          .ALIGN 4
jt_fpga:
          .DATA.L     _fifo_input	; r0 = 0
          .DATA.L     _fifo_status	; r0 = 1
          .DATA.L     _fifo_output	; r0 = 2
          .DATA.L     _fifo_msktxrx	; r0 = 3
          .DATA.L     _fifo_init	; r0 = 4
          .DATA.L     _fifo_full	; r0 = 5
          .DATA.L     _int_mask     ; r0 = 6
          .DATA.L     _pint_mask    ; r0 = 7
          .DATA.L     _make_FPGA_verstr ; r0 = 8
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
          .DATA.L     0             ; none
}


int
fpga_io_err(void)
{
	return(-1);
}


void
int_mask(int mask){
	*WCU_INT_MASK = (unsigned short)(mask);		/* Disable Outernal Interrupt */
}


void
pint_mask(int mask){
}


int
fifo_init(){
	PFC.PECR.WORD = (unsigned short)0xaaaa; /* プルアップ付き入力に設定 */
	PD.DR.BYTE = (unsigned char)0x28;
	PH.DR.BYTE = (unsigned char)0x30;

//	PC.DR.BYTE = (unsigned char)0x00;
//	wait66n(10000);
	PC.DR.BYTE = (unsigned char)0x01;
//	wait66n(10000);
	fifo_input();	//ダミーリード
	return(0);
}


unsigned char
fifo_input(){
	unsigned char Data8;

	PD.DR.BYTE = (unsigned char)0x08;
	Data8 = PE.DR.BYTE;
	PD.DR.BYTE = (unsigned char)0x28;

	return(Data8);
}


unsigned char
fifo_status(){
	unsigned char Data8;

	PD.DR.BYTE = (unsigned char)0x20;
	Data8 = PE.DR.BYTE;
	PD.DR.BYTE = (unsigned char)0x28;

	return(Data8);
}


void
fifo_output( unsigned char Data8 ){
	PD.DR.BYTE = (unsigned char)0x00;
	PFC.PECR.WORD = (unsigned short)0x5555; /* 出力に設定 */
	PE.DR.BYTE = Data8;
	PD.DR.BYTE = (unsigned char)0x28;
	PFC.PECR.WORD = (unsigned short)0xaaaa; /* プルアップ付き入力に設定 */
}


void
fifo_msktxrx(unsigned short flag){
	*WCU_FIFO_MASK = flag;
}


int
fifo_full(){
	unsigned short Busy_Check;

	Busy_Check = PJ.DR.BYTE;  //Read Input Register
	Busy_Check >>= 1;
	Busy_Check &= (unsigned short)0x03;

	return((int)Busy_Check);
}


int make_FPGA_verstr(char *verstr)
{
	char vertmp[20], tmp[10], progtmp[10];
	
	verstr[0] = '\0';
	/*--------------------------------------------------------------*/
	/* プログラム名を文字列に埋め込む	*/
	/*--------------------------------------------------------------*/
	/*	FPGA_PROG_ADDR_L  : プログラム名の先頭英字
		FPGA_PROG_ADDR_L+1: プログラム名の数字６、５桁目
		FPGA_PROG_ADDR_H  : プログラム名の数字４、３桁目
		FPGA_PROG_ADDR_H+1: プログラム名の数字２、１桁目
		
	  	例： プログラム名 R6302 の場合
		FPGA_PROG_ADDR_Lから２アドレス分の値：  52 FF → 文字列Rに変換
		FPGA_PROG_ADDR_Rから２アドレス分の値：  63 02 → 文字列6302に変換
	*/
	
	/* プログラム名の先頭英字を得る */
	progtmp[0] = (char)(*((unsigned char *)FPGA_PROG_ADDR_L));
	
	/* プログラム名の数字上位２桁を得る */
	Bin_Asc16( (long)(*((unsigned short *)FPGA_PROG_ADDR_L)), (unsigned char *)(&tmp[0]), (size_t)4 );
	tmp[4] = '\0';
	/* プログラム名の数字部分の上位２桁を文字列に埋め込む */
	if (tmp[2] >= '1' && tmp[2] <= '9') {	/* プログラム名の数字部分が６桁の場合に相当 */
		progtmp[1] = tmp[2];
		progtmp[2] = tmp[3];
		progtmp[3] = '\0';
		strcat(verstr, progtmp);
	}
	else if (tmp[3] >= '1' && tmp[3] <= '9') {	/* プログラム名の数字部分が５桁の場合に相当 */
		progtmp[1] = tmp[3];
		progtmp[2] = '\0';
		strcat(verstr, progtmp);
	}
	else {	/* プログラム名の数字部分が４桁の場合に相当 */
		progtmp[1] = '\0';
		strcat(verstr, progtmp);
	}
	
	/* プログラム名の数字部分の下位４桁を文字列に埋め込む */
	Bin_Asc16( (long)(*((unsigned short *)FPGA_PROG_ADDR_H)), (unsigned char *)(&progtmp[0]), (size_t)4 );
	progtmp[4] = '\0';
	strcat(verstr,progtmp);

	/*--------------------------------------------------------------*/
	/* バージョン番号を文字列に埋め込む		*/
	/*--------------------------------------------------------------*/
	
	/* バージョン番号文字列を得る */
	Bin_Asc16( (long)(*((unsigned short *)FPGA_VERSION_ADDR)), (unsigned char *)(&tmp[0]), (size_t)4 );
	tmp[4] = '\0';
	/* バージョン文字列の生成、0を削除して左詰にする。(例：FPGA_VERSION_ADDRの値 01 02 → 1.2) */
	if(tmp[0] == '0') {
		vertmp[0] = tmp[1];
		vertmp[1] = '\0';
	}
	else {
		vertmp[0] = tmp[0];
		vertmp[1] = tmp[1];
		vertmp[2] = '\0';
	}

	set_suffix(verstr, vertmp);
	strcat(verstr," ");
	strcat(verstr,vertmp);
	strcat(verstr,".");
	
	if(tmp[2] == '0') {
		vertmp[0] = tmp[3];
		vertmp[1] = '\0';
	}
	else {
		vertmp[0] = tmp[2];
		vertmp[1] = tmp[3];
		vertmp[2] = '\0';
	}
	strcat(verstr,vertmp);
	return 0;
}


/****************************************************************/
/*			ｌｏｎｇ型をアスキー１６進コードに変換する			*/
/****************************************************************/
void *Bin_Asc16( long value, unsigned char *asc, size_t keta )
{
	unsigned char	i, hi, lo;
	unsigned char *	p = asc + keta;

	for( i=(unsigned char)0; i<8; i++, value >>= 8 ) {
		if( keta == 0 ) break;
		keta--;
		p--;
		lo = (unsigned char)((unsigned char)value & 15);
		*p = (unsigned char)( lo >= 10 ? ('A' + lo - 10) : ('0' + lo) );
		if( keta == 0 ) break;
		keta--;
		p--;
		hi = (unsigned char)((unsigned char)value >> 4);
		*p = (unsigned char)( hi >= 10 ? ('A' + hi - 10) : ('0' + hi) );
	}
	return asc;
}


#define MAX_LEN (19)
void
set_suffix(char *verstr, char *vertmp){
	char	c,suffix[2];
	int		i,k;
	
	if (strlen(vertmp) >= MAX_LEN) return;
	
	for(i=0,k=0;i<=MAX_LEN;++i){
		c = vertmp[i];
		if(c=='.') break;
		if(c==(char)0) break;
		k *= 10;
		k += c & 0x0f;
	}
	
	suffix[0] = (char)(0x40 + k - 1);
	suffix[1] = (char)0;
	if((k>=2)&&(k<=27)) strcat(verstr,suffix);
	suffix[0] = (char)'_';
	if(k>=28) strcat(verstr,suffix);
	if(k<0) strcat(verstr,suffix);
}


