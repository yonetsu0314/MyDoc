/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/n4hook.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: n4hook.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/******************************************************************************
* NORTi4 Hook Routines definitions                                            *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 07/Mar/2000                                                                 *
******************************************************************************/

#ifndef N4HOOK_H
#define N4HOOK_H
#ifdef __cplusplus
extern "C" {
#endif
#include <stdarg.h>

/************************************/
/* Hook-routine definitions         */
/************************************/

void vsys_swtsk(long);
void vsys_nottsk(void);
void vsys_idltsk(void);
void vsys_call0(long);
void vsys_call1(long, long);
void vsys_call2(long, long, long);
void vsys_call3(long, long, long, long);
void vsys_call4(long, long, long, long, long);
void vsys_call5(long, long, long, long, long, long);
void vsys_call6(long, long, long, long, long, long, long);
void vsys_callv(long, va_list);

#ifdef __cplusplus
};
#endif
#endif /* N4HOOK_H */
