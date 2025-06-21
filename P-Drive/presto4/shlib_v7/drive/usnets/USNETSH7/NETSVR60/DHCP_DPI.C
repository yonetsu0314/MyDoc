/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/dhcp_dpi.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: dhcp_dpi.c $
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
/* DHCP_DPI.C  Edition: 6  Last changed: 23-Sep-99,17:54:54  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1996, 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    DHCP.C -- Dynamic Host Configuration Protocol for USNET

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.

** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** MODIFICATION HISTORY
**
** 23-SEP-1999 TTT  Set netconf IP address to 0 in DHCPrelease()
** 23-AUG-1999 TTT  Fix address probe for multiple interfaces
** 12-AUG-1999 TTT  Use unicast IP address when renewing
** 17-FEB-1999 TTT  Handle adding router correctly
** 29-JAN-1999 TTT  Ged rid of YIELD() between Nwrite and Nread
** 23-NOV-1998 TTT  Get rid of erroneous Nrelbuf() 
** 27-OCT-1998 TTT  Bug fixes + bring up to RFC 2131 specification
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

#define FILENAMESIZE 80         /* change if name longer */

#define MAXDELAY 30000          /* maximum delay in retry loop */

#define DHCPDISCOVER 	1
#define DHCPOFFER 	2
#define DHCPREQUEST 	3
#define DHCPDECLINE 	4
#define DHCPACK 	5
#define DHCPNACK 	6
#define DHCPRELEASE 	7

struct DHCPstr {                /* DHCP structure */
    unsigned char   op;         /* operation */
    unsigned char   htype;      /* hardware type */
    unsigned char   hlen;       /* hardware address length */
    unsigned char   hops;       /* gateway hops */
    unsigned long   xid;        /* transaction ID */
    unsigned short  secs;       /* seconds since boot began */
    unsigned short  flags;      /* flags */
    unsigned long   ciaddr;     /* client IP address */
    unsigned long   yiaddr;     /* 'your' IP address */
    unsigned long   siaddr;     /* server IP address */
    unsigned long   giaddr;     /* gateway IP address */
    unsigned char   chaddr[16]; /* client hardware address */
    unsigned char   sname[64];  /* server host name */
    unsigned char   file[128];  /* boot file name */
    unsigned char   options[312];   /* options */
};

#define DHCPsz 236              /* size without options */

#define TIMEINMS(sec, curtime) \
    sec > 0x7fffffffUL/1000UL ? curtime + 0x7fffffffUL : sec*1000UL + curtime

extern struct NETCONF netconf[];
extern struct CONNECT connblo[];
extern struct NET nets[];
#ifdef MIB2
extern struct IPgroup IPgroup;
#endif
#ifdef DNS
extern Iid      DNSiid[2];
extern char     DNSdomain[];
#endif

char            DHCPbootfile[FILENAMESIZE]; /* boot file name, NOT REENTRANT */

static const char discopts[] = {99, 130, 83, 99, 53, 1, DHCPDISCOVER};
static const char releopts[] = {99, 130, 83, 99, 53, 1, DHCPRELEASE};

/* internal routine to return pointer to next option, 0 at end */

#define InitOption(len, DHCPp) \
    DHCPp->htype = DHCPp->op = 0, DHCPp->flags = DHCPsz + 4, \
    DHCPp->secs = len - 4 - DHCPsz


