/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/igmp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: igmp.c $
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
/* IGMP.C  Edition: 3  Last changed: 8-Oct-99,10:48:24  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    IGMP.C -- Internet Group Management Protocol for USNET

    Copyright (C) 1998, 1999 By
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

  08-OCT-1999  BSK  Initialize mclist
  22-JAN-1999  RCA  Renamed ussMcastGroupJoin, etc.
  18-NOV-1998  RCA  Creation

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Implementation based on
    RFC 1112 Host Extensions for IP Multicasting
    RFC 2236 Internet Group Management Protocol, Version 2
*/


#include <string.h>
#include <stdlib.h>  /* for rand() */
#include "net.h"
#include "local.h"
#include "support.h"

#define IP_IGMP 2               /* protocol code for IP */

 /* datagrams directed to a multicast destination */

/* Values for IGMP message type field */
#define MEMBERSHIP_QUERY           0x11
#define VERSION1_MEMBERSHIP_REPORT 0x12
#define VERSION2_MEMBERSHIP_REPORT 0x16
#define LEAVE_GROUP                0x17

/* Values taken from RFC 2236 */
#define UNSOLICITED_REPORT_INTERVAL 10000UL

extern struct NET nets[];
extern struct NETCONF netconf[];
extern struct CONNECT connblo[];
extern PTABLE  *const P_tab[];
extern PTABLE   ICMP_T,
                IP_T;

/* Symbolic constants for multicast group states */
#define NONMEMBER       0
#define DELAYING_MEMBER 1
#define IDLE_MEMBER     2

/* Maximum number of multicast groups */
#define MCLISTMAX 10

/* Data structure for maintaining state of multicast groups */
static struct mclist_t {
    Iid             mcaddr;
    int             netno;
    int             refcnt;
    int             state;
    unsigned long   timerstart;
    unsigned long   timerperiod;
    int             lasthostflag;
}               mclist[MCLISTMAX];

/* Format of IGMP Message */
struct IGMPhdr {
    unsigned char   type;       /* was version and type in version 1 */
    char            maxresptime;/* was unused in version 1 */
    unsigned short  checksum;
    unsigned char   groupaddress[Iid_SZ];
};
#define IGMPhdr_SZ 8

static int writE(int conno, MESS *mess);

/* The following should be reworked once more flexible support for TTL and
IP options are available in the IP module. */
struct Ihdr {                   /* Internet header */
    char            message_header[MESSH_SZ];
    char            link_header[LHDRSZ];
    unsigned char   verlen;     /* header version, length in 32 bit words */
    unsigned char   tservice;   /* type of service */
    unsigned short  tlen;       /* total packet length including header */
    unsigned short  frid;       /* ID for fragmentation */
    unsigned short  fragm;      /* fragment flags, offset */
    unsigned char   time;       /* time to live (hops) */
    unsigned char   prot;       /* protocol */
    unsigned short  chksum;     /* header checksum */
/* the following are struct Iaddr but some C compilers force impossible
   alignments for structures so we have to use character arrays */
    unsigned char   from[Iid_SZ];       /* source name */
    unsigned char   to[Iid_SZ]; /* destination name */
    unsigned char   options;    /* start of optional options field */
};
#define MH(mb) ((struct Ihdr *)mb)
#define IPOFFSET (MESSH_SZ+LHDRSZ)      /* IP header offset */
extern unsigned short IPfragid;
#define IP_VER 4                /* internet version */
#define ET_IP NC2(0x0800)       /* protocol code */
const char      routeralert[] = {0x94, 0x04, 0x00, 0x00};
const char      all_hosts_address[] = {224, 0, 0, 1};
const char      general_query_address[] = {0, 0, 0, 0};

static int igmpstate;  /* used to identify first igmp activity */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  SendIGMPMessage()  Send an IGMP Message
**
**  void SendIGMPMessage(Iid iid, int netno, int type);
**
**  PARAMETERS:
**     (in)     iid    Internet address of message destination
**     (in)     netno  Index of network interface on which message will be sent
**     (in)     type   IGMP message type
**
**  DESCRIPTION:
**     Allocates a message buffer, and builds and sends an IGMP message using
**     the provided parameters.
**
**  RETURNS:
**     none
**
**  USAGE/COMMENTS:
**     This function is called from uss_JoinHostGroup(), uss_LeaveHostGroup()
**     and timeout().
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  18-NOV-1998  RCA  Creation
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void     SendIGMPMessage(Iid iid, int netno, int type)
{
    int i1;
    MESS           *mp;
    struct IGMPhdr *igmpp;

    if ((mp = Ngetbuf()) == 0)
        return;
    mp->target = iid.l;
    mp->mlen = MESSH_SZ + LHDRSZ + Ihdr_SZ + sizeof(routeralert) + IGMPhdr_SZ;
    mp->netno = netno;
    mp->offset = MESSH_SZ + LHDRSZ + Ihdr_SZ + sizeof(routeralert);
    mp->id = bRELEASE;
    igmpp = (struct IGMPhdr *) ((char *) mp + MESSH_SZ + LHDRSZ + Ihdr_SZ
                                + sizeof(routeralert));
    igmpp->type = type;
    igmpp->maxresptime = 0;
    memcpy((char *) &igmpp->groupaddress, (char *) &iid, Iid_SZ);

    i1 = writE(ussMulticastIndex, mp);
    if (i1)
        Nrelbuf(mp);

}


