/********************************************************************
*                                                                   *
*   NORTi Network IP reassembling Module                            *
*                                                                   *
*   Copyright (c) 1998-2003, MiSPO Co., Ltd.                        *
*   All rights reserved.                                            *
*                                                                   *
*   Filename : nontipf.c                                            *
*   31/JAN/2002  A.K - Created                                      *
*   01/Aug/2002  A.K - Fixed memory releasing problems              *
*   28/Aug/2002  A.K - Modified for Fixed memory pool               *
*   24/Feb/2003  A.K - Modified for SNMP                            *
********************************************************************/

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
#ifdef PPP
#include "noneppp.h"
#endif

BOOL ipf_timer_flag;

/* Memory pool reference */
T_RMPF rmpf;

extern ER ip_internal_error(ER ercd);
/************************************************************************************
* Module Name   : ipqhashfn                                                         *
* Purpose       : This funtion calulates the hash queue index value of the incoming *
*                 fragment using the source address, destination address,           *
*                 identification and protocol values of the fragment.               *
* Arguments     : UH id     - Identification value of the fragment.                 *
*                 UW saddr  - Source address of the fragment.                       *
*                 UW daddr  - Destinatin address of the fragment.                   *
*                 UB prot   - Protocol value of the fragment.                       *
* Return value  : Index of hash queue array.                                        *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
************************************************************************************/
UW ipqhashfn(UH id, UW saddr, UW daddr, UB prot)
{
    UW hash;                                /* hash index value */
    hash = ((id ^ saddr ^ daddr ^ prot) & (IPHASH_QSZ - 1));
    return hash;
}

/************************************************************************************
* Module Name   : search_ip_hashque                                                 *
* Purpose       : This funtion searches the hash queue using the hash key for the   *
*                 incoming fragment.                                                *
* Arguments     : T_IP_HEADER *iph - IP header of the fragment.                     *
* Return value  : Hash queue pointer.                                               *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
************************************************************************************/
struct iphash_que *search_ip_hashque(T_IP_HEADER *iph)
{
    struct iphash_que *p_hq;
    UW saddr, daddr, hash;
    UH id;
    UB protocol;

    id       = iph->id;                  /* Identification value of the IP packet*/
    saddr    = iph->sa;                  /* Source address of the IP packet      */
    daddr    = iph->da;                  /* Destination address of the IP packet */
    protocol = iph->prot;                /* Protocol value of the IP packet      */

    /* Get the hash index value */
    hash = ipqhashfn(id, saddr, daddr, protocol);

    /* Search the hash queue entry for the fragment */ 
    for (p_hq = ipq_hash[hash]; p_hq; p_hq = p_hq->next) {
        if (p_hq->iph->id == id      &&
           p_hq->iph->sa == saddr   &&
           p_hq->iph->da == daddr   &&
           p_hq->iph->prot == protocol) {
            break;                          /* Hash queue found */
        }
    }
    return p_hq;                            /* Return the hash queue pointer */
}

/************************************************************************************
* Module Name   : get_ip_hashque                                                    *
* Purpose       : This funtion allocates memory for a new reassembly process and    *
*                 initializes the IP header data and the reassembly timer.          *
* Arguments     : T_IP *pkt         - IP packet.                                    *
*                 T_IP_HEADER *iph  - Header of the fragment.                       *
* Return value  : Hash queue pointer.                                               *
* Author        : AK                                                                *
* Date          : 31-JAN-2002 Created                                               *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
************************************************************************************/

struct iphash_que *get_ip_hashque(T_IP *pkt, T_IP_HEADER *iph)
{
    struct iphash_que *p_hq;                /* Pointer for hash queue */
    UW hash;                                /* Hash queue index */
    UW ihlen;                               /* IP header length of the fragment */
    ER ercd;                                /* Error code */

    /* Get memory for the hash queue */
    p_hq = NULL;
    ercd = pget_blf((VP *)&p_hq, ID_ETH_MPF);
    if (ercd < 0) {
        ip_internal_error(ercd);
        return NULL;
    }
    memset(p_hq, 0, sizeof(struct iphash_que));
 
