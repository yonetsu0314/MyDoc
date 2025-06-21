/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/ethernet.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ethernet.c $
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
/* ETHERNET.C  Edition: 9  Last changed: 5-Apr-99,10:31:52  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ETHERNET.C -- Ethernet Link Layer for USNET

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

* * * * * * * * * * * * * * * * * * * * * * * * *

  MODIFICATION HISTORY:

  05-APR-1999  TTT  Subtract Ethernet header size from netp->maxblo
  22-JAN-1999  RCA  Renamed ussMcastGroupJoin etc.
  19-JAN-1999  RCA  Fix to mcast address translation
  18-JAN-1999  RCA  Added multicast support
  31-DEC-1998  RCA  Added ioctl stub  

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "ethernet.h"

#define MH(mb) ((struct Ehdr *)mb)

extern struct NET nets[];
extern struct NETCONF netconf[];
extern struct NETDATA netdata[];
extern struct CONNECT connblo[];
extern PTABLE  *const P_tab[];
#ifdef ARP
extern PTABLE   ussARPTable;
#endif

#if USS_IP_MC_LEVEL > 0
static const char mcastmac[] = {0x01, 0x00, 0x5e};
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  writE(): Ethernet writE function
**
**  static int      writE(int conno, MESS * mess)
**
**  PARAMETERS:
**      (in): conno:  Connection number associated with the message
**                    buffer being written. If negative, true 
**                    connection number is in the message buffer itself.
**      (in): mess:   Message buffer to be written
**
**  RETURNS:
**      >=0: Number of characters sent
**      < 0: Error
**
**  DESCRIPTION:
**      Ethernet write routine.  If we don't have the target Ethernet 
**      address, we send an ARP message and wait for the address.  
**      Creates an Ethernet header and calls the lower level write. 
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int      writE(int conno, MESS * mess)
{
    int             i1,
                    netno,
                    confix;
    unsigned short  us1;
    MESS           *mp;
    struct NETCONF *confp;
    register struct NET *netp;

/* At this point we have the network number but not the Ethernet address.
*/
    netno = mess->netno;
    netp = &nets[netno];
    mess->offset = MESSH_SZ;

/* If conno == -1, we return the message to the sender.
*/
    if (conno == -1) {
        memcpy((char *) &MH(mess)->to, (char *) &MH(mess)->from, Eid_SZ);
        goto lab2;
    }
/* Value mess-confix = USS_BROADCAST_INDEX means full broadcast.
*/
    if (mess->confix == ussBroadcastIndex) {
        memset((char *) &MH(mess)->to, 0xff, Eid_SZ);
        goto lab2;
    }
#if USS_IP_MC_LEVEL > 0
    else if (mess->confix == ussMulticastIndex) {
        /* Multicast maps low-order 23-bits of IP address into MAC address */
        /* with special 01:00:5e prefix. */
        memcpy((char *) &MH(mess)->to, mcastmac, sizeof(mcastmac));
        memcpy(&MH(mess)->to[3], (char *)&mess->target + 1, 3);
        MH(mess)->to[3] &= 0x7f;
        goto lab2;
    }
#endif
/* If we don't have the target hardware address we use ARP to get it.
*/
    confix = netconf[mess->confix].nexthix;
    confp = &netconf[confix];
    if (confp->hwaddyes == 0) {
#ifdef ARP
        if ((mp = Ngetbuf()) == 0)
            goto lab8;
        mp->netno = netno;
        mp->confix = confix;
        ussARPTable.writE(-1, mp);
        memset((char *) &MH(mp)->to, 0xff, Eid_SZ);
        memcpy((char *) &MH(mp)->from, (char *) &netp->id, Eid_SZ);
        mp->offset = MESSH_SZ;
        mp->id = bRELEASE;
        i1 = netp->protoc[1]->writE(netno, mp);
        if (i1)
            Nrelbuf(mp);
/* Write from network task can not wait.  We mark the message to be sent when
   the ARP reply arrives.  Only one message can be waiting, the rest have to
   be resent.  If no reply arrives, the waiting entry will be freed by the
   network task.
*/
        if (conno < 0) {
            i1 = ETIMEDOUT;
            BLOCKPREE();
            if (confp->ARPwait == 0) {
                confp->ARPwait = (char) conno;
                confp->ARPwaitmp = mess;
                confp->timems = TimeMS();
                i1 = 0;
            } else
                mess->offset = boTXDONE;    /* Indicate trasmit done */
            RESUMEPREE();
            return i1;
        }
#if NTRACE >= 1
/*
        Nprintf("wai_flg_sta(SIG_ARP),eth.c,writE\n");
*/
#endif
        WAITFOR(confp->hwaddyes, SIG_ARP, ET_TOUT, i1);
#if NTRACE >= 1
/*
        Nprintf("wai_flg_end(SIG_ARP),eth.c,writE\n");
*/
#endif
        if (i1) {
    lab8:   mess->offset = boTXDONE;
            return ETIMEDOUT;
        }
#else
#if NTRACE >= 1
        Nprintf("no ARP and Ethernet address 0\n");
#endif
        return EHOSTUNREACH;
#endif
    }
    memcpy((char *) &MH(mess)->to, (char *) &confp->Eaddr, Eid_SZ);

lab2:
    memcpy((char *) &MH(mess)->from, (char *) &netp->id, Eid_SZ);

