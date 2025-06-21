/******************************************************************************
* NORTi version4 Hook-routines                                                *
*                                                                             *
*  Copyright (c)  1995-2001, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 02/Jul/2000                                                                 *
* 17/Dec/2001  Ver.4.05 ‘Î‰ž                                               TA *
* 04/Feb/2003  modified T_HOOK structure.                                  TA *
******************************************************************************/

#ifndef HOOK
#define  HOOK   1
#endif
#include "kernel.h"
#include "nosys4.h"
#include "n4hook.h"


#ifndef dis_ims
void FNEAR dis_ims(void);
#endif
#ifndef ret_ims2
void FNEAR ret_ims2(void);
#endif

#if (HOOK!=1)
long Ros_nowID;
#endif

#ifdef AGENT
extern unsigned int tsk_running;
extern long ITMON_MAIN;
extern unsigned char TSK_API_RECV;
extern unsigned char TSK_API_SEND;
#endif

void visnd_debe(void)
{
  #if (HOOK!=1)
    if ((Ros_hook.sys_grp & THG_SYS) && (Ros_hook.flag & THF_INT))
  #endif
        visnd_deb0(THF_INT, TFN_ENT_INT);
}

void visnd_debne(void)
{
    if (RDQ[0] != SDISP)
    {
      #if (HOOK!=1)
        if (Ros_hook.sys_grp & THG_DSP)
      #endif
            vsys_nottsk();
      #if (HOOK!=1)
        Ros_nowID = 0;
      #endif
    }

  #if (HOOK!=1)
    if ((Ros_hook.sys_grp & THG_SYS) && (Ros_hook.flag & THF_INT))
  #endif
        visnd_deb0(THF_INT, TFN_ENT_INT);
}

void visnd_debr(void)
{
  #if (HOOK!=1)
    if ((Ros_hook.sys_grp & THG_SYS) && (Ros_hook.flag & THF_INT))
  #endif
        visnd_deb0(THF_INT, TFN_RET_INT);
}

void visnd_debs(void)
{
    if (RDQ[0] == (TSK_ID)(CFG.tskid_max+1))
    {
        if (SDISP == 0)
        {
          #if (HOOK!=1)
            if (Ros_hook.sys_grp & THG_DSP)
          #endif
                vsys_idltsk();
          #if (HOOK!=1)
            Ros_nowID = -1;
          #endif
        }
    } else {
      #if (HOOK!=1)
        if (Ros_hook.sys_grp & THG_DSP)
      #endif
            vsys_swtsk((long)RDQ[0]);
      #if (HOOK!=1)
#ifdef AGENT
		if((Ros_nowID)&&(Ros_nowID <= CFG.tskid_max)&&
			(Ros_nowID != (unsigned long)ITMON_MAIN)&&
			(Ros_nowID != (unsigned long)TSK_API_RECV)&&
			(Ros_nowID != (unsigned long)TSK_API_SEND))
            tsk_running = Ros_nowID;
#endif
        Ros_nowID = (long)RDQ[0];
      #endif
    }
}

void visnd_debds(void)
{
    if (RDQ[0] == (TSK_ID)(CFG.tskid_max+1))
    {
        if (SDISP == 0)
        {
          #if (HOOK!=1)
            if (Ros_hook.sys_grp & THG_DSP)
          #endif
                vsys_idltsk();
          #if (HOOK!=1)
            Ros_nowID = -1;
          #endif
        }
        else
        {
          #if (HOOK!=1)
            if (Ros_hook.sys_grp & THG_DSP)
          #endif
                vsys_nottsk();
          #if (HOOK!=1)
            Ros_nowID = 0;
          #endif
        }
    } else {
      #if (HOOK!=1)
        if (Ros_hook.sys_grp & THG_DSP)
      #endif
            vsys_swtsk((long)RDQ[0]);
      #if (HOOK!=1)
#ifdef AGENT
		if((Ros_nowID)&&(Ros_nowID <= CFG.tskid_max)&&
			(Ros_nowID != (unsigned long)ITMON_MAIN)&&
			(Ros_nowID != (unsigned long)TSK_API_RECV)&&
			(Ros_nowID != (unsigned long)TSK_API_SEND))
            tsk_running = Ros_nowID;
#endif
        Ros_nowID = (long)RDQ[0];
      #endif
    }
}