    /* Get the header length */
    ihlen = (iph->ver & IP_VER) * 4;

    /* Fill header information in hash que */
    p_hq->iph = &pkt->ip;
    p_hq->len = 0;
    p_hq->ihlen = ihlen;
    p_hq->fragments = NULL;

    /* Reassembly time out value */
    p_hq->rtime = IPF_REASM_TMO;

    /* Get the hash que index for the fragment */
    hash = ipqhashfn(iph->id, iph->sa, iph->da, iph->prot);

    /* Enter the new hash queue in hash queue array */
    if((p_hq->next = ipq_hash[hash]) != NULL)
        p_hq->next->pprev = &p_hq->next;

    ipq_hash[hash] = p_hq;
    p_hq->pprev = &ipq_hash[hash];

    /* Return the hash queue */
    return p_hq;
}

/************************************************************************************
* Module Name   : receive_all_frag                                                  *
* Purpose       : This funtion checks whether all fragments are arrived or not.     *
* Arguments     : iphash_que *p_hq - Pointer to the hash queue.                     *
* Return value  : Returns TRUE if all pacekts received otherwise returns FALSE.     *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
************************************************************************************/
UW receive_all_frag(struct iphash_que *p_hq)
{  
    T_IPF *p_frag;
    UW offset;                              /* Offset value of the fragments  */

    if (p_hq->len == 0)                     /* Final fragment not received */
        return 0;

    p_frag = (T_IPF *)p_hq->fragments;      /* Get the first fragment in hash queue */
    offset = 0;                             /* Set initial offset as zero */

    while (p_frag) {
        if (p_frag->offset > offset)
            return (0);                     /* Return FALSE if fragment missing */

        offset = p_frag->end;
        p_frag = (T_IPF *)p_frag->nextf;
    }
    return TRUE;                            /* Return TRUE if all fragments present */
}

/************************************************************************************
* Module Name   : make_ip_pkt                                                       *
* Purpose       : This funtion combines all the fragments present in the hash queue *
*                 as a single IP packet.                                            *
* Arguments     : iphash_que *p_hq - Pointer to the hash queue.                     *
* Return value  : IP packet on success else returns NULL.                           *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
************************************************************************************/
T_IP *make_ip_pkt(struct iphash_que *p_hq)
{
    UW len;
    UB proto;

    proto = p_hq->fragments->ip.prot;
    if (proto != PROT_ICMP && proto != PROT_UDP)
        return NULL;

    len = p_hq->ihlen + p_hq->len;          /* Get the length of the total data */

    if (len <= PATH_MTU && proto == PROT_ICMP)
        return NULL;

    p_hq->frag2 = ((T_IPF *)p_hq->fragments)->nextf;
    if (p_hq->frag2 == NULL) return NULL;
    p_hq->fragments->ip.fo = 0;
    p_hq->fragments->ip.tl = htons(len);
    p_hq->fragments->ctl.rcheck = TRUE;     /* A reassembly packet */
    p_hq->fragments->ctl.hedflg = FALSE;

    /* Reassembly time out value */
    p_hq->rtime = (W)-1;
    p_hq->fragments->ctl.cep = p_hq;

    return p_hq->fragments;
}

/************************************************************************************
* Module Name   : clr_ip_hashque                                                    *
* Purpose       : This funtion frees memory of fragment packets and also for the    *
*                 reassembly process.                                               *
* Arguments     : iphash_que *p_hq - Pointer to the hash queue.                     *
* Return value  : None.                                                             *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
************************************************************************************/
void clr_ip_hashque(struct iphash_que *p_hq)
{
    T_IP *p_frag, *temp;
    ER ercd;

    if (!p_hq)              /* hash queue not exists */
        return;
    
    if (p_hq->rtime < 0)    /* hash queue used by other functions */
        return;

    /* Seperate this hash queue from hash queue array */
    if (p_hq->next)
        p_hq->next->pprev = p_hq->pprev;

    *p_hq->pprev = p_hq->next;
    
    /* Release the fragments */
    p_frag = p_hq->frag2;
    while (p_frag) {
        temp = ((T_IPF *)p_frag)->nextf;
        if (p_frag) {
            ercd = rel_blf(ID_ETH_MPF,(VP)p_frag);
            p_frag = NULL;
            if ( ercd != E_OK) 
                ip_internal_error(ercd);
        }
        p_frag = temp;
    }

    /* Release the first fragment */
    p_frag = p_hq->fragments;
    if (p_frag) {
        ercd = rel_blf(ID_ETH_MPF,(VP)p_frag);
        p_frag = NULL;
        if (ercd != E_OK) 
            ip_internal_error(ercd);
    }

    /* Release hash que */
    if (p_hq)
       ercd = rel_blf(ID_ETH_MPF, (VP)p_hq);

    p_hq = NULL;
}

