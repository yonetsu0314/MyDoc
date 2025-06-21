/*----------------------------------------------------------------------*
 * File name	: flash.c												*
 * Original	: p:/dacs_i/mcu3/bios2/src/rcs/flash.c 1.6
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/dmu5562/RCS/flash.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: flash.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * FLASH用関数をBIOSに移行
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/11/11 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
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
#include <biosif\from.h>

//UW flhiadr;

/* function proto-type declaration */

W    flash_write(H *adr,H data);
W    sect_erase(UW sect);
void set_mode(UH cmd);
void set_fladr(UW flashadr);
UH   get_polling(void);

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
    return(_flash_write(adr, data));
}


W sect_erase(UW sect){
    return(_sect_erase(sect));
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
	_set_mode(cmd);
}


void set_fladr(UW flashadr)
{
	_set_fladr(flashadr);
}


UH   get_polling(void)
{
	return(_get_polling());
}


