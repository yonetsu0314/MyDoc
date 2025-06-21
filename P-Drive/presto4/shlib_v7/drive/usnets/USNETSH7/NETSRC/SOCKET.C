/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/socket.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: socket.c $
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
/* SOCKET.C  Edition: 23  Last changed: 21-Sep-99,14:32:50  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995, 1997, 1998 By
    United States Software Corporation
    7175 N.W. Evergreen Parkway, Suite 100
    Hillsboro, Oregon 97124
    http://www.ussw.com
    (503) 844-6614

    SOCKET.C -- BSD Socket Interface Routines for USNET

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

 30-AUG-1999  BSK  Add: SO_BINDTODEVICE
                   Add: Multicast socket support
 04-AUG-1999  BSK  Fix: Concurrent read and write on a single socket
 22-APR-1999  NJW  Protect used entries in host table.
 31-DEC-1998  RCA  Updated PTABLE names
 11-NOV-1998  BSK  Fix: Corrected non-blocking behavior in connect().

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include <stdarg.h>
#include "net.h"
#include "local.h"
#include "support.h"
#include "socket.h"

/* BSD continuation of connection block */
struct SOCBLOCK {
    int error;                  /* last error number */
    unsigned short sndbuf;      /* send buffer size */
    unsigned short rcvbuf;      /* receive buffer size */
    int sockopt;                /* socket options */
    MESS *istreamb;             /* input stream I/O buffer base */
    int istreamc;               /* I/O character counter */
    char *istreamp;             /* I/O character pointer */
    int doffsetsav;             /* saved data offset */
    unsigned int maxdatsav;     /* saved maximum user data */
    struct linger linger;       /* SO_LINGER data */
};

#define soDEBUG         0x0001  /* socket option bits */
#define soREUSEADDR     0x0002
#define soDONTROUTE     0x0008
#define soBROADCAST     0x0010
#define soOOBINLINE     0x0040
#define soLISTEN        0x0080
#define soSHUTRECV      0x0100
#define soSHUTSEND      0x0200
#define soNODELAY       0x0400

extern struct NETCONF netconf[];
extern struct NET nets[];
extern PTABLE *const P_tab[];
extern PTABLE ussIPTable;
#ifdef TCP
extern PTABLE ussTCPTable;
#endif
#ifdef UDP
extern PTABLE ussUDPTable;
#endif
#ifdef DNS
extern Iid DNSiid[2];
#endif

struct SOCBLOCK socblock[NCONNS];

