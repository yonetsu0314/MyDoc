/*----------------------------------------------------------------------*
 * File name	: fromio.c				/ flash rom io svc handler			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/dmu_v7/bios/src/rcs/fromio.c 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: fromio.c $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * friohﾎﾟｲﾝﾀの誤ったｷｬｽﾄを削除
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * 05.03.18 8KB境界でｾｸﾀ･ｲﾚｰｽする
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * from_ioのコマンド５種追加
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------*
 *		(C) Copyright 1994, ISHIDA CO., LTD.			*
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN SHIGA JAPAN	*
 *		PHONE (0775) 53-4141					*
 *----------------------------------------------------------------------*/
#include <kernel.h>
#include <sh7615\sh7615.h>
#include <biosif\from.h>
#include <drive\flash.h>
#include "dmu5562.h"

#define     FROM_ADDR_BASE  (UW)0x201f8000  /* FLASH device standard address   */

#define     ADR_555         (UW)0x20000AAA  /* write address(0x0555)        */
#define     ADR_2AA         (UW)0x20000554  /* write address(0x02AA)        */

#define     DAT_AA          (UH)0x0AA00  /* write data(0x0AA00)            */
#define     DAT_55          (UH)0x05500  /* erite data(0x05500)            */
#define     READ_CMD        (UH)0x0F000  /* read command                 */
#define     RESET_CMD       (UH)0x0F000  /* reset command                */
#define     CODE_CMD        (UH)0x09000  /* signature code read command  */
#define     PROG_CMD        (UH)0x0A000  /* program command              */
#define     ERA1_CMD        (UH)0x08000  /* chip erace command(1st)      */
#define     ERA2_CMD        (UH)0x01000  /* chip erace command(2nd)      */
#define     ERA_SECT        (UH)0x03000  /* sector erace command      */

#define     DWNLD_PROG_BASE (UW)0x20400000  /* FLASH device standard address   */
#define     FLASH_ADDR_BASE (UW)0x20200000  /* FLASH device standard address   */

#define     BIT_D7          (UH)0x08000  /* BIT 7 Mask                   */
#define     BIT_D5          (UH)0x02000  /* BIT 5 Mask                   */

/* macro declaration */
#define     _SET_MEM8(adr,dat)      *(volatile UB *)adr=(UB)dat  /* memory write(8Bit)   */
#define     _SET_MEM16(adr,dat)     *(volatile UH *)adr=(UH)dat  /* memory write(16Bit)  */
#define     _SET_MEM32(adr,dat)     *(volatile UW *)adr=(UW)dat  /* memory write(32Bit)  */
#define     _GET_MEM8(adr)          *(volatile UB *)adr          /* memory read(8Bit)  */
#define     _GET_MEM16(adr)         *(volatile UH *)adr          /* memory read(16Bit) */
#define     _GET_MEM32(adr)         *(volatile UW *)adr          /* memory read(32Bit) */

UW flhiadr;

void from_io(void);

