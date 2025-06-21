/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/net.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: net.h $
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
/* NET.H  Edition: 21  Last changed: 15-Sep-99,9:58:32  By: BRIANS */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    NET.H -- General Definitions for USNET

    Copyright (C) 1998 By
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

   15-SEP-1999  BTS  RMV: USSBUFALIGN -> local.h, MESSH_SZ -> support.h
   01-SEP-1999  BTS  Add: USSBUFALIGN for Nbuffer alignment
   04-AUG-1999  BSK  Add: ussChecksumAdjust() prototype
   16-JUN-1999  SCD  Chg: from CD21040 & CD21140 to DC21040 & DC21140
   15-JUN-1999  SCD  Add: PTABLE refs and Macros for en302 and en360
   23-APR-1999  NJW  Add: Added prototypes for ussHostXxxx() from route.c
   21-APR-1999  SCD  Chg: struct NETDATA field flags from char to short
   10-FEB-1999  RCA  Add: brought in driver protocol table declarations
   22-JAN-1999  RCA  Add: more ioctl constants
   19-JAN-1999  RCA  Chg: more efficient definition of ISCLASSD() macro
   29-DEC-1998  RCA  Add: ioctl function to PTABLE structure definition
   30-SEP-1998  BSK  Add: pppTimeout() prototype

 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>    /* needed for size_t type definition */

/* =========================================================================
   Protocol definitions.  To leave out a protocol, undefine the name.  To add
   a protocol, choose a name, write a protocol module (use the existing same
   level for model), and add to P_tab at start of module NET.C.

   Only the well-known protocols 1-6 need a number, used by the BSD socket
   interface.
*/
#define IP 1                    /* Internet Protocol: connectionless */
#define ICMP 2                  /* Internet Control Message Protocol */
#define ARP 3                   /* Address Resolution Protocol */
#define RARP 4                  /* Reverse Address Resolution Protocol */
#define UDP 5                   /* User Datagram Protocol: simple connections */
#define TCP 6                   /* Transport Control Protocol: full
                                   connections */
 /*#define LQRP*//* Link Quality Report Protocol */

/* =========================================================================
   Configuration definitions. */

#define LHDRSZ 14               /* size of link level header */
#define TOUT_READ 20000UL       /* default timeout ms for read */
#define REASSTOUT 60000UL       /* reassembly timeout ms */
#define ARPTOUT 60000UL         /* ARP address void in this time, 0 = never */
#define MAXWACK 4               /* maximum messages in the wait for ACK queue */
#define MAXWIND 4               /* window size in full buffers */
#define MAXOUTQLEN 4            /* maximum output queue length */


/* =========================================================================
   Macros and definitions to simplify coding. */

#define ASCII 0                 /* file mode: text */
#define IMAGE 1                 /* binary */

#define QUEUE_EMPTY(ptr, qname) (ptr->qname.mhead == ptr->qname.mtail)
#define QUEUE_FULL(ptr, qname) \
        (ptr->qname.mhead == ((ptr->qname.mtail + 1) & \
	    ((sizeof(ptr->qname.mp)/sizeof(ptr->qname.mp[0]))-1)))
#define QUEUE_IN(ptr, qname, mess) \
        ptr->qname.mp[ptr->qname.mtail] = mess; \
        ptr->qname.mtail = (ptr->qname.mtail + 1) & \
	    ((sizeof(ptr->qname.mp)/sizeof(ptr->qname.mp[0]))-1);
#define QUEUE_OUT(ptr, qname, mess) \
        mess = ptr->qname.mp[ptr->qname.mhead]; \
	ptr->qname.mhead = (ptr->qname.mhead + 1) & \
	    ((sizeof(ptr->qname.mp)/sizeof(ptr->qname.mp[0]))-1);
#define MESS_OUT(ptr, mess) \
    	mess = ptr->first; \
    	if ((ptr->first = mess->next) == 0) \
	    ptr->last = (MESS *)&ptr->first; \
	ptr->ninque--; \

#ifndef FARDEF
#define FAR
#define Nfarcpy memcpy
#else
#define FAR FARDEF
void            Nfarcpy(char FAR *, char FAR *, int);
#endif

/* =========================================================================
   Structures. */

