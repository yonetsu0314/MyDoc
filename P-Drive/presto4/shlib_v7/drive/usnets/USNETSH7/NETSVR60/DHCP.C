/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/dhcp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: dhcp.c $
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
/* DHCP.C  Edition: 12  Last changed: 23-Sep-99,17:53:52  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    DHCP.C -- Dynamic Host Configuration Protocol for USNET

    Copyright (C) 1993, 1996 By
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

** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** MODIFICATION HISTORY
**
** 23-SEP-1999 TTT  Set netconf IP address to 0 in DHCPrelease()
** 27-AUG-1999 TTT  Bind socket to host interface; unicast if renewing,
**                  erase DHCP server dynamic entry from netconf
** 05-APR-1999 TTT  Fix if block error when NTRACE=0
** 17-FEB-1999 TTT  Handle adding router correctly
** 22-JAN-1999 TTT  Change method of pointer addition in NextOption()
** 04-JAN-1999 TTT  Change to BSD Socket API (from DPI) +
**                  Minor modifications to correct compile warnings
** 21-OCT-1998 TTT  Bug fixes + bringing up to RFC 2131 specification
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include <stdio.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "socket.h"
#include "dhcp.h"
#include "icmp.h"

extern struct NETCONF netconf[];
extern struct CONNECT connblo[];
extern struct NET nets[];
extern char localhostname[];
#ifdef MIB2
extern struct IPgroup IPgroup;
#endif
#ifdef DNS
extern Iid      DNSiid[2];
extern char     DNSdomain[];
#endif

char            DHCPbootfile[FILENAMESIZE]; /* boot file name, NOT REENTRANT */

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
                    confix,
                    cs,
                    icmpsock,
                    rc,
                    clilen;
    unsigned long   ul1,
                    initime,
                    alltime,
                    xid,
                    inidelay,
                    delay;
    unsigned char   uc1,
                   *cp;
    char            buff[MAXBUF];
    struct NETCONF *confp,
                   *confp2;
    struct NET     *netp;
    Iid             iid;
    struct DHCPstr  DHCPrec,
                    bootsend;
    MESS           *mp;
    struct ICMPhdr  echoreq, *echorepl;
    struct sockaddr_in  
                    clisock,
                    socki;
    struct timeval  timeout;
    fd_set          client_set;
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
restart:
    /* Open socket */
    cs = socket(AF_INET, SOCK_DGRAM, 0);
    if (cs < 0) {
#if NTRACE > 0
        Nprintf("Error opening DHCP socket\n");
#endif            
        stat = cs;
        goto ret7;
    }
    memset(&clisock, 0, sizeof(clisock));
    
    /* Bind to local port 68 */
    clisock.sin_family = AF_INET;
    clisock.sin_port = htons(CLIENT_PORT); 
    /* Set to any address for purposes of bind */
    clisock.sin_addr.s_addr = htonl(INADDR_ANY);
    stat = bind(cs, (struct sockaddr *) &clisock, sizeof(clisock));
#if NTRACE > 0    
    if (stat < 0)
        Nprintf("Error in DHCP socket bind\n");
