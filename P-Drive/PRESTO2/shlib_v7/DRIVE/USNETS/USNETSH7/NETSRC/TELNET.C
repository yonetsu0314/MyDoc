/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/netsrc/RCS/telnet.c 1.2 1970/01/01 00:00:00 masahiro Exp $
 * $Log: telnet.c $
 * ÉäÉrÉWÉáÉì 1.2  1970/01/01  00:00:00  masahiro
 * 2002/08/22 09:37:00
 * ÉtÉ@ÉCÉãÉVÉXÉeÉÄÅAÇbÇeÉhÉâÉCÉoé¿ëï
 * 
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
/* TELNET.C  Edition: 3  Last changed: 17-Feb-98,14:12:56  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    TELNET.C -- Telnet Terminal Emulator (client) for USNET

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
*/

#include <string.h>
/*#include <stdio.h>*/
#include "net.h"
#include "local.h"
#include "support.h"
#include "socket.h"
#include "telnet.h"
#include "fsys.h"
#include "dos_util.h"

#define ffblk find_t
#define ff_attrib attrib
#define ff_fsize size
#define ff_ftime wr_time
#define ff_fdate wr_date
#define ff_name name
#define FA_DIREC ATTR_DIR
#define FILE ST_FILE

#define termtype "teletype-33"		/* terminal type */
#define escname "Alt-X"			/* name of escape key */
#define esccode 0x2d00			/* code from escape key */

/****************************************************************/
/*	Telnet Clientóp												*/
const unsigned char mess1[]={IAC,DO,NOGOAHEAD};
const unsigned char mess2[]={IAC,WILL,TERMTYPE};
const unsigned char mess3[]={IAC,WONT,ECHO};
const unsigned char mess31[]={IAC,DO,ECHO};
const unsigned char mess4[]={IAC,SB,TERMTYPE,0};
const unsigned char mess5[]={IAC,SE};
const unsigned char mess6[]={IAC,WONT,39};
const unsigned char mess7[]={IAC,WONT,31};
const unsigned char mess8[]={IAC,WONT,0};
const unsigned char username[]={'k','e','n','g','o',CR,LF};
const unsigned char password[]={'u','s','e','r',CR,LF};
static int cmdflg = 0;
static int loginflg = 0;

extern char remotehostname[];
extern char DNSdomain[];
/****************************************************************/

/****************************************************************/
/*	Telnet Serveróp												*/
const unsigned char mess101[]={IAC,WILL,NOGOAHEAD};
const unsigned char mess102[]={IAC,SB,TERMTYPE,SEND,IAC,SE};
const unsigned char mess103[]={IAC,WONT,ECHO};
const unsigned char mess104[]={IAC,DO,TERMTYPE};
const unsigned char greet[]="ASTRA-Super Telnet server\n\r";
const unsigned char loginmsg[]="login:";
const unsigned char passmsg[]="password:";
const unsigned char iderrmsg[]="Login failure: unknown user name\n\rLogin Faied\n\r";
const unsigned char passerrmsg[]="Login failure: bad password\n\rLogin Faied\n\r";
const unsigned char welcomemsg[]="Welcome to ASTRA-Super Telnet Server\n\r";
/****************************************************************/

