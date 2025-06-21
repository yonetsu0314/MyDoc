/******************************************************************************
* NORTi  Network LAN Controller Driver                                        *
* for National Semiconductor DP8390/8390x and Realtek RTL80x9(AS).            *
*                                                                             *
*  Copyright (c) 1999-2002, MiSPO Co., Ltd.                                   *
*  All rights reserved.                                                       *
*                                                                             *
*  5/Apr/1999 Created to port to Hitachi ULSI MS7709RP01 board.          [YT] *
* 28/Apr/1999 Initial build combining NORTi3/SH3 Network v3.02.          [YT] *
* 19/May/1999 Beta release for MS7709RP01 board.                         [YT] *
*  1/Jun/1999 Fixed missing reception interrupts in a certain timing.    [YT] *
*  3/Jul/1999 Beta 2nd release for MS7709RP01 board.                     [YT] *
* 13/Jul/1999 Moved vset_psw call from lan_put_end() to lan_set_len().   [YT] *
* 14/Jul/1999 Added the lan_dmy_data[] zero array.                       [YT] *
*             Replaced lan_blkset with lan_blkout in lan_put_dmy().      [YT] *
* 24/Jul/1999 Added the LANC_RESET macro in lan_ini().                   [YT] *
* 28/Jul/1999 Initial release for MS7709RP01 board at Edition 1.00.      [YT] *
*  3/Oct/1999 Added support for NS DP83905 and DP83907.                  [YT] *
*             Added support for Realtek RTL80x9(AS).                     [YT] *
*  4/Oct/1999 Second build combining NORTi3/SH Network v3.03.            [YT] *
* 25/Oct/1999 Moved the LANC_RESET() macro to NONETHW.H.                 [YT] *
*             Moved the lan_in() and lan_out() macros to NONETHW.H.      [YT] *
* 26/Oct/1999 Third build combining NORTi3/SH3 Network v3.10.            [YT] *
* 13/Nov/1999 Fixed several bugs in polled I/O process.                  [YT] *
* 24/Nov/1999 Changed the <board> macro MS7709RP01 to MS7709SE.          [YT] *
* 25/Nov/1999 Beta release for [K9/DP83902] and [K9/RTL8019AS] boards.   [YT] *
*  7/Dec/1999 Added support for MS7709ASE01 and MS7750SE01 boards.       [YT] *
*             Added support for MS7612SE02 boards.                       [YT] *
*             Added the MS7700SE macro to group MS7700SE01 series boards.[YT] *
* 11/Dec/1999 Changed the BIG_ENDIAN macro to BIG.                       [YT] *
* 12/Dec/1999 Fourth build combining NORTi3/SH4 Network v3.10.           [YT] *
* 14/Dec/1999 Beta release for MS7750SE01 board.                         [YT] *
* 19/Dec/1999 Added process for odd number of bytes in lan_put_*().      [YT} *
* 25/Dec/1999 Added dis/ena_dsp() at start and end of Rx/Tx processes.   [YT} *
* 27/Dec/1999 Added DISABLE_PREE() and  ENABLE_PREE() macros.            [YT} *
* 29/Dec/1999 Initial release for [K9/DP83902] and [K9/RTL8019AS] boards.[YT] *
*             Beta 2nd release for MS7750SE01 board.                     [YT] *
* 16/Feb/2000 lan_get_len 8ビット時のnext,status読み出し順序入れ替え     [HM] *
*                                                                             *
* 13/Jun/2000 リングバッファのポインタ計算ミス修正                       [HI] *
*             割込み対応                                                      *
* 15/Jun/2000 EEPROM Load完了待ち追加                                    [HI] *
* 18/Jun/2000 MAC Address読出(ワードアクセス)時ミス修正                  [HI] *
*             rxmanage,txmanageをDP8390.ｈに移動                         [HI] *
* 14/Sep/2000 割り込みからの起床方法を修正                               [HM] *
* 21/Sep/2000 LANC_RESET(); 後の for 文での待ちループ数 50000→5000      [HM] *
* 05/Oct/2000 全面的に見直し, FULLDPLX 0→1, ループ数 1000, loop_cnt 前へ[HM] *
* 14/Oct/2000 他ボードと共通化のため BUS16BIT,BUS8BITマクロを追加        [HM] *
* 14/Oct/2000 他ボードと共通化のため BIG, LITTLE に _ENDIAN 追加等       [HM] *
* 14/Oct/2000 rxerr_callback 削除、RxManage, TxManage を本ファイルへ戻す [HM] *
* 15/Nov/2000 受信割込みがクリアできない不具合を修正                     [HM] *
* 14/Apr/2001 macaddrを(UH *)で参照の不具合修正                          [HM] *
* 17/May/2001 lan_dmy_data UB -> UH                                      [YS] *
* 02/Mar/2002 受信エラー割込みを無視, BNRY初期値を修正                   [HM] *
* 10/Mar/2002 BNRY,CURR初期値見直し, 8bit接続の時はバッファRAM半分       [HM] *
* 24/Oct/2002 SHとH8用をマージ                                           [KA] *
******************************************************************************/

#include <string.h>
#ifndef NULL
#include <stdio.h>
#endif
#include "norti3.h"
#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"
#include "dp8390.h"         /* Do not change the order of #include's for */
#include "nonethw.h"        /* dp8390.h and nonethw.h.                   */

/*#define DEBUG*/       /* activate for temporary DEBUGGING purpose only */

#if !defined(BIG_ENDIAN) && !defined(LITTLE_ENDIAN)
error ENDIAN Not Defined
#endif

/*
 * Select some configurations for this driver.
 */

#define DEVICE 0        /* device type */
                        /*   0 = NS DP8390 or NS32490 (NIC) */
                        /*   1 = NS DP83901 (SNIC) */
                        /*   2 = NS DP83902 or DP83903 (ST-NIC) */
                        /*   5 = NS DP83905 (AT/LANTIC) */
                        /*   7 = NS DP83907 (AT/LANTIC II) */
                        /*  81 = Realtek RTL8019 or RTL8029 */
                        /*  82 = Realtek RTL8019AS or RTL8029AS */

