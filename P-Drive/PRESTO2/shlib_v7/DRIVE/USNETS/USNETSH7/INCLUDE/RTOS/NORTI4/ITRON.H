/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/rtos/norti4/RCS/itron.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: itron.h $
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
* ITRON Common Definitions/Declarations                                       *
*                                                                             *
*  Copyright (c)  1995-2000, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
******************************************************************************/

#ifndef ITRON_H
#define ITRON_H
#ifdef __cplusplus
extern "C" {
#endif

/*#define V3*/

#ifndef NULL
#define NULL    ((void *)0)
#endif
#define NADR    NULL            /* ((void*)(-1)) */

/************************************/
/* Processors                       */
/************************************/

#if defined(M_I86)              /* MS-C */
#define CPU_86
#elif defined(__TURBOC__)       /* TURBO-C, BORLAND-C */
#define CPU_86
#if defined(__HUGE__)
#error Not Supported Model!
#endif
#elif defined(LSI_C80)          /* LSIC-80 */
#define CPU_Z80
#elif defined(_MCC68K)          /* MCC68K */
#define CPU_68K
#elif defined(__TID__)
#if (__VER__>=122)              /* new ICC */
#define C_task  /**/
#if ((__TID__&0x7f00)==0x0a00)
#define CPU_H83                 /* H8/300H large */
#elif ((__TID__&0x7f00)==0x0900)/* H8/500 */
#define CPU_H85
#if ((__TID__&0xff)==0x00)      /* H8/500 minimum */
#define H85_MINIMUM
#endif
#elif (((__TID__&0xff)==0x11)||((__TID__&0xff)==0x21))
#define CPU_H8S                 /* H8S large */
#elif ((__TID__&0xff)==0x01)
#define CPU_H83                 /* H8/300H large */
#else
#error Not Supported CPU!
#endif
#else                           /* old ICC */
#if ((__TID__&0x7f00)==0x0a00)  /* H8/300H */
#define CPU_H83
#define OLD_ICC_H83
#elif ((__TID__&0x7f00)==0x1700)/* Z80 */
#define CPU_Z80
#else
#error Not Supported CPU!
#endif
#endif
#elif defined(__DCC__)          /* D-CC */
#if defined(m68k)               /* 68K */
#define CPU_68K
#elif defined(__ppc)            /* PowerPC */
#define CPU_PPC
#else
#error Not Supported CPU!
#endif
#elif defined(__GNUC__)         /* exeGCC */
#if defined(__sh__)             /* SH */
#define CPU_SH
#else
#error Not Supported CPU!
#endif
#elif defined(__SH7000)         /* Green Hills C-SH */
#define CPU_SH
#elif defined(__V850)           /* Green Hills V850 */
#define CPU_V850
#elif defined(__CC_NORCROFT)    /* Norcroft C   */
#if defined(__thumb)            /* Thumb mode */
#define CPU_THUMB
#elif defined(__arm)            /* ARM mode */
#define CPU_ARM
#else
#error Not Supported CPU!
#endif
#elif defined(__CPU__)          /* Hitachi C */
#if (__CPU__==3)                /* H8/300H */
#define CPU_H83
#elif (__CPU__==5||__CPU__==7)  /* H8S/2000,H8S/2600 */
#define CPU_H8S
#else
#error Not Supported CPU!
#endif
#elif defined(__MEMORYMODEL__)  /* H8/500 */
#define CPU_H85
#if (__MEMORYMODEL__ == 0)      /* H8/500 minimum */
#define H85_MINIMUM
#endif
#else                           /* Hitachi SHC */
#define CPU_SH
#endif

/* size of integer and pointer */

#ifndef SIZEOF_INT
#if (defined(CPU_Z80)||defined(CPU_78K0))
#define SIZEOF_INT  1
#elif (defined(CPU_86)||defined(CPU_H83)||defined(CPU_H8S)||defined(CPU_H85))
#define SIZEOF_INT  2
#else
#define SIZEOF_INT  4
#endif
#endif

#if (defined(CPU_Z80)||defined(CPU_78K0))
#define SIZEOF_PTR  2
#elif (defined(M_I86SM)||defined(M_I86MM)||defined(__SMALL__)||defined(__MEDIUM__)||defined(H85_MINIMUM))
#define SIZEOF_PTR  2
#else
#define SIZEOF_PTR  4
#endif

/* 8086 key words */

#ifdef CPU_86
#define FAR         far
#define NEAR        near
#else
#define FAR         /**/
#define NEAR        /**/
#define cdecl       /**/
#define NORTiAPI    /**/
#define pascal      /**/
#endif


/************************************/
/* Data Types                       */
/************************************/

/* general-purpose data types */

typedef signed char B;      /* signed 8-bit integer */
typedef long W;             /* signed 32-bit integer */
typedef unsigned char UB;   /* unsigned 8-bit integer */
typedef unsigned long UW;   /* unsigned 32-bit integer */
typedef char VB;            /* unpredictable data type (8-bit size) */
typedef long VW;            /* unpredictable data type (32-bit size) */
typedef void *VP;           /* pointer to an unpredictable data type */
typedef void (FAR *FP)();   /* program start address */

#if (SIZEOF_INT <= 2)       /* 16-bit,8-bit CPU */
typedef int H;              /* signed 16-bit integer */
typedef unsigned int UH;    /* unsigned 16-bit integer */
typedef int VH;             /* unpredictable data type (16-bit size) */
#else                       /* 32-bit CPU */
typedef short H;            /* signed 16-bit integer */
typedef unsigned short UH;  /* unsigned 16-bit integer */
typedef short VH;           /* unpredictable data type (16-bit size) */
#endif

#if (SIZEOF_PTR==2)
typedef unsigned int SIZE;
typedef unsigned int ADDR;
#else
typedef unsigned long SIZE;
typedef unsigned long ADDR;
#endif

typedef VP VP_INT;          /* pointer to an unpredictable data type
                               or signed integer (v4.0) */

/* data types dependent on ITRON */

#if (SIZEOF_INT == 1)       /* 8-bit CPU */
typedef signed char INT;    /* signed integer (bit width of processor) */
typedef unsigned char UINT; /* unsigned integer (bit width of processor) */
#elif defined(OLD_ICC_H83)  /* old ICC H8/300H */
typedef short INT;
typedef unsigned short UINT;
#else                       /* 16-bit,32-bit CPU */
typedef int INT;
typedef unsigned int UINT;
#endif

typedef INT BOOL;           /* boolean value TRUE(1) or FALSE(0) */
typedef INT FN;             /* function code */
typedef INT ID;             /* object ID number */
typedef INT BOOL_ID;        /* boolean value or ID number */
typedef INT HNO;            /* handler number */
typedef INT RNO;            /* rendezvous number */
typedef INT RDVNO;          /* rendezvous number (v4.0) */
typedef UINT RDVPTN;        /* rendezvous pattern (v4.0) */
typedef UINT ATR;           /* object or handler attribute */
typedef INT ER;             /* error code */
typedef INT PRI;            /* task priority */
typedef ER ER_ID;           /* error code or object ID (v4.0) */
typedef UINT STAT;          /* status of object (v4.0) */
typedef UINT ER_UINT;       /* error code or unsigned int (v4.0) */
typedef UINT TEXPTN;        /* causal pattern for task exception (v4.0) */
typedef UINT FLGPTN;        /* bit pattern for event flag (v4.0) */
typedef UINT INHNO;         /* interrupt handler number (v4.0) */
typedef UINT INTNO;         /* interrupt number (v4.0) */

#if (SIZEOF_INT == 1)       /* 8-bit CPU */

typedef H TMO;              /* timeout value */
typedef struct t_systime    /* time of system clock */
{   H utime;                /* upper 16 bits */
    UH ltime;               /* lower 16 bits */
} SYSTIME;
typedef SYSTIME CYCTIME;    /* time of cyclic handler */
typedef SYSTIME ALMTIME;    /* time of alarm handler */
typedef H DLYTIME;          /* time of task delay */

#else                       /* 16-bit,32-bit CPU */

typedef W TMO;              /* timeout value */
typedef struct t_systime    /* time of system clock */
{   H utime;                /* upper 16 bits */
    UW ltime;               /* lower 32 bits */
} SYSTIME;
typedef SYSTIME CYCTIME;    /* time of cyclic handler */
typedef SYSTIME ALMTIME;    /* time of alarm handler */
typedef W DLYTIME;          /* time of task delay */

#endif

typedef DLYTIME RELTIM;     /* relative time (v4.0) */
typedef SYSTIME V4_SYSTIM;  /* absolute time (v4.0) */
typedef DLYTIME OVRTIM;     /* overrun time (v4.0) */

#ifdef __cplusplus
};
#endif
#endif /* ITRON_H */
