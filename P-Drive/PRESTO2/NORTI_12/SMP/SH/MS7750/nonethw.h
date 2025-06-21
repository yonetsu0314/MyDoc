/******************************************************************************
* NORTi Network User's Hardware Definition                                    *
* This is for Hitachi SH-4.                                                   *
*                                                                             *
*   File:           NONETHW.H                                                 *
*   Last released:  29/Dec/1999                                               *
*                                                                             *
*  Written by Yasuo Tomita of Tygnus Computer Works.                          *
*                                                                             *
* 29/Nov/1999  Created to port to Hitachi ULSI MS7750SE01 board.         [YT] *
* 07/Dec/1999  Added the LANC_GETEA() macro.                             [YT] *
******************************************************************************/

/* Base Addresses of LAN Controller */

/*
  NOTE: Activate the LANC_BUFFER macro, only when your board supports
        shared memory mode for DP8390 buffer accesses.
*/
#define LANC_PORT   0xb0000000L     /* DP83902 port base address */
/*#define LANC_BUFFER 0xb0800000L*/ /* DP83902 buffer base address */

/* Low-level I/O Support for LAN Controller */

/*== When you modifiy this file for your new target board, you must   ==*
 *== confirm that the RO and RA below are defined correctlly for your ==*
 *== board. If the valuses of these two macros are incorrent, the LAN ==*
 *== driver will never work properly.                                 ==*/
#define RO 0x00                 /* offset to registers                */
#define RA 2                    /* alignment to registers             */
                                /*   1 = byte alignment               */
                                /*   2 = word (2-byte) alignment      */
                                /*   4 = long word (4-byte) alignment */

#define lan_port(p)     (LANC_PORT+(RA*(p)+RO))

/* The lan_in and lan_out macros are unique for MS7709RP01 board. */
/* #define lan_in(p)       (*(volatile UB *)lan_port(p)) */
/* #define lan_out(p,c)    (*(volatile UB *)lan_port(p) = (c)) */
#if 0 /* デバッグ中!! */
#define lan_in(p)       (*(volatile UH *)lan_port(p) >> 8)
#define lan_out(p,c)    (*(volatile UH *)lan_port(p) = (UH)(c) << 8)
#define lan_inw(p)      (*(volatile UH *)lan_port(p))
#define lan_outw(p,c)   (*(volatile UH *)lan_port(p) = (c))
#endif

/* Board-dependent Macros for LAN Controller */

#undef DATAPORT
#ifndef RA
#define DATAPORT  0x40000       /* DP83902 data port address */
#else
#define DATAPORT  (0x40000/RA)  /* DP83902 data port address */
#endif

#undef RESETPORT
#define RESETPORT 0xb0080000    /* DP83902 reset port address */
                                /* DP83902 reset procedure */
#define LANC_RESET() { \
    *(volatile UH *)RESETPORT = 0; \
    *(volatile UH *)RESETPORT = 0x100; \
}                                   /* put RESET/ pin in low once */

#define DIPSWPORT 0xb0800000    /* DIP switches (SW5-8) port address  */
                                /* Ethernet address get procedure */
#define LANC_GETEA(ea) { \
    UH sw; \
    ea[0] = 0x00; \
    ea[1] = 0x00; \
    sw = ~*(volatile UH *)DIPSWPORT; \
    ea[2] = (UB)(sw >> 8); \
    ea[3] = (UB)sw; \
    sw = ~*(volatile UH *)(DIPSWPORT+2); \
    ea[4] = (UB)(sw >> 8); \
    ea[5] = (UB)sw; \
}                                   /* read address from the switches */
                                    /* (EA = 00:00:SW5:SW6:SW7:SW8) */

/* Low-level Block I/O Support for LAN controller */

/*#define blkio_INASM*/     /* The support codes are written in assembler */
#undef blkio_INASM          /* The support codes are not written in assembler */
#ifdef blkio_INASM
void    _inbrep(UW port, UB *ptr, UW len);
void    _outbrep(UW port, UB *ptr, UW len);
void    _inwrep(UW port, UH *ptr, UW len);
void    _outwrep(UW port, UH *ptr, UW len);
#define lan_blkinb(p,c,n)   _inbrep(lan_port(p), (UB *)(c), n)
#define lan_blkoutb(p,c,n)  _outbrep(lan_port(p), (UB *)(c), n)
#define lan_blkinw(p,c,n)   _inwrep(lan_port(p), (UH *)(c), n)
#define lan_blkoutw(p,c,n)  _outwrep(lan_port(p), (UH *)(c), n)
#else
void    lan_inbrep(UW port, UB *ptr, UW len);
void    lan_outbrep(UW port, UB *ptr, UW len);
void    lan_inwrep(UW port, UH *ptr, UW len);
void    lan_outwrep(UW port, UH *ptr, UW len);
#define lan_blkinb(p,c,n)   lan_inbrep(p, (UB *)(c), n)
#define lan_blkoutb(p,c,n)  lan_outbrep(p, (UB *)(c), n)
#define lan_blkinw(p,c,n)   lan_inwrep(p, (UH *)(c), n)
#define lan_blkoutw(p,c,n)  lan_outwrep(p, (UH *)(c), n)
#endif

/* end */