#define PHYMEDIA 0      /* physical media type */
                        /*   0 = 10BASE-T */
                        /*   1 = 10BASE2 */
                        /*   2 = 10BASE5 */

#define FULLDPLX 0      /* full duplex operation (for Realtek devices only) */
                        /*   0 = half duplex */
                        /*   1 = full duplex */

#define LBTYPE 2        /* loopback operaion type */
                        /*   0 = not allow loopback */
                        /*   1 = MAC level (internal NIC modlue) loopback */
                        /*   2 = ENDEC level (internal ENDEC) loopback */
                        /*   3 = physical-media level (external transceiver) loopback */

#define DATAXFER 16     /* data width for controller FIFO accesses */
                        /*   8 = 8-bit transfers */
                        /*  16 = 16-bit transfers */

#define SFNCFLAGS 0     /* flags that specify special functions */
                        /*  bit 0 = loopback mode */
                        /*  bit 6 = polled Rx (Rx without interrupt use) */
                        /*  bit 7 = polled Tx (Tx without interrupt use) */

    /* Group the MS7700SE01 series boards together */
/*
  NOTE: We can group the MS7700SE01 series boards together, because these
        boards have the same Ethernet hardware sepcification. If a new
        board whose Ethernet hardware sepcification is compatible with
        the MS7700SE01 series boards, you should add the definition for
        that board here.
*/
/* 16/Nov/2000 MZ Correspond MS7709x */
#if defined(MS7709) || defined(MS7709A) || defined(MS7727) || defined(MS7729R)
#undef DATAPORT
#undef RSTPORT
#ifndef RA
#define DATAPORT  0x40000       /* offset to data port window */
#define RSTPORT   0x80000       /* offset to reset port window */
#else
#define DATAPORT  (0x40000/RA)  /* offset to data port window */
#define RSTPORT   (0x80000/RA)  /* offset to reset port window */
#endif
#endif

#if defined(MS7709) || defined(MS7709A) || defined(MS7727) || defined(MS7729R) || defined(MS7750) || defined(MS7750S)
#define MS7700SE
#endif

/*#define LANVGA16*/  /* SH7045F EVABOD + LAN/VGA 16bit接続の場合★ */
#if (defined(LANVGA16)&&(DATAXFER==16))
#include "sh7040.h"
#define BUS16BIT()  sfr_outw(BSC_BCR1, 0x207f)  /* CS0,CS1,CS2:32bit, CS3:16bit */
#define BUS8BIT()   sfr_outw(BSC_BCR1, 0x2077)  /* CS0,CS1,CS2:32bit, CS3: 8bit */
#else
#define BUS16BIT()
#define BUS8BIT()
#endif

    /* Device type for the supported boards */
/*
  NOTE: If you are customizing this driver for a new target board, your
        board may not be compatible with one of the supported boards.
        In that case, you should change the DEVICE macro above for your
        new target board.
*/
#if defined(MS7700SE)       /* Hitachi ULSI MS7700SE01 series boards */
#undef DEVICE
#define DEVICE 2                /* The device is NS DP83902. */
#elif defined(MS7612SE)     /* Hitachi ULSI MS7612SE02 board */
#undef DEVICE
#define DEVICE 2                /* The device is NS DP83902. */
#elif BOARD == 10010        /* Manufacture [N], Board [K9/DP83902] */
#undef DEVICE
#define DEVICE 2                /* The device is NS DP83902. */
#elif BOARD == 10011        /* Manufacture [N], Board [K9/RTL8019AS] */
#undef DEVICE
#define DEVICE 82               /* The device is Realtek RTL8019AS. */
#endif

    /* Compatibility type */
/* NS DP83905 in I/O port mode, DP83907, Realtek devices */
#if ((DEVICE == 5) && !defined(LANC_BUFFER)) || (DEVICE == 7) || (DEVICE >= 81)
#define COMPTYPE 1                      /* NE2000 compatible */
/* NS DP83905 in shared memory mode */
#elif (DEVICE == 5) && defined(LANC_BUFFER)
#define COMPTYPE 2                      /* EtherCard PLUS16 compatible */
#else
#define COMPTYPE 0                      /* Non-compatible */
#endif

    /* Configuration error check */
#if (FULLDPLX == 1) && (DEVICE < 81)
#error "Only Realtek device allows full duplex operation."
#endif
#if (PHYMEDIA == 2) && (LBTYPE == 3)
#error "Transceiver loopback is not allowed with 10BASE5."
#endif

/************** User Defined Functions/Macros *****************************/

    /* Controller reset procedure */
#if COMPTYPE == 1           /* NE2000 needs its sepcific reset. */
#define LANC_RESET2() \
{   UB x; \
    x = lan_in(RSTPORT); \
    lan_out(RSTPORT, x); \
}                               /*  issue a read and write for reset */
#endif

                            /* Realtek devices in big endian */
#if defined(BIG_ENDIAN) && (DEVICE >= 81)
#undef lan_inw                  /* lan_inw() is defined in NONETHW.C */
#undef lan_outw                 /* lan_outw() is defined in NONETHW.C */
#endif

#ifndef lan_in
UB lan_in(unsigned);            /* input byte from controller port */
void lan_out(unsigned, UB);     /* output byte to controller port */
#endif
#ifndef lan_inw
UH lan_inw(unsigned);           /* input word from controller port */
void lan_outw(unsigned, UH);    /* output word to controller port */
#endif
ER lan_def_int(void);           /* interrupt installation routine */

#undef lan_blkin                /* low-level block I/O macros,     */
#undef lan_blkout               /* in/out data on controller port repeatedlly */
#if DATAXFER == 8               /*  for 8-bit transfers */
#define lan_blkin(p,c,n)    lan_blkinb(p,c,n)
#define lan_blkout(p,c,n)   lan_blkoutb(p,c,n)
#else                           /*  for 16-bit transfers */
#define lan_blkin(p,c,n)    lan_blkinw(p,c,n)
#define lan_blkout(p,c,n)   lan_blkoutw(p,c,n)
#endif

