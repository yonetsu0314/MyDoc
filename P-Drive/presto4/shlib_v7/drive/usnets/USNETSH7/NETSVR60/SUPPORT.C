/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/support.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: support.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* SUPPORT.C  Edition: 11  Last changed: 12-Nov-99,11:22:48  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    SUPPORT.C -- Environment Support for USNET

    Copyright (C) 1993, 1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    http://www.ussw.com
    503.844.6614  

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without 
    notice and should not be construed as a commitment by United
    States Software Corporation.
    
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *

  MODFICATION HISTORY:

  12-NOV-1999  TTT  Took ; off of MTsupportRoutines macro call
  27-SEP-1999  BTS  Fixed previous alignment changes
  31-AUG-1999  BTS  Added alignment to Ninitbuf.
  22-MAR-1999  NJW  Added mp->next=NULL to Ngetbuf() routines; Detabbed

* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <stddef.h>    /* for size_t typedef */
#include "net.h"
#include "local.h"
#include "driver.h"
#include "support.h"

#if NTRACE > 0
void Npanic(char *text)
{
#if 0
    while(*text)
        Nputchr(*text++);
    Nputchr('\n');
    for (;;) ;
#else
	Nprintf(text);
/*    for (;;) ;*/
#endif
}
#endif

unsigned int clocks_per_sec;
int Nclocktick;
static int ticktype;
static short coef;
static unsigned long basems;
static unsigned long baseticks;
static unsigned long basewrap;
static unsigned int wrapcount;
static unsigned long lastticks;
static unsigned short portnumber;

#ifdef MTsupportRoutines
/* Multitaskers may need some little support routines of their own.  These
   would be defined with a macro in mtmacro.h, and placed into the following
   location. Note that MTsupportRoutines is a macro, not a function.
*/
MTsupportRoutines()
#endif

/* As we assume there is no floating point, we reject some values of clock
   frequency as impractical to handle exactly.  You can adjust the code in
   these situations, perhaps to use floating point, or perhaps to use a
   rounded value of clocks_per_sec/1000 with ticktype = 2. */

int Ninitsupp()
{
    unsigned long ul1;

    ul1 = clocks_per_sec;                /* set in local.h or clk_init() */
    if (ul1 == 18)
    {
        ticktype = 0;
        coef = 55;
    }
    else if (ul1 == 1000)
    {
        ticktype = 1;
    }
    else if ((ul1 % 1000) == 0)
    {
        ticktype = 2;
        coef = ul1 / 1000;
    }
    else if (ul1 <= 0xffff)
    {
        ticktype = 3;
        coef = ul1;
    }
    else
        return NE_PARAM;
    basems = baseticks = basewrap = wrapcount = lastticks = 0;
    Nclocktick = 1000 / ul1;
    portnumber = (unsigned short)Nclock() | 1024;
    return 0;
}

unsigned long TimeMS()
{
    unsigned long y0, y1, v1, x0, x1, quot, rem;

    y1 = Nclock(); 
    if (y1 < lastticks)
        if (lastticks - y1 > 1000)
            wrapcount++;
    lastticks = y1;
    y0 = wrapcount;
    if (y1 < baseticks)
        y0--;
    y1 -= baseticks;
    y0 -= basewrap;
    switch (ticktype)
    {
    case 0:                        /* PC: time = 55 * ticks */
        v1 = y1 << 6;
        v1 -= y1 + (y1 << 3);
        break;
    case 1:                        /* milliseconds = ticks */
    default:
        v1 = y1;
        break;
    case 3:                        /* milliseconds = (1000 * ticks) / N */
        x1 = y1 << 3;
        x0 = (y0 << 3) + (y1 >> 29);
        y0 = (y0 << 10) | (y1 >> 22);
        rem = y1 << 10;
        if (rem < x1)
            y0--;
        y1 = rem - x1;
        y0 -= x0;
        x0 = (x0 << 1) | (x1 >> 31);
        x1 <<= 1;
        if (y1 < x1)
            y0--;
        y1 -= x1;
        y0 -= x0;
    case 2:                        /* milliseconds = ticks / N */
        if (coef == 64)
            v1 = (y1 >> 6) | (y0 << 26);
        else
        {
            rem = (y1 >> 16) | (y0 << 16);
            quot = rem / coef;
            rem = rem % coef;
            v1 = quot << 16;
            v1 += ((rem << 16) | (y1 & 0xffff)) / coef;
        }
        break;
    }
    return v1 + basems;
}

void SetTimeMS(unsigned long val)
{
    baseticks = Nclock();
    if (baseticks < lastticks)
        wrapcount++;
    basewrap = wrapcount;
    basems = val;
}

