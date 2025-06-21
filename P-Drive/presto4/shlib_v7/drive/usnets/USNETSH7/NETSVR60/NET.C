/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/net.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: net.c $
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
/* NET.C  Edition: 24  Last changed: 4-Aug-99,11:30:04  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    http://www.ussw.com
    503.844.6614

    NET.C -- User Level Routines for USNET

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

   04-AUG-1999  BSK  Add: WAITNOMORE(SIG_GEN) for each iteration of nettask
   24-MAY-1999  NJW  Fix Nopen() to properly handle limited subnet Bcasts
   21-APR-1999  NJW  Include calls to acquire and release hosts in host table
   31-DEC-1998  RCA  Adjusted PTABLE names
   30-SEP-1998  BSK  Added pppTimeout() function call
   20-AUG-1998  NJW  Type cast due to MetaWare compiler error for PPC

 * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <stdlib.h>
#include <string.h>
#define NETMODULE
#include "net.h"
#include "local.h"
#include "support.h"
#include "mtmacro.h"

static int nullinit(int netno, char *params)
{
    (void) netno;
    (void) params;
    return 0;
}
static void nullshut(int netno)
{
    (void) netno;
}
static int nullscreen(MESS *mess)
{
    (void) mess;
    return -1;
}
PTABLE ussNullTable =
   {"null", nullinit, nullshut, nullscreen, 0, 0, 0, 0, 0, 0, 0};

#ifdef IP
extern PTABLE ussIPTable;
#else
#define ussIPTable ussNullTable
#endif
#ifdef ICMP
extern PTABLE ussICMPTable;
#else
#define ussICMPTable ussNullTable
#endif
#ifdef IGMP
extern PTABLE   ussIGMPTable;
#else
#define ussIGMPTable ussNullTable
#endif
#ifdef ARP
extern PTABLE ussARPTable;
#else
#define ussARPTable ussNullTable
#endif
#ifdef RARP
extern PTABLE ussRARPTable;
#else
#define ussRARPTable ussNullTable
#endif
#ifdef UDP
extern PTABLE ussUDPTable;
#else
#define ussUDPTable ussNullTable
#endif
#ifdef TCP
extern PTABLE ussTCPTable;
#else
#define ussTCPTable ussNullTable
#endif

/* Other than well-known protocols.  These do not have fixed numbers. */
#ifdef LQRP
extern PTABLE ussLQRPTable;
#endif

GLOBALCONST
PTABLE * const P_tab[] = {
    &ussNullTable, &ussIPTable, &ussICMPTable, &ussARPTable,
    &ussRARPTable, &ussUDPTable, &ussTCPTable,
#ifdef LQRP
        &ussLQRPTable,
#endif
#ifdef IGMP
        &ussIGMPTable,
#endif
        0
};

unsigned char Usnet_UserID[USERID_LEN+1] = {"test"};
unsigned char Usnet_PassWd[PASSWD_LEN+1] = {"usnet"};

char localhostname[HOSTNAME_LEN]={"local"};
char remotehostname[HOSTNAME_LEN]={"remote"};

struct NET nets[NNETS];
struct CONNECT connblo[NCONNS];
GLOBALCONST const struct Eid
    ecast = {255, 255, 255, 255, 255, 255},
    ezero = {0, 0, 0, 0, 0, 0};

#undef A
#undef B
#undef C
#undef X
#define A {0xff,0x00,0x00,0x00}		/* standard network masks */
#define B {0xff,0xff,0x00,0x00}
#define C {0xff,0xff,0xff,0x00}
#define X {0,0,0,0}			/* use with DHCP */
#define EA0 {0,0,0,0,0,0}		/* zero external (Ethernet) address */

GLOBALCONST
struct NETDATA netdata[]={
/* MS7615SE01用 */  /* 2001/08/28 NH */
    "local",  "net1", C, {157,108,17,161},{0,0x11,0x22,0x33,0x44,0x66}, 0, Ethernet,    SH7615,  0, 0,
    "remote",  "net1", C, {157,108,17,56},EA0, 0, Ethernet,    SH7615,  0, 0,
    "dnssvr1",  "net1", C, {0,0,0,0},EA0, DNSVER, Ethernet,    SH7615,  0, 0,
    "dnssvr2",  "net1", C, {0,0,0,0},EA0, DNSVER, Ethernet,    SH7615,  0, 0,
#if 0
/* LTESTの場合はこの設定を使用する*/
    "none", "wrap", C, {127,0,0,1}, EA0, 0, Ethernet, WRAP, 0, 0,
/* MS7709(A)SE01 / MS7750SE01用 */
    "none",  "net1", C, {192,168,  1,100},{0,0x11,0x22,0x33,0x44,0x66}, 0, Ethernet,    NS8390,  0, 0,

/* MS7709(A)SE01のSCIFをPPPとして使用する場合の例 
    "none",  "net1", C, {192,168,  1,101},EA0, 0, PPP, SCI7709,  0, "CLOCK=20000000 BAUD=38400",
*/
/* MS7615SE01でSMC37C935のCOM1をPPPで使用する場合の例 
    "none",  "net1", C, {192,168,  1,101},EA0, 0, PPP, S37C935,  0, "CLOCK=115200 BAUD=38400",
*/
/* その他のホストの設定例 */
    "server","net1", C, {192,168,  1, 12},EA0, 0, 0, 0, 0, 0,

#endif
/* デフォルトルータの設定例 */
    "router","net1", C, {157,108,17,254},EA0, ROUTER, 0, 0, 0, 0,
};

