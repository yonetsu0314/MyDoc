/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/dhcps.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: dhcps.c $
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
/* DHCPS.C  Edition: 4  Last changed: 5-Feb-99,15:43:08  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway, Suite 100
    Hillsboro, Oregon 97124
    503-844-6614  www.ussw.com

    DHCPS.C: Dynamic Host Configuration Protocol Server for USNET

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
** MODIFICATION HISTORY:
**
** 27-OCT-1998  TTT  Creation
** 04-JAN-1999  TTT  Minor modifications to correct compile warnings
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include <stdio.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "socket.h"
#include "icmp.h"
#include "dhcp.h"

/* Function prototypes for DHCP server utility functions */
struct client find_lease(FILE *fp, unsigned char chaddr[16], int *status);
int write_lease(char fname[80], struct client cid); 
int write_arpcache(unsigned char chaddr[16], unsigned long iaddr, 
                   char hwaddyes, char ncstat);
int read_conf(char fname[80], struct config * conf);
int read_lease(char fname[80], int range, struct client *clirec, struct addrpool *pool);


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** DHCPserv(): Dynamic Host Configuration Protocol Server for USNET
** 
** void DHCPserv(void)
** 
** PARAMETERS: none
** 
** RETURNS: none
** 
** DESCRIPTION: 
**    The Dynamic Host Configuration Protocol Server for USNET assigns IP
**    addresses to clients who request them. If DHCP is enabled, a USNET 
**    client requests an address in Portinit(). The USNET DHCP Server 
**    follows RFC's 2131 and 2132 with the restrictions noted in
**    the DHCP.TXT file in the USNET DOC directory. See DHCP.TXT for 
**    full information.
**
** COMMENTS: none
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