#ifdef MIB2
    netp->ifOutOctets += mess->mlen - mess->offset;
    if (MH(mess)->to[5] & 0x80)
        netp->ifOutNUcastPkts++;
    else
        netp->ifOutUcastPkts++;
#endif

    us1 = mess->id;
    i1 = netp->protoc[1]->writE(netno, mess);
    if (i1 != 0 || us1 <= bWACK)
        return i1;
#if NTRACE >= 1
/*
    Nprintf("wai_flg_sta(SIG_WN),eth.c,writE\n");
*/
#endif
    WAITFOR(mess->offset == boTXDONE, SIG_WN(netno), ET_TOUT, i1);
#if NTRACE >= 1
/*
    Nprintf("wai_flg_sta(SIG_WN),eth.c,writE\n");
*/
#endif
    return 1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  screen():  Ethernet screening function
**
**  static int      screen(MESS * mess)
**
**  PARAMETERS:
**     (in): mess     Message buffer that is being screened
**
**  RETURNS:
**      -3: call write
**      -2: no further action
**      -1: error ocurred
**       n: Enter in queue number n
**
**  DESCRIPTION:
**      Screen an arrived Ethernet message. Pass message on to next
**      protocol level screen.
**
**  USAGE/COMMENTS:
**     Ultimately returns to screen() call in NetTask, where the 
**     returned connection number is used to enter the message buffer 
**     in the correct queue.
** 
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int      screen(MESS * mess)
{
    int             i1,
                    nxtlev;

    mess->conno = MH(mess)->to[0] & 0x01;   /* bcast indicator */
#ifdef MIB2
    nets[mess->netno].ifInOctets += mess->mlen - MESSH_SZ;
    if ((char) MH(mess)->to[0] & 0x01)
        nets[mess->netno].ifInNUcastPkts++;
    else
        nets[mess->netno].ifInUcastPkts++;
#endif
    for (nxtlev = 1; P_tab[nxtlev]; nxtlev++)
        if (MH(mess)->type == P_tab[nxtlev]->Eprotoc) {
            mess->offset = MESSH_SZ + Ehdr_SZ;
            i1 = P_tab[nxtlev]->screen(mess);
            if (i1 == -3)
                writE(-1, mess);
            return i1;
        }
#ifdef MIB2
    nets[mess->netno].ifInUnknownProtos++;
#endif
    return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  init()  Initialize the protocol
**
**  static int init(int netno, char *params)
**
**  PARAMETERS:
**   (in) netno                A USNET network (interface) number
**   (in) params               A list of parameters (ignored)
**
**  RETURNS:
**     1: success
**    -1: error
**
**  DESCRIPTION:
**    Initialize the Ethernet level for a network. Calls the driver
**    initialization.  
** 
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  05-APR-1999  TTT  Subtract Ethernet header size from maxblo
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int      init(int netno, char *params)
{
    int             i1;
    struct NET     *netp;

    netp = &nets[netno];
    netp->id = netdata[netp->confix].Eaddr;
    netp->maxblo = ET_MAXLEN <= MAXBUF - MESSH_SZ ? ET_MAXLEN :
        MAXBUF - MESSH_SZ - Ehdr_SZ;
    i1 = netp->protoc[1]->init(netno, params);
    if (i1 < 0)
        return i1;
    netconf[netp->confix].Eaddr = netp->id;
    netp->sndoff = MESSH_SZ + Eid_SZ;
    netp->tout = ET_TOUT;
#ifdef MIB2
    netp->ifType = 6;
#endif
    return i1;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  shut():  Ethernet shut function
**
**  static void     shut(int netno)
**
**  PARAMETERS:
**     (in): netno:  USNET network (interface) number
**
**  RETURNS: none
**
**  DESCRIPTION: 
**      Shut down the Ethernet layer. Calls the driver shutdown.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void     shut(int netno)
{
    nets[netno].protoc[1]->shut(netno);
}


/* ===========================================================================
   Open a connection.
*/

static int      opeN(int conno, int flags)
{
    return 1;
}


/* ===========================================================================
   Close a connection at the Ethernet level.
*/

static int      closE(int conno)
{
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
**     (in)     size     size of information arg points to
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.
**
**  RETURNS:
**     0              Success
**     ussErrInval    Support for the request not implemented
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  04-JAN-1999  RCA  Added mcast support
**  31-DEC-1998  RCA  Created stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
#if USS_IP_MC_LEVEL != 2
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */
#else
    unsigned char macaddr[Eid_SZ];
    (void)size;      /* quietly ignore this argument */
#endif

    switch (request) {

#if USS_IP_MC_LEVEL == 2
    case ussMcastGroupJoinE:
    case ussMcastGroupLeaveE:
        memcpy(macaddr, mcastmac, sizeof(mcastmac));
        memcpy(&macaddr[3], (char *) arg + 1, 3);
        macaddr[3] &= 0x7f;
        return ((struct NET *) handle)->protoc[ussDriverIndex]->ioctl(handle,
                request, macaddr, sizeof(macaddr));
#endif

    default:
        return ussErrInval;
    }
}


/* ===========================================================================
   Protocol table for Ethernet. */

GLOBALCONST
PTABLE ussEthernetTable = {
    "Ethernet", init, shut, screen, opeN, closE,
        reaDD, writE, ioctl, ET_IP, LHDRSZ
};
