/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/am79c90.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: am79c90.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* AM7990.C  Edition: 2  Last changed: 17-Feb-98,15:39:22  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    AM7990.C -- Embedded Ethernet Driver for USNET

    Copyright (C) 1993,1995 By
    United States Software Corporation
    7175 NW Evergreen Parkway
    Hillsboro, OR 97124
    503-844-6614  www.ussw.com

    This software is furnished under a license and may be used
    and copied only in accordance with the terms of such license
    and with the inclusion of the above copyright notice.
    This software or any other copies thereof may not be provided
    or otherwise made available to any other person.  No title to
    and ownership of the software is hereby transferred.

    The information in this software is subject to change without
    notice and should not be construed as a commitment by United
    States Software Corporation.
*/

#include <string.h>
#include "net.h"
#include "local.h"
#include "driver.h"
#include "support.h"
#include "ethernet.h"
#include "am79c90.h"

/* RXPOWER/TXPOWER 1 = 2 buffers, 2 = 4 buffers, 3 = 8 buffers */
#define RXPOWER 3
#define NRECBUFS (1<<RXPOWER)
#define TXPOWER 2
#define NTXBUFS (1<<TXPOWER)

/*#define PROMISCUOUS					/* test mode : 全パケットを捨てる */
/*#define NOCOPY                        /* 共通バッファモード */

/*
#ifndef LITTLE
#define STORE24(value, a16, a8) \
    *(char *)&a16 = value, ((char *)&a16)[1] = (int)value>>8, a8 = (value>>16)
#else
*/
#define STORE24(value, a16, a8) \
    a16 = (unsigned short)(value), a8 = (unsigned char)((value)>>16)
/*
#endif
*/

#define GET24(a16,a8) (((unsigned long)(a8) << 16) + (a16))

#define AM2HI(adr) ((adr) + AM_BUFADDR - AM_BUFADDR_AM)
#define HI2AM(adr) ((adr) + AM_BUFADDR_AM - AM_BUFADDR)

extern struct CONNECT connblo[];
extern struct NET nets[];

/* NET構造体のフィールドのうち、このドライバ内で独自に使用しているものの一覧 */
/* bufbas     送信中のパケットの先頭アドレス                                 */
/* base[0]    Receive Message Descriptorの先頭のアドレス。CPU側アドレス      */
/* base[1]    Transmit Message Descriptorの先頭のアドレス。CPU側アドレス     */
/* irno[0]    使用する割込み番号                                             */
/* irno[1]    次にパケットが受信されることが期待されるrmdエントリ番号        */
/* irno[2]    次にパケットの送信に使用するtmdエントリ番号        */

/* 以下のグローバル変数はデバッグ用。各種のエラーの発生回数をカウントしている */
unsigned long QueueFullCount;	/* USNET側のarriveキューfullによる棄却回数 */
unsigned long MissedCount;		/* 79C90でのMISSエラーの発生回数 */
unsigned long MemAllocFail;		/* メモリブロックハント失敗回数 */

/* ==========================================================================
   C level interrupt handler for Ethernet.  Called from a stub, registers are
   saved.  Returns to the interrupt stub.  Queues the arrived message into
   the arrive queue of the network block.  */