/* external or Ethernet address format */
#define Eid_SZ 6
struct Eid {
    unsigned char   c[Eid_SZ];
};
/* internal or IP address format */
#define Iid_SZ 4
typedef union {
    unsigned char   c[Iid_SZ];
    unsigned short  s[Iid_SZ / 2];
    unsigned long   l;
}               Iid;
typedef union {
    unsigned char   b[4];
    unsigned long   l;
}               IPaddr;
#define EQIID(id1, id2) (*(long *)&id1 == *(long *)&id2)
#define Ihdr_SZ 20

/* IP address interpretation function for multicast */
/* note that IP address is stored in network byte order */
#ifdef LITTLE
#define USS_ISCLASSD(iidl) ((iidl & 0xe0UL) == 0xe0UL)
#else
#define USS_ISCLASSD(iidl) ((iidl & 0xe0000000UL) == 0xe0000000UL)
#endif

#define ussBroadcastIndex 255   /* special conf index for broadcast */
#define ussMulticastIndex 254   /* special conf index for multicast */

#define ussDfltMcNetno 0        /* Multicast default interface number */

/* Indices for the protoc array in the NET structure */
#define ussLinkIndex    0
#define ussDriverIndex  1
#define ussAdapterIndex 2

/* Commands for ioctl request argument */
enum ioctlreq { ussIdentE,
                ussDevBaudSetE,
                ussLinkIsUpE,
                ussLinkIsDownE,
                ussLinkBringUpE,
                ussLinkBringDownE,
                ussInterfaceBringUpE,
                ussInterfaceBringDownE,
                ussPPPUserIdSetE,
                ussPPPUserIdGetE,
                ussPPPPasswordSetE,
                ussPPPPasswordGetE,
                ussPPPDialEnableE,
                ussPPPDialDisableE,
                ussMcastGroupJoinE,
                ussMcastGroupLeaveE 
              };

struct MESSH {                  /* internal message header */
    struct MESSH   *next;       /* chain pointer */
    unsigned long   timems;     /* time stamp */
    unsigned long   target;     /* IP address of target */
    unsigned short  mlen;       /* message length */
    unsigned char   netno;      /* network number */
    char            offset;     /* offset to current level */
    unsigned char   confix;     /* network configuration index */
    char            conno;      /* connection number */
    short           id;         /* message identification */
};
typedef struct MESSH MESS;

#define bFUTURE 0x7777          /* buffer in future queue */
#define bFREE 0x7676            /* buffer free */
#define bALLOC 0x7575           /* buffer allocated */
#define bWACK 0x7474            /* buffer in wait-for-ack queue */
#define bRELEASE 0x7373         /* buffer to be released */
#define boRELEASED (char)0xff   /* offset for released buffer */
#define boTXDONE (char)0xfd     /* offset for transmit done */

#define PTABLE const struct P_tab   /* typedef caused trouble */
struct P_tab {                  /* protocol table, at end of each module */
    char            name[10];                     /* name of protocol */
    int             (*init) (int, char *);        /* initialize */
    void            (*shut) (int);                /* shut */
    int             (*screen) (MESS *);           /* screen */
    int             (*opeN) (int, int);           /* open */
    int             (*closE) (int);               /* close */
    MESS           *(*reaD) (int);                /* receive */
    int             (*writE) (int, MESS *);       /* send */
    int             (*ioctl) (void *, enum ioctlreq, void *, size_t);
                                                  /* i/o control */
    int             Eprotoc;                      /* external protocol num */
    unsigned char   hdrsiz;                       /* header size */
};

/* =========================================================================
   Link level protocols. */

extern PTABLE   ussEthernetTable;
extern PTABLE   ussARCNETTable;
extern PTABLE   ussSLIPTable;
extern PTABLE   ussPPPTable;

/* These #defines provide shorthand names for the above */
#define Ethernet &ussEthernetTable
#define ARCNET   &ussARCNETTable
#define SLIP     &ussSLIPTable
#define PPP      &ussPPPTable

/* =========================================================================
   Adapter level protocols. */

extern PTABLE ussPCITable;
extern PTABLE ussPCMCIA1Table;
extern PTABLE ussPCMCIA2Table;

#define PCI      &ussPCITable
#define PCMCIA1  &ussPCMCIA1Table
#define PCMCIA2  &ussPCMCIA2Table

