/******************************************************************************
* NORTi Network File System RAM Disk Driver                                   *
*                                                                             *
*  Copyright (c) 1999-2003, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 07/Mar/1999                                                                 *
* 13/Sep/2001 d->ercd�ւ̐ݒ蔲�����C��                            M.Kawahara *
* 29/Sep/2001 T_DISK�����v�Z�R�[�h���쐬. CALC_T_DISK�t���O��ǉ�  M.Kawahara *
* 21/Apr/2002 FATS, DIRS�}�N����ǉ�                                       KA *
* 18/Jan/2003 Sectors Per Cluster�̎����v�Z��ǉ�                          KA *
******************************************************************************/
/* CALC_T_DISK��L���ɂ���Ǝg�p�\�ȃN���X�^�����ő�ɂȂ�悤��T_DISK    *
 * �p�����[�^���������߂��܂��B�T�C�Y��X�s�[�h���C�ɂȂ���͈ȉ��̂悤�ɂ� *
 * �ĉ������B 1) identify()�̏I���s�Ƀu���[�N��ݒ肷��Ȃǂ��� d->sf�̒l�� *
 * �m�F����B 2) "d->sf = xx;"�Ɛݒ肵�Ă��镔����ҏW����قǊm�F�����l��  *
 * ���ڐݒ肵�܂��B3) #define CALC_T_DISK �̒�`���R�����g�A�E�g���܂�      */
#define CALC_T_DISK

#include <stdio.h>
#include <string.h>
#ifdef GAIO
#include <memory.h>
#endif
#include "norti3.h"
#include "nonfile.h"

/************************************************/
/* Identify (Inform Disk Parameter)             */
/************************************************/

#ifdef CALC_T_DISK
/* �g�p�\�ȃN���X�^�����ő�ɂȂ�悤�������� */
static ER GetClusterMax(T_DISK* d, B fatbits, W* pnClusterMax, W* pnSectorsPerFAT, W* pnRem, W* pnSectorsPerCluster)
{
    W nRootDirSec = (d->rde * 32) / SECTOR_SIZE;  /* ���[�g�f�B���N�g���Z�N�^�� */
    W nUseAbleSec = d->ts - d->rsc - nRootDirSec; /* FAT,DIR,Cluster�Ŏg�p�\�ȃZ�N�^�� */
    W nClusterMax;
    W MaxClusterMax;
    W n;
    W nSectorsPerFAT;

    for(*pnSectorsPerCluster = 1;; *pnSectorsPerCluster <<= 1) {
        MaxClusterMax = 0;
        for(nSectorsPerFAT = 1; ; nSectorsPerFAT++)
        {
            nClusterMax = (nUseAbleSec - d->fn * nSectorsPerFAT) / *pnSectorsPerCluster;
            if (nClusterMax < 1)
            {
                if (MaxClusterMax == 0)
                {
                    MaxClusterMax = nClusterMax;
                    break;
                }
            }
            /* FAT12/16�ɂ��N���X�^���̐��� */
            if (fatbits == 12)                /* 12bitFAT */
            {
                if (nClusterMax > 0xFF6)
                    nClusterMax = 0xFF6;
            }
            else if (fatbits == 16)                /* 16bitFAT */
            {
                if (nClusterMax > 0xFFF6)
                    nClusterMax = 0xFFF6;
            }
            /* nSectorsPerFAT�ɂ��N���X�^���̐��� */
            n = (nSectorsPerFAT * SECTOR_SIZE - 3) * 8 / fatbits;
            if (nClusterMax > n)
                nClusterMax = n;
            /* ���������̊m�F */
            if (MaxClusterMax >= nClusterMax)
                break;
            MaxClusterMax = nClusterMax;
        }
        *pnSectorsPerFAT = --nSectorsPerFAT;    /* ��O�������� */
        *pnClusterMax = MaxClusterMax;
        if (nSectorsPerFAT < 1 || MaxClusterMax < 1)
            return EV_FSYS_PAR;
        /* ���g�p�Z�N�^���̌v�Z */
        *pnRem = nUseAbleSec - d->fn * nSectorsPerFAT - MaxClusterMax * (*pnSectorsPerCluster);
        if(*pnRem <= *pnSectorsPerCluster)
            break;
    }
    return E_OK;
}
#endif  /* CALC_T_DISK */

