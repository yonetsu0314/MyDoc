/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/ip.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: ip.c $
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
/* IP.C  Edition: 18  Last changed: 21-Sep-99,18:29:30  By: STEVE */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995, 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    IP.C -- Internet Protocol for USNET

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.

** * * * * * *
 MODIFICATION HISTORY
    21-SEP-1999  SCD  Change screen, memcpy to memmove in fragment assembly
    24-AUG-1999  BSK  Fix: Fragmentation over PPP with protocol compression
    14-JUL-1999  BSK  Added NAT
    21-APR-1999  SCD  Added relay of messages to Proxy ARP host to screen()
    30-DEC-1998  RCA  Added ioctl stub  
    09-NOV-1998  BSK  Removed open/close link layer call.
    27-OCT-1998  TTT  Changed IP Screen to not send ICMP protocol unreachable
                      in response to broadcast and to accept message from a
                      source address of 0.0.0.0 if DHCP is enabled.
** * * * * * */

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

static unsigned char TTL = 64;  /* initial value for time to live */
static unsigned char TOS = 0;   /* type of service field */
static unsigned char MAXFRAG = 3;   /* maximum messages in the fragment queue */

int ussHostGroupCheck(Iid iid, int netno);

/* We assume that the compiler will not place any padding into the following
   structure.  (If it did, structures would be pretty much useless.)
*/
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
    unsigned char   from[Iid_SZ];   /* source name */
    unsigned char   to[Iid_SZ]; /* destination name */
};

/* RCA IPfragid is made public to share with igmp module.  Once this ip
   module allows other modules to specify TTL, IPfragid can be made
   private again (preface with static). */
unsigned short IPfragid; /* Datagram IP ID number */

#define IP_VER 4                /* internet version */
#define ET_IP NC2(0x0800)       /* protocol code */

#define MH(mb) ((struct Ihdr *)mb)

#define IPOFFSET (MESSH_SZ+LHDRSZ)  /* IP header offset */

#ifndef LITTLE
#define PUTL(to, from) memcpy(to, from.c, 4)
#else
#define PUTL(to, from) to[0] = from.c[3], to[1] = from.c[2], \
    to[2] = from.c[1], to[3] = from.c[0]
#endif

extern struct NET nets[];
extern struct CONNECT connblo[];
extern PTABLE  *const P_tab[];
extern PTABLE   ussIPTable;
extern struct ifgroup ifgroup;

static int      fragid;
#ifdef MIB2
struct IPgroup  IPgroup;
#endif

#ifdef NAT
void ussNATinit(void);
int ussNATmapout(MESS *mess);
int ussNATmapin(MESS *mess);
#endif

/* IP is connectionless and does not need this */
static int opeN(int conno, int flags)
{
    (void)conno;
    (void)flags;
    return 1;
}

/* IP is connectionless and does not need this */
static int closE(int conno)
{
    (void)conno;
    return 0;
}