/************** Global Variable Declarations ******************************/

UB lan_loopback_test;           /* is loopback test operation required? */
UB lan_inter_nouse;             /* is polled execution required? */
W lan_error_count;              /* error counter */

/************** Local Macros/Variables ************************************/

    /*-- configuration depended macros --*/

    /* Prevent preemption between receive and send tasks.     */
    /* (The necessity of these macros depends on controller.) */
#define DISABLE_PREE() \
{ \
    if (c_ip_rcv_tsk.itskpri != c_ip_snd_tsk.itskpri) \
        dis_dsp(); \
}                               /* disable kernel preemption */
#define ENABLE_PREE() \
{ \
    if (c_ip_rcv_tsk.itskpri != c_ip_snd_tsk.itskpri) \
        ena_dsp(); \
}                               /* enable kernel preemption */

    /*-- controller specific macros --*/

#define ETHADRSIZE 6            /* byte size of Ethernet address */
#define ETHMAXLEN 1514          /* maximum length of packet (excluding CRC) */

#define SMPAGESIZE 256          /* page size of local/shared memory */

#if COMPTYPE == 1           /* The board has a NE2000 compatible controller. */
#if DATAXFER == 8               /* 8bit接続の時はバッファRAM半分 */
#define SMPOOLSIZE 0x2000       /* local/shared memory buffer pool size */
#else
#define SMPOOLSIZE 0x4000       /* local/shared memory buffer pool size */
#endif
#define SM_TXSTART_PG 0x40      /* first page of trasnmit buffer */
#define SM_RXSTART_PG 0x46      /* starting page of receive buffer ring */
#define SM_RXSTOP_PG  0x40+(SMPOOLSIZE>>8) /* last page +1 of rx buffer ring */
#elif defined(MS7700SE)     /* Hitachi ULSI MS7700SE01 series boards */
#define SMPOOLSIZE 0x8000       /* local/shared memory buffer pool size */
#define SM_TXSTART_PG 0x80      /* first page of trasnmit buffer */
#define SM_RXSTART_PG 0x86      /* starting page of receive buffer ring */
#define SM_RXSTOP_PG  0x80+(SMPOOLSIZE>>8) /* last page +1 of rx buffer ring */
#elif defined(MS7612SE)     /* Hitachi ULSI MS7612SE02 board */
#define SMPOOLSIZE 0x10000      /* local/shared memory buffer pool size */
#define SM_TXSTART_PG 0x00      /* first page of trasnmit buffer */
#define SM_RXSTART_PG 0x06      /* starting page of receive buffer ring */
#define SM_RXSTOP_PG  0x00+(SMPOOLSIZE>>8) /* last page +1 of rx buffer ring */
#elif BOARD == 10010        /* Manufacture [N], Board [K9/DP83902] */
#define SMPOOLSIZE 0x8000       /* local/shared memory buffer pool size */
#define SM_TXSTART_PG 0x80      /* first page of trasnmit buffer */
#define SM_RXSTART_PG 0x86      /* starting page of receive buffer ring */
#define SM_RXSTOP_PG  0x80+(SMPOOLSIZE>>8) /* last page +1 of rx buffer ring */
#else                       /* Other boards (for a new board) */
/*== Change these values as needed for your new target board. ========*/
#define SMPOOLSIZE 0x2000       /* local/shared memory buffer pool size */
#define SM_TXSTART_PG 0x00      /* first page of trasnmit buffer */
#define SM_RXSTART_PG 0x06      /* starting page of receive buffer ring */
#define SM_RXSTOP_PG  0x00+(SMPOOLSIZE>>8)  /* last page +1 of rx buffer ring */
/*====================================================================*/
#endif

    /*-- controller specific variables --*/

/* Dummy zero data for transmit packets shorter than 60 bytes. When data
   size of a transmit packet is less than 46 bytes, we will pad zeros
   into its data field by using this lan_dmy_data[] array. This means
   that total size of Ethernet packets must always be equal to or greater
   than 60 bytes. */
static const UH lan_dmy_data[46/2] = { 0 };

static UB intr_mask;            /* interrupt mask */
static UW loop_count;           /* loop count for 2 ms wait */

static struct RxManage {        /* receive management table */
    UB      status;             /*  receive status */
    UB      startpage;          /*  page of receiving packet stored */
    UB      nextpage;           /*  page of next packet stored */
    UB      currpage;           /*  page of controller processing */
    UH      bytecount;          /*  byte count of receive packet */
    UH      eobsize;            /*  byte size to end of receive buffers */
    UH      shmempos;           /*  byte position index to receive buffer ring */
    UH      buffpos;            /*  byte position index to receive buffer */
} rxmanage;
static struct TxManage {        /* transmit management table */
    UB      initiated;          /*  is transmit operation initiated once? */
    UB      busy;               /*  is transmit operation in progress? */
    UB      status;             /*  transmit status */
    UH      buffpos;            /*  byte position index to transmit buffer */
    UB      lastbyte;           /*  last byte when number of data bytes is odd */
    UB      nullbyte;           /*  null byte when number of data bytes is odd */
} txmanage;

static UW get_time_msec()
{
    SYSTIME         pk_time;
    get_tim(&pk_time);
    return pk_time.ltime * MSEC;
}

static ID tx_wtskid;            /* 14/Sep/2000 追加 */
static ID rx_wtskid;            /* 14/Sep/2000 追加 */

int nop(void)                   { return *(volatile int *)0xa0000000; }
UB lan_in(unsigned p)           { nop(); return (UB)(*(volatile UH *)lan_port(p) >> 8); }
void lan_out(unsigned p, UB c)  { nop(); *(volatile UH *)lan_port(p) = (UH)(c) << 8; }
UH lan_inw(unsigned p)          { nop(); return *(volatile UH *)lan_port(p); }
void lan_outw(unsigned p, UH c) { nop(); *(volatile UH *)lan_port(p) = (c); }

