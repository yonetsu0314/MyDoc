/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/tcp.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: tcp.c $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  masahiro
 * èâä˙ÉäÉrÉWÉáÉì
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* TCP.C  Edition: 27  Last changed: 2-Dec-99,8:40:32  By: NICK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    Copyright (C) 1993, 1995-1999 By
    United States Software Corporation
    7175 N.W. Evergreen Parkway, Suite 100
    Hillsboro, Oregon 97124
    http://www.ussw.com
    (503) 844-6614

    TCP.C -- Transport Control Protocol for USNET

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

  02-DEC-1999  NJW  Flag reset connections so they are cleared properly
  12-NOV-1999  TTT  clearconn: clear incoming message queue
  09-NOV-1999  NJW  Corrected reset handling of incoming connection queue
  30-JUL-1999  TTT  timeout: adjust checksum if ackno changed on reTX
  30-JUN-1999  TTT  TCP Screen: remove window size from MSS calculation
  26-APR-1999  NJW  Added call to ussHostRelease() in timeout()
  26-APR-1999  TTT  Chg RST handling. clearconn(), screen() modified
  13-APR-1999  TTT  timeout(): use correct ackno in ReTX'ed messages
                      Also, set FATAL + NOACK if reTX limit reached.
  05-APR-1999  TTT  Change wackq processing for SYN in screen()
  11-MAR-1999  RCA  Leave TIME_WAIT if S_FATAL added to timeout()
  31-DEC-1998  RCA  Added support for ioctl entry in PTABLE
  09-NOV-1998  BSK  Corrected non-blocking behavior in active opeN().
  27-OCT-1998  RCA  Improvements to RTO calculation in writE(), screen() and
                      timeout().
  26-OCT-1998  TTT  Changed blockstat testing in writE() and timeout()
                      to test and clear only individual bit of flag.

* * * * * * * * * * * * * * * * * * * * * * * * *
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"

static long MSL = 120000UL;     /* Maximum Segment Lifetime */
static char MAXFUT = 5;         /* maximum messages in the future queue */
static long MAXCDEL = 7500;     /* maximum delay in close */
static long INIT_TXAVE = 0UL;   /* Initial value for smoothed RTT */
static long INIT_TXVAR = 3000UL;/* Initial value for mean RTT variation */
static long MINTXTOUT = 300UL;  /* minimum timeout ms for retransmission */
static long MAXTXTOUT = 60000UL;/* maximum timeout ms for transmit */
static long RXTOUT = 60000UL*4;/* maximum timeout ms for transmit */

static char RETX_THR_R2 = 12;   /* maximum number of retransmissions */
/* will handle cable break MAXTXTOUT/2 */
/* total timeout up to 2*MXTXTOUT */
/* delay before sending ACK */
static long ACKDELAY = 200;
static long ACKLIMIT = 3;       /* ACK forced after this many messages */

static char URGSTAND = 0;       /* urgent data, 0 = RFC 1122, 1 = 4.3BSD */
/* Note on above: 4.3BSD urgent data disagrees with the TCP/IP standard.
   As this is the biggest use of urgent data, we can't just ignore the
   error.  You can choose whether you want to comply with BSD or with the
   RFC.  You will be better off not using urgent data at all. */

static char NEGOTI = 0;         /* segment negotiation, 0 = local only, 1 =
                                 * always */
/* Note on above: If there are at least 2 hops, TCP alone can't determine
   the smallest segment size for the route.  There are route discovery
   protocols, but these are experimental, and not in USNET.  If the segment
   size is not know, value 536 is recommended.  If this kind of caution is
   clearly unnecessary, you can set this parameter to 1. */

#define IPOFFSET (MESSH_SZ+LHDRSZ)      /* IP header offset */
#define IP_TCP 6                /* protocol number */
#define MAXDAT 536              /* default TCP data, we must handle at least
                                 * this to be standard */
#if MAXBUF < 610
#error "TCP needs a MAXBUF (LOCAL.H) at least 590+MESSH_SZ"
#endif

struct Thdr {                   /* TCP header */
    unsigned short  myport;     /* source port */
    unsigned short  herport;    /* dest port */
    unsigned NLONG  seqno[4 / sizeof(NLONG)];   /* sequence number */
    unsigned NLONG  ackno[4 / sizeof(NLONG)];   /* acknowledgement number */
    unsigned char   hdrlen;     /* tcp header length in high 4 bits */
    unsigned char   flags;      /* message flags, see below */
    unsigned short  window;     /* window */
    unsigned short  chksum;     /* checksum */
    unsigned short  urgp;       /* urgent pointer */
};
#define Thdr_SZ 20              /* size of header */
#define ESTABLISHED   1         /* state machine states, see handbook */
#define FINWAIT_1     2
#define FINWAIT_2     3
#define CLOSED_WAIT   4
#define TIMEWAIT      5
#define LAST_ACK      6
#define CLOSED        7
#define SYN_SENT      8
#define SYN_RECEIVED  9
#define LISTEN       16
#define FIN 1                   /* message flag bits, see handbook */
#define SYN 2
#define RST 4
#define PSH 8
#define ACK 0x10
#define URG 0x20

extern struct NETCONF netconf[];
extern struct NET nets[];
extern int      Nclocktick;
extern PTABLE   ussTCPTable;