static void     irhan(int netno)
{
    int             i1,i2,
                    status;
    unsigned int    tport,
                    len;
    unsigned long   ul1;
    MESS           *mess;
    register struct NET *netp;
    struct MD      *rmd,
                   *tmd;

    netp = &nets[netno];
    tport = netp->port;
aga:
    status = _inw(tport + CSR);

/* If the 7990 dropped out, we try to restart it.  This may not always be a
   good idea, because the automatic recovery could hide some serious problem.
*/
    if ((status & INEA) == 0) {
        _outw(tport + CSR, INEA | STRT);    /* start and enable chip */
        status = _inw(tport + CSR);
        if (netp->hwflags)
            status |= TINT;
    }
    _outw(tport + CSR, status & 0xfff0);

/*=====================================================================*/
/* パケット受信処理 */
/* パケットの受信に使用されるrmdの位置はnetp->irno[1]で記憶されているので、 */
/* このrmdを最初に処理する。 */
    if (status & RINT) {			/* 受信割込み。 */
        rmd = (struct MD *)netp->base[0]+netp->irno[1];		/* RMDの位置。CPU側アドレス */
        for (i1=0,i2=netp->irno[1];i1<NRECBUFS;i1++,i2++,rmd++) {		/* RMDの各エントリを順に見る */
            if (i2 >= NRECBUFS) {
                rmd = (struct MD *)netp->base[0];
                i2 = 0;
            }
#if NTRACE >= 5
/*            Nprintf("irhan : trying rmd(%d) at 0x%08lX\n",i2,rmd);	/**/
#endif
            if (rmd->flags & MD_OWN) continue;	/* 受信されていなければそのエントリは無視 */
            netp->irno[1] = i2+1;
            if (netp->irno[1] >= NRECBUFS) netp->irno[1] = 0;
            len = rmd->cnterr;					/* 受信バイト数 | エラーフラグ */
            if (len > MAXBUF - MESSH_SZ + 4 || rmd->flags & MD_ERR) {	/* エラーならば */
                netp->ifInErrors++;
lab3:						/* データは捨てる。バッファは入れ替えずにそのまま使う */
                rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
                rmd->cnterr = 0;
                rmd->flags = MD_OWN;
                continue;
            }
#if NTRACE >= 5
/*Nprintf("irhan : message received.\n"); /**/
#endif
#ifdef PROMISCUOUS					/* test mode : 全パケットを捨てる */
            rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
            rmd->cnterr = 0;
            rmd->flags = MD_OWN;
            continue;
#endif								/* test mode end. */
/* ネットワークの受信キューがオーバーフローしている場合は、データは捨てざるを得ない。 */
/* ここがオーバーフローするという事は、過負荷かハングアップの可能性が高い。 */
/* ここで無理に受信しても結局上位層で無理が出る。 */
            if (QUEUE_FULL(netp, arrive)) {
#if NTRACE >= 1		/* こんな時にメッセージなんか出すと余計に事態を悪化させる... */
/*                Nprintf("irhan : receive queue full,discard packet.\n"); /**/
#endif
                QueueFullCount++;	/* デバッグ用。USNET側のarriveキューfullによる棄却回数 */
                netp->ifInDiscards++;
                goto lab3;
            }

#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
/* 受信データの格納アドレス。MESSヘッダ分補正する。 */
/* RMDに繋がれていたバッファをNetTaskに渡し、新しいバッファをRMDに繋ぐ */
            ul1 = GET24(rmd->adr16,rmd->adr8);
            mess = (MESS *)(AM2HI(ul1) - MESSH_SZ);

#if NTRACE >= 5
/*          Nprintf("irhan : received message available at 0x%08lX\n",mess); /**/
#endif
#endif /* ---- NOCOPYモードend ------------------------------------- */

/* ---- 通常モード：79C90バッファメモリとUSNETのバッファは独立。要コピー */
#ifndef NOCOPY /* ---- 通常モードbegin ---------------------------- */
            if ((mess = NgetbufIR()) == 0) {	/* バッファがない：パケットは捨てる */
#if NTRACE >= 1
/*                Nprintf("irhan : failed to get memory block, packet discarded.\n"); /**/
#endif
                MemAllocFail++;
                netp->ifInDiscards++;
                goto lab3;
            }
            ul1 = GET24(rmd->adr16,rmd->adr8);	/* 受信パケットが格納されているアドレス */
            ul1 = AM2HI(ul1);
#if NTRACE >= 5
/*            Nprintf("irhan : data at 0x%08lX ---> message block at 0x%08lX\n",ul1,mess+MESSH_SZ);	/**/
#endif

            memcpy((char *)mess+MESSH_SZ, (void *)ul1, len);
            rmd->cnterr = 0;
            rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
            rmd->flags = MD_OWN;
#endif /* ---- 通常モードend -------------------------------------- */

/* MESSヘッダに必要な情報を格納し、ネットワークの受信キューに繋ぐ */
            mess->netno = netno;
            mess->mlen = len + MESSH_SZ - 4;
            mess->offset = MESSH_SZ;
            QUEUE_IN(netp, arrive, mess);
            WAITNOMORE_IR(SIG_RN(netno));

#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
/* 新しいバッファを取得し、RMDに繋ぐ */
            if ((ul1 = (unsigned long)NgetbufIR()) == 0) {		/* バッファ取得できない */
/* この場合は空きバッファができるまで待つしかない。MISSED PACKETエラー発生時に */
/* リトライすべし。 */
/*              Nprintf("irhan : new buffer allocation failed. set rmd entry to void.\n");/**/
                rmd->adr16 = 0;
                rmd->adr8 = 0;
                rmd->cnterr = 0;
                rmd->bcnt = 0;
                rmd->flags = 0;
            } else {
/*              Nprintf("irhan : new buffer allocated at 0x%08lX\n",ul1);/**/
                ul1 = HI2AM(ul1+MESSH_SZ);			/* 新しいバッファのデータ格納位置の79C90側アドレス */
                STORE24(ul1, rmd->adr16, rmd->adr8);	/* RMDにセットする */
                rmd->cnterr = 0;
                rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
                rmd->flags = MD_OWN;
            }
#endif /* ---- NOCOPYモードend ------------------------------------- */
            continue;
        }										/*****/
    }

/*=====================================================================*/
/* パケット送信処理 */
    if (status & TINT) {			/* 送信完了割込み */

        tmd = (struct MD *)netp->base[1];	/* TMDの位置。CPU側アドレス */
        for (i1 = 0; i1 < NTXBUFS; i1++,tmd++) {
#if NTRACE >= 5
Nprintf("irhan : trying tmd at 0x%08lX bcnt = %d flags=0x%04X\n",tmd,(int)tmd->bcnt,tmd->flags); /**/
#endif
            if (tmd->bcnt != 0) {	/* TMDエントリ有効データあり */
/* tmd->cnterrのエラービットも本来見なければならないのだが....。 */

                if (!(tmd->flags & MD_OWN)) {	/* 送信完了している */
                    tmd->bcnt = 0;

/* オリジナルメッセージが格納されているUSNETバッファのアドレスを得る */
#ifndef NOCOPY /* ---- 通常モードbegin ----------------------------- */
                    ul1 = GET24(tmd->adr16,tmd->adr8);
                    ul1 = AM2HI(ul1);
                    mess = *(MESS **)(ul1-4);
#endif /* ---- 通常モードend --------------------------------------- */
#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
                    ul1 = GET24(tmd->adr16,tmd->adr8);
                    ul1 = AM2HI(ul1);
                    mess = (MESS *)(ul1-MESSH_SZ);
#endif /* ---- NOCOPYモードend ------------------------------------- */

/* 送信済みパケットのMESSヘッダに送信済をマークする */
                    if (mess != (MESS *)0) {		/* 絶対に成立する筈 */
                        mess->offset = boTXDONE;	/* 送信済をマークする */
#if NTRACE >= 5
Nprintf("irhan : send completed packet 0x%08lX.\n",mess);/**/
#endif
                        if (mess->id <= bWACK) {		/* バッファ開放可なら、開放する */
                            if (mess->id == bRELEASE) {
                                mess->id = bALLOC;
                                NrelbufIR(mess);
                            }
                        } else {					/* 開放不可の場合は多分送信完了を待っているタスクがいる */
                            WAITNOMORE_IR(SIG_WN(netno));
                        }
                    }

                    if (!(QUEUE_EMPTY(netp, depart))) {	/* まだ送信するパケットがあれば */
                        QUEUE_OUT(netp, depart, mess);	/* 送信キューから送信するパケットを取り出す */
#if NTRACE >= 5
Nprintf("irhan : send packet 0x%08lX\n",mess);/**/
#endif
                        netp->irno[2]++;
                        if (netp->irno[2] >= NTXBUFS) netp->irno[2] = 0;
                        len = mess->mlen - MESSH_SZ;		/* 送信データ長 */
                        if (len < ET_MINLEN) len = ET_MINLEN;

#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
                        ul1 = (unsigned long)mess+MESSH_SZ;	/* 実際の送信データの格納位置 */
                        ul1 = HI2AM(ul1);					/* 送信データの格納位置の79C90側アドレス */
                        STORE24(ul1, tmd->adr16, tmd->adr8);/* TMDにセットする */
#endif /* ---- NOCOPYモードend ------------------------------------- */
#ifndef NOCOPY /* ---- 通常モードbegin ---------------------------- */
                        ul1 = GET24(tmd->adr16,tmd->adr8);
                        ul1 = AM2HI(ul1);
                        memcpy((char *)ul1, (char *)mess+MESSH_SZ, len);
#endif /* ---- 通常モードend -------------------------------------- */
                        tmd->bcnt = -len;					/* 送信バイト数 。２の補数。*/
                        tmd->cnterr = 0;
                        tmd->flags = MD_OWN | MD_STP | MD_ENP;	/* 送信開始 */
/*                        _outw(tport + CSR, TDMD | INEA); /**/
                    }

                }
            }
        }
    }

/*=====================================================================*/
    if (status & MISS) {            /* Missed Packet (no receive buffer avail.) */
#if NTRACE >= 1
/*Nprintf("irhan : message missed.\n"); /**/
#endif
        MissedCount++;		/* デバッグ用。79C90でのMISSエラーの発生回数 */
        netp->ifInDiscards++;
#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
        rmd = (struct MD *)netp->base[0];		/* RMDの位置。CPU側アドレス */
        for (i1=0;i1<NRECBUFS;i1++) {			/* RMDの各エントリを順に見る */
            if ((rmd->adr16 | rmd->adr8) == 0) {  /* RMDにバッファ割り当てられていない */
                if ((ul1 = (unsigned long)NgetbufIR()) != 0) {  /* バッファ取得 */
                    ul1 = HI2AM(ul1+MESSH_SZ);            /* 新しいバッファのデータ格納位置の79C90側アドレス */
                    STORE24(ul1, rmd->adr16, rmd->adr8);  /* RMDにセットする */
                    rmd->cnterr = 0;
                    rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
                    rmd->flags = MD_OWN;
                }
            }
        }
#endif /* ---- NOCOPYモードend ------------------------------------- */
    }

/*=====================================================================*/
    if (status & MERR) {            /* Memory Error (DMA error) */
#if NTRACE >= 1
Nprintf("irhan : DMA error.\n"); /**/
#endif
        /* stop and restart controller */
        _outw(tport + CSR, STOP);    /* stop controller */
        _outw(tport + RAP, SELCSR3);
        _outw(tport + CSR3, BSWP);
        _outw(tport + RAP, SELCSR0);/* initialize the chip */
        _outw(tport + CSR, INIT);
        for (i1 = 0; i1 < 10000; i1++) {		/* 初期化終了を待つ */
            if (_inw(tport + CSR) & IDON) {
                return;
            }
        }
        _outw(tport + CSR, IDON);			/* clear IDON bit */
        _outw(tport + CSR, INEA | STRT);    /* start and enable chip */
    }

    if (_inw(tport + CSR) & 0xff00)
        goto aga;
}


