/*----------------------------------------------------------------------*
 * File name	: flash.h												*
 * Original	: p:/dacs_i/mcu3/bios2/src/rcs/flash.h 1.6
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shlib_v7/misc/fs/RCS/flash.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: flash.h $
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

/************************************************************************/
/*FILE  : FLASH.H                                                       */
/************************************************************************/

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

/* macro declaration */
#define     _SET_MEM8(adr,dat)      *(volatile UB *)adr=(UB)dat  /* memory write(8Bit)   */
#define     _SET_MEM16(adr,dat)     *(volatile UH *)adr=(UH)dat  /* memory write(16Bit)  */
#define     _SET_MEM32(adr,dat)     *(volatile UW *)adr=(UW)dat  /* memory write(32Bit)  */
#define     _GET_MEM8(adr)          *(volatile UB *)adr          /* memory read(8Bit)  */
#define     _GET_MEM16(adr)         *(volatile UH *)adr          /* memory read(16Bit) */
#define     _GET_MEM32(adr)         *(volatile UW *)adr          /* memory read(32Bit) */

extern UW flhiadr;

/* prototype declaration */
W Flash(H *, H *, int);
W FlashX(H *, H *, int, int);
W    flash_write(H *adr,H data);
W    chip_erase(void);
W    sect_erase(UW sect);
void set_mode(UH cmd);
void set_fladr(UW flashadr);
