/*---------------------------------------------------------------------
 * $Header: p:/presto2/shinc/usnets/RCS/socket.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: socket.h $
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * SH2用TCP/IPドライバ・インクルード
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* SOCKET.H  Edition: 8  Last changed: 26-Aug-99,10:15:24  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    SOCKET.H -- BSD Socket Definitions for USNET

    Copyright (C) 1993, 1995 By
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

** * * * * * *
 MODIFICATION HISTORY:
    26-AUG-1999  BSK  Add: SO_BINDTODEVICE
    22-JUL-1999  BSK  Add: Multicast options
    28-MAY-1999  BSK  Chg: fd_set size reduction
** * * * * * *
*/

#ifndef errno
extern int      errno;
#endif

/* protocol family */

#define	PF_UNSPEC	0           /* unspecified protocol family */
#define	PF_INET		2           /* TCP/IP and related */
#define	AF_UNSPEC	0           /* unspecified address family */
#define	AF_INET		2           /* TCP/IP and related */

/* socket types */

#define	SOCK_STREAM	1           /* stream socket */
#define	SOCK_DGRAM	2           /* datagram socket */
#define	SOCK_RAW	3           /* raw-protocol interface */

/* options for getsockopt() and setsockopt() */

#define	SOL_SOCKET	0xffff      /* options for socket level */
#define	IPPROTO_TCP	0x0001      /* options for TCP level */
#define	IPPROTO_IP	0x0002      /* options for IP level */

#define SO_DEBUG        0x0001  /* turn on debugging info recording */
#define	SO_REUSEADDR	0x0004  /* allow local address reuse */
#define	SO_KEEPALIVE	0x0008  /* keep connections alive */
#define	SO_DONTROUTE	0x0010  /* just use interface addresses */
#define	SO_BROADCAST	0x0020  /* permit sending of broadcast msgs */
#define SO_BINDTODEVICE 0x0040  /* Bind a socket to an interface */
#define SO_LINGER       0x0080  /* linger on close if data present */
#define	SO_OOBINLINE	0x0100  /* leave received OOB data in line */
#define SO_SNDBUF       0x1001  /* send buffer size */
#define SO_RCVBUF       0x1002  /* receive buffer size */
#define SO_ERROR        0x1007  /* get error status and clear */
#define SO_TYPE         0x1008  /* get socket type */

#define	TCP_MAXSEG	0x2000      /* maximum segment size */
#define	TCP_NODELAY	0x2001      /* immediate send() */

#define	IP_OPTIONS	0x0001      /* IP header options */

/* options for recv and send */

#define MSG_OOB		0x01        /* send or receive out of band data */
#define MSG_PEEK	0x02        /* take data but leave it */
#define MSG_DONTROUTE	0x04    /* do not route */

/* options for fcntl */

#define O_NDELAY 	0x04        /* non-blocking */
#define FNDELAY O_NDELAY        /* synonym */
#define F_GETFL		3           /* get flags */
#define F_SETFL		4           /* set flags */

/* options for ioctl */

#define SIOCATMARK	7           /* check for out of bound data */
#define FIONBIO		126         /* set/clear non-blocking I/O */
#define FIONREAD	127         /* number of bytes to read */

/* structures */

struct sockaddr {               /* generic socket address */
    unsigned short  sa_family;  /* address family */
    char            sa_data[14];/* up to 14 bytes of direct address */
};
struct in_addr {                /* Internet address */
    unsigned long   s_addr;
};
struct sockaddr_in {            /* Internet socket address */
    short           sin_family; /* should be unsigned but this is BSD */
    unsigned short  sin_port;   /* network order !!! */
    struct in_addr  sin_addr;
    char            sin_zero[8];
};

struct iovec {                  /* address and length */
    char           *iov_base;   /* base */
    int             iov_len;    /* size */
};
struct msghdr {                 /* Message header for recvmsg and sendmsg. */
    char           *msg_name;   /* optional address */
    int             msg_namelen;/* size of address */
    struct iovec   *msg_iov;    /* scatter/gather array */
    int             msg_iovlen; /* # elements in msg_iov */
    char           *msg_accrights;  /* access rights sent/received */
    int             msg_accrightslen;
};

struct hostent {                /* structure for gethostbyname */
    char           *h_name;     /* official name of host */
    char          **h_aliases;  /* alias list */
    int             h_addrtype; /* host address type */
    int             h_length;   /* length of address */
    char          **h_addr_list;/* list of addresses from name server */
#define	h_addr h_addr_list[0]   /* address, for backward compatiblity */
};

struct servent {                /* structure for getservbyname */
    char           *s_name;     /* official service name */
    char          **s_aliases;  /* alias list */
    int             s_port;     /* port # */
    char           *s_proto;    /* protocol to use */
};

struct linger {                 /* structure for the SO_LINGER option */
    int             l_onoff;    /* zero=off, nonzero = on */
    int             l_linger;   /* linger time, in seconds */
};

/*
** The maximum number of socket descriptors that USNET can have is the same
**   as the number of possible connections, NCONNS.
*/
#define FD_SETSIZE NCONNS
#define FD_SET(n, p) ((p)->fds_bits[(n)>>3] |= (1 << ((n) & 7)))
#define FD_CLR(n, p) ((p)->fds_bits[(n)>>3] &= ~(1 << ((n) & 7)))
#define FD_ISSET(n, p) ((p)->fds_bits[(n)>>3] & (1 << ((n) & 7)))
#define FD_ZERO(p) memset((void *)(p), 0, sizeof(*(p)))
typedef struct {
    unsigned char fds_bits [(FD_SETSIZE + 7) / 8];
} fd_set;