/*
** * * * * * *
** writE()  Create IP encapsulation and write to lower layer
**
** static int writE(int conno, MESS *mess)
**
** PARAMETERS:
**   (in/out) mess             A pointer to a message to encapsulate
**
** RETURNS:
**  -1                         error
**   number characters sent    success
**
** DESCRIPTION:
**   This function builds the IP header and then calls the link layer write.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**  24-AUG-1999  BSK  Fix:  Reset protocol field for fragmentation over PPP
**  09-NOV-1998  BSK  Created comment
** * * * * * *
*/
static int writE(int conno, MESS *mess)
{
    int i1, hdroff, ihdrsz;
    unsigned short us1;
    struct NET *netp;
    unsigned char *cp, *cp2;
#ifdef IPOPTIONS
    Iid iid;
#endif
#if FRAGMENTATION & 2
    unsigned short totdat, maxdat;
    MESS *mp;
#endif

#ifdef MIB2
    IPgroup.ipOutRequests++;
#endif

    netp = &nets[mess->netno];
    mess->offset = IPOFFSET;

   /* conno = -2 means we are relaying.  Relay will not build the IP header,
   ** but may need fragmentation. */
    if (conno < 0) {
        if (conno == -2)
            goto lab6;
        mess->confix = GetHostData(mess->target, 1);
    } else {
        mess->confix = connblo[conno].confix;
    }
    us1 = IPfragid++;           /* Increment IP ID number */
    MH(mess)->frid = NC2(us1);  /* Place ID in header */

   /* if not broadcast and ARP entry not static, we refresh the ARP entry,
    ** create if necessary and possible */
    if ((mess->confix != ussBroadcastIndex) &&
        (mess->confix != ussMulticastIndex)) {
        if (netconf[mess->confix].ncstat != 4)
            if ((mess->confix = GetHostData(mess->target, 3)) == 255)
              	return -1;
    }

   /* build the IP header */
    MH(mess)->verlen = (IP_VER << 4) | (Ihdr_SZ >> 2);
#ifdef MIB2
    MH(mess)->time = IPgroup.ipDefaultTTL;
#else
    MH(mess)->time = TTL;
#endif
    MH(mess)->fragm = 0;
    MH(mess)->tservice = TOS;
    memcpy((char *) &MH(mess)->to, (char *) &mess->target, Iid_SZ);
    memcpy((char *) &MH(mess)->from, (char *) &netconf[netp->confix].Iaddr,
           Iid_SZ);
#ifdef IPOPTIONS
    if (conno >= 0) {
       /* take possible options */
        i1 = connblo[conno].IPOtxlen;
        MH(mess)->verlen = (IP_VER << 4) | ((Ihdr_SZ + i1) >> 2);
       /* if source routing, we have to send to first in route list */
        if (i1 > 0) {
            cp = (unsigned char *) connblo[conno].IPOtxopt;
            memcpy((char *) MH(mess) + Ihdr_SZ, cp, i1);
            if (cp[0] == 0x83 || cp[0] == 0x89) {
                memcpy((char *) &iid.l, cp + 3, Iid_SZ);
                if ((i1 = GetHostData(iid.l, 1)) >= 0) {
                    if (cp[0] == 0x83 || netconf[i1].hops == 1)
                        mess->confix = i1;
                } else if (cp[0] == 0x89) {
#ifdef MIB2
                    IPgroup.ipOutNoRoutes++;
#endif
                    return EHOSTUNREACH;
                }
            }
        }
    }
#endif
    i1 = mess->mlen - IPOFFSET;
    MH(mess)->tlen = NC2(i1);

lab6:
    MH(mess)->chksum = 0;
    ihdrsz = (MH(mess)->verlen & 0xf) * 2;
    MH(mess)->chksum = ~Nchksum((unsigned short *) ((char *) mess + IPOFFSET),
                                ihdrsz);
    *(short *) ((char *) mess + MESSH_SZ + 12) = ET_IP;

   /* check if room enough on the wire, or if fragmentation is needed */
    if (mess->mlen - IPOFFSET <= netp->maxblo) {
        return netp->protoc[0]->writE(conno, mess);
    }

   /* Here message does not fit in one packet.  If fragmentation is
    ** configured, send it in fragments using a scratch buffer.  Otherwise
    ** return an error. */
#if (FRAGMENTATION & 2) == 0
    return NE_PARAM;
#else
    if ((mp = Ngetbuf()) == 0) {
#ifdef MIB2
        IPgroup.ipFragFails++;
#endif
        return NE_NOBUFS;
    }
#ifdef MIB2
    IPgroup.ipFragOKs++;
#endif
    mp->netno = mess->netno;
    mp->confix = mess->confix;
    ihdrsz = (MH(mess)->verlen & 0xf) * 4;
    memcpy((char *) mp + IPOFFSET - 2, (char *) mess + IPOFFSET - 2, ihdrsz + 2);
    maxdat = (netp->maxblo - ihdrsz) & ~7;
    cp = (unsigned char *) mess + IPOFFSET + ihdrsz;
    totdat = mess->mlen - IPOFFSET - ihdrsz;
    for (hdroff = 0x2000; totdat > 0; cp += maxdat) {
        i1 = totdat > maxdat ? maxdat : totdat;
        totdat -= i1;
        mp->mlen = i1 + ihdrsz + mess->offset;
        memcpy((char *) mp + mess->offset + ihdrsz, cp, i1);
       /*
        ** PPP may compress the protocol field, so subsequent transmissions
        ** of this field must be reset each time.
        */
        *(short *)((char *)mp + MESSH_SZ + 12) = ET_IP;
        if (totdat == 0) {
            hdroff &= ~0x2000;
            mess->offset = boTXDONE;
        }
        i1 += ihdrsz;
        MH(mp)->tlen = NC2(i1);
        MH(mp)->fragm = NC2(hdroff);
        hdroff += maxdat >> 3;
        MH(mp)->chksum = 0;
        MH(mp)->tservice = TOS;
        MH(mp)->chksum = ~Nchksum((unsigned short *) ((char *) mp + IPOFFSET),
                                  ihdrsz / 2);
#ifdef MIB2
        IPgroup.ipFragCreates++;
#endif
        i1 = netp->protoc[0]->writE(conno, mp);
        if (i1 < 0)
            break;
    }
    Nrelbuf(mp);
    return i1;
#endif
}


