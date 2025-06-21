/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/ns8390.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ns8390.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* NS8390.C  Edition: 2  Last changed: 17-Feb-98,15:36:46  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    NS8390.C -- National Semiconductor 8390x Ethernet Driver for USNET

    Copyright (C) 1993, 1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

/* NET構造体のフィールドのうち、このドライバ内で独自に使用しているものの一覧 */
/* irno[0]    使用する割込み番号                                             */
/* irno[1]    次にパケットが受信されることが期待されるバッファアドレス       */

#include <string.h>
#include "net.h"
#include "local.h"
#include "driver.h"
#include "support.h"
#include "ethernet.h"
#include "ns8390.h"

/*#define PROMISCUOUS /* Accept and discard all packets. */
                      /* For testing receive interrupt. */

#define XFER 16			/* 8 or 16 bit transfers */
#define POOLSIZE 0x8000		/* buffer pool size */

#define SM_TSTART_PG 0x80		/* First page of TX buffer */
#define SM_RSTART_PG 0x86		/* Starting page of RX ring */
#define SM_RSTOP_PG  0xFF	 	/* Last page of RX ring */
#define SHAPAGE 256				/* shared memory page size */
#define MAXSIZ (MAXBUF - MESSH_SZ)

extern struct CONNECT connblo[];
extern struct NET nets[];

static unsigned long loopc;

#if NTRACE >= 6
void dumpmem(unsigned char *buf,int dmpsize)
{
    int i;

    for (i=0;i<dmpsize;i++) {
        if ((i & 0xf) == 0) {
            Nprintf("\n%04x : ",i);
        }
        Nprintf("%02x ",*buf++);
    }
    Nprintf("\n");
}
#endif

/* ==========================================================================
   C level interrupt handler for Ethernet.  Called from a stub, registers are
   saved.  Returns to the interrupt stub.  Queues the arrived message into
   the arrive queue of the network block.  */