#endif
    /* Now set remote port to DHCP server port 67 */
    clisock.sin_port = htons(SERVER_PORT);         

    /* If not renewal, set destination address to limited broadcast;
    ** set to previous DHCP server IP address for renewal
    */
    if (renew == 0) {
        stat= setsockopt(cs, SOL_SOCKET, SO_BROADCAST, 0, 0);
        clisock.sin_addr.s_addr = 0xffffffff;    /* limited broadcast */         
    } 
    else
        clisock.sin_addr.s_addr = netp->DHCPserver; 


    /* Set socket option to bind to correct interface. This is 
    ** useful when using DHCP on multiple network interfaces
    */
    setsockopt(cs, SOL_SOCKET, SO_BINDTODEVICE, confp->pname,
               strlen(confp->pname));

    /*  Set timeout for read */
    timeout.tv_sec =  1;
    timeout.tv_usec = 0;    

    /* Create and write message, read answer, several times */
    memset(&bootsend, 0, sizeof(struct DHCPstr));
    bootsend.op = 1;
    bootsend.htype = 1;
    bootsend.hlen = sizeof(struct Eid);
    bootsend.xid = xid++;
    bootsend.secs = (TimeMS() - initime) >> 10;
    memcpy((char *) &bootsend.ciaddr, (char *) &confp->Iaddr, Iid_SZ);
    memcpy((char *) bootsend.chaddr, (char *) &confp->Eaddr, Eid_SZ);
    memcpy((char *) bootsend.options, discopts, sizeof(discopts));
    len = sizeof(discopts);
    cp = bootsend.options + len;
    /* If we requested a lease time; add it to the DHCP options */
    if (lease) {
        *cp++ = 51;
        *cp++ = 4;
        UL2.l = lease;
        PUTLONG(UL2, cp);
        cp += 4;
        len += 6;
    }
    /* If we already have an IP address, create a DHCP requested IP
    ** address option 
    */
    if (confp->Iaddr.l) {
        *cp++ = 50;
        *cp++ = 4;
        PUTLONG(confp->Iaddr, cp);
        cp += 4;
        len += 6;
    }
    /* If this is renewal, need to add DHCP server IP address option.
    ** Also, skip DHCPDISCOVER message if this is only renewal. 
    */
    if (renew) {             
        *cp++ = 54;
        *cp++ = 4;
        memcpy((char *) cp, (char *) &netp->DHCPserver, Iid_SZ);
        cp += 4;
        len += 6;
        goto renewaddress;
    }
    /* End of options marked by 255 */
    *cp = 255;
    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        clilen = sizeof(clisock);
        sendto(cs, (char *) &bootsend, DHCPsz + len + 1, 0, 
                       (struct sockaddr *)&clisock, clilen);
        YIELD();
        FD_ZERO(&client_set);
        FD_SET(cs, &client_set);
        rc = selectsocket(1, &client_set, 0, 0, &timeout);
        /* Ready to read */
        if (rc > 0) {
read3:      stat = recvfrom(cs, (char *) &DHCPrec, sizeof(struct DHCPstr), 0,
                         (struct sockaddr *) &clisock, &clilen);                        
            if (stat > DHCPsz) {
                if (DHCPrec.xid != bootsend.xid)
                    goto read3;
                if (DHCPrec.op != 2)
                    goto read3;
                goto lab2;
            }
        }
        /* Timeout or error -> try again */
        else {
            FD_CLR(cs, &client_set);
            stat = ETIMEDOUT;
        }
    } 
    goto ret7;

/* Scan the options, we need record type DHCPOFFER.  We take the first
   one as there is no strategy known to this level for selecting among
   several offers.
*/
lab2:
    InitOption;
    i1 = 0;
    while ((cp = NextOption(&DHCPrec)) != 0) {
        i2 = cp[0];
        /* Examine the DHCP message type */
        if (i2 == 53)  {
            if (cp[2] == DHCPOFFER)
                i1 = 1;
        }
        /* Examine the server identifier */
        if (i2 == 54) {
            i2 = cp[1] + 2;
            memcpy((char *) bootsend.options + len, cp, i2);
            len += i2;
        }
    }
    if (i1 == 0)
        goto read3;             /* wrong type, read again */

