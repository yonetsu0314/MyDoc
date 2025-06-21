/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/sh7615/RCS/driver.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: driver.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* =========================================================================
   Driver and interrupt related support. */

void IRinstall(int irno, int netno, void (*irhan) (int netn));
void IRLinstall(int irlno, int netno, void (*irhan) (int x));
void IRrestore(int irno);

#define _outb(port,val)		*(volatile unsigned char *)(port) = val
#define _outw(port,val)		*(volatile unsigned short *)(port) = val
#define _outl(port,val)		*(volatile unsigned long *)(port) = val
#define _inb(port)		(*(volatile unsigned char *)(port))
#define _inw(port)		(*(volatile unsigned short *)(port))
#define _inl(port)		(*(volatile unsigned long *)(port))

/* SH7615では内蔵イーサネットコントローラは専用のDMAでフレームを直接メモリに */
/* 転送するので、BLOCKIN/BLOCKOUT系のマクロは使用しないはず.... */
#define BLOCKIN(ptr, port, len)  Ninhrep(ptr, port, len)
void            Ninhhrep(unsigned short *ptr, unsigned long port, int len);
#define BLOCKOUT(ptr, port, len)  Nouthrep(ptr, port, len)
void            Nouthrep(unsigned short *ptr, unsigned long port, int len);
#define map24bit(x) (unsigned long)(x)

/* disable and enable interrupts */
#if 1     /* used with HI7000 */
/* If used with HI7700, chg_ims() is used to disable/enable interrupt. */
/* This example use SR_IMS15, this disables all interrupts. But this should */
/* be the interrupt level of LAN controller. */
void  DISABLE( void );
void  ENABLE( void );

#else     /* used in non-rtos */
/* When in non-rtos environment, the SR register is directly changed. */
/* See suppa2.s. */
#define DISABLE()  Ndisable()
#define ENABLE()   Nenable()

#endif

#define enableDMA(no)
#define disableDMA(no)