/* BSD call to reserve a connection block.
   We reserve a connection block.  We store the following protocol stack:
        SOCK_STREAM:  TCP/IP
        SOCK_DGRAM:   UDP/IP or ICMP/IP
        other:        none, meaning raw
*/
int socket(int domain, int type, int protocol)
{
    int i1, conno;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

    if (domain == PF_UNSPEC)
        domain = PF_INET;
    if (domain != PF_INET) {
        errno = EPROTONOSUPPORT;
        return -1;
    }
    BLOCKPREE();
    for (conno = 0; conno < NCONNS; conno++) {
        conp = &connblo[conno];
        if (!conp->blockstat) {
            memset((void *) conp, 0, sizeof(struct CONNECT));
            conp->blockstat = (char) 0x81;
            break;
        }
    }
    RESUMEPREE();
    if (conno == NCONNS) {
        errno = ENOBUFS;
        return -1;
    }
    socp = &socblock[conno];
    memset((void *) socp, 0, sizeof(struct SOCBLOCK));
    if (type == SOCK_STREAM) {
#ifdef TCP
        if (protocol == 0)
            protocol = TCP;
        if (protocol != TCP) {
            errno = EPROTONOSUPPORT;
            goto err9;
        }
        conp->txstat |= S_STRM;
        conp->protoc[0] = &ussTCPTable, conp->protoc[1] = &ussIPTable;
#else
        errno = EPROTONOSUPPORT;
        goto err9;
#endif
    } else if (type == SOCK_DGRAM) {
        conp->txstat |= S_NOCON;
#ifdef UDP
        if (protocol == 0)
            protocol = UDP;
        if (protocol != UDP)
#endif
#ifdef ICMP
            if (protocol != ICMP)
#endif
            {
                errno = EPROTONOSUPPORT;
                goto err9;
            }
        conp->protoc[0] = P_tab[protocol], conp->protoc[1] = &ussIPTable;
    }
    conp->doffset = MESSH_SZ + LHDRSZ;
    for (i1 = 0; conp->protoc[i1]; i1++)
        conp->doffset += conp->protoc[i1]->hdrsiz;
    conp->maxdat = nets[conp->netno].maxblo - conp->doffset + MESSH_SZ +
        LHDRSZ;
    socp->doffsetsav = conp->doffset;
    socp->maxdatsav = conp->maxdat;
    socp->sndbuf = socp->rcvbuf = conp->maxdat;
    conp->heriid.l = conp->rxtout = 0xffffffff;
    conp->confix = 255;
    conp->blockstat = 1;
    return conno;
err9:
    conp->blockstat = 0;
    return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  connect()      Initiates a connection on a socket
**
**  int connect( int s, struct sockaddr * name, int namelen );
**
**  PARAMETERS:
**     (in)     s       Socket ID number
**     (in)     name    Remote host definition
**     (in)     namelen Size of remote host's definition structure
**
**  RETURNS:
**     0        Remote connect was established
**    -1        Error occured.  Check sockets errno.
**
**  DESCRIPTION:
**     BSD routine to do an open for a connection.  The connection block
**     has already been reserved with the "socket" call, and the protocol
**     path (above the data link level) has been stored into it.  The
**     connect() call gives the port number and the Internet address.
**
**  USAGE/COMMENTS:
**     A call to socket() must be made first.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  23-APR-1999  NJW  Added header; added acquisition of host entry
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int connect(int s, struct sockaddr * name, int namelen)
{
    int i1, i2, confix;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;
    Iid iid;
    unsigned short us1;

    i1 = 0;
   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
    if (namelen == 0 || name == 0) {
        socp->error = errno = EFAULT;
        goto err1;
    }
    iid = *(Iid *) & ((struct sockaddr_in *) name)->sin_addr;
    if ((confix = GetHostData(iid.l, 1)) < 0) {
        socp->error = errno = EHOSTUNREACH;
        goto err1;
    }
    conp->rxstat = 0;
    conp->netno = netconf[confix].netno;
    conp->confix = confix;
    conp->heriid = iid;
    if (conp->myport == 0)
        us1 = Nportno(), conp->myport = NC2(us1);
    conp->herport = ((struct sockaddr_in *) name)->sin_port;

   /* call protocol stack open, needed for TCP and PPP and so on */
    i2 = conp->protoc[0]->opeN(s, 0);
    if (i2 > 0) {
        ussHostAcquire(conp->confix);  /* Acquire entry from host table */
        return 0;
    }
    if (i2 == 0) {
        i2 = EINPROGRESS;
        i1 = 8;
    }
    socp->error = errno = i2;
err1:
    conp->state = i1;
    return -1;
errbs:
    errno = EBADF;
    return -1;
}

/* BSD routine to specify the local side of a connection: local port number
   and local Internet address.  Routine socket has been called.  Bind is not
   needed (but can be used) before an active open, that is a connect().

   The address given MUST be local, though this is not very clear from some
   of the BSD documentation. */
int bind(int s, struct sockaddr * name, int namelen)
{
    int i1, netno;
    struct NET *netp;
    struct CONNECT *conp, *conp2;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
    if (namelen < sizeof(struct sockaddr_in) || name == 0) {
        socp->error = errno = EFAULT;
        return -1;
    }
    conp->myport = ((struct sockaddr_in *) name)->sin_port;
    netno = -1;
#ifdef MIB2
    conp->offerediid = *((Iid *) & ((struct sockaddr_in *) name)->sin_addr);
#endif
    if (((struct sockaddr_in *) name)->sin_addr.s_addr == 0)
        goto lab3;
    for (netno = 0; netno < NNETS; netno++) {
        netp = &nets[netno];
        if (netp->netstat == 0)
            continue;
        if (!EQIID(netconf[netp->confix].Iaddr,
                   *(Iid *) & ((struct sockaddr_in *) name)->sin_addr))
            continue;
        goto lab3;
    }
    socp->error = errno = EADDRNOTAVAIL;
    return -1;
lab3:
   /* check for duplicate local address unless REUSEADDR set */
    if ((socp->sockopt & soREUSEADDR) == 0) {
        for (i1 = 0; i1 < NCONNS; i1++) {
            conp2 = &connblo[i1];
            if (i1 == s)
                continue;
            if (conp2->blockstat == 0)
                continue;
            if (conp2->myport != conp->myport)
                continue;
            if (conp2->netno != netno)
                continue;
            if (conp2->protoc[0] != conp->protoc[0])
                continue;
            socp->error = errno = EADDRINUSE;
            return -1;
        }
    }
    return 0;
errbs:
    errno = EBADF;
    return -1;
}

/* BSD routine for specifying that passive open will be done. For TCP only.
   Make status of connection LISTEN, unless already done.
*/
int listen(int s, int backlog)
{
    struct CONNECT *conp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socblock[s].sockopt |= soLISTEN;
    if (conp->state == 0)
        conp->state = 16;
    conp->backlog = backlog;
    return 0;
errbs:
    errno = EBADF;
    return -1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  accept()       Accepts a passive connection on a socket
**
**  int accept( int s, struct sockaddr * name, int namelen );
**
**  PARAMETERS:
**     (in)     s       Socket ID number
**     (in)     name    Remote host definition
**     (in)     namelen Size of remote host's definition structure
**
**  RETURNS:
**    >=0       Socket ID number for established connection
**    -1        Error occured.  Check sockets errno.
**
**  DESCRIPTION:
**     BSD routine for passive open.  Returns when it has received an
**     active open request, with a new socket.  In other words, this
**     acts as a "socket fork" function.  For TCP only.
**
**  USAGE/COMMENTS:
**     We wait for a client, get a new connection block, copy the
**     orginal block to this.  Then we return the original block to LISTEN.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  23-APR-1999  NJW  Added header; added acquisition of host entry
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int accept(int s, struct sockaddr * name, int *namelen)
{
    int i2, conno;
    struct CONNECT *conp, *conp2;
    struct sockaddr_in sa;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
   /* the connection may not be established yet, wait if necessary */
    WAITFOR(conp->next, SIG_CC(s), conp->rxtout, i2);
    if (i2 != 0) {
        socp->error = errno = EWOULDBLOCK;
        return -1;
    }
   /* remove first connection from incoming connection queue */
    BLOCKPREE();
    conp2 = conp->next;
    conp->next = conp2->next;
    conp->icqcur--;
    RESUMEPREE();
    conno = conp2 - &connblo[0];
    ussHostAcquire(connblo[conno].confix);  /* Acquire entry from host tbl */
   /* clear new socket block */
    memset((void *) &socblock[conno], 0, sizeof(struct SOCBLOCK));
   /* fill in address structure */
    sa.sin_port = conp2->herport;
    sa.sin_addr = *(struct in_addr *) & conp2->realiid;
    sa.sin_family = PF_INET;
    if (name && namelen) {
        i2 = *namelen;
        if (i2 > sizeof(sa))
            i2 = sizeof(sa);
        memcpy((char *) name, (char *) &sa, i2);
        *namelen = sizeof(struct sockaddr_in);
    }
    return conno;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** closesocket()    Closes a connection on a socket.
**
** int closesocket(int s);
**
** PARAMETERS:
**  (in)int s                   Socket ID number
**
** RETURNS:
**  0                           Close succedded.
** -1                           Error occured.  Check sockets errno.
**
** DESCRIPTION:
**  Get rid of the socket.  Flushes streams, calls low-level close,
**  releases buffers.   The UNIX close does not report any I/O errors,
**  and does not wait at all unless LINGER is used.
**
** USAGE/COMMENTS:
**  This routine has been renamed to reduce conflicts with UNIX
**  BSD close().  UNIX close() can be used for file descriptors
**  this one can not.
**
** * * * * * *
**
** MODIFICATION HISTORY:
**
**  20-SEP-1999  BSK  Fix: Linger with time 0 on unsynchronized connection
**  02-AUG-1999  BSK  Chg: Removed removal of S_NOWA from txstat
**  23-APR-1999  NJW  Added header; added release of host entry
**
** * * * * * *
*/
int closesocket(int s)
{
    int stat;
    MESS *mess, *nmess;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    conp->blockstat = 2;
   /* release stream input, input queue */
    socp = &socblock[s];
    if (socp->istreamc)
        Nrelbuf(socp->istreamb);
    for (mess = conp->first; mess;) {
        nmess = mess->next;
        Nrelbuf(mess);
        mess = nmess;
    }
    WAITNOMORE(SIG_RC(s));      /* wake up any suspended read */
   /* LINGER with time zero discards all queued data and sends a RST. */
    if (socp->linger.l_onoff && socp->linger.l_linger == 0 &&
        (conp->txstat & S_STRM))
    {
        if (conp->ostreamb)
            Nrelbuf(conp->ostreamb);
       /* If the state is not unsynchronized, send RST */
        if (conp->state != 8 && conp->state != 9 && conp->state != 16)
            if ((mess = Ngetbuf()) != 0) {
               /* Set the TCP reset flag (RST in tcp.c) */
                mess->mlen = 0x8000 | 4;
                mess->netno = conp->netno;
                mess->confix = conp->confix;
                mess->id = bRELEASE;
                if (conp->protoc[0]->writE(s, mess))
                    Nrelbuf(mess);
            }
        conp->state = 5;
        conp->blockstat = 4;
        return 0;
    }
   /* flush stream output */
    if (conp->ostreamb) {
        if (conp->protoc[0]->writE(s, conp->ostreamb))
            Nrelbuf(conp->ostreamb);
        conp->ostreamb = 0;
    }
   /* BSD sockets expect normally a no-wait close. */
    if (socp->linger.l_onoff == 0)
        conp->txstat |= S_NOWA;
   /* call protocol stack close, return bits: */
   /*              0 = do not free connection block */
   /*              1 = protocol error has occurred */
    stat = conp->protoc[0]->closE(s);
    if ((stat & 1) == 0) {
        conp->blockstat = 0;
        ussHostRelease(conp->confix);  /* Release entry in host table */
    }
    return 0;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** getsockname()   put the host socket "name" into the name structure
**
** int getsockname(int s, struct sockaddr *name, int *namelen);
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (out) name                 Socket name structure pointer
**   (in) namelen               sizeof() socket name structure
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   This function will place the "name" of the host's socket into the
**   sockaddr structure passed as a parameter.
**
** USAGE/COMMENTS:
**   This routine is not very useful because it refers to the local host.
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int getsockname(int s, struct sockaddr * name, int *namelen)
{
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];
    if (name == 0 || namelen == 0) {
        socp->error = errno = EFAULT;
        return -1;
    }
    ((struct sockaddr_in *) name)->sin_port = conp->myport;
    ((struct sockaddr_in *) name)->sin_addr =
        *(struct in_addr *) & (netconf[nets[conp->netno].confix].Iaddr);
    ((struct sockaddr_in *) name)->sin_family = PF_INET;
    *namelen = sizeof(struct sockaddr_in);
    return 0;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** getpeername()   put the peer socket "name" into the name structure
**
** int getpeername(int s, struct sockaddr *name, int *namelen);
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (out) name                 Socket name structure pointer
**   (in) namelen               sizeof() socket name structure
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   This function will place the "name" of the peer's socket into the
**   sockaddr structure passed as a parameter.
**
** USAGE/COMMENTS:
**   This routine is not very useful because it refers to the local host.
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int getpeername(int s, struct sockaddr * peer, int *addrlen)
{
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];
    if (peer == 0 || addrlen == 0) {
        socp->error = errno = EFAULT;
        return -1;
    }
    ((struct sockaddr_in *) peer)->sin_port = conp->herport;
    ((struct sockaddr_in *) peer)->sin_addr.s_addr = conp->realiid.l;
    *addrlen = sizeof(struct sockaddr_in);
    return 0;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** getsockopt()     Get a socket option
**
** int getsockopt(int s, int level, int optname, char *optval, int *optlen)
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (in) level                 Option level
**   (in) optname               Option name
**   (out) optval               Value of option name at the level
**   (in/out) optlen            Potential and resultant length returned value
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   This function will place the value of a particular socket option at the
**   location of the optval pointer value assuming that the value of optlen
**   is larger than the length of the option value.  The optlen value will
**   be filled in with the true option length.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   26-AUG-1999  BSK  Add: Multicast support
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int getsockopt(int s, int level, int optname, char *optval, int *optlen)
{
    int actlen;
    unsigned int ui1;
    char *actval;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];
    if (level == 0)
        level = SOL_SOCKET;
    ui1 = 0;
    actlen = sizeof(int);
    actval = (char *) &ui1;

   /* handle IP level options */
    if (level == IPPROTO_IP) {
#ifdef IPOPTIONS
        if (optname == IP_OPTIONS) {
            if (optval == 0 || optlen == 0 || *optlen < conp->IPOtxlen)
                goto badarg;
            memcpy(optval, conp->IPOtxopt, conp->IPOtxlen);
            *optlen = conp->IPOtxlen;
            return 0;
        }
#endif
#if USS_IP_MC_LEVEL
        if (optname == IP_MULTICAST_IF) {
            actval = netconf[nets[conp->netno].confix].Iaddr.c;
            actlen = sizeof(struct in_addr);
            goto ret1;
        }
#endif
    }

   /* handle TCP level options */
    if (level == IPPROTO_TCP) {
        if (optname == TCP_NODELAY) {
            ui1 = socp->sockopt & soNODELAY;
            goto ret1;
        }
        if (optname == TCP_MAXSEG) {
            ui1 = conp->maxdat;
            goto ret1;
        }
    }

   /* handle socket level options */
    if (level != (int) SOL_SOCKET)
        goto errop;
    switch (optname) {
    case SO_DEBUG:              /* debugging */
        ui1 = socp->sockopt & soDEBUG;
        goto ret1;
    case SO_REUSEADDR:          /* local address reuse */
        ui1 = socp->sockopt & soREUSEADDR;
        goto ret1;
    case SO_KEEPALIVE:          /* keep connections alive */
        ui1 = conp->txstat & S_KEEPA;
        goto ret1;
    case SO_DONTROUTE:          /* just use interface addresses */
        ui1 = socp->sockopt & soDONTROUTE;
        goto ret1;
    case SO_BROADCAST:          /* permit sending of broadcast msgs */
        ui1 = socp->sockopt & soBROADCAST;
        goto ret1;
    case SO_LINGER:             /* linger on close if data present */
        actlen = sizeof(struct linger);
        actval = (char *) &socp->linger;
        goto ret1;
    case SO_OOBINLINE:          /* leave received OOB data in line */
        ui1 = socp->sockopt & soOOBINLINE;
        goto ret1;
    case SO_SNDBUF:             /* send buffer size */
        ui1 = socp->sndbuf;
        goto ret1;
    case SO_RCVBUF:             /* receive buffer size */
        ui1 = socp->rcvbuf;
        goto ret1;
    case SO_ERROR:              /* get error status and clear */
        ui1 = socp->error;
        socp->error = 0;
        goto ret1;
    case SO_TYPE:               /* get socket type */
        if (conp->txstat & S_STRM)
            ui1 = SOCK_STREAM;
        else if (conp->protoc[0] != 0)
            ui1 = SOCK_DGRAM;
        else
            ui1 = SOCK_RAW;
ret1:
        if (optval == 0 || optlen == 0 || *optlen < actlen)
            goto badarg;
        memcpy(optval, actval, actlen);
        *optlen = actlen;
        return 0;
    }
errop:
    socp->error = errno = ENOPROTOOPT;
    return -1;
errbs:
    errno = EBADF;
    return -1;
badarg:
    socp->error = errno = EFAULT;
    return -1;
}

/*
** * * * * * *
** setsockopt()     BSD routine to set the value of a requested option
**
** int setsockopt(int s, int level, int optname, char *optval, int optlen)
**
** PARAMETERS:
**   (in) int s                 A socket
**   (in) int level             Option level
**   (in) optname               Option name
**   (in) optval                Option value
**   (in) optlen                Option length
**
** RETURNS:
**  -1                          error occured
**   0                          Success
**
** DESCRIPTION:
**   This function will set the value of the requested BSD option.
**   If there is an error, it will return -1 and set errno.
**      EBADF -- Socket invalid
**      ENOPROTOOPT -- No support for the class of option
**      EFAULT --
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   26-AUG-1999  BSK  Add: SO_BINDTODEVICE
**   26-JUL-1999  BSK  Add: Multicast support
** * * * * * *
*/
int setsockopt(int s, int level, int optname, char *optval, int optlen)
{
    unsigned int ui1;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
    if (level == 0)
        level = SOL_SOCKET;

   /* handle IP level here */
    if (level == IPPROTO_IP) {
        if (optval == 0)
            goto badarg;
#ifdef IPOPTIONS
        if (optname == IP_OPTIONS) {
            if (optlen > sizeof(conp->IPOtxopt))
                goto badarg;
            memset(conp->IPOtxopt, 0, sizeof(conp->IPOtxopt));
            memcpy(conp->IPOtxopt, optval, optlen);
            optlen = (optlen + 3) & ~3;
            conp->IPOtxlen = optlen;
            conp->doffset = socp->doffsetsav + optlen;
            conp->maxdat = socp->maxdatsav - optlen;
            return 0;
        }
#endif
#if USS_IP_MC_LEVEL
        if (optname == IP_MULTICAST_IF) {
            if (optlen != sizeof(struct in_addr))
                goto badarg;
            if (((struct in_addr *)optval)->s_addr == 0)
                conp->netno = ussDfltMcNetno;   /* Use default interface */
            else {
                for (ui1 = 0; ui1 < NCONFIGS; ui1++)
                    if (netconf[ui1].flags & LOCALHOST &&
                        netconf[ui1].Iaddr.l ==
                        ((struct in_addr *)optval)->s_addr)
                    {
                        break;
                    }
                if (ui1 == NCONFIGS)
                    goto badarg;
                conp->netno = netconf[ui1].netno;
            }
            return 0;
        }
        else if (optname == IP_ADD_MEMBERSHIP) {
            if (optlen != sizeof(struct ip_mreq))
                goto badarg;
            if (((struct ip_mreq *)optval)->imr_interface.s_addr == 0)
                conp->netno = ussDfltMcNetno;   /* Use default interface */
            else {
                for (ui1 = 0; ui1 < NCONFIGS; ui1++)
                    if (netconf[ui1].flags & LOCALHOST &&
                        netconf[ui1].Iaddr.l ==
                            ((struct ip_mreq *)optval)->imr_interface.s_addr)
                    {
                        break;
                    }
                if (ui1 == NCONFIGS)
                    goto badarg;
                conp->netno = netconf[ui1].netno;
            }
            return ussHostGroupJoin(
                *(Iid *)&((struct ip_mreq *)optval)->imr_multiaddr.s_addr,
                conp->netno);
        }
        else if (optname == IP_DROP_MEMBERSHIP) {
            if (optlen != sizeof(struct ip_mreq))
                goto badarg;
            return ussHostGroupLeave(
                *(Iid *)&((struct ip_mreq *)optval)->imr_multiaddr.s_addr,
                conp->netno);
        }
#endif
    }

   /* handle TCP level here */
    if (level == IPPROTO_TCP) {
        if (optname == TCP_NODELAY) {
            socp->sockopt ^= soNODELAY;
            return 0;
        }
    }
   /* handle socket level here */
    if (level != (int) SOL_SOCKET)
        goto errop;
    switch (optname) {
    case SO_DEBUG:              /* toggle debugging */
        socp->sockopt ^= soDEBUG;
        return 0;
    case SO_REUSEADDR:          /* toggle local address reuse */
        socp->sockopt ^= soREUSEADDR;
        return 0;
    case SO_KEEPALIVE:          /* keep connections alive */
        conp->txstat ^= S_KEEPA;
        return 0;
    case SO_DONTROUTE:          /* just use interface addresses */
        socp->sockopt ^= soDONTROUTE;
        return 0;
    case SO_BINDTODEVICE:       /* Bind a socket to a device */
        if (optval == 0)
            goto badarg;
        for (ui1 = 0; ui1 < NCONFIGS; ui1++)
            if (strcmp(netconf[ui1].pname, optval) == 0)
                if (strcmp(localhostname, netconf[ui1].name) == 0)
                    break;
        conp->netno = (ui1 == NCONFIGS) ? 0 : netconf[ui1].netno;
        return 0;
    case SO_BROADCAST:          /* permit sending of broadcast msgs */
        socp->sockopt ^= soBROADCAST;
        return 0;
    case SO_LINGER:             /* linger on close if data present */
        if (optval == 0 || optlen != sizeof(struct linger))
            goto badarg;
        memcpy((char *) &socp->linger, optval, sizeof(struct linger));
        return 0;
    case SO_OOBINLINE:          /* leave received OOB data in line */
        socp->sockopt ^= soOOBINLINE;
        return 0;
    case SO_SNDBUF:             /* send buffer size */
    case SO_RCVBUF:             /* receive buffer size */
        if (optval == 0 || optlen != sizeof(int))
            goto badarg;
        ui1 = *(unsigned int *) optval;
        if (optname == SO_SNDBUF)
            socp->sndbuf = ui1;
        else
            socp->rcvbuf = ui1;
        return 0;
    }
errop:
    socp->error = errno = ENOPROTOOPT;
    return -1;
errbs:
    errno = EBADF;
    return -1;
badarg:
    socp->error = errno = EFAULT;
    return -1;
}

/*
** * * * * * *
** recv()   Recieve a message on an open connection
**
** int recv(int s, char *buf, int len, int flags)
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (out) buf                  Pointer to free memory to put incoming data
**   (in) len                   Length of free memory area
**   (in) flags                 Flags to manipulate operations of recv()
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   If no data is available, normal (not non-blocking) recv() will block.
**   If there is data, no blocking will be done.  Non-stream (UDP etc.)
**   returns one whole message.  Stream (TCP) returns 1-len bytes where len
**   is the requested amount.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int recv(int s, char *buf, int len, int flags)
{
    int i1, tlen, lalen;
    char *laptr;
    MESS *mp;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];


   /* reading out of bound data not in-line, we just have one character, in
    ** the connection block */
    if (flags & MSG_OOB && (socp->sockopt & SO_OOBINLINE) == 0) {
        if (conp->urgflag == 0)
            goto ret8;
        *buf = conp->urgdata;
        if ((flags & MSG_PEEK) == 0)
            conp->urgflag = 0;
        return 1;
    }
    for (tlen = 0; tlen < len;) {
        if (!socp->istreamc) {  /* read if nothing in storage */
            socp->istreamb = mp = conp->protoc[0]->reaD(s);
            if (mp == 0) {
                if (tlen)
                    break;
                if (conp->rxstat & S_FATAL)
                    goto ret9;
                if (conp->rxstat & S_EOF)
                    return 0;
                YIELD();
                socp->error = errno =
                    conp->txstat & S_NOWA ? EWOULDBLOCK : ETIMEDOUT;
                return -1;
            }
            socp->istreamc = mp->mlen - mp->offset;
            if (socp->istreamc < 0)
                goto err1;
            socp->istreamp = (char *) mp + mp->offset;
           /* check for out of band data */
            if (conp->tcpflags & S_URG) {
               /* if not in-line, remove from stream */
                if ((socp->sockopt & soOOBINLINE) == 0) {
                    socp->istreamc -= conp->urgcnt;
                    socp->istreamp += conp->urgcnt;
                    conp->tcpflags = 0;
                }
            }
        }
       /* copy the data (all or just the requested amount) from the buffer */
        i1 = socp->istreamc > len - tlen ? len - tlen : socp->istreamc;
        if (flags & MSG_OOB) {
            if ((conp->tcpflags & S_URG) == 0)
                goto ret8;
            if (i1 >= conp->urgcnt) {
                conp->tcpflags = 0;
                i1 = conp->urgcnt;
            }
        } else {
            if ((conp->tcpflags & S_URG) != 0)
                goto ret8;
        }
        Nmemcpy(buf, socp->istreamp, i1);

        buf += i1;
        tlen += i1;

       /* When MSG_PEEK is used for a byte stream connection, read through
        ** the rest of the messages queued for the connection, but don't use
        ** reaD(), as this would consume the messages. */
        if (flags & MSG_PEEK) {
            if (conp->txstat & S_STRM) {        /* Check if byte stream */
                mp = conp->first;       /* Point to first message */

               /* Only seach messages if exists and up to requested
                ** peek len. */
                while ((tlen < len) && mp) {
                    laptr = (char *) mp + mp->offset;   /* skip ether hdr */
                    laptr += ((*(laptr + 12) >> 4) << 2);       /* TCP hdr */
                    lalen = mp->mlen - (laptr - (char *) mp);   /* app data */
                    i1 = lalen > len - tlen ? len - tlen : lalen;
                    Nmemcpy(buf, laptr, i1);    /* copy data to buf */
                    buf += i1;  /* skip buf ahead */
                    tlen += i1; /* length peeked so far */
                    mp = mp->next;      /* next message */
                }
            }                   /* is S_STRM ? */
            return tlen;
        }
        socp->istreamp += i1;
       /* if buffer all used up release it */
        if ((socp->istreamc -= i1) == 0) {
            mp = socp->istreamb;
            Nrelbuf(mp);
        }
        if ((conp->txstat & S_STRM) == 0)       /* not stream: take 1 */
            break;
        if (conp->first == 0)   /* no next packet: return */
            break;
    }
   /* if stream, or if whole message used up, we are fine */
    if ((conp->txstat & S_STRM) || !socp->istreamc)
        return tlen;
err1:
    socp->istreamc = 0;
    socp->istreamb->id = bALLOC;
    Nrelbuf(socp->istreamb);
ret9:
    socp->error = errno = ECONNABORTED;
    return -1;
ret8:
    socp->error = errno = EOPNOTSUPP;
    return -1;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** recvfrom()   Recieve a message on an open UDP "connection"
**
** int recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from,
**     int *fromlen)
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (out) buf                  Pointer to free memory to put incoming data
**   (in) len                   Length of free memory area
**   (in) flags                 Flags to manipulate operations of recv()
**   (in) from                  Remote host that host received data from
**   (in) fromlen               sizeof() remote host structure
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   Get a message from the queued messages on the connection.  Store the
**   remote address in the sockaddr structure.
**
** USAGE/COMMENTS:
**   Calls the recv() function.
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int recvfrom(int s, char *buf, int len, int flags, struct sockaddr *from,
    int *fromlen)
{
    int i1;
    struct CONNECT *conp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
   /* call regular receive to get the message */
    if ((i1 = recv(s, buf, len, flags)) < 0)
        return -1;
    if (from != 0) {
        ((struct sockaddr_in *) from)->sin_port = conp->herport;
        ((struct sockaddr_in *) from)->sin_addr.s_addr = conp->realiid.l;
        *fromlen = sizeof(struct sockaddr_in);
    }
    return i1;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** recvmsg()    Recieve messages on an open UDP "connection"
**
** int recvmsg(int s, struct msghdr *msg, int flags)
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (in/out) msghdr            Message header
**   (in) flags                 Flags to manipulate operations of recv()
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   Get messages from the queued data on the connection.  Use the msghdr
**   structure to define the amount of data to receive.  Gets multiple
**   messages in a single call.
**
** USAGE/COMMENTS:
**   Calls the recv() function.
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int recvmsg(int s, struct msghdr * msg, int flags)
{
    int i1, msgix, tbytes;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;
    struct iovec *iovp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];
    if (msg->msg_name == 0)
        goto lab1;
    conp->herport = ((struct sockaddr_in *) msg->msg_name)->sin_port;
    conp->heriid.l = ((struct sockaddr_in *) msg->msg_name)->sin_addr.s_addr;
lab1:
    for (tbytes = msgix = 0; msgix < msg->msg_iovlen; msgix++) {
        iovp = &msg->msg_iov[msgix];
        i1 = recv(s, iovp->iov_base, iovp->iov_len, flags);
        if (i1 < 0)
            goto err1;
        tbytes += i1;
    }
    ((struct sockaddr_in *) msg->msg_name)->sin_port = conp->herport;
    ((struct sockaddr_in *) msg->msg_name)->sin_addr.s_addr =
        conp->realiid.l;
    msg->msg_namelen = sizeof(struct sockaddr_in);
    return tbytes;
err1:
    socp->error = errno = ECONNABORTED;
    return -1;
errbs:
    errno = EBADF;
    return -1;
}

/* BSD routine to send a message on an open connection, with an option
   parameter.

   First some checks.  Calculates data offset in buffer.  If not
   stream rejects call if record longer than allowed.  Allocates buffer if
   one isn't already allocated.  Moves data into buffer.  Calls low-level
   write if buffer becomes full.  Repeats until all data is gone.
*/
int send(int s, char *buf, int len, int flags)
{
    int i1, stat, tlen;
    unsigned int maxlen;
    unsigned long ul1;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;
    struct NET *netp;
    MESS *mp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
   /* reject if shutdown done */
    if (socp->sockopt & soSHUTSEND) {
        socp->error = errno = ESHUTDOWN;
        return -1;
    }
    if (conp->rxstat & S_FATAL) {
        socp->error = errno = ECONNABORTED;
        return -1;
    }
    tlen = 0;
    conp->blockstat = 0x21;
    while (tlen < len) {
        if (conp->txstat & S_STRM) {
            maxlen = conp->maxdat + conp->doffset;
            if (flags & MSG_OOB)
                conp->txstat |= S_URG;
            if (len - tlen <= conp->maxdat)
                conp->txstat |= S_PSH;
        } else {
            if (len + conp->doffset > MAXBUF) {
                socp->error = errno = EMSGSIZE;
                goto erret;
            }
            maxlen = len + conp->doffset;
        }
        if ((mp = conp->ostreamb) == 0) {
            if ((mp = conp->ostreamb = Ngetbuf()) == 0) {
                socp->error = errno = ENOBUFS;
                goto erret;
            }
            mp->netno = conp->netno;
            mp->target = conp->heriid.l;
            mp->mlen = mp->offset = conp->doffset;
        }
        i1 = maxlen - mp->mlen > len - tlen ? len - tlen :
            maxlen - mp->mlen;
        Nmemcpy((char *) mp + mp->mlen, buf + tlen, i1);
        tlen += i1;
        mp->mlen += i1;
        if (mp->mlen >= maxlen || conp->nwacks == 0 || (flags & MSG_OOB) ||
            (socp->sockopt & soNODELAY)) {
            netp = &nets[conp->netno];
            ul1 = conp->txstat & S_NOWA ? 0 : netp->tout;
            WAITFOR(((netp->depart.mtail - netp->depart.mhead) & 15) <
                    MAXOUTQLEN, SIG_WN(conp->netno), ul1, stat);
            if (stat)
                stat = ETIMEDOUT;
            else
                stat = conp->protoc[0]->writE(s, mp);
            if (stat == ETIMEDOUT && (conp->txstat & S_NOWA)) {
                mp->offset = conp->doffset;
                socp->error = errno = EWOULDBLOCK;
                stat = tlen;
                break;
            }
            if (stat)
                Nrelbuf(mp);
            conp->ostreamb = 0;
            if (stat < 0) {
                socp->error = errno = stat;
                stat = -1;
                break;
            }
        }
        stat = len;
    }
#if MT != 2
    YIELD();
#endif
    conp->blockstat = 1;
    return stat;
erret:
    conp->blockstat = 1;
    return -1;
errbs:
    errno = EBADF;
    return -1;
}


/*
** * * * * * *
** sendto()     Send a message to a remote host
**
** int sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen);
**
** PARAMETERS:
**  (in) s                      Socket ID number
**  (in) buf                    Data to transmit
**  (in) len                    Length of data to transmit
**  (in) flags                  DGRAM send options
**  (in) to                     Address of host to send to
**  (in) tolen                  Length of remote host address
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**  Transmit a message to a particular host in a connectionless manner.
**
** USAGE/COMMENTS:
**  This function finds the remote host's configuration index on the fly
**  and calls the normal socket send() operation.
**
** * * * * * *
** MODIFICATION HISTORY:
**   21-SEP-1999  BSK  Add: Multicasting index
** * * * * * *
*/
int sendto(int s, char *buf, int len, int flags, struct sockaddr *to, int tolen)
{
    int i1, confix;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;
    unsigned short us1;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
    if (conp->txstat & S_STRM)
        goto lab3;
    if (to == 0 || tolen < Iid_SZ) {
        socp->error = errno = EDESTADDRREQ;
        return -1;
    }
    conp->blockstat = 0x21;
   /* check out routing for the "to" address */
    conp->heriid.l = ((struct sockaddr_in *) to)->sin_addr.s_addr;
   /* flag to allow or disallow broadcasting */
    i1 = socp->sockopt & soBROADCAST ? 1 : 3;
   /* flag to allow or disallow routing */
    if (socp->sockopt & soDONTROUTE || flags & MSG_DONTROUTE)
        i1 |= 8;
    if ((confix = GetHostData(conp->heriid.l, i1)) < 0) {
        socp->error = errno = EHOSTUNREACH;
        conp->blockstat = 1;
        return -1;
    }
    if (confix != ussBroadcastIndex)
#if USS_IP_MC_LEVEL
      if (confix != ussMulticastIndex)
#endif
        conp->netno = netconf[confix].netno;
    conp->confix = confix;
    conp->herport = ((struct sockaddr_in *) to)->sin_port;
    if (conp->myport == 0)
        us1 = Nportno(), conp->myport = NC2(us1);
    conp->rxstat = 0;
    conp->state = 1;
    conp->blockstat = 1;
lab3:
    return send(s, buf, len, flags);
errbs:
    errno = EBADF;
    return -1;
}

/* BSD routine to send several messages with one call.
   As above, but calls send() for each requested message.
*/
int sendmsg(int s, struct msghdr * msg, int flags)
{
    int i1, confix, msgix, tbytes;
    unsigned short us1;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;
    struct iovec *iovp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS)
        goto errbs;
    conp = &connblo[s];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[s];
    if (conp->txstat & S_STRM)
        goto lab3;
    conp->blockstat = 0x21;
    conp->herport = ((struct sockaddr_in *) msg->msg_name)->sin_port;
    conp->heriid.l = ((struct sockaddr_in *) msg->msg_name)->sin_addr.s_addr;
   /* flag to allow or disallow broadcasting */
    i1 = socp->sockopt & soBROADCAST ? 1 : 3;
   /* flag to allow or disallow routing */
    if (socp->sockopt & soDONTROUTE || flags & MSG_DONTROUTE)
        i1 |= 8;
    if ((confix = GetHostData(conp->heriid.l, i1)) < 0) {
        socp->error = errno = EHOSTUNREACH;
        conp->blockstat = 1;
        return -1;
    }
    conp->confix = confix;
    conp->netno = netconf[confix].netno;
    if (conp->myport == 0)
        us1 = Nportno(), conp->myport = NC2(us1);
    conp->rxstat = 0;
    conp->state = 1;
    conp->blockstat = 1;
lab3:
    for (tbytes = msgix = 0; msgix < msg->msg_iovlen; msgix++) {
        iovp = &msg->msg_iov[msgix];
        i1 = send(s, iovp->iov_base, iovp->iov_len, flags);
        if (i1 < 0)
            return i1;
        tbytes += i1;
    }
    return tbytes;
errbs:
    errno = EBADF;
    return -1;
}

/*
** * * * * * *
** shutdown()   Shut down a connection
**
** int shutdown(int s, int how)
**
** PARAMETERS:
**   (in) s                     Socket ID number
**   (in) how                   The nature of the shutdown
**
** RETURNS:
**    0                         Succedded.
**   -1                         Error occured.  Check sockets errno.
**
** DESCRIPTION:
**   Shut down a connection how:
**      0 -- shut down recv()
**      1 -- shut down send(), send FIN for TCP
**      2 -- shut down both
**
**   The connection will remain open with all resources allocated, but the
**   how operation will no longer function.
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   02-AUG-1999  BSK  Fix: For single socket concurrent operations
** * * * * * *
*/
int shutdown(int s, int how)
{
    MESS *mp;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) s >= NCONNS || (unsigned int) how > 2)
        goto errbs;
    conp = &connblo[s];
    if ((conp->blockstat & 0x9f) != 1)
        goto errbs;
    socp = &socblock[s];
   /* shut down recv: just set flag */
    if ((how & 1) == 0)
        socblock[s].sockopt |= soSHUTRECV;
   /* shut down send: set flag, send FIN for TCP */
    if (how != 0) {
        socblock[s].sockopt |= soSHUTSEND;
        if (conp->txstat & S_STRM) {
            SOCKET_FIN(s);
            if ((mp = Ngetbuf()) == 0) {
                socp->error = errno = ENOBUFS;
                return -1;
            }
            mp->mlen = conp->doffset;
            mp->netno = conp->netno;
            mp->target = conp->heriid.l;
           /* this write will not block */
            if (conp->protoc[0]->writE(s, mp))
                Nrelbuf(mp);
        }
    }
    return 0;