#pragma	inline_asm(from_io)
static void from_io(){
          STS.L       PR,@-R15
          MOV.L       #H'000000F0,R1
          LDC         R1,SR
          AND         #H'0000000F,R0
          SHLL2       R0
          MOV.L       #jt_from,R2  ;arrangement Top Address
          ADD         R2,R0       ;Add offset_Value
          MOV.L       @R0,R3
          MOV.L       R3,R0
          CMP/EQ      #0,R0
          BT          from_exit
          JSR         @R0
          NOP
          LDS.L       @R15+,PR
          RTE
          NOP
from_exit:
          MOV.L       Lrsxxx,R0   ;from_io_err
          JSR         @R0
          NOP
          MOV.L       #-1,R0      ;システムコールの返り値は -1
          LDS.L       @R15+,PR
          RTE                     ;異常終了
          NOP
Lrsxxx:
          .DATA.L     _from_io_err

          .ALIGN 4
jt_from:
          .DATA.L     _flashrom_init		; r0 = 0
          .DATA.L     _flashrom_pgwr		; r0 = 1
          .DATA.L     _flash_write			; r0 = 2
          .DATA.L     _sect_erase			; r0 = 3
          .DATA.L     _set_mode				; r0 = 4
          .DATA.L     _set_fladr			; r0 = 5
          .DATA.L     _get_polling			; r0 = 6
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


int from_io_err(){
	rsstr("*** from_io err ***");
	return(0);
}

int flashrom_init(FRPHY  **fromphy, FRWRT  *intrvl){
//	rsstr("*** flashrom init ***");
	*fromphy = (FRPHY *)FROM_ADDR_BASE;
	*intrvl = (FRWRT)1;
	set_fladr(FROM_ADDR_BASE);
	set_mode(RESET_CMD);
	return(0);
}

typedef union uci {		//saka: High,Low反転に注意
	unsigned char	c[2];			/* charの配列データ	*/
	short		i;			/* intデータ		*/
} UCI;

int flashrom_pgwr(struct frioh  *drvinfop){
	union uci		shrt;
	unsigned char	*bp;
	unsigned int	gofs,pofs,eofs,bufmax;
	short	*flashadr;
	int		retry = 5;

//	rsstr("*** flashrom pgwr ***");
start:
	if((--retry)==0) goto err;
	eofs = *(drvinfop->filep);
	bp   = (unsigned char *)drvinfop->frbuf;
	gofs = *(drvinfop->getp);
	pofs = *(drvinfop->putp);
	bufmax = drvinfop->szbuf;

	if(gofs==pofs) return(1);

	set_mode(RESET_CMD);
	flashadr = (short *)(FROM_ADDR_BASE + eofs);

	if((eofs%2)!=0){
		eofs -= 1;	eofs &= (short)0x7fff;
		flashadr = (short *)(FROM_ADDR_BASE + eofs);
		shrt.i = *flashadr;
		shrt.c[1] = *(bp + (gofs++));	gofs &= bufmax;
		if(flash_write(flashadr, shrt.i) != E_OKEY){
			set_mode(RESET_CMD);
			if(sect_erase((UW)flashadr) != E_OKEY) goto err;
			if(flash_write(flashadr, shrt.i) != E_OKEY) goto err;
			goto start;
		}
		eofs += 2;	eofs &= (short)0x7fff;
		flashadr = (short *)(FROM_ADDR_BASE + eofs);
	}
	for(;;){
		shrt.i = (short)0xffff;
		shrt.c[0] = *(bp + (gofs++));	gofs &= bufmax;
		if(gofs == pofs) break;
		shrt.c[1] = *(bp + (gofs++));	gofs &= bufmax;
		if(gofs == pofs) break;
		if(flash_write(flashadr, shrt.i) != E_OKEY){
			set_mode(RESET_CMD);
			if(sect_erase((UW)flashadr) != E_OKEY) goto err;
			if(flash_write(flashadr, shrt.i) != E_OKEY) goto err;
			goto start;
		}
		eofs += 2;	eofs &= (short)0x7fff;
		flashadr = (short *)(FROM_ADDR_BASE + eofs);
//		dly_tsk((RELTIM)1);
	}
	if(flash_write(flashadr, shrt.i) != E_OKEY){
		set_mode(RESET_CMD);
		if(sect_erase((UW)flashadr) != E_OKEY) goto err;
		if(flash_write(flashadr, shrt.i) != E_OKEY) goto err;
		goto start;
	}
    set_mode(RESET_CMD);
	*(drvinfop->getp) = *(drvinfop->putp);
	return(1);			/* return semaphore number = 1 */
err:
    set_mode(RESET_CMD);
	*(drvinfop->getp) = *(drvinfop->putp);
	return(0);
}


/****************************************************************************/
/*SPEC  :                                                                   */
/*NAME  : flash_write                            　　　　　　　             */
/*FUNC  : write data to FLASH                                               */
/*      : MAX time to write = 5.2sec                                        */
/*INPUT : UW    *adr : write address                                        */
/*      : UW    data : write data                                           */
/*OUTPUT: W     rtcd : return code E_OKEY    : normal finish 　       　　　　*/
/*      :                       E_TIMOUT : TMOUT/device abnormal     　　　　*/
/*DATE  : 1998/06/16 new                                                    */
/*END   :                                                                   */
/****************************************************************************/
/*                                                                          */
W flash_write(H *adr,H data)
{
    W   rtcd;
    UH  rd;

    set_mode(PROG_CMD);                             /* PROGRAM MODE         */

    *adr = data;                                    /* data write           */
    while(1) {
        rd = _GET_MEM16(adr);                       /* polling data read    */
        if((rd & BIT_D7) == (data & BIT_D7)) {      /* D7 == DATA           */
            rtcd = E_OKEY;
            break;
        }

        if((rd & BIT_D5) == BIT_D5) {               /* D5 == 1  (TMOUT)     */
            rd = _GET_MEM16(adr);                   /* polling data read    */
            if((rd & BIT_D7) == (data & BIT_D7)) {  /* D7 == DATA           */
                rtcd = E_OKEY;
                break;
            } else {                                /* TMOUT                */
                rtcd = E_TIMOUT;                     /*                      */
                break;
            }
        }
    }

    return(rtcd);                                   /* return               */

}


W sect_erase(UW sect){
    W   rtcd;
    UH  rd;

    set_mode(ERA1_CMD);
    _SET_MEM16((ADR_555 | flhiadr),DAT_AA);
    _SET_MEM16((ADR_2AA | flhiadr),DAT_55);
    _SET_MEM16((sect & (UW)0x2fffe000),ERA_SECT);

    while(1) {
        rd = _GET_MEM16((ADR_555 | flhiadr));                    /* polling data read    */
        if((rd & BIT_D7) == BIT_D7) {                /* erace finish         */
            rtcd = E_OKEY;
            break;
        }
        if((rd & BIT_D5) == BIT_D5) {               /* TMOUT                */
            rd = _GET_MEM16((ADR_555 | flhiadr));               /* polling data read    */
            if((rd & BIT_D7) == BIT_D7) {           /* erace finish         */
                rtcd = E_OKEY;
                break;
            } else {                                /* TMOUT                */
                rtcd = E_TIMOUT;                     /*                      */
                break;
            }
        }
    }
    return(rtcd);                                   /* return               */
}

/****************************************************************************/
/*SPEC  :                                                                   */
/*NAME  : set_mode                               　　　　　　　             */
/*FUNC  : mode set                                                          */
/*INPUT : UW cmd : READ_CMD : read command                                  */
/*      :        : RESET_CMD: reset command                                 */
/*      :        : CODE_CMD : signature code read command                   */
/*      :        : PROG_CMD : program command                               */
/*      :        : ERA1_CMD : chip erace command(1st)                       */
/*      :        : ERA2_CMD : chip erace command(2nd)                       */
/*      :        : REN_CMD  : continuous mode command                       */
/*OUTPUT: None                                                              */
/*DATE  : 1998/06/16 new                                                    */
/*END   :                                                                   */
/****************************************************************************/
/*                                                                          */
void set_mode(UH cmd)
{
    _SET_MEM16((ADR_555 | flhiadr),DAT_AA);
    _SET_MEM16((ADR_2AA | flhiadr),DAT_55);
    _SET_MEM16((ADR_555 | flhiadr),cmd);
}


void set_fladr(UW flashadr)
{
	flhiadr = (flashadr & (UW)0x2fffe000);
}


UH get_polling()
{
	return((UH)_GET_MEM16((ADR_555 | flhiadr)));   /* polling data read    */
}


