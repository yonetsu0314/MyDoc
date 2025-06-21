/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/route.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: route.c $
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
/* ROUTE.C  Edition: 16  Last changed: 27-Sep-99,10:09:52  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    ROUTE.C -- Routing Routines for USNET

    Copyright (C) 1993, 1995, 1998, 1999 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    http://www.ussw.com
    (503) 844-6614

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

  27-SEP-1999  TTT  Added ; before line with label and }
  23-SEP-1999  SA   Reworked BuildRoutes(); old version in UNSUPP\buildrt.c
  07-MAY-1999  NJW  Removed debug Nprinf() from GetHostData()
  22-APR-1999  NJW  Protect entries in cache queue; added ussHostAcquire()
                      ussHostRelease(), ussHostUsed(), and USS_KEEP_HOSTS.
  17-FEB-1999  TTT  Modified handling of dynamically defined router
  04-JAN-1999  RCA  Added multicast support.
  22-OCT-1998  NJW  Added check in BuildRoutes() for hosts out side defined
                      network.  These hosts are now assigned default routers

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

/*
**  Routing and host configuration information
**
**  USS_KEEP_HOSTS:  Indicates that USNET should retain all entries
**       in the host table currently being used by the application.
**       If the host has a TCP or UDP connection, then the entry in 
**       the netconf[] table is flagged as used and host entry will not
**       timeout and be reused.  If reuse of entries is mandatory,
**       USS_KEEP_HOSTS should be undefined.  
**
**  Do not protect hosts in cache queue:
**    #undef USS_KEEP_HOSTS
*/
/*  Protect hosts in cache queue: */
#define USS_KEEP_HOSTS

#define NHOPS 8                 /* maximum number of hops */

/* constants stored in routflg to indicate type of route */
#define NETKNOWN   0
#define DEFAULTGW  1
#define BLINDFAITH 4

extern struct NETCONF netconf[];
extern const int confsiz;
#ifdef DNS
extern Iid      DNSiid[2];
#endif

/*
**  Cache queue:  Used to keep track of any hosts that are
**    entered into the netconf[] host table while
**    the application is running.  Hosts are added by the
**    IP layer through the use of the GetHostData() call. 
**    Hosts defined in netdata[] are not part of the cache 
**    queue because they are statically defined in netconf.c
*/
unsigned char   cqfirst,
                cqlast;

