/******************************************************************************
* ITRON Common Definitions/Declarations                                       *
*                                                                             *
*  Copyright (c)  1995-2003, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Apr/2000                                                                 *
* 07/Aug/2000  Supported H8 Series                                            *
* 05/Sep/2000  Supported ARM Series                                           *
* 28/Sep/2000  Supported PPC Series                                 OK(MiSPO) *
* 06/Nov/2000  Supported EW/IAR                                     TA        *
* 21/Nov/2000  corrected ER_UINT typedef                            TA        *
* 20/Dec/2000  Supported EW/IAR(H8) Series                          TA        *
* 15/Jan/2001  Supported Hitachi C new version                      TA        *
* 15/Feb/2001  Supported H8S/2600 series                            TA        *
* 27/Feb/2001  Supported Microtec Compiler for PPC Series           OK        *
* 28/Feb/2001  Supported XASSV(SH) series                           TA        *
* 05/Mar/2001  Supported Fujitsu FR30 Series                        TA        *
* 22/Jun/2001  Supported XASSV(ARM) series                          TA        *
* 29/Jul/2001  Supported MIPS Series(R4000/R3000)                   OK        *
* 10/Oct/2001  Supported M32R Series by CC32R                       OK        *
* 20/Dec/2001  Supported M16C Series                                TA        *
* 16/Jan/2002  Corrected NEAR                                       TA        *
* 14/May/2002  Corrected blk_alloc                                  TA        *
*              Corrected MODE typedef                               TA        *
* 20/Jun/2002  Supported Fujitsu FR Series with Softune             OK        *
* 28/Aug/2002  Supported EW/IAR(ARM) Series                         TA        *
* 24/Jun/2003  for kernel Ver. 4.07                                 TA        *
******************************************************************************/

#ifndef ITRON_H
#define ITRON_H
#ifdef __cplusplus
extern "C" {
#endif

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
#elif defined(__IAR_SYSTEMS_ICC__)
 #if defined(__ICCARM__)
  #if (__CPU_MODE__==1)
   #define CPU_THUMB
  #elif (__CPU_MODE__==2)
   #define CPU_ARM
  #else
   #error Not supported ARM!
  #endif
 #elif defined(__TID__)
  #if (__VER__>=122)            /* new ICC */
   #define C_task  /**/
   #if ((__TID__&0x7f00)==0x0a00)
    #define CPU_H83             /* H8/300H large */
   #elif ((__TID__&0x7f00)==0x0900)/* H8/500 */
    #define CPU_H85
    #if ((__TID__&0xff)==0x00)  /* H8/500 minimum */
     #define H85_MINIMUM
    #endif
   #elif (((__TID__&0xff)==0x11)||((__TID__&0xff)==0x21))
    #if ((__TID__&0xff)==0x21)
     #define H8S2600
    #endif
    #define CPU_H8S             /* H8S large */
    #ifndef INTM
     #define INTM    2
    #endif
   #elif ((__TID__&0xff)==0x01)
    #define CPU_H83             /* H8/300H large */
    #ifndef INTM
     #define INTM    1
    #endif
   #else
    #error Not Supported CPU!
   #endif
  #elif (__VER__>=111)          /* new ICC for SH */
   #define C_task /**/
   #if (__TID__==0x4600)        /* SH Series */
    #define CPU_SH
   #elif (__TID__==0x4610)
    #define CPU_SH
   #elif (__TID__==0x4620)
    #define CPU_SH
    #ifndef SH3
     #define SH3
    #endif
   #elif (__TID__==0x4630)
    #define CPU_SH
    #define DSP
   #elif (__TID__==0x4640)
    #define CPU_SH
    #ifndef SH3
     #define SH3
    #endif
    #define DSP
   #else
    #error Not Supported CPU!
   #endif
  #else                          /* old ICC */
   #if ((__TID__&0x7f00)==0x0a00)/* H8/300H */
    #define CPU_H83
    #define OLD_ICC_H83
   #elif ((__TID__&0x7f00)==0x1700)/* Z80 */
    #define CPU_Z80
   #else
    #error Not Supported CPU!
   #endif
  #endif
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__DCC__)          /* D-CC */
 #if defined(m68k)              /* 68K */
  #define CPU_68K
 #elif defined(__ppc)           /* PowerPC */
  #define CPU_PPC
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__HITACHI__)      /* Renesas new version */
 #if (defined(_SH1)||defined(_SH2))
  #define CPU_SH
 #elif defined(_SH2E)
  #ifndef SH2E
   #define SH2E
  #endif
  #define CPU_SH
 #elif defined(_SH3)
  #ifndef SH3
   #define SH3
  #endif
  #define CPU_SH
 #elif defined(_SH3E)
  #ifndef SH3E
   #define SH3E
  #endif
  #define CPU_SH
 #elif defined(_SH4)
  #ifndef SH4
   #define SH4
  #endif
  #define CPU_SH
 #elif defined(__300HA__)
  #define CPU_H83
  #ifndef INTM
   #define INTM    1
  #endif
 #elif (defined(__2000A__)||defined(__2600A__))
  #if defined(__2600A__)
   #define H8S2600
  #endif
  #define CPU_H8S
  #ifndef INTM
   #define INTM    2
  #endif
 #else
  #error Not Supported CPU!
 #endif