/* ==========================================================================
   Transmit routine.  If the transmitter is idle, starts the transmission and
   returns.  Otherwise adds message to the departure queue; the interrupt
   handler will transmit it.  Returns:
    error:    -1
 	success:  0
 */

static int      writE(int conno, MESS * mess)
{
    int             tport,
                    len;
    unsigned long   ul1;
    struct NET     *netp;
    struct MD      *tmd;
    int             i1;

    netp = &nets[mess->netno];
    tport = netp->port;
    len = mess->mlen - MESSH_SZ;		/* 送信データサイズ */
    if (len < ET_MINLEN) len = ET_MINLEN;	/* Ethernetには最小パケットサイズがあるらしい */
/*    mess->offset = mess->netno;	/**/

    DISABLE();
    tmd = (struct MD *)netp->base[1]+netp->irno[2];		/* TMDの位置。CPU側アドレス */
    if (tmd->bcnt != 0) {	/* 空きTMDエントリなし */
#if NTRACE >= 5
        Nprintf("am79c90 writE : send hold packet 0x%08lX.\n",mess);/**/
#endif
        if (QUEUE_FULL(netp,depart)) {
            netp->ifOutDiscards++;
            mess->offset = boTXDONE;
        } else {
            QUEUE_IN(netp, depart, mess);
        }

    } else {
        netp->irno[2]++;
        if (netp->irno[2] >= NTXBUFS) netp->irno[2] = 0;

#if NTRACE >= 5
        Nprintf("am79c90 writE : send packet 0x%08lX / use TMD at 0x%08lX\n",mess,tmd);/**/
#endif

#ifdef NOCOPY /* ---- NOCOPYモードbegin ---------------------------- */
/* 送信パケットをそのままTMDにつないでしまう。 */
        ul1 = (unsigned long)mess+MESSH_SZ;		/* 実際の送信データの格納位置 */
        ul1 = HI2AM(ul1);						/* 送信データの格納位置の79C90側アドレス */
        STORE24(ul1, tmd->adr16, tmd->adr8);	/* TMDにセットする */
#endif /* ---- NOCOPYバッファモードend ----------------------------- */
#ifndef NOCOPY /* ---- 通常モードbegin ----------------------------- */
        ul1 = GET24(tmd->adr16,tmd->adr8);
        ul1 = AM2HI(ul1);
        memcpy((char *)ul1, (char *)mess+MESSH_SZ, len);
        *(MESS **)(ul1-4) = mess;				/* 送信中のパケットの位置を記録しておく */
#endif /* ---- 通常モードend --------------------------------------- */
        tmd->bcnt = -len;					/* 送信データ長。２の補数。 */
        tmd->cnterr = 0;
        tmd->flags = MD_OWN | MD_STP | MD_ENP;	/* 送信開始 */
/*        _outw(tport + CSR, TDMD | INEA);	/**/
    }

    ENABLE();
    return 0;
}