errbs:
    errno = EBADF;
    return -1;
}


/*
** * * * * * *
** selectcheck()    Check conditions of select
**
** int selectcheck(int nfds, fd_set *readfds, fd_set *writefds,
**      fd_set *exceptfds)
**
** PARAMETERS:
**  (in) nfds                   The number of socket descriptors to test
**  (in) readfds                Sockets to test for read
**  (in) writefds               Sockets to test for write
**  (in) exceptfds              Sockets to test for exceptions
**  (in) timeout                Period to wait
**
** RETURNS:
**  0                           Succedded.
** -1                           Error occured.  Check sockets errno.
**
** DESCRIPTION:
**  For each descriptor that does not change, it's bit is cleared in a
**  temporary set.  If no descriptors changed, the function returns 0.
**  If any descriptor changed, the temporary sets with bits cleared are
**  copied back to the original sets and the function returns the total
**  number of changes.
**
**  Read tests for:
**      data available
**      connection half closed
**      listen in progress and client arrived
**      socket error
**
**  Write tests for:
**      Can send at least 100 bytes
**      socket error
**
**  Exception tests for:
**      out of band data available
**
** USAGE/COMMENTS:
**
** * * * * * *
** MODIFICATION HISTORY:
**   04-AUG-1999  BSK  Created
** * * * * * *
*/
static int selectcheck(int nfds, fd_set *readfds, fd_set *writefds,
    fd_set *exceptfds)
{
    int changes;
    fd_set rd, wr, ex;
    struct CONNECT *conp;

    if (readfds)
        rd = *readfds;
    if (writefds)
        wr = *writefds;
    if (exceptfds)
        ex = *exceptfds;

    for (changes = 0, nfds--; nfds >= 0; nfds--) {
        conp = &connblo[nfds];
        if (readfds)
            if (FD_ISSET(nfds, readfds)) {
                if (conp->blockstat != 1)
                    goto lab1;
                if (conp->first || socblock[nfds].istreamc ||
                    (conp->rxstat & (S_EOF | S_FATAL)) ||
                 ((socblock[nfds].sockopt & soLISTEN) && conp->next))
                    changes++;
                else {
lab1:
                   FD_CLR(nfds, &rd);
                }
            }
        if (writefds)
            if (FD_ISSET(nfds, writefds)) {
                if (conp->blockstat != 1)
                    goto lab2;
                if (SOCKET_CANSEND(nfds, 100) || (conp->rxstat & S_FATAL))
                    changes++;
                else {
lab2:
                    FD_CLR(nfds, &wr);
                }
            }
        if (exceptfds)
            if (FD_ISSET(nfds, exceptfds)) {
                if (conp->blockstat != 1)
                    goto lab3;
                if (conp->tcpflags & S_URG)
                    changes++;
                else {
lab3:
                    FD_CLR(nfds, &ex);
                }
            }
    }

   /*
    ** Only overwrite descriptors if things changed because this will be
    ** called again with the original descriptors if there were no changes.
    */
    if (changes) {
        if (readfds)
            *readfds = rd;
        if (writefds)
            *writefds = wr;
        if (exceptfds)
            *exceptfds = ex;
    }
    return changes;
}


