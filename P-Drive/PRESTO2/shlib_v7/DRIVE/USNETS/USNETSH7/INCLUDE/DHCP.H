/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/dhcp.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: dhcp.h $
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
/* DHCP.H  Edition: 4  Last changed: 5-Feb-99,15:49:48  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
   Copyright (C) 1998 By
   United States Software Corporation
   7175 NW Evergreen Parkway, Suite 100
   Hillsboro, Oregon 97124
   (503) 844-6614
   http://www.ussw.com

   DHCP.H: Include file definitions for USNET DHCP server

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
**
** MODIFICATION HISTORY
**
** 22-JAN-1999 TTT  Add client configuration option
** 04-JAN-1999 TTT  Clean up some comments
** 27-OCT-1998 TTT  Creation
**
* * * * * * * * * * * * * * * * * * * * * * * * *
*/

/* Define server name here. If you are using an embedded system, 
** (instead of DOS), you can define it to be LOCALHOSTNAME instead 
*/
#define DHCP_SERVER "test"

/* Define DHCP_PROBE to probe offered address with ICMP Echo.
** Undefine to disable probe
*/
#define DHCP_PROBE

/* Define DHCP_BROADCAST to always broadcast replies to the client.
** Undefine to unicast replies when client broadcast flag permits it.
*/
#undef DHCP_BROADCAST

/* DHCP configuration file */
#define CONF_FILE "dhcp.con"

/* DHCP client binding storage file */
#define LEASE_FILE 	"dhcp.lea"

/* DHCP client and server ports */
#define SERVER_PORT 67
#define CLIENT_PORT 68

/* Choose set of configuration options for server to return to client:
**
**    DHCP_CONFIG = 1: IP address only
**                = 2: IP address, Subnet Mask, Router, DNS server
*/
#define DHCP_CONFIG 2

/* Filename size for DHCP bootfile */
#define FILENAMESIZE 80         /* change if name longer */

/* Maximum delay in DHCP client retry loop */
#define MAXDELAY 30000         

/* End of user configurations. Do not change anything below
** this point.
*/

#define DHCPDISCOVER 	1
#define DHCPOFFER 	2
#define DHCPREQUEST 	3
#define DHCPDECLINE 	4
#define DHCPACK 	5
#define DHCPNAK 	6
#define DHCPRELEASE 	7
#define DHCPINFORM  8
#define BOOTREQUEST 1
#define BOOTREPLY 2

#ifndef EOF      /* check whether stdio.h already included */
#include <stdio.h>
#endif

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

#if DHCP_CONFIG == 2
static const char discopts[] = {99, 130, 83, 99, 53, 1, DHCPDISCOVER, 55, 5, 1, 3, 6, 12, 15};
#else
static const char discopts[] = {99, 130, 83, 99, 53, 1, DHCPDISCOVER};
#endif
static const char releopts[] = {99, 130, 83, 99, 53, 1, DHCPRELEASE};
static const char offopts[] = {99, 130, 83, 99, 53, 1, DHCPOFFER};
static const char ackopts[] = {99, 130, 83, 99, 53, 1, DHCPACK};
static const char nakopts[] = {99, 130, 83, 99, 53, 1, DHCPNAK};

struct client {		
	struct Eid haddr;
	Iid iaddr;
	short active; /* 0 = unused, 1 = active, 2 = offered, 3 = unavailable */
};


struct addrpool {
	Iid iaddr;
	short used; /* 0 = available, 1 = active, 2 = offered, 3 = unavailable */
};

struct config {
	Iid submask;
	Iid rangel;
	Iid rangeh;
	int nrouter;
	Iid routeaddr[5];
	int ndns;
	Iid dnsaddr[5];
	char dns_domain[65];
	char bootfile[128];
};


#define TIMEINMS(sec, curtime) \
    sec > 0x7fffffffUL/1000UL ? curtime + 0x7fffffffUL : sec*1000UL + curtime

extern short DHCPfirst;
extern short DHCPadv;

#define InitOption DHCPfirst = 0, DHCPadv = 0

#ifndef LITTLE
#define GETLONG2(w, net) memcpy(w, net, 6)
#define PUTLONG2(w, net) memcpy(net, w, 6)
#else
#define GETLONG2(w, net) w.c[0] = net[5], w.c[1] = net[4], \
        w.c[2] = net[3], w.c[3] = net[2], w.c[4] = net[1], \
		w.c[5] = net[0] 
#define PUTLONG2(w, net) net[0] = w.c[5], net[1] = w.c[4], \
        net[2] = w.c[3], net[3] = w.c[2], net[4] = w.c[1], \
		net[5] = w.c[0]   	
#endif

#define INADDR_ANY 0

/* Define some useful things from net.h and support.h */

#ifndef Eid_SZ		/* check to see whether net.h already included */

/* Ethernet address format */
#define Eid_SZ 6
struct Eid {
	unsigned char c[Eid_SZ];
};

/* IP address format */
#define Iid_SZ 4
typedef union {
	unsigned char c[Iid_SZ];
	unsigned short s[Iid_SZ/2];
	unsigned long l;
}			Iid;


#endif

#ifndef NLONG	/* check to see whether support.h already included */

#ifndef LITTLE
#define GETLONG(w, net) w.s[0] = net[0], w.s[1] = net[1]
#define PUTLONG(w, net) net[0] = w.s[0], net[1] = w.s[1]
#else
#define GETLONG(w, net) w.c[0] = net[3], w.c[1] = net[2], \
	w.c[2] = net[1], w.c[3] = net[0]
#define PUTLONG(w, net) net[0] = w.c[3], net[1] = w.c[2], \
	net[2] = w.c[1], net[3] = w.c[0]
#endif

#endif

/* Some definitions from local.h */

#ifndef NNETS

#define DNS 1

/* Local host name */
char            localhostname[32];
#define LOCALHOSTNAME(grommit) \
    if (getenv("HOST")) strcpy(grommit,getenv("HOST")); \
    else strcpy(grommit, "none")


#endif

void DHCPserv(void);
unsigned char *NextOption(struct DHCPstr * DHCPp);