void	debuglog( unsigned char *msg, int len ) {
	unsigned char cmd1[]={"IAC,"};
	unsigned char cmd2[]={"SE,"};
	unsigned char cmd3[]={"SB,"};
	unsigned char cmd4[]={"WILL,"};
	unsigned char cmd5[]={"WONT,"};
	unsigned char cmd6[]={"DO,"};
	unsigned char cmd7[]={"DONT,"};
	unsigned char cmd8[]={"NUL,"};
	unsigned char cmd9[]={"LF,"};
	unsigned char cmd10[]={"CR,"};
	unsigned char cmd11[]={"BEL,"};
	unsigned char cmd12[]={"BS,"};
	unsigned char cmd13[]={"HT,"};
	unsigned char cmd14[]={"VT,"};
	unsigned char cmd15[]={"FF,"};
	unsigned char buf[10], c;
	unsigned char *ptr, *str;
	int i;

	ptr = msg;
	for( i=0;i<len;i++ ) {
		switch( *ptr ) {
			case IAC:	str=cmd1;	cmdflg = 1;	break;
			case SE:	str=cmd2;	cmdflg = 1;	break;
			case SB:	str=cmd3;	cmdflg = 1;	break;
			case WILL:	str=cmd4;	cmdflg = 1;	break;
			case WONT:	str=cmd5;	cmdflg = 1;	break;
			case DO:	str=cmd6;	cmdflg = 1;	break;
			case DONT:	str=cmd7;	cmdflg = 1;	break;
			case NUL:	str=cmd8;	cmdflg = 0;	break;
			case LF:	str=cmd9;	cmdflg = 0;	break;
			case CR:	str=cmd10;	cmdflg = 0;	break;
			case BEL:	str=cmd11;	cmdflg = 0;	break;
			case BS:	str=cmd12;	cmdflg = 0;	break;
			case HT:	str=cmd13;	cmdflg = 0;	break;
			case VT:	str=cmd14;	cmdflg = 0;	break;
			case FF:	str=cmd15;	cmdflg = 0;	break;
			default:	c = *ptr;
						if( c < 20 || cmdflg == 1 ) {
							sprintf( buf,"%d,",c);
						}
						else {
							buf[0] = c;
							buf[1] = 0;
						}
						str = buf;
						cmdflg = 0;	
						break;
		}
		Nprintf("%s",str);
		ptr++;
	}
}

int TELNETClient( void )
{
    int i1, i2, s, locix, echo, col;
    unsigned char cc, par[32], buff[128], msg[10];
    char *hnp, *cp1;
    struct NETCONF *netp;
    struct sockaddr_in socka;
    struct hostent hostent;
    struct CONNECT *conp;
	char fullname[80];

    if (Ninit() < 0)
	return -1;
    if (Portinit("*") < 0)
    {
	Nterm();
	return -1;
    }
    echo = 0;
    memset(&socka, 0, sizeof(socka));
    memset( fullname, 0, sizeof( fullname ) );
    strcpy( fullname, remotehostname );
    strcat( fullname, "." );
    strcat( fullname, DNSdomain );
	hnp = fullname;
    socka.sin_family = AF_INET;
    gethostbyname_r(hnp, &hostent, (char *)buff, sizeof(buff), &i1);
    if (i1 < 0)
    {
	Nprintf("%s not known\n", hnp);
	goto err1;
    }
    memcpy((char *)&socka.sin_addr, (char *)hostent.h_addr_list[0], Iid_SZ);
    socka.sin_port = htons(23);
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
	Nprintf("socket\n");
	goto err1;
    }
    i1 = connect(s, (struct sockaddr *)&socka, 10);
    if (i1 < 0)
    {
	Nprintf("connect\n");
	goto err1;
    }
    i1 = send(s, (char *)mess1, 3, 0);
    if (i1 != 3)
    	goto err3;
#if NTRACE >= 1
    Nprintf("TN-TX IAC %d %d\n", mess1[1], mess1[2]);
