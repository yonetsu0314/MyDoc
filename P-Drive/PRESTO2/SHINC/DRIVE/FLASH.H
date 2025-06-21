/*----------------------------------------------------------------------*
 * File name	: flash.h												*
 * Original	: p:/presto/shlib_v7/misc/fs/rcs/flash.h 1.2
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/drive/rcs/flash.h 1.3 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: flash.h $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * NORTi用typedef H,W,UWを使わない表現に変更
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * FLASHドライバのﾊﾟﾗﾒｰﾀをSH7615から移動、整理
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

/************************************************************************/
/*FILE  : FLASH.H                                                       */
/************************************************************************/

#define     READ_CMD        (UH)0x0F000  /* read command                 */
#define     RESET_CMD       (UH)0x0F000  /* reset command                */
#define     CODE_CMD        (UH)0x09000  /* signature code read command  */
#define     PROG_CMD        (UH)0x0A000  /* program command              */
#define     ERA1_CMD        (UH)0x08000  /* chip erace command(1st)      */
#define     ERA2_CMD        (UH)0x01000  /* chip erace command(2nd)      */
#define     ERA_SECT        (UH)0x03000  /* sector erace command      */

#define	BIT_D7		(UH)0x08000	/* BIT 7 Mask			*/
#define	BIT_D5		(UH)0x02000	/* BIT 5 Mask			*/

/* return code */
#define     E_OKEY          0
#define     E_NG            1
#define     E_TIMOUT        2                       /* TMOUT                        */

//typedef     char            B;
//typedef     short           H;
//typedef     long            W;
typedef     float           F;
//typedef     unsigned char   UB;
//typedef     unsigned short  UH;
//typedef     unsigned long   UW;
#define     VP              void *
#define     BP              char *

/* prototype declaration */
long    Flash(short * , short *, int);
long    FlashX(short *, short *, int, int);
long    chip_erase(void);
long    chip_erasex(short *, int);

long    flash_write(short *adr,short data);
long    sect_erase(unsigned long sect);
void set_mode(unsigned short cmd);
void set_fladr(unsigned long flashadr);
unsigned short   get_polling(void);
int  flash_sys_init(unsigned short	siz_buf);