/*
** * * * * * *
** selectsocket()    Wait for events on multiple sockets
**
** int selectsocket(int nfds, fd_set *readfds, fd_set *writefds,
**      fd_set *exceptfds, struct timeval *timeout)
**
** PARAMETERS:
**  (in) nfds                   The number of socket descriptors to test
**  (in) readfds                Sockets to test for read
**  (in) writefds               Sockets to test for write
**  (in) exceptfds              Sockets to test for exceptions
**  (in) timeout                Period to wait
**
** RETURNS:
**  #                           The number of descriptors that are true
**
** DESCRIPTION:
**  Use FD_SET(), FD_CLR(), FD_ISSET() and FD_ZERO() to manage which
**  sockets are to be tested for read/write/exception cases.
**
**  This routine calls the selectcheck() routine periodically to find out
**  if anything has changed.
**
** USAGE/COMMENTS:
**  The BSD select() routine returns 0 for success or -1 for failure and
**  clears any untrue descriptors in the sets passed in.
**
**  The Linux select() routine returns the number of descriptors that were
**  true but labels the descriptor sets as undefined upon return.
**
**  This routine will return the number of true descriptors and will clear
**  any untrue descriptors.
**
**
**  If a multitasker is being used, nettask() should be signalling the
**  SIG_GEN event so that the selectcheck() routine is called for each
**  iteration of NetTask() (a series of network packets was processed).
**
**  If no multitasker is used, selectcheck() is polled until a change
**  in the read/write/except status for any selected descriptors occurs.
**  YIELD() will be called in the mean time.
**
** * * * * * *
** MODIFICATION HISTORY:
**   05-AUG-1999  BSK  Fix for multitasking mode
**   02-AUG-1999  BSK  Comment updated
** * * * * * *
*/
int selectsocket(int nfds, fd_set *readfds, fd_set *writefds,
    fd_set *exceptfds, struct timeval *timeout)
{
    int i1, i2;
    unsigned long tout; /* timeout in msecs, converted from timeout */

    if (timeout)
        tout = (timeout->tv_sec * 1000) + ((timeout->tv_usec + 500) / 1000);
    else
        tout = 0xffffffff;

    if (nfds > NCONNS)
        nfds = NCONNS;

   /* For every WAITNOMORE(SIG_GEN), check for socket read/write/except */
    WAITFOR((i2 = selectcheck(nfds, readfds, writefds, exceptfds)),
        SIG_GEN, tout, i1);

#if 1
    return i2;
#else
    return i1 ? -1 : 0;
#endif
}


