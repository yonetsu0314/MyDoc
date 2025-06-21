/*======================================================================
 * File name    : dmafpga.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/wcu/bios/src/rcs/fpga.c 1.2 1970/01/01 00:00:00 chimura Exp $
 *$Log: fpga.c $
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2004/01/14 16:00
 * p:/presto/wcu/bios/rcs/fpga.c 1.5 と同じにする。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */
#include	<sh7706\7706.h>

#define	FR_BYTE_ADR	(char *)(0xa11c0000)
#define BOOT_SIZE	(0x0002C01C)

void Start_SelectMAP( void );
void SelectMAP( unsigned short *flash_start_addr, unsigned short *flash_end_addr );
void SelectMAP_output( unsigned short Data16 );
void Check_BUSY(unsigned short Data16);
void Check_DONE_Bit(void);

void rsstr(char *str);
void rsout(char c);


void
init_fpga(void){
	Start_SelectMAP();
}


void
Start_SelectMAP( void ){
	//Address Range varaibles
	unsigned short *flash_start_addr;
	unsigned short *flash_end_addr;

	PFC.PECR.WORD = (unsigned short)0x5555; /* 出力に設定 */

	//Location of configuration data stored in flash
	flash_start_addr = (unsigned short *)(FR_BYTE_ADR);
	flash_end_addr   = (unsigned short *)(FR_BYTE_ADR + BOOT_SIZE);

	SelectMAP(flash_start_addr, flash_end_addr);

	Check_DONE_Bit();

	PFC.PECR.WORD = (unsigned short)0xaaaa; /* プルアップ付き入力に設定 */
	PH.DR.BYTE = (unsigned char)0x30;
	PD.DR.BYTE = (unsigned char)0x28;

	wait66n(10000);
	PC.DR.BYTE = (unsigned char)0x00;
	wait66n(10000);
	PC.DR.BYTE = (unsigned char)0x01;
}


void
SelectMAP( unsigned short *flash_start_addr, unsigned short *flash_end_addr ){
	unsigned short Data16;
	unsigned short *flash_addr;

	//Bring CS, WRITE and PROGRAM_OE LOW:
	PH.DR.BYTE = (unsigned char)0x10;
	PD.DR.BYTE = (unsigned char)0x00;

	//Program_OE bit Low brings the Virtex Program Pin to High Z:
	PH.DR.BYTE = (unsigned char)0x30;

	Data16 = 0;  //Initialize Data16 variable before entering While Loop

	while(Data16 == 0){
		Data16 = PJ.DR.BYTE;  //Read Input Register
		Data16 = Data16 & 0x0002;    	  //Check Status of /INIT
	}

	for(flash_addr = flash_start_addr; flash_addr < flash_end_addr; flash_addr++){
		Data16 = *flash_addr; //Read Flash

		//Pass the 16-bit Word to SelectMAP_output() function:
		SelectMAP_output(Data16);
	}
}


void
SelectMAP_output( unsigned short Data16 ){
	//Write to the Configuration Register:
		//	1)  The value of MS Byte
		//	2)  CCLK = Low

	//Shift Left  (i.e.  '0xABCD << 8' will become 0x00AB)
	PE.DR.BYTE = Data16 >> 8;
	PD.DR.BYTE = 0x00;

	//Keep MSByte on Databus, but Bring CCLK High:
	PD.DR.BYTE = 0x02;

// 	Check_BUSY(Data16);  //Check BUSY pin for this Upper Byte

	PE.DR.BYTE = Data16;
	PD.DR.BYTE = 0x00;

	PD.DR.BYTE = 0x02;

// 	Check_BUSY(Data16);  //Check BUSY pin for this Lower Byte
 }


void
Check_BUSY(unsigned short Data16){
	unsigned short Busy_Check;

	do{
		Busy_Check = PJ.DR.BYTE;  //Read Input Register
		Busy_Check = Busy_Check >> 2;
		Busy_Check = Busy_Check & 0x01;
		if(Busy_Check==0x0001){    //If Busy is High,
			//Bring CCLK Low:
			PD.DR.BYTE = 0x00;
			//Bring CCLK HIgh:
			PD.DR.BYTE = 0x02;
		}
	}while(Busy_Check == 0x0001); //Keep doing this until Busy is low
}


void Check_DONE_Bit(void){
	unsigned short Data16;

	//Read CPLD Input Register:
	Data16 = PJ.DR.BYTE;

	//Check the DONE bit of the CPLD Input Register:
	Data16 = (Data16 & 0x0001) ? 1 : 0;

	if (Data16 == 1){					    //If DONE Pin is High, display message
		rsstr("FPGA:Successful! DONE is High!\n");   //"Successful! DONE is High!"
	}
	else{	//Otherwise, if DONE is low, device is not configured
		Data16 = PJ.DR.BYTE;
		//Check the INIT bit of the CPLD Input Register
		Data16 = (Data16 & 0x0002) ? 1 : 0;
		if(Data16 == 1) { 					  //If INIT is High,
			rsstr("FPGA:Configuration Failed, DONE is Low, Init is High\n");	//"Configuration Failed, DONE is Low, Init is High"
		}
		else{
			rsstr("FPGA:Configuration Failed, DONE is Low, Init is Low\n");	//"Configuration Failed, DONE is Low, Init is Low"
		}
	}
}


/* デバッグ用ＲＳ２３２Ｃ文字列送信 */
void
rsstr(char *str){
	char	c;
	int	i;

	i = 0;
	for(;;){
		c = str[i];
		if (c == 0) break;
		if (c == 0x0a){
			rsout((char)0x0d);
		}
		rsout(c);
		++i;
	}
}


void
rsout(char c){
	SCI.SCSCR.BIT.TIE = (unsigned char)0;
	for(;;){
		if(SCI.SCSSR.BIT.TDRE){
			SCI.SCTDR = (unsigned char)c;
			SCI.SCSSR.BIT.TDRE = (unsigned char)0;
			break;
		}
	}
}


