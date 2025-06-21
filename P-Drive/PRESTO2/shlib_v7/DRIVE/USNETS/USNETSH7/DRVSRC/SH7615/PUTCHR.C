/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/putchr.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: putchr.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

typedef volatile unsigned char  PORT8;
typedef volatile unsigned short PORT16;
typedef volatile unsigned long  PORT32;

#include "..\sh7615.h"

void Nputchr(char cc)
{
    PORT16 *sc1ssr1 = (PORT16 *)SC1SSR1;
    PORT8 *scftdr1 = (PORT8 *)SCFTDR1;

    if (cc == '\n') {
        while (!(*sc1ssr1 & SCIF_TDFE));
        *scftdr1 = '\r';
        while (!(*sc1ssr1 & SCIF_TDFE));
        *sc1ssr1 &= ~SCIF_TDFE;
    }

    while (!(*sc1ssr1 & SCIF_TDFE));
    *scftdr1 = cc;
    while (!(*sc1ssr1 & SCIF_TDFE));
    *sc1ssr1 &= ~SCIF_TDFE;
}

int Ngetchr()
{
    PORT16 *sc1ssr1 = (PORT16 *)SC1SSR1;
    PORT8 *scfrdr1 = (PORT8 *)SCFRDR1;

    while (!(*sc1ssr1 & SCIF_RDF));
    return *scfrdr1;
}

int Nchkchr()
{
    PORT16 *sc1ssr1 = (PORT16 *)SC1SSR1;

    return (*sc1ssr1 & SCIF_RDF);
}
