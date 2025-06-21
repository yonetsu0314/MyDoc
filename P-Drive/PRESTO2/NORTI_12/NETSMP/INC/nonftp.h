/******************************************************************************
* NORTi Network FTP Header                                                    *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 26/Feb/1999                                                                 *
* 09/Jun/1999 FTPDATA_RBUFSZ 2048 -> 2920 (MTU - header size) * 2             *
* 12/Feb/2001 Change Task priority 4 -> 5  FTP_SERVER_PRI,FTP_CLIENT_PRI      *
* 10/Apr/2002 FTPDATA_SBUFSZ, FTPDATA_RBUFSZ and FTP_BUFSZ were enlarged.     *
* 18/Mar/2003 FTP_SERVER_SSZ and FTP_CLIENT_SSZ became configurable        YS *
******************************************************************************/

#ifndef NONFTP_H
#define NONFTP_H
#ifdef __cplusplus
extern "C" {
#endif

/* Constant */

#define FTP_SERVER_PRI  5       /* FTP Server Task Priority */
#define FTP_CLIENT_PRI  5       /* FTP Client Task Priority */
#ifndef FTP_SERVER_SSZ
#define FTP_SERVER_SSZ  512     /* FTP Server Task Stack Size */
#endif
#ifndef FTP_CLIENT_SSZ
#define FTP_CLIENT_SSZ  512     /* FTP Client Task Stack Size */
#endif

#define FTP_SBUFSZ      128     /* Send    Buffer Size for Control */
#define FTP_RBUFSZ      128     /* Receive Buffer Size for Control */
#define FTPDATA_SBUFSZ  4380    /* Send    Buffer Size for Data Transfer */
#define FTPDATA_RBUFSZ  4380    /* Receive Buffer Size for Data Transfer */

#define FTP_USER_LEN    10      /* FTP User Name Length */
#define FTP_BUFSZ       1460    /* FTP File Read/Write Buffer Size */

/* FTP Control Block */

typedef struct t_ftp {
    ER ercd;
    ID tskid;
    ID cepid1;
    ID cepid2;
    ID repid;
    ID repid2;
    T_IPV4EP myaddr;
    T_IPV4EP dstaddr;
    UB flag;
    B user[FTP_USER_LEN+1];
    FILE *fp;
    B buf[FTP_BUFSZ];
    FP callback;
    B sbuf1[FTP_SBUFSZ];
    B rbuf1[FTP_RBUFSZ];
    B sbuf2[FTPDATA_SBUFSZ];
    B rbuf2[FTPDATA_RBUFSZ];
    ID bufid; /* Add by Y.Y */
} T_FTP;

#define FTP_LOGGED_IN   0x01
#define FTP_DTP_OPEN    0x02
#define FTP_RETR        0x04
#define FTP_STOR        0x08
#define FTP_LIST        0x10
#define FTP_NLST        0x20
#define FTP_QUIT        0x40

#define FTP_READY       0x04
#define FTP_PASSWD_REQ  0x08
#define FTP_OPTION_V    0x10    /* Suppresses Display of Daemon Responses */
#define FTP_OPTION_D    0x20    /* Enables Debugging */
#define FTP_CLOSED      0x80

/* Callback Routine for Password Check  */

typedef B *(*FTP_CALLBACK)(T_FTP *, const B *, const B *);

/* FTP Service Call Prototype */

ER ftp_cre_srv(T_FTP *ftp, ID tskid, ID cepid1, ID cepid2, ID repid, FTP_CALLBACK callback);
ER ftp_sta_srv(T_FTP *ftp);
ER ftp_client(T_FTP *ftp, ID cepid1, ID cepid2, ID repid, UB option, UW ipaddr);
int ftp_command(int argc, char *argv[]);

/* MT用 */

#define FTP_MAX_MEM_BLOCK 2

typedef struct {
    VP    base;      /* FTPメモリ先頭アドレス */
    UW    size;      /* FTPメモリサイズ */
    UW    use_size;  /* 使用中のメモリ（Byte単位) */
    B     fname[13]; /* ファイル名 */
} T_FTP_MEM_CTL;

extern T_FTP_MEM_CTL ftp_mem_ctl[FTP_MAX_MEM_BLOCK];

#ifdef __cplusplus
}
#endif
#endif /* NONFTP_H */