/* =========================================================================
   Driver level protocols. */

extern PTABLE uss3C503Table;
extern PTABLE uss3C509Table;
extern PTABLE uss3C900Table;
extern PTABLE ussAM186ESTable;
extern PTABLE ussAM7990Table;
extern PTABLE ussAMD85C30Table;
extern PTABLE ussAMD961Table;
extern PTABLE ussCS8900Table;
extern PTABLE ussDC21040Table;
extern PTABLE ussDC21140Table;
extern PTABLE ussD2681Table;
extern PTABLE ussEN302Table;
extern PTABLE ussEN360Table;
extern PTABLE ussEXP16Table;
extern PTABLE ussH64180Table;
extern PTABLE ussHC11SCITable;
extern PTABLE ussHC16SCITable;
extern PTABLE ussI8250Table;
extern PTABLE ussI82557Table;
extern PTABLE ussI82595Table;
extern PTABLE ussI82596Table;
extern PTABLE ussMB86960Table;
extern PTABLE ussNDISTable;
extern PTABLE ussNE1000Table;
extern PTABLE ussNE2000Table;
extern PTABLE ussNE2100Table;
extern PTABLE ussNS16550Table;
extern PTABLE ussNS8390Table;
extern PTABLE ussODITable;
extern PTABLE ussPPC860Table;
extern PTABLE ussPPC860TTable;
extern PTABLE ussPPSMUARTTable;
extern PTABLE ussRTELAN32Table;
extern PTABLE ussS167Table;
extern PTABLE ussSH7604Table;
extern PTABLE ussSH7708Table;
extern PTABLE ussSM83C790Table;
extern PTABLE ussSMC91C92Table;
extern PTABLE ussWD8003Table;
extern PTABLE ussWrapTable;
extern PTABLE ussSWrapTable;
extern PTABLE ussSCI7709Table;
extern PTABLE ussSH7615Table;
extern PTABLE ussAM79C90Table;
extern PTABLE uss37C935Table;

/* More shorthand.  These names are used in netconf.c */
#define _3C503   &uss3C503Table
#define _3C509   &uss3C509Table
#define _3C900   &uss3C900Table
#define AM186    &ussAM186ESTable
#define AM186ES  &ussAM186ESTable
#define AM7990   &ussAM7990Table
#define AMD85C30 &ussAMD85C30Table
#define AMD961   &ussAMD961Table
#define CS8900   &ussCS8900Table
#define DC21040  &ussDC21040Table
#define DC21140  &ussDC21140Table
#define D2681    &ussD2681Table
#define EN302    &ussEN302Table 
#define EN360    &ussEN360Table 
#define EXP16    &ussEXP16Table
#define H64180   &ussH64180Table
#define HC11SCI  &ussHC11SCITable
#define HC16SCI  &ussHC16SCITable
#define I8250    &ussI8250Table
#define I82557   &ussI82557Table
#define I82595   &ussI82595Table
#define I82596   &ussI82596Table
#define MB86960  &ussMB86960Table
#define NDIS     &ussNDISTable
#define NE1000   &ussNE1000Table
#define NE2000   &ussNE2000Table
#define NE2100   &ussNE2100Table
#define NS16550  &ussNS16550Table
#define NS8390   &ussNS8390Table
#define ODI      &ussODITable
#define PPC860   &ussPPC860Table
#define PPC860T  &ussPPC860TTable
#define PPSMUART &ussPPSMUARTTable
#define RTELAN32 &ussRTELAN32Table
#define S167     &ussS167Table
#define SH7604   &ussSH7604Table
#define SH7708   &ussSH7708Table
#define SM83C790 &ussSM83C790Table
#define SMC91C92 &ussSMC91C92Table
#define WD8003   &ussWD8003Table
#define WRAP     &ussWrapTable
#define SWRAP    &ussSWrapTable
#define SCI7709  &ussSCI7709Table
#define SH7615   &ussSH7615Table
#define AM79C90  &ussAM79C90Table
#define S37C935  &uss37C935Table

struct FIFOQ8 {
    MESS           *mp[8];
    int             mhead,
                    mtail;
};
struct FIFOQ16 {
    MESS           *mp[16];
    int             mhead,
                    mtail;
};