/* * * * * * * * * * * * * * * * *
**  init()   IGMP init function
**
**  PARAMETERS:
**     (in)
**     (in)
**
**  DESCRIPTION:
**
**  RETURNS:
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * *
**  MODIFICATION HISTORY:
**  15-NOV-1998  RCA  Creation
** * * * * * * * * * * * * * * * */
static int      init(int netno, char *params)
{
    (void)netno;    /* quietly ignore this argument */
    (void)params;   /* quietly ignore this argument */

    igmpstate = 0;
    memset((void *)&mclist, 0, sizeof(mclist));
    return 0;
}


/* * * * * * * * * * * * * * * * *
**  opeN()   IGMP open function
**
**  PARAMETERS:
**     (in)     conno  Connection number
**     (in)     flags  Special processing flags
**
**  DESCRIPTION:
**    Performs any special processing needed associated with the opening of
**    a connection.  In the case of IGMP, no processing is needed, so this
**    call is cascaded down to the next lower layer in the stack.
**
**  RETURNS:
**    The return value comes from the next lower layer in the stack.
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * *
**  MODIFICATION HISTORY:
**  15-NOV-1998  RCA  Creation
** * * * * * * * * * * * * * * * */
static int opeN(int conno, int flags)
{
    return connblo[conno].protoc[1]->opeN(conno, flags);
}


