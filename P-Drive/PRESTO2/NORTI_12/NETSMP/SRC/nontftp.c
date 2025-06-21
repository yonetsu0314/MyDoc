/******************************************************************************
* NORTi Network TFTP Sample                                                   *
*                                                                             *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 26/Nov/1998                                                                 *
* 22/Sep/1999 resource ID auto assign added.                                  *
* 12/Feb/2001 Change Task priority 4 -> 5                                     *
******************************************************************************/

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nontftp.h"
#include "nonfile.h"

/* Creation Information */

const T_CTSK c_tftpsrv_tsk = { NULL, TA_HLNG, tftpsrv_tsk,  5,  512 };
const T_UDP_CCEP c_tftpsrv_cep = { 0, { IPV4_ADDRANY, UDP_PORT_TFTP }, (FP)tftpsrv_cbk };

/* Local Data */

static T_TFTP tftp[1];

/************************************************/
/* TFTP Server Callback                         */
/************************************************/

ER tftpsrv_cbk(ID cepid, FN fncd, VP parblk)
{
    return E_OK;
}

/************************************************/
/* TFTP Server Task Sample                      */
/************************************************/

TASK tftpsrv_tsk(ID cepid)
{
    T_IPV4EP dstaddr;
    FILE *fp = NULL;
    ER len;
    UW pos;
    int i;

    for (;;)
    {
        len = udp_rcv_dat(cepid, &dstaddr, tftp, sizeof (T_TFTP), TMO_FEVR);
        if (len < 4)
            continue;

        switch (tftp->code)
        {
        case TFTP_RRQ:
            if (fp != NULL)
                fclose(fp);
            fp = fopen((const char *)&(tftp->blkno), (char *)"rb");
            if (fp == NULL)
            {   tftp->code = TFTP_ERROR;
                tftp->blkno = 1;
                strcpy((char *)(tftp->data), (char *)"file not found");
                len = 4 + strlen((char *)(tftp->data));
                break;
            }
            i = (int)fread(tftp->data, 1, TFTP_SIZE, fp);
            tftp->code = TFTP_DATA;
            tftp->blkno = 1;
            len = 4 + i;
            break;

        case TFTP_ACK:
            if (fp == NULL)
            {   tftp->code = TFTP_ERROR;
                tftp->blkno = 4;
                strcpy((char *)(tftp->data), (char *)"illegal sequence");
                len = 4 + strlen((char *)(tftp->data));
                break;
            }
            pos = (UW)tftp->blkno * TFTP_SIZE;
            fseek(fp, pos, SEEK_SET);
            i = (int)fread(tftp->data, 1, TFTP_SIZE, fp);
            if (i < TFTP_SIZE)
            {   fclose(fp);
                fp = NULL;
                if (i == 0) /* ignore last ACK */
                    break;
            }
            tftp->code = TFTP_DATA;
            tftp->blkno++;
            len = 4 + i;
            break;

        case TFTP_WRQ:
            if (fp == NULL)
                fp = fopen((const char *)&(tftp->blkno), (char *)"wb");
            if (fp == NULL)
            {   tftp->code = TFTP_ERROR;
                tftp->blkno = 1;
                strcpy((char *)(tftp->data), (char *)"illegal file name");
                len = 4 + strlen((char *)(tftp->data));
                break;
            }
            tftp->code = TFTP_ACK;
            tftp->blkno = 0;
            len = 4;
            break;

        case TFTP_DATA:
            if (fp == NULL)
            {   tftp->code = TFTP_ERROR;
                tftp->blkno = 4;
                strcpy((char *)(tftp->data), (char *)"illegal sequence");
                len = 4 + strlen((char *)(tftp->data));
                break;
            }
            pos = (UW)(tftp->blkno - 1) * TFTP_SIZE;
            fseek(fp, pos, SEEK_SET);
            len -= 4;
            i = (int)fwrite(tftp->data, 1, len, fp);
            if (i < TFTP_SIZE)
            {   fclose(fp);
                fp = NULL;
                if (i < len)
                {   tftp->code = TFTP_ERROR;
                    tftp->blkno = 3;
                    strcpy((char *)(tftp->data), (char *)"disk full");
                    len = 4 + strlen((char *)(tftp->data));
                    break;
                }
            }
            tftp->code = TFTP_ACK;
            len = 4;
            break;

        case TFTP_ERROR:
            len = 0;
            break;

        default:
            tftp->code = TFTP_ERROR;
            tftp->blkno = 4;
            strcpy((char *)(tftp->data), (char *)"unknown operation");
            len = 4 + strlen((char *)(tftp->data));
            break;
        }

        if (len != 0)
            udp_snd_dat(cepid, &dstaddr, tftp, len, TMO_FEVR);
    }
}

/************************************************/
/* TFTP Server Intialize                        */
/************************************************/

ER tftpsrv_ini(ID tskid, ID cepid)
{
    ER ercd;

    /* Create TFTP Server Task */

    if(tskid == 0){/* ID auto (Add by Y.Y) */
        ercd = vcre_tsk(&c_tftpsrv_tsk);
        if (ercd < 0)
            return ercd;
        tskid = ercd;
    }
    else{
        ercd = cre_tsk(tskid, &c_tftpsrv_tsk);
        if (ercd != E_OK)
            return ercd;
    }

    /* Create UDP Commnunication End Point */

    if(cepid == 0)
    {   ercd = cepid = udp_vcre_cep(&c_tftpsrv_cep);
        if(cepid <= 0)
            return ercd;
    }
    else
    {   ercd = udp_cre_cep(cepid, &c_tftpsrv_cep);
        if (ercd != E_OK)
            return ercd;
    }

    /* Start TFTP Server Task */

    ercd = sta_tsk(tskid, cepid);
    if (ercd != E_OK)
        return ercd;

    return E_OK;
}

/* end */