static void irhan(int netno)
{
    int status, i2;
    unsigned long tport;
    unsigned short start,next,len,curr;
    unsigned long ul1;
    char *spt;
    MESS *mess;
    struct NET *netp;

/* get and clear interrupt status */

    netp = &nets[netno];
    tport = netp->port;
/*    _outw(tport+CMDR, MSK_PG0); /**/
/*    _outw(tport+IMR, 0x00);			/* disable chip interrupts */
aga:
    status = _inw(tport+ISR);

#if NTRACE >= 5
Nprintf("NS8390 irhan : ISR = 0x%04X\n",status); /**/
#endif
/* receive interrupt handling */

    if (status & MSK_OVW) {		/* ring overwrite */
/* Recovery from ring overwrite requires a reset.  Spending this time in an
   interrupt handler may be unacceptable.  Ring overwrite may be a sign of
   some other problem.
*/
#if NTRACE >= 1
Nprintf("irhan : ringbuffer overwrite\n"); /**/
#endif
        _outw(tport+CMDR, MSK_STP + MSK_RD2);	/* stop 8390 */
        for (ul1=0; ul1<loopc; ul1++) ;		/* 2 ms wait */
        _outw(tport+RBCR0, 0);			/* clear byte count */
        _outw(tport+RBCR1, 0);
        status |= _inw(tport+ISR) & (MSK_PTX+MSK_TXE);	/* TX status */
        _outw(tport+TCR, MSK_LOOP);		/* loopback mode */
        _outw(tport+CMDR, MSK_STA + MSK_RD2);	/* start */
    }

    if (status & MSK_RXE) {
        _outw(tport+ISR, MSK_RXE);		/* clear status */
        i2 = _inw(tport+RSR) >> 8;
#if NTRACE >= 1
        Nprintf("irhan : Receive error. ISR/RSR = 0x%02X/0x%02X\n",(status >> 8),i2);
#endif
        netp->ifInErrors++;
    }

    if (status & MSK_TXE) {
        _outw(tport+ISR, MSK_TXE);		/* clear status */
        i2 = _inw(tport+TSR) >> 8;
#if NTRACE >= 1
        Nprintf("irhan : Transmit error. ISR/TSR = 0x%02X/0x%02X\n",(status >> 8),i2);
#endif
        netp->ifOutErrors++;
    }

    while (status & MSK_PRX) {
        _outw(tport+ISR, MSK_PRX);
/*        start = netp->irno[1]; */

        start = (_inw(tport+BNRY)+0x100) & 0xff00;
/*
        if (start == 0xff00) {
            start = (unsigned short)SM_RSTART_PG << 8;
        } else {
            start += 0x0100;
        }
*/
#if NTRACE >= 5
Nprintf("irhan : start page = 0x%04X\n",start); /**/
#endif

        _outw(tport+RBCR0, 0x400);
        _outw(tport+RBCR1, 0);
        _outw(tport+RSAR0, 0);
        _outw(tport+RSAR1, start);
        start >>= 8;
        _outw(tport+CMDR, MSK_PG0+MSK_RRE+MSK_STA);

        i2 = _inw(tport+DATAPORT);
        len = _inw(tport+DATAPORT);
#ifndef LITTLE
        len = SWAPS(len);
#endif
/*        len -= 4; */
        next = i2 >> 8;
#if NTRACE >= 5
Nprintf("irhan : next/status = 0x%04X length = 0x%04X\n",i2,len);
#endif

        if (next < SM_RSTART_PG) {
#if NTRACE >= 1 /**/
Nprintf("irhan : illegal value in next pointer.\n"); /**/
Nprintf("irhan : start page = 0x%04X\n",start); /**/
Nprintf("irhan : next/status = 0x%04X length = 0x%04X\n",i2,len); /**/
#endif /**/
/*            _outw(tport+BNRY, start << 8); */
            netp->ifInErrors++;
/*            break; /**/
/*            goto lab12; */
/*            next = start+1;   /* 悪あがき */
/*            if (next > SM_RSTOP_PG) next = SM_RSTART_PG; /**/
            break;
        }

        if ((i2 & SMK_PRX) == 0 || len > MAXBUF - MESSH_SZ) {
#if NTRACE >= 1
Nprintf("irhan : error in status register or length overflow.\n"); /**/
Nprintf("irhan : start page = 0x%04X\n",start); /**/
Nprintf("irhan : next/status = 0x%04X length = 0x%04X\n",i2,len); /**/
#endif
            netp->ifInErrors++;
/*            next = start+1;   /* 悪あがき */
/*            if (next > SM_RSTOP_PG) next = SM_RSTART_PG; /**/
            goto lab11;
        }

        if (QUEUE_FULL(netp, arrive)) {
#if NTRACE >= 1
Nprintf("irhan : queue full.\n");
#endif
            goto lab4;
        }

        if ((mess = NgetbufIR()) == 0)
        {
#if NTRACE >= 1
Nprintf("irhan : no buffer.\n");
#endif
lab4:       netp->ifInDiscards++;
            goto lab11;
        }

#if NTRACE >= 5
Nprintf("irhan : allcate buffer, address = 0x%08lx\n",mess);
#endif
 
        mess->mlen = len + MESSH_SZ;
        mess->offset = MESSH_SZ;
        spt = (char *)mess + MESSH_SZ;

        if ((i2 = (SM_RSTOP_PG - start + 1) * SHAPAGE - 4) < len) {
#if NTRACE >= 5
Nprintf("irhan : ring buffer wrap-around, read %d bytes.\n",i2);
#endif
            _outw(tport+RBCR0, 0xfc00);
            _outw(tport+RBCR1, i2 & 0xff00);
            _outw(tport+CMDR, MSK_RRE+MSK_PG0+MSK_STA);
            len -= i2;
            BLOCKIN(spt, tport+DATAPORT, i2);
            spt += i2;
            _outw(tport+RSAR1, SM_RSTART_PG << 8);
        }
#if NTRACE >= 5
Nprintf("irhan : read %d bytes.\n",len);
#endif
        _outw(tport+RBCR0, len << 8);
        _outw(tport+RBCR1, len & 0xff00);
        _outw(tport+CMDR, MSK_PG0+MSK_RRE+MSK_STA);
        BLOCKIN(spt, tport+DATAPORT, len);
#if NTRACE >= 6
        dumpmem((unsigned char *)mess+MESSH_SZ,(len > 128) ? 128 : len);
#endif
        mess->netno = netno;
#ifdef PROMISCUOUS
        mess->id = bALLOC;
        NrelbufIR(mess);
#else
        QUEUE_IN(netp, arrive, mess);
#endif
        WAITNOMORE_IR(SIG_RN(netno));

lab11:  _outw(tport+BNRY, (next-1) << 8);
/*        netp->irno[1] = next << 8; /**/
        _outw(tport+CMDR, MSK_PG1+MSK_RD2+MSK_STA);
        curr = _inw(tport+CURR) >> 8;
        _outw(tport+CMDR, MSK_PG0+MSK_RD2+MSK_STA);
        if (curr == next) {
            break;
        }
#if NTRACE >= 5
Nprintf("irhan : try next buffer, CURR/next = 0x%04X/0x%04X\n",curr,next);
#endif
    }
lab12:

    if (status & MSK_OVW) {		/* ring overwrite */
#if NTRACE >= 1
        Nprintf("irhan : Ring OverWrite. ISR = 0x%02X\n",(status >> 8));
#endif
        _outw(tport+ISR, MSK_OVW);	/* reset IR */
        _outw(tport+TCR, 0);		/* out of loopback */
        if (netp->hwflags && (status & (MSK_PTX+MSK_TXE)) == 0)
            _outw(tport+CMDR, MSK_TXP+MSK_RD2+MSK_STA);	/* resend */
    }

/* transmit interrupt, send out the next message from the departure queue */

    if (status & MSK_PTX) {
        _outw(tport+ISR, MSK_PTX);
        i2 = _inw(tport+TSR);
#if NTRACE >= 1
        if (i2 & 0xfc00) {
            Nprintf("irhan : TSR status error (0x%04x)\n",i2);
        }
#endif
lab6:   if (QUEUE_EMPTY(netp, depart)) {
            netp->hwflags = 0;
        } else {
            QUEUE_OUT(netp, depart, mess);
            if (mess->offset != netno)
                goto lab6;
            spt = (char *)mess + MESSH_SZ;
            len = mess->mlen - MESSH_SZ;
            if (len < ET_MINLEN) len = ET_MINLEN;

            /* dummy read */
            _outw(tport+CMDR, MSK_RD2+MSK_STA);
            _outw(tport+RBCR0, 2);
            _outw(tport+RBCR1, 0);
            _outw(tport+RSAR0, 0xfe00);
            _outw(tport+RSAR1, (SM_TSTART_PG-1) << 8);
            _outw(tport+CMDR, MSK_PG0+MSK_RRE+MSK_STA);
            i2 = _inw(tport+DATAPORT);

            _outw(tport+CMDR, MSK_RD2+MSK_STA);
            _outw(tport+RBCR0, len << 8);
            _outw(tport+RBCR1, len & 0xff00);
            _outw(tport+RSAR0, 0);
            _outw(tport+RSAR1, SM_TSTART_PG << 8);
            _outw(tport+CMDR, MSK_PG0+MSK_RWR+MSK_STA);
#if NTRACE >= 6
            dumpmem((unsigned char *)mess+MESSH_SZ,(len > 128) ? 128 : len);
#endif
            BLOCKOUT(spt, tport+DATAPORT, len);

            mess->offset = boTXDONE;
            _outw(tport+TBCR0, len << 8);
            _outw(tport+TBCR1, len & 0xff00);
            _outw(tport+TPSR, SM_TSTART_PG << 8);
            _outw(tport+CMDR, MSK_TXP+MSK_RD2+MSK_STA);
            if (mess->id <= bWACK) {
                if (mess->id == bRELEASE) {
                    mess->id = bALLOC;
                    NrelbufIR(mess);
                }
            } else {
                WAITNOMORE_IR(SIG_WN(netno));
            }
        }
    }
/* this ugly check is needed for edge-triggered IR controllers
*/
    if (_inw(tport+ISR) & (MSK_PRX + MSK_RXE + MSK_PTX + MSK_TXE))
        goto aga;

/*    _outw(tport+IMR, 0x1700);			/* enable chip interrupts */
}