static unsigned char *NextOption(struct DHCPstr * DHCPp)
{
    unsigned char   optype,
                    oplen,
                   *cp;

aga1:
    while ((short) DHCPp->secs <= 0) {
        if (DHCPp->op & 1) {
            DHCPp->flags = 108;
            DHCPp->secs = sizeof(DHCPp->file);
            DHCPp->op &= ~1;
        } else if (DHCPp->op & 2) {
            DHCPp->flags = 44;
            DHCPp->secs = sizeof(DHCPp->sname);
            DHCPp->op &= ~2;
        } else
            return 0;
    }
    cp = (unsigned char *) DHCPp + DHCPp->flags;    /* ??? changed type cast */
    optype = cp[0];
    DHCPp->secs--;
    if (optype == 0)            /* fill */
        goto aga1;
    oplen = cp[1];
    DHCPp->secs -= oplen;
    DHCPp->flags += oplen + 2;
    if (optype == 255 || (short) DHCPp->secs < 0) { /* end */
        DHCPp->secs = 0;
        goto aga1;
    }
    if (optype == 52) {         /* option overload */
        DHCPp->htype = DHCPp->op = cp[2];
        goto aga1;
    }
    return cp;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** DHCPget: User routine to get DHCP bootload information
**
** int DHCPget(int netno, unsigned long lease)
**
** PARAMETERS: 
**    (in) netno: index into network configuration table
**    (in) lease: desired length of lease
** 
** DESCRIPTION: 
**    DHCPget contacts a DHCP server to obtain an IP address and other
**    network parameters for a host. Note that this client does not 
**    automatically obtain configuration parameters (such as network
**    mask, DNS server, etc) other than the IP address. See DHCP.TXT
**    in the DOC directory for further information on how to do this.
**    
** RETURNS:
**    < 0:  Error given by error code
**      0:  No action needed (lease doesn't need renewing)
**    > 0:  IP address allocated or renewed
**
**
** COMMENTS: This is the DPI version of the USNET DHCP client. The
**    latest version uses the BSD sockets interface. The USNET codesize
**    increases by about 10K when using the BSD interface, so this
**    version of the DHCP client is included in case that amount of
**    codesize is important. This version of the DHCP client should
**    be considered unsupported.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

int             DHCPget(int netno, unsigned long lease)
{
    int             i1,
                    i2,
                    renew,
                    retry,
                    stat,
                    oplen,
                    len,
                    conno,
                    confix;
    unsigned long   ul1,
                    initime,
                    alltime,
                    xid,
                    inidelay,
                    delay;
    unsigned char   uc1,
                   *cp;
    struct NETCONF *confp,
                   *confp2;
    struct NET     *netp;
    Iid             iid;
    struct DHCPstr *DHCPrec,				  
                   *bootsend,
                    DHCPr,
                    boot;
    MESS           *mp;
    union {
        unsigned char   c[4];
        short           s[2];
        long            l;
    }               UL2;
    const struct Eid ezero = {0,0,0,0,0,0};

    renew = retry = 0;
    DHCPbootfile[0] = 0;
    xid = initime = TimeMS();
    netp = &nets[netno];
    confp = &netconf[netp->confix];
    bootsend = &boot;
    DHCPrec = &DHCPr;

/* "random" delay, must be between 0 and 4 seconds */
    for (uc1 = stat = 0; stat < Eid_SZ; stat++)
        uc1 += confp->Eaddr.c[stat];
    inidelay = (unsigned int) uc1 << 4;

    if (netp->DHCPserver) {     /* if we already used DHCP */
        if ((long) (initime - netp->RenewalTime) < 0) { /* no action needed */
            stat = 0;
            netp->DHCPsecs = (netp->RenewalTime - initime) >> 10;
            goto ret8;
        }
        if ((long) (initime - netp->LeaseTime) > 0) {   /* too late */
#if NTRACE >= 1
            Nprintf("DHCP %d address expired\n", netno);
#endif
            netp->DHCPserver = 0;
            netconf[nets[netno].confix].Iaddr.l = 0;
            stat = ETIMEDOUT;
            goto ret8;
        }
        if ((long) (initime - netp->RebindingTime) < 0) /* renew only */
            renew = 1;
    }
/* open the connection block */

restart:
    if (renew == 0) {
        Nsprintf((void *) bootsend, "*/%s", confp->pname);
        conno = Nopen((void *) bootsend, "UDP/IP", 68, 67, S_NOCON);     
        connblo[conno].heriid.l = 0xffffffff;   /* limited broadcast */
    } else
        conno = Nopen((char *) &netp->DHCPserver, "UDP/IP", 68, 67, IPADDR);
    if (conno < 0) {
        stat = conno;
        goto ret7;
    }
    SOCKET_RXTOUT(conno, 500);

/* create and write message, read answer, several times */
    memset(bootsend, 0, sizeof(struct DHCPstr));
    bootsend->op = 1;
    bootsend->htype = 1;
    bootsend->hlen = sizeof(struct Eid);
    bootsend->xid = xid++;
    bootsend->secs = (TimeMS() - initime) >> 10;
    memcpy((char *) &bootsend->ciaddr, (char *) &confp->Iaddr, Iid_SZ);
    memcpy(bootsend->chaddr, (char *) &confp->Eaddr, Eid_SZ);
    memcpy(bootsend->options, discopts, sizeof(discopts));
    len = sizeof(discopts);
    cp = bootsend->options + len;
    if (lease) {
        *cp++ = 51;
        *cp++ = 4;
        UL2.l = lease;
        PUTLONG(UL2, cp);
        cp += 4;
        len += 6;
    }
    if (confp->Iaddr.l) {
        *cp++ = 50;
        *cp++ = 4;
        PUTLONG(confp->Iaddr, cp);
        cp += 4;
        len += 6;
    }
    if (renew) {                /* just renewal? */
        *cp++ = 54;
        *cp++ = 4;
        memcpy(cp, (char *) &netp->DHCPserver, Iid_SZ);
        cp += 4;
        len += 6;
        goto renewaddress;
    }
    *cp = 255;
    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        Nwrite(conno, (char *) bootsend, DHCPsz + len + 1);
read3:  stat = Nread(conno, (char *) DHCPrec, sizeof(struct DHCPstr));
        if (stat > DHCPsz) {
            if (DHCPrec->xid != bootsend->xid)
                goto read3;
            if (DHCPrec->op != 2)
                goto read3;
            goto lab2;
        }
        SOCKET_CLEAR(conno);
        stat = ETIMEDOUT;
    } 
    goto ret7;

/* Scan the options, we need record type DHCPOFFER.  We take the first
   one as there is no strategy known to this level for selecting among
   several offers.
*/
lab2:
    InitOption(stat, DHCPrec);
    i1 = 0;
    while ((cp = NextOption(DHCPrec)) != 0) {
        i2 = cp[0];
        if (i2 == 53)  {
            if (cp[2] == DHCPOFFER)
                i1 = 1;
		}
        if (i2 == 54) {
            i2 = cp[1] + 2;
            memcpy(bootsend->options + len, cp, i2);
            len += i2;
        }
    }
    if (i1 == 0)
        goto read3;             /* wrong type, read again */

/* Send the same request, this time as DHCPREQUEST, and with server
   identifier option.  If no good answer, go back to the beginning.
*/
renewaddress:
    bootsend->options[6] = DHCPREQUEST; 
	if (!renew) {
/* Add requested IP address option */
		cp = bootsend->options + len;
		*cp++ = 50;
		*cp++ = 4;
		memcpy(cp, (char *) &DHCPrec->yiaddr, 4);
		cp += 4;
		len += 6;
	}
    bootsend->options[len] = 255;
    bootsend->xid = xid++;
    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        alltime = TimeMS();
        Nwrite(conno, (char *) bootsend, DHCPsz + len + 1);
read4:  stat = Nread(conno, (char *) DHCPrec, sizeof(struct DHCPstr));
        if (stat > DHCPsz) {
            if (DHCPrec->xid != bootsend->xid)
                goto read4;
            if (DHCPrec->op != 2)
                goto read4;
            InitOption(stat, DHCPrec);
            while ((cp = NextOption(DHCPrec)) != 0) {
                if (cp[0] == 53) {
                    if (cp[2] == DHCPACK)
                        goto lab4;
                    if (cp[2] == DHCPNACK)
                        goto waitstart;
                }
            }
        }
    }
waitstart:
    if (retry++ > 5) {
        stat = -1;
        goto ret7;
    }
    Nclose(conno);
    for (ul1 = TimeMS(); TimeMS() - ul1 < 10000;)
        YIELD();
    renew = 0;
    goto restart;

lab4:

/* Use ARP to find out if this address might be in use already.  We send a
   message stub also, because this is less trouble than open/write/read for
   ARP.  This shouldn't hurt as we normally never get past the wait for ARP.
*/

#ifdef ARP
    if (netp->sndoff && DHCPrec->yiaddr != confp->Iaddr.l) {
        confp->Iaddr.l = 0;
        /* create fake ARP cache entry -> subnet in table -> can do ARP */
        confix = GetHostData((DHCPrec->yiaddr & confp->Imask.l), 0x17);
        confp2 = &netconf[confix];
        confp2->Imask.l = confp->Imask.l;        
        confp2->netno = confp->netno;
        i2 = Nopen((char *) &DHCPrec->yiaddr, "ICMP/IP", 0, 0, IPADDR);
        /* invalidate fake ARP cache entry */
        confp2->ncstat = 0;
        if (i2 < 0)
            goto lab5;
        i1 = Nwrite(i2, "test", 5);
        Nclose(i2);
        if (i1 < 0)
            goto lab5;
        /* address in use, send a decline and start again */
        bootsend->options[6] = DHCPDECLINE;
        /* Add requested IP address option if not there */
	    if (renew) {
		    cp = bootsend->options + len;
		    *cp++ = 50;
		    *cp++ = 4;
		    memcpy(cp, (char *) &DHCPrec->yiaddr, 4);
		    cp += 4;
		    len += 6;
	    }
        bootsend->options[len] = 255;
        bootsend->xid = xid++;
        bootsend->ciaddr = 0;
        Nwrite(conno, (char *) bootsend, DHCPsz + sizeof(discopts));
        goto waitstart;
    }
#endif

/* install the new IP address */

lab5:
    memcpy(confp->Iaddr.c, (char *) &DHCPrec->yiaddr, Iid_SZ);

/* go through options */

    InitOption(stat, DHCPrec);
    while ((cp = NextOption(DHCPrec)) != 0) {
        oplen = cp[1];
        switch (*cp) {
        default:
#if NTRACE >= 3
            Nprintf("DHCP unsupported option %d\n", *cp);
#endif
            break;
        case 1:                 /* subnet mask */
            memcpy(confp->Imask.c, cp + 2, Iid_SZ);			
            break;
        case 2:                 /* time offset */
            break;
        case 3:                 /* router, take 1 */
            memcpy(iid.c, cp + 2, 4);
            if ((confix = GetHostData(iid.l, 3)) >= 0) {
                confp2 = &netconf[confix];
                confp2->nexthix = confix;
                confp2->flags |= ROUTER;
                /*
                ** Make router have same subnet as DHCP client, so that 
                ** the client can send messages through the router.
                */
                confp2->Imask.l = confp->Imask.l;  
                confp2->netno = netno;
                /* Make router static so it does not get overwritten */
                confp2->ncstat = 4;
            }
        case 4:                 /* time server */
            break;
        case 6:                 /* DNS server, take 2 */
#ifdef DNS
            i1 = oplen > 8 ? 8 : 4;
            memcpy(DNSiid[0].c, cp + 2, i1);
#endif
            break;
        case 9:                 /* LPR server */
            break;
        case 12:                /* host name */
            if (confp->name[0])
                break;
            for (i1 = 2; i1 < oplen; i1++) {
                if (i1 == sizeof(confp->name) - 1)
                    break;
                if (cp[i1] == '@')
                    break;
                confp->name[i1] = cp[i1];
            }
            confp->name[i1] = 0;
            break;
#ifdef DNS
        case 15:                /* domain name */
            memcpy(DNSdomain, cp + 2, oplen);
            DNSdomain[oplen] = 0;
            break;
#endif
        case 18:                /* boot file name */
            if (oplen < sizeof(DHCPbootfile)) {
                memcpy(DHCPbootfile, cp + 2, oplen);
                DHCPbootfile[oplen] = 0;
            }
            break;
        case 19:                /* IP forwarding, 0 = no */
#if RELAYING == 1
#ifdef MIB2
            if (*(cp + 2) == 0)
                IPgroup.ipForwarding = 2;
            else
                IPgroup.ipForwarding = 1;
#endif
#endif
            break;
        case 51:                /* address lease time */
            cp += 2;
            GETLONG(UL2, cp);
            ul1 = UL2.l;
            netp->LeaseTime = TIMEINMS(UL2.l, alltime);
            break;
        case 53:                /* message type */
            break;
        case 54:                /* server identifier */
            memcpy((char *) &DHCPrec->siaddr, cp + 2, Iid_SZ);
            netp->DHCPserver = DHCPrec->siaddr;
            break;
        case 58:                /* renewal time */
            cp += 2;
            GETLONG(UL2, cp);
            netp->DHCPsecs = UL2.l;
            netp->RenewalTime = TIMEINMS(UL2.l, alltime);
            break;
        case 59:                /* rebinding time */
            cp += 2;
            GETLONG(UL2, cp);
            netp->RebindingTime = TIMEINMS(UL2.l, alltime);
            break;
        }
    }

/* Take possible boot file name.  This is a static field so bootloading is
   not reentrant, but then why should it?
*/
    if ((DHCPrec->htype & 1) == 0 && DHCPrec->file[0])
        memcpy(DHCPbootfile, DHCPrec->file, sizeof(DHCPbootfile));

/* if our IP address is still 0, this was not a success
*/
    if (confp->Iaddr.l == 0)
        stat = -1;
        
    /*
    **  Find ARP cache entry with new IP addr and 0 Eth address and erase.
    **  This eliminates having two entries in the netconf table with the new
    **  client IP address. 
    */
    for (i1 = 0; i1 < NCONFIGS; i1++) {
        confp2 = &netconf[i1];
        if (confp2->Iaddr.l == DHCPrec->yiaddr) {
            if (memcmp((char *) confp2->Eaddr.c, (char *) &ezero, 6) == 0) {
                confp2->Iaddr.l = 0;
                confp2->ncstat = 0;
                confp2->hops = 0;
                confp2->Imask.l = 0;
            }
        }
        /* Also, erase the DHCP server netconf dynamic entry. */
        if (confp2->Iaddr.l == netp->DHCPserver) {
            confp2->Iaddr.l = 0;
            confp2->ncstat = 0;
            confp2->hops = 0;
            confp2->Imask.l = 0;
        }        
    }
        

/* free connection block, return
*/
ret7:
#if NTRACE >= 3
    if (stat > 0)
        Nprintf("DHCP %d %d.%d.%d.%d for %lu sec\n", netno,
                confp->Iaddr.c[0], confp->Iaddr.c[1],
                confp->Iaddr.c[2], confp->Iaddr.c[3], ul1);
    else
        Nprintf("DHCP %d failed\n", netno);
#endif
    Nclose(conno);
ret8:
    return stat;
}

