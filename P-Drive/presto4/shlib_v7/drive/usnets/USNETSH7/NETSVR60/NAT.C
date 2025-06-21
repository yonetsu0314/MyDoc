/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/nat.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: nat.c $
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
/* NAT.C  Edition: 1  Last changed: 14-Jul-99,9:16:00  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995, 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway, Suite 100
    Hillsboro, Oregon 97124
    (503) 844-6614
    http://www.ussw.com

    nat.c   Network Address Translation module

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
  MODIFICATION HISTORY
    14-JUL-1999  BSK  Add: New to USNET
* * * * * * * * * * * * * * * * * * * * * * * * *
*/
#include <net.h>
#include <local.h>
#include <support.h>

#define NATNETNO 0              /* Globally accessable host interface */
#define NATTABLESZ 20           /* Number of mappable hosts */

struct Ihdr {                   /* Internet header */
    char message_header[MESSH_SZ];
    char link_header[LHDRSZ];
    unsigned char verlen;       /* header version, length in 32 bit words */
    unsigned char tservice;     /* type of service */
    unsigned short tlen;        /* total packet length including header */
    unsigned short frid;        /* ID for fragmentation */
    unsigned short fragm;       /* fragment flags, offset */
    unsigned char time;         /* time to live (hops) */
    unsigned char prot;         /* protocol */
    unsigned short chksum;      /* header checksum */
    unsigned char from[Iid_SZ]; /* source name */
    unsigned char to[Iid_SZ];   /* destination name */
};
#define IPH(mb) ((struct Ihdr *)mb)

#define NOMIPHDRSZ 20
struct Thdr {   /* TCP header */
    char message_header[MESSH_SZ];
    char link_header[LHDRSZ];
    char ip_header[NOMIPHDRSZ];
    unsigned short myport;                      /* source port */
    unsigned short herport;                     /* dest port */
    unsigned NLONG seqno[4 / sizeof(NLONG)];    /* sequence number */
    unsigned NLONG ackno[4 / sizeof(NLONG)];    /* acknowledgement number */
    unsigned char hdrlen;                       /* tcp header length */
    unsigned char flags;                        /* message flags */
    unsigned short window;                      /* window */
    unsigned short chksum;                      /* checksum */
    unsigned short urgp;                        /* urgent pointer */
};
#define TCPH(mb) ((struct Thdr *)mb)

struct Uhdr {   /* UDP header */
    char message_header[MESSH_SZ];
    char link_header[LHDRSZ];
    char ip_header[NOMIPHDRSZ];
    unsigned short myport;                      /* source port */
    unsigned short herport;                     /* dest port */
    unsigned short tlen;
    unsigned short chksum;                      /* checksum */
};
#define UDPH(mb) ((struct Uhdr *)mb)

struct {
    Iid trueIP;
    unsigned short truePort;
    unsigned short fakePort;
    unsigned char prot;
    unsigned long timestamp;
} NatTable[NATTABLESZ];

#define IP_ICMP 1
#define IP_TCP 6
#define IP_UDP 17

/*
** * * * * * *
** NATaddentry()   Add an entry to the NAT table
**
** int NATaddentry(trueIP, truePort, prot);
**
** PARAMETERS:
**   (in) Iid trueIP               The IP address of the isolated host
**   (in) unsigned short truePort  The port of the isolated host
**   (in) unsigned char prot       The IP protocol (TCP/UDP)
**
** RETURNS:
**   #                             Index of host in NatTable
**
** DESCRIPTION:
**   This function takes in a socket type entity and outputs an index into
**   the NAT table where the host entry was placed.
**
** USAGE/COMMENTS:
**   This routine can be called by the application when using Basic NAT to
**   administratively add members for the NAT host to map.
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
int NATaddentry(Iid trueIP, unsigned short truePort, unsigned char prot)
{
    int i1 = 0, replaceindex;
    unsigned long loweststamp;

   /* Use LRU to select entry */
    loweststamp = 0xffffffff;
    replaceindex = 0;
    for (i1 = 0; i1 < NATTABLESZ; i1++) {
        if (NatTable[i1].timestamp < loweststamp) {
            loweststamp = NatTable[i1].timestamp;
            replaceindex = i1;
        }
    }
   /* fill in new entry */
    i1 = replaceindex;
    NatTable[i1].trueIP.l = trueIP.l;
    NatTable[i1].truePort = truePort;
    NatTable[i1].prot = prot;
    NatTable[i1].fakePort = Nportno();

    return i1;
}