#define NN sizeof(netdata)/sizeof(struct NETDATA)
GLOBALCONST
const int confsiz=NN;

struct NETCONF netconf[NCONFIGS];
struct ifgroup ifgroup;
/*#ifdef DNS*/
GLOBALCONST
char domain[DOMAIN_LEN] = "";
Iid DNSiid[2];
char DNSdomain[HOSTNAME_LEN+DOMAIN_LEN];
/*#endif*/

static char DHCP_flg = 0;

void WaitLinkUp( void );

#ifdef MIB2
struct sysgroup sysgroup;
#endif

static int gflag, gnetno;

/*
**  Used only in other USSW products.  This does
**  not pertain to USNET core
*/
#ifdef NETPEER
extern char *target_name;
#endif


/* Initialize the network environment.  Called only once, before any other
 ** USNET calls. */
int Ninit()
{
    int i1, i3, netno;
    register const struct NETDATA *datap;
    register struct NETCONF *confp;

#if NTRACE>=1
    Nprintf("Ninit: initialize network data \n");
#endif
    if (NCONFIGS <= confsiz) {
#if NTRACE >= 1
        Nprintf("NCONFIGS %d too small, %d entries configured\n",
                NCONFIGS, confsiz);
#endif
        return ENOBUFS;
    }
#ifdef NMTINIT
    if (NMTINIT() < 0)
        return -1;
#endif
#if MT || !defined(WRAPTEST)
    i1 = LOCALSETUP();
    if (i1 < 0)
        return i1;
#endif
#ifndef WRAPTEST
    LOCALHOSTNAME(localhostname);
#else
    strcpy(localhostname, "none");
#endif
#if NTRACE>=1
    Nprintf("Ninit: this computer is `%s`\n", localhostname);
#endif

    if ((i1 = Ninitsupp()) < 0)
        return i1;
    memset((void *) connblo, 0, sizeof(connblo));
    memset((void *) nets, 0, sizeof(nets));
#ifdef DNS
    memset((void *) &DNSiid[0], 0, sizeof(DNSiid));
    memset((void *) &DNSiid[1], 0, sizeof(DNSiid));
    memset(DNSdomain, 0, sizeof(DNSdomain));
/*    memcpy(DNSdomain, remotehostname, strlen(remotehostname) );
    strcat(DNSdomain, "." );
*/
    strcpy(DNSdomain, domain);
#endif
    Ninitbuf(MAXBUF, NBUFFS);

   /* Initialize network configuration table.  For local connections,
    ** initialize the network table. */
    memset((void *) netconf, 0, sizeof(struct NETCONF) * NCONFIGS);
    for (netno = i1 = 0; i1 < confsiz; i1++) {
        confp = &netconf[i1];
        datap = &netdata[i1];
        strcpy(confp->name, datap->name);
        strcpy(confp->pname, datap->pname);
        memcpy((char *) confp->Imask.c, datap->Imask.c, Iid_SZ);
        memcpy((char *) confp->Iaddr.c, datap->Iaddr.c, Iid_SZ);
        confp->Eaddr = datap->Eaddr;
        if (memcmp((char *) &confp->Eaddr, (char *) &ezero, Eid_SZ))
            confp->hwaddyes = 2;
        confp->flags = datap->flags;
        if (confp->flags & NOTUSED)
            continue;
        confp->ncstat = 4;      /* static */
        if (strcmp(confp->name, localhostname) != 0)
            continue;
#if NTRACE>=5
    Nprintf("Ninit: %s has interface (net) %d\n", confp->name, netno);
#endif
        confp->flags |= LOCALHOST;
        if (netno == NNETS)
            return ENOBUFS;
        nets[netno].netstat = -1;
        nets[netno].confix = i1;
        nets[netno].cflags = confp->flags;
        confp->netno = netno;
        netno++;
    }
    ifgroup.ifNumber = netno;
#if NTRACE >= 1
    if (netno == 0) {
        Nprintf("This computer's name `%s` not in NETCONF.C \n", localhostname);
        return -1;
    }
#endif

    i3 = BuildRoutes();         /* calculate routing */

   /* Initialize network/transport protocols. */
    for (i1 = 0; P_tab[i1]; i1++)
        if (P_tab[i1]->init != 0)
            P_tab[i1]->init(0, 0);

    return i3;
}