/*****************************************************************************
*
* Initialize the Ethernet controller.
*
******************************************************************************/

ER lan_ini(UB *macaddr)
{
    int i1, i2, i3;
    UB ub1, ub2;
    UW ul1;
    UB *p;
    UB ma;
    UB mask;

    /* Set the flag if a special function is spcified. */

#if SFNCFLAGS & 0x1
    lan_loopback_test = TRUE;           /* loopback mode */
#else
    lan_loopback_test = FALSE;
#endif
    lan_inter_nouse = SFNCFLAGS & 0xc0; /* polled Rx, Tx */
    lan_error_count = 0;

    /* Check if we are required to read the MAC address from the board. */

    p = macaddr;                        /* get Ethernet address passed */
    if ((p[0] | p[1] | p[2] | p[3] | p[4] | p[5]) == 0x00)
        ma = 0x00;                      /* passed address is all 00 */
    else if ((p[0] & p[1] & p[2] & p[3] & p[4] & p[5]) == 0xff)
        ma = 0xff;                      /* passed address is all FF */
    else
        ma = 1;                         /* passed address is available */

    /* Initialize the local variables. */

    tx_wtskid = rx_wtskid = FALSE;      /* 14/Sep/2000 追加 */
    memset(&rxmanage, 0, sizeof(rxmanage));
    memset(&txmanage, 0, sizeof(txmanage));
    rxmanage.startpage =
    rxmanage.nextpage =
    rxmanage.currpage = SM_RXSTART_PG;

/* 16/Nov/2000 MZ Correspond MS7709x */
#if defined(MS7709) || defined(MS7709A) || defined(MS7727) || defined(MS7729R)
    /* Reset the NS8390 and configure it. */
    lan_out(RSTPORT, 0);                /* hardware reset to the board */
    lan_out(RSTPORT, 1);                /*  (RESET pin of DP83902 is low active) */
#endif

    /* Reset the controller and configure it. */

#ifdef LANC_RESET
    LANC_RESET();                       /* reset in hardware level */
    for (i1 = 0; i1 < 1000; i1++);      /* delay for reset */
#endif
#ifdef LANC_RESET2
    LANC_RESET2();                      /* reset in board level */
    for (i1 = 0; i1 < 1000; i1++);      /* delay for reset */
#endif

    /* Calibrate a delay loop for 2 milliseconds */

    loop_count = 1024;
    for (i1=(int)get_time_msec(); i1==(i2=(int)get_time_msec()); ) ;
    for (i3=0; i2==(i1=(int)get_time_msec()); i3++)
        for (ul1 = 0; ul1 < loop_count; ul1++) ;
    loop_count = (UW)i3 * (UW)(2048/(i1-i2));
#if DEVICE >= 81
    /* Wait EEPROM Loaded....   */
    lan_out(CMDR, CMDR_PS3+CMDR_RD2);   /* select Page 3   */
    do {
        ub1 = lan_in(_9346CR) & _9346CR_EMM;
    } while ( ub1 != 0 );
#endif
    lan_out(CMDR, CMDR_STP+CMDR_RD2);   /* stop the controller */
    lan_out(CMDR, CMDR_PS0);
    for (i1 = 0; i1 < 1000; i1++);      /* delay for stop */
    lan_out(ISR, 0xff);                 /* clear and mask interrupts */
    lan_out(IMR, 0x00);
#if DATAXFER == 8
    ub1 = DCR_LS+DCR_FT10;              /* configure 8-bit transfers */
#else
    ub1 = DCR_WTS+DCR_LS+DCR_FT10;      /* configure 16-bit transfers */
#ifdef BIG_ENDIAN
    ub1 += DCR_BOS;                     /* Motorola byte order */
#endif
#endif
    lan_out(DCR, ub1);

#if COMPTYPE == 0                   /* The board is non-compatible. */
    if (ma == 0x00 || ma == 0xff)       /* unless EA is configured */
    {
#ifdef LANC_GETEA
        LANC_GETEA(macaddr);            /* get address from board */
#else                                   /* if EA config is not allowed, */
        return -1;                      /*   return parameter error     */
#endif
    }
#endif

#if COMPTYPE == 1                   /* The board is NE2000 compatible. */
    /* The I/O port mode of NS DP83905, DP83907 or Realtek devices is
       compatible with Novell NE2000. So, in this mode, most of the
       initialization process is same as that board. */

    if (ma == 0x00 || ma == 0xff)       /* unless EA is configured */
    {                                   /* get address from board */
#if DATAXFER == 8
        lan_out(RBCR0, ETHADRSIZE);
#else
        lan_out(RBCR0, 2*ETHADRSIZE);
#endif
        lan_out(RBCR1, 0);
        lan_out(RSAR0, 0);
        lan_out(RSAR1, 0);
        lan_out(CMDR, CMDR_PS0+CMDR_RRE+CMDR_STA);
        BUS16BIT();
        for (i2 = 0; i2 < ETHADRSIZE; i2++)
#if DATAXFER == 8
            macaddr[i2] = lan_in(DATAPORT);
#else
            macaddr[i2] = (UB)(lan_inw(DATAPORT)>>8);
            /* Mac address はワードアクセスで読むと下位に入りますが
               lan_inw(BIG ENDIAN)ではSWAPされるので、上位に入る。
               それを、UB配列に保存すると下位が残るため             */
#endif
        BUS8BIT();
    }
#endif

#if COMPTYPE == 2                   /* The board is  EtherCard compatible. */
    /* The shared memory mode of NS DP83905 is compatible with SMC
       EtherCard  PLUS16 (formerly called WD8013). So, in this mode, most
       of the initialization process is same as that board. */

    if (ma == 0x00 || ma == 0xff)       /* unless EA is configured */
    {                                   /* get address from board */
        ub2 = 0;
        for (i2 = 0; i2 < 8; i2++)
        {
            ub1 = lan_in(NADDR+i2);
            ub2 += ub1;
            if (i2 < ETHADRSIZE)
                macaddr[i2] = ub1;
        }
        if (ub2 != (UB)0xff)            /* if the checksum is invalid, */
            return -1;                  /*  return hardware error */
    }

    lan_out(CTLR1, CTLR1_RESET);        /* reset controller and buffer */
    lan_out(CTLR1, 0);
                                        /* set 18-13 bits of buffer address */
    lan_out(CTLR1, CTLR1_MEME+(UB)((LANC_BUFFER>>13)&CTLR1_DECSA));
#if DATAXFER == 8                       /* set 23-19 bits of buffer address */
    lan_out(CTLR2, (UB)((LANC_BUFFER>>19)&CTLR2_DECLA));
#else
    lan_out(CTLR2, CTLR2_MEM16+CTLR2_MEMW+(UB)((LANC_BUFFER>>19)&CTLR2_DECLA));
#endif
#endif

    /* Set the physical media type. */

#if (DEVICE == 5) || (DEVICE == 7)      /* NS DP83905 or DP83907 */
    ub1 = lan_in(MCRB) & ~(MCRB_GDLINK+MCRB_PHYS);
    lan_out(MCRB, ub1+PHYMEDIA);
#endif
#if DEVICE >= 81                        /* Realtek devices */
    lan_out(CMDR, CMDR_PS3+CMDR_RD2);
    ub1 = lan_in(_9346CR) & ~_9346CR_EMM;
    lan_out(_9346CR, ub1+_9346CR_CRWE); /* enter config regs write enable */
    ub1 = lan_in(CONFIG2) & ~CONFIG2_PL;
#if PHYMEDIA == 0
    lan_out(CONFIG2, ub1+CONFIG2_PL0);  /* 10BASE-T with link test enable */
#elif PHYMEDIA == 1
    lan_out(CONFIG2, ub1+CONFIG2_PL3);  /* 10BASE2 */
#elif PHYMEDIA == 2
    lan_out(CONFIG2, ub1+CONFIG2_PL2);  /* 10BASE5 */
#endif
#if BOARD == 10011                  /* Manufacture [N], Board [K9/RTL8019AS] */
#ifdef DEBUG
    lan_out(CONFIG3, 0);            /*+< for debugging with invalid EEPROM >+*/
#endif
#endif
#if FULLDPLX == 1                       /* if full duplex is configured, */
    ub1 = lan_in(CONFIG3) & ~(CONFIG3_FUDP+CONFIG3_RESRVD);
    lan_out(CONFIG3, ub1+CONFIG3_FUDP); /* puts RTL8019 in full duplex mode */
#endif
    ub1 = lan_in(_9346CR) & ~_9346CR_EMM;
    lan_out(_9346CR, ub1+_9346CR_NORM); /* return to normal mode */
/*- lan_out(CMDR, CMDR_PS0+CMDR_RD2); -*/
#endif

    /* Set the MAC address to the controller. */

    lan_out(CMDR, CMDR_PS1+CMDR_RD2);
    for (i2 = 0; i2 < PARsize; i2++)    /* initialize physical address */
        lan_out(PAR+i2, macaddr[i2]);
    for (i2 = 0; i2 < MARsize; i2++)    /* clear all multicast addresses */
        lan_out(MAR+i2, 0);
    lan_out(CURR, SM_RXSTART_PG);       /* current receive page */

    /* Configure the receiver and tramsmitter operations. */

    if (lan_loopback_test)              /* if loopback is required,   */
    {
        lan_out(CMDR, CMDR_PS2+CMDR_RD2);
        ub1 = lan_in(DCR);
        ub1 -= DCR_LS;
        lan_out(DCR, ub1 - DCR_LS);     /* activate loopback (LS is 0 active) */
    }
    lan_out(CMDR, CMDR_PS0+CMDR_RD2);
    lan_out(PSTART, SM_RXSTART_PG);     /* start page for receive */
    lan_out(BNRY, SM_RXSTOP_PG-1);      /* initial boundary page for receive */
    lan_out(PSTOP, SM_RXSTOP_PG);       /* stop page for receive */
    if (lan_in(BNRY) != SM_RXSTOP_PG-1) /* read back check (added 21/Mar/2001) */
        return -1;                      /* return hardware error */
    mask = 0;
  #if 0 /* 05/Oct/2000 削除, 割り込みは必要な時だけ許可のため */
    if (!(lan_inter_nouse & 0x40))      /* mask for receive interrupts */
        mask |= IMR_PRXE+IMR_RXEE+IMR_OVWE;
    if (!(lan_inter_nouse & 0x80))      /* mask for transmit interrupts */
        mask |= IMR_PTXE+IMR_TXEE;
    if (mask != 0)
        lan_out(IMR, mask);             /* enable interrupts */
  #endif
    intr_mask = mask;                   /* save the interrupt mask */
    lan_out(RCR, RCR_MON);              /* disable the receiver once */
    ub1 = 0;                            /* assume normal transmit operation */
    if (lan_loopback_test)              /* if loopback is required, configure it */
#if LBTYPE == 0                         /*  if we should not allow loopback, */
        return -1;                      /*   return parameter error          */
#elif LBTYPE == 1
        ub1 += TCR_LB01;                /* internal NIC module loopback */
#elif LBTYPE == 2
        ub1 += TCR_LB10;                /* internal ENDEC module loopback */
#elif LBTYPE == 3
        ub1 += TCR_LB11;                /* external transceiver loopback */
#endif
    ub2 = RCR_AB;                       /* accept broadcast packets */
    if (ma == 0xff)                     /* is promiscuous mode required? */
        ub2 += RCR_PRO;                 /* accept all packets with any phys addr */
    lan_out(TCR, ub1);
    lan_out(CMDR, CMDR_STA+CMDR_RD2);   /* put the controller on-line */
    lan_out(RCR, ub2);                  /* enable the receiver */

    /* Calibrate a delay loop for 2 milliseconds */
 /*      move the upper
    loop_count = 1024;
    for (i1=(int)get_time_msec(); i1==(i2=(int)get_time_msec()); ) ;
    for (i3=0; i2==(i1=(int)get_time_msec()); i3++)
        for (ul1 = 0; ul1 < loop_count; ul1++) ;
    loop_count = (UW)i3 * (UW)(2048/(i1-i2));
*/
    /* Install an interrupt handler for our interrupts. */

    if ((lan_inter_nouse & 0xc0) != 0xc0)
        return lan_def_int();

    return E_OK;
}