void visnd_debns(void)
{
    if (RDQ[0] == (TSK_ID)(CFG.tskid_max+1))
    {
      #if (HOOK!=1)
        if (Ros_hook.sys_grp & THG_DSP)
      #endif
            vsys_idltsk();
      #if (HOOK!=1)
        Ros_nowID = -1;
      #endif
    }
}

void vsnd_deb0(long flag, long fnc)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call0(fnc);
    ret_ims2();
}

void vsnd_deb1(long flag, long fnc,long p1)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call1(fnc, p1);
    ret_ims2();
}

void vsnd_deb2(long flag, long fnc,long p1,long p2)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call2(fnc, p1, p2);
    ret_ims2();
}

void vsnd_deb3(long flag, long fnc,long p1,long p2,long p3)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call3(fnc, p1, p2, p3);
    ret_ims2();
}

void vsnd_deb4(long flag, long fnc,long p1,long p2,long p3,long p4)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call4(fnc, p1, p2, p3, p4);
    ret_ims2();
}

void vsnd_deb5(long flag, long fnc,long p1,long p2,long p3,long p4,long p5)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call5(fnc, p1, p2, p3, p4, p5);
    ret_ims2();
}

void vsnd_deb6(long flag, long fnc,long p1,long p2,long p3,long p4,long p5,long p6)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    dis_ims();
    vsys_call6(fnc, p1, p2, p3, p4, p5, p6);
    ret_ims2();
}

#if (HOOK==2)
void vsnd_deb0e(long flag, long fnc, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call0(fnc);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb1e(long flag, long fnc, long p1, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call1(fnc, p1);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb2e(long flag, long fnc, long p1, long p2, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call2(fnc, p1, p2);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb3e(long flag, long fnc, long p1, long p2, long p3, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call3(fnc, p1, p2, p3);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb4e(long flag, long fnc, long p1, long p2, long p3, long p4, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call4(fnc, p1, p2, p3, p4);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb5e(long flag, long fnc, long p1, long p2, long p3, long p4, long p5, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call5(fnc, p1, p2, p3, p4, p5);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

void vsnd_deb6e(long flag, long fnc, long p1, long p2, long p3, long p4, long p5, long p6, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    dis_ims();
    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call6(fnc, p1, p2, p3, p4, p5, p6);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
    ret_ims2();
}

ER   vsnd_deber(long flag, long ercd)
{
    if (Ros_hook.sys_grp & THG_RET)
    {   if (Ros_hook.flag & flag)
        {   if (Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F)))
                vsys_ret(ercd);
        }
    }
    ret_ims2();
    return ercd;
}

ER   vsnd_deber1(long flag, long ercd)
{
    if (Ros_hook.sys_grp & THG_RET)
    {   if (Ros_hook.flag & flag)
        {   if (Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F)))
            {   dis_ims();
                vsys_ret(ercd);
                ret_ims2();
            }
        }
    }
    return ercd;
}

void visnd_deber(long flag, long ercd)
{
    if (!(Ros_hook.sys_grp & THG_RET))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    vsys_ret(ercd);
}

void visnd_deb0e(long flag, long fnc, long ercd)
{
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;

    if (Ros_hook.sys_grp & THG_SYS)
        vsys_call0(fnc);
    if (Ros_hook.sys_grp & THG_RET)
        vsys_ret(ercd);
}

#endif

void visnd_deb0(long flag, long fnc)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call0(fnc);
}

void visnd_deb1(long flag, long fnc, long p1)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call1(fnc, p1);
}

void visnd_deb2(long flag, long fnc, long p1, long p2)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call2(fnc, p1, p2);
}

void visnd_deb3(long flag, long fnc, long p1, long p2, long p3)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call3(fnc, p1, p2, p3);
}

void visnd_deb4(long flag, long fnc, long p1, long p2, long p3, long p4)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call4(fnc, p1, p2, p3, p4);
}

void visnd_deb5(long flag, long fnc, long p1, long p2, long p3, long p4, long p5)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call5(fnc, p1, p2, p3, p4, p5);
}

void visnd_deb6(long flag, long fnc, long p1, long p2, long p3, long p4, long p5, long p6)
{
  #if (HOOK!=1)
    if (!(Ros_hook.sys_grp & THG_SYS))
        return;
    if (!(Ros_hook.flag & flag))
        return;
    if (!(Ros_hook.task_id[Ros_nowID>>5] & (0x00000001<<(Ros_nowID & 0x1F))))
        return;
  #endif

    vsys_call6(fnc, p1, p2, p3, p4, p5, p6);
}