/* Shut off the networking.  Will shut off any open ports. */
int Nterm()
{
    Portterm("*");
#if MT || !defined(WRAPTEST)
    LOCALSHUTOFF();
#endif
    return 0;
}

/* Initialize a network connection or all the connections. */
int  Portinit(char *port)
{
	int i1, i2, netno;
	struct NET *netp;
	const struct NETDATA *datap;
	PTABLE **ppp;
	{
	}
#if NTRACE>=1
	Nprintf("Portinit: initialize device drivers\n");
#endif
	i2 = EHOSTUNREACH;
	for (netno = 0; netno < NNETS; netno++) {
		netp = &nets[netno];
		i1 = netp->confix;
#if NTRACE>=5
		Nprintf("Portinit: net=%d stat=%d pname=%s\n",
			netno, netp->netstat, netconf[i1].pname);
#endif
		if (netp->netstat >= 0)
			continue;
		datap = &netdata[i1];
		if (port[0] != '*' && strcmp(port, netconf[i1].pname))
			continue;
		ppp = netp->protoc;
		ppp[0] = datap->lprotoc;
		ppp[1] = datap->dprotoc;
		ppp[2] = datap->adapter;
		if (!ppp[2])
			ppp[2] = &ussNullTable;
			i2 = ppp[0]->init(netno, datap->params);
		if (i2 < 0)
			break;
#ifdef MIB2
		netp->ifDescr = (char *)ppp[1]->name;
		netp->ifLastChange = TimeMS() / 10;
#endif
#if MT
		if (netp->netstat == -1) {
#if 1
			gflag = 0;
			gnetno = netno;
#ifndef V3
			RUNTASK0(NetTask, NET_PRIOR);
#else
			RUNNETTASK();
#endif
			WAITFOR(gflag, SIG_GEN, 1000, i2);
#endif
		}
#endif
		netp->netstat = 1;
#ifdef DHCP
		if( DHCP_flg == 1 ) {
			if (netconf[netp->confix].Iaddr.l == 0) {
				i2 = DHCPget(netno, 0);
				if (i2 < 0)
				break;
			}
		}
#endif
    }
    return i2;
}