#ifdef MIB2
struct TCPgroup TCPgroup;
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  clearconn(): Clears pending messages associated with a connection.
**
**  static void clearconn(struct CONNECT * conp)
**
**  PARAMETERS:
**     (in/out)   conp  Pointer to connection block
**
**  RETURNS:
**     none
**
**  DESCRIPTION:
**     Internal routine to clear pending messages associated with
**     a connection. Clears both the wait-for-ack and the future
**     queue. Also resets the conp->wackf pointer and the conp->nwacks
**     counter.
**
**  USAGE/COMMENTS:
**     Called internally from TCP functions: closE(), timeout(), and
**     screen(). Connection block can be reused after calling this
**     function. Note that the buffer queue must be null terminated
**     for clearconn() to work -> this implies Edition 4 or higher
**     of support.c MUST be used.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  12-NOV-1999  TTT  conp->first (incoming message queue) cleared
**  26-APR-1999  TTT  conp->wackf, conp->nwacks, conp->future now cleared
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static void clearconn(struct CONNECT * conp)
{
    MESS *mp, *nmess;

    /* Clear future queue. Since buffer queue is null terminated,
    ** conp->future will point to null when cleared
    */
    while ( (mp = conp->future) != 0) {
        conp->future = mp->next;
        Nrelbuf(mp);
    }

    /* Clear wait-for-ack queue. Also, decrement count of messages
    ** in wack queue. conp->wackf should point to null when cleared
    */
    while ( (mp = conp->wackf) != 0) {
        conp->wackf = mp->next;
        conp->nwacks -= MAXWACK;
        mp->id = bALLOC;
        Nrelbuf(mp);
    }
    
    
    /* Clear incoming message queue. Decrement count of incoming
    ** messages in queue. 
    */    
    while ( (mp = conp->first) != 0) {
        conp->first = mp->next;
        conp->ninque--;
        Nrelbuf(mp);
    }    
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**  writE()     TCP write routine
**
**  static int writE(int aconno, MESS * mess)
**
**  PARAMETERS:
**     (in)     aconno   Connection number associated with the message buffer
**                         that is being written.  If a negative connection
**                         number is being used to pass additional information
**                         then the true connection number is in the message
**                         buffer.
**     (in)     mess     Message buffer that is being written
**
**  DESCRIPTION:
**     Creates a header.  If the host's window is too small, waits.  Adds the
**     message to ACK wait list.  Calls lower-level write.
**
**  RETURNS:
**     >0       number of characters sent
**     0        waiting for ACK
**     -1       Error
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**  27-OCT-1998  RCA  Set up new initialization values for retransmission
**                      timeout parameters so that the initial RTO is 3 seconds
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int writE(int aconno, MESS * mess)
{
    int i1, conno, tlen, hlen, hdroff, probe;
    long leftout;
    short *sp;
    union {
        unsigned char c[4];
        short s[2];
        long l;
    } UL1, UL2;
    struct NET *netp;
    MESS *mp;
    register struct CONNECT *conp;
    register struct Thdr *thdrp;
    struct {
        short s[2];
        unsigned long Iadd1, Iadd2;
    } pseudo;
#define pseudo_SZ 12

   /* preparations, check of previous error */
    conno = aconno < 0 ? mess->conno : aconno;
    conp = &connblo[conno];
    netp = &nets[conp->netno];
    hlen = Thdr_SZ;
    mess->offset = hdroff =
#ifdef IPOPTIONS
        conp->IPOtxlen +
#endif
        IPOFFSET + Ihdr_SZ;

sendnext:
   /* If high bit of mlen set, it's a control message, size 0.  Control bits
    ** in txstat can also define a control message. */
    thdrp = (struct Thdr *) ((char *) mess + hdroff);
    thdrp->urgp = 0;
    if (mess->mlen & 0x8000) {
        thdrp->flags = (unsigned char) mess->mlen;
        mess->mlen = hdroff + Thdr_SZ;
        tlen = mess->mlen - hdroff - hlen;
    } else {
        tlen = mess->mlen - hdroff - hlen;
        thdrp->flags = ACK;
        if ((UL1.c[0] = conp->txstat & (S_PSH | S_URG | S_FIN)) != 0) {
            thdrp->flags |= UL1.c[0];
            conp->txstat &= ~UL1.c[0];
            if (UL1.c[0] & S_FIN)
                conp->state = conp->rxstat & S_EOF ? LAST_ACK : FINWAIT_1;
            if (UL1.c[0] & S_URG)
                thdrp->urgp = NC2(tlen + URGSTAND - 1);
        }
    }

   /* if SYN, initialize some fields, put in maximum block size */
    if (thdrp->flags & SYN) {
        conp->txseq = TimeMS(); /* "random" initial seq */
        hlen += 4;
        mess->mlen = hdroff + Thdr_SZ + 4;
        sp = (short *) ((char *) mess + hdroff + Thdr_SZ);
		if ( NEGOTI == 0 ) {
			i1 = MAXDAT;
			if (netconf[conp->confix].hops <= 2) {    /* if at most 2 hops */
				i1 = netp->maxblo - Ihdr_SZ - Thdr_SZ;
			}
		}
		else {
       		i1 = netp->maxblo - Ihdr_SZ - Thdr_SZ;
		}
		conp->mywindow = i1 * MAXWIND;
        *sp++ = NC2(0x0204), *sp = NC2(i1);

       /* Per RFC1122, set up smoothed round trip time to 0 (already done
        ** with memset() when structure allocated), mean variation to a
        ** value that results in an initial retransmission timeout of 3
        ** seconds. */
        conp->txvar = INIT_TXVAR;
        conp->txtout = (INIT_TXAVE >> 3) + INIT_TXVAR;

       /* calculate fixed part of the pseudo-checksum for efficiency */
        pseudo.s[1] = NC2(IP_TCP);
        pseudo.s[0] = conp->myport;
        pseudo.Iadd1 = conp->heriid.l;
        pseudo.Iadd2 = netconf[netp->confix].Iaddr.l;
        conp->pseudosum = Nchksum((unsigned short *) &pseudo, pseudo_SZ >> 1);
    }
   /* If this is a data message and the flow control window is shut, or there
    ** are too many messages in the wait-for-ACK queue, we need to wait. */
    probe = 0;
    if (tlen) {
        UL2.l = conp->txstat & S_NOWA ? 0 : netp->tout * 8;
aga6:
       /* slow start & congestion avoidance */
        WAITFOR(conp->nwacks <= conp->wackmax, SIG_CC(conno), UL2.l, i1);
        if (i1) {
            if (conp->rxstat & S_FATAL)
                return ECONNABORTED;
            if (conp->txstat & S_NOWA)
                return ETIMEDOUT;
            goto aga6;
        }
       /* wait for window to open */
        UL1.l = conp->txseq + tlen;
        WAITFOR((leftout = (long) (UL1.l - conp->ackno - conp->window)) <= 0,
                SIG_CC(conno), UL2.l, i1);
        if (i1) {
           /* did not, NOWAIT mode returns */
            if (conp->txstat & S_NOWA)
                return ETIMEDOUT;
           /* Now we need to split the data and send so that the window
            ** becomes 0. If the window already is 0, we send 1 byte. */
            tlen -= (int) leftout;
            probe = -1;
            if (tlen <= 0) {
                leftout = tlen + leftout - 1;
                tlen = 1;
                probe = 1;
            }
            mp = mess;
            if ((mess = Ngetbuf()) == 0)
                return ENOBUFS;
            memcpy((char *) mess, (char *) mp, mp->mlen);
            memcpy((char *) mp + hdroff + hlen,
                 (char *) mess + mess->mlen - (int) leftout, (int) leftout);
            mess->mlen -= (int) leftout;
            mp->mlen = hdroff + hlen + (int) leftout;
            thdrp = (struct Thdr *) ((char *) mess + hdroff);
        }
    }
   /* put in header length, port numbers */
    thdrp->hdrlen = (hlen << 4) >> 2;
    thdrp->herport = conp->herport;

   /* protocol code for IP */
    *((char *) mess + IPOFFSET + 9) = IP_TCP;

   /* put in sequence numbers, window size, finish the checksum */
    UL1.l = conp->txseq;
#ifdef KEEPALIVETIME
   /* The keepalive feature sends a bad sequence number to force response. */
    if (conp->sendack == 100)
        UL1.l--;
#endif
    PUTLONG(UL1, thdrp->seqno);
    conp->ackdseq = UL1.l = conp->seqtoack;
    PUTLONG(UL1, thdrp->ackno);
    conp->sendack = 0;
    thdrp->window = NC2(conp->mywindow);
    conp->prevwindow = conp->mywindow;
    i1 = tlen + hlen;
    if (i1 & 1)
        *((char *) thdrp + i1) = 0;
    thdrp->myport = NC2(i1);
    thdrp->chksum = conp->pseudosum;
    thdrp->chksum = ~Nchksum((unsigned short *) thdrp, (i1 + 1) / 2);
    thdrp->myport = conp->myport;

#if NTRACE >= 2
    GETLONG(UL1, thdrp->seqno);
    GETLONG(UL2, thdrp->ackno);
    Nprintf("TX %lu C%d/%x ST%d DL%d W%u/%u SQ%lx AK%lx %x\n",
            TimeMS(), conno, conp->myport, conp->state, tlen,
            conp->mywindow, conp->window, UL1.l, UL2.l, thdrp->flags);
#endif

   /* FIN or SYN counts as 1 in the sequence number. */
    if (thdrp->flags & (FIN | SYN))
        tlen++;

   /* We set retry count, timeout, add message to "wait for ack" queue */

    if (tlen) {
        mess->conno = 0;        /* used as retry count */
#if NTRACE > 0
        if (mess->id != bALLOC)
            Npanic("wack q");
#endif
        mess->id = bWACK;
        mess->next = 0;
        Ndisable();

        if (conp->wackf)
            conp->wackl->next = mess;
        else
            conp->wackf = mess;
        conp->wackl = mess;
        conp->nwacks += MAXWACK;
        Nenable();
        conp->txseq += tlen;    /* bump TX sequence number */
        mess->timems = TimeMS();/* time stamp for retransmission */
    }
#ifdef MIB2
    TCPgroup.tcpOutSegs++;
#endif
   /* call low-level transmit */
    i1 = conp->protoc[1]->writE(aconno, mess);
    if (tlen) {
       /* If we could only send a part of the message because the window was
        ** too small, we go back to try the rest.  If we sent a 1-byte probe,
        ** we wait until it's been ACK'd, as long as the remote host seems
        ** alive. */
        if (probe) {
            if (probe > 0) {

                WAITFOR(conp->window || (conp->rxstat & S_FATAL),
                        SIG_CC(conno), 0xffffffff, probe);
                if (conp->rxstat & S_FATAL)
                    return ETIMEDOUT;
                conp->blockstat |= 0x40;
                WAITFOR(conp->nwacks == 0, SIG_CC(conno), conp->txtout, probe);
                conp->blockstat &= 0xbf;
            }
            mess = mp;
            if ((int) leftout)
                goto sendnext;
            return 1;
        }
        return 0;
    }
    return i1;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  screen()     TCP screening function
**
**  static int screen(MESS * mess)
**
**  PARAMETERS:
**
**     (in)     mess     Message buffer that is being screened
**
**  DESCRIPTION:
**     Checks if the coming message is for this host.  Searches the connection
**     tables for one that is open for the right port.
**
**  RETURNS:
**     n        please enter in queue number n (starts at 0)
**    -1        message rejected.  SYN arrives on listening connection, no
**                  connection available.
**    -2        processed, no further action, free message
**    -4        do not release
**    -5        processed, check future queue
**    -6        connection not found
**
**  USAGE/COMMENTS:
**     Called from lower layer screen() function, typically IP screen().
**     Ultimately returns to screen() call in NetTask, where the returned
**     connection number is used to enter the message buffer in the correct
**     queue.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-DEC-1999  NJW  Flag ESTABLISHED connblo[] to be cleared when reset
**  09-NOV-1999  NJW  Remove reset connections from incoming conn. queue
**  30-JUN-1999  TTT  Remove window size from conp->maxdat calculation
**  26-APR-1999  TTT  Change RST handling for SYN_RECEIVED, SYN_SENT states
**                    Added S_PASSIVE state to signal passive open.
**  05-APR-1999  TTT  Check if message is SYN before removing from wackq.
**                      Only remove if incoming ACK also has SYN bit set.
**  27-OCT-1998  RCA  Moved the ACK processing so that it takes place before
**                      the sequence number check.  This avoids a problem where
**                      the ACK information in an above sequence segments would
**                      not be recognized until the intervening segment was
**                      received.
**  27-OCT-1998  RCA  Modified the retransmission timeout calculation to use
**                      the formula RTO = R + 4V, per the updated version of
**                      Van Jacobson's "Congestion Avoidance and Control"
**                      paper.  The previous formula was RTO = R + 2V.
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int screen(MESS * mess)
{
    int i1, stat, netno, conno, portno, herport, tlen, hlen;
    int match;
    int bestcount;
    int wildcount;
    union {
        char c[4];
        short s[2];
        unsigned long l;
    } UL1, UL2, UL3;
    unsigned short us1;
    char cflg, *cp, *cp2;
    register struct Thdr *thdrp, *thdrp2;
    register struct CONNECT *conp;
    struct CONNECT *conp2;
    MESS *mp, *mp2;
    struct {
        unsigned short s[2];
        unsigned long Iadd1, Iadd2;
    } pseudo;

#ifdef MIB2
    TCPgroup.tcpInSegs++;
#endif

   /* Nothing should be done for a message with a bad checksum, so we
    ** calculate it here, before anything else */
    thdrp = (struct Thdr *) ((char *) mess + mess->offset);
    tlen = mess->mlen - mess->offset;
    if (tlen & 1)
        *((char *) thdrp + tlen) = 0;
    us1 = ~thdrp->chksum;
    pseudo.s[1] = NC2(IP_TCP);
    pseudo.s[0] = NC2(tlen);
    memcpy((char *) &pseudo.Iadd1, (char *) mess + mess->conno, 2 * Iid_SZ);
    thdrp->chksum = Nchksum((unsigned short *) &pseudo, pseudo_SZ >> 1);
    if ((Nchksum((unsigned short *) thdrp, (tlen + 1) / 2)) != us1) {
#ifdef MIB2
        TCPgroup.tcpInErrs++;
#endif
        goto err1;
    }
    thdrp->chksum = ~us1;

   /* Scan the connection blocks.
    **  1.  The block must be allocated, and in TCP.
    **  2.  Local port number in message == number in connection block.
    * The remote port and remote address may be wildcarded, but the best
    ** match will be accepted.  Wildcards are accepted on connections in
    ** the LISTEN state, which are indicated with a broadcast remote IP
    ** address and 0 for the remote port number. */
    netno = mess->netno;
    portno = thdrp->herport;
    herport = thdrp->myport;
    hlen = (thdrp->hdrlen >> 4) << 2;
    tlen -= hlen;
    GETLONG(UL1, thdrp->seqno);
    GETLONG(UL2, thdrp->ackno);

    bestcount = 3;
    for (conno = 0; conno < NCONNS; conno++) {
        conp = &connblo[conno];
        wildcount = 0;
        if ((conp->blockstat == 0) ||
            (conp->protoc[0] != &ussTCPTable) ||
            (conp->myport != portno))
            continue;
        if (conp->herport != herport) {
            if (conp->herport != 0)
                continue;
            else
                wildcount++;
        }
        if (conp->heriid.l != mess->target) {
            if ((conp->heriid.l != 0xffffffff) &&
                (conp->heriid.l !=
                   mess->target | ~netconf[mess->confix].Imask.l))
                continue;
            else
                wildcount++;
        }
        if (wildcount < bestcount) {
            match = conno;
            bestcount = wildcount;
            if (bestcount == 0)
                break;
        }
    }

    if (bestcount == 3) {
       /* Send RST.  To avoid RST storms, not in response to RST. */
        if (thdrp->flags & RST)
            return -1;
        if (thdrp->flags & (FIN | SYN))
            tlen++;
        if ((thdrp->flags & ACK) == 0)
            UL2.l = 0;
        thdrp->hdrlen = Thdr_SZ << 2;
        thdrp->flags = RST + ACK;
        PUTLONG(UL2, thdrp->seqno);
        UL1.l += tlen;
        PUTLONG(UL1, thdrp->ackno);
        thdrp->myport = portno;
        thdrp->herport = herport;
        mess->mlen = mess->offset + Thdr_SZ;
        pseudo.s[0] = NC2(Thdr_SZ);
        thdrp->chksum = Nchksum((unsigned short *) &pseudo, pseudo_SZ >> 1);
        thdrp->chksum = ~Nchksum((unsigned short *) thdrp, Thdr_SZ / 2);
#if NTRACE >=1
        Nprintf("RST %lu to %d P%x/%x C%d\n", TimeMS(), mess->confix,
                portno, herport, conno);
#endif
#ifdef MIB2
        TCPgroup.tcpOutSegs++;
        TCPgroup.tcpOutRsts++;
#endif
        return -3;
    }

   /* The incoming segment matches an existing connection.  Update conno and
    ** conp from the matching connection */
    conno = match;
    conp = &connblo[conno];

   /* We handle RST here so we don't have to check for it anymore.
    **  State         Action
    ** SYN_RECEIVED  go back to LISTEN if came from there OR
    **               mark as FATAL if came from SYN_SENT
    **               In both cases, clear the retransmission queue.
    ** SYN_SENT      If ACK with RST, mark as FATAL, otherwise ignore
    ** ESTABLISHED   get a fatal error.
    */
    if (thdrp->flags & RST) {
        if (conp->state == SYN_SENT) {
            if (thdrp->flags & ACK)
                conp->rxstat |= S_FATAL;
            goto ret2;
        }
        if (UL1.l - conp->rxseq > conp->mywindow)
            goto ret2;
#if NTRACE >=1
        Nprintf("RST %lu from %d P%x/%x C%d\n", TimeMS(), mess->confix,
                portno, herport,conno);
#endif
        if (conp->state >= SYN_RECEIVED) {
            /* Clear retranmission queue */
            clearconn(conp);
            /*  For passive open, return to LISTEN state. Clear
            **  the remote port and address back to initial
            **  values for passive open so the next SYN
            **  will be accepted
            */
            if (conp->txstat & S_PASSIVE) {
            
                /*
                **  Check to see if there is a backlog.  If backlog
                **  is greater than 0, then this connection block
                **  was copied from a listening connection and needs
                **  freed and delinked from the incoming connection
                **  queue
                */
                if ( conp->backlog ) {             /* Check if copied */
                    BLOCKPREE();                   /*  Protect queue */
                    connblo[conp->icqcur].icqcur--;/*  Dec count */
                    RESUMEPREE();                  /*  Resume tasking */
                    conp->blockstat=0;             /*  Free block */
                } else {                           /* Not copied */
                    conp->herport = 0;             /*  Zero remote port */
                    conp->heriid.l = 0xffffffff;   /*  Reset remote addr */
                    conp->state = LISTEN;          /*  Move to LISTEN state */
                }  
            }
            /*  If we came to SYN_RECEIVED from an active open,
            **  mark connection as FATAL
            */
            else
                conp->rxstat |= (S_FATAL + S_RST);

        } else {   /*  Connect is not in LISTEN or SYN_RECEIVED state */        

            /*
            **  If in the ESTABLISHED state and this connection block
            **  was copied from a listening socket as idetnified by
            **  bakclog > 0, then check to see if it is still in the
            **  incoming connection queue.  If so, remove it.
            */
            if ( conp->backlog && conp->state==ESTABLISHED) {
               BLOCKPREE();                                  
               
               /* Point to listening socket queue */
               conp2 = &connblo[conp->icqcur];             
               
               /* Find connection if incoming queue */
               while ( conp2->next && conp2->next != conp)      
                 conp2 = conp2->next;
                                            
               /* If in queue, remove it */
               if ( conp2->next != 0 && conp2->next == conp ) { 
                 conp2->next = conp->next;       /* Delink entry */
                 connblo[conp->icqcur].icqcur--; /* Decreament count */           
                 conp->next = 0;                 /* NULL out next pointer */
                 conp->blockstat |= 4;           /* Flag to be cleared */
                 /* TCP.timeout() will clear the connection block */
               }                                                
               RESUMEPREE();                                    
            }                                                   
            conp->rxstat |= (S_FATAL + S_RST);   /* Mark as Reset and Fatal */
        }
        WAITNOMORE(SIG_RC(conno));
        WAITNOMORE(SIG_CC(conno));
ret2:
        return -2;
    }

lab1:
   /* get variables from the header */
    us1 = NC2(thdrp->window);
    conp->lastrxtime = TimeMS();
#if NTRACE >= 3
    Nprintf("SC %lu C%d/%x ST%d DL%d W%u/%u SQ%lx AK%lx %x\n",
            conp->lastrxtime, conno, conp->myport, conp->state, tlen,
            conp->mywindow, us1, UL1.l, UL2.l, thdrp->flags);
#endif

   /* if this is an ACK, we remove from the "wait for ack" queue all messages
    ** up to this ACK sequence number.  The connection block ackno is updated
    ** only when the ACK is actually used.  This ensures that out-of-order
    ** ACK's will not leave ackno to an old value. */
    if (thdrp->flags & ACK) {
        cflg = 0;
        while ((mp = conp->wackf) != 0) {
            thdrp2 = (struct Thdr *) ((char *) mp +
                     (((*((char *) mp + IPOFFSET)) & 0xf) << 2) + IPOFFSET);
            GETLONG(UL3, thdrp2->seqno);
            UL3.l = UL2.l - UL3.l;
            if (UL3.l >= 8 * MAXBUF || UL3.l == 0) {
               /* Any reasonable ACK will refresh the timeout so as long as
                ** the other host keeps ACK'ing, we keep sending. */
                if (conp->txtout > MAXTXTOUT / 4)
                    conp->txtout = MAXTXTOUT / 4;
                break;
            }
#if NTRACE > 0
            if (mp->id != bWACK)
                Npanic("wack rem");
#endif

           /* Adjust timeout if there were no retransmissions.  We use the
            ** updated Van Jacobson formula from "Congestion Avoidance and
            ** Control", which uses rto = R + 4V.  ALL messages in the queue
            ** after one that timed out are discarded before timeout
            ** recalculation.  If there was 1 retransmission, we make no
            ** changes to timeout.  If there were more than 1, we take the
            ** actually used txtout as the new txave. */
            if ((cflg | mp->conno) == 0) {
                UL3.l = conp->lastrxtime - mp->timems + Nclocktick;
                if (UL3.l > MAXTXTOUT)
                    goto lab7;
                UL3.l -= conp->txave / 8;
                conp->txave += UL3.l;
                if ((long) conp->txave < 0)
                    conp->txave = 40;
                if ((long) UL3.l < 0)
                    UL3.l = -UL3.l;
                UL3.l -= conp->txvar / 4;
                conp->txvar += UL3.l;
                if (conp->txvar < 10)
                    conp->txvar = 10;
                conp->txtout = (conp->txave / 8) + conp->txvar;
                if (conp->txtout < MINTXTOUT)
                    conp->txtout = MINTXTOUT;
               /* Slow start & congestion avoidance: we allow more messages
                ** in the window, if we are not at the limit yet. */
                if (conp->wackmax < (MAXWACK - 1) * MAXWACK) {
                    if (conp->wackmax < conp->wackslow)
                        conp->wackmax += MAXWACK;
                    else
                        conp->wackmax++;
                }
            } else {
                cflg = 1;
                if (mp->conno > 1 && conp->state == ESTABLISHED) {
                    conp->txave = conp->txtout << 2;
                    conp->txvar = conp->txtout;
                   /* Slow start & congestion avoidance: we force a slow
                    ** start, and recalculate the congestion avoidance
                    ** threshold.  ONLY do this when txtout is actually
                    ** adjusted, otherwise you may end up in permanent
                    ** slow start. */
                    conp->wackslow = conp->wackmax / 2;
                    if (conp->wackslow < MAXWACK)
                        conp->wackslow = MAXWACK;
                    conp->wackmax = MAXWACK;
                }
            }
lab7:
            /* If wackq message is a SYN, don't remove it unless
            ** the received ACK also has the SYN bit set. This is
            ** useful in half-open connection scenarios
            */
            if (thdrp2->flags & SYN) {
                if (conp->state == SYN_SENT && !(thdrp->flags & SYN))
                    break;
            }
            conp->wackf = mp->next;
            conp->nwacks -= MAXWACK;
            if (thdrp2->flags & FIN) {
                if (conp->state == FINWAIT_1)
                    conp->state = FINWAIT_2;
                else if (conp->state == LAST_ACK)
                    conp->state = CLOSED;
            }
            Ndisable();
            if (mp->offset == boTXDONE) {
                mp->id = bALLOC;
                NrelbufIR(mp);
            } else
                mp->id = bRELEASE;
            Nenable();
            conp->ackno = UL2.l;
        }
    }

    /* 
    ** If not SYN, check sequence number.  If not right, we still accept
    ** "future" messages that are slightly out of order, we enter them into
    ** the future queue. 
    ** conp->rxseq holds the sequence number of the last message
    ** screened; Ul1.l is the sequence number of the current 
    ** message we are screening. These are compared to determine if the 
    ** message is in sequence or, if not, how far out of order is it? 
    */
    if (!(thdrp->flags & SYN) && conp->state != SYN_SENT) {
        if (conp->rxseq - UL1.l < MAXBUF && UL1.l + tlen - conp->rxseq < MAXBUF) {
            /* Is message out of order? */
            if ((i1 = conp->rxseq - UL1.l) > 0) {       
                /* remove duplicated bytes */
                cp = (char *) thdrp + hlen;
                cp2 = cp + i1;
                mess->mlen -= i1;
                UL1.l += i1;
                PUTLONG(UL1, thdrp->seqno);
                i1 = tlen = tlen - i1;
                if (tlen <= 0)
                    goto err2;
                while (i1--)
                    *cp++ = *cp2++;
            }
            /* Message is in correct order; process it */
            goto inseq;
        }
        /* 
        ** If message is out of order, how far out of order is it?
        ** If the last and current sequence numbers are more than 8
        ** buffer lengths apart, this is too far out of order; return 
        ** an error and drop the message.
        */
        if (conp->rxseq - UL1.l < 8 * MAXBUF)
            goto err2;
        if (UL1.l - conp->rxseq >= 8 * MAXBUF)
            goto err1;
        /* Message is out of order, but only slightly; enter it into
        ** the future queue
        */            
        mp2 = (MESS *) & conp->future;
        for (i1 = 0, mp = mp2->next; mp; mp2 = mp, mp = mp->next, i1++) {
            thdrp2 = (struct Thdr *) ((char *) mp + mp->offset);
            GETLONG(UL3, thdrp2->seqno);
            if (UL3.l == UL1.l && mp->mlen == mess->mlen)
                goto err1;
            if (UL3.l > UL1.l)
                break;
        }
        if (i1 >= MAXFUT) {
#if NTRACE >= 3
            Nprintf("FQ discard\n");
#endif
            goto err1;
        }
#if NTRACE >= 3
        Nprintf("FQ queued\n");
#endif
        mess->next = mp;
        mp2->next = mess;
        if (!mp)                /* no window update unless last */
            conp->window = us1;
        goto err4;
    }
inseq:
    conp->window = us1;         /* update window in proper order */

   /* here handle the control options, in practice maximum block size */
    for (cp = (char *) mess + mess->offset + Thdr_SZ;
         cp < (char *) mess + mess->offset + hlen;) {
        switch (cp[0]) {
        case 1:                 /* no operation */
            cp++;
            break;
        case 2:                 /* block size */
            us1 = ((unsigned char *) cp)[3] | (((unsigned char *) cp)[2] << 8);
            stat = nets[netno].maxblo -
#ifdef IPOPTIONS
                conp->IPOtxlen -
#endif
                Ihdr_SZ - Thdr_SZ;
            conp->maxdat = us1 <= stat ? us1 : stat;
        default:
            if (cp[1] <= 0)
                goto lab2;
            cp += cp[1];
            break;
        }
    }

lab2:
   /* process message depending on the state of the connection */
    switch (stat = conp->state) {
    case SYN_RECEIVED:
        if ((thdrp->flags & ACK) == 0)
            goto err1;
        conp->state = ESTABLISHED;

       /* If backlog is set, this connection is copied from a listening
        ** connection, and icqcur is the index of the listening connection.
        ** Add connection to the incoming connection queue for the listening
        ** connection and send signal through listening connection. */
        if (conp->backlog) {
            conp2 = &connblo[conp->icqcur];
            BLOCKPREE();
            while (conp2->next) { /* add new conn to end of q */
                conp2 = conp2->next;
            }
            conp2->next = conp;
            conp->next = 0;
            RESUMEPREE();
            WAITNOMORE(SIG_CC(conp2 - &connblo[0]));
        }

#ifdef MIB2
        TCPgroup.tcpCurrEstab++;
#endif
    case ESTABLISHED:
    case FINWAIT_1:
    case FINWAIT_2:
        if (thdrp->flags & SYN)
            goto err2;
        if ((i1 = tlen + (thdrp->flags & FIN)) == 0)
            break;

       /* urgent flag set here, even if window is closed */
        if (thdrp->flags & S_URG) {
            conp->urgcnt = NC2(thdrp->urgp) + URGSTAND;
            conp->urgseq = UL1.l;
            conp->urgflag = 1;
        }
       /* Window made smaller here.  If the window would become negative,
        ** we discard the message but send an ACK. */
        if (conp->mywindow < tlen)
            goto err2;
        conp->mywindow -= tlen;

       /* Here we need to request ack, this is always delayed because we
        ** expect the read to be done soon. */
        conp->seqtoack = conp->rxseq = UL1.l + i1;
        if (conp->sendack == 0) {
            conp->acktime = conp->lastrxtime;
            conp->sendack = 1;
        }
        if (thdrp->flags & FIN) {
            conp->rxstat |= S_EOF;
            conp->sendack = ACKLIMIT;
            if (stat == FINWAIT_2)
                conp->state = TIMEWAIT;
            else if (stat == FINWAIT_1)
                conp->state = LAST_ACK;
            WAITNOMORE(SIG_CC(conno));
            WAITNOMORE(SIG_RC(conno));
            if (tlen == 0)
                goto retm5;
        }
       /* We combine any messages that arrive too quickly to read at once. */
        if (tlen && conp->first) {
            mp = conp->last;
            if (mp->mlen + tlen <= MAXBUF) {
                thdrp2 = (struct Thdr *) ((char *) mp + mp->offset);
                memcpy((char *) mp + mp->mlen, (char *) thdrp + hlen, tlen);
                mp->mlen += tlen;
                thdrp2->flags |= thdrp->flags & S_PSH;
retm5:
                mess->conno = conno;
                return -5;
            }
        }
        break;
    case CLOSED_WAIT:
        conp->state = CLOSED;
        break;
    case LISTEN:                /* this is passive open finding a partner */
       /* send RST if we got something else than SYN with possible PSH */
        if ((thdrp->flags & ~PSH) != SYN)
            goto sendrst;
        if (conp->backlog) {
            if (conp->icqcur == conp->backlog)
                return -1;
            BLOCKPREE();
            for (conno = 0; conno < NCONNS; conno++) {
                conp2 = &connblo[conno];
                if (!conp2->blockstat) {
                    *conp2 = *conp;    /* copy listening conn to new */
                    conp->icqcur++;    /* incr incoming conn count */
                    conp2->icqcur = conp - &connblo[0]; /* save sock */
                    conp = conp2;      /* leave pointing to new conn */
                    break;
                }
            }
            RESUMEPREE();
            if (conno == NCONNS)
                return -1;
        }
       /* set up connection block */
        conp->txseq = conp->lastrxtime; /* "random" initial seq */
        conp->seqtoack = conp->rxseq = UL1.l + 1;
        conp->state = SYN_RECEIVED;
        conp->txstat |= S_PASSIVE;
#ifdef MIB2
        TCPgroup.tcpPassiveOpens++;
#endif
        conp->herport = thdrp->myport;
        conp->realiid.l = conp->heriid.l = pseudo.Iadd1;
        conp->netno = netno;
        conp->confix = mess->confix;
       /* respond with SYN+ACK, do not release as this goes into the
        ** wack queue */
        mess->mlen = 0x8000 | SYN | ACK;
        mess->conno = conno;
        writE(-5, mess);
        return -4;
    case SYN_SENT:
        if (thdrp->flags & ACK) {
            conp->txseq = UL2.l;
            if (thdrp->flags & SYN) {
                conp->seqtoack = conp->rxseq = UL1.l + 1;
                conp->state = ESTABLISHED;
                conp->realiid.l = pseudo.Iadd1;
#ifdef MIB2
                TCPgroup.tcpCurrEstab++;
#endif
                mess->mlen = 0x8000 | ACK;
            } else {
sendrst:
#ifdef MIB2
                TCPgroup.tcpOutRsts++;
#endif
                mess->mlen = 0x8000 | RST;
            }
            mess->conno = conno;
            mess->id = bRELEASE;
            if (writE(-6, mess))
                Nrelbuf(mess);
            WAITNOMORE(SIG_CC(conno));
            goto err4;
        } else if (thdrp->flags & SYN) {
            conp->seqtoack = conp->rxseq = UL1.l + 1;
            mess->mlen = 0x8000 | ACK | SYN;
            mess->conno = conno;
            writE(-5, mess);
            conp->state = SYN_RECEIVED;
            goto err4;
        }
        break;
    }
   /* Signaling here may or may not be necessary, but should do no harm. */
    WAITNOMORE(SIG_CC(conno));
    return tlen ? conno : -2;
err2:
    conp->sendack = ACKLIMIT;
err1:
    return -1;
err4:
    return -4;
}


/* ===========================================================================
   Read routine.  Calls the lower level read.  If successfull, checks the
   header.  If good, gives the caller the message.  Returns:
        success: message address
        error:   0
*/
static MESS *reaD(int conno)
{
    int hdroff, len;
    union {
        char c[4];
        short s[2];
        long l;
    } UL1, UL2;
    MESS *mp;
    register struct Thdr *thdrp;
    register struct CONNECT *conp;

    conp = &connblo[conno];

   /* call low-level read */
    mp = conp->protoc[1]->reaD(conno);
    if (mp == 0)
        return 0;

   /* get some variables */
    hdroff = mp->offset;
    thdrp = (struct Thdr *) ((char *) mp + hdroff);
    conp->herport = thdrp->myport;
    mp->offset = hdroff + ((thdrp->hdrlen >> 4) << 2);
    len = mp->mlen - mp->offset;
    conp->tcpflags = thdrp->flags;
    GETLONG(UL1, thdrp->seqno);
   /* set URG variables for application */
    if (conp->urgflag == 1) {
        hdroff = conp->urgseq - UL1.l;
        if (hdroff >= 0 && hdroff < len) {
            conp->tcpflags |= S_URG;
            conp->urgcnt += hdroff;
            conp->urgdata = *((char *) mp + conp->doffset + len - 1);
        }
    }
   /* We'll ask for an ACK, immediate if there's a lot of data to ACK,
    ** delayed otherwise. */
    Ndisable();
    conp->mywindow += len;
    Nenable();
    if (conp->first == 0 && (conp->seqtoack - conp->ackdseq > conp->maxdat ||
                         conp->mywindow - conp->prevwindow >= conp->maxdat))
        conp->sendack = ACKLIMIT;
    else
        conp->sendack++;
    nets[mp->netno].worktodo = 1;
    WAITNOMORE(SIG_RN(mp->netno));
#if NTRACE >= 2
    GETLONG(UL2, thdrp->ackno);
    Nprintf("RX %lu C%d/%x ST%d DL%d SQ%lx AK%lx %x\n", TimeMS(), conno,
            conp->myport, conp->state, len, UL1.l, UL2.l, thdrp->flags);
#endif
    return mp;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** opeN()  Open a TCP connection either passively or actively
**
** static int opeN(int conno, int flags)
**
** PARAMETERS:
**   (in) conno                a USNET connection number
**   (in) flags                flags for TCP
**
** RETURNS:
**   NE_NOBUFS                 No buffers available
**   ETIMEDOUT                 Timed out wating for established TCP
**   0                         Non-blocking mode initiated
**   1                         success
**   message address           success
**
** DESCRIPTION:
**   This opens a connection, either actively or passively.  If non-blocking
**   mode is in place, it will return immediately after spawning TCP.
**   Otherwise, it will wait for TCP established or timed out.
**
** USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** MODIFICATION HISTORY:
**   11-NOV-1998  BSK  Chg: Removed call to network layer open.
**                     Fix: Corrected non-blocking behavior.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int opeN(int conno, int flags)
{
    int i1;
    MESS *mess;
    register struct CONNECT *conp;

    (void) i1;
    conp = &connblo[conno];
    conp->wackslow = (MAXWACK - 1) * MAXWACK;
    conp->wackmax = MAXWACK;
    conp->maxdat = MAXDAT;

   /* passive open */
    if (conp->herport == 0) {
        conp->rxtout = 0xffffffff;
        conp->state = LISTEN;
    }
   /* active open */
    else {
        if ((mess = Ngetbuf()) == 0)
            return NE_NOBUFS;
        conp->state = SYN_SENT;
        conp->rxtout = RXTOUT;
#ifdef MIB2
        TCPgroup.tcpActiveOpens++;
#endif
        mess->mlen = 0x8000 | SYN;
        mess->netno = conp->netno;
        mess->target = conp->heriid.l;
        writE(conno, mess);
    }

   /* If not just open is non-blocking, make it so */
    if (conp->txstat & S_NOWA)
        conp->rxtout = 0;
   /* If non-blocking mode, return in progress */
    if (flags & S_NOWA || conp->txstat & S_NOWA)
        return 0;

    WAITFOR(conp->state == ESTABLISHED || (conp->rxstat & S_FATAL),
        SIG_CC(conno), conp->rxtout, i1);
    if (conp->state != ESTABLISHED) {
#if NTRACE >= 1
        Nprintf("OP C%d/%x S%d failed\n", conno, conp->myport, conp->state);
#endif
#ifdef MIB2
        TCPgroup.tcpAttemptFails++;
#endif
        return ETIMEDOUT;
    }
    return 1;
}

/* ===========================================================================
   Close function.  The TCP close is asynchronous: the connection block is
   freed by the network task after a delay.  If S_NOWA is not set, we wait
   for all data to be ACK'd.  The close returns with "pending" status.
   Return values:
       1 = no errors seen
       2 = error has occurred, no waiting
       3 = error has occurred, wait for full close
*/
static int closE(int conno)
{
    int i1, stat;
    MESS *mp;
    register struct CONNECT *conp;

   /* send the close message, if not already sent */
    conp = &connblo[conno];
    switch (conp->state) {
    case LISTEN:
        return 0;
    case SYN_SENT:
        clearconn(conp);
        conp->protoc[1]->closE(conno);
        return 0;
    case ESTABLISHED:
        conp->state = conp->rxstat & S_EOF ? LAST_ACK : FINWAIT_1;
#ifdef MIB2
        TCPgroup.tcpCurrEstab--;
#endif
        if (conp->rxstat & S_FATAL) {
            stat = 3;
            goto ret3;
        }
        if ((mp = Ngetbuf()) != 0) {
            mp->mlen = 0x8000 | FIN | ACK;
            mp->netno = conp->netno;
            mp->target = conp->heriid.l;
            if (writE(conno, mp))
                Nrelbuf(mp);
        }
        break;
    }
    if (conp->rxstat & S_FATAL) {
        stat = 3;
        goto ret3;
    }
   /* Set status value.  If NOWAIT set, return from here */
    stat = conp->rxstat & S_NOACK ? 3 : 1;
    if (conp->txstat & S_NOWA)
        goto ret3;

   /* Not NOWAIT, wait for the status to become TIMEWAIT or CLOSED. */
    WAITFOR(conp->state == CLOSED || conp->state == TIMEWAIT ||
            (conp->rxstat & S_FATAL), SIG_CC(conno), MAXTXTOUT * 2, i1);
    if (i1) {
        stat = 3;
#if NTRACE >= 1
        Nprintf("CL %ld C%d/%x incomplete\n", TimeMS(), conno, conp->myport);
#endif
    }
ret3:
   /* Mark connection block so that network task will free it after the
    ** required delay.  Call low-level close, for PPP and so on. */
    conp->blockstat = 4;
    conp->protoc[1]->closE(conno);
    return stat;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  timeout()     TCP timeout function
**
**  static void timeout(int conno)
**
**  PARAMETERS:
**     (in)     conno    Connection for which timeout conditions should be
**                         checked
**
**  DESCRIPTION:
**     Checks timeout conditions for a given connection.
**
**  RETURNS:
**     none
**
**  USAGE/COMMENTS:
**     Called from the network task.  This function occupies the position of
**     the shut() function in the PTABLE.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
** MODIFICATION HISTORY:
**
**   02-DEC-1999  NJW  Clear conn. blocks if S_RST and blockstat flagged
**   09-NOV-1999  NJW  Clear reset connection blocks if rxstat & S_RST
**   30-JUL-1999  TTT  Adjust checksum if ackno changed on reTX
**   13-APR-1999  TTT  Change ackno in TCP header of message buffer to
**                     conp->seqtoack for retransmitted messages. Also set
**                     S_FATAL + S_NOACK on retranmission limit exceeded.
**   11-MAR-1999  RCA  Modified check when blockstat == 4 so that either
**                     the timeout can expire or the S_FATAL flag can be
**                     set to leave the state.
**   27-OCT-1998  RCA  Modified check for excessive retransmissions to be based
**                     on RETX_THR_R2 rather than MAXTXTOUT.  MAXTXTOUT is now
**                     the ceiling for the retransmission timeout time.
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static void timeout(int conno)
{
    int i1;
    int hdroff;
    unsigned short us1;
    unsigned long ul1;
    union {
        char c[4];
        short s[2];
        long l;
    } UL, UL2;
    unsigned NLONG *ulp;
    MESS *mp;
    struct CONNECT *conp;
    register struct Thdr *thdrp;

    conp = &connblo[conno];

   /* This code is to flush any partial segments left in ostreamb.  Moved to
    ** execute before the current time is stored in ul1 so that the segment
    ** that flushes the buffer has a timestamp earlier or equal to the time
    ** stored in ul1. */
    if (conp->ostreamb)
        if (conp->blockstat == 1 && conp->nwacks == 0 && conp->window >= MAXBUF) {
            mp = conp->ostreamb;
            conp->ostreamb = 0;
            mp->conno = conno;
            writE(-5, mp);
        }

    ul1 = TimeMS();

    /* 
    ** Check if this connection is waiting to be freed 
    ** The connection block will be cleared if the application
    ** has called a close() and the require timeout has occured OR
    ** the connection has been reset and the block is flagged to be
    ** cleared.
    */
    if ((conp->blockstat == 4 && ((ul1 - conp->lastrxtime > MAXCDEL) ||
        (conp->rxstat & S_FATAL))) || 
        ((conp->blockstat & 0x04) && (conp->rxstat & S_RST))) { 
            clearconn(conp);
        conp->blockstat = 0;           /* free connection block */
        ussHostRelease(conp->confix);  /* Release host entry */
        goto lab3;
    }

#ifdef KEEPALIVETIME
   /* This code is for the BSD socket KEEPALIVE option, which is not a true
    ** part of TCP. */
    if ((conp->txstat & S_KEEPA) && conp->state == ESTABLISHED &&
        (conp->rxstat & S_FATAL) == 0) {
/* If nothing has arrived in KEEPALIVETIME, we send an ACK with a bad sequence
   number.  If the KEEPALIVE messages seem to elicit no response, we set an
   error for the connection and wake up any readers
*/
        if (ul1 - conp->lastrxtime > KEEPALIVETIME &&
            ul1 - conp->acktime > KEEPALIVETIME) {
            if (ul1 - conp->lastrxtime < 8 * KEEPALIVETIME) {
                conp->acktime = ul1;
                conp->sendack = 100;
            } else {
                conp->rxstat |= S_FATAL + S_EOF;
                WAITNOMORE(SIG_RC(conno));
            }
        }
    }
#endif

   /* We send an ACK if the flag "send ack" is set. */
    if (conp->sendack) {
        if (conp->sendack < ACKLIMIT && ul1 - conp->acktime < ACKDELAY) {
#if MT
            nets[conp->netno].nettasktout = ACKDELAY;
#endif
            goto noack;
        }
        if ((mp = Ngetbuf()) != 0) {
            mp->mlen = 0x8000 | ACK;
            mp->netno = conp->netno;
            mp->target = conp->heriid.l;
            mp->conno = conno;
            mp->id = bRELEASE;
            if (writE(-6, mp))
                Nrelbuf(mp);
        }
    }

noack:
   /* We handle retransmissions here for the wait-for-ack queue.  The timeout
    ** after the first retransmission will double, until it reaches the value
    ** MAXTXTOUT. */
    if ((mp = conp->wackf) == 0)
        goto lab3;
    if (mp->offset != boTXDONE)
        goto lab3;
#if NTRACE > 0
    if (mp->id != bWACK)
        Npanic("timeouts wack");
#endif
    if ((conp->blockstat & 0x41) == 0x41)
        conp->blockstat &= 0xbf;
    else if (ul1 - mp->timems < conp->txtout)
        goto lab3;

   /* if excessive retransmission, or fatal error set, we give up */
    conp->txtout <<= 1;
    if (conp->txtout > MAXTXTOUT)
        conp->txtout = MAXTXTOUT;
    if ((mp->conno == RETX_THR_R2) || (conp->rxstat & S_FATAL)) {
        conp->wackf = mp->next; /* take out of WACK queue */
        conp->nwacks -= MAXWACK;/* decrement WACK counter */
        mp->id = bALLOC;        /* release buffer */
        Nrelbuf(mp);
        if (conp->state == SYN_RECEIVED) {
#ifdef MIB2
            TCPgroup.tcpAttemptFails++;
#endif
            /*
            **  If the connection block is part of a listening
            **  connection block's queue, remove it.
            */
            if (conp->backlog) {                 /* Member of queue ? */
                BLOCKPREE();                     /*   Protect queue */
                connblo[conp->icqcur].icqcur--;  /*   Just dec. count */
                RESUMEPREE();                    /*   Unprotect queue */
                conp->blockstat = 0;             /*   Free block */
            } else {
               conp->state = LISTEN;
               conp->heriid.l = 0xffffffff;
            }
          } else {
            /* Set error flag on connection even if S_EOF flag is set.
            ** Changed because WAITFOR ACK to probe waited forever, or
            ** until FATAL was set.
            */
            conp->rxstat |= S_NOACK + S_FATAL;
        }
        WAITNOMORE(SIG_RC(conno));
        goto lab3;
    }
   /* here we retransmit */
    mp->conno++;
    mp->timems = ul1;

    /* Insert the latest sequence number to acknowledge into the
    ** message buffer to retransmit. Otherwise, the message has an old
    ** ack number. This may seem counterintuitive, but it is demonstrated
    ** in the case of receiving a FIN (which consumes one sequence number)
    ** while one is still retransmitting data.  It also is important when
    ** sending and receiving on the same connection.  The checksum must
    ** be recalculated.
    */
    hdroff =
#ifdef IPOPTIONS
        conp->IPOtxlen +
#endif
        IPOFFSET + Ihdr_SZ;

    thdrp = (struct Thdr *) ((char *) mp + hdroff);
    UL.l = conp->seqtoack;             /* New Acknow. number */
    GETLONG(UL2,thdrp->ackno);         /* Current Acknow number */
    if ( UL.l != UL2.l ) {             /* Did data arrive ? */
      us1 = NC2(thdrp->chksum);        /*   Remember chksum */

      /*
      **   Adjust the checksum for the new acknowledgment number.
      **   All parameters must be in the proper byte ordering
      **   as well.
      */
      ussChecksumAdjust((unsigned char *)&us1,     /* point to chksum */
                        (unsigned char *)UL2.c, 4, /* pnt to old data */
                        (unsigned char *)UL.c, 4); /* pnt to new data */
      PUTLONG(UL, thdrp->ackno);         /* Place new acknow. in header */
      thdrp->chksum = NC2(us1);          /* Place new chksum in header */
    }  /* endif data arrived */

#if NTRACE >= 1
    ulp = (unsigned NLONG *) ((char *) mp + IPOFFSET + 24);
    GETLONG(UL, ulp);
    Nprintf("reTX%d %lu C%d/%x ST%d SQ%lx MS%ld\n", mp->conno, ul1,
            conno, conp->myport, conp->state, UL.l, conp->txtout);
#endif

#ifdef MIB2
    TCPgroup.tcpRetransSegs++;
#endif
    conp->protoc[1]->writE(-5, mp);

lab3:
    return;
}


/* Initialize the protocol. */
static int init(int netno, char *params)
{
#ifdef MIB2
    memset(&TCPgroup, 0, sizeof(TCPgroup));
    TCPgroup.tcpRtoMax = MAXTXTOUT;
#endif
    (void) netno;
    (void) params;
    return 0;
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg);
**
**  PARAMETERS:
**     (in)     handle   likely to be netno if any functions are added
**                       at this level
**     (in)     request  constant for the command to perform
**     (in)     arg      pointer to the parameters to the command, which
**                       will be specific to the command
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.  At the moment, there are no functions
**     defined for this layer.
**
**  RETURNS:
**     0              Success
**     USS_ERR_INVAL  Support for the request not implemented
**
**  USAGE/COMMENTS:
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  31-DEC-1998  RCA  Created stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */

    return ussErrInval;
}


/* Protocol table for TCP. */
GLOBALCONST
PTABLE ussTCPTable = {
    "TCP", init, timeout, screen, opeN, closE, reaD, writE, ioctl,
       IP_TCP, Thdr_SZ
};


/*****************************************************************
			ÇsÇbÇo Maximum Segment Lifetime ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetMSL( long msl )
{
	MSL = msl;
}
/*****************************************************************
			ÇsÇbÇo Maximum Segment Lifetime éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetMSL( void )
{
	return MSL;
}
/*****************************************************************
		ÇsÇbÇo maximum messages in the future queue ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetMAXFUT( char maxfut )
{
	MAXFUT = maxfut;
}
/*****************************************************************
		ÇsÇbÇo maximum messages in the future queue éÊìæèàóù
*****************************************************************/
char             UsnetTCPGetMAXFUT( void )
{
	return MAXFUT;
}
/*****************************************************************
			ÇsÇbÇo maximum delay in close ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetMAXCDEL( long maxcdel )
{
	MAXCDEL = maxcdel;
}
/*****************************************************************
			ÇsÇbÇo maximum delay in close éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetMAXCDEL( void )
{
	return MAXCDEL;
}
/*****************************************************************
			ÇsÇbÇo Initial value for smoothed RTT ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetINIT_TXAVE( long txave )
{
	INIT_TXAVE = txave;
}
/*****************************************************************
			ÇsÇbÇo Initial value for smoothed RTT éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetINIT_TXAVE( void )
{
	return INIT_TXAVE;
}
/*****************************************************************
		ÇsÇbÇo Initial value for mean RTT variation RTT ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetINIT_TXVAR( long txvar )
{
	INIT_TXVAR = txvar;
}
/*****************************************************************
		ÇsÇbÇo Initial value for mean RTT variation RTT éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetINIT_TXVAR( void )
{
	return INIT_TXVAR;
}
/*****************************************************************
		ÇsÇbÇo minimum timeout ms for retransmission ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetMINTXTOUT( long mintxtout )
{
	MINTXTOUT = mintxtout;
}
/*****************************************************************
		ÇsÇbÇo minimum timeout ms for retransmission éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetMINTXTOUT( void )
{
	return MINTXTOUT;
}
/*****************************************************************
		ÇsÇbÇo maximum timeout ms for transmit ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetMAXTXTOUT( long maxtxtout )
{
	MAXTXTOUT = maxtxtout;
}
/*****************************************************************
		ÇsÇbÇo maximum timeout ms for transmit éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetMAXTXTOUT( void )
{
	return MAXTXTOUT;
}
/*****************************************************************
		ÇsÇbÇo timeout ms for read ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetRXTOUT( long rxtout )
{
	RXTOUT = rxtout;
}
/*****************************************************************
		ÇsÇbÇo timeout ms for read éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetRXTOUT( void )
{
	return RXTOUT;
}
/*****************************************************************
		ÇsÇbÇo maximum number of retransmissions ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetRETX_THR_R2( char retx )
{
	RETX_THR_R2 = retx;
}
/*****************************************************************
		ÇsÇbÇo maximum number of retransmissions éÊìæèàóù
*****************************************************************/
char             UsnetTCPGetRETX_THR_R2( void )
{
	return RETX_THR_R2;
}
/*****************************************************************
			ÇsÇbÇo delay before sending ACK ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetACKDELAY( char ackdely )
{
	ACKDELAY = ackdely;
}
/*****************************************************************
			ÇsÇbÇo delay before sending ACK éÊìæèàóù
*****************************************************************/
char             UsnetTCPGetACKDELAY( void )
{
	return ACKDELAY;
}
/*****************************************************************
			ÇsÇbÇo ACK forced after this many messages ê›íËèàóù
*****************************************************************/
void             UsnetTCPSetACKLIMIT( long acklimit )
{
	ACKLIMIT = acklimit;
}
/*****************************************************************
			ÇsÇbÇo ACK forced after this many messages éÊìæèàóù
*****************************************************************/
long             UsnetTCPGetACKLIMIT( void )
{
	return ACKLIMIT;
}
/*****************************************************************
			ÇsÇbÇo TCP/IP Standard ê›íËèàóù
				 0 = RFC 1122, 1 = 4.3BSD
*****************************************************************/
void             UsnetTCPSetURGSTAND( char urgstand )
{
	URGSTAND = urgstand;
}
/*****************************************************************
			ÇsÇbÇo TCP/IP Standard éÊìæèàóù
				 0 = RFC 1122, 1 = 4.3BSD
*****************************************************************/
char             UsnetTCPGetURGSTAND( void )
{
	return URGSTAND;
}
/*****************************************************************
			ÇsÇbÇo segment negotiation ê›íËèàóù
				 0 = local only, 1 = always
*****************************************************************/
void             UsnetTCPSetNEGOTI( char negoti )
{
	NEGOTI = negoti;
}
/*****************************************************************
			ÇsÇbÇo segment negotiation éÊìæèàóù
				 0 = local only, 1 = always
*****************************************************************/
char             UsnetTCPGetNEGOTI( void )
{
	return NEGOTI;
}