#elif defined(_MICROTEC)
 #if defined(_MCCPPC) || defined(_CCCPPC)
  #define CPU_PPC
 #elif defined(_MCC68K)         /* MCC68K */
  #define CPU_68K
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__GNUC__)         /* exeGCC */
 #if defined(__sh__)            /* SH */
  #define CPU_SH
  #if defined(__SH3__)
   #ifndef SH3
    #define SH3
   #endif
  #elif defined(__SH4__)
   #ifndef SH4
    #define SH4
   #endif
  #endif
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__ghs)
 #if defined(__SH7000)          /* Green Hills C-SH */
  #define CPU_SH
  #if defined(__SH_3)
   #ifndef SH3
    #define SH3
   #endif
  #elif defined(__SH_4)
   #ifndef SH4
    #define SH4
   #endif
  #endif
 #elif defined(__V850)          /* Green Hills V850 */
  #define CPU_V850
 #elif (defined(__V810)||defined(__V830))  /* Green Hills V800 */
  #define CPU_V800
 #elif defined(__THUMB)         /* Green Hills Thumb mode */
  #define CPU_THUMB
 #elif defined(__ARM)           /* Green Hills ARM mode */
  #define CPU_ARM
 #elif defined(__mips)          /* MIPS CPU             */
  #if defined(__mips64)
   #define MIPS64               /* MIPS 64bit mode  */
  #endif
  #define CPU_R4000             /* R4xxx 32bit or R3xxx 32bit */
 #elif defined(__FR)            /* Green Hills FR30 */
  #define CPU_FR
 #else
  #error Not Supported CPU!
 #endif
#elif defined(GAIO)             /* GAIO XASS-V */
 #if defined(SH7000)            /* XASSV(SH) */
  #define CPU_SH
 #elif defined(ARM)             /* XASSV(ARM) */
  #define CPU_ARM
 #elif defined(THUMB)           /* XASSV(THUMB) */
  #define CPU_THUMB
 #elif defined(H8S_Adva)         /* XASSV(H8S) */
  #define CPU_H8S
  #ifndef INTM
  #define INTM     2
  #endif
 #elif defined(H8S_Norm)         /* XASSV(H8S) Normal mode not support */
  #error Not Supported CPU!
 #elif defined(H8300H)           /* XASSV(H8/300H) need manual define */
  #define CPU_H83
  #ifndef INTM
  #define INTM     1
  #endif
 #elif defined(M16C60)          /* XASSV(M16C) */
  #define CPU_M16C
  #define LARGE
 #else
  #error Not Supported CPU (GAIO) !
 #endif
#elif defined(__CC_NORCROFT)    /* Norcroft C   */
 #if defined(__thumb)           /* Thumb mode */
  #define CPU_THUMB
 #elif defined(__arm)           /* ARM mode */
  #define CPU_ARM
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__CC_ARM)         /* ARM C   */
 #if defined(__thumb)           /* Thumb mode */
  #define CPU_THUMB
 #elif defined(__arm)           /* ARM mode */
  #define CPU_ARM
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__MWERKS__)       /* Metrowerks CodeWarrior */
 #if defined(__SH3__)
  #define CPU_SH
 #elif defined(__POWERPC__)
  #define CPU_PPC
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__CPU__)          /* Renesas H8C */
 #ifndef __HITACHI__
  #define __HITACHI__
 #endif
 #if (__CPU__==3)               /* H8/300H */
  #define CPU_H83
  #ifndef INTM
   #define INTM    1
  #endif
 #elif (__CPU__==5||__CPU__==7) /* H8S/2000,H8S/2600 */
  #if (__CPU__==5)
   #define H8S2600
  #endif
  #define CPU_H8S
  #ifndef INTM
   #define INTM    2
  #endif
 #else
  #error Not Supported CPU!
 #endif