/* Shut off the networking. Called with "*" means a all ports. */
int Portterm(char *port)
{
    int netno, stat;
    register struct NET *netp;
    {
    }
    for (netno = 0; netno < NNETS; netno++) {
        netp = &nets[netno];
        if (netp->netstat != 1)
            continue;
        if (port[0] != '*' && strcmp(port, netconf[netp->confix].pname))
            continue;

#ifdef DHCP
		if ( DHCP_flg == 1 ) {
   	    	if (netp->DHCPserver) {
           		DHCPrelease(netno);
			}
		}
#endif
        WAITFOR( netp->hwflags == 0, SIG_WN(netno), netp->tout, stat);
        for (stat = (int) TimeMS(); (int) TimeMS() - stat < 100;) {
        }
        if (netp->protoc[0])
            netp->protoc[0]->shut(netno);
        netp->netstat = -2;
    }
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Nopen()        Open a connection
**
**  int Nopen(char *to, char *protoc, int p1, int p2, int flags);
**
**  PARAMETERS:
**    (in)      *to          Name, IP addr. string, or literal IP addr.
**    (in)      *protoc      Protocol -- e.g. "TCP/IP"
**    (in)      p1           The local host's port
**    (in)      p2           The remotes host's port
**    (in)      flags        Modifier flags
**
**  RETURNS:
**    conno                  Connection number
**    ENOBUFS                No connection blocks available
**    EHOSTUNREACH           Cannot reach host
**    negetive value         Lower layer open failure
**
**  DESCRIPTION:
**    Nopen() analyzes the "to" argument.  If it's not an IP address,
**    we scan the configuration table for the given name, host, or
**    host/port.  The information is placed into the connection block
**    and the transport layer opeN() call is made.
**
**  USAGE COMMENTS:
**    For hosts with multiple interfaces, a limited broadcast will
**    implement the first interface configured.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  24-MAY-1999  NJW  Fix to allow limited subnet broadcast.  Variable
**                      sconfix contains info for broadcasts.
**  21-APR-1999  NJW  Added host table entry acquisition.
**  19-DEC-1998  BSK  Chg: Fix for limited broadcast
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int Nopen(char *to, char *protoc, int p1, int p2, int flags)
{
    int i1, i2, i3, i4, conno, confix, sconfix;
    char *cp1, *cp2, name[32], pname[16];
    Iid iid;
    struct NETCONF *confp;
    register struct CONNECT *conp;


    if (flags & IPADDR)
        iid = *(Iid *) to;
    else if ((conno = Nsscanf(to, "%d.%d.%d.%d", &i1, &i2, &i3, &i4)) == 4)
        iid.c[0] = i1, iid.c[1] = i2, iid.c[2] = i3, iid.c[3] = i4;
    else {
#if NTRACE>=2
        Nprintf("Nopen: to=`%s` \n", to);
#endif
        name[0] = pname[0] = '*';
        Nsscanf(to, "%[^/]/%s", name, pname);
        for (i1 = 0, i3 = 255; i1 < NCONFIGS; i1++) {
            confp = &netconf[i1];
           /* If the interface is not configured, continue */
            if (confp->ncstat == 0)
                continue;
           /* If the port is not broadcast or doesn't match, continue */
            if (pname[0] != '*' && strcmp(pname, confp->pname) != 0)
                continue;
           /* If broadcasting, set broadcast IP address */
            if (name[0] == '*') {
               /* Port is also a broadcast means 'limited broadcast' */
                sconfix = i1;               /* Host with subnet interface */
                if (pname[0] == '*') {
                    iid.l = 0xffffffff;
                }
                else    /* Otherwise, a subnet directed broadcast */
                    iid.l = confp->Iaddr.l | ~confp->Imask.l;
                goto lab1;
            }
            if (strcmp(confp->name, name) != 0)
                continue;
            if (confp->hops < i3) {
                iid = confp->Iaddr;
                confix = i1;
                i3 = confp->hops;
            }
        }
        if (i3 == 255) {
#ifdef DNS
#if DNS == 2
            i1 = DNSresolve(name, &iid);
            if (i1 >= 0)
                goto lab1;
#endif
#endif
#if NTRACE >= 1
            Nprintf("Nopen: remote computer `%s` is not in NETCONF.C or unreachable\n", to);
#endif
            goto err1;
        }
        if (iid.l == 0 && netconf[confix].ncstat == 4)
            goto lab11;
    }
lab1:
    if ((confix = GetHostData(iid.l, 1)) < 0) {
#if NTRACE >= 1
        Nprintf("Nopen: no route to host %s\n", to);
#endif
        goto err1;
    }
lab11:
    BLOCKPREE();
    for (conno = 0; conno < NCONNS; conno++)
        if (!connblo[conno].blockstat) {
            conp = &connblo[conno];
            memset((void *) conp, 0, sizeof(struct CONNECT));
            conp->blockstat = (char) 0x81;
            break;
        }
    RESUMEPREE();
    if (conno == NCONNS)
        return ENOBUFS;
    for (i1 = 0, cp1 = protoc; *cp1; i1++) {
        for (cp2 = pname; *cp1; cp2++)
            if ((*cp2 = *cp1++) == '/')
                break;
        *cp2 = 0;
        for (i2 = 0; i2 < sizeof(P_tab) / sizeof(PTABLE *); i2++) {
            if (strcmp(pname, P_tab[i2]->name) == 0) {
                conp->protoc[i1] = P_tab[i2];
                goto lab2;
            }
        }
        goto err5;
lab2:   ;
    }
    conp->confix = confix;
    if (confix == ussBroadcastIndex)
        conp->netno = netconf[sconfix].netno;    /* Find subnet bcast iface */
#if USS_IP_MC_LEVEL > 0
    else if (confix == ussMulticastIndex)
        conp->netno = ussDfltMcNetno;
#endif
    else
        conp->netno = netconf[confix].netno;
#ifdef MIB2
    if (pname[0] != '*')
        conp->offerediid.l = netconf[nets[conp->netno].confix].Iaddr.l;
#endif
    conp->heriid = iid;
    conp->myport = NC2(p1);
    conp->herport = NC2(p2);
    conp->doffset = MESSH_SZ + LHDRSZ;
    for (i1 = 0; conp->protoc[i1]; i1++)
        conp->doffset += conp->protoc[i1]->hdrsiz;
    conp->maxdat = nets[conp->netno].maxblo - conp->doffset + MESSH_SZ +
        LHDRSZ;
    conp->blockstat = 1;
    i2 = conp->protoc[0]->opeN(conno, flags);
    if (i2 < 0) {
        conp->protoc[0]->closE(conno);
        conp->blockstat = 0;
        return i2;
    }
    ussHostAcquire(conp->confix);      /* Indicate host is acquired */
    conp->rxtout = TOUT_READ;
    return conno;
err5:
    conp->blockstat = 0;
err1:
    return EHOSTUNREACH;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  Nclose()     Dynamic Protocol Interface close connection function
**
**  int Nclose( int conno );
**
**  PARAMETERS:
**     (in)     conno   Connection ID number
**
**  DESCRIPTION:
**     Closes the specified connection, possibly waiting for
**     a complete close handshake.  In no case should the application
**     retry the close.  In some cases, such as TCP, the connection
**     block associated with conno will be freed after a timeout,
**     on the order on a minute.
**
**  RETURNS:
**     0        Normal close occured
**     EBADF    The connection number was invalid.  No close was performed.
**     ECONNABORTED    Protocol problem.  Connection was closed.
**
**  USAGE/COMMENTS:
**     Each call to Nopen() should be followed by a call to Nclose() if the
**     desired connection is to be shut down.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  21-APR-1999  NJW  Add release of host table entry
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
int             Nclose(int conno)
{
    int             stat;
    MESS           *mess,
                   *nmess;
    struct CONNECT *conp;

    if ((unsigned int) conno >= NCONNS)
        return EBADF;
    conp = &connblo[conno];
    if (conp->blockstat != 1)
        return EBADF;
    conp->blockstat = 2;
/* if you call close, you don't want to read any more, so we release the
   possibly queued buffers
*/
    for (mess = conp->first; mess;) {
        nmess = mess->next;
        Nrelbuf(mess);
        mess = nmess;
    }
    WAITNOMORE(SIG_RC(conno));  /* wake up any suspended read */
/* call protocol stack close, return bits: */
/*              0 = do not free connection block */
/*              1 = protocol error has occurred */
    stat = conp->protoc[0]->closE(conno);
    if ((stat & 1) == 0) {
        conp->blockstat = 0;
        ussHostRelease(conp->confix);  /* Release host entry for reuse */
    }
    return stat & 2 ? ECONNABORTED : 0;
}

int             Nwrite(int conno, char *buff, int len)
{
    int             stat;
    unsigned long   ul1;
    MESS           *mp;
    struct NET     *netp;
    register struct CONNECT *conp;

    if ((unsigned int) conno >= NCONNS)
        return EBADF;
    conp = &connblo[conno];
    if (conp->blockstat != 1)
        return EBADF;
    if (conp->rxstat & S_FATAL)
        return ECONNABORTED;
    if (len + conp->doffset > MAXBUF)
        return EMSGSIZE;
    if ((mp = Ngetbuf()) == 0)
        return ENOBUFS;
    mp->offset = conp->doffset;
    Nmemcpy((char *) mp + conp->doffset, buff, len);
    mp->mlen = len + conp->doffset;
    mp->netno = conp->netno;
    mp->target = conp->heriid.l;
    netp = &nets[conp->netno];
    ul1 = conp->txstat & S_NOWA ? 0 : netp->tout;
    WAITFOR(((netp->depart.mtail - netp->depart.mhead) & 15) < MAXOUTQLEN,
            SIG_WN(conp->netno), ul1, stat);
    if (stat)
        stat = ETIMEDOUT;
    else
        stat = conp->protoc[0]->writE(conno, mp);
    if (stat)
        Nrelbuf(mp);
#if MT != 2
    YIELD();
#endif
    if (stat >= 0)
        stat = len;
    return stat;
}

int             Nread(int conno, char *buff, int len)
{
    int             i1,
                    mlen;
    MESS           *mp;
    struct CONNECT *conp;

    if ((unsigned int) conno >= NCONNS)
        return EBADF;
    conp = &connblo[conno];
    if (conp->blockstat != 1)
        return EBADF;
    mp = conp->protoc[0]->reaD(conno);
    if (mp == 0) {
        if (conp->rxstat & S_FATAL)
            return ECONNABORTED;
        if (conp->rxstat & S_EOF)
            return 0;
        if (conp->txstat & S_NOWA)
            return EWOULDBLOCK;
        else
            return ETIMEDOUT;
    }
    i1 = mp->offset;
    mlen = mp->mlen - i1;
    if (mlen < 0 || mlen > len)
        mlen = EMSGSIZE;
    else
        Nmemcpy(buff, (char *) mp + i1, mlen);
    Nrelbuf(mp);
    return mlen;
}


#define NETTASKDELAY 1000

/*
** * * * * * *
** NetTask()   Network interface handler
**
** void NetTask(int netno)      -- without a multitasker
** TASKFUNCTION NetTask(void)   -- with a multitasker
**
** PARAMETERS:
**   (in) int netno           Interface ID number
**
** DESCRIPTION:
**   This function takes the input data from the network interface driver
**   and passes it up the stack for a potential reading connection.  If
**   the packet needs to be queued in a connection read queue, it will do so.
**
**   In addition, it calls the TCP timeout() routine indirectly through the
**   PTABLE.shut() routine.  It will also call the IGMP timeout() routine
**   in the same manner.  PPP's state machine is driver by this task as
**   well through the pppTimeout(netno) routine.
**
** USAGE/COMMENTS:
**   In a multitasking application, this routine runs as a task over a single
**   interface.  A new instantiation is created for each interface
**   initialized.
**
**   In a non-multitasking environment, this routine is called from the
**   YIELD() and WAITFOR() macros.
**
**   The NETTASKDELAY macro definition above this comment specifies the
**   time in milliseconds between a forced check for queued data.  That
**   is the maximum amount of time (assuming there is an RTOS) that nettask
**   will wait to execute.
**
** * * * * * *
**
**  MODIFICATION HISTORY:
**
**  04-AUG-1999  BSK  Add: WAITNOMORE(SIG_GEN) for each iteration
**
** * * * * * *
*/
#if MT == 0
void            NetTask(int netno)
{
#else
TASKFUNCTION    NetTask(void)
{
    int             netno;
#endif
    int             i1,
                    conno;
    register struct CONNECT *conp;
    struct NET     *netp;
    MESS           *mp;
    PTABLE         *pp;
    struct NETCONF *confp;
    unsigned long   ul1;

#if MT != 0
    netno = gnetno;
    gflag = 1;
    WAITNOMORE(SIG_GEN);
#endif
    netp = &nets[netno];
#if MT == 0
    if (netp->netstat <= 0)
        return;
    netp->netstat = -1;
#else
aga:
#endif
    while (!CHECK_DRIVER_PACKET(netno)) {

/* take the message, call the screen function */

        TAKE_DRIVER_PACKET(netno, mp);
        pp = netp->protoc[0];
lab3:   i1 = pp->screen(mp);
        if (netp->fragmh) {
            mp = netp->fragmh;  /* we had a fragment, get the head */
            netp->fragmh = 0;
        }
        if (i1 < 0) {
            conno = mp->conno;  /* return negative */
            if (i1 != -4)       /* unless -4 free buffer */
                Nrelbuf(mp);
            if (i1 == -5) {     /* -5 means try future queue */
                conp = &connblo[conno];
                if (conp->blockstat == 1)
                    goto lab4;
            }
        } else {                /* return 0 or positive */
            mp->next = 0;       /* queue message for connection read */
            conp = &connblo[i1];
            if (conp->blockstat & 6) {
                Nrelbuf(mp);
                continue;
            }
            Ndisable();
            if (conp->first)
                conp->last->next = mp;
            else
                conp->first = mp;
            conp->last = mp;
            conp->ninque++;
            Nenable();
            WAITNOMORE(SIG_RC(i1));
    lab4:   if (conp->future) { /* try future queue */
                mp = conp->future;
                conp->future = mp->next;
                pp = conp->protoc[0];
                goto lab3;
            }
        }
    }

/* This loop calls the protocol timeout handler for each connection. */

#if MT
    netp->nettasktout = NETTASKDELAY;
#endif
    for (conno = 0; conno < NCONNS; conno++) {
        conp = &connblo[conno];
        if (conp->blockstat <= 0 || conp->netno != netno)
            continue;
        pp = conp->protoc[0];
        if (pp->shut)
            pp->shut(conno);
    }

#ifdef PPP
   /* Call the timeout handler for PPP links */
    if (netp->protoc[0] == PPP)
        pppTimeout(netno);
#endif
#if USS_IP_MC_LEVEL == 2
    ussIGMPTable.shut(0);
#endif

/* Here we stamp out ARP hardware addresses (Ethernet, ARCNET) that are too
   old.  Also we remove any messages waiting for an ARP reply that didn't
   arrive.
*/
#ifdef ARP
    ul1 = TimeMS();
    for (i1 = 0; i1 < NCONFIGS; i1++) {
        confp = &netconf[i1];
        if (confp->netno != netno || confp->ncstat == 0)
            continue;
#if ARPTOUT > 0
        if (confp->hwaddyes == 1 && (confp->flags & (LOCALHOST + NOTUSED)) == 0)
            if (ul1 - confp->timems > ARPTOUT)
                confp->hwaddyes = 0;
#endif
        if (confp->ARPwait && ul1 - confp->timems > NETTASKDELAY * 2) {
            confp->ARPwaitmp->offset = boTXDONE;
            if (confp->ARPwait != -5)
                Nrelbuf(confp->ARPwaitmp);
            confp->ARPwait = 0;
        }
    }
#endif

#ifdef FRAGMENTATION
/* Here we see if the top fragment queue buffer is so old that we'll
   give up on it. */

    if (netp->fragmq) {
        mp = netp->fragmq;
        if (TimeMS() - mp->timems > REASSTOUT) {
            netp->fragmq = mp->next;
/* if fragment 0 received, send ICMP message */
            if (*(unsigned short *) ((char *) mp + MESSH_SZ) >= Ihdr_SZ * 2)
                ICMPreply(mp, 11, 0);
            Nrelbuf(mp);
        }
    }
#endif

#if MT
    WAITNOMORE(SIG_GEN);
    WAITFOR((!QUEUE_EMPTY(netp, arrive)) || netp->worktodo, SIG_RN(netno),
            netp->nettasktout, i1);
    if (netp->netstat > 0) {
        netp->worktodo = 0;
        goto aga;
    }
    netp->netstat = -1;
#ifdef KILLTASK
    KILLTASK();
#endif
#else
    netp->netstat = 1;
#endif
}
/*****************************************************************
			ネットワークインターフェイスタスク生成
*****************************************************************/
int	CreNetTask( int netno )
{
	int i1;

	gnetno = netno;
	RUNTASK0(NetTask, NET_PRIOR);
}
/*****************************************************************
				ローカルＨＯＳＴ名登録処理
*****************************************************************/
void UsnetSetLocalHostname( char *buf )
{
	char i;
 	if( sizeof( buf ) >= HOSTNAME_LEN ) {
		return;
	}
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, localhostname ) ) ){
		    memset( (char*)&netdata[i].name, 0, HOSTNAME_LEN+DOMAIN_LEN );
		    memcpy( (char*)&netdata[i].name, buf, strlen(buf) );
		    memset( localhostname, 0, sizeof( localhostname ) );
			strcpy( localhostname, buf );
		}
	}
}
/*****************************************************************
				ローカルＨＯＳＴ名取得処理
*****************************************************************/
char *UsnetGetLocalHostname( void )
{
    return localhostname;
}
/*****************************************************************
			ローカルＨＯＳＴ　ＩＰアドレス登録処理
*****************************************************************/
void UsnetSetLocalIpAddress( char *buf )
{
	char i;
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, localhostname ) ) ){
		    memcpy( (char*)&netdata[i].Iaddr, buf, Iid_SZ);
		}
	}
}
/*****************************************************************
			ローカルＨＯＳＴ　ＩＰアドレス取得処理
*****************************************************************/
char *UsnetGetLocalIpAddress( void )
{
	char i;
	for (i = 0; i < NCONFIGS; i++) {
		if(!(strcmp( netconf[i].name, localhostname ) ) ){
		    return (char*)&netconf[i].Iaddr;
		}
	}
	return NULL;
}
/*****************************************************************
				リモートＨＯＳＴ名登録処理
*****************************************************************/
void UsnetSetRemoteHostname( char *buf )
{
	char i;
 	if( sizeof( buf ) >= HOSTNAME_LEN ) {
		return;
	}
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, remotehostname ) ) ){
		    memset( (char*)&netdata[i].name, 0, HOSTNAME_LEN+DOMAIN_LEN );
		    memcpy( (char*)&netdata[i].name, buf, strlen(buf) );
		    memset( remotehostname, 0, sizeof( remotehostname ) );
			strcpy( remotehostname, buf );
		}
	}
}
/*****************************************************************
				リモートＨＯＳＴ名取得処理
*****************************************************************/
char *UsnetGetRemoteHostname( void )
{
    return remotehostname;
}
/*****************************************************************
			リモートＨＯＳＴ　ＩＰアドレス登録処理
*****************************************************************/
void UsnetSetRemoteIpAddress( char *buf )
{
	char i;
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, remotehostname ) ) ){
		    memcpy( (char*)&netdata[i].Iaddr, buf, Iid_SZ);
		}
	}
}
/*****************************************************************
			リモートＨＯＳＴ　ＩＰアドレス取得処理
*****************************************************************/
char *UsnetGetRemoteIpAddress( void )
{
	char i;
	for (i = 0; i < NCONFIGS; i++) {
		if(!(strcmp( netconf[i].name, remotehostname ) ) ){
		    return (char*)&netconf[i].Iaddr;
		}
	}
	return NULL;
}
/*****************************************************************
			デフォルトゲートウェイＩＰアドレス登録処理
*****************************************************************/
void UsnetSetGwAddress( char *buf )
{
	char i;
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, "router" ) ) ){
		    memcpy( (char*)&netdata[i].Iaddr, buf, Iid_SZ);
		}
	}
}
/*****************************************************************
			デフォルトゲートウェイＩＰアドレス取得処理
*****************************************************************/
char *UsnetGetGwAddress( void )
{
	char i;
	for (i = 0; i < NCONFIGS; i++) {
		if(!(strcmp( netconf[i].name, "router" ) ) ){
		    return (char*)&netconf[i].Iaddr;
		}
	}
	return NULL;
}
/*****************************************************************
				ＤＮＳサーバーＩＰアドレス登録処理
*****************************************************************/
void UsnetSetDNSAddress( char *buf1, char *buf2 )
{
	char i;
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, "dnssvr1" ) ) ){
		    memcpy( (char*)&netdata[i].Iaddr, buf1, Iid_SZ);
		}
		if(!(strcmp( netdata[i].name, "dnssvr2" ) ) ){
		    memcpy( (char*)&netdata[i].Iaddr, buf2, Iid_SZ);
		}
	}
}
/*****************************************************************
			ＤＮＳサーバーＩＰアドレス取得処理
*****************************************************************/
void *UsnetGetDNSAddress( char *buf1, char *buf2 )
{
    memcpy( buf1, &DNSiid[0].c, Iid_SZ );
    memcpy( buf2, &DNSiid[1].c, Iid_SZ );
}
/*****************************************************************
					ドメイン名登録処理
*****************************************************************/
void UsnetSetDomainname( char *buf )
{
 	if( sizeof( buf ) >= DOMAIN_LEN ) {
		return;
	}
    memset( domain, 0, sizeof( domain ) );
    strcpy( domain, buf );
}
/*****************************************************************
					ドメイン名取得処理
*****************************************************************/
char *UsnetGetDomainname( void )
{
    return DNSdomain;
}
/*****************************************************************
					サブネットマスク登録処理
*****************************************************************/
void UsnetSetSubnetMask( char *buf )
{
	char i;
	for (i = 0; i < confsiz; i++) {
	    memcpy( (char*)&netdata[i].Imask, buf, Iid_SZ);
	}
}
/*****************************************************************
					サブネットマスク取得処理
*****************************************************************/
char *UsnetGetSubnetMask( void )
{
	char i;
	for (i = 0; i < NCONFIGS; i++) {
		if(!(strcmp( netconf[i].name, localhostname ) ) ) {
		    return (char*)&netconf[i].Imask;
		}
	}
	return NULL;
}
/*****************************************************************
					ＭＡＣアドレス登録処理
*****************************************************************/
void UsnetSetMacAddress( char *buf )
{
	char i;
	for (i = 0; i < confsiz; i++) {
		if(!(strcmp( netdata[i].name, localhostname ) ) ) {
		    memcpy( (char*)&netdata[i].Eaddr, buf, Eid_SZ);
		}
	}
}
/*****************************************************************
					ＭＡＣアドレス取得処理
*****************************************************************/
char *UsnetGetMacAddress( void )
{
	char i;
	for (i = 0; i < NCONFIGS; i++) {
		if(!(strcmp( netconf[i].name, localhostname ) ) ) {
		    return (char*)&netconf[i].Eaddr;
		}
	}
	return NULL;
}
/*****************************************************************
					ユーザー名登録処理
*****************************************************************/
void UsnetSetUserID( char *buf )
{
    memset( Usnet_UserID, 0, sizeof( Usnet_UserID ) );
    memcpy( Usnet_UserID, buf, sizeof( Usnet_UserID ) );
}
/*****************************************************************
					ユーザー名取得処理
*****************************************************************/
char *UsnetGetUserID( void )
{
	return Usnet_UserID;
}
/*****************************************************************
					パスワード登録処理
*****************************************************************/
void UsnetSetPassWord( char *buf )
{
    memset( Usnet_PassWd, 0, sizeof( Usnet_PassWd ) );
    memcpy( Usnet_PassWd, buf, sizeof(Usnet_PassWd) );
}
/*****************************************************************
					パスワード取得処理
*****************************************************************/
char *UsnetGetPassWord( void )
{
	return Usnet_PassWd;
}
/*****************************************************************
					ネットワークの初期化処理
戻り値： 1: 初期化成功
         0: 初期化失敗
*****************************************************************/
int	UsnetInit( void )
{
    int             i1;
    long            li1;
    unsigned long   ul1;
    {
    }

	/* 回線接続されるまで待つ */
	WaitLinkUp();

#if NTRACE >= 1
    Nprintf("UsnetInit - start \n");
#endif
    if ((i1 = Ninit()), i1 < 0) {
#if NTRACE >= 1
        Nprintf("! Ninit() failed: %d \n", i1);
#endif
        return 0;
    }

    if ((i1 = Portinit("*")), i1 < 0) {
#if NTRACE >= 1
        Nprintf("! Portinit('*') failed: %d", i1);
#endif
        goto err1;
    }
    return 1;
err1:
    Nterm();
    return 0;
}
/*****************************************************************
					ＤＨＣＰイネーブル処理
*****************************************************************/
void UsnetDHCPEnable( void )
{
    DHCP_flg = 1;
}
/*****************************************************************
					ＤＨＣＰディセーブル処理
*****************************************************************/
void UsnetDHCPDisable( void )
{
    DHCP_flg = 0;
}
/*****************************************************************
					ＤＨＣＰ許可状態取得処理
*****************************************************************/
char UsnetDHCPStates( void )
{
    return DHCP_flg;
}
/*****************************************************************
				ネットワークのシャットダウン処理
*****************************************************************/
int	UsnetTerm( void )
{
	Nterm();
}
/*****************************************************************
			ネットワークインターフェイスの初期化処理
*****************************************************************/
int	UsnetPortinit( char *port )
{
	Portinit( port );
}
