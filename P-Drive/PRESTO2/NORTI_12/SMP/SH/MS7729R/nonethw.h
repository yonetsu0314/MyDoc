/******************************************************************************
* NORTi Network User's Hardware Definition                                    *
*                                                                             *
*  Written by Yasuo Tomita of Tygnus Computer Works.                          *
*                                                                             *
*  Copyright (c) 1999-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
* 17/Dec/1998                                                                 *
* 28/Apr/1999  Modified for MS7709RP01 board.                            [YT] *
******************************************************************************/

/* Base Addresses of LAN Controller */

#define LANC_PORT   0xb0000000L     /* MS7709RP01 DP83902 port base address */
/*#define LANC_BUFFER 0xb0800000L*/ /* xxxxxxxxxx DP8390x buffer base address */

/* Low-Level I/O Support for LAN Controller */

/*== When you modifiy this file for your new target board, you must   ==*
 *== confirm that the RO and RA below are defined correctlly for that ==*
 *== board. If the valuses of these two macros are incorrent, the LAN ==*
 *== driver will never work properly.                                 ==*/
#define RO 0x00                 /* offset to registers                */
#define RA 2                    /* alignment to registers             */
                                /*   1 = byte alignment               */
                                /*   2 = word (2-byte) alignment      */
                                /*   4 = long word (4-byte) alignment */

#define lan_port(p)     (LANC_PORT+(RA*(p)+RO))

#define lan_inb(p)      (*(volatile UB *)lan_port(p))
#define lan_outb(p,c)   (*(volatile UB *)lan_port(p) = (c))
#if 0
#define lan_inw(p)      (*(volatile UH *)lan_port(p))
#define lan_outw(p,c)   (*(volatile UH *)lan_port(p) = (c))
#endif

void    _inbrep(UW port, UB *ptr, UH len);
void    _outbrep(UW port, UB *ptr, UH len);
void    _setbrep(UW port, UB data, UH len);
void    _inwrep(UW port, UH *ptr, UH len);
void    _outwrep(UW port, UH *ptr, UH len);
void    _setwrep(UW port, UB data, UH len);
#define lan_blkinb(p,c,n)   _inbrep(p, (UB *)(c), n)
#define lan_blkoutb(p,c,n)  _outbrep(p, (UB *)(c), n)
#define lan_blksetb(p,c,n)  _setbrep(p, (UB)(c), n)
#define lan_blkinw(p,c,n)   _inwrep(p, (UH *)(c), n)
#define lan_blkoutw(p,c,n)  _outwrep(p, (UH *)(c), n)
#define lan_blksetw(p,c,n)  _setwrep(p, (UB)(c), n)

/* end */