/*
** * * * * * *
** NATmapoutport()  Map an outbound IP address and port
**
** static unsigned short NATmapoutport(trueIP, truePort, prot);
**
** PARAMETERS:
**   (in) Iid trueIP               The IP address of the isolated host
**   (in) unsigned short truePort  The port of the isolated host
**   (in) unsigned char prot       The IP protocol (TCP/UDP)
**
** RETURNS:
**   The port used by NAT
**
** DESCRIPTION:
**   This function takes in a socket type entity and outputs a NAT port by
**   which to map.
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
static unsigned short NATmapoutport(Iid trueIP, unsigned short truePort,
                                    unsigned char prot)
{
    int i1;

   /* search for an entry that matches source IP and Port */
    for (i1 = 0; i1 < NATTABLESZ; i1++) {
        if ((NatTable[i1].trueIP.l == trueIP.l) &&
            (NatTable[i1].truePort == truePort) &&
            (NatTable[i1].prot == prot))
            break;
    }
   /* Try to find a new one if it's not found */
    if (i1 == NATTABLESZ)
        i1 = NATaddentry(trueIP, truePort, prot);
    NatTable[i1].timestamp = TimeMS();
    return NatTable[i1].fakePort;
}

/*
** * * * * * *
** NATmapinport()  Map an inbound port
**
** static unsigned short NATmapinport(fakePort);
**
** PARAMETERS:
**   (in) unsigned short fakePort      The port used by NAT
**
** RETURNS:
**   The real port used by the host or 0.
**
** DESCRIPTION:
**   This function takes in a NAT port and returns the real port.
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
static unsigned short NATmapinport(unsigned short fakePort)
{
    int i1;

    for (i1 = 0; i1 < NATTABLESZ; i1++) {
        if (NatTable[i1].fakePort == fakePort)
            break;
    }
    if (i1 < NATTABLESZ)
        return NatTable[i1].truePort;
    else
        return 0;
}

/*
** * * * * * *
** NATmapinip()  Map an inbound IP address
**
** static Iid NATmapinip(fakePort);
**
** PARAMETERS:
**   (in) unsigned short fakePort      The port used by NAT
**
** RETURNS:
**   The real IP address associated with the NAT port.
**
** DESCRIPTION:
**   This function takes in a NAT port and returns the real IP address.
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
static const Iid bad_ip = {0, 0, 0, 0};
static Iid NATmapinip(unsigned short fakePort)
{
    int i1;

    for (i1 = 0; i1 < NATTABLESZ; i1++) {
        if (NatTable[i1].fakePort == fakePort)
            break;
    }
    if (i1 < NATTABLESZ)
        return NatTable[i1].trueIP;
    else
        return bad_ip;
}


/*
** * * * * * *
** ussNATmapout()    Map an inbound packet if possible
**
** int ussNATmapout(mess);
**
** PARAMETERS:
**   (in/out) MESS *mess               A packet to map if possible
**
** RETURNS:
**   0                                 No mapping performed
**   1                                 Mapping performed
**
** DESCRIPTION:
**   This function takes in a datagram and translates the addresses if
**   possible to the address of this NAT router and a random port.
**
** USAGE/COMMENTS:
**   To Do:
**     Review the issue of fragmentation
**     Deal with IP options
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
int ussNATmapout(MESS *mess)
{
    unsigned short NATport;
    unsigned char olddata[6], newdata[6], NATprot;
    Iid NATip, tempIP;

    NATprot = IPH(mess)->prot;
   /* If the transport is unsupported or not right interface */
    if (((NATprot != IP_TCP) && (NATprot != IP_UDP) && (NATprot != IP_ICMP)) ||
            (mess->netno != NATNETNO))
    {
        return 0;   /* Unsupported */
    }

    NATip = netconf[nets[mess->netno].confix].Iaddr;
   /* first do database lookup to determine port remapping */
    memcpy((char *)&tempIP, &IPH(mess)->from, Iid_SZ);
    NATport = NATmapoutport(tempIP, TCPH(mess)->myport, NATprot);
    memcpy(olddata, &TCPH(mess)->myport, 2);
    memcpy(olddata + 2, &IPH(mess)->from, 4);
    memcpy(newdata, &NATport, 2);
    memcpy(newdata + 2, &NATip, 4);