/** * * * * * *
** screen()  Screen an IP layer datagram
**
** static int screen(MESS *mess);
**
** PARAMETERS:
**   (in/out) mess             A pointer to a message to screen
**
** RETURNS:
**  -4                         do not release
**  -2                         no further action
**  -1                         error occured
**   n                         please enter in queue number n
**
** DESCRIPTION:
**   This screens the datagram and relays it or sends it to another layer.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   21-SEP-1999  SCD  Changed memcpy to memmove in fragment assembly
**   21-APR-1999  SCD  Added relay of messages to Proxy ARP host
**   18-JAN-1999  RCA  Added support for multicast
**   09-NOV-1998  BSK  Created comment
** * * * * * *
*/
static int screen(MESS *mess)
{
    int i1, i2, netno, nxtlev, confix, ihdrsz, bcast;
    struct NETCONF *ownconfp, *confp;
    Iid iid, iid2;
    struct NET *netp;
    unsigned char *bp, *bp2;
#ifdef IPOPTIONS
    union {
        unsigned char c[4];
        short s[2];
        long l;
    } UL1;
#endif
#if FRAGMENTATION & 1
    MESS *mp, *mp3;
    unsigned short holebelow, databelow, holeabove, dataabove, holebeyond;
    unsigned short base, length, us3;
#endif

#ifdef MIB2
    IPgroup.ipInReceives++;
#endif

   /* build some useful temporary variables */
    netno = mess->netno;
    netp = &nets[netno];
    i1 = NC2(MH(mess)->tlen) + IPOFFSET;
    memcpy((char *) &iid, &MH(mess)->to, Iid_SZ);

   /* Check for any clear signs of a bad message. */
    if (mess->mlen < (unsigned int) i1)
        goto err1;
    mess->mlen = i1;
    if (MH(mess)->verlen >> 4 != IP_VER)
        goto err1;
    ihdrsz = (MH(mess)->verlen & 0xf) * 4;
    if (Nchksum((unsigned short *) ((char *) mess + IPOFFSET), ihdrsz / 2) != 0xffff)
        goto err1;
    ownconfp = &netconf[netp->confix];

   /* Find sender in the configuration table, create entry if not there. */
    memcpy((char *) &mess->target, &MH(mess)->from, Iid_SZ);
	confix = GetHostData(mess->target, 0x17);
    if (confix == -1) {
       /* if sender's IP address is 0, we'll use the hardware address */
        if (mess->target)
            goto err1;
        for (confix = 0; confix < NCONFIGS; confix++) {
            confp = &netconf[confix];
            if (confp->netno != netno)
                continue;
            if (confp->flags & (LOCALHOST + NOTUSED))
                continue;
            if (netp->sndoff == 0 || memcmp((char *) &confp->Eaddr,
                                 (char *) mess + netp->sndoff, Eid_SZ) == 0) 
                goto lab6;
        }
#if DHCP == 2
       /* if using DHCP, create a fake netconf entry if one isn't there yet */
        confix = GetHostData((netconf[netp->confix].Iaddr.l & 
                              netconf[netp->confix].Imask.l), 0x17);
		memcpy(netconf[confix].Eaddr.c, (char *) mess + netp->sndoff, Eid_SZ);
#else
        goto err1;
#endif
    }

lab6:
    mess->confix = confix;
    confp = &netconf[confix];
    if (confp->nexthix == 255) {
       /* If the remote host is completely unknown, we store the sender as
        ** first hop.  We'll eventually get a redirect message if this is
        ** not the best route. */
        confp->nexthix = confix;
        memcpy((char *) &confp->Eaddr, (char *) mess + netp->sndoff, Eid_SZ);
        confp->hwaddyes = 2;
        confp->netno = netno;
        confp->hops = 253;
    }
    netconf[confp->nexthix].timems = TimeMS();  /* timestamp for ARP */

   /* If the target Internet address is ours, or broadcast to us, we accept
    ** the message. */
    bcast = mess->conno;        /* broadcast indicator */
    if (iid.l == ownconfp->Iaddr.l) /* home address */
        goto lab8;
    if (iid.l == 0xffffffff)    /* all points */
        goto lab88;
    i2 = (iid.c[0] & 0xc0) >> 3;/* for class broadcast */
    if (i2 == 0)
        i2 = 8;
    for (i1 = 0; i1 < NNETS; i1++) {    /* check all local addresses */
        if (nets[i1].netstat == 0)
            continue;
        confp = &netconf[nets[i1].confix];
        iid2.l = confp->Iaddr.l;
        if (iid.l == iid2.l)    /* non-home */
            goto lab8;
        if (iid.l == (iid2.l | ~confp->Imask.l))    /* subnet */
            goto lab88;
        if (iid.l == (iid2.l | (0xffffffffUL >> i2)))  /* class */
            goto lab88;
    }
#if USS_IP_MC_LEVEL == 2
    if (ussHostGroupCheck(iid, mess->netno))
        goto lab8;
#endif
    if (ownconfp->Iaddr.l == 0)  /* own address unknown */
        goto lab8;
    if (iid.l == 0)
        goto err1;

/* This was not for us, see if we can relay.  We must be a gateway.  The
 ** hop count in the message can't be exhausted.  It can't be a broadcast. */
#if RELAYING != 1
#ifdef MIB2
    IPgroup.ipInAddrErrors++;
#endif
    goto err1;
#else
#ifdef MIB2
    if (IPgroup.ipForwarding != 1)
        goto err1;
#endif
    if (MH(mess)->time-- == 0)
        goto err1;
    if (bcast)
        goto err1;

#ifdef NAT
checkrelay:
#endif

#ifdef IPOPTIONS
   /* handle possible source routing here */
    for (bp = (unsigned char *) mess + IPOFFSET + Ihdr_SZ;
         bp < (unsigned char *) mess + IPOFFSET + ihdrsz; bp += i1) {
        if (bp[0] == 0)
            break;
        i1 = 1;
        if (bp[0] == 1)
            continue;
        i1 = (unsigned char) bp[1];
        if (bp[0] == 68) {      /* timestamp */
           /* Timestamp pointer is base 1.  None of the other options use
            ** base 1 pointers so this is hard to believe, but that's what
            ** RFC791 says. */
            i2 = (unsigned char) bp[2] - 1;
            if (i2 < i1) {
                if (i2 + 4 < i1)
                    goto err1;
                nxtlev = bp[3] & 0xf;
                if (nxtlev == 3) {  /* only if IP address matches */
                    if (memcmp((char *) bp + i2, (char *) ownconfp->Iaddr.c,
                               Iid_SZ) != 0)
                        continue;
                    i2 += 4;
                } else if (nxtlev == 1) {   /* store own IP address */
                    memcpy((char *) bp + i2, (char *) ownconfp->Iaddr.c, Iid_SZ);
                    i2 += 4;
                    bp[2] += 4;
                }
                if (i2 + 4 < i1)
                    goto err1;
                UL1.l = TimeMS();
                PUTL((bp + i2), UL1);
                bp[2] += 4;
            } else {
                if (bp[3] >= 0xf0)  /* timestamp overflow */
                    goto err1;
                bp[3] += 0x80;
            }
        } else if (bp[0] == 7) {/* record route */
            i2 = (unsigned char) bp[2];
            if (i2 < i1) {
                if (i2 + 4 < i1)
                    goto err1;
                memcpy((char *) bp + i2, (char *) ownconfp->Iaddr.c, Iid_SZ);
                bp[2] += 4;
            }
        }
       /* 3 = loose routing, 9 = strict routing */
        else if (bp[0] == 0x83 || bp[0] == 0x89) {
            i2 = (unsigned char) bp[2] - 1; /* one-based!! */
            if (i2 < i1) {
               /* Send to next router if that is connected.  Loose routing
                ** sends to next router even if that is not directly
                ** connected, and tries the local routing tables if the
                ** router in the list is totally unknown. */
                if (i2 + 4 < i1)
                    goto err1;
                memcpy((char *) &iid2, bp + i2, Iid_SZ);
                if ((confix = GetHostData(iid2.l, 1)) >= 0) {
                    if (bp[0] == 0x83 || netconf[confix].hops == 1) {
                        memcpy((char *) bp + i2, (char *) ownconfp->Iaddr.c,
                               Iid_SZ);
                        bp[2] += 4;
                        goto relay;
                    }
                }
            }
            if (bp[0] == 0x89) {
                ICMPreply(mess, 3, 5);
                goto noroute;
            }
        }
    }
#endif

   /* This is normal free routing, we consult our routing tables. */
    if ((confix = GetHostData(iid.l, 1)) < 0) {
        ICMPreply(mess, 3, 0);
noroute:
#ifdef MIB2
        IPgroup.ipOutNoRoutes++;
#endif
        return -1;
    }

/* This is where we try to relay to a better place. */
relay:
    confp = &netconf[confix];

#ifdef MIB2
    IPgroup.ipForwDatagrams++;
#endif
    mess->confix = confix;
  
#ifdef USS_PROXYARP
    if (mess->netno == confp->netno) {
        int i, j;
        struct NETCONF *confp2, *confp3;
        /*  Look for a match for the internet address  */
        for (i = 0; i < NCONFIGS; i++) {
            confp2 = &netconf[i];
            if (confp2->Iaddr.l == iid.l) {
                /*  Look for a match for the proxy port name  */
                for (j = 0; j < NCONFIGS; j++) {
                    confp3 = &netconf[j];
                    if (!strcmp(confp2->pname, confp3->pname)
                            && (confp3->flags & LOCALHOST)) {
                        mess->netno = confp3->netno;
                        break;
                    }
                }
                break;
            }
        }
    } else
#endif
        mess->netno = confp->netno;

    mess->target = confp->Iaddr.l;
#if NTRACE >= 2
    Nprintf("RL %d->%d %d.%d.%d.%d\n", netno, mess->netno,
            iid.c[0], iid.c[1], iid.c[2], iid.c[3]);
#endif

#ifdef NAT
   /* If we can map the address and port */
    if (ussNATmapout(mess))
        goto lab5;
#endif

   /* if fragmentation needed, check that it is possible */
    i2 = nets[mess->netno].maxblo;
    if (mess->mlen - IPOFFSET > i2) {
#if FRAGMENTATION & 2
        if (MH(mess)->fragm & NC2(0x4000))
#endif
        {
#ifdef MIB2
            IPgroup.ipFragFails++;
#endif
            mess->mlen = i2;
            ICMPreply(mess, 3, 4);
            return -2;
        }
lab5:
        mess->id = bRELEASE;
        if (writE(-2, mess))
            return -2;
        return -4;
    }
/* If there are IP header options (rarely used), we have to relculate the
 ** checksum, so the shortcut will not work. */
#ifdef IPOPTIONS
    if (ihdrsz != Ihdr_SZ)
        goto lab5;
#endif

   /* No fragmentation and no IP options, we write directly from here for
    ** speed, updating but not recalculating the checksum. */
    if ((MH(mess)->chksum += NC2(0x0100)) < NC2(0x0100))
        MH(mess)->chksum += NC2(1);
    mess->id = bRELEASE;
    *(short *) ((char *) mess + MESSH_SZ + 12) = ET_IP;
    if (nets[mess->netno].protoc[0]->writE(-2, mess))
        return -2;
    return -4;
#endif

/* Here the message is meant for us.  We find and possibly create the
 ** configuration entry. */
lab88:
    bcast = 1;
lab8:
#ifdef NAT
   /* If we are able to find a local host that uses this address and port */
    if (ussNATmapin(mess)) {
        memcpy((char *)&iid, &MH(mess)->to, Iid_SZ);    /* New destination */
        bcast = 0;
        ihdrsz = 0;
        goto checkrelay;        /* will recalculate IP header checksum */
    }
#endif

    mess->offset = IPOFFSET + ihdrsz;

#if FRAGMENTATION & 1
   /* Fragmented messages go into the fragment queue until we have the entire
    ** message. */
    if (MH(mess)->fragm & NC2(0xbfff)) {
#ifdef MIB2
        IPgroup.ipReasmReqds++;
#endif
       /* Base is offset in the collection buffer from start of IP header;
        ** length the amount to be copied;
        ** Fragment at zero includes header. */
        length = NC2(MH(mess)->tlen);
        base = NC2(MH(mess)->fragm) << 3;
        bp2 = (unsigned char *) mess + IPOFFSET;
        if (base != 0) {
            base += Ihdr_SZ;
            length -= Ihdr_SZ;
            bp2 += Ihdr_SZ;
        }
       /* Scan collection buffer list for ours, go to end if new. */
        mp3 = (MESS *) & netp->fragmq;
        nxtlev = -4;
        for (i2 = 0, mp = mp3->next; mp; i2++, mp3 = mp, mp = mp->next) {
            if (memcmp(&MH(mess)->from, &MH(mp)->from, Iid_SZ))
                continue;
            if (MH(mess)->frid == MH(mp)->frid)
                goto lab3;
        }
        if (i2 >= MAXFRAG || base + length > MAXBUF - IPOFFSET) {
#ifdef MIB2
            IPgroup.ipReasmFails++;
#endif
            return -2;
        }
       /* New, we link into collection queue, set parameters for that case. */
        mp3->next = mess;
        mess->next = 0;
        bp = (unsigned char *) mess + IPOFFSET;
        mp = mess;
        mp->mlen = holebeyond = dataabove = 0xffff;
        databelow = base;
        holeabove = base + length;
        *(unsigned short *) ((char *) mp + MESSH_SZ) = holebelow = 0;
        goto lab4;
lab3:
       /* Old, scan to find hole below and hole above. */
        nxtlev = -2;
        if (base + length > MAXBUF - IPOFFSET) {
            mp3->next = mp->next;
            Nrelbuf(mp);
#ifdef MIB2
            IPgroup.ipReasmFails++;
#endif
            return -2;
        }
        bp = (unsigned char *) mp + IPOFFSET;
        holebelow = us3 = *(unsigned short *) ((char *) mp + MESSH_SZ);
        for (; us3 < base; us3 = *(unsigned short *) (bp + ((us3 + 1) & ~1)))
            holebelow = us3;
        if ((databelow = *(unsigned short *) (bp + ((holebelow + 3) & ~1))) > base)
            databelow = base;
        us3 = holebelow;
        for (; (dataabove = *(unsigned short *) (bp + ((us3 + 3) & ~1))) <= base + length;)
            us3 = *(unsigned short *) (bp + ((us3 + 1) & ~1));
        holeabove = base + length;
        if (holeabove < us3)
            holeabove = us3;
        holebeyond = *(unsigned short *) (bp + ((us3 + 1) & ~1));
lab4:
       /* Copy the data, put the hole descriptors in place. */
        memmove(bp + base, bp2, length);
        if (holebelow < databelow) {
            *(unsigned short *) (bp + ((holebelow + 1) & ~1)) = holeabove;
            *(unsigned short *) (bp + ((holebelow + 3) & ~1)) = databelow;
        } else
            *(unsigned short *) ((char *) mp + MESSH_SZ) = holeabove;
        *(unsigned short *) (bp + ((holeabove + 1) & ~1)) = holebeyond;
        *(unsigned short *) (bp + ((holeabove + 3) & ~1)) = dataabove;
        mp->timems = TimeMS();
       /* Check for complete message. */
        if ((MH(mess)->fragm & NC2(0x2000)) == 0)
            mp->mlen = base + length;
        if (*(unsigned short *) ((char *) mp + MESSH_SZ) < mp->mlen)
            return nxtlev;
       /* Yes we have it, take from queue, adjust pointers. */
        mp->mlen += IPOFFSET;
        mp3->next = mp->next;
        Nrelbuf(mess);
        netp->fragmh = mp;
        mess = mp;
#ifdef MIB2
        IPgroup.ipReasmOKs++;
#endif
    }
#endif

   /* There's a message for us, get the higher protocol number and call
    ** screen() for that protocol.  Offset to Internet addresses (stored
    ** for the call in mess->conno) is needed in TCP/UDP screen for the
    ** checksum. */
    mess->conno = IPOFFSET + 12;
    i1 = MH(mess)->prot;
    for (nxtlev = 1; P_tab[nxtlev]; nxtlev++) 
        if (i1 == P_tab[nxtlev]->Eprotoc) {
#ifdef MIB2
            IPgroup.ipInDelivers++;
#endif
            i1 = P_tab[nxtlev]->screen(mess);
            if (i1 == -6 && bcast == 0) /* no port, send ICMP if not bcast */
                ICMPreply(mess, 3, 3);
            if (i1 == -3) {
                writE(-1, mess);
                i1 = -2;
            }
            return i1;
        }
#ifdef MIB2
    IPgroup.ipInUnknownProtos++;
#endif
	if (bcast == 0) 	/* send ICMP if not broadcast */
	    ICMPreply(mess, 3, 2);
    return -1;

err1:
#ifdef MIB2
    IPgroup.ipInHdrErrors++;
#endif
    return -1;
}