#elif defined(__MEMORYMODEL__)  /* H8/500 */
 #define CPU_H85
 #if (__MEMORYMODEL__ == 0)     /* H8/500 minimum */
  #define H85_MINIMUM
 #endif
#elif defined(M16C)             /* Renesas NC30WA */
 #define CPU_M16C
#elif defined(__M32R__)         /* Renesas CC32R */
 #define CPU_M32R
#elif defined(__CPU_FR__)       /* Softune FR */
 #define CPU_FR
#else                           /* Renesas SHC */
 #ifndef __HITACHI__
  #define __HITACHI__
 #endif
 #ifndef __HITACHI_VERSION__
  #define __HITACHI_VERSION__ 0x0501
 #endif
 #if (defined(_SH1)||defined(_SH2))
  #define CPU_SH
 #elif defined(_SH2E)
  #ifndef SH2E
   #define SH2E
  #endif
  #define CPU_SH
 #elif defined(_SH3)
  #ifndef SH3
   #define SH3
  #endif
  #define CPU_SH
 #elif defined(_SH3E)
  #ifndef SH3E
   #define SH3E
  #endif
  #define CPU_SH
 #elif defined(_SH4)
  #ifndef SH4
   #define SH4
  #endif
  #define CPU_SH
 #else
  #error Not Supported CPU!
 #endif
#endif

/* size of integer and pointer */

#ifndef SIZEOF_INT
#if (defined(CPU_Z80)||defined(CPU_78K0))
#define SIZEOF_INT  1
#elif (defined(CPU_86)||defined(CPU_H83)||defined(CPU_H8S)||defined(CPU_H85)||defined(CPU_M16C))
#define SIZEOF_INT  2
#else
#define SIZEOF_INT  4
#endif
#endif

#if (defined(CPU_Z80)||defined(CPU_78K0))
#define SIZEOF_PTR  2
#elif (defined(M_I86SM)||defined(M_I86MM)||defined(__SMALL__)||defined(__MEDIUM__)||defined(H85_MINIMUM))
#define SIZEOF_PTR  2
#elif defined(CPU_M16C)
#ifdef LARGE
#define SIZEOF_PTR  4
#else
#define SIZEOF_PTR  2
#endif
#else
#define SIZEOF_PTR  4
#endif

/* alignment size for memory management */

#ifndef SIZEOF_ALIGN
#if ((SIZEOF_INT<=2)&&(SIZEOF_PTR<=2))
#define SIZEOF_ALIGN 2
#else
#define SIZEOF_ALIGN 4
#endif
#endif

#if (SIZEOF_ALIGN >= (SIZEOF_PTR*2))
#define MINIMUM_BLK 0
#else
#define MINIMUM_BLK 1
#endif

/* 8086 & M16C/60 key words */

#ifdef CPU_86
 #define FFAR        far
 #define SFAR        far
 #define DFAR        far
 #define PFAR        /**/
 #define FNEAR       near
 #define SNEAR       near
 #define NEAR        near
 #define cdecl       /**/
 #define NORTiAPI    /**/
 #define pascal      /**/
 #define SIZEOF_NALIGN 2
#else
#ifdef CPU_M16C
 #define FFAR        /**/
 #define SFAR        near
 #define DFAR        far
 #ifdef LARGE
  #define PFAR        far   /* large mode ‚Å‚Í far (Check) */
 #else
  #define PFAR        /**/
 #endif
 #define FNEAR       /**/
 #define SNEAR       near
 #define NEAR        near
 #define cdecl       /**/
 #define NORTiAPI    /**/
 #define pascal      /**/
 #define SIZEOF_NALIGN 2
#else
 #define FFAR        /**/
 #define SFAR        /**/
 #define DFAR        /**/
 #define PFAR        /**/
 #define FNEAR       /**/
 #define SNEAR       /**/
 #define NEAR        /**/
 #define cdecl       /**/
 #define NORTiAPI    /**/
 #define pascal      /**/
 #define SIZEOF_NALIGN SIZEOF_ALIGN
#endif
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
typedef void PFAR *VP;      /* pointer to an unpredictable data type */
typedef void (FFAR *FP)();  /* program start address */

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
typedef UINT MODE;          /* system call mode (v4.0) */
typedef INT ER;             /* error code */
typedef INT PRI;            /* task priority */
typedef ER ER_ID;           /* error code or object ID (v4.0) */
typedef UINT STAT;          /* status of object (v4.0) */
typedef INT ER_UINT;        /* error code or unsigned int (v4.0) */
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
}
#endif
#endif /* ITRON_H */