#endif
    Nprintf("key %s will get you out\n", escname);
    col = 0;
    for (;;)
    {
	SOCKET_RXTOUT(s, 0);
	i1 = recv(s, (char *)&cc, 1, 0);
	SOCKET_RXTOUT(s, 2000);
#if NTRACE >= 1
	if (i1 > 0 ) {
		debuglog(&cc,1);
	}
#endif
	if (i1 != 1)
	    goto rdkey;
	switch(cc)
	{
	case IAC:
	    i1 = recv(s, (char *)par, 2, 0);
#if NTRACE >= 1
	if (i1 > 0 ) {
		debuglog(par,2);
	}
#endif
	    if (i1 < 0)
	        goto err2;
	    switch(par[0])
	    {
	    case WILL:
			switch(par[1])
			{
				case ECHO:
		            i1 = send(s, (char *)mess31, 3, 0);
		            if (i1 != 3)
		                goto err3;
#if NTRACE >= 1
	    		    Nprintf("TN-TX IAC %d %d\n", mess31[1], mess31[2]);
#endif
		            break;
			}
	        break;
	    case WONT:
	        break;
	    case DO:
			switch(par[1])
			{
				case TERMTYPE:
		            i1 = send(s, (char *)mess2, 3, 0);
	   	         	if (i1 != 3)
	   	            goto err3;
#if NTRACE >= 1
	    		    Nprintf("TN-TX IAC %d %d\n", mess2[1], mess2[2]);
#endif
		            break;
				case 39:
		            i1 = send(s, (char *)mess6, 3, 0);
	   	         	if (i1 != 3)
	   	            goto err3;
#if NTRACE >= 1
	    		    Nprintf("TN-TX IAC %d %d\n", mess6[1], mess6[2]);
#endif
		            break;
				case 31:
		            i1 = send(s, (char *)mess7, 3, 0);
	   	         	if (i1 != 3)
	   	            goto err3;
#if NTRACE >= 1
	    		    Nprintf("TN-TX IAC %d %d\n", mess7[1], mess7[2]);
#endif
		            break;
				case 0:
		            i1 = send(s, (char *)mess8, 3, 0);
	   	         	if (i1 != 3)
	   	            goto err3;
#if NTRACE >= 1
	    		    Nprintf("TN-TX IAC %d %d\n", mess8[1], mess8[2]);
#endif
		            break;
			}
			break;
	    case DONT:
		switch(par[1])
		{
		case ECHO:
	            i1 = send(s, (char *)mess3, 3, 0);
	            if (i1 != 3)
	                goto err3;
	            break;
		}
	        break;
	    case SB:
		switch(par[1])
		{
		case TERMTYPE:
		    for (;;)
		    {
			i1 = recv(s, (char *)&cc, 1, 0);
#if NTRACE >= 1
		if (i1 > 0 ) {
			debuglog(&cc,1);
		}
#endif
	    	        if (i1 != 1)
	        	    goto err2;
			if ((unsigned char)cc == (unsigned char)SE)
			    break;
		    }
	            i1 = send(s, (char *)mess4, sizeof(mess4), 0);
	            if (i1 != sizeof(mess4))
	                goto err3;
	            i1 = send(s, (char *)termtype, sizeof(termtype)-1, 0);
	            if (i1 != sizeof(termtype)-1)
	                goto err3;
	            i1 = send(s, (char *)mess5, sizeof(mess5), 0);
	            if (i1 != sizeof(mess5))
	                goto err3;
		    break;
		}
		break;
	    }
	    break;
	case 0x00:			/* null */
	case 0x07:			/* bell */
	    break;
	case 0x08:			/* backspace */
	    if (col)
	    {
		col--;
	    }
	    break;
	case 0x09:
	    while (col & 7)		/* horizontal tab */
	    {
		col++;
	    }
	    break;
	case 0x0a:			/* line feed */
	    break;
	case 0x0b:			/* home */
	    col = 0;
	    break;
	case 0x0c:			/* clear screen and home */
	    col = 0;
	    break;
	case 0x0d:			/* carriage return */
	    col = 0;
	    break;
	case 0x1d:			/* clear line */
	    col = 0;
	    break;
	case 0x1e:
	    break;
	default:
		if( cc == 'l' ) {
			i1 = recv(s, (char *)buff, 5, 0);
#if NTRACE >= 1
			if (i1 > 0 ) {
				debuglog((char *)buff,i1);
			}
#endif
			if( memcmp( (char *)buff, "ogin:", 5 ) == 0 ) {
				loginflg=1;
			    i1 = send(s, (char *)username, sizeof(username), 0);
		   		if (i1 != sizeof(username)) {
   			    	goto err3;
				}
			}
	    }
		if( cc == 'p' ) {
			i1 = recv(s, (char *)buff, 8, 0);
#if NTRACE >= 1
			if (i1 > 0 ) {
				debuglog((char *)buff,i1);
			}
#endif
			if( memcmp( (char *)buff, "assword:", 8 ) == 0 ) {
				loginflg=1;
			    i1 = send(s, (char *)password, sizeof(password), 0);
		   		if (i1 != sizeof(password)) {
   			    	goto err3;
				}
			}
	    }
		break;
	}
rdkey:
	while (Nchkchr())
	{
	    cc = i1 = Ngetchr();
	    if (i1 == esccode)
		goto ret9;
	    if (echo)
            send(s, (char *)&cc, 1, 0);
	    if (cc == '\r')
                send(s, "\n", 1, 0);
	}
    	YIELD();
    }