#if NTRACE >= 7
    Nprintf("NAT: map out %u.%u.%u.%u.%u =>",
        tempIP.c[0], tempIP.c[1], tempIP.c[2], tempIP.c[3], TCPH(mess)->myport);
    Nprintf(" %u.%u.%u.%u.%u\n",
        NATip.c[0], NATip.c[1], NATip.c[2], NATip.c[3], NATport);
#endif

    if (NATprot == IP_TCP) {
        ussChecksumAdjust((unsigned char *)&TCPH(mess)->chksum,
            olddata, 6,
            newdata, 6);
    } else if (NATprot == IP_UDP) {
        ussChecksumAdjust((unsigned char *)&UDPH(mess)->chksum,
            olddata, 6,
            newdata, 6);
    }
    TCPH(mess)->myport = NATport;

   /* next, modify source IP, and recalculate IP header checksum */
    memcpy((char *)&IPH(mess)->from, (char *)&NATip, Iid_SZ);

    return 1;
}


/*
** * * * * * *
** ussNATmapin()    Map an inbound packet if possible
**
** int ussNATmapin(mess);
**
** PARAMETERS:
**   (in/out) MESS *mess               A packet to map in if possible
**
** RETURNS:
**   0                                 No mapping performed
**   1                                 Mapping performed
**
** DESCRIPTION:
**   This function takes in a datagram and translates the addresses and
**   port if possible to the address and port used by a host on the local
**   network.
**
** USAGE/COMMENTS:
**   To Do:
**     Review the issue of fragmentation
**     Support UDP
**     Deal with IP options
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
int ussNATmapin(MESS *mess)
{
    unsigned short NATport;
    unsigned char olddata[6], newdata[6], NATprot;
    Iid NATip, tempIP;

    NATprot = IPH(mess)->prot;
   /* If the transport is unsupported or port not mapped */
    if (((NATprot != IP_TCP) && (NATprot != IP_UDP) && (NATprot != IP_ICMP)) ||
        (NATmapinport(TCPH(mess)->herport) == 0))
    {
        return 0;
    }

   /* first do database lookup to determine port remapping */
    NATport = NATmapinport(TCPH(mess)->herport);
    NATip = NATmapinip(TCPH(mess)->herport);

#if NTRACE >= 7
    Nprintf("NAT: map in %u =>", TCPH(mess)->herport);
    Nprintf(" %u.%u.%u.%u.%u\n",
        NATip.c[0], NATip.c[1], NATip.c[2], NATip.c[3], NATport);
#endif

    memcpy(olddata, &TCPH(mess)->herport, 2);
    memcpy(olddata + 2, &IPH(mess)->to, 4);
    memcpy(newdata, &NATport, 2);
    memcpy(newdata + 2, &NATip, 4);

    if (NATprot == IP_TCP) {
        ussChecksumAdjust((unsigned char *)&TCPH(mess)->chksum,
            olddata, 6,
            newdata, 6);
    } else {
        ussChecksumAdjust((unsigned char *)&UDPH(mess)->chksum,
        olddata, 6,
        newdata, 6);
    }
    TCPH(mess)->herport = NATport;

   /* next, modify destination IP, and recalculate IP header checksum */
    memcpy((char *)&IPH(mess)->to, (char *)&NATip, Iid_SZ);

    return 1;
}

/*
** * * * * * *
** ussNATinit()    Initialize NAT information
**
** void ussNATinit(void);
**
** DESCRIPTION:
**   This function initializes the NAT table
**
** * * * * * *
** MODIFICATION HISTORY:
**   06-MAY-1999  BSK  Included in source
** * * * * * *
*/
void ussNATinit(void)
{
    int i1;

#if NTRACE >= 7
    Nprintf("NAT: Initializing . . . ");
#endif
    for (i1 = 0; i1 < NATTABLESZ; i1++) {
        NatTable[i1].truePort = 7;
        NatTable[i1].timestamp = 0;
    }
#if NTRACE >= 7
    Nprintf("Done!\n");
#endif
}