/*****************************************************************************
*
* Interrupt handler for the Ethernet controller.
*
******************************************************************************/

void lan_intr(void) /* 05/Oct/2000 全面的に修正 */
{
    int isr;
    ID id;

    isr = lan_in(ISR);                  /* 割込みステータスを得る */
    isr &= (ISR_PRX|ISR_PTX|ISR_TXE);   /* 使わない割込みを除外 */
    lan_out(ISR, (UB)isr);              /* 割込みステータスをクリア */

    if (isr & ISR_PRX) {                /* 受信側の割込みの場合 */
        if ((id = rx_wtskid) != FALSE) {/* 受信割込み待ち中のタスクあるなら */
            rx_wtskid = FALSE;
            wup_tsk(id);                /* その(IP受信)タスクを起床 */
        }
    }

    if (isr & (ISR_PTX|ISR_TXE)) {      /* 送信側の割込みの場合 */
        if ((id = tx_wtskid) != FALSE) {/* 送信割込み待ち中のタスクあるなら */
            tx_wtskid = FALSE;
            wup_tsk(id);                /* その(IP送信)タスクを起床 */
        }
    }
}

/*****************************************************************************
*
* Overflow Routine
*
******************************************************************************/

static void overflow(void) /* 05/Oct/2000 追加 */
{
    int txp, i;

    txp = lan_in(CMDR) & CMDR_TXP;      /* コマンドレジスタのTXPビット記憶 */
    lan_out(CMDR, CMDR_STP+CMDR_RD2);   /* STOPコマンド発行 */
    for (i = 0; i < loop_count; i++);   /* 2ms wait */
    lan_out(RBCR0, 0);                  /* Remote DMAのバイトカウントレジス */
    lan_out(RBCR1, 0);                  /* タをクリア */
    if (txp                             /* TXP=1ででも、割込みステータスの */
      && !(lan_in(ISR) & (ISR_PTX+ISR_TXE))) /* PTXやTEXが1の場合 */
        txp = 0;                        /* 再送信は不要 */
    lan_out(TCR, TCR_LB01);             /* ループバックモードで */
    lan_out(CMDR, CMDR_STA+CMDR_RD2);   /* STARTコマンドを発行 */
    lan_out(ISR, ISR_OVW);              /* 割込みステータスのOVWをクリア */
    lan_out(TCR, 0);                    /* ループバックモードから抜ける */
    if (txp)                            /* 再送信が必要な場合 */
        lan_out(CMDR, CMDR_TXP+CMDR_RD2); /* 送信開始コマンドを発行 */
}