ret9:
    closesocket(s);
    Nterm();
    return 0;
err3:
    Nprintf("send\n");
    goto err8;
err2:
    Nprintf("recv\n");
err8:
    closesocket(s);
err1:
    Nterm();
    return -1;
}

/* dummy command processor */

static char TelnetDirExec( char *name );
static char TelnetCdExec( char *name );
static char TelnetMkdirExec( char *name );
static char TelnetRdExec( char *name );

typedef struct {
	char	com;
	char	str[5];
	char	(*func)(char *);
} TELCOM;

enum {
	TEL_COM_DIR = 0,
	TEL_COM_CD,
	TEL_COM_MKDIR,
	TEL_COM_RD,
	TEL_COM_END
};
static TELCOM	Tel_com_tbl[] = {
	{	TEL_COM_DIR,	"dir",		TelnetDirExec	},
	{	TEL_COM_CD,		"cd",		TelnetCdExec	},
	{	TEL_COM_MKDIR,	"mkdir",	TelnetMkdirExec	},
	{	TEL_COM_RD,		"rd",		TelnetRdExec	},
	{	TEL_COM_END,	"   ",		NULL			}
};
static int ns;
static int (*TelnetFunc)( char * );

static char TelnetCdExec( char *name ){
	return chdir(name);
}

static char buff[1024];
static char TelnetDirExec( char *name ){
    struct ffblk    ffblk;
	int		i,i1,i2;
    char	cc;
	i = 1;
	for (;;) {
		if (i == 1) {
 			i = findfirst("*.*", 0xff, &ffblk);
 		} else {
 			i = findnext(&ffblk);
		}
		if (i<0) {
 			break;
		}
		memset(buff, ' ', 80);
		memcpy(buff, ffblk.ff_name, strlen(ffblk.ff_name));
 		if (ffblk.ff_attrib & FA_DIREC) {
  			memcpy(buff + 13, "<DIR>", 5);
 		}
		else {
 			memcpy(buff + 13, "     ", 5);
		}
		i1 = ffblk.ff_ftime >> 11;
		cc = i1 < 12 ? 'a' : 'p';
 		if (i1 > 13)
 			i1 -= 12;
		Nsprintf(buff + 18, "%9ld %02d-%02d-%02d  %02d:%02d%c",
				ffblk.ff_fsize, (ffblk.ff_fdate >> 5) & 0xf,
				ffblk.ff_fdate & 0x1f, (ffblk.ff_fdate >> 9) + 1980,
				i1, (ffblk.ff_ftime >> 5) & 0x3f, cc);
		strcat(buff, "\r\n");
		send(ns, buff, strlen(buff), 0);
	}
	return 0;
}

/*---------------------------------------------------------------
						MDÉRÉ}ÉìÉhèàóù
----------------------------------------------------------------*/
static char TelnetMkdirExec( char *name ){
	return mkdir(name);
}
/*---------------------------------------------------------------
						RDÉRÉ}ÉìÉhèàóù
----------------------------------------------------------------*/
static char TelnetRdExec( char *name ){
	return rmdir(name);
}

/*****************************************************************
			ÇsÇdÇkÇmÇdÇsÉRÉ}ÉìÉhèàóùä÷êîìoò^èàóù
*****************************************************************/
int TELNETSetFunc( int(*func)(char *cmd) ) {
	TelnetFunc=func;
}
/*****************************************************************
				ÇsÇdÇkÇmÇdÇsÉfÉoÉbÉOï\é¶èàóù
*****************************************************************/
int TELNETPrint( char *msg ) {
	int err;
	if( loginflg>=3 ) {
	    err = send(ns, msg, strlen(msg), 0);
	}
}