/* Routine to return host information when a name is supplied.
   The name is normally "hostname", but "hostname/portname" can be used when
   you want to talk using a specific port or network connection.

   NOTE: The initial BSD definition gethostbyname() is not reentrant,
   do not use with multitasking.  The reentrant version uses the Solaris
   interface.
*/
struct hostent *gethostbyname_r(char *hnp, struct hostent *result,
    char *buffer, int buflen, int *h_errnop)
{
    int retstat, netno, remix, len, hops;
    char name[HOSTNAME_LEN+DOMAIN_LEN], pname[16];
    Iid iid;
    struct NETCONF *confp;

    retstat = -1;
    name[0] = pname[0] = '*';
    Nsscanf(hnp, "%[^/]/%s", name, pname);
    len = strlen(name) + 1;
   /* buffer must have room for name, IP address, 2 long pointers */
    if (buflen < Iid_SZ + len + 11)
        goto err1;
    strcpy(buffer, name);
    for (remix = 0, hops = 255; remix < NCONFIGS; remix++) {
        confp = &netconf[remix];
        if (confp->ncstat == 0)
            continue;
        if (pname[0] != '*' && strcmp(pname, confp->pname) != 0)
            continue;
        if (name[0] == '*') {
            if (pname[0] == '*')
                iid.l = 0xffffffff;
            else
                iid.l = confp->Iaddr.l | ~confp->Imask.l;
            goto lab1;
        }
        if (strcmp(confp->name, name) != 0)
            continue;
        if (confp->hops < (unsigned int) hops) {
            iid = confp->Iaddr;
            hops = confp->hops;
        }
    }
    if (hops == 255) {
#ifdef DNS
#if DNS == 2
        retstat = DNSresolve(name, (IPaddr *) & iid);
        if (retstat >= 0)
            goto lab1;
#endif
#endif
        goto err1;
    }
lab1:
    result->h_name = buffer;
    result->h_aliases = 0;
    result->h_addrtype = AF_INET;
    result->h_length = Iid_SZ;
    netno = (4 - (int) buffer) & 3;
    len = netno + ((len + 3) & ~3);
    memcpy(buffer + len, (char *) &iid, Iid_SZ);
    result->h_addr_list = (char **) (buffer + len + Iid_SZ);
    result->h_addr_list[0] = buffer + len;
    result->h_addr_list[1] = 0;
    *h_errnop = 0;
    return result;
err1:
    *h_errnop = retstat;
    return 0;
}