struct timeval {                /* Timeout format for select() */
    long            tv_sec;     /* seconds */
    long            tv_usec;    /* microseconds */
};


/*
** Multicast
*/
struct ip_mreq {
    struct in_addr imr_multiaddr;   /* IP class D multicast address */
    struct in_addr imr_interface;   /* IP address of local interface */
};
#define IP_MULTICAST_IF     0x0006
#define IP_ADD_MEMBERSHIP   0x0009
#define IP_DROP_MEMBERSHIP  0x000a


/* BSD socket error codes */

#undef EBADF

#define NE_PARAM	-10         /* user parameter error */
#define EHOSTUNREACH	-11     /* host not reachable */
#define ETIMEDOUT	-12         /* timeout */
#define ECONNABORTED	-14     /* protocol error */
#define ENOBUFS		-15         /* no buffer space */
#define EBADF		-16         /* connection block invalid */
#define EFAULT		-17         /* invalid pointer argument */
#define	EWOULDBLOCK	-18         /* operation would block */
#define	EMSGSIZE	-19         /* message too long */
#define	ENOPROTOOPT	-20         /* Protocol not available */

#define	EDESTADDRREQ	-50     /* Destination address required */
#define	EPROTOTYPE	-52         /* Protocol wrong type for socket */
#define	EPROTONOSUPPORT	-54     /* Protocol not supported */
#define	ESOCKTNOSUPPORT	-55     /* Socket type not supported */
#define	EOPNOTSUPP	-56         /* Operation not supported on socket */
#define	EPFNOSUPPORT	-57     /* Protocol family not supported */
#define	EAFNOSUPPORT	-58     /* Address family not supported by */
 /* protocol family */
#define	EADDRINUSE	-59         /* Address already in use */
#define	EADDRNOTAVAIL	-60     /* Can't assign requested address */
#define	ENETDOWN	-61         /* Network is down */
#define	ENETUNREACH	-62         /* Network is unreachable */
#define	ENETRESET	-63         /* Network dropped connection because */
 /* of reset */
#define	ECONNRESET	-65         /* Connection reset by peer */
#define	EISCONN		-67         /* Socket is already connected */
#define	ENOTCONN	-68         /* Socket is not connected */
#define	ESHUTDOWN	-69         /* Can't send after socket shutdown */
#define	ECONNREFUSED	-72     /* Connection refused */
#define	EHOSTDOWN	-73         /* Host is down */
#define	EALREADY	-76         /* operation already in progress */
#define	EINPROGRESS	-77         /* operation now in progress */

/* byte swapping routines */

unsigned short  htons(unsigned short);
#define ntohs(val) htons(val)
unsigned long   htonl(unsigned long);
#define ntohl(val) htonl(val)

/* function prototypes */

int             Ninit(void);
int             Nterm(void);
int             Portinit(char *port);
int             Portterm(char *port);
int             Nprintf(char *form,...);
void            Nputchr(char);
int             Nchkchr(void);
int             Ngetchr(void);

int             selectsocket(int nfds, fd_set * readfds, fd_set * writefds,
                              fd_set * exceptfds, struct timeval * timeout);
int             accept(int s, struct sockaddr * name, int *namelen);
int             bind(int s, struct sockaddr * name, int namelen);
int             connect(int s, struct sockaddr * name, int namelen);
int             getsockname(int s, struct sockaddr * name, int *namelen);
int             getpeername(int s, struct sockaddr * peer, int *addrlen);
int             getsockopt(int s, int level, int optname, char *optval, int *optlen);
int             setsockopt(int s, int level, int optname, char *optval, int optlen);
int             listen(int s, int backlog);
int             recv(int s, char *buf, int len, int flags);
#define readsocket(s, buf, len) recv(s, buf, len, 0)
int             recvfrom(int s, char *buf, int len, int flags,
                         struct sockaddr * from, int *fromlen);
int             recvmsg(int s, struct msghdr * msg, int flags);
int             send(int s, char *buf, int len, int flags);
#define writesocket(s, buf, len) send(s, buf, len, 0)
int             sendto(int s, char *buf, int len, int flags,
                         struct sockaddr * to, int tolen);
int             sendmsg(int s, struct msghdr * msg, int flags);
int             shutdown(int s, int how);
int             socket(int domain, int type, int protocol);
int             closesocket(int conno);
struct hostent *gethostbyname_r(char *hnp, struct hostent * result,
                                   char *buffer, int buflen, int *h_errnop);
struct hostent *gethostbyname(char *hnp);
struct hostent *gethostbyaddr_r(char *addr, int len, int type,
                                                struct hostent * result, char *buffer, int buflen, int *h_errnop);
struct hostent *gethostbyaddr(char *addr, int len, int type);
int             fcntlsocket(int fildes, int cmd, int arg);
int             ioctlsocket(int fildes, int request,...);
void            sleepsocket(int sec);
unsigned long   inet_addr(char *dotted);
char           *inet_ntoa(struct in_addr addr);
