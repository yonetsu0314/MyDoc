/*======================================================================
 * File name	: usnet.h
 * Original 	: 
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/drive/RCS/usnet.h 1.1 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: usnet.h $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  sakaguti
 * usnetÇÃsocketä÷êî
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
 */

void UsnetSetLocalIpAddress( char *buf );
void UsnetSetSubnetMask( char *buf );
void UsnetSetGwAddress( char *buf );
void UsnetSetRemoteIpAddress( char *buf );
void UsnetSetUserID( char *buf );
void UsnetSetPassWord( char *buf );
void UsnetSetMacAddress( char *buf );

int		UsnetSocket_closesocket(int);
int		UsnetSocket_readsocket(int s, char *buf, int len);
int		UsnetSocket_writesocket(int s, char *buf, int len);
int		UsnetSocket_socket(int domain, int type, int protocol);
int		UsnetSocket_bind(int s, struct sockaddr_in * name, int namelen);
int		UsnetSocket_listen(int s, int backlog);
int		UsnetSocket_accept(int s, struct sockaddr_in * name, int *namelen);


void UsnetFTPserver( void );



