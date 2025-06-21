/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/support.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: support.h $
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
/* SUPPORT.H  Edition: 22  Last changed: 14-Dec-99,11:43:54  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1998 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    http://www.ussw.com

    SUPPORT.H -- Support Level Definitions for USNET

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
    
    06-OCT-1999  BSK  Add: #define IGMP if included
    15-SEP-1999  BTS  Chg: moved MESSH_SZ from net.h to here
    24-MAY-1999  BSK  Chg: prototype for Nmemcpy added
  	26-APR-1999  TTT  Chg: struct CONNECT rxstat, txstat from char to short
    21-APR-1999  SCD  Chg: struct NETCONF field flags from char to short
    08-FEB-1999  RCA  Chg: optN members of NET struct does not depend on PPP
    30-DEC-1998  BSK  Chg: index to PPP script is now a pointer, not int
    30-OCT-1998  BSK  Chg: Conditionally included PPP structure.  Added
                           prototype of MD5digest for PPP, SNMP, HTTP.
    30-SEP-1998  BSK  Add: PPP specific variables to NET

 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#define MESSH_SZ ((sizeof(MESS)+USSBUFALIGN-1)&~(USSBUFALIGN-1))


#if USS_IP_MC_LEVEL == 2
#define IGMP
#endif


/* =========================================================================
   Multitasking support.  You choose the multitasker when you install the
   product, or you can write your own MTMACRO.H, based on the supplied versions.
*/

/* event numbers, dependent on the multitasker */
#if 1
#define SIG_RC(conno) (conno+conno+EVBASE)      /* read connection */
#define SIG_CC(conno) (conno+conno+1+EVBASE)    /* control connection */
#define SIG_RN(netno) (2*netno+2*NCONNS+EVBASE) /* read network */
#define SIG_WN(netno) (2*netno+2*NCONNS+1+EVBASE)       /* write network */
#define SIG_ARP (2*(NCONNS+NNETS)+EVBASE)       /* ARP */
#define SIG_GEN (2*(NCONNS+NNETS)+1+EVBASE)     /* general purpose */
#else
#define SIG_RC(conno) 0x0100+conno    /* read connection */
#define SIG_CC(conno) 0x0200+conno    /* control connection */
#define SIG_RN(netno) 0x0400+27       /* read network */
#define SIG_WN(netno) 0x0800+28       /* write network */
#define SIG_ARP       0x1000+29       /* ARP */
#define SIG_GEN       0x2000+30       /* general purpose */
#endif

#include <mtmacro.h>
#if MT == 0
void            NetTask(int netno);
#else
TASKFUNCTION    NetTask(void);
#endif

/* =========================================================================
   Structures that are affected by LOCAL.H so they can't be in NET.H */

struct NET {                    /* structure defining a network */
    int             netstat;    /* status bits */
    PTABLE         *protoc[3];  /* link, driver, adapter protocol */
    unsigned char   confix;     /* number in configuration table */
    char            sndoff;     /* offset to sender's address */
    unsigned long   tout;       /* basic timeout in milliseconds */
    int             maxblo;     /* maximum block size for link */
    int             nettasktout;/* timeout for network task */
    char            cflags;     /* configuration flags */
    char            worktodo;   /* flag for network task */
    unsigned char   none1;      /* alignment */
    unsigned char   state;      /* state */
    struct FIFOQ16  arrive;     /* circular list of arrivals */
    MESS           *fragmq;     /* linked sorted list of fragments */
    MESS           *fragmh;     /* reassembled message */
    int             irno[4];    /* interrupt numbers */
    int             port;       /* I/O port */
    char FAR       *base[2];    /* for memory-mapped I/O */
    struct Eid      id;         /* board id */
    unsigned char   netno;      /* network number */
    char            hwflags;    /* hardware level flags */
    MESS           *bufbas;     /* input buffer base */
    MESS           *bufbaso;    /* output buffer base */
    struct FIFOQ16  depart;     /* circular list of departures */
    long            bps;        /* bits per second */
#ifdef DHCP
    unsigned long   DHCPserver; /* DHCP server IP address */
    unsigned long   LeaseTime;  /* DHCP times */
    unsigned long   RenewalTime;
    unsigned long   RebindingTime;
    unsigned long   DHCPsecs;
#endif
    char           *ifDescr;
    int             ifType;
    int             ifAdminStatus;
#ifdef MIB2
    unsigned long   ifLastChange;
    unsigned long   ifInOctets;
    unsigned long   ifInUcastPkts;
    unsigned long   ifInNUcastPkts;
#endif
    unsigned long   ifInDiscards;
    unsigned long   ifInErrors;
#ifdef MIB2
    unsigned long   ifInUnknownProtos;
    unsigned long   ifOutOctets;
    unsigned long   ifOutUcastPkts;
    unsigned long   ifOutNUcastPkts;
#endif
    unsigned long   ifOutDiscards;
    unsigned long   ifOutErrors;
#ifdef LQRP
    unsigned long   ownLQRPms;
    unsigned long   peerLQRPms;
    char            lastins[32];
    unsigned long   OutLQRs;
    unsigned long   InLQRs;
    unsigned long   InGoodOctets;
#endif
/* all hardware net structures must fit in SERIAL, use filler if necessary */
    struct SERIAL {             /* hardware net data for serial lines */
        unsigned long   ul1,
                        ul2;    /* miscellaneous */
        unsigned long   timems; /* time stamp */
        void            (*comec) (int, struct NET *);   /* character from driver */
        int             (*goingc) (struct NET *);       /* character to driver */
        char           *bufin;  /* input buffer pointer */
        char           *buflim; /* input buffer upper limit */
        char           *bufout; /* output buffer pointer */
        char            baudctl[2];     /* baud rate controls */
        int             chsout; /* output buffer counter */
        unsigned char   lastin; /* last arrived character */
        unsigned char   nxtout; /* next character out */
        char opt1, opt2, opt3, opt4;   /* various internal options */
        char opt5, opt6, opt7, opt8;
#ifdef PPP
        unsigned short locopts, remopts;
        char *userid, *passwd;      /* User ID and password for PPP */
        unsigned char *script, *sp; /* Script and script pointer */
        unsigned short st;          /* State of script */
        unsigned long mnum, idle_time, echo_time;
    } hw;
    struct FIFOQ16 future;  /* queued messages from dial on demand */
    Iid haddr, raddr;       /* New Host/Remote IP for PPP */
#else
    } hw;