static struct hostent hostent;
static char     buff[64];
struct hostent *gethostbyname(char *hnp)
{
    int             stat;

    gethostbyname_r(hnp, &hostent, buff, sizeof(buff), &stat);
    if (stat >= 0)
        return &hostent;
    return 0;
}

/* Routine to return host information when an IP address is supplied.

   NOTE: The initial BSD definition gethostbyaddr() is not reentrant,
   do not use with multitasking.  The reentrant version uses the Solaris
   interface.
*/
struct hostent *gethostbyaddr_r(char *addr, int len, int type,
    struct hostent *result, char *buffer, int buflen, int *h_errnop)
{
    int retstat, netno, remix, wlen;
    struct NETCONF *confp;

    retstat = -1;
    if (len != Iid_SZ)
        goto err1;
    if (type != AF_INET)
        goto err1;
    for (remix = 0; remix < NCONFIGS; remix++) {
        confp = &netconf[remix];
        if (confp->ncstat == 0)
            continue;
        if (confp->Iaddr.l == ((struct in_addr *) addr)->s_addr)
            goto lab1;
    }
    goto err1;
lab1:
    wlen = strlen(confp->name) + 1;
   /* buffer must have room for name, IP address, 2 long pointers */
    if (buflen < Iid_SZ + wlen + 11)
        goto err1;
    strcpy(buffer, confp->name);
    result->h_name = buffer;
    result->h_aliases = 0;
    result->h_addrtype = AF_INET;
    result->h_length = Iid_SZ;
    netno = (4 - (int) buffer) & 3;
    wlen = netno + ((wlen + 3) & ~3);
    memcpy(buffer + wlen, addr, Iid_SZ);
    result->h_addr_list = (char **) (buffer + wlen + Iid_SZ);
    result->h_addr_list[0] = buffer + wlen;
    result->h_addr_list[1] = 0;
    *h_errnop = 0;
    return result;
err1:
    *h_errnop = retstat;
    return 0;
}