/************************************************************************************
* Module Name   : ip_evictor                                                        *
* Purpose       : This funtion releases old reassembly processes until required     *
*                 memory space become free.                                         *
* Arguments     : None.                                                             *
* Return value  : None.                                                             *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
************************************************************************************/
void ip_evictor(void)
{
    struct iphash_que *p_hq;
    UW index, check;
    
    index = 0;
begin:                                       /* check all hash queue entries */
    check = 0;
    ref_mpf(&rmpf, ID_ETH_MPF);
    if (rmpf.frbcnt > IPF_UMEM_BLK)          /* Return if required memory free */
        return;

    for (; index < IPHASH_QSZ; index++) {
        p_hq = ipq_hash[index];
        if (p_hq) {
            while (p_hq->next)               /* find the oldest queue */
                p_hq = p_hq->next;

            clr_ip_hashque(p_hq);            /* clear the hash */
            ref_mpf(&rmpf, ID_ETH_MPF);
            index++;
            check = 1;
        }
        if (check)
            goto begin;
    }
}

/************************************************************************************
* Module Name   : ip_frag_timeout (Cyclic timer handler)                            *
* Purpose       : This cyclic handler checks the reassemble time for all reassembly *
*                 process. If time out it releases the process and send a ICMP      *
*                 timeout error message to the remote end                           *
* Arguments     : None.                                                             *
* Return value  : TMRHDR handler for the timer.                                     *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
*                 24-Feb-2002 This func called by ARP timer for every 1 second      *
************************************************************************************/
void ip_frag_timeout(void)
{
    struct iphash_que *hash_que, *temp;
    T_ICMP_HEADER ficmp;
    UW index, ipaddr;
    UB reset;

    reset = 0;                                          /* Check for timeout occurrence */
                                                        /* Check all hash queue for timeout*/
    for (index = 0; index < IPHASH_QSZ; index++) {
        temp = ipq_hash[index];                         /* Get the exists hash que */
        while (temp) {
                hash_que = temp;
                temp = hash_que->next;
                reset = 1;                              /* Set reset flag */
                if (hash_que && (hash_que->rtime >= 0)) {
                    hash_que->rtime--;                  /* Reduce reassembly timeout */
                    if (hash_que->rtime <= 0) {         /* if timeout occurs */
                        /* Construct and send ICMP timeout message to the remote host */
                        ficmp.type = 11;                /* ICMP type = timeout */
                        ficmp.code = 1;                 /* ICMP code =  timeout during reassembling */
                        ficmp.id   = hash_que->iph->id; 
                        ipaddr = ntohl(hash_que->iph->sa);
                        icmp_snd_dat(IPV4_ADDRANY, ipaddr, &ficmp, hash_que->iph, 64);
                        clr_ip_hashque(hash_que);       /* clear the hash queue */
                    }   
                }
        }
    } 

    /* Disable reassembly timer if no hash que exists */
    if (reset == 0)
        ipf_timer_flag = FALSE;
}