/*
**  Pointers to USNET fixed block buffers
*/
static MESS *Nfirstbuf;      /* Pointer to first available USNET msg buff */
MESS *Nbufbase;              /* Pointer to USNET buffer pool bufpool[] */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  NgetbufIR()         Get a USNET fixed block buffer from an ISR
**
**  MESS *NgetbufIR( void );
**
**  PARAMETERS:
**     N/A
**
**  RETURNS:
**     NULL             No buffer available
**     != NULL          Pointer to the buffer
**
**  DESCRIPTION:  
**     NgetbufIR() returns a pointer to a buffer from the USNET fixed 
**     block buffer pool, bufpool[].  If the buffer's ID has any value 
**     other than bFREE, the routine will call Npanic().  This indicates
**     a buffer corruption problem.
**
**  USAGE/COMMENTS:  
**     This routine assumes interrupts are disabled and should only
**     be used within an interrupt handler like irhan() of USNET
**     serial and Ethernet drivers.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-MAR-1999  NJW  Added header; NULL terminated mp->next
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
MESS *NgetbufIR(void)                        /* from interrupt level */
{
    register MESS *mp;
    
    if ((mp = Nfirstbuf) == 0)
        return 0;
    Nfirstbuf = mp->next;
#if NTRACE > 0
    if (mp->id != bFREE)
        Npanic("NgetbufIR id");
#endif
    mp->id = bALLOC;
    mp->next = 0;                 /* NULL terminate buffer link */
    return mp;
}  /* end NgetbufIR() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Ngetbuf()           Get a USNET fixed block buffer
**
**  MESS *Ngetbuf( void );
**
**  PARAMETERS:
**     N/A
**
**  RETURNS:
**     NULL             No buffer available
**     != NULL          Pointer to the buffer
**
**  DESCRIPTION:  
**     Ngetbuf() returns a pointer to a buffer from the USNET fixed 
**     block buffer pool, bufpool[].  If the buffer's ID has any 
**     value other than bFREE, the routine will call Npanic().  This 
**     indicates a buffer corruption problem.
**
**  USAGE/COMMENTS:  
**     This routine will diable interrupts and re-enable them while
**     it acquires a buffer from the free buffer linked list.  It
**     is called throughout USNET.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-MAR-1999  NJW  Added header; NULL terminated mp->next
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
MESS *Ngetbuf(void)                        /* from main level */
{
    register MESS *mp;
    
    DISABLE();
    if ((mp = Nfirstbuf) == 0)
        goto lab3;
    Nfirstbuf = mp->next;
#if NTRACE > 0
    if (mp->id != bFREE)
        Npanic("Ngetbuf id");
#endif
    mp->id = bALLOC;
    mp->next = 0;                 /* NULL terminate buffer link */
lab3:

    ENABLE();
    return mp;
}  /* end Ngetbuf() */

/* release message buffer */

void NrelbufIR(MESS *mp)                        /* from interrupt level */
{
#if NTRACE > 0
    if (mp->id != bALLOC && mp->id != bRELEASE)
        Npanic("NrelbufIR id");
#endif
    mp->id = bFREE;
    mp->offset = boRELEASED;
    mp->next = Nfirstbuf;
    Nfirstbuf = mp;
}

