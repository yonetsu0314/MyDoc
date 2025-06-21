/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/arp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: arp.c $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  masahiro
 * èâä˙ÉäÉrÉWÉáÉì
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* ARP.C  Edition: 6  Last changed: 24-May-99,10:57:48  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ARP.C -- Address Resolution Protocol for USNET

    Copyright (C) 1993, 1995, 1998 By
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

* * * * * * * * * * * * * * * * * * * * * * * * *

  MODIFICATION HISTORY:

  21-APR-1999  SCD  Add scan of netconf for proxy ARP to screen()
  30-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#ifdef ARP

#define TAKEALL 0               /* 0 = partners only, 1 = everybody */

static unsigned short H_A_TYPE = 1;	/*	1:DIX 6:IEEE	*/

/* We assume that the compiler will not place any padding into the following
   structure.  (If it did, structures would be pretty much useless.)
*/
struct ARM {                    /* address resolution message */
    char            message_header[MESSH_SZ];
    char            link_header[LHDRSZ];
    short           hardware;   /* 1 = Ethernet */
    short           protocol;   /* 0800 = Internet */
    char            halen,
                    ialen;      /* lengths of addresses */
    short           opcode;     /* 1=request, 2=reply */
    unsigned char   shadd[Eid_SZ];  /* sender hardware address */
    unsigned char   siadd[Iid_SZ];  /* sender Internet address */
    unsigned char   thadd[Eid_SZ];  /* target hardware address */
    unsigned char   tiadd[Iid_SZ];  /* target Internet address */
};
#define ARM_SZ 28               /* size of structure */
#define EA_REQ NC2(1)           /* ARP request */
#define EA_RES NC2(2)           /* ARP response */
#define ET_IP NC2(0x0800)       /* Internet message */
#define ET_ARP NC2(0x0806)      /* address request message */

#define MH(mb) ((struct ARM *)mb)