/* ==========================================================================
   Transmit routine.  If the transmitter is idle, starts the transmission and
   returns.  Otherwise adds message to the departure queue; the interrupt
   handler will trasmit it.  Returns:
    error:    -1
    success:  0
 */

static int writE(int conno, MESS *mess)
{
    int len, stat;
    unsigned long tport;
    char *spt;
    struct NET *netp;
    unsigned short uh1;

    netp = &nets[mess->netno];
    tport = netp->port;
    mess->offset = mess->netno;

    DISABLE();
    if (netp->hwflags)
    {
        if (QUEUE_FULL(netp, depart)) {
#if NTRACE >= 1
            Nprintf("NS8390 writE : tx queue full. discard packet.\n");
#endif
            netp->ifOutDiscards++;
            mess->offset = boTXDONE;
            stat = ENOBUFS;
        } else {
#if NTRACE >= 5
            Nprintf("NS8390 writE : tx busy. queue packet.\n");
#endif
            QUEUE_IN(netp, depart, mess);
            stat = 0;
        }
        ENABLE();
    }
    else
    {
        _outw(tport+IMR, 0x00);			/* disable chip interrupts */
        netp->hwflags = 1;
        ENABLE();
        spt = (char *)mess + MESSH_SZ;
        len = mess->mlen - MESSH_SZ;
        if (len < ET_MINLEN) len = ET_MINLEN;

        /* dummy read */
        _outw(tport+CMDR, MSK_RD2+MSK_STA);
        _outw(tport+RBCR0, 2);
        _outw(tport+RBCR1, 0);
        _outw(tport+RSAR0, 0xfe00);
        _outw(tport+RSAR1, (SM_TSTART_PG-1) << 8);
        _outw(tport+CMDR, MSK_RRE+MSK_STA);
        stat = _inw(tport+DATAPORT);
/*
        Nprintf("writE : waiting CRDA0...");
        while ((_inw(tport+CRDA0) & 0xff00) == 0xfe00);
        Nprintf("end.\n");
*/
        _outw(tport+CMDR, MSK_RD2+MSK_STA);
        _outw(tport+RBCR0, len << 8);
        _outw(tport+RBCR1, len & 0xff00);
        _outw(tport+RSAR0, 0);
        _outw(tport+RSAR1, SM_TSTART_PG << 8);	/**/
        _outw(tport+CMDR, MSK_RWR+MSK_STA);
/*
    uh1 = _inw(tport+CMDR);
    Nprintf("writE : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CMDR);
    Nprintf("writE : CMDR = 0x%04X\n",uh1);
*/
#if NTRACE >= 6
        dumpmem((unsigned char *)mess+MESSH_SZ,(len > 128) ? 128 : len);
#endif
        BLOCKOUT(spt, tport+DATAPORT, len);

        mess->offset = boTXDONE;
        _outw(tport+TBCR0, len << 8);
        _outw(tport+TBCR1, len & 0xff00);
        _outw(tport+TPSR, SM_TSTART_PG << 8);
        _outw(tport+CMDR, MSK_TXP+MSK_RD2+MSK_STA);

/*
    uh1 = _inw(tport+CMDR);
    Nprintf("writE : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CMDR);
    Nprintf("writE : CMDR = 0x%04X\n",uh1);
*/
        stat = 1;
        _outw(tport+IMR, 0x1700);			/* enable chip interrupts */
    }
    return stat;
}