/* * * * * * * * * * * * * * * * *
**  screen()   IGMP screen function
**
**  PARAMETERS:
**     (in)
**
**  DESCRIPTION:
**
**  RETURNS:
**  -1  drop message
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * *
**  MODIFICATION HISTORY:
**  15-NOV-1998  RCA  Creation
** * * * * * * * * * * * * * * * */
static int screen(MESS *mess)
{
    int             mlen;
    int             i;
    unsigned long   ul1;
    struct IGMPhdr  *hdrp;
    struct mclist_t *mcptr;

    hdrp = (struct IGMPhdr *) ((char *) mess + mess->offset);
    mlen = mess->mlen - mess->offset;
    if (mlen < 8)  /* msg must be at least 8 bytes long */
        goto err1;
    if (mlen & 1)
        *((char *)hdrp + mlen) = 0;
    if (Nchksum((unsigned short *)hdrp, (mlen + 1) / 2) != 0xffff)
        goto err1;
    ul1 = TimeMS();
    switch (hdrp->type) {

    case MEMBERSHIP_QUERY:
        if (*(unsigned long *)hdrp->groupaddress ==
            *(unsigned long *)general_query_address) {
            for (i = 0; i < MCLISTMAX; i++) {
                mcptr = &mclist[i];
                if ((mcptr->refcnt) && (mcptr->netno == mess->netno)) {
                    if (((mcptr->state == DELAYING_MEMBER) &&
                        (mcptr->timerperiod - (ul1 - mcptr->timerstart) >
                        hdrp->maxresptime * 100UL)) ||
                        (mcptr->state == IDLE_MEMBER)) {
                        mcptr->lasthostflag = 1;
                        mcptr->timerstart = TimeMS();
                        mcptr->timerperiod =
                           ((unsigned long)hdrp->maxresptime * 100UL *
                            (unsigned long)rand()) /
                            (unsigned long)RAND_MAX;
                        mcptr->state = DELAYING_MEMBER;    /* starts timer */
                        return -1;
                    }
                }
            }
        }
        else {
            for (i = 0; i < MCLISTMAX; i++) {
                mcptr = &mclist[i];
                if ((mcptr->refcnt) && (mcptr->netno == mess->netno) &&
                    (mcptr->mcaddr.l == *(unsigned long *)hdrp->groupaddress)) {
                    if (((mcptr->state == DELAYING_MEMBER) &&
                        (mcptr->timerperiod - (ul1 - mcptr->timerstart) >
                        hdrp->maxresptime * 100UL)) ||
                        (mcptr->state == IDLE_MEMBER)) {
                        mcptr->lasthostflag = 1;
                        mcptr->timerstart = TimeMS();
                        mcptr->timerperiod =
                           ((unsigned long)hdrp->maxresptime * 100UL *
                            (unsigned long)rand()) /
                            (unsigned long)RAND_MAX;
                        mcptr->state = DELAYING_MEMBER;    /* starts timer */
                        return -1;
                    }
                }
            }
        }
        break;
    case VERSION1_MEMBERSHIP_REPORT:
        break;
    case VERSION2_MEMBERSHIP_REPORT:
        for (i = 0; i < MCLISTMAX; i++) {
            mcptr = &mclist[i];
            if ((mcptr->refcnt) && (mcptr->netno == mess->netno) &&
                (mcptr->mcaddr.l == *(unsigned long *)hdrp->groupaddress) &&
                (mcptr->state == DELAYING_MEMBER)) {
                mcptr->state = IDLE_MEMBER;
                mcptr->lasthostflag = 0;
                return -1;
            }
        }
        break;
    case LEAVE_GROUP:
        break;
    }

err1:
    return -1;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  closE()  close an IGMP binding
**
**  int closE(int conno);
**
**  PARAMETERS:
**     (in)     conno  connection number
**
**  DESCRIPTION:
**     This function simply calls the next lower layer closE()
**
**  RETURNS:
**     ...value returned from the next lower layer
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  18-NOV-1998  RCA  Creation
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int      closE(int conno)
{
    return connblo[conno].protoc[1]->closE(conno);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  writE()  write an IGMP message
**
**  int writE(int conno, MESS * mess);
**
**  PARAMETERS:
**     (in)     conno  connection number
**     (in)     mess   message buffer
**
**  DESCRIPTION:
**     Calculates IGMP header checksum and writes out IGMP message.
**
**  RETURNS:
**     ...value returned from the next lower layer
**
**  USAGE/COMMENTS:
**     Called from SendIGMPMessage() or from an application that has opened an
**     IGMP connection.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  18-NOV-1998  RCA  Creation
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int      writE(int conno, MESS * mess)
{
    int             i1;
    int             ihdrsz;
    int             mlen;
    unsigned short  us1;
    struct IGMPhdr *hdrp;
    struct NET     *netp;

    hdrp = (struct IGMPhdr *) ((char *) mess + mess->offset);
    mlen = mess->mlen - mess->offset;
    hdrp->checksum = 0;
    if (mlen & 1)
        *((char *) hdrp + mlen) = 0;
    hdrp->checksum = ~Nchksum((unsigned short *) hdrp, (mlen + 1) / 2);
/* protocol code for IP */
    *((char *) mess + MESSH_SZ + LHDRSZ + 9) = IP_IGMP;

    /* Ordinarily we would make a call into the network layer writE()
     * function at this point.  It would look like this:
     * 
     * return IP_T.writE(conno, mess);
     * 
     * Unfortunately, the current implementation doesn't provide for two
     * features required by IGMP: 1) We need to specify a TTL of 1. 2) We
     * need to include the IP Router Alert Option.  This will be addressed by
     * building the IP header here in the IGMP writE() function. */

#if 0
    IPgroup.ipOutRequests++;
#endif

    netp = &nets[mess->netno];
    mess->offset = IPOFFSET;

    mess->confix = ussMulticastIndex;
    us1 = IPfragid++;           /* Increment IP ID number */
    MH(mess)->frid = NC2(us1);  /* Place ID in header */

/* build the IP header */

    MH(mess)->verlen = (IP_VER << 4) | ((Ihdr_SZ + sizeof(routeralert)) >> 2);
    MH(mess)->time = 1;
    MH(mess)->fragm = 0;
    MH(mess)->tservice = 0;
    memcpy((char *) &MH(mess)->to, (char *) &mess->target, Iid_SZ);
    memcpy((char *) &MH(mess)->from, (char *) &netconf[netp->confix].Iaddr,
           Iid_SZ);
    memcpy((char *) &MH(mess)->options, routeralert, sizeof(routeralert));

    i1 = mess->mlen - IPOFFSET;
    MH(mess)->tlen = NC2(i1);

    MH(mess)->chksum = 0;
    ihdrsz = (MH(mess)->verlen & 0xf) * 2;
    MH(mess)->chksum = ~Nchksum((unsigned short *) ((char *) mess + IPOFFSET),
                                ihdrsz);
    *(short *) ((char *) mess + MESSH_SZ + 12) = ET_IP;

    return netp->protoc[0]->writE(conno, mess);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ussHostGroupJoin()  Join a host group
**
**  int ussHostGroupJoin(Iid iid, int netno);
**
**  PARAMETERS:
**     (in)     iid    internet address of group to join
**     (in)     netno  index of interface on which the group is joined
**
**  DESCRIPTION:
**     Joins a host group.
**
**  RETURNS:
**     xxx invalid group address
**     xxx invalid interface identifier
**     xxx no resources available
**
**  USAGE/COMMENTS:
**     This function is part of the USNET DPI.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  18-NOV-1998  RCA  Creation
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int             ussHostGroupJoin(Iid iid, int netno)
{
    int             i;
    Iid             iid2;
    struct mclist_t *mcptr;

    if (igmpstate == 0) {
        /* join all hosts group as soon as there is activity */
        iid2.c[0] = 224;
        iid2.c[1] = 0;
        iid2.c[2] = 0;
        iid2.c[3] = 1;

        nets[netno].protoc[ussLinkIndex]->ioctl(&nets[netno],
           ussMcastGroupJoinE, &iid2, sizeof(iid2));
        igmpstate = 1;
    }

    for (i = 0; i < MCLISTMAX; i++) {
        mcptr = &mclist[i];
        if ((mcptr->mcaddr.l == iid.l) &&
            (mcptr->netno == netno) &&
            (mcptr->refcnt))
        {
            mcptr->refcnt++;
            return 0;
        }
    }

    for (i = 0; i < MCLISTMAX; i++) {
        mcptr = &mclist[i];
        if (mcptr->refcnt == 0) {
            mcptr->mcaddr = iid;
            mcptr->netno = netno;
            mcptr->refcnt = 1;
            nets[netno].protoc[ussLinkIndex]->ioctl(&nets[netno],
               ussMcastGroupJoinE, &iid, sizeof(iid));
            SendIGMPMessage(iid, netno, VERSION2_MEMBERSHIP_REPORT);
            mcptr->lasthostflag = 1;
            mcptr->timerstart = TimeMS();
            mcptr->timerperiod =
                           (UNSOLICITED_REPORT_INTERVAL *
                            (unsigned long)rand()) /
                            (unsigned long)RAND_MAX;
            mcptr->state = DELAYING_MEMBER;    /* starts timer */
            return 0;
        }
    }
    return -1;
}


/*  ussHostGroupLeave()
    Give up membership in a host group
    Possible Failures:
    invalid group address
    invalid interface identifier
    group not found
*/
int             ussHostGroupLeave(Iid iid, int netno)
{
    int             i;
    struct mclist_t *mcptr;

    for (i = 0; i < MCLISTMAX; i++) {
        mcptr = &mclist[i];
        if ((mcptr->mcaddr.l == iid.l) &&
            (mcptr->netno == netno) &&
            (mcptr->refcnt)) {
            mcptr->refcnt--;
            if (mcptr->refcnt == 0) {
                nets[netno].protoc[ussLinkIndex]->ioctl(&nets[netno],
                    ussMcastGroupLeaveE, &iid, sizeof(iid));
                if (mcptr->lasthostflag) {
                    SendIGMPMessage(mcptr->mcaddr, mcptr->netno,
                                    LEAVE_GROUP);
                }
                mcptr->state = NONMEMBER;    /* stops timer */
            }
            return 0;
        }
    }
    return -1;
}


/* ussHostGroupCheck()
   Check for membership in a host group
   Returns:
   0 if this system is not a member of the host group
   1 if this system is a member of the host group
*/
int             ussHostGroupCheck(Iid iid, int netno)
{
    int             i;
    struct mclist_t *mcptr;

    for (i = 0; i < MCLISTMAX; i++) {
        mcptr = &mclist[i];
        if ((mcptr->mcaddr.l == iid.l) &&
            (mcptr->netno == netno) &&
            (mcptr->refcnt))
            return 1;
    }
    /* always accept messages sent to all hosts */
    if (iid.l == *(unsigned long *)all_hosts_address)
        return 1;
    return 0;
}


static void timeout(int conno)
{
    int i1;
    unsigned long ul1;
    struct mclist_t *mcptr;

    (void)conno;    /* quietly ignore this argument */

    ul1 = TimeMS();
    for (i1 = 0; i1 < MCLISTMAX; i1++) {
        mcptr = &mclist[i1];
        if (mcptr->state == DELAYING_MEMBER) {
            if (ul1 - mcptr->timerstart > mcptr->timerperiod) {
                 SendIGMPMessage(mcptr->mcaddr, mcptr->netno,
                            VERSION2_MEMBERSHIP_REPORT);
                 mcptr->lasthostflag = 1;
                 mcptr->state = IDLE_MEMBER;
            }
        }
    }
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
**  04-JAN-1999  RCA  Added mcast support
**  31-DEC-1998  RCA  Created stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */

    switch (request) {

    default:
        return ussErrInval;
    }
}


/* ===========================================================================
   Protocol table for IGMP. */

GLOBALCONST
PTABLE ussIGMPTable = {
    "IGMP", init, timeout, screen, opeN, closE, 0, writE, ioctl, IP_IGMP, 0
};