#endif
};

struct NETCONF {                /* network configuration table - RAM */
    /*	配列のサイズはnet.h NETDATAのnameと同じにすること。*/
	char            name[64];    /* host name */

    char            pname[9];   /* port name */
    Iid             Imask;      /* address mask, 0 = host part */
    Iid             Iaddr;      /* internal (Internet) address */
    struct Eid      Eaddr;      /* external (Ethernet) address */
    short           flags;      /* configuration flags */
    char            ncstat;     /* block status */
    char            cqnext,
                    cqprev;     /* next, previous in cache queue */
    unsigned char   netno;      /* network number */
    unsigned char   hops;       /* number of needed hops */
    unsigned char   nexthix;    /* next host index */
    unsigned char   gnid;       /* for routing logic */
    char            hwaddyes;   /* flag for hardware address present */
#ifdef ARP
    char            ARPwait;    /* flag for message waiting for ARP */
    MESS           *ARPwaitmp;  /* 1 message waiting for ARP */
#endif
    unsigned long   timems;     /* time stamp */
};

struct CONNECT {                /* logical connection structure */
    char            blockstat;  /* connection block status: */
    /* 0 = free */
    /* 1 = allocated */
    /* 2 = close initiated */
    /* 4 = close in final timeout */
    short            txstat;     /* main level status flags, see below */
    short            rxstat;     /* nettask level status flags. see below */
    char            state;      /* state machine state */
    struct CONNECT *next;       /* link for incoming connection queue */
    int             backlog;    /* maximum size of backlog */
    int             icqcur;     /* num in icq, or idx of listening socket */
    unsigned char   sendack;    /* flag to send ACK */
    unsigned char   netno;      /* net number */
    unsigned char   confix;     /* target configuration index */
    unsigned char   routeidx;   /* route table index */
    unsigned char   arpidx;     /* arp table index */
    char            tcpflags;   /* TCP read places flags here */
    unsigned long   rxtout;     /* timeout value for receive */
    unsigned long   txtout;     /* transmission timeout */
    long            txvar,
                    txave;      /* values needed to calculate txtout */
    unsigned long   txseq;      /* my sequence number */
    unsigned long   rxseq;      /* the other sequence number */
    unsigned long   ackno;      /* acknowledged up to this */
    unsigned long   seqtoack;   /* next we'll ACK this number */
    unsigned long   ackdseq;    /* last ACK'd number */
    unsigned long   acktime;    /* time from which ACK delayed */
    MESS           *first,
                   *last;       /* linked list of arrived messages */
    MESS           *wackf,
                   *wackl;      /* linked list of messages waiting for ACK */
    unsigned char   ninque;     /* number of messages in input queue */
    unsigned char   nwacks;     /* number of entries in the WACK queue */
    unsigned char   wackmax;    /* maximum WACK length */
    unsigned char   wackslow;   /* WACK length where we slow down */
    MESS           *future;     /* linked sorted list of future messages */
    PTABLE         *protoc[5];  /* protocol path */
    int             myport;     /* my port number, network order */
    int             herport;    /* the other port number, network order */
    Iid             heriid;     /* requested remote Internet address */
    Iid             realiid;    /* received remote Internet address */
#ifdef MIB2
    Iid             offerediid; /* offered local Internet address */
#endif
    unsigned int    window;     /* how much we can send */
    unsigned int    mywindow;   /* size of our window */
    unsigned int    prevwindow; /* last advertized window */
    unsigned int    maxdat;     /* maximum data size for application */
    unsigned int    doffset;    /* user data offset in buffer */
    unsigned long   lastrxtime; /* time of last RX */
    char            urgflag;    /* urgent flag: 1 = data present */
    char            urgdata;    /* urgent data */
    unsigned short  pseudosum;  /* fixed part of pseudo-header checksum */
    unsigned short  frid;       /* fragment id for the IP header */
#ifdef IPOPTIONS
    char            IPOtxlen;   /* IP options for TX: length in bytes */
    char            IPOrxlen;   /* IP options for RX: length in bytes */
    char            IPOtxopt[40];       /* IP level options for write */
    char            IPOrxopt[40];       /* IP level options for read */
#endif
    int             urgcnt;     /* amount of urgent data */
    unsigned long   urgseq;     /* starting point of urgent data */
    MESS           *ostreamb;   /* output stream I/O buffer base */
};
#define S_FIN   0x01            /* main level txstat: write end of file */
#define S_MON   0x02            /* monitor mode */
#define S_KEEPA 0x04            /* keepalive mode */
#define S_PSH   0x08            /* push */
#define S_NOCON 0x10            /* no connection, for UDP */
#define S_URG   0x20            /* urgent, out of band */
#define S_STRM  0x40            /* stream */
#define S_NOWA  0x80            /* non-blocking */
#define S_PASSIVE 0x100	 		/* open started in LISTEN */

