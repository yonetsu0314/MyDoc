/******************************************************************************
* NORTi Network Echo Service Header                                           *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Feb/1999                                                                 *
* 18/Mar/2003 Add ECHO_SSZ                                                 YS *
******************************************************************************/

#ifndef NONECHO_H
#define NONECHO_H
#ifdef __cplusplus
extern "C" {
#endif

#ifndef ECHO_SSZ
#define ECHO_SSZ 512
#endif

/* Echo Server Function Prototype */

ER udpecho_ini(ID tskid, ID cepid);
ER tcpecho_ini(ID tskid, ID cepid, ID repid);

/* Echo Client Function Prototype */

void udp_echo_client(ID cepid);
void tcp_echo_client(ID cepid);

#ifdef __cplusplus
}
#endif
#endif /* NONECHO_H */