/************************************************************************************
* Module Name   : ip_reassembling                                                   *
* Purpose       : This funtion receives the incoming fragmented packets and make as *
*                 single IP packet if all the fragments arrived with in timeout.    *
* Arguments     : T_IP *pkt - Pointer to the fragment.                              *
* Return value  : Returns the reassembled IP packet else retunrs NULL.              *
* Author        : AK                                                                *
* Date          : 31-JAN-2002                                                       *
*               : 28-Aug-2002 Modified for Fixed Memory pool                        *
*                 24-Feb-2002 Modified for SNMP.                                    *
*                 24-Feb-2002 Timer initialization code are removed, ARP timer is   *
*                              replaced for this.                                   *
************************************************************************************/

T_IP *ip_reassembling(T_IP *pkt)
{
    T_IP *prev, *next, *tp_frag;            /* Pointer for hash queue fragments */
    T_IP_HEADER *iph;                       /* Pointer to IP header of incoming fragment */
    T_IPF *tmp;                             /* Pointer to fragment */ 
    struct iphash_que *p_hq;                /* Pointer to hash queue */
    UW ihl, flags, offset;                  /* Values of IP header fields */
    UW datapos, end;                        /* Data processing variables */
    UB *ptr;                                /* Pointer to fragment Data */
    ER ercd;                                /* Error code */

    SNMP_BYPCNT(snmp_mibbyp.ipReasmReqds, 1);       /* ipReasmReqds */

    /* If not enough memory then release old incomplet fragments */
    ercd = ref_mpf(&rmpf, ID_ETH_MPF);
    if (ercd != E_OK || rmpf.frbcnt <= IPF_MREQ_BLK) {
#ifndef PPP
        lan_ignore_pkt();
#endif
        ip_evictor();
        ip_internal_error(ercd);
        return NULL;
    }

    /* IP reassembly in progress */
    if (ipf_timer_flag == FALSE)
        ipf_timer_flag = TRUE;

    /* Process fragment IP header */
    iph = &pkt->ip;                         /* Pointer to IP header of incoming fragment */

    offset = ntohs(iph->fo);                /* Get the offset value of the fragment */
    flags = offset & ~IP_OFFSET;
    offset &= IP_OFFSET;
    offset <<= 3;       
    ihl = (iph->ver & IP_VER) * 4;          /* Get the header length of the fragment */

    /* Find hash que for this fragment */
    p_hq = NULL; 
    p_hq = search_ip_hashque(iph);          /* Searches a hash queue for the fragment */

    if (p_hq) {                             /* hash queue exists */ 
        if (offset == 0) {                  /* First fragment */
            if ((flags & IP_MF) == 0) {     /* More fragment flag not set */
#ifndef PPP
                lan_ignore_pkt();
#endif
                clr_ip_hashque(p_hq);
                return NULL;
            }
            p_hq->ihlen = ihl;              /* Copy the header information */
            p_hq->iph = &pkt->ip;
            memcpy(p_hq->data, ((UB *)pkt) + 4, sizeof(T_CTL_HEADER) + sizeof(T_ETH_HEADER));
        }
    } 
    else {                                  /* hash queue not exists */
        if ((offset == 0) && ((flags & IP_MF) == 0)) {
#ifndef PPP
            lan_ignore_pkt();
#endif
            return NULL;
        }   

        if (rmpf.frbcnt < (IPF_MREQ_BLK + 1)) { /* No memory for new process */
#ifndef PPP
            lan_ignore_pkt();
#endif
            return NULL;
        }

        p_hq = get_ip_hashque(pkt, iph);    /* Create hash queue for the fragment */
        if (!p_hq) {
#ifndef PPP
            lan_ignore_pkt();
#endif
            return NULL;
        }   
        memcpy(p_hq->data, ((UB *)pkt) + 4, sizeof(T_CTL_HEADER) + sizeof(T_ETH_HEADER));
    }

    pkt->ctl.cep = p_hq;        /* added */

    /* Ignore any invalid length fragment */
    if ((ntohs(iph->tl) + ((UW) offset)) > 65535) {
#ifndef PPP
        lan_ignore_pkt();
#endif
        clr_ip_hashque(p_hq);
        SNMP_BYPCNT(snmp_mibbyp.ipReasmFails, 1);   /* ipReasmFails */
        return NULL;
    }

    /* Get the last data byte for position check */
    end = offset + ntohs(iph->tl) - ihl;

    /* Get the total length from last fragment */
    if ((flags & IP_MF) == 0)
        p_hq->len = end;

    /* Read the data of IP fragment */
    datapos = ntohs(iph->tl) - ihl;

#ifndef PPP
    /* 25Apr2003 - Should not read more than PATH_MTU */
    if (!lan_read_pkt_end(pkt->data, (int)datapos, PATH_MTU)) {
        if (p_hq) {
            clr_ip_hashque(p_hq);
            SNMP_BYPCNT(snmp_mibbyp.ipReasmFails, 1);   /* ipReasmFails */
        }    
        return pkt; 
    }
#else
    if (!ppp_read_pkt_end(pkt->data, (int)datapos, datapos)) {
        if (p_hq) {
            clr_ip_hashque(p_hq);
            SNMP_BYPCNT(snmp_mibbyp.ipReasmFails, 1);   /* ipReasmFails */
        }    
        return pkt; 
    }
#endif

    /* Pointer fragment data */
    ptr = (UB *)pkt->data;
    
    /* Get the position of the fragment in the hash queue */
    prev = NULL;
    for (next = p_hq->fragments; next != NULL; next = ((T_IPF*)next)->nextf) {
        if (((T_IPF *)next)->offset >= offset)
            break;  
        prev = next;
    }

    /* Check for any overlap with earlier received fragments */
    if ((prev != NULL) && (offset < ((T_IPF *)prev)->end)){
        datapos = ((T_IPF *)prev)->end - offset;
        offset += datapos;  
        ptr += datapos;
    }    

    tp_frag = NULL;
    for (tmp = (T_IPF *)next; tmp != NULL; tmp = (T_IPF *)tp_frag) {
        tp_frag = tmp->nextf;
        if (tmp->offset >= end)
            break;                          /* No overlap */

        /* datapos bytes are overlapped */
        datapos = end - ((T_IPF *)next)->offset; 
        tmp->len -= datapos;                /* Reduce size */
        tmp->offset += datapos;             /* Next fragment */
        tmp->ptr += datapos;

        /* Whole fragment overlapped then free the fragment */
        if (tmp->len <= 0) {                  
            if (tmp->prevf != NULL)
                ((T_IPF *)(tmp->prevf))->nextf = tmp->nextf;
            else
                p_hq->fragments = tmp->nextf;

            if (tmp->nextf != NULL)
                ((T_IPF *)(tmp->nextf))->prevf = tmp->prevf;
        
            p_hq->frag2 = tmp->nextf;
            next = tp_frag;
            if (tmp)
                ercd = rel_blf(ID_ETH_MPF, tmp);
            if (ercd != E_OK)
                ip_internal_error(ercd);
        }
    } 

    /* Fill the header informations of this fragment */
    tmp = (T_IPF *)pkt;
    tmp->offset = offset;
    tmp->end = end;
    tmp->len = end - offset;

    /* Insert the fragment in the linked list of fragments */
    tmp->prevf = prev;
    tmp->nextf = next;
    if (prev != NULL)
        ((T_IPF *)prev)->nextf = pkt;
    else
        p_hq->fragments = pkt;

    if (next != NULL)
        ((T_IPF *)next)->prevf = pkt;

    p_hq->frag2 = ((T_IPF *)p_hq->fragments)->nextf; /* get next pointer */

    /* Check all fragments are received or not */
    if (receive_all_frag(p_hq)) {
        pkt = make_ip_pkt(p_hq);
        if (pkt == NULL) {
            p_hq->rtime = 1;    /* Memory cleared by timeout */
            pkt = p_hq->fragments;
            return pkt;
        }
        ((T_IPF *)pkt)->prevf = (T_IP *)p_hq;
        SNMP_BYPCNT(snmp_mibbyp.ipReasmOKs, 1);      /* ipReasmOKs */
        return pkt;
    }
    p_hq->rtime = IPF_REASM_TMO;            /* Redefine the timeout value */
    return pkt;                             /* framents to be received */
}