/* ==========================================================================
   Open a connection. */

static int      opeN(int conno, int flag)
{
    return 0;
}


/* ==========================================================================
   Close the connection. */

static int      closE(int conno)
{
    return 0;
}


/* ==========================================================================
   Configure and start up the Ethernet interface.  We process the user-level
   text parameters and store the values into the net table.  We set up the
   board.  Then we store the interrupt address and enable the interrupt.
   Returns:
    error:    -1
 	success:   0
 */

static int      init(int netno, char *params)
{
    int             i1,i2,
                    tport;
    unsigned long   ul1;
    char           *cp1,
                   *cp2,
                    par[16],
                    val[16];
    struct NET     *netp;
    struct MD      *rmd,			/* Receive Message Descriptor (RMD) */
                   *tmd;			/* Transmit Message Descriptor (TMD) */
    struct INITBL   *initbl;		/* Initialization Block */

    netp = &nets[netno];
/* 初期化ブロックの作成はあっちこっちに分散している。 */
    initbl = (struct INITBL *)AM_BUFADDR;
    memset(initbl, 0, sizeof(struct INITBL));  /* clear init block */
#ifdef PROMISCUOUS				/* test mode : promiscuous mode */
    initbl->mode = PROM;
#endif							/* test mode end. */

#if 0
/* netconfで指定されたパラメータを読む。MACアドレスの格納アドレスのみ */
/* MACアドレスをROMの特定領域ではなく、netdataテーブルに格納する様に変更 */
/* ROMの特定領域に格納する様に変更するには、このブロックの#if 0を解除し、 */
/* netdataテーブルでのドライバオプションで以下の様に指定する */
/* ..... AM79C90, 0, "ENA=0xa0100000", */
    for (cp1 = params; *cp1;) {
        Nsscanf(cp1, "%[^=]=%s %n", par, val, &i1);
        cp1 += i1;
        if (strcmp(par, "ENA") == 0) {
            Nsscanf(val, "%i", &cp2);   /* address from board ROM */
            for (i1 = 0; i1 < 6; i1++)
                netp->id.c[i1] = *cp2++;
        } else {
#if NTRACE >= 1
            Nprintf("am79c90 : invalid parameter.\n");
#endif
            return NE_PARAM;
        }
    }
#endif

/* MACアドレスを読む。ここではnetdataテーブルに直接記述してある */
    cp2 = (char *)netp->id.c;
    for (i1 = 0; i1 < 6; i1+=2) {
#ifdef LITTLE
        initbl->eaddr[i1] = *cp2;
        initbl->eaddr[i1+1] = *(cp2+1);
#else
        initbl->eaddr[i1] = *(cp2+1);
        initbl->eaddr[i1+1] = *cp2;
#endif
		cp2+=2;
    }

    tport = netp->port = AM_PORTADDR;	/* 79C90のレジスタの配置アドレス。固定。 */
    netp->irno[0] = AM_INTNO;			/* 79C90の使用する割込み番号。0～14。固定。 */
    netp->bps = 10000000;

/* 79C90がSTOPしている事を確認する。STOPしていなければ止める。 */
    _outw(tport + RAP, SELCSR0);
    for (i2 = 0; i2 < 10; i2++) {
        for (i1 = 0; i1 < 10000; i1++) {
            if (_inw(tport + CSR) & STOP) {
                i2 = 100;
                break;
            }
        }
        _outw(tport + CSR, STOP);
    }
    if (i2 == 10) {			/* 止まらな....い？ */
#if NTRACE >= 1
        Nprintf("am79c90 : failed to stop controller.\n");
#endif
        return NE_HWERR;
    }

/* RMDを作成する位置を計算し、初期化ブロックにその位置を格納する */
    ul1 = sizeof (struct INITBL) + AM_BUFADDR_AM;	/* RMDは初期化ブロックの直後 */
    ul1 = ((ul1 >> 3)+1) << 3;						/* 8バイト強制バウンダリ */
    STORE24(ul1, initbl->rxd16, initbl->rxd8);		/* 初期化ブロックに格納する */
    initbl->rxdlen = RXPOWER << 5;   				/* RMDのエントリ数。2を底とする対数で指定する。 */
    rmd = (struct MD *)AM2HI(ul1);					/* RMD先頭のCPU側アドレス */
    netp->base[0] = (char FAR *)rmd;				/* ....を記録しておく */
    netp->irno[1] = 0;
    netp->irno[2] = 0;

/* TMDを作成する位置を計算し、初期化ブロックにその位置を格納する */
    ul1 += (sizeof (struct MD) * NRECBUFS);			/* TMDはRMDの直後 */
    ul1 = ((ul1 >> 3)+1) << 3;						/* 8バイト強制バウンダリ */
    STORE24(ul1, initbl->txd16, initbl->txd8);		/* 初期化ブロックに格納する */
    initbl->txdlen = TXPOWER << 5;					/* RMDのエントリ数。2を底とする対数で指定する。 */
    tmd = (struct MD *)AM2HI(ul1);					/* TMDのCPU側アドレス */
    netp->base[1] = (char FAR *)tmd;				/* ....を記録しておく */

#if NTRACE >= 5
    Nprintf("rmd at 0x%08lX / tmd at 0x%08lX\n",netp->base[0],netp->base[1]); /**/
#endif

#ifndef NOCOPY /* ---- 通常モードbegin ----------------------------- */
/* 通常モードでは受信バッファ及び送信バッファはTMDの直後に配置される。 */
    ul1 += (sizeof (struct MD) * NTXBUFS);	/* 受信バッファはTMDの直後 */
#endif /* ---- 通常モードend --------------------------------------- */

/* RMDの各エントリを初期化する */
/* NOCOPYモードでは79C90の受信バッファはUSNETのバッファそのものを使用する。 */
/* 従って、ここでUSNETからバッファを取得し、RMDにそのアドレスをセットする。 */
/* 一方、通常モードでは79C90の受信バッファには固定アドレスを割り当てる。 */
/* このアドレスはRMD及びTMDの領域の直後を使用する */
    for (i1 = 0; i1 < NRECBUFS; i1++) {
#if NTRACE >= 5
        Nprintf("init : initialize rmd at 0x%08lX\n",rmd); /**/
#endif
#ifdef NOCOPY /* ---- NOCOPYモードbegin ----------------------------- */
        ul1 = (unsigned long)Ngetbuf();	/* バッファを取得する。バッファは共通メモリ上に配置してある筈 */
        if (ul1 == 0) {		/* バッファ取得不能－init時に既に１つもないって....。 */
#if NTRACE >= 1
            Nprintf("am79c90 : failed to get buffer\n");
#endif
            return NE_HWERR;
        }
        ul1 = HI2AM(ul1+MESSH_SZ);	/* データ格納位置を79C90側アドレスに変換する */
#endif /* ---- NOCOPYモードend ------------------------------------- */
        STORE24(ul1, rmd->adr16, rmd->adr8);	/* RMDにデータ格納位置をセットする */
        rmd->cnterr = 0;
        rmd->bcnt = -MAXBUF - 4 + MESSH_SZ;
        rmd->flags = MD_OWN;
        rmd++;

#if NTRACE >= 5
/*        Nprintf("receive buffer(%d) allocated at 0x%08lX\n",i1+1,ul1); /**/
#endif

#ifndef NOCOPY /* ---- 通常モードbegin ----------------------------- */
        ul1 += MAXBUF;
#endif  /* ---- 通常モードend -------------------------------------- */
    }


/* TMDのエントリを初期化する。 */
/* NOCOPYモードではバッファは送信時に割り当てるので、ここではアドレスは */
/* 設定しない。 */
/* 通常モードでは固定アドレスを割り当てる。受信バッファの直後のアドレスを使用 */
/* する。 */
/* 送信完了割込時に、メッセージヘッダに送信完了をマークするためにオリジナル */
/* メッセージが格納されているUSNETバッファのアドレスが必要になる。通常モード */
/* ではTMDエントリにセットするバッファアドレスを４バイト後ろにずらし、そこに */
/* 格納する。NOCOPYモードではTMDエントリ中のバッファアドレスから逆算できる。 */
    for (i1 = 0; i1 < NTXBUFS; i1++) {
#if NTRACE >= 5
        Nprintf("init : initialize tmd at 0x%08lX\n",tmd); /**/
#endif
#ifndef NOCOPY /* ---- 通常モードbegin ----------------------------- */
        STORE24(ul1+4, tmd->adr16, tmd->adr8);
        ul1 += MAXBUF;
#if NTRACE >= 5
        Nprintf("transmit buffer allocated at 0x%08lX\n",ul1); /**/
#endif

#endif  /* ---- 通常モードend -------------------------------------- */
        tmd->bcnt = 0;		/* bcnt == 0はTMDエントリに有効なデータが入っていない事を示す */
        tmd->cnterr = 0;
        tmd->flags = MD_STP | MD_ENP;
        tmd++;
    }

/* 79C90を初期化する。 */
    ul1 = AM_BUFADDR_AM;		/* 初期化ブロックの位置。79C90側アドレス */
    _outw(tport + RAP, SELCSR3);
#ifdef LITTLE
    _outw(tport + CSR3,0);
#else
    _outw(tport + CSR3, BSWP);
#endif
    _outw(tport + RAP, SELCSR1);
    _outw(tport + CSR1, (unsigned short) ul1);
    _outw(tport + RAP, SELCSR2);
    _outw(tport + CSR2, (unsigned short) (ul1 >> 16));
    _outw(tport + RAP, SELCSR0);/* initialize the chip */
    _outw(tport + CSR, INIT);
    for (i1 = 0; i1 < 10000; i1++) {		/* 初期化終了を待つ */
        if (_inw(tport + CSR) & IDON) {
            goto lab8;
        }
    }
#if NTRACE >= 1				/* 初期化が終わらないって....。 */
    Nprintf("am79c90 : failed to init controller.\n");
#endif
    goto err1;
lab8:
    _outw(tport + CSR, IDON);			/* clear IDON bit */
    IRinstall(netp->irno[0], netno, irhan); /* install IR vector */
    _outw(tport + CSR, INEA | STRT);    /* start and enable chip */

#if NTRACE >= 1
    Nprintf("AM7990 %02x%02x%02x%02x%02x%02x IR%d P%x\n",
            netp->id.c[0], netp->id.c[1], netp->id.c[2], netp->id.c[3],
            netp->id.c[4], netp->id.c[5], netp->irno[0], tport);
#endif

/* 以下のグローバル変数はデバッグ用。各種のエラーの発生回数をカウントしている */
    QueueFullCount = 0;	/* USNET側のarriveキューfullによる棄却回数 */
    MissedCount = 0;		/* 79C90でのMISSエラーの発生回数 */
    MemAllocFail = 0;		/* メモリブロックハント失敗回数 */

    return 0;
err1:
    return NE_HWERR;
}


