/*======================================================================
 * File name    : arcsub.c
 * Original     :p:/rcu32/ccw/main/comm500/rcs/arcsub.c 1.6
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/misc/arcnet/RCS/arcsub.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: arcsub.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 2003/08/05 compiler ver7 vup
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * VxWorksとshlibのインターフェース
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1988-1999, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		PHONE +81 775 53-4141
 *======================================================================
 */
#include	<biosif\com.h>			/* Show bios		*/
#include	<drive\comdrva.h>		/* Show drive		*/
#include	"arcdrv.h"

#define	__ARCSUB_C__

/*======================================================================*/

#define		CCW_DEVNO		(0)
#define		CCW_RCU_ID		(90)
#define		WCU_ID			(80)
#define		NHIC_ID			(100)

/*======================================================================*/

int  cw_chk_arcnet_tbl(int  N_ID) {return _chk_arcnet_tbl(CCW_DEVNO,N_ID);}
int  cw_arc_map(void) {return _arc_map(CCW_DEVNO);}
int  cw_map_chk(void) {return _map_chk(CCW_DEVNO);}
int  cw_arc_stat(enum statspc cmd) {return _arc_stat(CCW_DEVNO,cmd);}

int  cw_arc_node(int  id) {return _arc_node(CCW_DEVNO,id);}
int  cw_arc_nextid(void) {return _arc_nextid(CCW_DEVNO);}

