/******************************************************************************
* Driver file for PCMCIA Controller MR-SHPC-01 V2）                           *
*                                                                             *
*  File name : mrshpc.c                                                       *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 04/Oct/2001     ; file created by [SP]                                      *
* 07/Apr/2002     ; corrected for new file system [SP]                        *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え     KA *
******************************************************************************/

#ifdef V2
 #include "norti.h"
#elif defined(V3)
 #include "norti3.h"
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
#endif

#include "sh7751.h"
#include "mrshpc.h"

/*****************************************************************************
    User adjustable small delay for 1 microsecond.

*****************************************************************************/
volatile void us_wait(void)
{
    short cnt=1;    /* Adjust cnt Initialization value for 1 us */
    while (--cnt);
}
/*****************************************************************************
    Small programmable delay routine.

*****************************************************************************/
static void wait(volatile long time)
{
    for (; time>0; us_wait(), time--);
}
/*****************************************************************************
 PC Card Controller Initialization Function

*****************************************************************************/
ER PCCard_init(void)
{
    UH tmp;

    if (port_inw(SHPC_ID) != SHPC_DEVICE_ID)
        return E_ID;        /* Indicate Driver failure id not same device */

    port_outw(SHPC_MODE, MR_MODE_SELMODE);      /* Set MODE register */
   tmp = MR_ICR_EN_DETECT | MR_ICR_PSYS_IRQ;
    if (PCCard_check() == E_OK) {
        PCCard_pwr_ON();
    }
    else {
        PCCard_end();
        return E_OBJ;
    }

    wait(1000);
    tslp_tsk(500/MSEC);
    return E_OK;
}
/*****************************************************************************
 Routine to check if the PC card inserted in PCMCIA socket.

    Output -
            TRUE if CARD is inserted else FALSE.
*****************************************************************************/
ER PCCard_check(void)
{
    if (!(port_inw(SHPC_STATUS) & MR_STATUS_CARD_IN))
        return E_OK;
    else
        return E_OBJ;
}
/*****************************************************************************
 Routine to open PC card memory area for access by system.

*****************************************************************************/
void PCCard_open(void)
{
    UH tmp;

    /* Setup Memory Window0 */
    tmp = MRSHPC_MEM_WIN >> 20;
    tmp &= MR_START_ADDR;       /* Get SA25-SA20 Address bits */
    tmp |= MR_WIN_ENABLE | MR_WIN_TIMING;
    port_outw(SHPC_MW0CR1, tmp);

    tmp = MR_SIZE | MR_CREG;    /* Setup 16-bit Size and Common memory */
    port_outw(SHPC_MW0CR2, tmp);

    if(port_inw(SHPC_STATUS) & MR_STATUS_ENDIAN) {
        port_outw(SHPC_MW0CR2, tmp | MR_SWAP);  /* No SWAP if Little Endian */
    }

    /* Setup Memory Window1 */
    tmp = MRSHPC_ATR_WIN >> 20;
    tmp &= MR_START_ADDR;       /* Get SA25-SA20 Address bits */
    tmp |= MR_WIN_ENABLE | MR_WIN_TIMING;
    port_outw(SHPC_MW1CR1, tmp);

    tmp = MR_SIZE;              /* Setup 16-bit Size and Common memory */
    port_outw(SHPC_MW1CR2, tmp);

    if(port_inw(SHPC_STATUS) & MR_STATUS_ENDIAN) {
        port_outw(SHPC_MW1CR2, tmp | MR_SWAP);  /* No SWAP if Little Endian */
    }

    /* Setup IO Window */
    tmp = MRSHPC_IO_WIN >> 20;
    tmp &= MR_START_ADDR;       /* Get SA25-SA20 Address bits */
    tmp |= MR_WIN_ENABLE | MR_WIN_TIMING;
    port_outw(SHPC_IOWCR1, tmp);

    tmp = MR_SIZE;              /* Setup 16-bit Size  */
    port_outw(SHPC_IOWCR2, tmp);

    if(port_inw(SHPC_STATUS) & MR_STATUS_ENDIAN) {
        port_outw(SHPC_IOWCR2, tmp | MR_SWAP);  /* No SWAP if Little Endian */
    }

    port_outw(SHPC_CDCR, MR_CDCR_IO_CARD | MR_CDCR_EN_LED | MR_CDCR_EN_INPACK);
    wait(10000);
}
/*****************************************************************************
 Routine to switch ON the PC card power supply.

*****************************************************************************/
void PCCard_pwr_ON(void)
{
    UH  sts, tmp, tmp2;

    sts = port_inw(SHPC_STATUS);
    tmp = MR_CPWCR_CDPWR_MSK | MR_CPWCR_RSTCARD | MR_CPWCR_ENCARD
         | MR_CPWCR_VCCPWR | MR_CPWCR_AUTOPWR | MR_CPWCR_VPP0 | MR_CPWCR_VPP1;

    tmp2 = tmp;
    port_outw(SHPC_CPWCR, tmp);
    tslp_tsk(100/MSEC);

    if (sts & MR_STATUS_VS1 == 0)
        tmp2 |= MR_CPWCR_VCC3V;
    else
        tmp2 |= MR_CPWCR_VCC5V;

    port_outw(SHPC_CPWCR, tmp2);
    tslp_tsk(100/MSEC);
    port_outw(SHPC_CPWCR, tmp);
    tslp_tsk(100/MSEC);
    port_outw(SHPC_CPWCR, tmp2);

    wait(1000);
}
/*****************************************************************************
 Routine to switch OFF the PC card power supply.

*****************************************************************************/
ER PCCard_end(void)
{
    port_outw(SHPC_CPWCR, 0x0);     /* Turn Off the Card Power Supply */
    wait(1000);                     /* Wait for 1ms till supply off */
    return E_OK;
}
/*****************************************************************************
 Memory Read (word access)

*****************************************************************************/
UH  PCCard_mem_readw( UW addr )
{
    UH  data;
    data = port_inw( MRSHPC_MEM_WIN+addr );
    return  data;
}
/*****************************************************************************
 Memory Read (byte access)

*****************************************************************************/
UB  PCCard_mem_readb( UW addr )
{
    UB  data;
    data = port_in( MRSHPC_MEM_WIN+addr );
    return  data;
}
/*****************************************************************************
 Memory Write (word access)

*****************************************************************************/
void    PCCard_mem_writew( UW addr, UH data)
{
    port_outw( MRSHPC_MEM_WIN+addr , data );
}
/*****************************************************************************
 Memory Write (byte access)

*****************************************************************************/
void    PCCard_mem_writeb( UW addr, UB data)
{
    port_out( MRSHPC_MEM_WIN+addr , data );
}
/*****************************************************************************
 Attribute Read (word access)

*****************************************************************************/
UH  PCCard_atr_readw( UW addr )
{
    UH  data;
    data = port_inw( MRSHPC_ATR_WIN+addr );
    return  data;
}