/** * * * * * *
** static MESS *reaD(int conno)
**
** PARAMETERS:
**   (in) conno                a USNET connection number
**
** RETURNS:
**  -1                         error
**   message address           success
**
** DESCRIPTION:
**   This waits until the screener has queued a coming message for the
**   particular connection.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   09-NOV-1998  BSK  Created comment
** * * * * * *
*/
static MESS *reaD(int conno)
{
    int i1;
    MESS *mess;
    register struct CONNECT *conp;

    (void) i1;
    conp = &connblo[conno];
    WAITFOR(conp->first || (conp->rxstat & (S_EOF + S_FATAL)), SIG_RC(conno),
            conp->rxtout, i1);
    if (!conp->first)
        return 0;
    Ndisable();
    MESS_OUT(conp, mess);
    Nenable();
#ifdef IPOPTIONS
    conp->IPOrxlen = (((*((char *) mess + IPOFFSET)) & 0xf) << 2) - Ihdr_SZ;
    memcpy(conp->IPOrxopt, (char *) mess + IPOFFSET + Ihdr_SZ, conp->IPOrxlen);
#endif
    conp->realiid.l = mess->target;
    return mess;
}


/** * * * * * *
** init()  Initialize the protocol
**
** static int init(int netno, char *params)
**
** PARAMETERS:
**   (in) netno                A USNET network (interface) number
**   (in) params               A list of parameters (ignored)
**
** RETURNS:
**   0                         success
**
** DESCRIPTION:
**   This initializes the MIB data for the IPgroup if using SNMP.
**   Otherwise, it doesn't do anything.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   09-NOV-1998  BSK  Created comment
** * * * * * *
*/
static int init(int netno, char *params)
{
    (void) netno;
    (void) params;
   /* initial value irrelevant but some systems give an error otherwise */
    fragid = 0;
#ifdef MIB2
    memset(&IPgroup, 0, sizeof(IPgroup));
    IPgroup.ipForwarding = RELAYING;
    IPgroup.ipDefaultTTL = TTL;
    IPgroup.ipReasmTimeout = 64;
#endif
#ifdef NAT
    ussNATinit();
#endif
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg, size_t size);
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