/* ==========================================================================
   Shut down the Ethernet interface.  Restores original IRQ, mask and vector.
   Turns off the controller. */

static void     shut(int netno)
{
    int             tport;
    struct NET     *netp;

    netp = &nets[netno];
    tport = netp->port;
    _outw(tport + RAP, SELCSR0);/* stop the chip */
    _outw(tport + CSR, STOP);
    IRrestore(netp->irno[0]);
}


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  ioctl()      I/O control function
**
**  int ioctl(int handle, int request, void *arg);
**
**  PARAMETERS:
**     (in)     handle   likely to be netno if any functions are added
**                       at this level
**     (in)     request  constant for the command to perform
**     (in)     arg      pointer to the parameters to the command, which
**                       will be specific to the command
**
**  DESCRIPTION:
**     This function allows special purpose functions specific to this
**     network layer to be performed.  At the moment, there are no functions
**     defined for this layer.
**
**  RETURNS:
**     0              Success
**     USS_ERR_INVAL  Support for the request not implemented
**
**  USAGE/COMMENTS:  
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
**
**  MODIFICATION HISTORY:
**
**  19-JAN-1999  SCD  Stub version
**
** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*/
static int ioctl(void *handle, enum ioctlreq request, void *arg, size_t size)
{
    (void)handle;    /* quietly ignore this argument */
    (void)request;   /* quietly ignore this argument */
    (void)arg;       /* quietly ignore this argument */
    (void)size;      /* quietly ignore this argument */

    return ussErrInval;
}


/* ===========================================================================
   Protocol table for the driver. */

GLOBALCONST
PTABLE ussAM79C90Table = {
    "AM79C90", init, shut, 0, opeN, closE, 0, writE, ioctl, 0, MESSH_SZ };