void DHCPserv(void)
{
    int             ss,
                    cs,
                    icmpsock,
                    rc,
                    i1,
                    i2,
                    iclient,
                    len,
                    dlen,
                    range,
                    preq[11],
                    recsiz,
                    nottous,
                    send_nak,
                    clilen;
    short           mtype = 0,
                    broadcast,
                    cnt,
                    used;
    unsigned char   *cp;
    unsigned long   subnet;
    char            buff[MAXBUF];		
    struct DHCPstr  DHCPreq,
                    DHCPrepl;
    MESS            *mp,
                    *mp2; 
    struct hostent  hostent;
    unsigned char   scratch_buff[900];
    struct sockaddr_in socka, 
                    clisock,
                    socki;
    Iid             reqaddr,
                    giveaddr,
                    serveriid,
                    ouraddr,
                    UL2;
#if NTRACE > 1
    Iid             nakaddr;
#endif					
    struct ICMPhdr  echoreq, *echorepl;
    struct client   cid, clirec[100];
    struct addrpool pool[100];
    struct config   conf;


/*  Read DHCP configuration information */
    rc = read_conf(CONF_FILE, &conf);
    if (rc < 0) {
#if NTRACE > 0
        Nprintf("Error reading DHCP configuration file\n");
#endif
        return;
    }
    range = conf.rangeh.c[3] - conf.rangel.c[3] + 1;
    for (i1 = 0; i1 < range; i1++) {
        memcpy((char *) pool[i1].iaddr.c, (char *) conf.rangeh.c, Iid_SZ);
        pool[i1].iaddr.c[3] = conf.rangel.c[3] + i1;	
        pool[i1].used = 0;
    }
    subnet = pool[0].iaddr.l & conf.submask.l;

/*  Read lease information from lease record file */
    rc = read_lease(LEASE_FILE, range, clirec, pool);

/*  Open server socket */
    ss = socket(AF_INET, SOCK_DGRAM, 0);
    if (ss < 0) {
#if NTRACE > 0	
        Nprintf("Error opening DHCP server socket\n");
#endif
        return;
    }

/* Bind server socket to DHCP server port */
    memset(&socka, 0, sizeof(socka));
    socka.sin_family = AF_INET;
    socka.sin_addr.s_addr = htonl(INADDR_ANY);
    socka.sin_port = htons(SERVER_PORT);
    rc = bind(ss, (struct sockaddr *)&socka, sizeof(socka));
    if (rc < 0) {
#if NTRACE > 0
        Nprintf("Error in DHCP server bind.\n");
#endif
        return;
    }

/* Open client socket - write to this one */
    cs = socket(AF_INET, SOCK_DGRAM, 0);
    if (cs < 0) {
#if NTRACE > 0	
        Nprintf("Error opening DHCP client socket\n");
#endif
        return;
    }
    memset(&clisock, 0, sizeof(clisock));
    rc = setsockopt(cs, SOL_SOCKET, SO_BROADCAST, 0, 0);
    if (rc < 0) {
#if NTRACE > 0
        Nprintf("Error setting client socket options\n");
#endif
        return;
    }
/* Bind client socket to local port 67 - Microsoft clients appear
   to require message source port = DHCP server port */
    clisock.sin_family = AF_INET;
    clisock.sin_port = htons(SERVER_PORT); 
    rc = bind(cs, (struct sockaddr *) &clisock, sizeof(clisock));
    if (rc < 0)
#if NTRACE > 0
        Nprintf("Error in bind of sendto socket -> server won't work with Microsoft clients.\n");
#endif
/* Now set remote port to DHCP client port 68 */
    clisock.sin_port = htons(CLIENT_PORT); 

/* Set up ICMP probe socket */
#ifdef DHCP_PROBE
    icmpsock = socket(AF_INET, SOCK_DGRAM, ICMP);
    if (icmpsock < 0) {
#if NTRACE > 0
        Nprintf("Error creating ICMP socket\n");        
#endif	
    }
#endif

/* Determine our IP address */
    gethostbyname_r(DHCP_SERVER, &hostent, (char *) scratch_buff, 
                    sizeof(scratch_buff), &rc);
    if (rc < 0) {
#if NTRACE > 0
        Nprintf("Error obtaining own IP address for server %s\n", DHCP_SERVER);
#endif
        return;	
    }
    memcpy((char *) ouraddr.c, (char *)hostent.h_addr_list[0], Iid_SZ);

    memset((char *) reqaddr.c, 0, Iid_SZ);
    preq[0] = 255;	

/* Infinite loop: Reads data from DHCP client, formulates appropriate reply
   and sends it, then comes back to wait for more data from client.
*/
    for ( ; ; ) {
    while ((recvfrom(ss, (char *) &DHCPreq, sizeof(struct DHCPstr), 0, 
        (struct sockaddr *)&socka, &clilen)) < 0) {
        YIELD();
    }
   
/* Get information from DHCP message options */
    InitOption;
    memset((char *) serveriid.c, 0, Iid_SZ);
    while ((cp = NextOption(&DHCPreq)) != 0) {
        switch (*cp) {
        case 50:		/* Requested IP address */
            memcpy((char *) reqaddr.c, cp+2, 4);
            break;
        case 53:   		/* DHCP Message Type */
            mtype = *(cp+2);
            break;
        case 54: 		/* Server Identifier */			
            memcpy((char *) serveriid.c, cp+2, 4);
            break;
        case 55:		/* Parameter Request List */
            for (i1 = 0; i1 < *(cp+1); i1++) {
                if (i1 > 10)
                    break;
                preq[i1] = *(cp+2+i1);
            }
            preq[i1] = 255;
            break;
        default:
            break;
        }
    }

    nottous = 0;
    send_nak = 0;
#ifdef DHCP_BROADCAST
    broadcast = 1;
#else
    broadcast = (DHCPreq.flags == 1) ? 1 : 0;
#endif

/* Process message */
    switch (mtype) {
    case DHCPDISCOVER:
#if NTRACE > 1
        Nprintf("DHCPDISCOVER from %x:%x:%x:%x:%x:%x\n", DHCPreq.chaddr[0],
        DHCPreq.chaddr[1], DHCPreq.chaddr[2], DHCPreq.chaddr[3],
        DHCPreq.chaddr[4], DHCPreq.chaddr[5]);
#endif
        giveaddr.l = 0;

/* Invalidate possible ARP cache entry so it won't respond to a ping */
        rc = write_arpcache(DHCPreq.chaddr, 0, 0, 3);

/* Check if client already has binding */
        cid.iaddr.l = 0;
        for (iclient = 0; ; iclient++) {
            if (memcmp((char *) clirec[iclient].haddr.c, 
                  (char *) DHCPreq.chaddr, Eid_SZ) == 0) {
                /* Don't use binding if on different subnet */
                if ((clirec[iclient].iaddr.l & conf.submask.l) != subnet)
                    break;
                /* Don't use binding if status is unavailable */
                if (clirec[iclient].active == 3)
                    break;
                memcpy((char *) &cid, (char *) &clirec[iclient], 
                        sizeof(struct client));
                break;
            }
            if (clirec[iclient].iaddr.l == 0) 
                break;
        }
        giveaddr.l = cid.iaddr.l;		

/* If no binding, check for requested ip address */
        if ((giveaddr.l == 0) && (reqaddr.l != 0)) {
            /* Don't use requested address if on different subnet */
            if ((reqaddr.l & conf.submask.l) == subnet) 
                memcpy((char *) giveaddr.c, (char *) reqaddr.c, Iid_SZ);
        }		

/* Check whether address has already been used in pool */
        for (i1 = 0, cnt = 0; i1 < range; i1++) {
            if (memcmp((char *) pool[i1].iaddr.c, (char *) giveaddr.c, Iid_SZ) == 0) {
                if (pool[i1].used != 0) {
                    if (pool[i1].used == 3) {
                        cnt = 1;
                        break;
                    }
/* Find if anyone besides current client actively has address. Give 
   address back to current client if it's the only one */
                    for (i2 = 0; ; i2++) {
                        if ((memcmp((char *) giveaddr.c, 
                             (char *) clirec[i2].iaddr.c, Iid_SZ) == 0) &&
                             (memcmp((char *) DHCPreq.chaddr, 
                             (char *) clirec[i2].haddr.c, Eid_SZ) != 0)
                              && (clirec[i2].active != 0))
                            cnt++;
                        if (clirec[i2].iaddr.l == 0)
                            break;
                    }
                }
                break;   
            }
        }	
/* Find new address if someone other than current client has it */
        if (cnt > 0) 
            giveaddr.l = 0;

/* Get address from pool (if needed), then probe offered address. Loop until
   we find an unused address to give. */
        for ( ;; ) {
        /* If no address set yet, get it from pool */
            if (giveaddr.l == 0) {
                for (i1 = 0; i1 < range; i1++) {
                    if (pool[i1].used != 0) {
                        if (pool[i1].used == 3)
                            continue;
            /* If address has already been offered to this client, offer it
               again (to take care of multiple DISCOVERS) */
                        if ((pool[i1].iaddr.l == clirec[iclient].iaddr.l) && 
                             clirec[iclient].active == 2)
                            break;
                        continue;
                    }
                    break;
                }
                if (i1 != range) {
                    memcpy((char *) giveaddr.c, (char *) pool[i1].iaddr.c, Iid_SZ);
                    pool[i1].used = 1;
                }
                else {
#if NTRACE > 0
                    Nprintf("DHCP server: no IP addresses left in range\n");
#endif
                    goto nottous;
                }
            }

#ifdef DHCP_PROBE
            recsiz = 64;
            for ( ; ; ) {
                memset(&socki, 0, sizeof(socki));
                socki.sin_family = AF_INET;
                socki.sin_addr.s_addr = giveaddr.l;
                echoreq.type = IC_ECHO;
                echoreq.scode = 0;
                echoreq.chksum = 0;
                rc = sendto(icmpsock, (char *)&echoreq, recsiz+8, 0, 
                    (struct sockaddr *)&socki, sizeof(socki));
                if (rc < 0) {
                /* No reply -> address unreachable */
                    break;
                }
                YIELD();
                rc = recvfrom(icmpsock, buff, sizeof(buff), 0, 
                      (struct sockaddr *)&socki, &clilen);
                echorepl = (struct ICMPhdr *) buff;
                /* ICMP Echo Reply means address in use - find another */
                if (echorepl->type == IC_EREP) {
                /* Mark address as used in address pool */
                    for (i1 = 0; i1 < range; i1++) {
                        if (memcmp((char *) pool[i1].iaddr.c, 
                            (char *) giveaddr.c, Iid_SZ) == 0) {
                            pool[i1].used = 1;
                            break;			
                        }
                    }	
                    giveaddr.l = 0; 
                    break;			
                }
                else if (echorepl->type == IC_UNRE) {
                /* ICMP Destination Unreachable -> address ok */
                    break;
                }
                else {
                /* If any other reply, rewrite message and read again */
                    continue;
                }
            }
#endif  /* End #ifdef DHCP_PROBE */

            if (giveaddr.l)	{
               /* Now that we've pinged, erase ARP cache entry which was
                  created for the ping - otherwise sendto will create ARP
                  for correct hardware address and client can't answer 
                  ARP yet */
                memset((char *) scratch_buff, 0, 16);
                write_arpcache(scratch_buff, giveaddr.l, 0, 0);
                break;
            }

        }

/* Mark address as offered in address pool */
        for (i1 = 0; i1 < range; i1++) {
            if (memcmp((char *) pool[i1].iaddr.c, 
                (char *) giveaddr.c, Iid_SZ) == 0) {
                pool[i1].used = 2;
                break;			
            }
        }

/*  Record offered address in client binding */
        clirec[iclient].iaddr.l = giveaddr.l;
        memcpy((char *) clirec[iclient].haddr.c, 
                (char *) DHCPreq.chaddr, Eid_SZ);
        clirec[iclient].active = 2;

/* Write yiaddr to ARP cache */
        rc = write_arpcache(DHCPreq.chaddr, giveaddr.l, 1, 3);
        if (rc < 0) 
            broadcast = 1;

/* Prepare rest of reply */
        DHCPrepl.yiaddr = giveaddr.l;
        DHCPrepl.ciaddr = 0;
        memcpy((char *) DHCPrepl.options, offopts, sizeof(offopts));
        len = sizeof(offopts);

#if NTRACE > 1
        Nprintf("DHCPOFFER on %d.%d.%d.%d to %x:%x:%x:%x:%x:%x\n", 
         giveaddr.c[0], giveaddr.c[1], giveaddr.c[2], giveaddr.c[3],
         clirec[iclient].haddr.c[0], clirec[iclient].haddr.c[1], 
         clirec[iclient].haddr.c[2], clirec[iclient].haddr.c[3], 
         clirec[iclient].haddr.c[4], clirec[iclient].haddr.c[5]);
#endif
        break;

    case DHCPREQUEST:
/* First, evaluate server id option to see if it's to us. Server id option is 
   only set from client in SELECTING state */
        if (serveriid.l != 0 && (memcmp((char *) serveriid.c, 
             (char *) ouraddr.c, Iid_SZ) != 0)) {
            nottous = 1;
            break;
        }

/* This server does not handle limited leases. Therefore, if a client is 
   in the RENEWING or REBINDING state (indicated by a non-zero ciaddr field),
   give the client the network address of ciaddr and an infinite lease. 

   If the ciaddr field is zero, then the client is in the INIT-REBOOT or the
   SELECTING state. In both states, the requested IP address option must be 
   set. Give the client this address and an infinite lease. 
*/
/*  Find client binding for REQUEST */
        cid.iaddr.l = 0;
        for (iclient = 0; ; iclient++) {
            if (memcmp((char *) clirec[iclient].haddr.c, 
                 (char *) DHCPreq.chaddr, Eid_SZ) == 0) {
                memcpy((char *) &cid, (char *) &clirec[iclient], 
                  sizeof(struct client));
                break;
            }
            if (clirec[iclient].iaddr.l == 0)
                break;
        }

        if (DHCPreq.ciaddr) {
            if (cid.iaddr.l != DHCPreq.ciaddr) {
                send_nak = 1;
#if NTRACE > 1
                nakaddr.l = DHCPreq.ciaddr;
#endif
                break;
            }
            else 
                giveaddr.l = DHCPreq.ciaddr;
        }
        else if (reqaddr.l) {
            if (reqaddr.l != cid.iaddr.l) {
#if NTRACE > 1
                nakaddr.l = reqaddr.l;
#endif
                send_nak = 1;
                break;
            }
            else
                giveaddr.l = reqaddr.l;

        }
        else {
#if NTRACE > 0
            Nprintf("Ciaddr and Requested address option both zero\n");
#endif
            send_nak = 1;	
#if NTRACE > 1
            nakaddr.l = 0;
#endif
            break;
        }

#if NTRACE > 1
        Nprintf("DHCPREQUEST for %d.%d.%d.%d from %x:%x:%x:%x:%x:%x\n",
          giveaddr.c[0], giveaddr.c[1], giveaddr.c[2], giveaddr.c[3],
          DHCPreq.chaddr[0], DHCPreq.chaddr[1], DHCPreq.chaddr[2], 
          DHCPreq.chaddr[3], DHCPreq.chaddr[4], DHCPreq.chaddr[5]);
#endif   

/* Mark address as used in address pool */
        for (i1 = 0; i1 < range; i1++) {
            if (memcmp((char *) pool[i1].iaddr.c, (char *) giveaddr.c, 
                 Iid_SZ) == 0) {
                pool[i1].used = 1;
                break;
            }
        }

/* Record address in client binding */
        if (clirec[iclient].iaddr.l == 0) {
            clirec[iclient].iaddr.l = giveaddr.l;
            memcpy((char *) clirec[iclient].haddr.c, 
                   (char *) DHCPreq.chaddr, Eid_SZ);
        }
        clirec[iclient].active = 1;

/* Record client binding to persistent storage */
        rc = write_lease(LEASE_FILE, clirec[iclient]);

/* Prepare reply */
        DHCPrepl.yiaddr = giveaddr.l;
        DHCPrepl.ciaddr = DHCPreq.ciaddr;
        len = strlen(conf.bootfile);
        if (len > 0)
            strcpy((char *) DHCPrepl.file, conf.bootfile);
        memcpy((char *) DHCPrepl.options, ackopts, sizeof(ackopts));
        len = sizeof(ackopts);

#if NTRACE > 1
        Nprintf("DHCPACK on %d.%d.%d.%d to %x:%x:%x:%x:%x:%x\n",
         giveaddr.c[0], giveaddr.c[1], giveaddr.c[2], giveaddr.c[3],
         clirec[iclient].haddr.c[0], clirec[iclient].haddr.c[1], clirec[iclient].haddr.c[2], 
         clirec[iclient].haddr.c[3], clirec[iclient].haddr.c[4], clirec[iclient].haddr.c[5]);
#endif
        break;

    case DHCPINFORM:

        DHCPrepl.yiaddr = 0;
        DHCPrepl.ciaddr = DHCPreq.ciaddr;
        memcpy((char *) DHCPrepl.options, ackopts, sizeof(ackopts));
        len = sizeof(ackopts);
        break;

    case DHCPDECLINE:
    case DHCPRELEASE:
        nottous = (memcmp((char *) serveriid.c, (char *) ouraddr.c, 
                   Iid_SZ) == 0) ? 0 : 1;
        if (nottous) 
            break;

        if (mtype == DHCPDECLINE) {
#if NTRACE > 1
            Nprintf("DHCPDECLINE on %d.%d.%d.%d from %x:%x:%x:%x:%x:%x\n",
             reqaddr.c[0], reqaddr.c[1], reqaddr.c[2], reqaddr.c[3],
             DHCPreq.chaddr[0], DHCPreq.chaddr[1], DHCPreq.chaddr[2], 
             DHCPreq.chaddr[3], DHCPreq.chaddr[4], DHCPreq.chaddr[5]);
#endif
            cid.iaddr.l = reqaddr.l;
            cid.active = 3;  /* unavailable */
        }
        else {
#if NTRACE > 1
            memcpy((char *) UL2.c, (char *) &DHCPreq.ciaddr, Iid_SZ);
            Nprintf("DHCPRELEASE of %d.%d.%d.%d from %x:%x:%x:%x:%x:%x\n",
             UL2.c[0], UL2.c[1], UL2.c[2], UL2.c[3],
             DHCPreq.chaddr[0], DHCPreq.chaddr[1], DHCPreq.chaddr[2], 
             DHCPreq.chaddr[3], DHCPreq.chaddr[4], DHCPreq.chaddr[5]);
#endif
            cid.iaddr.l = DHCPreq.ciaddr;
            cid.active = 0;
            /* erase IP address from ARP cache */
            write_arpcache(DHCPreq.chaddr, 0, 0, 0);
        }
        /* Mark address status in pool */
        for (i1 = 0; i1 < range; i1++) {
            if (memcmp((char *) pool[i1].iaddr.c, (char *) cid.iaddr.c, 
                Iid_SZ) == 0) {
                pool[i1].used = cid.active;
            break;
            }
        }
        /* Mark client binding with new status */
        for (iclient = 0; ; iclient++) {
            if (memcmp((char *) clirec[iclient].haddr.c, 
                (char *) DHCPreq.chaddr, Eid_SZ) == 0) 
                break;
            if (clirec[iclient].iaddr.l == 0)
                break;
        }
        clirec[iclient].active = cid.active;
        memcpy((char *) cid.haddr.c, (char *) DHCPreq.chaddr, Eid_SZ);
/*  Write inactive status to persistent storage */
        rc = write_lease(LEASE_FILE, cid);

        break;

    default:
        break;	
    }      /* End message type case statement */

/* If message was DHCPDECLINE or DHCPRELEASE, we don't reply */
    if (mtype == DHCPDECLINE || mtype == DHCPRELEASE) 
        continue;
/* If message wasn't to this server, go back and wait for next message */
    if (nottous) 
nottous:
        continue;

/* Set some fields common to all replies */
    DHCPrepl.op = BOOTREPLY;
    DHCPrepl.htype = 1;		  
    DHCPrepl.hlen = sizeof(struct Eid);
    DHCPrepl.hops = 0;
    DHCPrepl.xid = DHCPreq.xid;
    DHCPrepl.secs = 0;
    DHCPrepl.siaddr = 0;
    DHCPrepl.flags = DHCPreq.flags;
    DHCPrepl.giaddr = DHCPreq.giaddr;
    memcpy((char *) DHCPrepl.chaddr, (char *) DHCPreq.chaddr, Eid_SZ);

    if (send_nak) {
        DHCPrepl.yiaddr = 0;
        memcpy((char *) DHCPrepl.options, nakopts, sizeof(nakopts));
        len = sizeof(nakopts);
#if NTRACE > 1
        Nprintf("DHCPNAK on %d.%d.%d.%d to %x:%x:%x:%x:%x:%x\n",
         nakaddr.c[0], nakaddr.c[1], nakaddr.c[2], nakaddr.c[3],
         DHCPreq.chaddr[0], DHCPreq.chaddr[1], DHCPreq.chaddr[2], 
         DHCPreq.chaddr[3], DHCPreq.chaddr[4], DHCPreq.chaddr[5]);
#endif
    }	
/* Set up pointer to options */
    cp = DHCPrepl.options + len;
/*  Add server identifier option */
    *cp++ = 54;
    *cp++ = 4;
    memcpy((char *) cp, (char *) ouraddr.c, 4);
    cp += 4;
    len += 6;

    if (send_nak == 0) {
        if (mtype != DHCPINFORM) {
            /*  Add lease time option */
            *cp++ = 51;
            *cp++ = 4;
            UL2.l = 0xffffffff;
            PUTLONG(UL2, cp);
            cp += 4;
            len += 6;
            /* Sendto address is yiaddr for reply to DISCOVER and REQUEST */
            clisock.sin_addr.s_addr = giveaddr.l;
        }
        else {
            /* Sendto address is ciaddr for reply to INFORM */
            clisock.sin_addr.s_addr = DHCPreq.ciaddr;
        }
        /* Handle parameter request list */
        for (i1 = 0; preq[i1] != 255; i1++) {
            switch(preq[i1]) {
            case 1:    /* Subnet Mask */
                *cp++ = 1;
                *cp++ = 4;
                memcpy((char *) cp, (char *) conf.submask.c, 4);
                cp += 4;
                len += 6;
                break;
            case 3:		 /* Router */
                *cp++ = 3;
                *cp++ = conf.nrouter*4;
                memcpy((char *) cp, (char *) &conf.routeaddr[0], conf.nrouter*4);
                len += (2 + 4*conf.nrouter);
                cp += 4*conf.nrouter;
                break;
            case 6:    /* DNS Server */
#ifdef DNS
                *cp++ = 6;
                *cp++ = conf.ndns*4;
                memcpy((char *) cp, (char *) &conf.dnsaddr[0], conf.ndns*4);
                len += (2 + 4*conf.ndns);
                cp += 4*conf.ndns;
#endif
                break;
            case 12:     /* Host Name */
                break;
            case 15:    /* Domain Name */
#ifdef DNS
                *cp++ = 15;
                dlen = strlen(conf.dns_domain) + 1;
                *cp++ = dlen;
                memcpy((char *) cp, conf.dns_domain, dlen);
                cp += dlen;
                len += (2 + dlen);
#endif
                break;
            default:
                break;
            }
        }
    }
/*  Add end option */
    *cp = 255;

/*  Send reply */
    if (broadcast || send_nak == 1) 
        clisock.sin_addr.s_addr = 0xffffffff;
    clilen = sizeof(clisock);
    cnt = 0;
    while (sendto(cs, (char *) &DHCPrepl, DHCPsz + len + 1, 0, 
        (struct sockaddr *)&clisock, clilen) < 0) {
        YIELD();
        /* Send it a few times, then give up */
        if (cnt++ > 5)
            break;
    }

    }

/*  Should never reach this point, but close connections anyway */
    closesocket(cs);
    closesocket(ss);
    closesocket(icmpsock);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** write_lease: writes client record to DHCP lease file 
**
** int write_lease(char fname[80], struct client cid) 
**
** PARAMETERS: 
**    (in) fname: DHCP lease file name
**    (in) cid: Client binding to be written
**
** RETURNS:
**    0: OK
**   -1: Error reading lease file
**   -2: Error opening lease file
**
** DESCRIPTION: 
**    Internal routine which finds existing lease with find_lease() or 
**    finds end of lease file if no existing lease, then writes client 
**    binding to file.  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int write_lease(char fname[80], struct client cid) 
{
    struct client temp;
    int status;
    FILE *fp;

    fp = fopen(fname, "r+");
    if (fp == NULL) {
#if NTRACE > 0
        Nprintf("Error opening lease file\n");
        return -2;
#endif
    }
/* Find lease record if it already exists */
    temp = find_lease(fp, cid.haddr.c, &status);
    if (status < 0) {
        if (status != EOF) 
            return -1;
        else 
            /* Go to end of file if this is new binding */
            fseek(fp, 0, SEEK_END);
    }
    else 
        fprintf(fp,"\n");

    fprintf(fp, "%x:%x:%x:%x:%x:%x %lu %d\n", cid.haddr.c[0], cid.haddr.c[1], 
        cid.haddr.c[2], cid.haddr.c[3], cid.haddr.c[4], cid.haddr.c[5], 
        cid.iaddr.l, cid.active);

    fclose(fp);
    return 0;
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** read_lease: reads DHCP lease file into client bindings
**
** int read_lease(char fname[80], int range, struct client *clirec, 
**                struct addrpool *pool)
**
** PARAMETERS: 
**    (in)  fname: DHCP lease file name
**    (in)  range: range of addresses in server address pool
**    (in/out) clirec: pointer to first client binding 
**    (in/out) pool: pointer to first element of address pool 
**
** RETURNS:
**    0: OK
**   -2: Error reading lease file
**
** DESCRIPTION: 
**    Internal routine called at server start to fill in client bindings 
**    from persistent storage record of client leases. Note that last 
**    unused binding is filled in with zeros. Also marks used addresses in 
**    server address pool.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *            
*/
int read_lease(char fname[80], int range, struct client *clirec, 
               struct addrpool *pool)
{
    int i1, i2, status;
    FILE *leasefp;

    leasefp = fopen(fname, "r+");
    if (leasefp == NULL) 
        return -2;
    for (i1 = 0;  ; i1++) {
        clirec[i1] = find_lease(leasefp, 0, &status);
        if (status < EOF) {
#if NTRACE > 0
            Nprintf("Error reading lease file\n");
#endif
            return -2;	
        }
        if (clirec[i1].iaddr.l == 0)
            break;
        /* Mark used addresses in pool */
        for (i2 = 0; i2 < range; i2++) {
            if (memcmp((char *) pool[i2].iaddr.c, 
                 (char *) clirec[i1].iaddr.c, Iid_SZ) == 0) {
                pool[i2].used = clirec[i1].active;
            break;			
            }
        }
    }
    fclose(leasefp);

    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** find_lease: Locates lease in DHCP lease file by hardware address
**
** struct client find_lease(FILE *fp, unsigned char chaddr[16], 
**                          int *status) 
**
** PARAMETERS: 
**    (in)  fp: pointer to DHCP lease file
**    (in)  chaddr: hardware address as formatted in DHCP message
**    (out) status: error status: 0 OK
**                               -1 EOF on lease file (lease not found)
**                               -2 Error reading lease file
** RETURNS:
**    struct client: client binding structure
**    (Note: fills structure with 0's on error or at EOF)
**
** DESCRIPTION: 
**    Internal routine which finds existing lease by hardware address. 
**    If called with chaddr=0, returns a single client binding as read. 
**    Only called from other functions which have already opened lease 
**    file. 
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *                        
*/  
struct client find_lease(FILE *fp, unsigned char chaddr[16], int *status) 
{
    int i1, rc;
    struct client cid;
    long offset;
    const struct Eid ezero = {0,0,0,0,0,0};

    if (memcmp((char *) chaddr, 0, 16) != 0) 
        rewind (fp);
    for ( ; ; ) {
        /* Note position in file */
        offset = ftell(fp);
        rc = fscanf(fp, "%x:%x:%x:%x:%x:%x %lu %d", &cid.haddr.c[0], 
            &cid.haddr.c[1], &cid.haddr.c[2], &cid.haddr.c[3], &cid.haddr.c[4], 
            &cid.haddr.c[5], &cid.iaddr.l, &cid.active);
        if (rc < 0) {
            *status = -2;
            if (rc == EOF) 
                *status = EOF;
            cid.iaddr.l = 0;
            memcpy((char *) cid.haddr.c, (char *) ezero.c, Eid_SZ);
            cid.active = 0;
            return cid;
        }
        if (memcmp((char *) chaddr, 0, 16) == 0) 
            return cid;
        if (memcmp((char *) cid.haddr.c, (char *) chaddr, Eid_SZ) == 0) {
        /* If lease is found, back up to just before it's position (so
           we can modify it in another function) */
            fseek(fp, offset, SEEK_SET);
            *status = 0;
            return cid;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** read_conf: reads DHCP configuration file 
**
** int read_conf(char fname[80], struct config * conf)
**
** PARAMETERS: 
**    (in)  fname: DHCP configuration file name
**    (out) conf: structure containing configuration information
**
** RETURNS:
**    0: OK
**   -2: Error opening file
**
** DESCRIPTION: 
**    Internal routine to read information from DHCP configuration 
**    file into configuration structure.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *                        
*/
int read_conf(char fname[80], struct config * conf)
{
    FILE * conffp;
    int i1;

    conffp = fopen(fname, "r");
    if (conffp == NULL) 
        return -2;
   
/*  Read in configuration info */
    fscanf(conffp, "%*s %d.%d.%d.%d", &conf->submask.c[0], &conf->submask.c[1], 
            &conf->submask.c[2], &conf->submask.c[3]);		
    fscanf(conffp, "%*s %d.%d.%d.%d %d.%d.%d.%d", &conf->rangel.c[0], 
        &conf->rangel.c[1], &conf->rangel.c[2], &conf->rangel.c[3], &conf->rangeh.c[0], 
        &conf->rangeh.c[1], &conf->rangeh.c[2], &conf->rangeh.c[3]);
    fscanf(conffp, "%*s %d", &conf->nrouter);
    for (i1 = 0; i1 < conf->nrouter; i1++) 
        fscanf(conffp, "%d.%d.%d.%d", &conf->routeaddr[i1].c[0], &conf->routeaddr[i1].c[1], 
            &conf->routeaddr[i1].c[2], &conf->routeaddr[i1].c[3]);
    fscanf(conffp, "%*s %d", &conf->ndns);
    for (i1 = 0; i1 < conf->ndns; i1++)
        fscanf(conffp, "%d.%d.%d.%d", &conf->dnsaddr[i1].c[0], 	&conf->dnsaddr[i1].c[1], 	
            &conf->dnsaddr[i1].c[2], 	&conf->dnsaddr[i1].c[3]);
    fscanf(conffp, "%*s %s", conf->dns_domain);
	fscanf(conffp, "%*s %s", conf->bootfile);
    if (strcmp(conf->bootfile, "none") == 0)
        conf->bootfile[0] = 0;  	    

    fclose(conffp);

    return 0;

}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
**
** write_arpcache: Modifies ARP cache in USNET netconf table
**
** int write_arpcache(unsigned char chaddr[16], unsigned long iaddr, 
**                   char hwaddyes, char ncstat) 
** 
** PARAMETERS: 
**    (in) chaddr: hardware address
**    (in) iaddr: internet address
**    (in) hwaddyes: flag for hardware address present
**    (in) ncstat: block status in netconf (0 invalidates entry)
**
** RETURNS:
**    0: OK
**   -1: Entry not found
**
** DESCRIPTION: 
**    Internal routine to write parameters to ARP cache in netconf table. 
**    When called with:
**    (chaddr=0, iaddr, x, x) finds entry by iaddr and writes iaddr=0 to entry
**    (chaddr, iaddr, x, x)   finds entry by chaddr and writes iaddr to entry 
**            
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *                        
*/
int write_arpcache(unsigned char chaddr[16], unsigned long iaddr, 
                   char hwaddyes, char ncstat) 
{
    int i1;
    const struct Eid ezero = {0,0,0,0,0,0};
    struct NETCONF  *confp;

    for (i1 = 0; i1 < NCONFIGS; i1++) {
        confp = &netconf[i1];
        /* If chaddr = 0, search by iaddr */
        if (memcmp((char *) chaddr, (char *) &ezero, Eid_SZ) == 0) {
            if (confp->Iaddr.l == iaddr) 
                break;
            continue;
        }
        /* Search by chaddr */
        if (memcmp((char *) &confp->Eaddr, (char *)chaddr, Eid_SZ) == 0) {
            break;
        }
    }
    if (i1 == NCONFIGS) 	/* didn't find entry */
        return -1;

    if (memcmp((char *) chaddr, (char *) &ezero, Eid_SZ) == 0) {
        confp->Iaddr.l = 0;
        confp->ncstat = 0;
    }
    else {
	    confp->Iaddr.l = iaddr;
        confp->hwaddyes = hwaddyes;
        confp->ncstat = ncstat; 
    }
    return 0;
}