/*****************************************************************************
*
* Wait for a reception interrupt by the Ethernet controller
*
******************************************************************************/

ER lan_wai_rcv(TMO tmout) /* 05/Oct/2000 全面的に修正 */
{
  #if 0 /* static for debug */
    static int curr, bnry, i;
  #else
    int curr, bnry, i;
  #endif
    ER ercd;
    UINT psw;

    for (;;) {

        /* バッファメモリに受信データがあるならOK */

        bnry = lan_in(BNRY);                /* 境界ページBNRYを得る */
        psw = vdis_psw();
        lan_out(CMDR, CMDR_PS1+CMDR_RD2);
      #if 1
        curr = lan_in(CURR);                /* カレントページCURRを得る */
      #else
        for (i = 0; i < 3; i++) {
            curr = lan_in(CURR);            /* カレントページCURRを得る */
            if (curr != 0xff)               /* DP83902Aで0xffが読める場合がある */
                break;
        }
      #endif
        lan_out(CMDR, CMDR_PS0+CMDR_RD2);
        vset_psw(psw);
        rxmanage.currpage = (UB)curr;       /* for debug or monitor */
        if (++bnry >= SM_RXSTOP_PG)         /* バッファの切れ目をまたいでい */
            bnry = SM_RXSTART_PG;           /* る場合の補正 */
        if (bnry != curr) {                 /* BNRY+1≠CURR ならデータあり */
            lan_out(ISR, ISR_PRX|ISR_RXE);  /* 受信割込みステータスをクリア */
            return E_OK;                    /* 正常終了 */
        }

        /* オーバーフロー処理 */

        while (lan_in(ISR) & ISR_OVW)
            overflow();

        /* 割込みを使用しない場合はポーリング */

        if (lan_inter_nouse & 0x40) {
            for (;;) {
                dly_tsk(1);
                if (lan_in(ISR) & (ISR_PRX|ISR_RXE))
                    break;
                if (tmout != TMO_FEVR && tmout-- == 0)
                    return E_TMOUT;
            }
        }

        /* 割込みを使用する場合は受信割込み待ち */

        rx_wtskid = vget_tid();
        lan_out(IMR, intr_mask |= IMR_PRXE);
        ercd = tslp_tsk(tmout);
        lan_out(IMR, intr_mask &= ~IMR_PRXE);
        if (ercd != E_OK) {
            rx_wtskid = FALSE;
            return ercd;
        }
    }
}

/*****************************************************************************
*
* Wait for a transmission interrupt by the Ethernet controller
*
******************************************************************************/

ER lan_wai_snd(TMO tmout) /* 05/Oct/2000 全面的に修正 */
{
    ER ercd;
    UINT psw;

    for (;;) {

        /* 送信中でなければOK */

        if (!(lan_in(CMDR) & CMDR_TXP)) {   /* TXP = 0 なら */
            lan_out(ISR, ISR_PTX|ISR_TXE);  /* 送信割込みステータスをクリア */
            return E_OK;                    /* 正常終了 */
        }

        /* 割込みを使用しない場合はポーリング */

        if (lan_inter_nouse & 0x80) {
            for (;;) {
                dly_tsk(1);
                if (lan_in(ISR) & (ISR_PTX+ISR_TXE))
                    break;
                if (tmout != TMO_FEVR && tmout-- == 0)
                    return E_TMOUT;
            }
        }

        /* 割込みを使用する場合は送信割込み待ち */

        tx_wtskid = vget_tid();
        lan_out(IMR, intr_mask |= (IMR_PTXE|IMR_TXEE));
        ercd = tslp_tsk(tmout);
        lan_out(IMR, intr_mask &= ~(IMR_PTXE|IMR_TXEE));
        if (ercd != E_OK) {
            tx_wtskid = FALSE;
            return ercd;
        }
    }
}