extern struct NET nets[];
extern struct NETCONF netconf[];
extern struct CONNECT connblo[];


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  writE()      Write routine
**
**  int writE(int conno, MESS * mess);
**
**  PARAMETERS:
**     (in)     conno    USNET connection number
**     (in)     mess     pointer to message buffer to be written
**
**  DESCRIPTION:
**     This function creates a header and calls the lower level write.
**
**  RETURNS:
**     0 <       Success - number of characters sent
**     -1      Error
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  21-APR-1999  SCD  Add function header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int writE(int conno, MESS * mess)
{
    int             netno,
                    confix;

    netno = mess->netno;
    confix = mess->confix;
    mess->mlen = MESSH_SZ + LHDRSZ + ARM_SZ;
    MH(mess)->opcode = EA_REQ;
    MH(mess)->hardware = NC2(H_A_TYPE);
    MH(mess)->protocol = ET_IP;
    MH(mess)->halen = Eid_SZ;
    MH(mess)->ialen = Iid_SZ;
    Nmemcpy((char *) &MH(mess)->thadd, (char *) &netconf[confix].Eaddr, Eid_SZ);
    Nmemcpy((char *) &MH(mess)->tiadd, (char *) &netconf[confix].Iaddr, Iid_SZ);
    Nmemcpy((char *) &MH(mess)->shadd, (char *) &netconf[nets[netno].confix].Eaddr,
           Eid_SZ);
    Nmemcpy((char *) &MH(mess)->siadd, (char *) &netconf[nets[netno].confix].Iaddr,
           Iid_SZ);
    *(short *) ((char *) mess + MESSH_SZ + 12) = ET_ARP;
    if (conno < 0)
        return 0;
    mess->confix = 255;
    return nets[netno].protoc[0]->writE(conno, mess);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  screen()      Screen an arrived message
**
**  int screen(MESS * mess);
**
**  PARAMETERS:
**     (in)     mess     pointer to message buffer to be screen
**
**  DESCRIPTION:
**     Process ARP request.  If this is a request for our IP address, we
**     send a response with our ethernet address.  If proxy ARP in
**     enabled, check the netconf table to see if the IP address is one
**     that we are the proxy for.
**
**  RETURNS:
**     -2      No further action required
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  21-APR-1999  SCD  Add scan of netconf for proxy ARP IP addresses
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int screen(MESS * mess)
{
    int             nottous,
                    confix;
    struct NETCONF *netcfp;
    struct NET     *netp;
    Iid             iid;

    netp = &nets[mess->netno];
    nottous = memcmp((char *) &netconf[netp->confix].Iaddr,
                     (char *) &MH(mess)->tiadd, Iid_SZ);
                     
#ifdef USS_PROXYARP
    {
        int i;
        
        for (i = 0; i < NCONFIGS; i++) {
            if ((!memcmp((char *) &netconf[i].Iaddr, &MH(mess)->tiadd, Iid_SZ)) &&
                   (netconf[i].flags & PROXYARP)) {
                nottous = 0;  /* it is now */
                break;
            }
        }
    }
#endif

#if TAKEALL == 0
    if (nottous)
        return -2;
#endif

    /* If this is to us, we create a new cache entry.  We update any
       existing entry.  */
       
    Nmemcpy((char *) &iid, MH(mess)->siadd, Iid_SZ);
    confix = GetHostData(iid.l, !nottous);
    if (confix >= 0) {
        netcfp = &netconf[confix];
        Nmemcpy((char *) &netcfp->Eaddr, (char *) &MH(mess)->shadd, Eid_SZ);
        netcfp->hwaddyes = 1;
        netcfp->timems = TimeMS();
        WAITNOMORE(SIG_ARP);
#if NTRACE >= 1
        Nprintf("ARP %02x%02x%02x%02x%02x%02x -> %d.%d.%d.%d\n",
                netcfp->Eaddr.c[0], netcfp->Eaddr.c[1], netcfp->Eaddr.c[2],
                netcfp->Eaddr.c[3], netcfp->Eaddr.c[4], netcfp->Eaddr.c[5],
                netcfp->Iaddr.c[0], netcfp->Iaddr.c[1], netcfp->Iaddr.c[2],
                netcfp->Iaddr.c[3]);
#endif
        /* If there is a message waiting for ARP, send it out.  */
        if (netcfp->ARPwait) {
            if (netp->protoc[0]->writE(netcfp->ARPwait, netcfp->ARPwaitmp))
                if (netcfp->ARPwait != -5)
                    Nrelbuf(netcfp->ARPwaitmp);
            netcfp->ARPwait = 0;
        }
    }

    /* If this a request to us, we reply. */
    /*  Test for proxy by scanning proxy table  */
    /*    if the IA is in the proxy table, set nottous = 0 */
    
    if (MH(mess)->opcode == EA_REQ && !nottous) {
        char temp_ia[Iid_SZ];
        Nmemcpy(temp_ia, (char *) &MH(mess)->tiadd, Iid_SZ);
        Nmemcpy((char *) &MH(mess)->thadd, (char *) &MH(mess)->shadd,
               Eid_SZ + Iid_SZ);  /*  copy HA and IA  */
        Nmemcpy((char *) &MH(mess)->shadd, (char *) &netp->id, Eid_SZ);
        Nmemcpy((char *) &MH(mess)->siadd, temp_ia, Iid_SZ);
        MH(mess)->opcode = EA_RES;
        netp->protoc[0]->writE(-1, mess);
    }
    return -2;
}


/* ===========================================================================
   Open an ARP connection.
*/

static int opeN(int conno, int flag)
{
    (void) conno, (void) flag;  /* quietly ignore these arguments */
    return 0;
}


/* ===========================================================================
   Close an ARP connection.
*/

static int closE(int conno)
{
    (void) conno;      /* quietly ignore this argument */
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size);
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
**  30-DEC-1998  RCA  Created stub version
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
   Protocol table for ARP. */

GLOBALCONST
PTABLE ussARPTable = {
    "ARP", 0, 0, screen, opeN, closE, reaDD, writE, ioctl,
        ET_ARP, 0
};

/*****************************************************************
			Ç`ÇqÇo SetHardwareAdrressType ê›íËèàóù
*****************************************************************/
void             UsnetARPSetHardwareAdrressType( char type )
{
	H_A_TYPE = type;
}
/*****************************************************************
			Ç`ÇqÇo SetHardwareAdrressType éÊìæèàóù
*****************************************************************/
char    UsnetARPGetHardwareAdrressType( void )
{
	return H_A_TYPE;
}
#endif
