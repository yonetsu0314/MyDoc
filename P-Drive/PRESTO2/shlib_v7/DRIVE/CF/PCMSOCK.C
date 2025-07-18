/*======================================================================
 * File name    : pcmsock.c
 * Original		: r:/c/astra-s/exp/rcs/pcmsock.c 1.2
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/cf/rcs/pcmsock.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmsock.c $
 * rW 1.2  1970/01/01  00:00:00  sakaguti
 * 05.03.18 CD_CHKðBIOSÉÚ®
 * 
 * rW 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*                                                                      */
/*      t@C¼      : PCMSOCK.C                                     */
/* ==================================================================== */
/*      @\ªÞ        : oblbh`\PbgT[rX`oh	        */
/*      ì¬út        : 1997.12. 5 < Fri >                            */
/*      ì¬Ò¼        :                                               */
/*                                                                      */
/*  ðÇ                                                            */
/* ------------+----------+-------------------------------------------- */
/*  ú t      | ÇÔ | SÒ¼yÑÏXàe                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#include <stddef.h>
//#include </itron4/kernel.h>/*020726*/
#include <kernel.h>/*020726*/
#include <sh7615\7615.h>
#include <biosif\cf.h>
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#include "drive\pcmtpl.h"

	/*==========================================================
						àWX^é¾
	==========================================================*/
static char fake[1];
static char pcm_busy;
static void (*PcmcFptr)( PCMEVT *evt );
static PCMEVT PcmcEvent;


/*---------------------------------------------------------------------
				\PbgT[rXÌj^Ö
 ---------------------------------------------------------------------*/
static void ss_monitor( void )
{
	unsigned long *events;
	int i=0;

	if( !pcm_busy )	{
		events = &PcmcEvent.sock[i].event;
		if( fake[i] ) {
			*events =  0;
		}
		else {
			*events = _cd_chk( i ) ? SS_DETECT : 0;			/* J[hm 		*/
		}
	}
}

/*---------------------------------------------------------------
				oblbh`Cxg
 ---------------------------------------------------------------*/
static void PcmciaHandler( PCMEVT *evt )
{
	if( PcmcFptr != NULL ) {
		PcmcFptr( evt );
		PcmcEvent.sock[0].last = PcmcEvent.sock[0].event;
		if( fake[0] ) {
			fake[0] = 0;
		}
	}
	pcm_busy = 0;
}

/*****************************************************************
				oblbh`CxgÌo^
*****************************************************************/
void SetPcmciaHandler( void (*fptr)( void *evt ) )
{
	PcmcFptr = (void (*)(PCMEVT *))fptr;
}

/*****************************************************************
				oblbh`Cxg
*****************************************************************/
void PcmciaEvent( void )
{
	ss_monitor();
	pcm_busy = 1;
	if(PcmcEvent.sock[0].event != PcmcEvent.sock[0].last) {
		PcmciaHandler( &PcmcEvent );
		return;
	}
	pcm_busy = 0;
}

/*****************************************************************
				wè\Pbgð[IÉCWFNg·é
*****************************************************************/
int PcmciaEjectFake( int psock )
{
	if( psock == 0  )  {
		pcm_busy = 1;
		fake[psock] = 1;
		pcm_busy = 0;
		return 0;
	}
	return -1;
}

/*====================================================================
				oblbh`\PbgÌn[heXg
 ====================================================================*/
int PcmciaHardTest( int sock )
{
	tuple_t tuple;
	cisparse_t parse;
	unsigned char buf[128];

	/* bcMÌ¸ */
	if( !_cd_chk( sock )) {
		return -3;
	}
	/* Agr[gÌ¸ */
	tuple.Attributes = 0;
	tuple.DesiredTuple = CISTPL_FUNCID;
	tuple.TupleData = buf;
	tuple.TupleDataMax = 64;
	tuple.TupleOffset = 0;
	if( ReadFirstTuple( sock, &tuple, &parse ) != CS_SUCCESS ) {
		return -2;
	}
	return 0;
}

