/******************************************************************************
* NORTi Network User's Hardware-dependent Functions                           *
*                                                                             *
*  Copyright (c) 1999-2000, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 23/Nov/1998                                                                 *
* 28/Apr/1999  Modified for MS7709RP01 board.                            [YT] *
* 16/May/1999  Added codes for IRQ/PINT interrupt support.               [YT] *
* 12/Sep/2001 §ŒäŒn‚Ì”»’f‚ðŒ©’¼‚µ                                       [TA] *
******************************************************************************/

#ifndef SH3
#define SH3
#endif

#undef blkio_INASM          /* low-level block I/O support not in assembler */
                            /* (These codes are written in this file.)      */
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#ifndef blkio_INASM
#include "nonethw.h"
#endif
#ifdef MS7709A
#include "sh7709a.h"
#else
#include "sh7709.h"
#endif

#if !defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)
error ENDIAN Not Defined
#endif

/*
 * Define Interrupt Configurations for LAN Controller
 */

/* Interrupt number  */
#define INT_NO INT_IRL3     /* use a value defined in sh770x.h */

/* PINT number (define only if PINT interrupt is used) */
#if INT_NO == INT_PINT0 || INT_NO == INT_PINT8
#define PINT_NO     1       /* PINTx */
#endif

/* Interrupt priority level */
#define IP          7
    /* NOTE: With Hitachi ULSI MS7709RP01 board, this value must be */
    /*       higher than timer interrupt priority. if not, the LAN  */
    /*       driver will not work properly.                         */

/* Interrupt detection mode (to configure ICR1/ICR2) */
#define INT_MOD     2       /* when IRQ interrupt is used  */
                            /*  0 = falling edge           */
                            /*  1 = rising edge            */
                            /*  2 = low level              */
                            /* when PINT interrupt is used */
                            /*  0 = low level              */
                            /*  1 = high level             */


/* IRQ number is calculated from interrupt number */

#if INT_NO >= INT_IRQ0 && INT_NO <= INT_IRQ5
#define IRQ_NO  (INT_NO-INT_IRQ0)
#endif

/* Port address and number of shift bits for IPR register */

#ifdef IRQ_NO
#if IRQ_NO >= 0 && IRQ_NO <= 3
#define INTC_IPR  INTC_IPRC
#define IPR_SHIFT (4*IRQ_NO)
#endif
#if IRQ_NO >= 4 && IRQ_NO <= 5
#define INTC_IPR  INTC_IPRD
#define IPR_SHIFT (4*(IRQ_NO-4))
#endif
#endif
#ifdef PINT_NO
#if PINT_NO >= 0 && PINT_NO <= 7
#define INTC_IPR  INTC_IPRD
#define IPR_SHIFT 12
#endif
#if PINT_NO >= 8 && PINT_NO <= 15
#define INTC_IPR  INTC_IPRD
#define IPR_SHIFT 8
#endif
#endif

/* Interrupt Handler Definition Information */

#ifdef SH3
const T_DINT dint_lan = { TA_HLNG, lan_int, IP };
#else
const T_DINT dint_lan = { TA_HLNG, lan_int };
#endif

/* Interrupt Handling Routine in LAN Driver (declared in <landrvr>.c) */
void lan_intr(void);

/*****************************************************************************
*
* Set up interrupt for LAN controller
*
* DP83902 INT -++-> SH77709 IRL3
******************************************************************************/

ER lan_def_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_NO, &dint_lan);  /* set interrupt vector */

#ifdef IRQ_NO
    psw = vdis_psw();                   /* interrupt detection mode */
    sfr_setw(INTC_ICR1, INT_MOD<<(2*IRQ_NO));
    sfr_setw(INTC_IPR, IP<<IPR_SHIFT);  /* interrupt priority level */
    if (sfr_inw(INTC_IRR0) & (0x1<<IRQ_NO))
        sfr_setw(INTC_IRR0, 0x1<<IRQ_NO); /* reset interrupt request */
    vset_psw(psw);
#endif
#ifdef PINT_NO
    psw = vdis_psw();                   /* interrupt detection mode */
    sfr_setw(INTC_ICR2, INT_MOD<<PINT_NO);
    sfr_setw(INTC_IPR, IP<<IPR_SHIFT);  /* interrupt priority level */
    sfr_setw(INTC_PINTE, 0x1<<PINT_NO); /* interrupt enable mask */
    vset_psw(psw);
#endif

    return ercd;
}

/*****************************************************************************
*
* Interrupt handler for LAN controller
*
******************************************************************************/

#if defined(__HITACHI__)                /* Hitach C */
#pragma interrupt(lan_int)
#endif

INTHDR lan_int(void)
{
    ent_int();
    lan_intr();
    ret_int();
}


/*****************************************************************************
*
* Low-level block I/O support for LAN controller
*
******************************************************************************/

#ifndef blkio_INASM

#ifdef BIG_ENDIAN
#define lan_inwb(p)      (*(volatile UH *)lan_port(p) >> 8)
#define lan_outwb(p,c)   (*(volatile UH *)lan_port(p) = (UH)(c) << 8)
#else
#define lan_inwb(p)      (*(volatile UH *)lan_port(p))
#define lan_outwb(p,c)   (*(volatile UH *)lan_port(p) = (c))
#endif

void    _inbrep(UW port, UB *ptr, UH len)
{
    while (len--)
        *ptr++ = lan_inb(port);
}
void    _outbrep(UW port, UB *ptr, UH len)
{
    while (len--)
        lan_outb(port, *ptr++);
}
void    _setbrep(UW port, UB data, UH len)
{
    while (len--)
        lan_outb(port, data);
}
void    _inwrep(UW port, UH *ptr, UH len)
{
    while (len > 1)
    {
        *ptr++ = lan_inw(port);
        len -= 2;
    }
    if (len == 1)
        *(UB *)ptr = lan_inwb(port);
}
void    _outwrep(UW port, UH *ptr, UH len)
{
    while (len > 1)
    {
        lan_outw(port, *ptr++);
        len -= 2;
    }
    if (len == 1)
        lan_outwb(port, *(UB *)ptr);
}
void    _setwrep(UW port, UB data, UH len)
{
    UH cc;

    cc = ((UH)data << 8) | data;
    while (len > 1)
    {
        lan_outw(port, cc);
        len -= 2;
    }
    if (len == 1)
        lan_outwb(port, data);
}

#endif

/* end */