/* 
*/
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  BuildRoutes()       Build routing table based on host table netconf[]
**
**  int BuildRoutes( void );
**
**  PARAMETERS:
**      N/A
**
**  RETURNS:
**      0           Always returns 0.  Table netconf[] is updated with
**                    routing information.
**
**  DESCRIPTION:  
**      This routine builds the routing information for the complete 
**      configuration table, netconf[].  The information for each host 
**      consists of the network number for the first hop, and the total 
**      number of hops needed.  If there is no route, these two fields 
**      are stored as 255.
**
**  USAGE/COMMENTS:  
**      BuildRoutes is called in Ninit() right after the ROM netdata[]
**      table is copied to the RAM netconf[] table.  BuildRoutes() can
**      be called after USNET has hosts dynamically added, but hosts
**      outside subnet will assume origignal default routers.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  27-SEP-1999  TTT  Added ; to line 553
**  23-SEP-1999  SA   Reworked BuildRoutes() so it runs faster.
**  22-APR-1999  NJW  Added header; zeroed netconf[].gnid for reuse as
**                      host entry acquisition level.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int             BuildRoutes(void)
{
    int             i1,            /* Generic counter and index */
                    i2,            /* Generic counter and index */
                    i3,            /* Generic counter and index */
                    i4,            /* Generic counter and index */
                    uix,           /* Unmatching hop index into netconf[] */
                    confix,        /* Index into netconf[] table */
                    confix2,       /* Index into netconf[] table */
                    maxhop,        /* Max number of hops from local */
                                   /*   host to remote host */
                    fnode,         /* First node to check when counting */
                                   /*   hops */
                    sfnode,        /* Used to store value of fnode */
                    netno,         /* Physical Network interface id */
                    hops,          /* Counter for number of hopes */
                    remix;         /* Remote host index in netconf[] */
    int             routerix,      /* Router index in netconf[] */
                    routernet,     /* Router network netno */
                      routerhops;    /* Router hops */
    int             hix[NHOPS * 2];/* Host index for hops */
    Iid             iid,           /* Generic IP address */
                    hiid[NHOPS];   /* IP addresses for each hop */
    struct NETCONF *confp,         /* Pointer to netconf[] entry */
                   *confp2,        /* Pointer to netconf[] entry */
                   *confp3,        /* Pointer to netconf[] entry */
                   *confp4;        /* Pointer to netconf[] entry */

    /* 
    ** A loop to prepare for the routing analysis:
    **    1.  Sets NODE flag if host has more than one network interface
    **    2.  Counts number of unique networks defined by (Imask & Iaddr)  
    **    3.  Gets the DNS server.  If an IP address 
    **        is 0 (RARP/BOOTP/DHCP needed), we use the 
    **        port name to determine  the network.
    */
    routerix = -1, routernet = -1, routerhops = -1;   /* assume no router */
    for (maxhop = confix = fnode = 0; confix < NCONFIGS; confix++) {
        confp2 = confp = &netconf[confix];
        if (confp->ncstat == 0)        /* skip entry if not real entry */
            continue;
            
        confp->flags &= ~NODE;         /* assume it is not a NODE */ 
        
        /*
        **  If no IP address, check for common port names
        **    between current entry, confp, and any other
        **    entry.  This defines connectivity through a 
        **    physical interface.
        */
        if (confp->Iaddr.l == 0) {     
            for (confix2 = 0; confix2 < NCONFIGS; confix2++) {
                confp2 = &netconf[confix2];
                if (confp->Imask.l != confp2->Imask.l)
                    continue;          /* Skip if masks don't match */
                    
                if (confp2 == confp)
                    continue;          /* Skip if self */
                    
                if (strcmp(confp->pname, confp2->pname) != 0)
                    continue;          /* Skip if port names don't match */
                    
                goto lab11;            /* Got match! */
            }
            continue; /*  IP address is 0 and no match for port name so
                      **  goto next iteration of the routing
                      **  analysis preparation loop. 
                      */       
        }
        
        /*
        **  Store unique network address (Imask & Iaddr)
        */
lab11:  iid.l = confp2->Iaddr.l & confp2->Imask.l;

        /*
        **  Check to see if current entry is actually a NODE
        */
        for (confix2 = 0; confix2 < NCONFIGS; confix2++) {
            if (confix == confix2)     /* skip self */
                continue;
            confp2 = &netconf[confix2];
            if (confp2->ncstat == 0)   /* not a valid entry */
                continue;
            if (strcmp(confp->name, confp2->name) == 0) 
                confp->flags |= NODE;  /* host is listed more than once */
                                       /*   so it is a NODE */
        }    
        
        /*
        **  Store all unique network IDs.
        */
        for (i2 = 0; i2 < maxhop; i2++)
            if (iid.l == hiid[i2].l)
                goto cnt3;
        hiid[maxhop++] = iid;
cnt3:   confp->gnid = i2; /*  gnid is used later for routing; tells us if 
                          **  the subnet id's match; if 2 entries have the
                          **  same gnid value then their subnet id's match. 
                          */

#ifdef DNS
        if ((confp->flags & DNSVER) && fnode < 2)
            memcpy((char *) &DNSiid[fnode++], confp->Iaddr.c, sizeof(Iid));
#endif
    }
    maxhop <<= 1;                     /* Multiply by 2; used in hops calc */

    /* 
    ** Build the cache queue.  The first 'if' statement checks to see if
    ** BuildRoutes() is being executed immediately after netconf[] is cleared
    ** in Ninit().  Otherwise this is a rebuild, and the cache queue is left
    ** as is.
    */
    if (netconf[NCONFIGS - 1].cqprev == 0) {
        i1 = confsiz;
        for (confix = NCONFIGS - 1; confix >= confsiz; confix--) {
            confp = &netconf[confix];
            netconf[i1].cqnext = confix;
            confp->cqprev = i1;
            confp->gnid = 0;           /* Field gnid protects host entries */
            i1 = confix;
        }
        cqfirst = NCONFIGS - 1;        /* First entry is last in netconf[] */
        cqlast = i1;                   /* Last is after prededined hosts */
    }
    
    /*
    ** We have to first of all find the default router index and netno
    ** if there is one; the first entry with the ROUTER flag set is the 
    ** default router. The default router HAS to be connected to the 
    ** network else the results will be in error.
    */
    for (i1 = 0; i1 < NCONFIGS && routerix < 0 ; i1++) 
         if (netconf[i1].flags & ROUTER)  
             routerix = i1;

    /* 
    ** This is to find the routernet - netno to goto the router; 
    ** and to set the routerhops to default router value.
    */
    if ( routerix >= 0 ){
        for (i2 = 0; i2 < NCONFIGS && routernet < 0; i2++)
             if ( (netconf[i2].flags & LOCALHOST) != 0 &&
            netconf[routerix].gnid == netconf[i2].gnid )
                             routernet = netconf[i2].netno;   /* got match */
        routerhops -= 1; /* set routerhops = 254 for default router */
    }           

    /* 
    ** Loop over each host to determine:
    **    netconf[].hops == Number of hops packet makes from a LOCALHOST
    **              to entry remix.
    **    netconf[].nexthix == The entry in netconf[] that all packets should
    **              be sent to to reach entry remix.  This is the first hop.
    **    netconf[].netno == Physical network interface the LOCALHOST should
    **              send packets through to reach entry remix.
    */
    for (remix = 0; remix < NCONFIGS; remix++) {
        
        /*
        **  Check to see if this remix entry has been defined
        **  as outside prefined netdata[] network.  If hops
        **  has a value > =  253, then this entry was defined 
        **  while interacting with the network.  
        **    1.  Assume the next hop index is it self
        **    2.  Assume no network interface
        **    3.  Set hops to 255 (set to 510; gets divided by 2 later)
        **  Hop of 255 forces this entry to pick up the
        **  default router, if any.
        */
        confp3 = &netconf[remix];
        if ( confp3->hops >= 0xFD ) { /* 253 */
            hix[2] = confp3->nexthix; /* Set relay entry */
            /*
            ** Do not modify interface number for a dynamically
            ** defined router; i.e. one provided by DHCP
            */
            if (confp3->flags & ROUTER)
               netno = confp3->netno; /* Interface number already set */
            else
               netno = 255;           /* Set interface number */
            /*
            ** If entry is a router, set hops to 0; otherwise
            ** assume no route to host - will assign to use router later
            */
            hops = (confp3->flags & ROUTER) ? 0 : 510;                
            goto lab6;     /* Update netconf[] table with route info */ 
        }

        /* 
        ** Skip LOCALHOST entries and invalid entries 
        */
        if (confp3->ncstat == 0)       /* Skip entries that are invalid */
            continue;
        if (confp3->flags & LOCALHOST){/* Skip LOCALHOSTs; */
            confp3->nexthix = remix;   /* Point to self */
            continue;                  /* next remix */
        }

       /*
       ** Check to see if the remix is not connected to anybody. Here
       ** we assume that if an entry is a NODE or if the entry is 
       ** connected to another NODE then the entry is connected. If
       ** the entry is connected directly to the LOCALHOST then also it
       ** is connected.
       ** If not connected then make it use the default router if any.
       */
       if (netconf[remix].flags & NODE)
           goto conrem;    /* if remix is a node assume it is connected. */
       for(i1 = 0; i1 < NCONFIGS; i1++)
           if ( (netconf[remix].gnid == netconf[i1].gnid) && (i1 != remix)
                 && (netconf[i1].flags & (NODE | LOCALHOST)) )
                 goto conect;  /* if remix has a same subnet id as any
                               ** other NODE or LOCALHOST 
                               ** assume it is connected. 
                               */                                             
       /* 
       ** This host is disconnected so let it use the default router
       ** if there is one else mark it as disconnected. 
       */
       goto discon;            /* fill table appropriately */

conrem:/* 
       ** Check if this remix name already has route found before - 
       ** if any other previous entry has the same name as this remix
       ** then we do not need to find the route to this remix - we can
       ** use the route of the previous entry.
       */
       for (i2 = 0; i2 < remix; i2++) 
           if ( strcmp(netconf[i2].name,netconf[remix].name) == 0) {
                hix[2] = netconf[i2].nexthix;
                hops  = netconf[i2].hops*2; /* gets divided later */
                netno = netconf[i2].netno;
                goto lab6;         /* route found - fill in the table */
           } 

conect:/* A suitable route has to be found since this host is 
       ** connected. 
       **  Find index of first node that should be checked for routing hops
       **  Count NON-NODE entries, invalid entries, and skip self
       **  (even if it is a node).
       **  fnode is the first entry found in table that COULD
       **  be a hop(only a node can be a hop). The LOCALHOST or the 
       **  remote host cannot be used as a hop.
       */
       fnode = 0;                     /* Assume zero */
       /* condition true only if not a node or is self or is invalid entry 
       ** or is remote host or has the same name as remix. */
       while ((netconf[fnode].flags & (NODE | LOCALHOST)) != NODE ||
               netconf[fnode].ncstat == 0 || 
               fnode == remix ||
               strcmp(netconf[fnode].name,netconf[remix].name) == 0 ) {
            fnode++;
            if (fnode >= NCONFIGS) {   /* No valid first node found */
                fnode = 0;             /* Assume 0 */
                break;                 /* out of while loop */
            }
       }
       sfnode = fnode;                 /* save the value of fnode */

       /* 
       ** Look for a solution with "hops" hops, as low as possible 
       ** remember that hops will be even only because one node has two
       ** hops the first entering the host and the other within the host
       ** eg: hix[2] is the hopindex entering Node 1 and hix[3] will be 
       ** the hopindex for leaving Node 1. Node 1 is the first hop from
       ** source to destination.
       */
       for (hix[2] = remix, hops = 2; hops <= maxhop; hops += 2) {
            hix[hops] = remix;
            
            /* 
            ** First leg is any local network connection
            ** only routes from the LOCALHOST are calculated - if the entry
            ** is not the LOCALHOST the loop continues to the next 
            ** entry of the table.
            ** hix[1] = hopindex[1] is the LOCALHOST from which we are 
            ** trying to find the route to the remote host index.
            ** hix[2] = first hop, and so on, upto hix[hops - 1] = last hop,
            ** hix[hops] = remote host(hop)index.
            ** A hop may be inside a host(hix[even] to hix[odd]) or from 
            ** one host to another(hix[odd] to hix[even])       
            */
            for (hix[1] = 0; hix[1] < NCONFIGS; hix[1]++, fnode = sfnode) {
                confp4 = &netconf[hix[1]];
                if (confp4->ncstat == 0 || (confp4->flags & LOCALHOST) == 0)
                   continue;    /* invalid or non LOCALHOSTs are skipped */          
                netno = confp4->netno;     /* Remember network interface */
nexttry:        uix = 2;          /* initialize the unmatching hopindex */
                for (i2 = 2; i2 < hops; i2++)
                /* Initialize all connection hops to first node. hix[2] 
                ** is the first hop and so on.... 
                */
                   hix[i2] = fnode;     /* Initialize connection hops */
                goto lab2;        /* check if all intermediate hops 
                                  ** are good.
                                  */
                
nxt2:           uix = i2; 
                /* i2 is the index of hix[] at which it does not match */
                i1 = (hix[i2] + 1) % NCONFIGS;
                /* i1 is the next entry of the netconf[] table after the 
                ** unmatching hopindex. If i1 = any of the hops already
                ** found increment i1, since these can't be a hop 
                ** again anyways. 
                */
                for (i4 = 1; i4 <= hops; i4++){
                   /* check if next entry(i1) is already past table */
                   if (i1 == fnode)  /* gone through the table once */ 
                      goto nxtfnode; /* change the first node */
                   if ( i1 == hix[i4] || 
                        (netconf[i1].flags & (NODE | LOCALHOST)) != NODE ||
                         netconf[i1].ncstat == 0 ){
                      i1 = (++i1) % NCONFIGS;             /* next entry */
                      i4 = 0;    /* check again from hix[1] to hix[hops].*/
                   }
                } 
                hix[i2] = i1;  /* make hopindex[i2] the next entry index */
lab2:           /* 
                ** Loop from second hop to the last hop(before the remix)
                ** A hop has two legs: 
                **   1) to a host on the same network - subnet id must match 
                **   2) to a network connection in the same host - host name 
                **      match.  
                ** Loop over all the legs and check that they
                ** connect for both the above cases. Check only from the
                ** unmatching hopindex onwards since the previous hops are
                ** good. 
                */
                for (i2 = (uix & 1) ? uix - 1 : uix; i2 < hops; i2 += 2) {
                  /* 
                  ** Check for the subnet id match between a hop
                  ** and its previous hop; if they do not match
                  ** then goto the next entry ahead in the table.
                  */
                  if ((netconf[hix[i2]].gnid != netconf[hix[i2 - 1]].gnid)
                       || ( hix[i2] == hix[i2 - 1]) )                                           
                       goto nxt2;              /* try the next entry */
                    
                  /* 
                  ** Compare the names to see if they have the same host 
                  ** name. compare the names of this hop and the next hop
                  ** (within the same host); if they do not match then goto
                  ** the next entry ahead.
                  */
                  if ( (strcmp(netconf[hix[i2 + 1]].name,
                               netconf[hix[i2]].name) )
                         || (hix[i2] == hix[i2 + 1]) ){
                       i2++;  /* the index where it does not match */ 
                       goto nxt2;      /* try the next entry */
                  }
                }
                /* 
                ** Since it passed the above test
                ** all intermediate hops were good, but does the last 
                ** one go to connection "remix"? 
                */
                i1 = hix[hops - 1];                      /* the last hop */ 
                /* checks for the subnet match for the last hop to remix */
                if (i1 != remix && netconf[i1].gnid == confp3->gnid)  
                    goto lab6;        /* route found - fill in the table */

                /* this loop is to check for remote hosts with the 
                ** same host name. 
                */
                for (i3 = 0; i3 < NCONFIGS; i3++)
                   /* if the last hix has the same subnet as this entry
                   ** and this entry has the same name as remix then we can
                   ** use this entry as the last hix - and the route can be 
                   ** found.
                   */ 
                   if ( (netconf[i3].gnid == netconf[i1].gnid) &&
                        (strcmp(netconf[i3].name,netconf[remix].name) == 0)) {
                      i4 = i3;
                      do { 
                         /* find the entry which has the same subnet as the
                         ** source - i.e. that will be the first hop.
                         */
                         if( (hix[1] != i4) &&
                              (netconf[hix[1]].gnid == netconf[i4].gnid) )
                              /* Confirm if this is actually the first hop 
                              ** by checking for a name match with the next
                              ** hix. If hops = 2(1)there will be no 3rd hix
                              */
                              if ( hops == 2 || strcmp(netconf[hix[3]].name,
                                  netconf[i4].name) == 0 ){
                                  hix[2] = i4;    /* this is the first hop */
                                  break;
                              } 
                         i4 = (i4 + 1) % NCONFIGS;
                      }
                      while (i4 != i3);
                      goto lab6;   /* if we have a match it means this remote 
                                   ** host's name is already there in the 
                                   ** table and we have found the route.
                                   */
                   }
                if (hops > 2){
                  if (fnode >= NCONFIGS) /* exhausted all fnode 
                                         ** possibilities
                                         */  
                         goto nexthix;   /* next hix[1] loop */
                  i2 = hops - 1;  /* last hop does not match */ 
                   goto nxt2;      /* try next entry for last hop */
                }

nxtfnode:       /* if hops > 2 change the first node to another node and 
                ** try again, else goto nexthix[1].
                */
                if (hops > 2) {
                  fnode++; 
                  while((netconf[fnode].flags & (NODE | LOCALHOST)) != NODE 
                         || netconf[fnode].ncstat == 0 || fnode == remix ||
                            strcmp(netconf[fnode].name,
                                   netconf[remix].name) == 0 ){
                      if (fnode >= NCONFIGS) /* exhausted all fnode 
                                             ** possibilities
                                             */  
                         goto nexthix;       /* next hix[1] loop */ 
                      fnode++;             
                  } 
                   goto nexttry; /*  find the route again with different
                                 **   first node.
                                 */
                } 
nexthix:        ; 
            }                              /* end for(;;) loop of hix[1] */
       }                                   /* end for(;;) loop of hops */
discon:/* not connected */
       hops = routerhops*2;  /* set router hops */ 
       netno = routernet;    /* set router network interface */
       hix[2] = routerix;    /* Set default router index */
lab6:  confp3->hops = hops >> 1;      /* divide by 2 */
       confp3->netno = netno;         /* Set interface */
       confp3->nexthix = hix[2];      /* set first hop */
    }                                 /* end for(;;) loop of remix */
    return 0;
}  /* end BuildRoutes() */

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  GetHostData()      search for route for a given IP address
**
**  int GetHostData(unsigned long iidl, int flag);
**
**  PARAMETERS:
**     (in)     iidl   IP address
**     (in)     flag   bit 0 = create if not present
**                         1 = broadcast address not legal
**                         2 = create entry even if route not known
**                         3 = do not send to default router
**                         4 = accept unreachable
**
**  DESCRIPTION:  
**     This routine searches for a route for a given IP address, and creates
**     it if necessary and possible.
**
**  RETURNS:
**     -1                         error
**     0 ~ USS_MULTICAST_INDEX-1  index to table netconf
**     USS_MULTICAST_INDEX        address is multicast address
**     USS_BROADCAST_INDEX        address is limited broadcast address
**
**  USAGE/COMMENTS:
**     GetHostData() will return an index into the netconf[] table.
**     Entries in the cache queue will be reused if they have not
**     been updated recently.  This function tries to protect routers
**     and if USS_KEEP_HOSTS is defined, will protect entries that
**     are actively being used.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  07-MAY-1999  NJW  Removed debug Nprintf() statement
**  22-APR-1999  NJW  Protect active entries in cache queue.
**  04-JAN-1999  RCA  Added multicast support.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int             GetHostData(unsigned long iidl, int flag)
{
    int             i1,
                    i2,
                    cntidx,       /* Counts entries check in cache queue */
                    found_idx,    /* Flag showing entry is a router */
                    idx,          /* Loop index into netconf[] */
                    confix,       /* Index into netconf[] */
                    routflg;      /* Routing mechanism flag */
    unsigned long   ul1;
    struct NETCONF *confp,        /* Pointer to entry in netconf[] */
                   *confp2;       /* Pointer to entry in netconf[] */

    /* 
    ** If full broadcast, return USS_BROADCAST_INDEX or error.  
    ** If 0, return error.
    */
    if (iidl == 0)
        return -1;
    if (iidl == 0xffffffff) {
        if (flag & 2)
            return -1;
        return ussBroadcastIndex;
    }
    else if (USS_ISCLASSD(iidl)) {
        return ussMulticastIndex;
    }

    /* 
    ** If the IP address itself is in the table, we return the index.
    */
again:
    for (confix = 0; confix < NCONFIGS; confix++) {
        confp = &netconf[confix];
        if (confp->ncstat == 0)
            continue;
        ul1 = confp->Iaddr.l;
        if (iidl != ul1)
            continue;
        /* 
        ** Move dynamic entry to beginning of the cache queue 
        */
refresh:
        if (confp->ncstat == 3) {
            BLOCKPREE();
            if (confp->Iaddr.l != ul1) {
                RESUMEPREE();
                goto again;
            }
            if (confix != cqfirst) {
                netconf[confp->cqprev].cqnext = confp->cqnext;
                if (confix != cqlast)
                    netconf[confp->cqnext].cqprev = confp->cqprev;
                else
                    cqlast = confp->cqprev;
                netconf[cqlast].cqnext = netconf[cqfirst].cqprev = confix;
                confp->cqnext = cqfirst;
                confp->cqprev = cqlast;
                cqfirst = confix;
            }
            RESUMEPREE();
        }
        if (confp->netno == 255 && (flag & 0x10) == 0)
            return -1;
        return confix;
    }

    /* 
    **  We'll build an entry for the host if we can.  First we 
    **  check for multicast destinations. 
    */
    routflg = NETKNOWN;

    /* 
    ** Next we check if the network or the subnet is in the table.
    */
    for (i1 = 0, i2 = 255; i1 < NCONFIGS; i1++) {
        confp = &netconf[i1];
        if (confp->ncstat == 0)
            continue;
        if ((iidl & confp->Imask.l) != (confp->Iaddr.l & confp->Imask.l))
            continue;
        if (confp->hops < (unsigned int) i2)
            confix = i1, i2 = confp->hops;
    }
    /* 
    ** For subnet broadcast, return index to subnet if not local, 
    ** 255 otherwise 
    */
    if (i2 < 255) {
        confp2 = &netconf[confix];
        ul1 = confp2->Iaddr.l;
        if ((iidl | confp2->Imask.l) == 0xffffffff) {
            if (flag & 2)
                return -1;
            if (confp2->hops < 2)
                return 255;
            confp = confp2;
            goto refresh;
        }
        goto lab6;
    }
    /* 
    ** No information matched, find a default router.
    */
    if (flag & 8)
        return -1;
    routflg = DEFAULTGW;
    for (confix = 0; confix < NCONFIGS; confix++) {
        confp2 = &netconf[confix];
        if (confp2->ncstat == 0)
            continue;
        if ((confp2->flags & ROUTER) == 0)
            continue;
        goto lab6;
    }

    /* 
    **  Nothing found. If flag tells so we'll create the entry 
    **  anyway and the caller will complete it.
    */
    if ((flag & 4) == 0)
        return -1;
    routflg = BLINDFAITH;

    /* 
    ** Here we'll build a host entry.  Take the top entry of the 
    ** old age queue.
    */
lab6:
    if ((flag & 1) == 0)
        return -1;
        
    /* 
    **  Attempt to return the oldest entry of the cache queue.  USNET 
    **  needs to be aware of any host that may use the returned entry 
    **  as a router.  If any other host has the current entry as the 
    **  next hop, then skip the entry and go to the next entry in the 
    **  cache queue.  If all entries are used as a router, then return
    **  the oldest one.  If the entry is actively being used by the
    **  application, skip it.  netconf[].gnid is the acquisition level.
    **
    **  confsiz = Number of entries in netdata[] table.
    **  (NCONFIGS-confsiz) = Number of entries in cache queue
    */
    cntidx = 0;                        /* Initialize index count */
    BLOCKPREE();                       /* Protect linking of queue */
next_idx:                              /* Try again label */
    cntidx++;                          /* Number of entries tried */
    confix = cqlast;                   /* Pick up next entry */
    confp = &netconf[confix];          /* Point to it */
    found_idx = 0;                     /* Inidcate not cross linked */

    if ( confp->ncstat != 0 ) {        /* Check defined entries */
      for ( idx = cqfirst;             /* Start at back of queue */
           (idx != confix) && !found_idx;/*Quit if linked or end */
            idx = netconf[idx].cqnext) {/* Point to next entry */
        if( netconf[idx].ncstat == 0 ) /*  Has entry been defined? */
          continue;                    /*   Skip undefined entires in q */
        found_idx = (confix == netconf[idx].nexthix );/* Found link? */
      }  /* end for(idx) */
    }  /* end if ncstat is not 0 */ 

    cqlast = confp->cqprev;            /* Relink queue, next oldest */
    confp->cqnext = cqfirst;           /* Move to newest */
    netconf[cqfirst].cqprev = confix;  /* Link entry in */
    cqfirst = confix;                  /* Put entry in to newest position */
    if ( cntidx < ( NCONFIGS - confsiz ) ) {/*Been through cache que? */
      if ( found_idx ||                 /*  If router */
          ussHostUsed(confix)) {       /*     or being used */
        goto next_idx;                 /*   Try again */
      }
    }
    RESUMEPREE();                      /* Done processing list */
    
    if ( ussHostUsed(confix) ) {       /* If entry is used */
      return -1;                       /*   Can't be reused */
    }

    /* 
    ** We have a table entry and we'll fill it in here.
    */
    confp->Iaddr.l = iidl;
    confp->name[0] = confp->pname[0] = 0;
    confp->Imask.l = 0;
    if (routflg == BLINDFAITH) {
        confp->Imask.l = 0xffffffff;
        confp->nexthix = 255;
    } else {
        if (routflg)
            confp->hops = confp2->hops + 1;
        else {
            strcpy(confp->pname, confp2->pname);
            confp->Imask = confp2->Imask;
            confp->hops = confp2->hops;
        }
        if (confp->hops <= 1) {
            confp->hwaddyes = 0;
            confp->nexthix = confix;
        } else
            confp->nexthix = confp2->nexthix;
        confp->netno = confp2->netno;
    }
    confp->ncstat = 3;
    return confix;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ussHostAcquire()         Acuire host entry to protect from over writes.
**
**  void ussHostAcquire( int confix );
**
**  PARAMETERS:
**     (in)     confix       Index of entry in netconf[] to acquire
**
**  RETURNS:
**     N/A
**
**  DESCRIPTION:  
**     This function increments the specified host entry's acquisition 
**     count, netconf[confix].gnid.  If this count is 0, the entry is
**     not currently actively being used by the application.  If it
**     is non-zero the entry is actively being used.  This routine
**     will check the value of gnid to ensure it makes sense within
**     USNET's build context.  If confix is invalid, nothing is done.
**     The acquisition level is decremented by calling ussHostRelease()
**     and the entry's acquisition level is checked by calling
**     ussHostUsed().
**
**  USAGE/COMMENTS:  
**     Several files within USNET use this routine to acquire the
**     host's entry in the netconf[] table.  The DPI will protect
**     TCP and UDP hosts on Nopen().  The BSD interface will only 
**     protect TCP hosts on calls to connect() and accept().  ICMP
**     redirect will protect the new router entry.  This function is 
**     only used when USS_KEEP_HOSTS is defined.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  NJW  Created function
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void ussHostAcquire(int confix){
#ifdef USS_KEEP_HOSTS        /* Include only if required */

  struct NETCONF *confp;               /* Pointer to host table */

  if( confix >= NCONFIGS ) return;     /* Index out of range, return */
  confp = &netconf[confix];            /* Point to entry */
  if( confp->ncstat == 0 ||            /* If not yet defined or */
      confp->ncstat == 4 )             /* Entry is statically defined */
    return;                            /*   Return */
  if( confp->gnid >= NCONNS ||         /* If bad count or */
      confp->gnid >= 255 )             /* If Count exceeds max value */
    return;                            /*   Return */
  confp->gnid++;                       /* Increment access count */
  if( confix != confp->nexthix)        /* If router is listed */
    netconf[confp->nexthix].gnid++;    /*   Increament its count as well */

#endif /* USS_KEEP_HOSTS */
  /*
  **  NOTE:  There may be more than one router in the chain.
  **         We are assuming a single router.
  */  
  return;                              /* Return */
}  /* end ussHostAcquire() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ussHostRelease()         Release host entry to allow entry reuse
**
**  void ussHostRelease( int confix );
**
**  PARAMETERS:
**     (in)     confix       Index of entry in netconf[] to release
**
**  RETURNS:
**     N/A
**
**  DESCRIPTION:  
**     This function decrements the specified host entry's acquisition 
**     count, netconf[confix].gnid.  If this count is 0, the entry is
**     no longer actively being used by the application.  If it
**     is non-zero the entry is actively being used.  This routine
**     will check the value of gnid to ensure it makes sense within
**     USNET's build context.  If confix is invalid, nothing is done.
**     The acquisition level is incremented by calling ussHostAcquire()
**     and the entry's acquisition level is checked by calling
**     ussHostUsed().
**
**  USAGE/COMMENTS:  
**     Several files within USNET use this routine to release the
**     host's entry in the netconf[] table.  The DPI will release
**     the entry for TCP and UDP hosts on Nclose().  The BSD interface 
**     will only release TCP hosts on calls to closesocket().  ICMP
**     redirect will release the old router entry.  This function is 
**     only used when USS_KEEP_HOSTS is defined.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  NJW  Created function
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
void ussHostRelease(int confix){
#ifdef USS_KEEP_HOSTS        /*  Include only if required */

  struct NETCONF *confp;               /* Pointer to host table */
      
  if( confix >= NCONFIGS ) return;     /* Index out of range, return */      
  confp = &netconf[confix];            /* Point to entry */
  if( confp->ncstat == 0 ||            /* If not yet defined or */
      confp->ncstat == 4 ||            /* Statically defined or */
      confp->gnid <= 0 )               /* If Count at min avlue */
    return;                            /*   Return */
  confp->gnid--;                       /* Decreament access count */
  if( confix != confp->nexthix)        /* If router is listed */
    netconf[confp->nexthix].gnid--;    /*   Decreament its count as well */

#endif /*  USS_KEEP_HOSTS */
  /*
  **  NOTE:  There may be more than one router in the chain.
  **         We are assuming a single router.
  */  
  return;                              /* Return */
}  /* end ussHostRelease() */


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ussHostUsed()            Returns netconf[] entry's acquisition level
**
**  int ussHostUsed( int confix );
**
**  PARAMETERS:
**     (in)     confix       Index of entry in netconf[] host table
**
**  RETURNS:
**     0        Entry is free and can be reused
**    >0        Entry is actively being used by system.
**    -1        Entry is static or confix out of range.
**
**  DESCRIPTION:  
**     Return the acquisition level of specified entry in netconf[]
**     host table.  A return value of 0 indicates the entry is not
**     currently being used by the system so the entry can be re-used
**     if desired.  The acquisition count is netconf[confix].gnid.  
**     If the value is non-zero the entry is actively being used.  
**     If confix is invalid, an error is returned, -1. The acquisition 
**     level is incremented by calling ussHostAcquire() and the
**     acquisition level is decremented by ussHostRelease().
**
**  USAGE/COMMENTS:  
**     This routine is used by ICMP redirects to check if the 
**     re-directed host is actively being used by the system.  Function
**     GetHostData() uses this routine to check if the returned entry
**     should be protected.  This function is only used when USS_KEEP_HOSTS 
**     is defined.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-APR-1999  NJW  Created function
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int ussHostUsed(int confix){
#ifdef USS_KEEP_HOSTS        /* Include only if specified */

  if( confix >= NCONFIGS ||            /* Index out of range or */      
      netconf[confix].ncstat == 4 )    /* Entry is static */
    return(-1);                        /*   return */
  return ( netconf[confix].gnid );     /* Return acquisition level */
  
#else   /* USS_KEEP_HOSTS */

  return(0);

#endif  /* USS_KEEP_HOSTS */
}  /* end ussHostUsed */