void Nrelbuf(MESS *mp)                                /* from main level */
{
#if NTRACE > 0
    if (mp->id != bALLOC && mp->id != bRELEASE)
        Npanic("Nrelbuf id");
#endif
    mp->id = bFREE;
    mp->offset = boRELEASED;
    DISABLE();
    mp->next = Nfirstbuf;
    Nfirstbuf = mp;
    ENABLE();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Ninitbuf( )
**
**  void Ninitbuf( int size, int count )
**
**  PARAMETERS:
**		size		This is the size of each buffer (MAXBUF)
**		count		This is the Number of buffers (NBUFFS)
**
**  RETURNS:		Nothing
**			
**  DESCRIPTION:  	Initialize buffer pool.
**					The buffer pool is aligned to a byte boundary
**					given by USSBUFALIGN.  This is for DMA, Cache,
**					and other "things" that need to share common
**					data structures, sometimes in multiple byte
**					accesses.  This assumes that the data region
**					of an Nbuffer will always be gotten to by using
**					(char *)mp + MESSH_SZ.  This will ensure the 
**					proper alignment.
**					
**					It is assumed that MAXBUF contains enough room for the
**					MESSH structure and the Data area of the Nbuffer.  
**                  The declaration of bufpool allocates enough extra 
**					space for the alignment of Nbuffers
**
**  USAGE/COMMENTS:
**					Called in Net.c from Ninit()  
**					USSBUFALIGN is defined in /include/local.h
**                  MESSH_SZ is defined in /include/support.h
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  08/31/1999		BTS	 Modified to add alignment
**  08/31/1999    	BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/


/* The ( +4 ) in the declaration of bufpool is for some network cards which */
/* need to have space allocated for CRC bytes.  (amd7990.c, Amd961.c,       */
/* dc21040.c, ne2100.c )  This value is not included in the size of MAXBUF  */


#define EXTRAPADDING  ( USSBUFALIGN - 1 )
static unsigned char FAR bufpool[ EXTRAPADDING + ( MAXBUF + 4 + 2*EXTRAPADDING ) * (unsigned)NBUFFS ];
#if NTRACE>=1
static MESS *bufptr[NBUFFS];
#endif
void Ninitbuf( int size, int count )
{
    int i1;
    int tSize;			/* temporary size of NBuffers, adjusted for alignment */
    char *cp1;			/* temporary offset of bufpool, adjusted for alignment */
    MESS *mp, *mp2;     /* MESS *'s used for setting up linked list of buffers */

	/* We need a USSMESSALIGN aligned address for DMA in some systems. */
	/* Align the start of the bufpool, referenced by Nbufpool */
    /* Use Nbufbase to refer to bufpool, not bufpool!!! */
    
    /* Align the beginning of the buffer pool */
	cp1 = (char *)(((long)&bufpool + USSBUFALIGN - 1 ) & ~(USSBUFALIGN-1));

	/* The size of an Nbuffer in the format of */
    /* padding in front | MESSH | pad | Data Area | pad | */

    /* tSize ensures that each buffer is a multiple of USSBUFALIGN */
    tSize = (int)( size + 4 + 2 * EXTRAPADDING ) & ~(USSBUFALIGN - 1);

    Nbufbase = Nfirstbuf = mp2 = mp = (MESS *)cp1;
    for( i1 = 0; i1 < count; i1++ )
    {
#if NTRACE>=1
        bufptr[i1] = mp;			/* */
#endif
		mp->id = bFREE;				/* Set the ID to bFREE */
        mp2 = mp;					/* This is the link we are about to set */
        mp = (MESS *)((char *)mp + tSize);		/* calculate the "next" link */
        mp2->next = mp;				/* Set the link to the next Nbuffer */
    }
    mp2->next = 0;					/* End of the Linked List */
}

#if NTRACE > 0
/* Insist that all buffers are free.  Test code. */
void Ncntbuf()
{
    int i1;
    MESS *mp;
    int tSize;			/* temporary size of NBuffers, adjusted for alignment */

    tSize = (int)( MAXBUF + 4 + 2 * EXTRAPADDING ) & ~(USSBUFALIGN - 1);
    
    for (i1=0,mp=Nbufbase; i1<NBUFFS; i1++,mp=(MESS*)((char *)mp+tSize))
        if (mp->id != bFREE)
            Npanic("cntbuf");
}
#endif

/* Return a global port number, initial value is random and then we just
   bump by one, skipping the range 0-1023.  
*/
unsigned short Nportno()
{
    portnumber++;
    if (portnumber < 1024)
        portnumber = 1024;
    return portnumber;
}

/* Routine to check if a connection can accept a write without blocking.
   At least 2 buffers must be free, the window big enough, not too many
   messages waiting for ACK.  For UDP, always give len as zero. 
*/
int socket_cansend(int conno, unsigned int len)
{
    MESS *mp;
    struct CONNECT *conp;

    BLOCKPREE();
    if ((mp = Nfirstbuf) != 0)
        mp = mp->next;
    RESUMEPREE();
    if (mp != 0)
    {
        conp = &connblo[conno];
        if (conp->nwacks <= conp->wackmax &&
            (long)(conp->ackno + conp->window - conp->txseq - len) >= 0)
            return 1;
    }
    return 0;
}

/* Internet checksum routine.  If anything this should be in assembler.
   See the I386 or I8086 version SUPPA.ASM. */

#if !defined(chksum_INASM)
unsigned short Nchksum(unsigned short *cp, int cnt)
{
    register unsigned short i1=0, i2;

    while (cnt--)
    {
        i2 = *cp++;
        i1 += i2;
        if (i1 < i2)
            i1++;
    }
    return i1;
}
#endif


/* Common read routine, used in those protocol modules that have no special
   processing for read.  TCP, UDP and IP have their own read.

   Waits until the screener has queued a coming message for the particular
   connection.  Returns:
        error:    -1
         success:  message address
 */

MESS *reaDD(int conno)
{
    int i1;
    MESS *mess;
    register struct CONNECT *conp;
    
    conp = &connblo[conno];
    WAITFOR(conp->first || (conp->rxstat & S_EOF), SIG_RC(conno),
        conp->rxtout, i1);
    if (!conp->first)
        return 0;
    BLOCKPREE();
    MESS_OUT(conp, mess);
    RESUMEPREE();
    return mess;
}


