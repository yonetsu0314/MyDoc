/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/clockmt.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: clockmt.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/*
    CLOCK.C -- Clock routines for USNET
    Copyright (C) 1994
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This is the for the Hitachi SH7000 with uITRON
*/
#include <umachine.h>
#include <local.h>

#ifdef V3
#include "norti3.h"
#include "nosys3.h"
#else
#include "itron.h"
#include "kernel.h"
#endif

extern unsigned int clocks_per_sec;
/*
** Clock initializaiton.  Set number of clock ticks per second.
** this value must be in sync with value set in tstsup3.c.  See
** macros TINTVAL and TCORDAT.
*/
int             Nclkinit()
{
//    clocks_per_sec = 100;
    clocks_per_sec = 1000;	//nh
    return 0;
}

/*
**  Find current clock time.  Nclock only uses the lower
**  32bits of the T_TIM structure.
*/
unsigned long   Nclock()
{
    SYSTIME pk_time;
    get_tim(&pk_time);
    return pk_time.ltime;
}

void            Nclkterm()
{
}