/*---------------------------------------------------------------
				ÉJÉåÉìÉgÉfÉBÉåÉNÉgÉäï\é¶èàóù
----------------------------------------------------------------*/
static void printdir( void )
{
	memset( buff,0,sizeof(buff) );
    if( getcwd(buff, sizeof(buff) ) == NULL ) {
		return;
	}
	buff[strlen(buff)] = '>';
	buff[strlen(buff)+1] = 0x00;

	send(ns, buff, strlen(buff), 0);
}
/*---------------------------------------------------------------
				ÇsÇdÇkÇmÇdÇsÉTÅ[ÉoÅ[ÉRÉ}ÉìÉhèàóù
----------------------------------------------------------------*/
static void command(int ns, char *name)
{
	int i, next, err;
	char *ptr;

	memset( buff,0,sizeof(buff) );
	strcpy( buff, name );
	ptr = buff;
	next = 0;
	for( i=0; i<strlen(name); i++ ) {
		if( *ptr == ' ' ) {
			*ptr = 0x00;
			next = 1;
		}
		else if( next == 1 ) {
			break;
		}
		ptr++;
	}
	send(ns, "\r\n", 2, 0);
	err = 1;
	for( i=0; i<TEL_COM_END; i++ ) {
		if( !(strcmp( Tel_com_tbl[i].str, buff ) ) ){
			err = Tel_com_tbl[i].func(ptr);
			break;
		}
	}
	if( i>=TEL_COM_END ) {
		if( TelnetFunc!=NULL ) {
			err = TelnetFunc(name);
		}
		else {
			send(ns, "command not supported.\r\n", 24, 0);
		}
	}
	if( err < 0 ) {
		send(ns, "command failed.\r\n", 17, 0);
	}
	else if( err == 0 ) {
		send(ns, "command successful.\r\n", 21, 0);
	}
	printdir();
}
/*****************************************************************
				ÇsÇdÇkÇmÇdÇsÉTÅ[ÉoÅ[èàóù
*****************************************************************/
int TELNETServ( void )
{
    int i1, s, echo;
    char *comp, par[32];
    unsigned char cc;
    struct sockaddr_in socka;
    struct CONNECT *conp;

build:
    loginflg=0;

    echo = 0;
    memset(&socka, 0, sizeof(socka));
#if NTRACE >= 1
    Nprintf("running as server\n");
#endif
    socka.sin_port = htons(23);
    s = socket(PF_INET, SOCK_STREAM, 0);
    if (s < 0)
    {
	Nprintf("socket\n");
	goto err1;
    }
    i1 = bind(s, (struct sockaddr *)&socka, sizeof(socka));
    if (i1 < 0)
    {
	Nprintf("bind\n");
	goto err1;
    }
    i1 = listen(s, 10);
    if (i1 < 0)
    {
	Nprintf("listen\n");
	goto err1;
    }
    memset(&socka, 0, sizeof(socka));
    i1 = sizeof(socka);
    ns = accept(s, (struct sockaddr *)&socka, &i1);
    if (ns < 0)
    {
        Nprintf("accept\n");
        goto err1;
    }
    i1 = send(ns, (char *)mess104, 3, 0);
    if (i1 != 3)
	goto err3;
#if NTRACE >= 1
	Nprintf("TN-TX IAC %d %d\n", mess104[1], mess104[2]);
#endif
    send(ns, (char *)greet, sizeof(greet)-1, 0);
    send(ns, (char *)loginmsg, sizeof(loginmsg)-1, 0);
    loginflg=1;
	comp = par;
    for (;;)
    {
	SOCKET_RXTOUT(ns, 300);
	i1 = recv(ns, (char *)&cc, 1, 0);
	SOCKET_RXTOUT(ns, 2000);
#if NTRACE >= 1
	if (i1 > 0 ) {
		debuglog(&cc,1);
	}
#endif
	if (i1 != 1)
	{
	    if (i1 == 0)		/* connection closed, quit */
	    {
		Nprintf("connection closed\n");
		goto restart;
	    }
	    if (errno != ETIMEDOUT)
	    {
		Nprintf("connection broken\n");
		goto restart;
	    }
	    goto cont;			/* timeout, loop back */
	}
	switch(cc)
	{
	case IAC:
	    i1 = recv(ns, par, 2, 0);
#if NTRACE >= 1
	if (i1 > 0 ) {
		debuglog(&par,2);
	}
#endif
	    if (i1 != 2)
	        goto err2;
#if NTRACE >= 1
/*	    Nprintf("TN-RX IAC %d %d\n", par[0], par[1]);*/
#endif
	    switch(par[0])
	    {
	    case WILL:
		switch(par[1])
		{
		case TERMTYPE:
	            i1 = send(ns, (char *)mess102, sizeof(mess102), 0);
	            if (i1 != sizeof(mess102))
	                goto err3;
#if NTRACE >= 1
	    	    Nprintf("TN-TX IAC %d %d %d\n",mess102[1],mess102[2],mess102[3]);
#endif
		    break;
		}
	        break;
	    case WONT:
	        break;
	    case DO:
		switch(par[1])
		{
		case NOGOAHEAD:
	            i1 = send(ns, (char *)mess101, 3, 0);
	            if (i1 != 3)
	                goto err3;
#if NTRACE >= 1
	    	    Nprintf("TN-TX IAC %d %d\n", mess101[1], mess101[2]);
#endif
	            break;
		case TERMTYPE:
	            break;
		}
		break;
	    case DONT:
		switch(par[1])
		{
		case ECHO:
	            i1 = send(ns, (char *)mess103, 3, 0);
	            if (i1 != 3)
	                goto err3;
#if NTRACE >= 1
	    	    Nprintf("TN-TX IAC %d %d\n", mess103[1], mess103[2]);
#endif
	            break;
		}
	        break;
	    case SB:
		switch(par[1])
		{
		case TERMTYPE:
#if NTRACE >= 1
/*		    Nprintf("TN-RX");*/
#endif
		    for (;;)
		    {
			i1 = recv(ns, (char *)&cc, 1, 0);
#if NTRACE >= 1
	if (i1 > 0 ) {
		debuglog(&cc,1);
	}
#endif
	    	        if (i1 != 1)
	        	    goto err2;
#if NTRACE >= 1
/*			Nprintf(" %x", cc);*/
#endif
			if (cc == SE)
			    break;
		    }
#if NTRACE >= 1
		    Nprintf("\n");
#endif
		    send(ns, "USNET Telnet Server\r\n", 22, 0);
		    break;
		}
		break;
	    }
	    break;
	default:

	    switch( loginflg ) {
			case 1:
		        i1 = send(ns, (char *)&cc, 1, 0);
        		break;
			case 2:
		        i1 = send(ns, "*", 1, 0);
        		break;
			case 3:
		        i1 = send(ns, (char *)&cc, 1, 0);
        		break;
		}
		if (i1 != 1)
			goto err3;

	    if (cc == '\n' || cc == '\r')		/* end of line */
	    {
	    	*comp = 0;
		if (comp != par)
		    switch( loginflg ) {
				case 1:
					send(ns, "\r\n", 2, 0);
					if( !(strcmp( USERID, par )) ) {
					    send(ns, (char *)passmsg, sizeof(passmsg)-1, 0);
						loginflg=2;
					}
					else {
					    send(ns, (char *)iderrmsg, sizeof(iderrmsg)-1, 0);
					    send(ns, (char *)loginmsg, sizeof(loginmsg)-1, 0);
						loginflg=1;
					}
					break;
				case 2:
					send(ns, "\r\n", 2, 0);
					if( !(strcmp( PASSWD, par )) ) {
					    send(ns, (char *)welcomemsg, sizeof(welcomemsg)-1, 0);
						loginflg=3;
						printdir();
					}
					else {
					    send(ns, (char *)passerrmsg, sizeof(passerrmsg)-1, 0);
					    send(ns, (char *)loginmsg, sizeof(loginmsg)-1, 0);
						loginflg=1;
					}
					break;
				case 3:
					command(ns, par);
					break;
			}
			comp = par;
	    }
	    else if (cc == ('H'&0x1f))			/* backspace */
	    {
		if (comp != par)
		    *(--comp) = 0;
	    }
	    else if (cc != 0)
	    {
	        if (comp >= par + sizeof(par) - 1)
	            comp = par;
	        *comp++ = cc;
	    }
	    break;
	}
cont:	YIELD();
    }
	
err3:
    Nprintf("send\n");
    goto err8;
err2:
    Nprintf("recv\n");
err8:
    closesocket(ns);
    closesocket(s);
err1:
	goto build;

restart:
    closesocket(ns);
    closesocket(s);
	goto build;
}