static struct hostent hostent2;
static char     buff2[64];
struct hostent *gethostbyaddr(char *addr, int len, int type)
{
    int stat;

    gethostbyaddr_r(addr, len, type, &hostent2, buff2, sizeof(buff2), &stat);
    if (stat >= 0)
        return &hostent2;
    return 0;
}

/* Routine fcntl to get and set various socket options. */
int fcntlsocket(int fildes, int cmd, int arg)
{
    int status;
    struct CONNECT *conp;

   /* reject invalid socket number, socket not open */
    if ((unsigned int) fildes >= NCONNS)
        goto errbs;
    conp = &connblo[fildes];
    if (conp->blockstat != 1)
        goto errbs;
    status = -1;
    switch (cmd) {
   /* get status flags, non-blocking sockets */
    case F_GETFL:
        status = conp->txstat & S_NOWA ? O_NDELAY : 0;
        break;
    case F_SETFL:
       /* set status flags, non-blocking sockets */
        if (arg & O_NDELAY)
            conp->txstat |= S_NOWA, conp->rxtout = 0;
        else
            conp->txstat &= ~S_NOWA, conp->rxtout = 0xffffffff;
        status = 0;
        break;
    }
    return status;
errbs:
    errno = EBADF;
    return -1;
}

/* Routine ioctl to get and set various socket options. */
int ioctlsocket(int fildes, int request,...)
{
    int status;
    int thdrsz;     /* size of transport layer header */
    int *ip;
    va_list args;
    MESS *mp;
    struct CONNECT *conp;
    struct SOCBLOCK *socp;

    va_start(args, request);

   /* reject invalid socket number, socket not open */
    if ((unsigned int) fildes >= NCONNS)
        goto errbs;
    conp = &connblo[fildes];
    if (conp->blockstat != 1)
        goto errbs;
    socp = &socblock[fildes];
    switch (request) {
    default:
        status = -1;
        break;
    case FIONBIO:               /* set or clear non-blocking I/O */
        if (*(int *) va_arg(args, int *) !=0)
            conp->txstat |= S_NOWA, conp->rxtout = 0;
        else
            conp->txstat &= ~S_NOWA, conp->rxtout = 0xffffffff;
        status = 0;
        break;
    case FIONREAD:              /* number of bytes to read */
       /* Add the number of bytes not yet read from the incoming socket
        ** stream buffer to the number of application bytes queued in
        ** message buffers that have not yet been read.  Note that the
        ** offset field in the message header points to the start of the
        ** transport layer header when message buffers are in the
        ** connection's "first" queue. */
        ip = va_arg(args, int *);
        BLOCKPREE();
        status = socp->istreamc;
        for (mp = conp->first; mp; mp = mp->next) {
            thdrsz = 0;
            if (conp->txstat & S_STRM)
                thdrsz = ((*((char *) mp + mp->offset + 12) >> 4) << 2);
#ifdef UDP
            else if (conp->protoc[0] == &ussUDPTable)
                thdrsz = 8;
#endif
            status += mp->mlen - mp->offset - thdrsz;
        }
        RESUMEPREE();
        *ip = status;
        status = 0;
        break;
   /* check if there is out-of-bound data for OOBINLINE case */
    case SIOCATMARK:
        status = 0;
        ip = va_arg(args, int *);
        if (conp->tcpflags & S_URG)
            status = 1;
        *ip = status;
        status = 0;
        break;
    }
    return status;
errbs:
    errno = EBADF;
    return -1;
}

