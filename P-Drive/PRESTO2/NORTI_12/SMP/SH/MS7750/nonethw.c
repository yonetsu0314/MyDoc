/******************************************************************************
* NORTi Network User's Hardware-dependent Functions                           *
* This is for Hitachi SH-4.                                                   *
*                                                                             *
*   File:           NONETHW.C                                                 *
*   Last released:  29/Dec/1999                                               *
*                                                                             *
*  Written by Yasuo Tomita of Tygnus Computer Works.                          *
*                                                                             *
* 29/Nov/1999  Created to port to Hitachi ULSI MS7750SE01 board.         [YT] *
* 11/Dec/1999  Changed the BIG_ENDIAN macro to BIG.                      [YT] *
* 12/Sep/2001 êßå‰ånÇÃîªífÇå©íºÇµ                                       [TA] *
******************************************************************************/

#ifndef SH4
#define SH4
#endif

#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#include "nonethw.h"
#include "sh7750.h"

/*
 * Define Interrupt Configurations for LAN Controller
 */
 
/* Interrupt number  */
#define INT_LANC INT_IRL3   /* use a macro defined in sh775x.h */

/* Interrupt priority level */
#ifndef IP
#define IP          8
#endif
/* 
   NOTE: With a board using NS DP8390 family controller, the value for 
         the IP macro must be higher than interrupt priority for timer
         device. If not, the LAN driver may not work properly.
*/

/* Interrupt function mode for IRL pins (to configure ICR) */
#define IRL_MOD     0       /* 0 = used as level-encorded interrupts */
                            /* 1 = used as four individual interrupts *

/* Interrupt Handler Definition Information */

#if defined(SH3) || defined(SH4)
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
* DP83902 INT -++-> SH7750 IRL3
******************************************************************************/

ER lan_def_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_LANC, &dint_lan); /* set interrupt vector */

    psw = vdis_psw();
    sfr_setw(INTC_ICR, IRL_MOD<<7);     /* interrupt control mode */
    vset_psw(psw);

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

#define lan_indw(p)     lan_inw(p)
#define lan_outdw(p,c)  lan_outw(p, c)
#ifdef LITTLE_ENDIAN
#define lan_indb(p)     ((UB)*(volatile UH *)lan_port(p))
#define lan_outdb(p,c)  (*(volatile UH *)lan_port(p) = (UB)(c))
#else
#define lan_indb(p)     (*(volatile UH *)lan_port(p) >> 8)
#define lan_outdb(p,c)  (*(volatile UH *)lan_port(p) = (UH)(c) << 8)
#endif

void    lan_inbrep(UW port, UB *ptr, UW len)
{
    while (len--)
        *ptr++ = lan_in(port);
}
void    lan_outbrep(UW port, UB *ptr, UW len)
{
    while (len--)
        lan_out(port, *ptr++);
}
void    lan_inwrep(UW port, UH *ptr, UW len)
{
    while (len > 1)
    {
        *ptr++ = lan_indw(port);
        len -= 2;
    }
    if (len == 1)
        *(UB *)ptr = lan_indb(port);
}
void    lan_outwrep(UW port, UH *ptr, UW len)
{
    while (len > 1)
    {
        lan_outdw(port, *ptr++);
        len -= 2;
    }
    if (len == 1)
        lan_outdb(port, *(UB *)ptr);
}

#endif

/* end */