static ER identify(T_DISK* d)
{
#ifdef CALC_T_DISK
    ER ercd16;
    W ClusterMax16;     /* �g�p�\�N���X�^�� */
    W nSectorsPerFAT16; /* FAT���g�p����Z�N�^�� */
    W nRem16;           /* �����ɗ]��g�p�s�\�ȃZ�N�^�� */
    W nSectorsPerCluster16;
#endif   /* CALC_T_DISK */

    d->sc  = 1;                     /* Sectors per Cluster */
    d->rsc = 1;                     /* Reserved Sector Count */

#ifdef FATS
    d->fn  = FATS;                  /* Number of FATs */
#else
    d->fn  = 2;                     /* Number of FATs */
#endif

#ifdef DIRS
    d->rde = DIRS;                  /* Root Directry Entrys */
#else
    d->rde = 224;                   /* Root Directry Entrys */
#endif

    d->ts  = d->par / SECTOR_SIZE;  /* Total Sectors */
#ifndef CALC_T_DISK
    d->sf  = xx;                     /* Sectors per FAT */
#endif   /* CALC_T_DISK */

#ifdef CALC_T_DISK
    /* SectorsPerFAT�̍ő�l�����߂� */
    ercd16 = GetClusterMax(d, 16, &ClusterMax16, &nSectorsPerFAT16, &nRem16, &nSectorsPerCluster16);
    if (ercd16 != E_OK)
        return ercd16;
    d->sc = nSectorsPerCluster16;       /* Sectors per Cluster */
    d->sf = nSectorsPerFAT16;
#endif /* CALC_T_DISK */

    return E_OK;
}

/************************************************/
/* Format                                       */
/************************************************/

static ER format(T_DISK *d)
{
    UW *p, *end;

    p = (UW *)d->addr;
    end = (UW *)(d->addr + (d->par & ~3));
    while (p != end)
        *p++ = 0L;
    return E_OK;
}

/************************************************/
/* Hardware Initialize                          */
/************************************************/

static ER hardware_init(T_DISK *d)
{
    UW *p, *end;

    /* Boot Sector All Zero (Backup OK) ? */

    p = (UW *)d->addr;
    end = (UW *)(d->addr + SECTOR_SIZE);
    while (p != end)
    {   if (*p++ != 0L)
            return format(d);
    }
    return E_OK;
}

/************************************************/
/* Read Sector                                  */
/************************************************/

static ER read_sector(T_DISK *d, UW sectno, UB **buf)
{
    if (sectno >= d->ts)
        return E_PAR;
    *buf = (UB *)d->addr + sectno * SECTOR_SIZE;
    return E_OK;
}

/************************************************/
/* Write Sector                                 */
/************************************************/

static ER write_sector(T_DISK *d, UW sectno, UB *buf)
{
    UB *p;

    if (sectno >= d->ts)
        return E_PAR;
    p = (UB *)d->addr + sectno * SECTOR_SIZE;
    if (p != buf)
        memcpy(p, buf, SECTOR_SIZE);
    return E_OK;
}

/************************************************/
/* RAM Disk Read/Write Function                 */
/************************************************/

ER ramdisk(T_DISK *d, FN fncd, UW sectno, VP par)
{
    switch (fncd)
    {
    case TFN_READ_SECTOR:
        return read_sector(d, sectno, (UB **)par);

    case TFN_WRITE_SECTOR:
        return write_sector(d, sectno, (UB *)par);

    case TFN_IDENTIFY:
        return identify(d);

    case TFN_FORMAT:
        return format(d);

    case TFN_HARDWARE_INIT:
        return hardware_init(d);

    case TFN_HARDWARE_RESET:
        return E_OK;

    default:
        return E_NOSPT;
    }
}

/* end */