/* Send the same request, this time as DHCPREQUEST, and with server
** identifier option.  If no good answer, go back to the beginning.
*/
renewaddress:
    bootsend.options[6] = DHCPREQUEST; 
    if (!renew) {
        /* Add requested IP address option */
        cp = bootsend.options + len;
        *cp++ = 50;
        *cp++ = 4;
        memcpy((char *) cp, (char *) &DHCPrec.yiaddr, 4);
        cp += 4;
        len += 6;
    }
    bootsend.options[len] = 255;
    bootsend.xid = xid++;
    for (delay = inidelay; delay < MAXDELAY; delay <<= 1) {
        for (ul1 = TimeMS(); TimeMS() - ul1 < delay;)
            YIELD();
        alltime = TimeMS();
        clisock.sin_addr.s_addr = 0xffffffff;
        stat = sendto(cs, (char *) &bootsend, DHCPsz + len + 1, 0, 
                       (struct sockaddr *)&clisock, sizeof(clisock));
        YIELD();
        FD_ZERO(&client_set);
        FD_SET(cs, &client_set);
        rc = selectsocket(1, &client_set, 0, 0, &timeout);
        /* Ready to read */
        if (rc > 0) {
read4:      stat = recvfrom(cs, (char *) &DHCPrec, sizeof(struct DHCPstr), 0,
                         (struct sockaddr *) &clisock, &clilen);                        
            if (stat > DHCPsz) {
                if (DHCPrec.xid != bootsend.xid)
                    goto read4;
                if (DHCPrec.op != 2)
                    goto read4;
                InitOption;
                while ((cp = NextOption(&DHCPrec)) != 0) {
                    if (cp[0] == 53) {
                        if (cp[2] == DHCPACK)
                            goto lab4;
                        if (cp[2] == DHCPNAK)
                            goto waitstart;
                    }
                }
            }
        }
        /* Timeout or error -> try again */
        else {
            FD_CLR(cs, &client_set);
            stat = ETIMEDOUT;
        }
    }
waitstart:
    if (retry++ > 5) {
        stat = -1;
        goto ret7;
    }
    closesocket(cs);
    for (ul1 = TimeMS(); TimeMS() - ul1 < 10000;)
        YIELD();
    renew = 0;
    goto restart;

lab4:

/* Ping offered IP address to see if it is in use already. We create
** a fake entry in netconf (the ARP cache) so the write attempt will
** go out. Otherwise, a duplicate IP address would not be detected 
*/

#ifdef ARP
    if (netp->sndoff && DHCPrec.yiaddr != confp->Iaddr.l) {
        confp->Iaddr.l = 0;
        /* create fake ARP cache entry -> subnet in table -> can do ARP */
        confix = GetHostData((DHCPrec.yiaddr & confp->Imask.l), 0x17);
        confp2 = &netconf[confix];
        confp2->Imask.l = confp->Imask.l;        
        confp2->netno = confp->netno;
        /* Create ICMP request packet - ping */        
        icmpsock = socket(AF_INET, SOCK_DGRAM, ICMP);
        if (icmpsock < 0) {
#if NTRACE > 0
            Nprintf("Error creating socket for DHCP address probe\n");        
#endif	
        }
icmp:   memset(&socki, 0, sizeof(socki));
        socki.sin_family = AF_INET;
        socki.sin_addr.s_addr = DHCPrec.yiaddr;
        echoreq.type = IC_ECHO;
        echoreq.scode = 0;
        echoreq.chksum = 0;
        rc = sendto(icmpsock, (char *)&echoreq, 64+8, 0, 
                    (struct sockaddr *)&socki, sizeof(socki));
        /* Invalidate fake ARP cache entry */
        confp2->ncstat = 0;
        /* No reply -> address unreachable */        
        if (rc <= 0) 
            goto lab5;
        /* Address in use, send a decline and start again */        
        else {
            bootsend.options[6] = DHCPDECLINE;
            /* Add requested IP address option if not there */
    	    if (renew) {
         	    cp = bootsend.options + len;
                *cp++ = 50;
                *cp++ = 4;
                memcpy((char *) cp, (char *) &DHCPrec.yiaddr, 4);
            	cp += 4;
                len += 6;
            }
            bootsend.options[len] = 255;
            bootsend.xid = xid++;
            bootsend.ciaddr = 0;
            clisock.sin_addr.s_addr = 0xffffffff;
            sendto(cs, (char *) &bootsend, DHCPsz + len + 1, 0, 
                       (struct sockaddr *)&clisock, sizeof(clisock));
            closesocket(icmpsock);
            goto waitstart;
        }
    }
#endif

/* Install the new IP address */

lab5:
    memcpy((char *) confp->Iaddr.c, (char *) &DHCPrec.yiaddr, Iid_SZ);

