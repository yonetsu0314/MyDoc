/******************************************************************************
* NORTi version4 Hook Routines definitions                                    *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 07/Mar/2000                                                                 *
* 02/Jul/2000                                                                 *
* 04/Feb/2003  modified T_HOOK structure.                                 TA  *
******************************************************************************/

#ifndef N4HOOK_H
#define N4HOOK_H
#ifdef __cplusplus
extern "C" {
#endif

#ifdef  LARGE_KERNEL
#ifndef TSKID_MAX
#define TSKID_MAX 65533
#endif
#endif

/************************************/
/* Hook-routine definitions         */
/************************************/

#if (HOOK==1)

#define vsnd_deb0e(a,b,c)               vsnd_deb0(a,b)
#define vsnd_deb1e(a,b,c,d)             vsnd_deb1(a,b,c)
#define vsnd_deb2e(a,b,c,d,e)           vsnd_deb2(a,b,c,d)
#define vsnd_deb3e(a,b,c,d,e,f)         vsnd_deb3(a,b,c,d,e)
#define vsnd_deb4e(a,b,c,d,e,f,g)       vsnd_deb4(a,b,c,d,e,f)
#define vsnd_deb5e(a,b,c,d,e,f,g,h)     vsnd_deb5(a,b,c,d,e,f,g)
#define vsnd_deb6e(a,b,c,d,e,f,g,h,i)   vsnd_deb6(a,b,c,d,e,f,g,h)

#define visnd_deb0e(a,b,c)              visnd_deb0(a,b)

#define vsnd_deb0(a,b)                  vsnd_deb0(b)
#define vsnd_deb1(a,b,c)                vsnd_deb1(b,c)
#define vsnd_deb2(a,b,c,d)              vsnd_deb2(b,c,d)
#define vsnd_deb3(a,b,c,d,e)            vsnd_deb3(b,c,d,e)
#define vsnd_deb4(a,b,c,d,e,f)          vsnd_deb4(b,c,d,e,f)
#define vsnd_deb5(a,b,c,d,e,f,g)        vsnd_deb5(b,c,d,e,f,g)
#define vsnd_deb6(a,b,c,d,e,f,g,h)      vsnd_deb6(b,c,d,e,f,g,h)

#define visnd_deb0(a,b)                 visnd_deb0(b)
#define visnd_deb1(a,b,c)               visnd_deb1(b,c)
#define visnd_deb2(a,b,c,d)             visnd_deb2(b,c,d)
#define visnd_deb3(a,b,c,d,e)           visnd_deb3(b,c,d,e)
#define visnd_deb4(a,b,c,d,e,f)         visnd_deb4(b,c,d,e,f)
#define visnd_deb5(a,b,c,d,e,f,g)       visnd_deb5(b,c,d,e,f,g)
#define visnd_deb6(a,b,c,d,e,f,g,h)     visnd_deb6(b,c,d,e,f,g,h)

void vsnd_deb0(long, long);
void vsnd_deb1(long, long, long);
void vsnd_deb2(long, long, long, long);
void vsnd_deb3(long, long, long, long, long);
void vsnd_deb4(long, long, long, long, long, long);
void vsnd_deb5(long, long, long, long, long, long, long);
void vsnd_deb6(long, long, long, long, long, long, long, long);

void visnd_deb0(long, long);
void visnd_deb1(long, long, long);
void visnd_deb2(long, long, long, long);
void visnd_deb3(long, long, long, long, long);
void visnd_deb4(long, long, long, long, long, long);
void visnd_deb5(long, long, long, long, long, long, long);
void visnd_deb6(long, long, long, long, long, long, long, long);

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

#else

#if (HOOK==2)
#define return_dispatch(a)              visnd_deber(a,E_OK);return dispatch()
#define return_dispatch1(a)             return vsnd_deber1(a,dispatch1())
#define return_dispatch2(a)             visnd_deber(a,E_OK);return dispatch2()
#define ret_ims2_(a)                    vsnd_deber(a,E_OK)
#define return_ret_ims2(a,b)            return vsnd_deber(a,b)
#define return_ret_ims(a)               return vsnd_deber(a,E_OK)
#define add_rdq_dispatch_(a,b,c)        add_rdq_dispatch(a,b,c)
#define chg_rdq_dispatch_(a,b,c)        chg_rdq_dispatch(a,b,c)
#define del_que_ret_ims_(a,b,c)         del_que_ret_ims(a,b,c)
#define del_que_dispatch1_(a,b,c)       del_que_dispatch1(a,b,c)
#define chg_que_dispatch1_(a,b,c,d)     chg_que_dispatch1(a,b,c,d)
#endif

#if (HOOK==3)
#define vsnd_deb0e(a,b,c)               vsnd_deb0(a,b)
#define vsnd_deb1e(a,b,c,d)             vsnd_deb1(a,b,c)
#define vsnd_deb2e(a,b,c,d,e)           vsnd_deb2(a,b,c,d)
#define vsnd_deb3e(a,b,c,d,e,f)         vsnd_deb3(a,b,c,d,e)
#define vsnd_deb4e(a,b,c,d,e,f,g)       vsnd_deb4(a,b,c,d,e,f)
#define vsnd_deb5e(a,b,c,d,e,f,g,h)     vsnd_deb5(a,b,c,d,e,f,g)
#define vsnd_deb6e(a,b,c,d,e,f,g,h,i)   vsnd_deb6(a,b,c,d,e,f,g,h)
#define visnd_deb0e(a,b,c)              visnd_deb0(a,b)
#endif

void vsnd_deb0(long, long);
void vsnd_deb1(long, long, long);
void vsnd_deb2(long, long, long, long);
void vsnd_deb3(long, long, long, long, long);
void vsnd_deb4(long, long, long, long, long, long);
void vsnd_deb5(long, long, long, long, long, long, long);
void vsnd_deb6(long, long, long, long, long, long, long, long);

ER   vsnd_deber(long, long);
ER   vsnd_deber1(long, long);
void vsnd_deb0e(long, long, long);
void vsnd_deb1e(long, long, long, long);
void vsnd_deb2e(long, long, long, long, long);
void vsnd_deb3e(long, long, long, long, long, long);
void vsnd_deb4e(long, long, long, long, long, long, long);
void vsnd_deb5e(long, long, long, long, long, long, long, long);
void vsnd_deb6e(long, long, long, long, long, long, long, long, long);

void visnd_deber(long, long);
void visnd_deb0(long, long);
void visnd_deb1(long, long, long);
void visnd_deb2(long, long, long, long);
void visnd_deb3(long, long, long, long, long);
void visnd_deb4(long, long, long, long, long, long);
void visnd_deb5(long, long, long, long, long, long, long);
void visnd_deb6(long, long, long, long, long, long, long, long);

void visnd_deb0e(long, long, long);

void vinit_hook(void);
void vsys_swtsk(long);
void vsys_nottsk(void);
void vsys_idltsk(void);
void vsys_ret(long);
void vsys_call0(long);
void vsys_call1(long, long);
void vsys_call2(long, long, long);
void vsys_call3(long, long, long, long);
void vsys_call4(long, long, long, long, long);
void vsys_call5(long, long, long, long, long, long);
void vsys_call6(long, long, long, long, long, long, long);

#ifndef LARGE_KERNEL
typedef struct
{   long task_id[8];
    long sys_grp;
    long flag;
} T_HOOK;
#else
typedef struct
{	long sys_grp;
	long flag;
	long task_id[2];
} T_HOOK;
#endif

typedef struct
{   unsigned long size;
    unsigned long putp;
    unsigned long getp;
    unsigned long count;
    unsigned long rate;
} T_TRACE;

extern T_HOOK Ros_hook;
extern T_TRACE Ros_trace;
extern long Ros_syscall[];
extern long Ros_nowID;

#endif

#ifdef __cplusplus
}
#endif
#endif /* N4HOOK_H */
