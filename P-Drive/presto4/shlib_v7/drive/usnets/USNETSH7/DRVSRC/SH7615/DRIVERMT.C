/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/rcs/drivermt.c 1.2 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: drivermt.c $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * 割込みテーブル書換無し
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* DRIVER.C  Edition: 2  Last changed: 17-Feb-98,15:17:40  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    DRIVER.C -- Driver Support Functions for USNET

    Copyright (C) 1993 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#include "net.h"
#include "local.h"
#include "driver.h"
#include <machine.h>

#ifdef V3
#include "norti3.h"
#include "nosys3.h"
#else
#include "itron.h"
#include "kernel.h"
#endif

#include "..\sh7615.h"

typedef volatile unsigned short PORT16;

/*extern unsigned long vector_table[];*/
// NH
//extern unsigned long *vector_table;

/* Save address of actual interrupt handlers for each network interface. */
static void (*irnew) (int netno);
static int irarg;

/* Stack area for the interrupt handler */
/*#define ETHER_INT_STACK_SZ   512*/
#define ETHER_INT_STACK_SZ   0x2000
VW ether_stack_area[ETHER_INT_STACK_SZ / sizeof (VW)];
static const VP p_ether_stack_area = (VP)&ether_stack_area[ETHER_INT_STACK_SZ / sizeof (VW)];

/*#pragma interrupt (irhan0(sp=p_ether_stack_area, tn=25))*/
/*#pragma interrupt (irhan0(sp=p_ether_stack_area))*/
#pragma interrupt (irhan0)

/* interrupt stub routines for the network interfaces. */

void irhan0(void)
{
#if 0
	static int multi;

	ent_int();
	if( !multi ) {
		multi = 1;					/* 多重割り込み禁止フラグset */
	    (*irnew)(irarg);
	}
	multi = 0;
	ret_int();
#else
	ent_int();
    (*irnew)(irarg);
	ret_int();
#endif
}

#if 0
/* Install E-DMAC interrupt. */
void IRinstall(int irno, int netno, void (*irhan) (int x))
{
    T_DINT iw;
    ER rc;
    volatile unsigned short *iprb = (unsigned short *)IPRB;
    volatile unsigned short *vcra = (unsigned short *)VCRA;

    irnew = irhan;
    irarg = netno;
    *iprb = (*iprb & 0x0fff) | (EDMAC_PRIOR << 12);
    *vcra = (*vcra & 0x007f) | (irno << 8);
/*    iw.intatr = TA_HLNG;*/
    iw.inhatr = TA_HLNG;
    iw.inthdr = (FP)irhan0;
/*    iw.intsr = 0;*/
    iw.imask = 0;
    rc = def_inh((UINT)irno,&iw);
#if NTRACE >= 1
    Nprintf("IRinstall : failed to install interrupt(0x%08lx)\n",rc);
#endif
}
#endif

/* 指定された割り込みレベルのハンドラを設定する。E-DMAC専用 */
/* irno  - ベクタ番号。ベクタ番号は72～127の範囲で自由に設定可能である。 */
/* netno - この割り込みを使用するLAN I/Fを管理するネットワーク番号。実際の */
/*         割り込みハンドラに引数として渡される */
/* irhan - 実際の割り込みハンドラのエントリアドレス */
void IRinstall(int irno, int netno, void (*irhan) (int x))
{
    PORT16 *vcra =   (PORT16 *)VCRA;
    PORT16 *iprb =   (PORT16 *)IPRB;
	long *vect;

    if ((irno < 68)||(irno > 127)) return;

    *iprb = (*iprb & 0x0fff) | (EDMAC_PRIOR << 12);
    *vcra = (*vcra & 0x007f) | (irno << 8);

//	vect = (long*)vector_table;
//	vect[irno] = (unsigned long)irhan0;

//	irnew = irhan;
//	irarg = netno;


	irnew = irhan;
	irarg = netno;
//saka	set_vector(irno, irhan0);
}

/* There is no service to delete interrup handler in HI7000. */
void IRrestore(int irno)
{
}


static UINT DISABLE_MSK;
static UINT Ndisable_MSK;

void  DISABLE( void ) {
	get_ims( &DISABLE_MSK );
	chg_ims(13);/*020715*/
}
void  ENABLE( void ) {
	chg_ims( DISABLE_MSK );
}

void Ndisable( void ) {
	Ndisable_MSK = get_imask();
	set_imask(15);
}

void Nenable( void ) {
	set_imask(Ndisable_MSK);
}