/* the UNIX sleep routine */
void sleepsocket(int sec)
{
    unsigned long ul1, ul2;

    ul2 = sec * 1000;
    for (ul1 = TimeMS(); TimeMS() - ul1 < ul2;)
        YIELD();
}

/* byte swapping routines */
unsigned short htons(unsigned short val)
{
#ifdef LITTLE
    return ((val & 0xff) << 8) | ((unsigned short) val >> 8);
#else
    return val;
#endif
}

unsigned long htonl(unsigned long val)
{
#ifdef LITTLE
    return ((long) val << 24) | (((long) val & 0xff00) << 8) |
    (val >> 8) & 0xff00 | ((unsigned long) val >> 24);
#else
    return val;
#endif
}

/* conversions to and from dotted decimal notation */

/* This function exists in two forms, one returns unsigned long and one
   struct in_addr.  We use here the Solaris definition. */
unsigned long inet_addr(char *dotted)
{
    int i1, i2, i3, i4;
    Iid iid;

    Nsscanf(dotted, "%d.%d.%d.%d", &i1, &i2, &i3, &i4);
    iid.c[0] = i1;
    iid.c[1] = i2;
    iid.c[2] = i3;
    iid.c[3] = i4;
    return iid.l;
}

/* This is not reentrant by definition, be careful in using. */
static char ntoabuf[16];
char *inet_ntoa(struct in_addr addr)
{
    Nsprintf(ntoabuf, "%d.%d.%d.%d", ((unsigned char *) &addr.s_addr)[0],
    ((unsigned char *) &addr.s_addr)[1], ((unsigned char *) &addr.s_addr)[2],
             ((unsigned char *) &addr.s_addr)[3]);
    return ntoabuf;
}
/*****************************************************************
				ソケットセットでの動作を待ちます。
*****************************************************************/
int             UsnetSocket_selectsocket(int nfds, fd_set * readfds, fd_set * writefds,
                              fd_set * exceptfds, struct timeval * timeout)
{
	selectsocket( nfds, readfds, writefds, exceptfds, timeout);
}

/*****************************************************************
			ソケットに繋がるコネクションを受け付けます。
*****************************************************************/
int             UsnetSocket_accept(int s, struct sockaddr * name, int *namelen)
{
	accept( s, name, namelen);
}

/*****************************************************************
			ソケットに名前をバインディングします。
*****************************************************************/
int             UsnetSocket_bind(int s, struct sockaddr * name, int namelen)
{
	bind( s, name, namelen);
}

/*****************************************************************
			ソケットのコネクションを開始します。
*****************************************************************/
int             UsnetSocket_connect(int s, struct sockaddr * name, int namelen)
{
	connect( s, name, namelen);
}

/*****************************************************************
			ソケットのローカルアドレス情報を抽出します。
*****************************************************************/
int             UsnetSocket_getsockname(int s, struct sockaddr * name, int *namelen)
{
	getsockname( s, name, namelen);
}

/*****************************************************************
			ソケットのリモートアドレス情報を抽出します。
*****************************************************************/
int             UsnetSocket_getpeername(int s, struct sockaddr * peer, int *addrlen)
{
	getpeername( s, peer, addrlen);
}

/*****************************************************************
				ソケットのオプションを獲得します。
*****************************************************************/
int             UsnetSocket_getsockopt(int s, int level, int optname, char *optval, int *optlen)
{
	getsockopt( s, level, optname, optval, optlen);
}

/*****************************************************************
				ソケットのオプションを設定します。
*****************************************************************/
int             UsnetSocket_setsockopt(int s, int level, int optname, char *optval, int optlen)
{
	setsockopt( s, level, optname, optval, optlen);
}

/*****************************************************************
				ソケットのコネクションを開始します。
*****************************************************************/
int             UsnetSocket_listen(int s, int backlog)
{
	listen( s, backlog);
}

/*****************************************************************
				メッセージを受け取ります。
*****************************************************************/
int             UsnetSocket_recv(int s, char *buf, int len, int flags)
{
	recv( s, buf, len, flags);
}

/*****************************************************************
			ソケットＩＤからメッセージを受け取ります。
*****************************************************************/
int             UsnetSocket_readsocket(int s, char *buf, int len)
{
	readsocket( s, buf, len);
}

/*****************************************************************
			コネクションからメッセージを受け取ります。
*****************************************************************/
int             UsnetSocket_recvfrom(int s, char *buf, int len, int flags,
										 struct sockaddr * from, int *fromlen)
{
	recvfrom( s, buf, len, flags, from, fromlen);
}

/*****************************************************************
			コネクションを確立し、メッセージを受け取ります。
*****************************************************************/
int             UsnetSocket_recvmsg(int s, struct msghdr * msg, int flags)
{
	recvmsg( s, msg, flags);
}

/*****************************************************************
		確立されたコネクション上でメッセージを送信します。
*****************************************************************/
int             UsnetSocket_send(int s, char *buf, int len, int flags)
{
	send( s, buf, len, flags);
}

/*****************************************************************
				メッセージをソケットに送ります。
*****************************************************************/
int             UsnetSocket_writesocket(int s, char *buf, int len)
{
	writesocket( s, buf, len);
}

/*****************************************************************
			コネクションを確立しメッセージを送信します。
*****************************************************************/
int             UsnetSocket_sendto(int s, char *buf, int len, int flags,
									 struct sockaddr * to, int tolen)
{
	sendto( s, buf, len, flags, to, tolen);
}

/*****************************************************************
	複数のバッファに分割して置くことのできるメッセージを送信します。
*****************************************************************/
int             UsnetSocket_sendmsg(int s, struct msghdr * msg, int flags)
{
	sendmsg( s, msg, flags);
}

/*****************************************************************
	複数のバッファに分割して置くことのできるメッセージを送信します。
*****************************************************************/
int             UsnetSocket_shutdown(int s, int how)
{
	shutdown( s, how);
}

/*****************************************************************
						ソケットを生成します。
*****************************************************************/
int             UsnetSocket_socket(int domain, int type, int protocol)
{
	socket( domain, type, protocol);
}

/*****************************************************************
					ソケットをクローズします。
*****************************************************************/
int             UsnetSocket_closesocket(int conno)
{
	closesocket(conno);
}

/*****************************************************************
				ホスト名のＩＰアドレスを返します。
*****************************************************************/
struct hostent *UsnetSocket_gethostbyname_r(char *hnp, struct hostent * result,
                            	       char *buffer, int buflen, int *h_errnop)
{
	gethostbyname_r(hnp, result, buffer, buflen, h_errnop);
}

/*****************************************************************
				ホスト名のＩＰアドレスを返します。
*****************************************************************/
struct hostent *UsnetSocket_gethostbyname(char *hnp)
{
	gethostbyname(hnp);
}

/*****************************************************************

*****************************************************************/
struct hostent *UsnetSocket_gethostbyaddr_r(char *addr, int len, int type,
             struct hostent * result, char *buffer, int buflen, int *h_errnop)
{
	gethostbyaddr_r(addr, len, type, result, buffer, buflen, h_errnop);
}

/*****************************************************************

*****************************************************************/
struct hostent *UsnetSocket_gethostbyaddr(char *addr, int len, int type)
{
	gethostbyaddr(addr, len, type);
}

/*****************************************************************
				ソケットのフラグを制御します。
*****************************************************************/
int             UsnetSocket_fcntlsocket(int fildes, int cmd, int arg)
{
	fcntlsocket( fildes, cmd, arg);
}

/*****************************************************************
				ソケットの制御パラメータを設定します。
*****************************************************************/
int             UsnetSocket_ioctlsocket(int fildes, int request, char *arg)
{
	ioctlsocket( fildes, request, arg);
}

/*****************************************************************
				ソケットの制御パラメータを設定します。
*****************************************************************/
void            UsnetSocket_sleepsocket(int sec)
{
	sleepsocket(sec);
}

