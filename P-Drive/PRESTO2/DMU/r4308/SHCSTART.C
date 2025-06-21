/*======================================================================
 * File name    : shcstart.c
 * Original		: p:/wcu/main/nz_wcu/rcs/shcstart.c 1.4
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto/dmu/main/comm100/rcs/shcstart.c 1.3.63.2 1970/01/01 00:00:00 itami Exp $
 *$Log: shcstart.c $
 * リビジョン 1.3.63.2  1970/01/01  00:00:00  itami
 * 2005/04/13 option.defを追加する。
 * 
 * リビジョン 1.3.63.1  1970/01/01  00:00:00  chimura
 * 2005/02/14
 * BIOSとAPEENDするため、ベクターなどのコピー処理を追加
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  hasiguci
 * 2003/07/19 15:10
 * 起動時の処理を一部整理
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  hasiguci
 * 2003/06/11 10:26
 * includeファイル類を整理
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  hasiguci
 * 初期リビジョン
 * 
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

#ifndef CPU_SH
#define CPU_SH			/* CPU_SH を定義してkernel.hをincludeしてください */
#endif
#include	<stdio.h>
#include	<kernel.h>
#include	<nosys4.h>
#define	TSKID_MAX	250	/* Maximum ID for TASK				*/
#define	SEMID_MAX	200	/* Maximum ID for SEMAPHORE			*/
#define	TPRI_MAX	31	/* Maximum Priority				*/
#define	FLGID_MAX	99	/* Maximum ID for EVENT FLAG			*/
#define	MBXID_MAX	99	/* Maximum ID for MAIL BOX			*/
#define	MBFID_MAX	1	/* Maximum ID for MESSAGE BUFFER		*/
#define	PORID_MAX	1	/* Maximum ID for RENDEVOUZ PORT		*/
#define	MPLID_MAX	1	/* Maximum ID for VALIABLE LENGTH MEMORY POOL	*/
#define	MPFID_MAX	1	/* Maximum ID for FIXED LENGTH MEMORY POOL	*/
#define	CYCNO_MAX	1	/* Maximum ID for CYCRIC HANDLER		*/
#define	ALMNO_MAX	1	/* Maximum ID for ALARM HANDLER			*/
#define	TMRQSZ		256
#define	CYCQSZ		256
#define	ALMQSZ		256
#define	ISTKSZ		2048	/* Stack size for Interval timer interrupt	*/
#define	TSTKSZ		2048
#include	<nocfg4.h>

#include	<biosif\init.h>			/* Show include dir.	*/

/*======================================================================
 *      セクション初期化
 *----------------------------------------------------------------------
*/
static void INITSCT(void)
{
	extern int *_D_ROM, *_B_BGN, *_B_END, *_D_BGN, *_D_END;
	int	*p, *q;

	for(p=_D_BGN, q=_D_ROM; p<_D_END; p++, q++) *p=*q;	/* SECTION (D)  */
	for(p=_B_BGN; p<_B_END; p++) *p=0;			/* SECTION (B)  */
}

/*----------------------------------------------------------------
				ライブラリ関数使用の変数の初期化
----------------------------------------------------------------*/
static void _INIT_IOLIB(void){
    FILE *fp;

    for(fp=_iob; fp<_iob+_NFILE; fp++ ){
        fp->_bufptr=NULL;
        fp->_bufcnt=0;
        fp->_buflen=0;
        fp->_bufbase=NULL;
        fp->_ioflag1=(char)0;
        fp->_ioflag2=(char)0;
        fp->_iofd=(char)0;
    }

    if(freopen( "stdin" , "r", stdin)==NULL)
        stdin->_ioflag1=(char)0xff;
    stdin->_ioflag1 |= _IOUNBUF;

    if(freopen( "stdout" , "w", stdout)==NULL)
        stdout->_ioflag1=(char)0xff;
    stdout->_ioflag1 |= _IOUNBUF;

    if(freopen( "stderr" , "w", stderr)==NULL)
        stderr->_ioflag1=(char)0xff;
    stderr->_ioflag1 |= _IOUNBUF;
}

static void _INITLIB( void ){
	_INIT_IOLIB();
}

void init_tsk(void);

const T_CTSK ctsk_main  = { TA_HLNG, NULL, init_tsk,  1, 4096, NULL, (B *)"" };
const T_CMPL tcmpl_main = {TA_MPRI, 4096, NULL, (B *)""};
#define	TSK_MAIN	1
#define	MPL_ID		1

void startup(void)
{
#if 1
	_init_vector();
	_init_section();
	_init_registers();
#endif
	INITSCT();
	sysini();
	_INITLIB();

	cre_tsk(TSK_MAIN, &ctsk_main);
	sta_tsk(TSK_MAIN, 0);
	cre_mpl(MPL_ID, &tcmpl_main);

	intsta();
	syssta();
	for(;;);
}