/* Go through options */

    InitOption;
    while ((cp = NextOption(&DHCPrec)) != 0) {
        oplen = cp[1];
        switch (*cp) {
        default:
#if NTRACE >= 3
            Nprintf("DHCP unsupported option %d\n", *cp);
#endif
            break;
        case 1:                 /* subnet mask */
            memcpy((char *) confp->Imask.c, cp + 2, Iid_SZ);			
            break;
        case 2:                 /* time offset */
            break;
        case 3:                 /* router, take 1 */
            memcpy((char *) iid.c, cp + 2, 4);
#if 0
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
            }
#else
			for (i1 = 0; i1 < NCONFIGS; i1++) {
				if(!(strcmp( netconf[i1].name, "router" ) ) ){
				    memcpy( (char*)&netconf[i1].Iaddr, (char*)iid.c, Iid_SZ);
				}
			}
#endif
        case 4:                 /* time server */
            break;
        case 6:                 /* DNS server, take 2 */
#ifdef DNS
            i1 = oplen > 8 ? 8 : 4;
            memcpy((char *) DNSiid[0].c, cp + 2, i1);
#endif
            break;
        case 9:                 /* LPR server */
            break;
        case 12:                /* host name */
#if 0
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
#else
            for (i1 = 2, i2 = 0; i2 < oplen; i1++, i2++ ) {
                if (cp[i1] == '@')
                    break;
                confp->name[i2] = cp[i1];
				localhostname[i2] = cp[i1];
            }
            confp->name[i2] = 0;
			localhostname[i2] = 0;
#endif
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
            memcpy((char *) &DHCPrec.siaddr, cp + 2, Iid_SZ);
            netp->DHCPserver = DHCPrec.siaddr;
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

    /* Take possible boot file name.  This is a static field so 
    ** bootloading is not reentrant, but then why should it?
    */
    if ((DHCPrec.htype & 1) == 0 && DHCPrec.file[0])
        memcpy(DHCPbootfile, (char *) DHCPrec.file, sizeof(DHCPbootfile));

    /* If our IP address is still 0, this was not a success */
    if (confp->Iaddr.l == 0)
        stat = -1;
        
    /*  Find ARP cache entry with new IP addr and 0 Ethernet address  
    **  and erase. This eliminates having two entries in the netconf  
    **  table with the new client IP address.
    */
    for (i1 = 0; i1 < NCONFIGS; i1++) {
        confp2 = &netconf[i1];
        if (confp2->Iaddr.l == DHCPrec.yiaddr) {
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
    closesocket(cs);
ret8:
    return stat;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** DHCPrelease: User-called routine to release IP address
**
** int DHCPrelease(int netno)
**
** PARAMETERS: 
**    (in) netno: index into network configuration table
** 
** DESCRIPTION: 
**     DHCPrelease() sends a message to a DHCP server to release
**     the currently held client IP address. There is no response
**     to this message; so there is no guarantee that the server
**     actually receives the release. Note that the release is 
**     unicast to the DHCP server which originally handed out the
**     client's IP address.
**    
** RETURNS:
**    < 0:  Error given by error code
**      0:  DHCP server not stored in network configuration structure
**    > 0:  IP address released
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

int             DHCPrelease(int netno)
{
    int             stat,
                    len,
                    conno,
                    cs;
    unsigned char  *cp;
    struct NETCONF *confp;
    struct DHCPstr bootsend;
    struct NET     *netp;
    MESS           *mp;
    struct sockaddr_in  
                    clisock;
    union {
        unsigned char   c[4];
        short           s[2];
        long            l;
    }               UL2;


    netp = &nets[netno];
    if (netp->DHCPserver == 0)
        return 0;
    confp = &netconf[nets[netno].confix];

    /* Open socket */
    cs = socket(AF_INET, SOCK_DGRAM, 0);
    if (cs < 0) {
#if NTRACE > 0
        Nprintf("Error opening DHCP socket\n");
#endif            
        stat = cs;
        goto ret8;
    }
    stat= setsockopt(cs, SOL_SOCKET, SO_BROADCAST, 0, 0);
    /*
    ** Set socket option to bind to correct interface. This is 
    ** useful when using DHCP on multiple network interfaces
    */
    setsockopt(cs, SOL_SOCKET, SO_BINDTODEVICE, confp->pname,
               strlen(confp->pname));    

    /* Bind to local port 68 */
    clisock.sin_family = AF_INET;
    clisock.sin_addr.s_addr = htonl(INADDR_ANY);
    clisock.sin_port = htons(CLIENT_PORT); 
    stat = bind(cs, (struct sockaddr *) &clisock, sizeof(clisock));
#if NTRACE > 0    
    if (stat < 0)
        Nprintf("Error in DHCP socket bind\n");
#endif
    /* Now set remote port to DHCP server port 67 */
    clisock.sin_port = htons(SERVER_PORT);      

    /* Create and write message */
    memset(&bootsend, 0, sizeof(struct DHCPstr));
    bootsend.op = 1;
    bootsend.htype = 1;
    bootsend.hlen = sizeof(struct Eid);
    bootsend.xid = TimeMS();
    memcpy((char *) &bootsend.ciaddr, (char *) &confp->Iaddr, Iid_SZ);
    memcpy((char *) bootsend.chaddr, (char *) &confp->Eaddr, Eid_SZ);
    memcpy((char *) bootsend.options, releopts, sizeof(releopts));
    /* Add server identifier option */
    len = sizeof(releopts);
    cp = bootsend.options + len;
    *cp++ = 54;
    *cp++ = 4;
    memcpy((char *) cp, (char *) &netp->DHCPserver, Iid_SZ);
    len += 6;
    cp += 4;
    /* Add end option */
    *cp++ = 255;
    clisock.sin_addr.s_addr = 0xffffffff;   /* limited broadcast */ 
    sendto(cs, (char *) &bootsend, DHCPsz + len + 1, 0, 
                 (struct sockaddr *)&clisock, sizeof(clisock));	
    closesocket(cs);
    stat = 0;
    
    /* Delete IP address from netconf table */
    confp->Iaddr.l = 0;

    /* Set DHCPserver to 0 so DHCPget() will start new */
    netp->DHCPserver = 0;
    
ret8:
    return stat;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** NextOption: returns pointer to next DHCP option
**
** unsigned char *NextOption(struct DHCPstr * DHCPp)
**
** PARAMETERS: 
**    (in) pointer to DHCP message structure
** 
** DESCRIPTION: 
**    Internal routine to return pointer to next DHCP option. 
**    Returns 0 to signify end of options.
**    
** RETURNS:
**    0: Error or end of options
**   cp: pointer to next DHCP option
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

short DHCPfirst;
short DHCPadv;

unsigned char *NextOption(struct DHCPstr * DHCPp)
{
    unsigned char   optype,
                    oplen,
                   *cp;
    int             i1;

/* Check message for BOOTREQUEST/BOOTREPLY first time */
    if (DHCPfirst == 0) {
        if (DHCPp->op & 1 || DHCPp->op & 2) 
            DHCPadv = 44;
        else {
#if NTRACE > 0
        Nprintf("Message type not BOOTREQUEST or BOOTREPLY\n");
#endif
            return 0;
        }
    }
/* Advance pointer to start of options */
    cp = (unsigned char *) DHCPp + DHCPadv;

/* Check for magic cookie {99,130,83,99} which defines start of options
   If cookie isn't there, then this is sname field (length 64)
   Next is file field (length 128)
*/
    if (DHCPfirst == 0) {
        DHCPfirst++;
        for (i1 = 0; i1 < 3; i1++) {
            if ((cp[0] == 99) && (cp[1] == 130) && (cp[2] == 83) 
                   && (cp[3] == 99)) {
                cp += 4;
                break;
            }
            else { 
                cp += 64;
                if (i1)
                    cp += 64;
            }           
        }
        DHCPadv = cp - (unsigned char *)DHCPp;	  
    }
 
    if (cp[0] != 255) {		
        DHCPadv += (cp[1] + 2);
        if (cp[0] == 52) {         /* option overload */
#if NTRACE > 0
            Nprintf("DHCP server: option overload not allowed\n");
#endif
        }
    }
    else 
        return 0;
    return cp;
}
