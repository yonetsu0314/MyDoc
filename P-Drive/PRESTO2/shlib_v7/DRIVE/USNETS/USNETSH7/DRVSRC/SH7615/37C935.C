/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/37c935.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: 37c935.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "driver.h"
#include "support.h"

#define SMC_CONFIG	0x220007E0	/* SMC37C935 Configration port */
#define SMC_DATA	0x220007E2	/* SMC37C935 Configration data port */

#define SIO0_BASE	0x220007F0	/* SIO0 base address */
#define THR     0x0
#define RDR     0x0
#define BRDL    0x0
#define BRDH    0x2
#define IER     0x2
#define IIR     0x4
#define FCR     0x4
#define LCR     0x6
#define MCR     0x8
#define LSR     0xa
#define MSR     0xc

#define SIO0_IRLNO	8

#define LED		0x22200000	/* on-board LED */


extern struct NET nets[];


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  irhan()      Interrupt handler
**
**  void irhan(int netno);
**
**  PARAMETERS:
**     (in)     netno   Index into nets[] array
**
**  DESCRIPTION:  
**     Process interrupts from the serial interface.
**     On a receive interrupt, a character is read in for processing
**     by the upper layers of the stack.
**     If there is room, get another character from a higher layer and
**     write it out.
**
**  USAGE/COMMENTS:  
**     This function is not called directly but is activated when the
**     serial interface generates an interrupt.  Installation of this
**     handler is processor specific.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void irhan(int netno)
{
    int i1;
    unsigned int tport;
    unsigned short status;
    struct NET *netp;

    netp = &nets[netno];
    tport = netp->port;

    while ((status = _inw(tport+IIR) & 0x0700) != 0x0100)
    {
        switch (status)
        {
        case 0:                         /* MODEM status change */
            _inw(tport+MSR);
            break;
        case 0x0200:                         /* THR empty */
txaga:      i1 = netp->hw.goingc(netp);
            if (i1 != -1)
            {
                _outw(tport+THR, i1 << 8);
                if (_inw(tport+LSR) & 0x2000)
                    goto txaga;
            }
            else
                _outw(tport+IER, _inw(tport+IER) & 0x0d00);
            break;
        case 0x0400:                         /* received data available */
rxaga:      netp->hw.comec(_inw(tport+RDR) >> 8, netp);
            if (_inw(tport+LSR) & 0x0100)
                goto rxaga;
            break;
        case 0x0600:                         /* Line status change */
            _inw(tport+LSR);
            break;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  writE()      Transmit function
**
**  int writE(int conno, MESS *mess);
**
**  PARAMETERS:
**     (in)     conno  Not used in this function
**     (in)     mess   Pointer to message to be written
**
**  RETURNS:
**     0        Always returns 0
**
**  DESCRIPTION:  
**     Queue message for transmission by serial protocol layer and wake
**     up serial interface.
**
**  USAGE/COMMENTS:  
**     Called indirectly using pointer defined in PTABLE
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int writE(int conno, MESS *mess)
{
    int tport;
    struct NET *netp;

    (void)conno;
    netp = &nets[mess->netno];
    tport = netp->port;
    mess->offset = mess->netno;
    BLOCKPREE();
    QUEUE_IN(netp, depart, mess);
    _outw(tport+IER, _inw(tport+IER) | 0x0200);
    RESUMEPREE();
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  opeN()      Open a connection
**
**  int opeN(int conno, int flag);
**
**  PARAMETERS:
**     (in)     conno  Not used in this function
**     (in)     flag   Not used in this function
**
**  RETURNS:
**     0         always returns 0
**
**  DESCRIPTION:  
**     Program stub 
**
**  USAGE/COMMENTS:  
**     Maintains PTABLE format consistency with other stack layers
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int opeN(int conno, int flag)
{
    (void)conno, (void)flag;
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  closE()      Close the connection
**
**  int closE(int conno);
**
**  PARAMETERS:
**     (in)     conno  Not used in this function
**
**  RETURNS:
**     0         always returns 0
**
**  DESCRIPTION:  
**     Program stub 
**
**  USAGE/COMMENTS:  
**     Maintains PTABLE format consistency with other stack layers
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int closE(int conno)
{
    (void)conno;
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  init()      Initialize serial interface
**
**  int init(int netno, char *params);
**
**  PARAMETERS:
**     (in)     netno   Index into nets[] array
**     (in)     params  Pointer to parameter list
**
**  RETURNS:
**     0            Initialization successful
**     NE_PARAM     Bad parameters
**
**  DESCRIPTION:  
**     Configure and start up the serial interface.  Process the
**     user-level text parameters and store the values into the net
**     table.  Store the interrupt address and enable the interrupt.
**
**  USAGE/COMMENTS:  
**     Called indirectly using pointer defined in PTABLE
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  20-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int init(int netno, char *params)
{
    int i1, tport;
    long l1;
    char *cp1, par[16], val[16];
    struct NET *netp;

    netp = &nets[netno];
    for (cp1=params; *cp1; )
    {
        Nsscanf(cp1, "%[^=]=%s %n", par, val, &i1);
        cp1 += i1;
        if (strcmp(par, "CLOCK") == 0)
            Nsscanf(val, "%ld", &l1);
        else if (strcmp(par, "BAUD") == 0)
            Nsscanf(val, "%ld", &netp->bps);
    }

    i1 = netp->protoc[2]->init(netno, params);
    if (i1 < 0)
        return i1;

    netp->irno[0] = SIO0_IRLNO;
    netp->port = SIO0_BASE;
    tport = netp->port;

    _outw(SMC_CONFIG,0x5500);	/* enter to configuration mode */
    _outw(SMC_CONFIG,0x5500);

    _outw(SMC_CONFIG,0x2200);	/* power control */
    _outw(SMC_DATA,  0x0100);	/* com1 power on */

    _outw(SMC_CONFIG,0x2300);	/* power management */
    _outw(SMC_DATA,  0x0000);	/* power management off */

    _outw(SMC_CONFIG,0x0700);	/* Logical device select */
    _outw(SMC_DATA,  0x0400);	/* select com1 */

    _outw(SMC_CONFIG,0x3000);	/* activate */
    _outw(SMC_DATA,  0x0100);	/* activate selected device */

    _outw(SMC_CONFIG,0x6000);	/* I/O base address */
    _outw(SMC_DATA,  0x0300);
    _outw(SMC_CONFIG,0x6100);	/* I/O base address */
    _outw(SMC_DATA,  0xf800);	/* base addr = 0x3f8 (0x220007f0) */

    _outw(SMC_CONFIG,0x7000);	/* Interrupt request level 0 */
    _outw(SMC_DATA,  0x0400);	/* This must be set to com4...why? */

    _outw(SMC_CONFIG,0xaa00);	/* exit from configuration mode */

    _outw(tport+IER, 0x0000);           /* disable interrupt */
    _outw(tport+LCR, 0x8000);           /* set baud reg access */
    i1 = l1 / netp->bps;                /* set baud rate */
    _outw(tport+BRDH, i1 & 0xff00);
    _outw(tport+BRDL, i1 << 8);
    _outw(tport+LCR, 0x0300);           /* set LCR value */
    _outw(tport+FCR, 0xc700);           /* set FCR value for 16550 */
    _outw(tport+MCR, 0x0b00);           /* set MCR value */
    (void)_inw(tport+LSR);              /* clear any line status interrupt */
    (void)_inw(tport+RDR);              /* clear any receive interrupt */
    (void)_inw(tport+IIR);              /* clear any transmitter interrupt */
    (void)_inw(tport+MSR);              /* clear any modem status interrupt */

#if NTRACE >= 5
    Nprintf("37C935 : install interrupt to IRL%d\n",netp->irno[0]);
#endif

    IRLinstall(netp->irno[0], netno, irhan);
    _outw(tport+IER, 0);                /* some clones require this */
    _outw(tport+IER, 0x0300);

#if NTRACE >= 1
    Nprintf("37C935 IR%d P%x BPS%ld\n", netp->irno[0], tport, netp->bps);
#endif
    return 0;
err1:
    return NE_PARAM;
err2:
    return NE_HWERR;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  shut()      Shut down controller
**
**  void shut(int netno);
**
**  PARAMETERS:
**     (in)     netno   Index into nets[] array
**
**  RETURNS:
**
**  DESCRIPTION:  
**     Restore the interrupt and turn off the controller
**
**  USAGE/COMMENTS:  
**     Called indirectly using pointer defined in PTABLE
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void shut(int netno)
{
    int tport;
    struct NET *netp;

    netp = &nets[netno];
    tport = netp->port;
    while (!(_inw(tport+LSR) & 0x4000));
    _outw(tport+IER, 0);
    _outw(tport+MCR, 0);
    IRrestore(netp->irno[0]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg);
**
**  PARAMETERS:
**     (in)     handle   likely to be netno if any functions are added
**                       at this level
**     (in)     request  constant for the command to perform
**     (in)     arg      pointer to the parameters to the command, which
**                       will be specific to the command
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.  At the moment, there are no functions
**     defined for this layer.
**
**  RETURNS:
**     0              Success
**     USS_ERR_INVAL  Support for the request not implemented
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */

    return ussErrInval;
}


/* ===========================================================================
   Protocol table for the driver. */
GLOBALCONST
PTABLE uss37C935Table = {
    "37C935", init, shut, 0, opeN, closE, 0, writE, ioctl,
        0, MESSH_SZ
};
