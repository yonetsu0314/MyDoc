/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsvr60/rcs/ftp.c 1.2 1970/01/01 00:00:00 kagatume Exp $
 * $Log: ftp.c $
 * リビジョン 1.2  1970/01/01  00:00:00  kagatume
 * 2005/02/17 12:30
 * ファイルシステム変換関数(cnvfsys.c)の適用
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  kagatume
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/
/* 説明                                                  			   */
/*	Filesystem "SAVER60 for FAT"用	FTP								   */
/*---------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* FTP.C  Edition: 17  Last changed: 22-Sep-99,9:36:02  By: TRACY */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    FTP.C -- FTP File Transfer Routines for USNET

    Copyright (C) 1993 By
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

** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-SEP-1999 TTT  server2(): access FTP command by byte; break on
**                   data conn open failure
**  02-SEP-1999 BTS  Added non-blocking open to FTP Data Port in getput
**  29-OCT-1998 TTT  Added EXTENDED_C support for Microsoft DOS; fixed 
**                   bugs in EXTENDED_C server commands.
**  21-DEC-1998 TTT  Fix bug handling remote file open fail
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

#define ENOBUFCNT  255            /* number of times an attempt to reissue */
                                /*   a USNET call will occur up on receiveing */
                                /*   an ENOBUF error code. 1 to 255 OK*/
#define EXTENDED_C 2            /* 0 = ANSI C support only */
                                /* 1 = Borland DOS support */
                                /* 2 = Microsoft DOS support */
                                /* 3 = UNIX support */

#include <stdlib.h>
#include <string.h>
#include "net.h"
#include "local.h"
#include "support.h"
#if EXTENDED_C == 1
#include <dir.h>
#include <dos.h>
#endif
#if EXTENDED_C == 2
#if 0
#include <direct.h>
#define ffblk find_t
#define ff_attrib attrib
#define ff_fsize size
#define ff_ftime wr_time
#define ff_fdate wr_date
#define ff_name name
#define FA_DIREC _A_SUBDIR
#define findfirst _dos_findfirst
#define findnext _dos_findnext
#include <dos.h>
#else

#include "drive\fssvr60\fsbase.h"
#include "drive\fssvr60\shellsrc.h"
#define __S60FAT_H__	/* 二重定義エラーを避けるため */
#include "drive\fssvr60\cnvfsys.h"

#define ffblk find_t
#define ff_attrib attrib
#define ff_fsize size
#define ff_ftime wr_time
#define ff_fdate wr_date
#define ff_name name
#define FA_DIREC IFS_ATTR_DIR
#define FILE ST_FILE

#endif
#endif
#if EXTENDED_C == 3
#include <unistd.h>
#endif

extern struct NETCONF netconf[];
extern struct NET nets[];
extern PTABLE   ussTCPTable;

static int      gflag,
                gconno;

struct LINESTR {
    int             conno;      /* connection number */
    int             pchs;       /* number of characters left to process */
    MESS           *mp;         /* pointer to current message buffer */
    char           *bp;         /* pointer to current position in message
                                   buffer */
    char            prevcc;     /* previous character processed */
};

static char fullpass[DFMR_MAXPASSSZ+1];


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  gline()
**
**  int gline( struct LINESTR *lsp, unsigned char *lbuf, int buflen )
**
**  PARAMETERS:
**      lsp         Pointer to LINESTR structure that saves state information
**                  for the reply that is being read
**      lbuf        Pointer to a buffer where the received line will be stored
**      buflen      The length of lbuf
**
**  RETURNS:        The number of characters in the response line, or -2
**                  on a read error.
**
**  DESCRIPTION:  
**      gline() interprets the response from the ftp server, returning when
**      a full response has been received.
**
**      RFC 959 describes multi-line replies.  A multi-line reply contains
**      a 3 digit code followed by a dash on the first line of a reply, and
**      the same 3 digit code followed by a space on the last line of the
**      reply.  gline() will read input until the last line of the reply
**      has been received, and save the last line in lbuf.
**
**      When NTRACE is >= 1, the first buflen-1 characters of all reply lines
**      will be displayed.
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-SEP-1999  BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/


