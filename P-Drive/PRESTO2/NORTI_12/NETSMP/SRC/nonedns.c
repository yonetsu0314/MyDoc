/******************************************************************************
* NORTi Network DNS Resolver sample                                           *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 10/Jan/2000                                                                 *
* 04/Mar/2000 Bug Fix                                                         *
* 27/Sep/2000 Little endian supported.                                        *
* 15/Oct/2002 Supported Uncompressed domain names.                            *
* 19/Apr/2003 DNS request is retryed.                                      MI *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

/* DNS Message Format */

typedef struct t_dns {
    UH    id;        /* Identifier */
    UH    flag;      /* Flags */
    UH    qdcnt;     /* Query Count */
    UH    ancnt;     /* Answer Count */
    UH    nscnt;     /* Authority Count */
    UH    arcnt;     /* Additional Count */
    UB    data[512]; /* DNS Message */
}T_DNS;

const T_UDP_CCEP dns_cep = {0, {IPV4_ADDRANY, UDP_PORTANY}, NULL};
static UH id = 0;

UB  dns_ipaddr[4];

/************************************************/
/* Get remoto host IP address by name           */
/************************************************/

ER dns_resolver(ID cepid, UW dns_ip, B *name, UW* ipaddr)
{
    T_DNS *txdns;
    T_DNS *rxdns;
    T_IPV4EP dstaddr;
    TMO tmout;
    ER ercd;
    UH i, n1, n2;
    INT len;
    UB *p1, *p2, *p3, c;

    /* Get memory buffer for DNS packet */
    ercd = get_blf(&txdns, ID_ETH_MPF);
    if (ercd != E_OK)
        return ercd;

    /* Create CEP for DNS */
    if (cepid == 0)
        ercd = cepid = udp_vcre_cep(&dns_cep);
    else
        ercd = udp_cre_cep(cepid, &dns_cep);
    if (ercd < 0) {
        rel_blf(ID_ETH_MPF, txdns);
        return ercd;
    }

    dstaddr.ipaddr = dns_ip;
    dstaddr.portno = UDP_PORT_NAMESERVER;

    /* Set DNS message header */
/*    memset(txdns, 0, sizeof(T_DNS)); */
    txdns->id = htons(id++);
    txdns->flag = htons(0x0100);
    txdns->qdcnt = htons(1);
    txdns->ancnt = 0;
    txdns->nscnt = 0;
    txdns->arcnt = 0;
    rxdns = txdns + 1;

    /* Set QNAME */
    p1 = txdns->data;
    p2 = (UB *)name;
    p3 = txdns->data + 1;
    for (n1 = n2 = 0;;) {
        c = *p2++;
        if (c == '.' || c == 0) {
            *p1 = (UB)n2;
            n1 += n2 + 1;
            if (c == 0)
                break;
            p1 = p3++;
            n2 = 0;
        }
        else {
            *p3++ = c;
            n2++;
        }
    }
    *p3++ = 0;
    *p3++ = 0;
    *p3++ = 1; /* QTYPE */
    *p3++ = 0;
    *p3++ = 1; /* QCLASS */

    /* Data size */
    p2 = (UB *)txdns;
    len = (INT)(p3 - p2);

    for (i = 10, tmout = 20/MSEC;;) {
        /* Send DNS Request */
        ercd = udp_snd_dat(cepid, &dstaddr, txdns, len, 3000/MSEC);
        if (ercd <= 0)
            goto END;

        /* Receive Answer */
        ercd = udp_rcv_dat(cepid, &dstaddr, rxdns, sizeof(T_DNS), tmout);
        if (ercd > 0)
            break;
        if (ercd != E_TMOUT)
            goto END;
        if (--i == 0)
            goto END;
        txdns->id = htons(id++);
        tmout <<= 1;
    }

    /* Opcode Check */
    if (ntohs(rxdns->flag) & 3) {
        ercd = E_PAR;
        goto END;
    }

    p1 = rxdns->data + n1 + 4 + 1; /* p1= NAME */

    /* Number of packet */
    n2 = ntohs(rxdns->qdcnt);
    n2+= ntohs(rxdns->ancnt);
    n2+= ntohs(rxdns->nscnt);
    n2+= ntohs(rxdns->arcnt);

    /* Serch data */
    for (i = 0; i < n2; i++) {
        if (*p1 >= 0xC0)
            p1 += 3; /* p1 = TYPE */
        else {
            for (; *p1 != 0; p1++)
                ; /* p1 = TYPE */
            p1 += 2;
        }
        if (*p1 == 1) {
            p1 += 4 + 4 + 1; /* p1 = RDATA */
            memcpy(ipaddr, p1, 4);
            *ipaddr = ntohl(*ipaddr);
            ercd = E_OK;
            goto END;
        }
        p1 += 4 + 4; /* p1 = RDLENGTH */
        n1 = *p1;
        p1 += n1+1;
    }
    ercd = E_OBJ;
END:
    rel_blf(ID_ETH_MPF, txdns);
    udp_del_cep(cepid);
    return ercd;
}

/* end */