#define S_EOF   0x01            /* rx level stat: nothing more to read */
#define S_NOACK 0x02            /* we failed to get an ack */
#define S_UNREA 0x04            /* destination unreachable ICMP */
#define S_TIMEX 0x08            /* time exceeded ICMP */
#define S_QUENC 0x10            /* source quench ICMP */
#define S_FATAL 0x20            /* fatal error */
#define S_RST   0x40            /* restart message received */


/* =========================================================================
   Macros to simplify coding. */

/* Check/take a packet from network (physical connection) netno.
   These would be redifined in an environment that does not use USNET drivers.
*/
#define CHECK_DRIVER_PACKET(netno) QUEUE_EMPTY(netp, arrive)
#define TAKE_DRIVER_PACKET(netno, mp) QUEUE_OUT(netp, arrive, mp)

#ifndef LITTLE
#define NLONG short
#define NC2(val) val
#define GETLONG(w, net) w.s[0] = net[0], w.s[1] = net[1]
#define PUTLONG(w, net) net[0] = w.s[0], net[1] = w.s[1]
#else
#define NLONG char
#define NC2(val) (((val&0xff) << 8) | ((unsigned short)val >> 8))
#define GETLONG(w, net) w.c[0] = net[3], w.c[1] = net[2], \
	w.c[2] = net[1], w.c[3] = net[0]
#define PUTLONG(w, net) net[0] = w.c[3], net[1] = w.c[2], \
	net[2] = w.c[1], net[3] = w.c[0]
#endif

#ifdef __cplusplus
#define GLOBALCONST extern
#else
#define GLOBALCONST
#endif

/* Turn on and off the non-blocking mode.  This mode is not a good idea in
   multi-tasking applications. */
#define SOCKET_CLEAR(s) connblo[s].rxstat = 0
#define SOCKET_NOBLOCK(s) connblo[s].txstat |= S_NOWA, connblo[s].rxtout = 0
#define SOCKET_BLOCK(s) connblo[s].txstat &= ~S_NOWA, \
    connblo[s].rxtout = TOUT_READ
#define SOCKET_IPADDR(s) connblo[s].realiid
#define SOCKET_OWNIPADDR(s) netconf[nets[connblo[s].netno].confix].Iaddr
#define SOCKET_PUSH(s) connblo[s].txstat |= S_PSH
#define SOCKET_FIN(s) connblo[s].txstat |= S_FIN
#define SOCKET_TESTFIN(s) (connblo[s].rxstat & S_EOF)
#define SOCKET_ISOPEN(s) (connblo[s].state == 1)
#define SOCKET_HASDATA(s) (connblo[s].first != 0)
#define SOCKET_CANSEND(s, len) socket_cansend(s, len)
int             socket_cansend(int, unsigned int);
#define SOCKET_MAXDAT(s) connblo[s].maxdat
/* Set read timeout to milliseconds. */
#define SOCKET_RXTOUT(s, val) connblo[s].rxtout = val