/*****************************************************************************
*
* Inform the byte size of the packet to be read.
*
******************************************************************************/

ER lan_get_len(UH *len)
{
  #if 0 /* static for debug */
    static UH uh1, count;
    static UB status;
    static int bnry, start, next;
  #else
    UH uh1, count;
    UB status;
    int bnry, start, next;
  #endif

    DISABLE_PREE();

    /* In big endian mode with Realtek devices, We found that the memory
       places of the Receive Status and Next Packet Pointer were in reverse
       order. This is different from NS devices. This problem may be a bug
       of Realtek devices. */

    bnry = lan_in(BNRY);                /* 境界ページBNRYを得る */
    start = bnry + 1;                   /* get start page for receive packet */
    if (start >= SM_RXSTOP_PG)          /* If the start points at end of ring,  */
        start = SM_RXSTART_PG;          /*  we must loop it back to top of ring. */
    lan_out(RBCR0, 4);                  /* remote read 4 bytes */
    lan_out(RBCR1, 0);
    lan_out(RSAR0, 0);
    lan_out(RSAR1, (UB)start);              /* at start address of packet stored */
    lan_out(CMDR, CMDR_PS0+CMDR_RRE+CMDR_STA);
    BUS16BIT();
#if defined(LITTLE_ENDIAN) || (DEVICE < 81)    /* NS, or Realtek in little endian */
#if DATAXFER == 8
    status = lan_in(DATAPORT);          /* get the receive status */
    next = lan_in(DATAPORT);            /* get the next packet pointer */
    count = lan_in(DATAPORT);
    count |= lan_in(DATAPORT) << 8;     /* get the receive byte count */
#else
    uh1 = lan_inw(DATAPORT);
    status = (UB)uh1;                   /* get the receive status */
    next = uh1 >> 8;                    /* get the next packet pointer */
    count = lan_inw(DATAPORT);          /* get the receive byte count */
#ifdef BIG_ENDIAN                              /* swap upper and lower bytes */
    count = (count >> 8) | (count << 8);
#endif
#endif
#else                                   /* Realtek in big endian */
#if DATAXFER == 8
    status = lan_in(DATAPORT);          /* get the receive status */
    next = lan_in(DATAPORT);            /* get the next packet pointer */
    count = lan_in(DATAPORT);
    count |= lan_in(DATAPORT) << 8;     /* get the receive byte count */
#else
    uh1 = lan_inw(DATAPORT);
    next = (UB)uh1;                     /* get the next packet pointer */
    status = uh1 >> 8;                  /* get the receive status */
    count = lan_inw(DATAPORT);          /* get the receive byte count */
#ifdef BIG_ENDIAN                              /* swap upper and lower bytes */
    count = (count >> 8) | (count << 8);
#endif
#endif
#endif
    BUS8BIT();

    /* If the next packet pointer does not point within receive buffer
       range, that packet information is something wrong. */

    if (next < SM_RXSTART_PG || next >= SM_RXSTOP_PG)
    {
        lan_error_count++;
        ENABLE_PREE();
        return -1;
    }

    rxmanage.status = status;
    rxmanage.bytecount = count;
    rxmanage.startpage = (UB)start;
    rxmanage.nextpage = (UB)next;
    rxmanage.eobsize = (SM_RXSTOP_PG - start) * SMPAGESIZE - 4;
    rxmanage.shmempos = start * SMPAGESIZE + 4;

    /* If the receive status indicates an error, or the receive byte
       count is greater than maximum length of Ethernet packet, we
       should discard this packet. */

    if ((status & RSR_PRX) == 0 || (count - 4) > ETHMAXLEN)
    {
        lan_error_count++;
        lan_get_end();
        return -1;
    }

    *len = rxmanage.bytecount - 4;      /* rx packets are including CRC */
    return E_OK;
}

/*****************************************************************************
*
* Read out packet data from the controller in specified size.
*
* buf       buffer to store packet data
* len       number of bytes to be read
*
******************************************************************************/

ER lan_get_pkt(void *buf, int len)
{
    UB *p;
    UH n;

    p = (UB *)buf;

    /* If the receive packet is placed across the end of buffer, we need
       to copy the packet bytes turning round in receive buffer ring. */

    if (rxmanage.eobsize < rxmanage.bytecount &&
        rxmanage.buffpos < rxmanage.eobsize)
    {   /* read packet bytes within the range reaching the end of buffer */
        n = rxmanage.eobsize - rxmanage.buffpos;
        if (len < n)
            n = len;
        lan_out(RBCR0, (UB)n);          /* read the bytes calculated */
        lan_out(RBCR1, n>>8);
        lan_out(CMDR, CMDR_PS0+CMDR_RRE+CMDR_STA);
        BUS16BIT();
        lan_blkin(DATAPORT, p, n);
        BUS8BIT();
        len -= n;
        p += n;
        rxmanage.shmempos += n;
        rxmanage.buffpos += n;
        if (rxmanage.buffpos >= rxmanage.eobsize)
        {
            lan_out(RSAR0, 0);          /* turn round the buffer ring */
            lan_out(RSAR1, SM_RXSTART_PG);
            rxmanage.shmempos = SM_RXSTART_PG * SMPAGESIZE;
        }
    }

    if (len > 0)
    {
        lan_out(RBCR0, (UB)len);        /* read the remaining bytes */
        lan_out(RBCR1, len>>8);
        lan_out(CMDR, CMDR_PS0+CMDR_RRE+CMDR_STA);
        BUS16BIT();
        lan_blkin(DATAPORT, p, len);
        BUS8BIT();
        rxmanage.shmempos += len;
        rxmanage.buffpos += len;
    }
    return E_OK;
}

