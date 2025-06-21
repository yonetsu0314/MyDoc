/****************************************************************************
* NORTi Network User's Hardware-dependent Functions for M1543C driver       *
*                                                                           *
* File name         : mc1543int.c                                           *
*                                                                           *
* 05/Sept/2001      : SP                                                    *
****************************************************************************/

#include "norti3.h"
#include "sh7751.h"
#include "ms7751.h"


/*  "CH2" should be pre-defined during compilation if UART1 is implemented
    "CH3" should be pre-defined during compilation if UART3 is implemented
    "LANC" should be pre-defined during compilation if LAN controller driver
    is implemented
*/

#ifdef CH2
extern void uart1_intr(void);   /* Serial Ch2 Interrupt handler routine */
#endif

#ifdef CH3
extern void uart3_intr(void);   /* Serial Ch3 Interrupt handler routine */
#endif

#ifdef LANC
extern void lan_intr(void);     /* LANC Interrupt handler routine */
#endif

INTHDR sio_int(void);

#ifndef IP
    #define IP  IP_SIO          /* Interrupr priority declaration */
#endif

const T_DINT dsio_int = { TA_HLNG, sio_int, IP };

/*****************************************************************************
* Set up interrupt for SuperIO and LAN controller
*
******************************************************************************/

ER def_sio_int(void)
{
    UINT psw;
    ER ercd;

    ercd = def_int(INT_SIO, &dsio_int);    /* set interrupt vector */
    psw = vdis_psw();
    sfr_clrw(INTC_ICR, 0x0080);
    vset_psw(psw);

    return ercd;
}

/*****************************************************************************
* Interrupt handler for LAN controller
*
******************************************************************************/
#if defined(__HITACHI__)            /* Hitach C */
#pragma interrupt(sio_int)      /* required for Renesas Compiler */
#endif

INTHDR sio_int(void)
{
    ent_int();      /* Start interrupt handler */

#ifdef CH2
    uart1_intr();   /* If UART1 enabled then update Interrupt Task */
#endif

#ifdef CH3
    uart3_intr();   /* If UART3 enabled then update Interrupt Task */
#endif

#ifdef LANC
    lan_intr();
#endif

    ret_int();      /* End interrupt handler */
}

/*****************************************************************************/
/* end */
