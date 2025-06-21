/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/route.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: route.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/*
    Copyright (C) By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com
*/
/* route.h */
#ifndef _ROUTE_H_
#define _ROUTE_H_

/* ADDed */
#define HOSTNAMESZ 20
#define MAXROUTES  8
#define MAXHOSTS   16

#define ROUTERR ((unsigned char)-1)

typedef unsigned char ETHaddr[6];

/* USER defined table */
typedef struct _hosts {
    IPaddr          ip;
    char           *name;
    char           *alias;
}               HOSTS;

/* USER defined table */
/*All routes must be define in dependant order */
typedef struct _routes {
    IPaddr          ip;         /* {0,0,0,0} default */
    IPaddr          mask;
    IPaddr          gw;
    unsigned char   metric;
    unsigned char   flag;
}               ROUTES;

/* FROM netconf.c file */
extern const struct _hosts hosts[];
extern const int hostscnt;
extern const struct _routes routes[];
extern const int routescnt;

typedef struct {
    char            name[HOSTNAMESZ];
    char            alias[10];
    IPaddr          ipaddr;
}               NAME;

#define STATICROUTE	0x01
#define HOSTROUTE	0x02
#define	GWROUTE		0x04
#define DEFAULTROUTE 0x08
#define	REDIR_MAKE	0x10
#define REDIR_MOD	0x20
#define IFACE		0x40

typedef struct {
    IPaddr          dest;       /* The base address of destination, cooked ,
                                   the net */
    IPaddr          mask;       /* The address mask */
    IPaddr          raw;        /* The origonal address for display */
    IPaddr          gw;         /* gateway address */
    unsigned long   use;
    unsigned char   flag;       /* flag byte */
    unsigned char   iface;      /* the index into to netconf table of device */
    unsigned char   metric;     /* metric info */
    unsigned char   confidx;    /* index into netconf table  for arp */
    long            livetm;     /* discard time */
}               ROUTE;

extern ROUTE    routetbl[MAXROUTES];
extern ROUTE   *routeptr[MAXROUTES];
extern int      routecnt;
extern NAME     hosttbl[MAXHOSTS];
extern int      hostcnt;

extern ROUTE   *find_route(IPaddr * dest);
extern IPaddr  *local_host(char *name);
extern int      add_hosts(char *name, char *alias, IPaddr * addr);
extern ROUTE   *add_route(const IPaddr * dest, const IPaddr * mask, const IPaddr * gw, int metric, int flag);

#endif          /* _ROUTE_H_ */