/* ==========================================================================
   Open a connection. */

static int opeN(int conno, int flag)
{
    (void)conno, (void)flag;
    return 0;
}


/* ==========================================================================
   Close the connection. */

static int closE(int conno)
{
    (void)conno;
    return 0;
}


/* ==========================================================================
   Configure and start up the Ethernet interface.  We process the user-level
   text parameters and store the values into the net table.  We take the
   Ethernet address, and set up the controller.  Then we store the interrupt
   address and enable the interrupt.  Returns:
    error:    -1
    success:   0
 */

static int init(int netno, char *params)
{
    int i1, i2, i3;
    unsigned long tport;
    short *spt;
    char *cp1, par[16], val[16];
    unsigned long ul1;
    struct NET *netp;
    unsigned short uh1;

    netp = &nets[netno];
#if 0
    for (cp1=params; *cp1; )
    {
        Nsscanf(cp1, "%[^=]=%s %n", par, val, &i1);
        cp1 += i1;
        if (strcmp(par, "ENA") == 0)
            Nsscanf(val, "%i", &netp->base[0]);
    }
#endif

    i1 = netp->protoc[2]->init(netno, params);
    if (i1 < 0)
        return i1;
    tport = netp->port = REGPORT;
    netp->irno[0] = INTNO;
    netp->bps = 10000000;

    spt = (short *)&netp->id;			/* get Ethernet address */
    if ((spt[0] | spt[1] | spt[2]) == 0)	/* unless configured */
        memcpy(netp->id.c, (char *)netp->base[0], Eid_SZ);

    _outw(tport+RESET,0xffff);
/*    _outw(tport+CMDR, MSK_STP|MSK_RD2);	/* stop the controller */
    _outw(tport+CMDR, MSK_STP);	/* stop the controller */

#ifdef LITTLE
    _outw(tport+DCR, MSK_WTS|MSK_BMS|MSK_FT10);		/* configure */
/*    _outw(tport+DCR, MSK_FT10);		/* configure (loopback) */
#else
    _outw(tport+DCR, MSK_WTS|MSK_BOS|MSK_BMS|MSK_FT10);		/* configure */
/*    _outw(tport+DCR, MSK_BOS|MSK_FT10);		/* configure (loopback) */
#endif

    _outw(tport+RBCR0,0);			/* clear RBCR */
    _outw(tport+RBCR1,0);
#ifdef PROMISCUOUS
    _outw(tport+RCR,MSK_PRO);			/* promiscuous */
#else
    _outw(tport+RCR,MSK_AB);			/* accept broadcast */
#endif
/*    _outw(tport+RCR,MSK_PRO|MSK_AM|MSK_AB|MSK_SEP);			/* accept all */
    _outw(tport+TCR, MSK_LOOP);			/* loopback mode 1 */
    _outw(tport+BNRY, (SM_RSTART_PG-1) << 8);
    _outw(tport+PSTART, SM_RSTART_PG << 8);
    _outw(tport+PSTOP, SM_RSTOP_PG << 8);
/*    for (i2=0; i2<1000; i2++);			/* delay */
    _outw(tport+ISR, 0xff00);			/* clear and mask interrupts */
    _outw(tport+IMR, 0x1700);			/* enable interrupts */
    _outw(tport+CMDR, MSK_PG1+MSK_RD2);		/* initial physical addr */
/*
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
*/
    _outw(tport+CURR, SM_RSTART_PG << 8);		/* current RX page */
    for (i1=0; i1<Eid_SZ; i1++)
        _outw(tport+PAR+i1*2, (unsigned short)netp->id.c[i1] << 8);
    for (i1=0; i1<MARsize; i1++)		/* clear multicast */
        _outw(tport+MAR+i1*2, 0);
    netp->irno[1] = SM_RSTART_PG << 8;
    _outw(tport+CMDR, MSK_STA+MSK_RD2);		/* put 8390 on line */
/*
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
*/
    _outw(tport+TCR, 0);			/* normal operation */
/*    _outw(tport+TCR, 0x0400);			/* internal loopback operation */

/* calibrate a delay loop for 2 milliseconds */
    loopc = 1024;
    for (i1=(int)TimeMS(); i1==(i2=(int)TimeMS()); ) ;
    for (i3=0; i2==(i1=(int)TimeMS()); i3++)
        for (ul1=0; ul1<loopc; ul1++) ;
    loopc = (unsigned long)i3 * (unsigned long)(2048/(i1-i2));

/*
    _outw(tport+CMDR, MSK_PG1+MSK_RD2+MSK_STA);
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);
    uh1 = _inw(tport+CURR);
    Nprintf("init : CURR = 0x%04lX\n",uh1);
    uh1 = _inw(tport+CURR);
    Nprintf("init : CURR = 0x%04lX\n",uh1);
    uh1 = _inw(tport+CURR);
    Nprintf("init : CURR = 0x%04lX\n",uh1);

    for (i1=0; i1<Eid_SZ; i1++) {
        uh1 = _inw(tport+PAR+i1*2);
        Nprintf("init : PAR%d = 0x%04X\n",i1,uh1);
    }
    _outw(tport+CMDR, MSK_RD2+MSK_STA);
    uh1 = _inw(tport+CMDR);
    Nprintf("init : CMDR = 0x%04X\n",uh1);

    uh1 = _inw(tport+BNRY);
    Nprintf("init : BNRY = 0x%04lX\n",uh1);
    uh1 = _inw(tport+TSR);
    Nprintf("init : TSR = 0x%04lX\n",uh1);
    uh1 = _inw(tport+ISR);
    Nprintf("init : ISR = 0x%04lX\n",uh1);
    uh1 = _inw(tport+RSR);
    Nprintf("init : RSR = 0x%04lX\n",uh1);
*/

    IRinstall(netp->irno[0], netno, irhan);
#if NTRACE >= 1
    Nprintf("NS8390 %02x%02x%02x%02x%02x%02x IR%d P%x\n", netp->id.c[0],
        netp->id.c[1], netp->id.c[2], netp->id.c[3], netp->id.c[4],
        netp->id.c[5], netp->irno[0], tport); 
#endif
    return 0;
}


/* ==========================================================================
   Shut down the Ethernet interface.  Restores original IRQ, mask and vector.
   Turns off the controller. */

static void shut(int netno)
{
    struct NET *netp;

    netp = &nets[netno];
    IRrestore(netp->irno[0]);
    _outw(netp->port+CMDR, MSK_STP + MSK_RD2);
    netp->protoc[2]->shut(netno);
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
PTABLE ussNS8390Table = {"NS8390", init, shut, 0, opeN, closE, 0, writE,  ioctl, 0, MESSH_SZ};
