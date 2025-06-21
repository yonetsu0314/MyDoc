/******************************************************************************
* NORTi Network TFTP Sample Header                                            *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 28/Nov/1998                                                                 *
******************************************************************************/

#ifndef NONTFTP_H
#define NONTFTP_H
#ifdef __cplusplus
extern "C" {
#endif

/* TFTP Packet */

#define TFTP_SIZE   512

typedef struct t_tftp {
    UH code;
    UH blkno;
    UB data[TFTP_SIZE];
} T_TFTP;

#define TFTP_RRQ    1
#define TFTP_WRQ    2
#define TFTP_DATA   3
#define TFTP_ACK    4
#define TFTP_ERROR  5

/* Function Prototype */

ER tftpsrv_cbk(ID, FN, VP);
TASK tftpsrv_tsk(ID);
ER tftpsrv_ini(ID, ID);

#ifdef __cplusplus
}
#endif
#endif /* NONTFTP_H */