/*****************************************************************************
 Attribute Read (byte access)

*****************************************************************************/
UB  PCCard_atr_readb( UW addr )
{
    UB  data;
    data = port_in( MRSHPC_ATR_WIN+addr );
    return  data;
}
/*****************************************************************************
 Attribute Write (word access)

*****************************************************************************/
void    PCCard_atr_writew( UW addr, UH data)
{
    port_outw( MRSHPC_ATR_WIN+addr , data );
}

/*****************************************************************************
 Attribute Write (byte access)

*****************************************************************************/
void    PCCard_atr_writeb( UW addr, UB data)
{
    port_out( MRSHPC_ATR_WIN+addr , data );
}

/*****************************************************************************
 I/O Read (word access)

*****************************************************************************/
UH  PCCard_io_inpw( UW addr )
{
    UH  data;
    data = port_inw( MRSHPC_IO_WIN+addr );
    return  data;
}

/*****************************************************************************
 I/O Read (byte access)

*****************************************************************************/
UB  PCCard_io_inpb( UW addr )
{
    UB  data;
    data = port_in( MRSHPC_IO_WIN+addr );
    return  data;
}

/*****************************************************************************
 I/O Write (word access)

*****************************************************************************/
void    PCCard_io_outw( UW addr, UH data)
{
    port_outw( MRSHPC_IO_WIN+addr , data );
}

/*****************************************************************************
 I/O Write (byte access)

*****************************************************************************/
void    PCCard_io_outb( UW addr, UB data)
{
    port_out( MRSHPC_IO_WIN+addr , data );
}

/************ End Of File (mrshpc01v2.c) ************************************/