/* Protocol table for Internet. */
GLOBALCONST PTABLE ussIPTable = {
    "IP", init, 0, screen, opeN, closE, reaD, writE, ioctl, ET_IP, Ihdr_SZ
};

/*****************************************************************
			ÇhÇo initial value for time to live ê›íËèàóù
*****************************************************************/
void             UsnetIPSetTTL( unsigned char ttl )
{
	TTL = ttl;
}
/*****************************************************************
			ÇhÇo initial value for time to live éÊìæèàóù
*****************************************************************/
unsigned char    UsnetIPGetTTL( void )
{
	return TTL;
}
/*****************************************************************
			ÇhÇo type of service field ê›íËèàóù
*****************************************************************/
void             UsnetIPSetTOS( unsigned char tos )
{
	TOS = tos;
}
/*****************************************************************
			ÇhÇo type of service field éÊìæèàóù
*****************************************************************/
unsigned char	UsnetIPGetTOS( void )
{
	return TOS;
}
/*****************************************************************
			ÇhÇo maximum messages in the fragment queue ê›íËèàóù
*****************************************************************/
void             UsnetIPSetMAXFRAG( unsigned char maxfrag )
{
	MAXFRAG = maxfrag;
}
/*****************************************************************
			ÇhÇo maximum messages in the fragment queue éÊìæèàóù
*****************************************************************/
unsigned char	UsnetIPGetMAXFRAG( void )
{
	return MAXFRAG;
}