/* This is a user subroutine to release the IP address.  There seems to
   be no response. */

int             DHCPrelease(int netno)
{
    int             stat,
					len,
                    conno;
	unsigned char  *cp;
    struct NETCONF *confp;
    struct DHCPstr *bootsend,
                    boot;
    struct NET     *netp;
    MESS           *mp;
    union {
        unsigned char   c[4];
        short           s[2];
        long            l;
    }               UL2;


    netp = &nets[netno];
    if (netp->DHCPserver == 0)
        return 0;
    confp = &netconf[nets[netno].confix];
    bootsend = &boot;

/* open the connection block */
    Nsprintf((char *) bootsend, "*/%s", confp->pname);
    stat = conno = Nopen((void *) bootsend, "UDP/IP", 68, 67, S_NOCON); 
    if (conno < 0)
        goto ret8;
/* use limited broadcast */
    connblo[conno].heriid.l = 0xffffffff;

/* create and write message */
    memset(bootsend, 0, sizeof(struct DHCPstr));
    bootsend->op = 1;
    bootsend->htype = 1;
    bootsend->hlen = sizeof(struct Eid);
    bootsend->xid = TimeMS();
    memcpy((char *) &bootsend->ciaddr, (char *) &confp->Iaddr, Iid_SZ);
    memcpy(bootsend->chaddr, (char *) &confp->Eaddr, Eid_SZ);
    memcpy(bootsend->options, releopts, sizeof(releopts));
/*  Add server identifier option */
	len = sizeof(releopts);
	cp = bootsend->options + len;
	*cp++ = 54;
	*cp++ = 4;
	memcpy(cp, (char *) &netp->DHCPserver, Iid_SZ);
	len += 6;
	cp += 4;
/*  Add end option */
	*cp++ = 255;
	
    Nwrite(conno, (char *) bootsend, DHCPsz + len + 1);
    Nclose(conno);
    stat = 0;
    
    /* Delete IP address from netconf table */
    confp->Iaddr.l = 0;

    /* Set DHCPserver to 0 so DHCPget() will start new */
    netp->DHCPserver = 0;
    
        
ret8:
    return stat;
}
