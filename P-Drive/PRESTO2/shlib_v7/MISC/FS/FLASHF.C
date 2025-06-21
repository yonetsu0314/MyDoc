/*----------------------------------------------------------------------*
 * File name	: flash.c												*
 * Original	: p:/dacs_i/mcu3/bios2/src/rcs/flash.c 1.6
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/fs/rcs/flashf.c 1.4 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: flashf.c $
 * リビジョン 1.4  1970/01/01  00:00:00  sakaguti
 * drive\flash.hを参照。get_polling()使用
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * ﾜｰﾆﾝｸﾞ取り
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ファイル・システムとFLASH書込み
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/

/****************************************************************************/
/*FILE  : FLASH.C                                                           */
/****************************************************************************/
/*                                                                          */
#include <kernel.h>
#include <drive\flash.h>

//extern UW flhiadr;

W    Flash(H *, H *, int);
W    FlashX(H *, H *, int, int);
W    chip_erase(void);
W    chip_erasex(H *, int);

void	rsstr(char *str);
//void	rsout(char c);
//void	incBAR(int);


/****************************************************************************/
/*SPEC  :                                                                   */
/*NAME  : _Flash_Test_Main                       　　　　　　               */
/*FUNC  : _FLASH_Test_Main                                                  */
/*INPUT : None                                   　  　 　　    　　　　    */
/*OUTPUT: W rtcd : return code E_OKEY : normal sinish      　　   　　　　    */
/*      :                      E_NG : abnormal finish           　　　　    */
/*DATE  : 1998/06/01 new                                                    */
/*END   :                                                                   */
/****************************************************************************/
/*                                                                          */
W	Flash(H *flashadr, H *ramadr, int size)
{
	W	rtcd;
	W	i, j;
	B	flag;

	set_mode(RESET_CMD);

	rsstr("Flash chip erase: ");
	if((rtcd = chip_erasex(flashadr, size)) != E_OKEY){
		if(rtcd == E_TIMOUT){
			rsstr("erase time-out\n");
		}
		else{
			rsstr("erase error\n");
		}
		return(rtcd);
	}
	rsstr("complete\n");

	set_fladr((UW)flashadr);
	rsstr("Program :");
	size += (size % 2);
	flag = (B)0;
	rsstr("-\x8");
	for(i = (size / 2), j = 0; i > 0; i--){
		if(E_OKEY != (rtcd = flash_write(flashadr++, *(ramadr++)))){
			rsstr("error\n");
			break;
		}
    	if(j++ > 0x1000){
			switch(flag){
				case 0:
					rsstr("/\x8");
					flag = (B)1;
					break;
				case 1:
					rsstr("|\x8");
					flag = (B)2;
					break;
				case 2:
					rsstr("X\x8");
					flag = (B)3;
					break;
				case 3:
					rsstr("-\x8");
					flag = (B)0;
					break;
			}
			j = 0;
			dly_tsk((RELTIM)1);
		}
	}
	if(rtcd == E_OKEY){
		rsstr("complete\n");
	}
    set_mode(RESET_CMD);
    return(rtcd);                               /* return                   */
}

W	FlashX(H *flashadr, H *ramadr, int size, int barcnt)
{
	W	rtcd;
	W	i, j, k;
	B	flag;

	set_mode(RESET_CMD);

	rsstr("Flash chip erase: ");
	if((rtcd = chip_erasex(flashadr, size)) != E_OKEY){
		if(rtcd == E_TIMOUT){
			rsstr("erase time-out\n");
		}
		else{
			rsstr("erase error\n");
		}
		return(rtcd);
	}
	rsstr("complete\n");

	set_fladr((UW)flashadr);
	rsstr("Program :");
	size += (size % 2);
	flag = (B)0;
	rsstr("-\x8");
	for(i = (size / 2), j = 0, k = 0; i > 0; i--){
		if(E_OKEY != (rtcd = flash_write(flashadr++, *(ramadr++)))){
			rsstr("error\n");
			break;
		}
		if(barcnt!=0){
			++k;
			if(k > barcnt){
//				incBAR(1);
				k = 0;
			}
		}
		if(j++ > 0x1000){
			if(barcnt==0){
				switch(flag){
					case 0:
						rsstr("/\x8");
						flag = (B)1;
						break;
					case 1:
						rsstr("|\x8");
						flag = (B)2;
						break;
					case 2:
						rsstr("X\x8");
						flag = (B)3;
						break;
					case 3:
						rsstr("-\x8");
						flag = (B)0;
						break;
				}
			}
			j = 0;
			dly_tsk((RELTIM)2);
		}
	}
	if(rtcd == E_OKEY){
		rsstr("complete\n");
	}
    set_mode(RESET_CMD);
    return(rtcd);                               /* return                   */
}


/****************************************************************************/
/*SPEC  :                                                                   */
/*NAME  : chip_erase                             　　　　　　　             */
/*FUNC  : erace FLASH data                                                  */
/*INPUT : None                                                              */
/*OUTPUT: W     rtcd : return code  E_OKEY    : normal finish    　           */
/*      :                           E_TIMOUT : TMOUT/device abnormal       　*/
/*DATE  : 1998/06/16 new                                                    */
/*END   :                                                                   */
/****************************************************************************/
/*                                                                          */
W chip_erasex(H *flashadr, int size){
	B		flag;
	unsigned int	sect,sectend;
	long	rtcd;

	sect = (unsigned int)flashadr;
	sectend = sect + (unsigned int)size;
	sect &= 0xffff0000;

	rsstr("\nSector erase: ");
	flag = (B)0;
	rsstr("-\x8");
	dly_tsk((RELTIM)1);
	for(;sect < sectend; sect+=0x10000){
		set_fladr((UW)sect);
		if((rtcd = sect_erase((unsigned long)sect)) != 0){
			if(rtcd == 2){
				rsstr("erase time-out\n");
				return(rtcd);
			}
			else{
				rsstr("erase error\n");
				return(rtcd);
			}
		}
		switch(flag){
			case 0:
				rsstr("/\x8");
				flag = (B)1;
				break;
			case 1:
				rsstr("|\x8");
				flag = (B)2;
				break;
			case 2:
				rsstr("X\x8");
				flag = (B)3;
				break;
			case 3:
				rsstr("-\x8");
				flag = (B)0;
				break;
		}
		dly_tsk((RELTIM)1);
	}
	rsstr("complete\n");
	return(rtcd);
}


W chip_erase(void)
{
    W   rtcd;
    UH  rd;
	W	i;
	B	flag;

    set_mode(ERA1_CMD);
    set_mode(ERA2_CMD);

	i = 0;
	flag = (B)0;
	rsstr("-\x8");
    while(1) {
    	if(i++ > 300){
			switch(flag){
				case 0:
					rsstr("/\x8");
					flag = (B)1;
					break;
				case 1:
					rsstr("|\x8");
					flag = (B)2;
					break;
				case 2:
					rsstr("X\x8");
					flag = (B)3;
					break;
				case 3:
					rsstr("-\x8");
					flag = (B)0;
					break;
			}
			i = 0;
		}
		dly_tsk((RELTIM)1);
        rd = get_polling();                    /* polling data read    */
        if((rd & BIT_D7) == BIT_D7) {                /* erace finish         */
            rtcd = E_OKEY;
            break;
        }

        if((rd & BIT_D5) == BIT_D5) {               /* TMOUT                */
            rd = get_polling();               /* polling data read    */
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