/* =========================================================================
   Function prototypes for support functions. */

#ifndef Ndisable
void            Ndisable(void);
#endif
#ifndef Nenable
void            Nenable(void);
#endif
#ifndef Nputchr
void            Nputchr(char);
#endif

#if defined(memcpy_INASM) && !defined(WRAPTEST)
void Nmemcpy(void *, void *, int);
#else
#define Nmemcpy memcpy
#endif

MESS           *reaDD(int conno);
unsigned short  Nchksum(unsigned short *sp, int cnt);
unsigned short  Nportno(void);
unsigned long   Nclock(void);
int             Nchkchr(void);
int             Ngetchr(void);
int             Ninitsupp(void);
unsigned long   TimeMS(void);
void            SetTimeMS(unsigned long);
int             Nsprintf(char *buffer, char *format,...);
int             Nprintf(char *format,...);
int             Nsscanf(char *buffer, char *format,...);
void            Npanic(char *);
void            ConfLock(void);
void            ConfFree(void);
int             ConfFind(int startix, struct NETCONF * argp);
int             ConfDel(int argix);
int             ConfAdd(struct NETCONF * argp);
int             ConfRename(int argix, Iid iaddr);
void            ConfDisplay(void);
int             RARPget(int netno);
int             Ndial(int netno, char *phonenumber);
#ifdef ICMP
void            ICMPreply(MESS * mess, int type, int scode);
#else
#define ICMPreply(mess, type, scode)
#endif
#ifdef DHCP
int             DHCPget(int netno, unsigned long lease);
int             DHCPrelease(int netno);
#endif
#ifdef DNS
int             DNSresolve(char *fullname, IPaddr * iidp);
#endif
#ifdef PPP
int             MPjoin(int netno, int oldnetno);
int             MPleave(int netno);
#endif
void MD5digest(unsigned char *data, int len, unsigned char *digest);

/* =========================================================================
   Public variables to be used in applications. */

#ifndef NETMODULE
extern char     localhostname[];
extern struct CONNECT connblo[];
extern struct NET nets[];
extern struct NETCONF netconf[];
#endif

/* =========================================================================
   Optional SNMP MIB II definitions. */
struct ifgroup {
    int             ifNumber;
};
#ifdef MIB2
struct sysgroup {
    const char     *sysDescr;
    char           *sysContact;
    char           *sysName;
    char           *sysLocation;
};
struct IPgroup {
    int             ipForwarding;
    int             ipDefaultTTL;
    unsigned long   ipInReceives;
    unsigned long   ipInHdrErrors;
    unsigned long   ipInAddrErrors;
    unsigned long   ipForwDatagrams;
    unsigned long   ipInUnknownProtos;
    unsigned long   ipInDiscards;
    unsigned long   ipInDelivers;
    unsigned long   ipOutRequests;
    unsigned long   ipOutDiscards;
    unsigned long   ipOutNoRoutes;
    unsigned long   ipRoutingDiscards;
    int             ipReasmTimeout;
    unsigned long   ipReasmReqds;
    unsigned long   ipReasmOKs;
    unsigned long   ipReasmFails;
    unsigned long   ipFragOKs;
    unsigned long   ipFragFails;
    unsigned long   ipFragCreates;
};
struct ICMPgroup {
    unsigned long   icmpInMsgs;
    unsigned long   icmpInErrors;
    unsigned long   icmprx[19];
    unsigned long   icmpOutMsgs;
    unsigned long   icmpOutErrors;
    unsigned long   icmptx[19];
};
struct TCPgroup {
    int             tcpRtoMax;
    unsigned long   tcpActiveOpens;
    unsigned long   tcpPassiveOpens;
    unsigned long   tcpAttemptFails;
    unsigned long   tcpEstabResets;
    unsigned long   tcpCurrEstab;
    unsigned long   tcpInSegs;
    unsigned long   tcpOutSegs;
    unsigned long   tcpRetransSegs;
    unsigned long   tcpInErrs;
    unsigned long   tcpOutRsts;
};
struct UDPgroup {
    unsigned long   udpInDatagrams;
    unsigned long   udpNoPorts;
    unsigned long   udpInErrors;
    unsigned long   udpOutDatagrams;
};
#endif