static int      gline(struct LINESTR * lsp, unsigned char *lbuf, int buflen)
{
    unsigned char   cc,
                    *cp,
                   *linep;
    int             multiline;

    multiline = 0;
    linep = lbuf;
    for (;;) {
        if (lsp->pchs <= 0) {
            lsp->mp = ussTCPTable.reaD(lsp->conno);
            if (lsp->mp == 0)
                return -2;
            lsp->pchs = lsp->mp->mlen - lsp->mp->offset;
            lsp->bp = (char *) lsp->mp + lsp->mp->offset;
        }
        cc = *lsp->bp++;
        lsp->pchs--;
        if (cc == 0xa && lsp->prevcc == 0xd) {
            linep--;
            *linep = 0;
            if (lsp->pchs <= 0) {
                Nrelbuf(lsp->mp);
                lsp->mp = 0;
            }
#if NTRACE >= 1
            Nprintf("FTP.gline: `");
            for (cp = lbuf; *cp; cp++)
                Nputchr(*cp);
            Nprintf("` \n");
#endif
            if (!multiline) {
                if (lbuf[3] != '-')
                    return linep - lbuf;
                multiline = atoi((const char *) lbuf);
            } else {
                if ((atoi((const char *) lbuf) == multiline) && (lbuf[3] == ' '))
                    return linep - lbuf;
            }
            lsp->prevcc = 0;
            linep = lbuf;
        } else {
            lsp->prevcc = cc;
            if (linep - lbuf < buflen - 2)
                *linep++ = cc;
        }
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  FTPtx()
**
**  int FTPtx( MESS *mp )
**
**  PARAMETERS:
**      (in/out) mp  USNET message structure
**
**  RETURNS: 0
**
**  DESCRIPTION:  
**      This is a subroutine to send a file to the host.  We pick up the
**      necessary information from the message.  Then we open a new connection
**      to the remote host.  Then we start reading and sending the file.
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-SEP-1999  BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int      FTPtx(MESS * mp)
{
    int             i1,
                    hdroff,
                    cconno,
                    conno,
                    herport,
    				len;
    unsigned char  nobufcnt;   /* ENOBUFS error count */
    char           *bp;
#if NTRACE >= 1
    long            l1;
#endif
    long            tlen,
                    l2;
    long            handle;

    l2 = 0;
    herport = mp->mlen;
    cconno = mp->conno;
    hdroff = mp->offset;

	memset( fullpass, 0, sizeof(fullpass) );
	strcpy( fullpass, (char *) mp + hdroff + 5 );
    handle = ff_fopen(fullpass, "r");
/*    ffp = fopen((char *) mp + hdroff + 5, "r");*/
    SOCKET_PUSH(cconno);
    if (handle < 0) {
        Nwrite(cconno, "510 file open fails.\r\n", 22);
        goto err3;
    }
    /* Open data connection */
    nobufcnt = 0;
#if 1
    while( ((conno = Nopen((char *) &SOCKET_IPADDR(cconno), "TCP/IP", 20,
                  herport, IPADDR)) == ENOBUFS) &&
                  (nobufcnt < ENOBUFCNT)) {
        nobufcnt++;
        YIELD();
#if NTRACE >= 1
        Nprintf("FTP.server2 Nopen retry %d \n", nobufcnt);
#endif
    }  /* end while to retry Nopen() */
#else
	conno = Nopen((char *) &SOCKET_IPADDR(cconno), "TCP/IP", 20,
                  herport, IPADDR);
#endif
    if (conno < 0) {
        SOCKET_PUSH(cconno);
        Nwrite(cconno, "510 port open fails.\r\n", 22);
        goto err2;
    }
    Nwrite(cconno, "150 ready to send file.\r\n", 25);
    IFS_seek(handle, 0, IFS_SEEK_END);
    tlen = IFS_tell(handle);
    IFS_seek(handle, 0, IFS_SEEK_SET);
    bp = (char *) mp + hdroff;
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    for (;;) {
        len = IFS_read(handle, bp, SOCKET_MAXDAT(conno));
        if (len <= 0)
            break;
        l2 += len;
        if (l2 >= tlen)
            SOCKET_PUSH(conno);
        nobufcnt = 0;
        while ( ((i1 = Nwrite(conno, bp, len)) == ENOBUFS ) &&
               (nobufcnt < ENOBUFCNT)) {
          nobufcnt++;
          YIELD();
#if NTRACE >= 1
          Nprintf("FTP.Tx: Nwrite retry %d\n",nobufcnt);
#endif
        }  /* end while on Nwrite() retry */
        if (i1 < 0) {
#if NTRACE >= 1
            Nprintf("FTP.Tx: Nwrite error %d\n",i1);
#endif
			SOCKET_PUSH(cconno);
            Nwrite(cconno, "426 Data Connection: File read error. \r\n", 39);
            goto err1;
        }
    }
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("FTP.Tx: %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1,
                (l2 * 1000) / l1);
#endif
    SOCKET_PUSH(cconno);
err1:
    Nclose(conno);
    Nwrite(cconno, "226 closing.\r\n", 14);
err2:
    IFS_fclose(handle);
err3:
    Nrelbuf(mp);
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  FTPrx()
**
**  FTPrx( MESS *mp )
**
**  PARAMETERS:
**      (in/out)  mp  USNET Message structure
**
**  RETURNS: 0
**
**  DESCRIPTION:  
**      This is a subroutine to receive a file from the host.  We pick up the
**      necessary information from the message.  Then we open a new connection
**      to the remote host.  Then we receive data from the host and write it into
**      the file.
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-SEP-1999  BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/

static int      FTPrx(MESS * mp)
{
    unsigned long   i1;
    int             hdroff,
                    cconno,
                    conno,
                    herport,
                    len;
    char           *cp;
#if NTRACE >= 1
    long            l1;
#endif
    long            l2;
    unsigned char  nobufcnt;   /* ENOBUFS error count */
    long            handle;
	char str1[]="w";
	char str2[]="a";

    herport = mp->mlen;
    cconno = mp->conno;
    hdroff = mp->offset;
    for (cp = (char *) mp + hdroff + 5; *cp > ' '; cp++);
    *cp = 0;
/*    cp = mp->netno == 14 ? "wb" : "ab";*/
/*    cp = mp->netno == 14 ? "w" : "a";*/
	if( mp->netno == 14 ) {
    	cp = str1;
	}
	else {
    	cp = str2;
	}
	memset( fullpass, 0, sizeof(fullpass) );
	strcpy( fullpass, (char *) mp + hdroff + 5 );
	handle = ff_fopen(fullpass, cp);
    Nrelbuf(mp);
    SOCKET_PUSH(cconno);
    if (handle < 0) {
        Nwrite(cconno, "510 file open fails.\r\n", 22);
        goto err3;
    }
    /* Open data connection */
    nobufcnt = 0;
#if 1
    while( ((conno = Nopen((char *) &SOCKET_IPADDR(cconno), "TCP/IP", 20,
                  herport, IPADDR)) == ENOBUFS) &&
                  (nobufcnt < ENOBUFCNT)) {
        nobufcnt++;
        YIELD();
#if NTRACE >= 1
        Nprintf("FTP.server2 Nopen retry %d \n", nobufcnt);
#endif
    }  /* end while to retry Nopen() */
#else
	conno = Nopen((char *) &SOCKET_IPADDR(cconno), "TCP/IP", 20,
                  herport, IPADDR);
#endif
    if (conno < 0) {
        SOCKET_PUSH(cconno);
        Nwrite(cconno, "510 port open fails.\r\n", 22);
        goto err2;
    }

    Nwrite(cconno, "150 ready to take file.\r\n", 25);
    l2 = 0;
#if NTRACE >= 1
    l1 = TimeMS();
#endif
    for (;;) {
        mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0)
            break;
        len = mp->mlen - mp->offset;
        i1 = IFS_write(handle, (char *) mp + mp->offset, len);
        Nrelbuf(mp);
        if (i1 <= 0) {
#if NTRACE >= 1
            Nprintf("FTP.Rx: fwrite error %d (probably disk full) \n,i1");
#endif
            SOCKET_FIN(conno);    /* Send FIN on data connection */
            Nwrite(conno, "", 0); /* Send it */
            /* Read and discard incoming data as long as 
             * remote side hasn't sent FIN
             */
            for ( ; ; ) {
                YIELD();
                if (SOCKET_TESTFIN(conno)) 
                    break;
                mp = connblo[conno].protoc[0]->reaD(conno);
                if (mp != 0) 
                    Nrelbuf(mp);
                else
                   break;
            }
            SOCKET_PUSH(cconno);
            Nwrite(cconno, "426 Data Connection: File write error. \r\n", 41);
            Nclose(conno);
            goto err2;
        }
        l2 += len;
    }
#if NTRACE >= 1
    l1 = TimeMS() - l1;
    if (l1 && l2)
        Nprintf("FTP.Rx: %ld bytes in %ld ms = %ld bytes/sec\n", l2, l1,
                (l2 * 1000) / l1);
#endif
err1:
    SOCKET_PUSH(cconno);
    Nwrite(cconno, "226 closing.\r\n", 14);
    Nclose(conno);
err2:
    IFS_fclose(handle);
err3:
    return 0;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  FTPServ
**
**  void FTPserv( void )
**
**  PARAMETERS: none
**
**  RETURNS: Never returns unless error
**
**  DESCRIPTION:  
**      This runs as a server task for FTP.  When it gets a new open, it starts
**      a task to do the FTP for this client, and goes back to the passive open.
**      The started task is responsible for closing the connection.  The server
**      will run without multitasking, but then only one client at a time can be
**      active.
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-SEP-1999  BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/


static const char cmds[] = "\
userpassacctcwd cdupsmntquitrein\
portpasvtypestrumoderetrstorstou\
appeallorestrnfrrntoabordelermd \
mkd pwd listnlstsitesyststathelp\
noopxmkdxrmdxpwdxcupxcwd";
/****************************************/
static const char helpmsg[] = "214 The following commands are recognized.\n\
USER  PORT  RETR  DELE  MKD  XMKD\n\
PASS  STOR  CWD   XCWD  RMD  XRMD\n\
TYPE  HELP  RNFR  RNTO  PWD  XPWD\n\
LIST  NLST  QUIT                 \r\n";
/****************************************/
static const char OKmess[] = "200 OK.\r\n";
TASKFUNCTION server2(void);

void            FTPserv(void)
{
    int             conno,
                    i1;

    for (;;) {
        if ((conno = Nopen("*", "TCP/IP", 21, 0, 0)) < 0) {
#if NTRACE >= 1
/*		    Nprintf("wai_flg_sta(SIG_GEN),ftp.c,FTPserv\n");
*/
#endif
            WAITFOR(0, SIG_GEN, 300, i1);
#if NTRACE >= 1
/*		    Nprintf("wai_flg_end(SIG_GEN),ftp.c,FTPserv\n");
*/
#endif
            continue;
        }
#if NTRACE >= 1
        Nprintf("FTP Nopen'*'\n");
#endif

        gflag = 0;
        gconno = conno;
#ifndef V3
        RUNTASK3(server2, SERV_PRIOR);
#else
		RUNFTPSERVERTASK();
#endif
#if NTRACE >= 1
/*	    Nprintf("wai_flg_sta(SIG_GEN),ftp.c,FTPserv\n");
*/
#endif
        WAITFOR(gflag, SIG_GEN, 1000, i1);
#if NTRACE >= 1
/*	    Nprintf("wai_flg_end(SIG_GEN),ftp.c,FTPserv\n");
*/
#endif
    }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  server2()
**
**  TASKFUNCTION server2( void )
**
**  PARAMETERS: none
**
**  RETURNS:  Never returns unless error
**
**  DESCRIPTION: This is the FTP server task started by FTPserv().
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  22-SEP-1999  TTT  Read FTP command by byte instead of by word; 
**                    break from loop if error on data conn open
**  02-SEP-1999  BTS  Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/


TASKFUNCTION server2(void)
{
    int             conno;
    int             i1,
                    i2,
                    i3,
                    i4,
                    i5,
                    i6,
                    er,
					er1,
					len,
                    herport,
                    cno,
                    loggedin;
    char           *lp;
    char		    l1[4];
    unsigned char  nobufcnt;   /* ENOBUFS error count */
    char           *rp,
                   *cp,
                   *argp;
    MESS           *mp;
#if EXTENDED_C == 1 || EXTENDED_C == 2
    int             dconno;
    char            cc,
                    oldname[32];
    struct ffblk    ffblk;
#endif

    conno = gconno;
    gflag = 1;
#if NTRACE >= 1
/*
    Nprintf("set_flg_(SIG_GEN),ftp.c,server2\n");
*/
#endif
    WAITNOMORE(SIG_GEN);
    loggedin = herport = 0;
    SOCKET_PUSH(conno);
    Nwrite(conno, "220 FTP server ready.\r\n", 23);
    SOCKET_RXTOUT(conno, 300000);   /* 5 minute read timeout */
    for (;;) {
        mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0)
            goto err2;
        len = mp->mlen - mp->offset;
        rp = (char *) mp + mp->offset;
        for (cp = rp + len; cp > rp;) {
            *cp-- = 0;
            if (*cp >= ' ')
                break;
        }

        argp = rp + 5;
        if (rp[3] <= ' ') {
            argp--;
            rp[3] = ' ';
        }
        /* 
        **  Get the command out of the packet.  These are the first 4 
        **  bytes. Have to access by bytes because TCP data is not
        **  aligned and non-aligned word can cause a fault on some 
        **  machines.
        */        
        memcpy( l1, rp, 4 );
		l1[0] |= 0x20;
		l1[1] |= 0x20;
		l1[2] |= 0x20;
		l1[3] |= 0x20;
        for (lp = cmds, cno = 0; cno < sizeof(cmds) / 4; cno++) {
            if (lp[cno*4]   == l1[0] && 
				lp[cno*4+1] == l1[1] &&
				lp[cno*4+2] == l1[2] &&
				lp[cno*4+3] == l1[3] ) {
/************************************************/
#if 0
				if( l1[3] == ' ' ) {
					argp--;
				}
#endif
/************************************************/
				break;
			}
		}
#if NTRACE >= 1
        if (cno != 1)
            Nprintf("FTP.server2: `%s` \n", rp);
        else
            Nprintf("FTP: PASS\n");
#endif
        SOCKET_PUSH(conno);
        if (loggedin != 2 && cno != 0 && cno != 1 && cno != 6) {
            Nwrite(conno, "530 access denied.\r\n", 20);
            Nrelbuf(mp);
            continue;
        }
        switch (cno) {
        default:
            Nwrite(conno, "500 command not supported.\r\n", 28);
            break;
        case 0:                 /* USER */
            loggedin = 0;
            if (strcmp(argp, USERID) == 0) {
                if (strcmp("", PASSWD) != 0) {
                    loggedin = 1;
                    Nwrite(conno, "331 Password required.\r\n", 24);
                } else {
                    loggedin = 2;
                    Nwrite(conno, "230 User name accepted.\r\n", 25);
                }
            } else
                Nwrite(conno, "530 access denied.\r\n", 20);
            break;
        case 1:                 /* PASS */
            if (strcmp(argp, PASSWD) != 0)
                Nwrite(conno, "530 access denied.\r\n", 20);
            else {
                loggedin = 2;
                Nwrite(conno, "230 User name accepted.\r\n", 25);
            }
            break;
        case 6:                 /* QUIT */
            SOCKET_PUSH(conno);
            SOCKET_FIN(conno);
            Nwrite(conno, "221 Goodbye.\r\n", 14);
            goto err1;
        case 8:                 /* PORT */
            Nsscanf(argp, "%d,%d,%d,%d,%d,%d", &i1, &i2, &i3, &i4, &i5, &i6);
            herport = (i5 << 8) | i6;
            er = Nwrite(conno, (char *) OKmess, sizeof(OKmess) - 1);
            if( er<0 ) {
				er1=1;
			}
			break;
        case 10:                /* TYPE */
            Nwrite(conno, (char *) OKmess, sizeof(OKmess) - 1);
            break;
#if 0
        case 11:                /* STRU */
            break;
        case 12:                /* MODE */
            break;
#endif
        case 13:                /* RETR */
        case 14:                /* STOR */
        case 16:                /* APPE */
            mp->conno = conno;
            mp->netno = cno;
            mp->mlen = herport;
            if (cno == 13)
                i2 = FTPtx(mp);
            else
                i2 = FTPrx(mp);
            if (i2 < 0)
                goto err1;
            continue;
#if EXTENDED_C
        case 3:                 /* CWD */
        case 37:                /* XCWD */
            BLOCKPREE();
            i1 = chdir(argp);
            goto cmde1;
        case 4:                 /* CDUP */
        case 36:                /* XCUP */
            BLOCKPREE();
            i1 = chdir("..");
            goto cmde1;
        case 19:                /* RNFR */
            strcpy(oldname, argp);
            Nwrite(conno, "350 ready.\r\n", 12);
            break;
        case 20:                /* RNTO */
            BLOCKPREE();
            i1 = ff_rename(oldname, argp);
            goto cmde1;
        case 22:                /* DELE */
            BLOCKPREE();
            i1 = ff_remove(argp);
            goto cmde1;
        case 23:                /* RMD */
        case 34:                /* XRMD */
            BLOCKPREE();
            i1 = ff_rmdir(argp);
            goto cmde1;
        case 24:                /* MKD */
        case 33:                /* XMKD */
            BLOCKPREE();
            i1 = ff_mkdir(argp);
            goto cmde2;
    cmde1:   RESUMEPREE();
            if (i1 == 0)
                Nwrite(conno, "250 command successful.\r\n", 25);
            else
                Nwrite(conno, "502 failed.\r\n", 13);
            break;
    cmde2:   RESUMEPREE();
            if (i1 == 0)
                Nwrite(conno, "257 command successful.\r\n", 25);
            else
                Nwrite(conno, "502 failed.\r\n", 13);
            break;
        case 25:                /* PWD */
            strcpy(rp, "257 ");
            BLOCKPREE();
            getcwd(argp, MAXBUF - mp->offset - 4);
            goto labcd;
        case 35:                /* XPWD */
            strcpy(rp, "257 ");
            argp--;
            BLOCKPREE();
            getcwd(argp, MAXBUF - mp->offset - 5);
labcd:      RESUMEPREE();
            strcat(rp, "\r\n");
            Nwrite(conno, rp, strlen(rp));
            break;
        case 26:                /* LIST */
        case 27:                /* NLST */
            nobufcnt = 0;        
#if 1
            while ( ((dconno = Nopen((char *) &SOCKET_IPADDR(conno), "TCP/IP",
                             20, herport, IPADDR)) == ENOBUFS) &&
                   (nobufcnt < ENOBUFCNT) ) {
              nobufcnt++;
              YIELD();
#if NTRACE >= 1
              Nprintf("FTP.server2 Nopen cno %d retry %d \n",cno,nobufcnt);
#endif
            }  /* end while to retry Nopen() */
#else
			dconno = Nopen((char *) &SOCKET_IPADDR(conno), "TCP/IP", 20,
                  herport, IPADDR);
#endif
            if (dconno < 0) {
                SOCKET_PUSH(conno);
                Nwrite(conno, "425 port open fails.\r\n", 22);
                goto err1;
            }
            Nwrite(conno, "150 data port open.\r\n", 21);
            i3 = 1;
            for (;;) {
	            BLOCKPREE();
                if (i3 == 1) {
                    if (*argp == 0) {
                        argp = "*";
					}
#if EXTENDED_C == 1
                    i3 = findfirst(argp, &ffblk, 0xff );
#endif
#if EXTENDED_C == 2
                    i3 = findfirst(argp, 0xff, &ffblk);
#endif
                } else {
                    i3 = findnext(&ffblk);
                }
				if (i3<0) {
		            RESUMEPREE();
                    break;
                }
				if (cno == 27) {
                    memcpy(rp, ffblk.ff_name, 13);
                    rp[4 + 13] = 0;
                } else {
                    memset(rp, 0, 80);

 					i1 = ffblk.ff_ftime >> 11;
                    cc = i1 < 12 ? 'a' : 'p';
                    if (i1 > 12)
                        i1 -= 12;
                    Nsprintf(rp, "%02d-%02d-%02d  %02d:%02d%c ",
                             (ffblk.ff_fdate >> 5) & 0xf,
                          ffblk.ff_fdate & 0x1f, (ffblk.ff_fdate >> 9) - 20,
                             i1, (ffblk.ff_ftime >> 5) & 0x3f, cc);

                    if (ffblk.ff_attrib & FA_DIREC) {
                        strcpy(rp + strlen(rp), "<DIR>");
                        Nsprintf(rp + strlen(rp), "          ", 10);
                    }
					else {
                        strcpy(rp + strlen(rp),  "     ");
                        Nsprintf(rp + strlen(rp), "%9ld ", ffblk.ff_fsize);
					}

                    memcpy(rp + strlen(rp), ffblk.ff_name, strlen(ffblk.ff_name));
                } 
                strcat(rp, "\r\n");
	            RESUMEPREE();
                SOCKET_PUSH(dconno);
		        nobufcnt = 0;
				while (( (i3=Nwrite(dconno, rp, strlen(rp))) == ENOBUFS ) &&
	               (nobufcnt < ENOBUFCNT)) {
					nobufcnt++;
		        	YIELD();
#if NTRACE >= 1
					Nprintf("FTP.LIST: Nwrite retry \n");
#endif
		        }  /* end while on Nwrite() retry */
		        if (i3 < 0) {
#if NTRACE >= 1
        		    Nprintf("FTP.Tx: Nwrite error %d\n",i1);
#endif
					SOCKET_PUSH(conno);
            		Nwrite(conno, "426 Data Connection: File read error. \r\n", 39);
		            Nclose(dconno);
					goto err1;
		        }
			}
            i3 = Nclose(dconno);
           	if( i3<0 ) {
		        Nprintf("%x", i3);
			}
            SOCKET_PUSH(conno);
			while ( Nwrite(conno, "226 closing.\r\n", 14) == ENOBUFS ) {
	        	YIELD();
#if NTRACE >= 1
				Nprintf("FTP.LIST: Nwrite retry \n");
#endif
	        }  /* end while on Nwrite() retry */
            break;
#endif
            case 31:                /* HELP */
            SOCKET_PUSH(conno);
            
			Nwrite(conno, (char *) helpmsg, sizeof(helpmsg)-1 );
			break;
        }
        Nrelbuf(mp);
	}
err1:
    Nrelbuf(mp);
err2:
    Nclose(conno);
#ifdef KILLTASK
    KILLTASK();
#endif
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  FTPgetput()
**
**  int FTPgetput( char *host, char *file, int mode )
**
**  PARAMETERS:
**      host        The other station
**      file        file names in format "local remote"
**                  if only one is given, then remote is the same
**      mode        ASCII or IMAGE, if call is put value 2 added
**
**  RETURNS:  0 on success
**          < 0 on error - return code from Nclose or gline
**
**  DESCRIPTION:    User subroutine to send or receive a file using 
**                  the FTP protocol.
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  02-SEP-1999 BTS     Nonblocking DATA Port open
**  02-SEP-1999 BTS     Added Header
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/


int FTPgetput(char *host, char *file, int mode)
{
    int             stat,
                    i1,
                    i2,
                    len,
                    cconno,     /* Control Connection */
                    conno;      /* Data connection */
    unsigned int    myport;
    unsigned long   ul1,
                    ul2;
    long            tlen,
                    handle;
    unsigned char  nobufcnt;   /* ENOBUFS error count */
    char           *cp1,
                   *cp2,
                   *bp,
                   *fnp;
    unsigned char  *cp;
    MESS           *mp;
    char            buff[81];
    struct LINESTR  linestr;

    memset((char *) &linestr, 0, sizeof(linestr));
    stat = -29;
    cp1 = "RETR";
    cp2 = "w";
    if (mode & 2) {
        cp1 = "STOR";
        cp2 = "r";
    }
    fnp = file;
    for (i1 = 0; file[i1] > ' '; i1++)
        buff[i1] = file[i1];
    if (file[i1] == ' ')
        fnp = file + i1 + 1;
    buff[i1] = 0;
    handle = ff_fopen(buff, cp2);
    if (handle < 0)
        return -1;
    IFS_seek(handle, 0, IFS_SEEK_END);
    tlen = IFS_tell(handle);
    IFS_seek(handle, 0, IFS_SEEK_SET);
    myport = Nportno();
aga3:
    stat = cconno = Nopen(host, "TCP/IP", myport, 21, 0);
    if (cconno < 0) {
        if (cconno == ENOBUFS) {
            YIELD();
            goto aga3;
        }
        goto err1;
    }
    linestr.conno = cconno;
aga1:
    stat = i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err2;
    if (buff[0] != '2')
        goto aga1;
    i1 = Nsprintf(buff, "USER %s\r\n", USERID);
    SOCKET_PUSH(cconno);
    Nwrite(cconno, buff, i1);
    stat = i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err2;
    if (buff[0] == '2')
        goto lab3;
    if (buff[0] != '3')
        goto err4;
    i1 = Nsprintf(buff, "PASS %s\r\n", PASSWD);
    SOCKET_PUSH(cconno);
    Nwrite(cconno, buff, i1);
aga2:
    stat = i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err2;
    if (buff[0] != '2')
        goto err4;
lab3:
    SOCKET_PUSH(cconno);
    if ((mode & ~2) == ASCII)
        Nwrite(cconno, "TYPE A\r\n", 8);
    else
        Nwrite(cconno, "TYPE I\r\n", 8);
    i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err2;
    if (buff[0] != '2')
        goto err4;
    cp = SOCKET_OWNIPADDR(cconno).c;
    myport = Nportno();
    i1 = Nsprintf(buff, "PORT %d,%d,%d,%d,%d,%d\r\n", cp[0], cp[1],
                  cp[2], cp[3], myport >> 8, myport & 0xff);
    SOCKET_PUSH(cconno);
    Nwrite(cconno, buff, i1);
    stat = i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err2;
    if (buff[0] != '2')
        goto err4;
    /* Build the command in buff, but don't send it yet */    
    i1 = Nsprintf(buff, "%s %s\r\n", cp1, fnp);

aga4:
    /* Open the data port before sending the command */
    stat = conno = Nopen(host, "TCP/IP", myport, 0, S_NOWA);
    if (conno < 0) {
        if (conno == ENOBUFS) {
            YIELD();
            goto aga4;
        }
        goto err2;
    }
    /* Write the command now */
    SOCKET_PUSH(cconno);
    Nwrite(cconno, buff, i1);

    /* Wait for the connection to open */
    while( !SOCKET_ISOPEN( conno ) )
        YIELD( );
    
    /* process the response from the STOR/RETR/etc command */
    stat = i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
    if (i1 <= 0)
        goto err31;
    if (buff[0] != '1')
        goto err3;
    for (ul1 = TimeMS(); TimeMS() - ul1 < 3000;) {
        YIELD();
        if (connblo[conno].state == 1)
            goto lab16;
    }
    i1 = ETIMEDOUT;
    goto err2;
lab16:
#if NTRACE >= 1
    ul1 = TimeMS();
#endif
    if (mode & 2)
        goto putl;

    for (ul2 = 0;;) {
        mp = connblo[conno].protoc[0]->reaD(conno);
        if (mp == 0)
            break;
        len = mp->mlen - mp->offset;
        i1 = IFS_write(handle, (char *) mp + mp->offset, len);
        Nrelbuf(mp);
        ul2 += len;
    }
lab1:
    i2 = Nclose(conno);
    if (i2 < 0)
        Nprintf("FTP.getput: Nclose data %d\n", i2);
#if NTRACE >= 1
    ul1 = TimeMS() - ul1;
    if (ul1 && ul2)
        Nprintf("FTP.getput: %cX %ld bytes in %ld ms = %ld bytes/sec\n", mode & 2 ? 'T' : 'R',
                ul2, ul1, (ul2 * 1000) / ul1);
#endif
    SOCKET_PUSH(cconno);
    Nwrite(cconno, "QUIT\r\n", 6);
    for (;;) {
        i1 = gline(&linestr, (unsigned char *) buff, sizeof(buff));
        if (i1 <= 0)
            break;
    }
    Nclose(cconno);
    IFS_fclose(handle);
    return i2;

putl:
    if ((mp = Ngetbuf()) == 0)
        return NE_NOBUFS;
    bp = (char *) mp + connblo[conno].doffset;
    for (ul2 = 0;;) {
        len = IFS_read(handle, bp, SOCKET_MAXDAT(conno));
        if (len <= 0)
            break;
        ul2 += len;
        if (ul2 >= tlen)
            SOCKET_PUSH(conno);
            
        nobufcnt = 0;
        while( ( (stat = Nwrite(conno, bp, len)) == ENOBUFS) &&
               (nobufcnt < ENOBUFCNT)){
          nobufcnt++;
          YIELD();
#if NTRACE >= 1
          Nprintf("FTP.getput: Nwrite retry %d \n",nobufcnt);
#endif           
        }
        if (stat < 0) {
            Nprintf("FTP.getput: Nwrite fails %d \n",stat);
            Nrelbuf(mp);
            goto err3;
        }
    }
    Nrelbuf(mp);
    goto lab1;

err31:
    Nprintf("FTP.getput: Nread control %d\n", i1);
err3:
    Nclose(conno);
err21:
    Nclose(cconno);
err1:
    IFS_fclose(handle);
    return stat;
err4:
    buff[i1] = 0;
    Nprintf("FTP.getput: bad response: %s", buff);
    goto err21;
err2:
    Nprintf("FTP.getput: Nread control %d\n", i1);
    goto err21;
}
/*****************************************************************
				ＦＴＰファイル受信処理
*****************************************************************/
int             UsnetFTPget(char *host, char *file, int mode)
{
	FTPgetput(host, file, mode);
}
/*****************************************************************
				ＦＴＰファイル送信処理
*****************************************************************/
int             UsnetFTPput(char *host, char *file, int mode)
{
	FTPgetput(host, file, mode+2);
}
/*****************************************************************
					ＦＴＰサーバー処理
*****************************************************************/
void             UsnetFTPserver( void )
{
	FTPserv();
}