#define HOSTNAME_LEN	32
#define DOMAIN_LEN		32

struct NETDATA {                /* network configuration table - ROM */
    char           name[HOSTNAME_LEN+DOMAIN_LEN];       /* host name */
    char           *pname;      /* port name */
/* the following are really type Iid but some compilers will not allow
   initial data for unions */
    struct {
        unsigned char   c[Iid_SZ];
    }                           /* address mask, 0 = host part */
                    Imask, Iaddr;   /* internal (Internet) address */
    struct Eid      Eaddr;      /* external (Ethernet) address */
    short           flags;      /* configuration flags */
    PTABLE         *lprotoc;    /* link level protocol */
    PTABLE         *dprotoc;    /* driver */
    PTABLE         *adapter;    /* adapter */
    char           *params;     /* pointer to setup parameters */
};
/* configuration flag bits */
#define NOTUSED    0x0001       /* configured out */
#define BOOTSERVER 0x0002
#define TIMESERVER 0x0004       /* will respond to ICMP time requests */
#define ROUTER     0x0008       /* host is a default router or gateway */
#define LOCALHOST  0x0010       /* this is me */
#define NODE       0x0020       /* host has at least 2 networks */
#define DIAL       0x0040       /* dial-up line */
#define DNSVER     0x0080       /* host is a DNS server */
#define PROXYARP   0x0100       /* proxy arp for this host */

/* flags to be used in Nopen */
#define IPADDR  0x0100          /* name is IP address (binary */


/* =========================================================================
   Error codes.  Use values -100, -101 etc. for private codes. */

#undef EBADF

#define NE_PARAM	-10         /* user parameter error */
#define EHOSTUNREACH	-11     /* host not reachable */
#define ETIMEDOUT	-12         /* timeout */
#define NE_HWERR	-13         /* hardware error */
#define ECONNABORTED	-14     /* protocol error */
#define ENOBUFS		-15         /* no buffer space */
#define EBADF		-16         /* connection block invalid */
#define EFAULT		-17         /* invalid pointer argument */
#define	EWOULDBLOCK	-18         /* operation would block */
#define	EMSGSIZE	-19         /* message too long */
#define	ENOPROTOOPT	-20         /* Protocol not available */
#define ussErrInval     -21         /* this ioctl request not implemented */

/* old codes */
#define NE_NOTCONF EHOSTUNREACH
#define NE_TIMEOUT ETIMEDOUT
#define NE_NOBUFS ENOBUFS
#define NE_PROTO ECONNABORTED

/* =========================================================================
   Function prototypes. */

void            Ninitbuf(int size, int count);
MESS           *Ngetbuf(void);
MESS           *NgetbufIR(void);
void            Nrelbuf(MESS * bufptr);
void            NrelbufIR(MESS * bufptr);
void            Ncntbuf(void);
int             BuildRoutes(void);
int             GetHostData(unsigned long iid, int addflag);
void            ussHostAcquire( int confix );
void            ussHostRelease( int confix );
int             ussHostUsed( int confix );
int             Ninit(void);
int             Nterm(void);
int             Portinit(char *port);
int             Portterm(char *port);
int             Nopen(char *to, char *protoc, int myport, int herport, int flags);
int             Nclose(int conno);
int             Nwrite(int conno, char *mess, int len);
int             Nread(int conno, char *mess, int len);
int             BOOTPnames(char *host, char *file);
void FAR       *BOOTP(char FAR * base, char *host, char *file);
void            BOOTPserv(void);
void            TFTPserv(void);
int             TFTPput(char *host, char *file, int mode);
int             TFTPget(char *host, char *file, int mode);
void            FTPserv(void);
int             FTPgetput(char *host, char *file, int mode);
#define FTPget(host, file, mode) FTPgetput(host, file, mode)
#define FTPput(host, file, mode) FTPgetput(host, file, mode+2)
int             getEid(unsigned long iid);
int             pppTimeout(int netno);
int             ussHostGroupJoin(Iid iid, int netno);
int             ussHostGroupLeave(Iid iid, int netno);
void            ussChecksumAdjust(unsigned char *,
                   unsigned char *, int,
                   unsigned char *, int);

void Ndisable( void );
void Nenable( void );


