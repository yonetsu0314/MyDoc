/******************************************************************************
* NORTi version4 Hook-routines for ADViCE                                     *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 19/Jan/2000  システムクロックを追加                                         *
* 15/Mar/2000  積算チッククロック用に修正                                     *
* 03/Jul/2000  NORTi version4 用に                                            *
******************************************************************************/

#ifndef HOOK
#define HOOK    1
#endif

#include "norti3.h"
#include "nosys3.h"

long Ros_syscall[11];
long tickck_read(void);

#define TASK_SWITCH     3
#define NOT_TASK        4
#define TASK_IDLE       5

void vinit_hook(void)
{
}

void vsys_swtsk(long tskid)
{
    Ros_syscall[0] = TASK_SWITCH;
    Ros_syscall[1] = tskid;
    Ros_syscall[2] = tickck_read();
}

void vsys_nottsk(void)
{
    Ros_syscall[0] = NOT_TASK;
    Ros_syscall[1] = 0;
    Ros_syscall[2] = tickck_read();
}

void vsys_idltsk(void)
{
    Ros_syscall[0] = TASK_IDLE;
    Ros_syscall[1] = 0;
    Ros_syscall[2] = tickck_read();
}

void vsys_call0(long fnc)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = 0;
    Ros_syscall[10] = tickck_read();
}

void vsys_call1(long fnc,long p1)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[10] = tickck_read();
}

void vsys_call2(long fnc,long p1,long p2)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[5] = p2;
    Ros_syscall[10] = tickck_read();
}

void vsys_call3(long fnc,long p1,long p2,long p3)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[5] = p2;
    Ros_syscall[6] = p3;
    Ros_syscall[10] = tickck_read();
}

void vsys_call4(long fnc,long p1,long p2,long p3,long p4)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[5] = p2;
    Ros_syscall[6] = p3;
    Ros_syscall[7] = p4;
    Ros_syscall[10] = tickck_read();
}

void vsys_call5(long fnc,long p1,long p2,long p3,long p4,long p5)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[5] = p2;
    Ros_syscall[6] = p3;
    Ros_syscall[7] = p4;
    Ros_syscall[8] = p5;
    Ros_syscall[10] = tickck_read();
}

void vsys_call6(long fnc,long p1,long p2,long p3,long p4,long p5,long p6)
{
    Ros_syscall[3] = fnc;
    Ros_syscall[4] = p1;
    Ros_syscall[5] = p2;
    Ros_syscall[6] = p3;
    Ros_syscall[7] = p4;
    Ros_syscall[8] = p5;
    Ros_syscall[9] = p6;
    Ros_syscall[10] = tickck_read();
}