/*****************************************************************************
*
* Finish reading packet data.
*
******************************************************************************/

ER lan_get_end(void)
{
    int bnry;

    /* Update the boundary pointer to the receive buffer ring. */
    bnry = (int)rxmanage.nextpage - 1;
    if (bnry < SM_RXSTART_PG)
        bnry = SM_RXSTOP_PG - 1;
    lan_out(BNRY, (UB)bnry);

    rxmanage.bytecount = 0;
    rxmanage.buffpos = 0;

    ENABLE_PREE();
    return E_OK;
}

/*****************************************************************************
*
* Skip to read packet data in specified size.
*
* len       size of the pakcet bytes to be skipped
*
******************************************************************************/

ER lan_skp_pkt(int len)
{
    UH n;

    /* If the receive packet is placed across the end of buffer, we
       need to skip the packet bytes turning round in receive buffer
       ring. */

    if (rxmanage.eobsize < rxmanage.bytecount &&
        rxmanage.buffpos < rxmanage.eobsize)
    {
        n = rxmanage.eobsize - rxmanage.buffpos;
        if (len < n)
            n = len;
        len -= n;
        rxmanage.shmempos += n;
        rxmanage.buffpos += n;
        if (rxmanage.buffpos >= rxmanage.eobsize)
            rxmanage.shmempos = SM_RXSTART_PG * SMPAGESIZE;
    }

    if (len > 0)
    {
        rxmanage.shmempos += len;
        rxmanage.buffpos += len;
    }
    lan_out(RSAR0, (UB)rxmanage.shmempos);
    lan_out(RSAR1, rxmanage.shmempos>>8);

    /* If all the remaining bytes of the receive packet were skipped,
       release the buffer memory in which the packet was stored. */

    if (rxmanage.buffpos >= rxmanage.bytecount - 4)
        lan_get_end();
    return E_OK;
}

/*****************************************************************************
*
* Informed the byte size of the packet data to write.
*
******************************************************************************/

ER lan_set_len(int len)
{
    UINT psw;

 /* txmanage.initiated = TRUE; 14/Sep/2000 削除 */

    txmanage.buffpos = 0;

    DISABLE_PREE();

  #if 0 /* 05/Oct/2000 削除 */
    if ((lan_inter_nouse & 0xc0) != 0xc0)
    {
        psw = vdis_psw();               /* disable CPU interrupts */
        lan_out(IMR, 0x00);             /* disable controller interrupts */
        vset_psw(psw);                  /* enable CPU interrupts */
    }
  #endif
    lan_out(TBCR0, (UB)len);            /* trasnmit byte count */
    lan_out(TBCR1, len>>8);
    lan_out(CMDR, CMDR_RD2+CMDR_STA);
    lan_out(RBCR0, (UB)len);            /* remote DMA byte count */
    lan_out(RBCR1, len>>8);
    lan_out(RSAR0, 0);                  /* remote DMA start address */
    lan_out(RSAR1, SM_TXSTART_PG);      /* start remote DMA write */
    lan_out(CMDR, CMDR_PS0+CMDR_RWR+CMDR_STA);
    return E_OK;
}

/*****************************************************************************
*
* Write packet data into controller buffer in specified size
*
* data      buffer containing packet data
* len       number of bytes to write (> 0)
*
******************************************************************************/

ER lan_put_pkt(const void *data, int len)
{
    const UB *p;
    int even_len;

    p = (const UB *)data;

#if DATAXFER == 8
    lan_blkout(DATAPORT, p, len);
#else
    if (!(len & 1))
        even_len = len;                 /* even number of bytes */
    else
    {                                   /* odd number of bytes */
        even_len = len - 1;
        txmanage.lastbyte = p[len-1];   /* save last byte */
    }
    BUS16BIT();
    lan_blkout(DATAPORT, p, even_len);
    BUS8BIT();
#endif

    txmanage.buffpos += len;
    return E_OK;
}

/*****************************************************************************
*
* Write null bytes into controller buffer in specified size.
*   (Called if a packet sending is shorter than 60 bytes.)
*
* len       number of null bytes to write (> 0)
*
******************************************************************************/

ER lan_put_dmy(int len)
{
    int even_len;

#if DATAXFER == 8
    lan_blkout(DATAPORT, lan_dmy_data, len);
#else
    BUS16BIT();
    if (!(txmanage.buffpos & 1))
        even_len = len;                 /* even number of bytes */
    else
    {                                   /* odd number of bytes */
        even_len = len - 1;             /* write last byte appending a zero */
        lan_blkout(DATAPORT, &txmanage.lastbyte, 2);
    }
    lan_blkout(DATAPORT, lan_dmy_data, even_len);
    BUS8BIT();
#endif

    txmanage.buffpos += len;
    return E_OK;
}

/*****************************************************************************
*
* Finish writing packet data.
*   (Start a packet transmit operation actually.)
*
******************************************************************************/

ER lan_put_end(void)
{
#if DATAXFER == 16
    if (txmanage.buffpos & 1) {         /* write last byte */
        BUS16BIT();
        lan_blkout(DATAPORT, &txmanage.lastbyte, 1);
        BUS8BIT();
    }
#endif

    txmanage.buffpos = 0;

    /* Start the transmist operation. */

    lan_out(TPSR, SM_TXSTART_PG);       /* set transmit start page  */
    lan_out(CMDR, CMDR_TXP+CMDR_RD2+CMDR_STA);  /* start transmission */
  #if 0 /* 05/Oct/2000 削除 */
    if ((lan_inter_nouse & 0xc0) != 0xc0)
        lan_out(IMR, intr_mask);        /* enable controller interrupts */
  #else
    while (lan_in(CMDR) & CMDR_TXP)
        ;
  #endif

    ENABLE_PREE();
    return E_OK;
}

/* end */
