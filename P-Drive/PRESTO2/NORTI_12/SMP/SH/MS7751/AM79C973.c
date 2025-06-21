/******************************************************************************
* Driver file for LAN controller AM79C973                                     *
*                                                                             *
*  File name : AM79C973.c                                                     *
*  Copyright (c)  1995-2002, MiSPO Co., Ltd.                                  *
*  All rights reserved.                                                       *
*                                                                             *
*                                                                             *
* 01/Jul/2001      file created                                         [SP]  *
* 14/Mar/2002      lan_wai_rcv modified                                 [SP]  *
* 26/Mar/2002      Auto-Negotiation Control added                       [SP]  *
* 07/Apr/2002      Comments and Text guide added                        [SP]  *
* 08/Aug/2002      rcv_pkt fn modified & Rx/Tx buffer initialize        [SP]  *
* 15/Aug/2002      Configuration macros added                           [SP]  *
* 11/Dec/2002      Modified for Speed improvement                       [SP]  *
* 18/Dec/2002      chg_ims() updated                                    [SP]  *
* 27/Dec/2002 V4マクロの不要化、 esle を V4 と見なすよう順序を入れ替え     KA *
******************************************************************************/

#include <string.h>

#ifdef V2
 #include "norti.h"
 #define PSW         UW
#elif defined(V3)
 #include "norti3.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #define PSW         UINT
 #ifndef iwup_tsk
  #define iwup_tsk(i) wup_tsk(i)
 #endif
#else /* V4 */
 #ifndef V4
 #define V4     1
 #endif
 #include "kernel.h"
 #define wai_tsk(t)  tslp_tsk(t)
 #define PSW         UINT
#endif


#include "nonet.h"
#include "nonets.h"
#define INCLUDED_FROM_NONET
#include "nonetc.h"

#include "nonethw.h"
#include "AM79C973.h"

#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif

#ifndef BIG_ENDIAN
    #ifndef LITTLE_ENDIAN
        #define LITTLE_ENDIAN       /* Default LITTLE_ENDIAN memory system */
    #endif
#else
    #ifdef LITTLE_ENDIAN
        #error Multiple definations for memory byte order!!
    #endif
#endif

void lan_intr(void);            /* Lan interrupt handler routine */
static ER rcv_pkt(void);        /* Data Receive handler function */
static BOOL lan_chk_cfg(void);  /* Lan Driver valid Configuration check */

/* external functions declared here */
extern void pci_iowrite_long(UW, UW, UW);   /* PCI 32-bit IO write */
extern void pci_iowrite_word(UW, UH, UW);   /* PCI 16-bit IO write */
extern void pci_iowrite_byte(UW, UB, UW);   /* PCI 8-bit IO write */

extern void pci_ioread_long(UW, UW *, UW);  /* PCI 32-bit IO read */
extern void pci_ioread_word(UW, UH *, UW);  /* PCI 16-bit IO read */
extern void pci_ioread_byte(UW, UB *, UW);  /* PCI 8-bit IO read */
extern UW htopci(UW);
extern UW addr_cpu_to_pci(UW);
extern UW addr_pci_to_cpu(UW);

/****  BCR and CSR read/write low level functions *****************/
static void write_bcr(UW , UW );
static UW read_bcr(UW );
static void write_csr(UW , UW );
static UW read_csr(UW );
static void write_phy(UW , UW );
static UW read_phy(UW );

W lan_error_count;              /* error counter */
static UW rx_dscp_addr;         /* start address of RX descriptor ring */
static UW tx_dscp_addr;         /* start address of TX descriptor ring */
static UW rx_ringptr;           /* Receive ring pointer */
static UW tx_ringptr;           /* Transmit ring pointer */

static ID tx_wtskid;            /* receive fnction task ID */
static ID rx_wtskid;            /* Transmit function task ID */

/***** General LAN driver structure declarations ******************************/

#define ETHMAXLEN       1514        /* maximum length of packet (excluding CRC) */

#ifndef RX_RING_COUNT
    #define RX_RING_COUNT   0x0010      /* count of RCV descriptor */
#endif

#ifndef TX_RING_COUNT
    #define TX_RING_COUNT   0x0004      /* count of TRX descriptor */
#endif

#ifndef TX_SIZE
    #define TX_SIZE         0x0600      /* 1536 bytes each TX buffer */
#endif

#ifndef RX_SIZE
    #define RX_SIZE         0x0600      /* 1536 bytes each RX buffer */
#endif

#if (TX_SIZE <ETHMAXLEN)
    #error Transmitter Buffer size low.
#endif
#if (RX_SIZE <ETHMAXLEN)
    #error Receiver Buffer size low.
#endif

typedef struct LAN_CFG {
    BOOL    sel_autoneg;    /*  0 = Auto-Negotiation disabled
                                1 = Auto-Negotiation enabled */
    BOOL    sel_speed;      /*  0 = 10 Mbps speed selection
                                1 = 100 Mbps speed selection */
    BOOL    sel_duplex;     /*  0 = Half Duplex mode selection
                                1 = Full Duplex mode selection */
    BOOL    sel_loopback;   /*  0 = Normal operation , No loopback mode
                                1 = Internal loopback mode */
    BOOL    rcv_all_pkt;    /*  0 = Disable all packets reception.
                                1 = Receive packets with all physical address */
    BOOL    rcv_mcast_pkt;  /*  0 = Disable multicast packets reception.
                                1 = Receive packets with multicast addr match */

}Lan_Cfg;

typedef struct RX_STS {
    BOOL    rx_on;              /*  0 = Receiver controller disabled.
                                    1 = Receiver Controller enabled.  */
    BOOL    rx_int_flag;        /*  0 = Waiting for Rx interrupt.
                                    1 = Rx interrupt acknowledged.  */
    BOOL    rx_flag;            /*  0 = Waiting for packet reception.
                                    1 = Packet received.  */
    BOOL    rx_error_flag;      /*  0 = Data reception OK.
                                    1 = Data reception error.  */
    BOOL    rx_overflow_flag;   /*  0 = Rx controller no-overflow detected.
                                    1 = Receive ring overflow detected.  */
    BOOL    phy_addr_match;     /*  0 = No physical addr match found.
                                    1 = Pkt with physical addr match detected.  */
    BOOL    broadcast_match;    /*  0 = No broadcast addr match found.
                                    1 = Pkt with broadcast addr match detected.  */
}Rx_Sts;

typedef struct TX_STS {
    BOOL    tx_on;              /*  0 = Transmit controller disabled.
                                    1 = Transmit Controller enabled.  */
    BOOL    tx_int_flag;        /*  0 = Waiting for Tx interrupt.
                                    1 = Tx interrupt acknowledged.  */
    BOOL    tx_flag;            /*  0 = Last Packet Transmission over.
                                    1 = Packet transmission busy.  */
    BOOL    tx_error_flag;      /*  0 = Packet transmission OK.
                                    1 = Packet transmission error.  */
}Tx_Sts;

typedef struct LAN_DEVICE {
    Lan_Cfg     lan_cfg;        /* LAN driver configuration settings */
    Rx_Sts      rx_sts;         /* Receiver controller status */
    UW          rx_start_ptr;   /* Start address of received packet */
    UW          rx_stop_ptr;    /* Stop address of received packet */
    UW          rx_pkt_size;    /* Byte Count / Size of received packet */
    UW          rx_read_ptr;    /* Read pointer to current byte in Rx Data buffer */
    Tx_Sts      tx_sts;         /* Transmit controller status */
    UW          tx_start_ptr;   /* Packet start address in Tx Buffer */
    UW          tx_pkt_size;    /* Byte length / size of packet to be transmitted */
    UW          tx_write_ptr;   /* Position of current byte in Tx buffer */
    UW          lnk_sts_old;    /* LINK/Media status backup */
}Lan_Device;

Lan_Device    lanc_amd;

extern ID mplid_ncram;

static UW init_blk;
static UW rx_ring;
static UW tx_ring;
static UW rx_buffer;
static UW tx_buffer;

static BOOL lan_int_bsy;

/***** LAN controller dependent Structure Declarations *************************/

/* LAN controller initialization block structure */
typedef struct INIT_BLOCK {
    UW  mode_len;           /* mode with TX/RX length word */
    UW  phys_addr0;         /* Physical node address */
    UW  phys_addr1;
    UW  filter0;            /* logical address filter */
    UW  filter1;
    UW  rx_ring;            /* receive descriptor ring address */
    UW  tx_ring;            /* transmit descriptor ring address */
} Init_Block;
Init_Block  init_block;

#ifdef DEBUG
/************** Debug mode message display functions ************************/

char msg_lpbk[] = "\n\r>>Debug>> Internal Loopback mode selected..\r";
char msg_hdup[] = "\n\r>>Debug>> Half Duplex mode selected..\r";
char msg_fdup[] = "\n\r>>Debug>> Full Duplex mode selected..\r";
char msg_10M[] = "\n\r>>Debug>> Controller configured for 10Mbps media..\r";
char msg_100M[] = "\n\r>>Debug>> Controller configured for 100Mbps media..\r";
char msg_angon[] = "\n\r>>Debug>> Auto-Negotiation started, please wait...\r";
char msg_angoff[] = "\n\r>>Debug>> Auto-Negotiation over.\r";
char msg_lnkerr[] = "\n\r>>Debug>> Link connection error!! Please reconnect..\r";
char msg_angerr[] = "\n\r>>Debug>> Auto-Negotiation timeout error...\r";
char msg_init[] = "\n\r>>Debug>> Device Initialization over...\r";
char msg_rcvok[] = "\n\r>>Debug>> Packet received..";
char msg_rcverr[] = "\n\r>>Debug>> .....reception error..";
char msg_sndok[] = "\n\r>>Debug>> Packet transmitted..";
/* ---------------------------------------------------------------------
    Please configure and initialize serial UART controller (DBG_CH)
    in main application program. Please configure serial console
    for 80 line character display.
    DEBUG macro is used for testing purpose only. Please note than the
    LAN driver operating speed will be lower in DEBUG mode.
-----------------------------------------------------------------------*/

#ifndef DBG_CH      /* Define UART channel number for Debug message display */
#define DBG_CH  0
#endif

static void print_msg(char*);   /* To print text message in DEBUG mode */
static void print_rxsts(void);  /* To print new packet Status in DEBUG mode */
static void print_txsts(void);  /* To print transmit status in DEBUG mode */
static void print_H8(UB);       /* To print 8-bit hex digits in DEBUG mode */
static void print_H16(UH);      /* To print 16-bit hex digits in DEBUG mode */

#endif

/*****************************************************************************
*
* Rx Interrupt handler for the Ethernet controller.
*
******************************************************************************/
static void lan_rx_intr(void)
{
    static UW tmp;

    if ((tmp = read_csr(LAN_CSR0)) & (CSR0_RINT|CSR0_MISS)) {
        lanc_amd.rx_sts.rx_flag = TRUE;
                            /* set flag for Data Receive task */
        write_csr(LAN_CSR0, tmp & (~0x024f));
    }
}
/*****************************************************************************
*
* Tx Interrupt handler for the Ethernet controller.
*
******************************************************************************/
static void lan_tx_intr(void)
{
    static UW tmp;

    if ((tmp = read_csr(LAN_CSR0)) & CSR0_TINT) {
        lanc_amd.tx_sts.tx_flag = FALSE;
                            /* set flag for data transmit task */
        write_csr(LAN_CSR0, tmp & (~0x044f));
    }
}
/*****************************************************************************
*
* Interrupt handler for the Ethernet controller.
*
******************************************************************************/
void lan_intr(void)
{
    ID id;
    UW tmp;

    lan_int_bsy = TRUE;

    if (!(read_csr(LAN_CSR0) & CSR0_INTR)) {
        lan_int_bsy = FALSE;
        return;
    }

    lan_rx_intr();          /* Check for receiver interrupt */
    /* Polling mode Rx control is disabled if Rx interrupt arrives */
    if (lanc_amd.rx_sts.rx_flag) {
        lanc_amd.rx_sts.rx_int_flag = TRUE;
            /* Disable the Rx interrupt till Data Reception task is processed */
        tmp = read_csr(LAN_CSR3);
        if ((id = rx_wtskid) != FALSE) {
            iwup_tsk(id);
        }
        write_csr(LAN_CSR0, 0x7940);                    /* Enable interrupts */
    }

    lan_tx_intr();          /* Check for transmitter interrupt */
    /* Polling mode Tx control is disabled if Tx interrupt arrives */
    if (!lanc_amd.tx_sts.tx_flag) {
        lanc_amd.tx_sts.tx_int_flag = TRUE;
            /* Disable the Tx interrupt till Data Transmission task is processed */
        tmp = read_csr(LAN_CSR3);
        if ((id = tx_wtskid) != FALSE) {
            iwup_tsk(id);
        }
        write_csr(LAN_CSR0, 0x7940);                    /* Enable interrupts */
    }

    if ((tmp = read_csr(LAN_CSR7)) & CSR7_MAPINT) {
                                /* Interrupt after PHY MANAGEMENT status is changed */
        if ((id = rx_wtskid) != FALSE) {
            iwup_tsk(id);
        }
        if ((id = tx_wtskid) != FALSE) {
            iwup_tsk(id);
        }
        tmp = read_csr(LAN_CSR7);
        write_csr(LAN_CSR7, tmp | CSR7_MAPINT);
    }


    lan_int_bsy = FALSE;

}
/******************************************************************************
*   write to the Bus Control Register with the specified data
*
* Input -
*   bcr_addr    - Bus Control Register address.
*   bcr_data    - data to be written in Bus Control Register.
*******************************************************************************/
static void write_bcr(UW bcr_addr, UW bcr_data)
{
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    if (!(lan_int_bsy)) chg_ims(IP);
    pci_iowrite_long((LANC_IO_BASE + LAN_RAP), bcr_addr, 0);
    pci_iowrite_long((LANC_IO_BASE + LAN_BDP), bcr_data, 0);
    if (!(lan_int_bsy)) chg_ims(imask);
}
/******************************************************************************
*   read the Bus Control Register at the specified address
*
* Input -
*   bcr_addr    - Bus Control Register address.
* Output -       data read from Bus Control Register.
*******************************************************************************/
static UW read_bcr(UW bcr_addr)
{
    static UW data;
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif

    if (!(lan_int_bsy)) chg_ims(IP);
    pci_iowrite_long((LANC_IO_BASE + LAN_RAP), bcr_addr, 0);
    pci_ioread_long((LANC_IO_BASE + LAN_BDP), &data, 0);
    if (!(lan_int_bsy)) chg_ims(imask);
    return data;
}

/******************************************************************************
*   write to the Control/Status Register with the specified data
*
* Input -
*   csr_addr    - Control/Status Register address.
*   csr_data    - data to be written in Control/Status Register.
*******************************************************************************/
static void write_csr(UW csr_addr, UW csr_data)
{
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    if (!(lan_int_bsy)) chg_ims(IP);
    pci_iowrite_long((LANC_IO_BASE + LAN_RAP), csr_addr, 0);
    pci_iowrite_long((LANC_IO_BASE + LAN_RDP), csr_data, 0);
    if (!(lan_int_bsy)) chg_ims(imask);
}
/******************************************************************************
*   read the Control/Status Register at the specified address
*
* Input -
*   csr_addr    - Control/Status Register address.
* Output -       data read from Control/Status Register.
*******************************************************************************/
static UW read_csr(UW csr_addr)
{
    static UW data;
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif

    if (!(lan_int_bsy)) chg_ims(IP);
    pci_iowrite_long((LANC_IO_BASE + LAN_RAP), csr_addr, 0);
    pci_ioread_long((LANC_IO_BASE + LAN_RDP), &data, 0);
    if (!(lan_int_bsy)) chg_ims(imask);
    return data;

}
/******************************************************************************
*   write to the PHY Register with the specified data
*
* Input -
*   phy_addr    - PHY Register address.
*   phy_data    - data to be written in PHY Register.
*******************************************************************************/
static void write_phy(UW phy_addr, UW phy_data)
{
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif
    if (!(lan_int_bsy)) chg_ims(IP);
    write_bcr(LAN_BCR33, phy_addr);     /* write the PHY reg address */
    write_bcr(LAN_BCR34, phy_data);     /* latch phy_data to the data PORT */
    if (!(lan_int_bsy)) chg_ims(imask);
}

/******************************************************************************
*   read the PHY Register at the specified address
*
* Input -
*   phy_addr    - PHY Register address.
* Output -       data read from PHY Register.
*******************************************************************************/
static UW read_phy(UW phy_addr)
{
    static UW data;
    UINT imask;
#ifdef V4
    get_ims(&imask);
#else
    ref_ims(&imask);
#endif

    if (!(lan_int_bsy)) chg_ims(IP);
    write_bcr(LAN_BCR33, phy_addr);     /* write the PHY reg address */
    data = read_bcr(LAN_BCR34);         /* read data from the data PORT */
    if (!(lan_int_bsy)) chg_ims(imask);
    return data;

}
/******************************************************************************
*   Update LAN controller initialization table.
*
*******************************************************************************/
static void lan_init_tbl(void)
{
    static UW tmp, rx_dscp_no, tx_dscp_no;

    rx_dscp_no = 0;
    tmp = RX_RING_COUNT;
    while ((tmp = tmp/2) != 0) rx_dscp_no++;

    tx_dscp_no = 0;
    tmp = TX_RING_COUNT;
    while ((tmp = tmp/2) != 0) tx_dscp_no++;

    tmp = MODE_PORTSEL;
    if (lanc_amd.lan_cfg.rcv_all_pkt){
        tmp |= EN_RCV_ALL;
    }
    write_csr(LAN_CSR15, tmp);  /* set operating mode */
    tmp |= (rx_dscp_no << 20);
    tmp = tmp | (tx_dscp_no << 28);
    init_block.mode_len = htopci(tmp);
    init_block.filter0 = 0xffffffff;
    init_block.filter1 = 0xffffffff;

    tmp = ethernet_addr[0];
    tmp |= (ethernet_addr[1] << 8);
    tmp |= (ethernet_addr[2] << 16);
    tmp |= (ethernet_addr[3] << 24);
    init_block.phys_addr0 = htopci(tmp);

    tmp = ethernet_addr[4];
    tmp |= (ethernet_addr[5] << 8);
    init_block.phys_addr1 = htopci(tmp);

    tmp = addr_cpu_to_pci(rx_dscp_addr);
    init_block.rx_ring = htopci(tmp);
    tmp = addr_cpu_to_pci(tx_dscp_addr);
    init_block.tx_ring = htopci(tmp);

}
/******************************************************************************
*   Update Receiver Descriptor Ring Entry.
*
*******************************************************************************/
static void lan_rx_tbl(void)
{
    static UW tmp, temp;

    for (tmp =0; tmp < RX_RING_COUNT; tmp++) {
        *((UW*)(rx_dscp_addr + (tmp*16) + RMD0)) = 0x00;
        temp = addr_cpu_to_pci((UW)(rx_buffer+(tmp*RX_SIZE)));
        *((UW*)(rx_dscp_addr + (tmp*16) + RMD2)) = htopci(temp);
        *((UW*)(rx_dscp_addr + (tmp*16) + RMD3)) = 0x00;

        temp = RX_SIZE;
        temp = (-temp) & 0xffff;    /* Get 2's complement form for count */
        if (tmp < (RX_RING_COUNT-1)) {
            temp |= RMD_OWN|RMD_STP|RMD_ENP;
        }
        *((UW*)(rx_dscp_addr + (tmp*16) + RMD1)) = htopci(temp);
    }
}
/******************************************************************************
*   Update Transmitter Descriptor Ring Entry.
*
*******************************************************************************/
static void lan_tx_tbl(void)
{
    static UW tmp, temp;

    for (tmp =0; tmp < TX_RING_COUNT; tmp++) {
        *((UW*)(tx_dscp_addr + (tmp*16) + TMD0)) = 0x00;
        temp = addr_cpu_to_pci((UW)(tx_buffer+(tmp*TX_SIZE)));
        *((UW*)(tx_dscp_addr + (tmp*16) + TMD2)) = htopci(temp);
        *((UW*)(tx_dscp_addr + (tmp*16) + TMD3)) = 0x00;

        *((UW*)(tx_dscp_addr + (tmp*16) + TMD1)) = 0x00;
    }
}
/******************************************************************************
*   Re-initialize The LAN Controller..
*
*******************************************************************************/
static lan_reini(void)
{
    static UW tmp;

    tmp = read_csr(LAN_CSR0);       /* Stop the controller operation */
    write_csr(LAN_CSR0, CSR0_STOP);

    lan_rx_tbl();                   /* Clear the descriptor data */
    lan_tx_tbl();

    /* start transmitter and receiver operation */
    tmp = read_csr(LAN_CSR15);
    tmp &= (~(DIS_TRX | DIS_RCV | DIS_CRC));
    write_csr(LAN_CSR15, tmp & 0xffff);

    tmp = read_csr(LAN_CSR0);
    write_csr(LAN_CSR0, CSR0_START | CSR0_IENA | CSR0_RXON | CSR0_TXON);
    tmp = read_csr(LAN_CSR0);

    tmp = read_csr(LAN_CSR4);   /* Clear Transmit Start status */
    write_csr(LAN_CSR4, tmp | CSR4_TXSTRT);

    /* set the interrupt mask */
    tmp = CSR3_MISSM|CSR3_MERRM|CSR3_DXSUFLO|CSR3_IDONM|CSR3_RINTM|CSR3_TINTM;
    write_csr(LAN_CSR3, tmp);

    lan_error_count = 0;
    lanc_amd.lnk_sts_old = 0;
    rx_ringptr = 0;             /* initialize receive ring pointer */
    tx_ringptr = 0;             /* initialize transmit ring pointer */
    lanc_amd.rx_sts.rx_overflow_flag = FALSE;    /* reset overflow status */
    lanc_amd.rx_sts.rx_flag = FALSE;
    lanc_amd.tx_sts.tx_flag = FALSE;
    lanc_amd.rx_sts.rx_on = TRUE;
    lanc_amd.tx_sts.tx_on = TRUE;

}

/*****************************************************************************
* Routine to upload LAN driver configuration settings.
*
******************************************************************************/
static void lan_get_cfg (void)
{
/*  Functional description ----------------------------------------------------
    This functional block is executed during LAN controller device initialization.
    Device control mode is selected as per the user settings. Device can be
    configured by using pre-processor definitions as described below.
    ==========================================================================
        Definition                  Device Configuration
    ==========================================================================
        AUTO_NEG            LAN controller set to Auto-Negotiation mode
        SPEED_100M          Media set to 100Mbps speed
        FULL_DUPLEX         Full Duplex mode selection
        LOOPBACK            Device configured to Internal loopback mode
        RCV_ALL             Configure driver to accept all packets
        RCV_MCAST           Configure driver to accept Multicast packets
    ==========================================================================
    Pre-definitions other than above values are neglected. In case of no
    pre-definition, the default setting is for 10Mbps/Half-Duplex/Normal mode.
    Please note that when auto-Negotiation mode is selected, SPEED_100M and
    FULL_DUPLEX are ignored. Media speed and duplex mode are selected as per the
    autonegotiation result.
    In case of LOOPBACK mode Auto-negiation setting is ignored.
    ---------------------------------------------------------------------------*/

    memset(&lanc_amd.lan_cfg, 0, sizeof(lanc_amd.lan_cfg));

#ifdef AUTO_NEG                 /* Auto-Negotiation mode selection */
    lanc_amd.lan_cfg.sel_autoneg = TRUE;
#endif
#ifdef SPEED_100M               /* 100 Mbps LAN speed selectoin */
    lanc_amd.lan_cfg.sel_speed = TRUE;
#endif
#ifdef FULL_DUPLEX              /* Full Duplex mode selection */
    lanc_amd.lan_cfg.sel_duplex = TRUE;
#endif
#ifdef LOOPBACK                 /* Internal loopback mode selection */
    lanc_amd.lan_cfg.sel_loopback = TRUE;
#endif
#ifdef RCV_ALL                  /* Enable Receive all packets at node */
    lanc_amd.lan_cfg.rcv_all_pkt = TRUE;
#endif
#ifdef RCV_MCAST                /* Enable Multicast packet reception */
    lanc_amd.lan_cfg.rcv_mcast_pkt = TRUE;
#endif
}
/******************************************************************************
*   Initialize the Ethernet controller
*
* Output -
*   id_addr     - ethernet address (6-bytes) from the device.
*******************************************************************************/
ER lan_ini(UB *id_addr)
{
    ER ercd;
    UH mask;
    UW mode, tmp, temp;
    UW rx_dscp_no, tx_dscp_no;

    lan_int_bsy = FALSE;

    /* Upload and Check for the proper configuration settings */
    lan_get_cfg();
    if (lan_chk_cfg() == FALSE) {
        return E_DRIVER_FAIL;
    }

    ercd = get_mpl(mplid_ncram, sizeof(Init_Block), &init_blk);
    if (ercd != 0) {
        return ercd;
    }

    ercd = get_mpl(mplid_ncram, 4*(RX_RING_COUNT*4 + 3), &rx_ring);
    if (ercd != 0) {
        return ercd;
    }

    ercd = get_mpl(mplid_ncram, 4*(TX_RING_COUNT*4 + 3), &tx_ring);
    if (ercd != 0) {
        return ercd;
    }

    ercd = get_mpl(mplid_ncram, RX_RING_COUNT*RX_SIZE, &rx_buffer);
    if (ercd != 0) {
        return ercd;
    }

    ercd = get_mpl(mplid_ncram, TX_RING_COUNT*TX_SIZE, &tx_buffer);
    if (ercd != 0) {
        return ercd;
    }


    /* set the LAN controller in 32-bit access mode after reset */
    write_csr(LAN_CSR0, 0);

    /* Initialize the local variables */
    tx_wtskid = rx_wtskid = FALSE;
    memset(&lanc_amd.rx_sts, 0, sizeof(lanc_amd.rx_sts));
                        /* initialize Rx status bits */
    memset(&lanc_amd.tx_sts, 0, sizeof(lanc_amd.tx_sts));
                        /* initialize Tx status bits */
    memset(&init_block, 0, sizeof(init_block));
    memset((UB*)rx_ring, 0, 4*(RX_RING_COUNT*4 + 3));
    memset((UB*)tx_ring, 0, 4*(TX_RING_COUNT*4 + 3));

    /* Clear Receiver and Transmitter data buffer */
    memset((UB*)rx_buffer, 0, RX_RING_COUNT*RX_SIZE);
    memset((UB*)tx_buffer, 0, TX_RING_COUNT*TX_SIZE);

    /* set software style for 32-bit data access */
    tmp = read_bcr(LAN_BCR20);
    write_bcr(LAN_BCR20, (tmp & ~0x0f) | SOFTWARE_STYLE);
    tmp = read_bcr(LAN_BCR20);

    write_csr(LAN_CSR0, CSR0_STOP);
                    /* stop the controller till initiaization is over */

        /* update receive descriptor table address on 16 byte boundary */
    rx_dscp_addr = rx_ring;
    for (tmp=0; tmp<4; tmp++) {
        if (!((UW)(rx_ring+tmp*4) & 0x0f)) {
            rx_dscp_addr = (UW)(rx_ring+tmp*4);
            break;
        }
    }

        /* update transmit descriptor table address on 16-byte boundary */
    tx_dscp_addr = tx_ring;
    for (tmp=0; tmp<4; tmp++) {
        if (!((UW)(tx_ring+tmp*4) & 0x0f)) {
            tx_dscp_addr = (UW)(tx_ring+tmp*4);
            break;
        }
    }

    lan_init_tbl();    /* Prepare the LAN Controller Initialization Table */

    memcpy((UB*)init_blk, &init_block, sizeof(Init_Block));

    lan_rx_tbl();      /* Initialize Rx Descriptor table */
    lan_tx_tbl();      /* Initialize Tx Descriptor table */

        /* set the PHY registers */


    write_bcr(LAN_BCR32, read_bcr(LAN_BCR32) | BCR32_XPHYRST);
    for (tmp=0; tmp < 400; tmp++);
    write_bcr(LAN_BCR32, read_bcr(LAN_BCR32) & ~BCR32_XPHYRST);

    tmp = read_phy(LAN_ANR1);
    tmp = read_phy(LAN_ANR1);       /* Read multiple time as ANR1 reg is latch type */

    if (!(tmp & ANR1_ANGABLE)) {    /* Check if Auto_Negotiation mode is supported */
                                    /* Disable Auto-Negotiation if not supported */
        lanc_amd.lan_cfg.sel_autoneg = FALSE;
    }

    if (lanc_amd.lan_cfg.sel_autoneg) {      /* Setting for Auto-Negotiation */
        /*  Auto-Negotiation mode is Enabled.
            Auto Setup for Auto-Negotiation mode is enabled.
            Auto-polling is selected to check for Auto-Negotiation */

        tmp = read_bcr(LAN_BCR32);
        tmp &= ~(BCR32_DANAS | BCR32_XPHYFD | BCR32_XPHYSP);
        tmp |= (BCR32_APEP | BCR32_XPHYANE);
        write_bcr(LAN_BCR32, tmp);

        write_bcr(LAN_BCR9, 0x0);

        tmp = read_phy(LAN_ANR0);
        tmp |= ANR0_PHYANE;         /* Enable Auto-Negotiation */
        write_phy(LAN_ANR0, tmp);
        tmp |= ANR0_RSTANG;
        write_phy(LAN_ANR0, tmp);   /* Restart Auto-Negotiation */

#ifdef DEBUG
        print_msg(msg_angon);
#endif
        while (read_phy(LAN_ANR0) & ANR0_RSTANG);
                                    /* Wait till Auto-Negotiation is complete */
        while (!(read_phy(LAN_ANR1) & ANR1_ANGEND));
                                    /* Confirm End of Auto-Negotiation */
#ifdef DEBUG
        print_msg(msg_angoff);
#endif
        write_phy(LAN_ANR16, 0x3F00);   /* Enable PHY interrupts */
        tmp = read_csr(LAN_CSR7);
        write_csr(LAN_CSR7, tmp | CSR7_MAPINTE|CSR7_FSPND);
                            /* Enable PHY change detect int */

    }
    else {

        /*  Auto-Negotiation mode is Disabled.
            Auto Setup for Auto-Negotiation mode is Disabled.
            Auto-polling is Disabled */
        tmp = read_bcr(LAN_BCR32);
        tmp |= BCR32_DANAS | BCR32_APEP;
        tmp &= ~(BCR32_XPHYANE);
        write_bcr(LAN_BCR32, tmp);
        tmp &= ~(BCR32_XPHYSP | BCR32_XPHYFD);
        write_bcr(LAN_BCR32, tmp);

        tmp = read_phy(LAN_ANR1);
        tmp = read_phy(LAN_ANR1);

        temp = read_phy(LAN_ANR0);
        temp &= ~(ANR0_PHYANE | ANR0_RSTANG);
        temp &= ~(ANR0_PHYSP | ANR0_PHYFD | ANR0_LPBACK);

        if (lanc_amd.lan_cfg.sel_duplex) {       /* If Full duplex mode selected */
            temp |= ANR0_PHYFD;
            write_bcr(LAN_BCR9, 0x01);
            tmp = read_bcr(LAN_BCR32);
            write_bcr(LAN_BCR32, tmp | BCR32_XPHYFD);
        }
        if (lanc_amd.lan_cfg.sel_speed){         /* If 100Mbps speed selected */
            temp |= ANR0_PHYSP;
            tmp = read_bcr(LAN_BCR32);
            write_bcr(LAN_BCR32, tmp | BCR32_XPHYSP);
        }
        write_phy(LAN_ANR0, temp);
        tmp = read_phy(LAN_ANR1);
        tmp = read_phy(LAN_ANR1);
        temp = read_phy(LAN_ANR0);
    }

    lanc_amd.lnk_sts_old = read_phy(LAN_ANR24) & 0x0f;  /* Get new status */
#ifdef DEBUG
    if(lanc_amd.lnk_sts_old & ANR24_DUPLEX)   /* Display current duplex mode */
        print_msg(msg_fdup);
    else
        print_msg(msg_hdup);
    if(lanc_amd.lnk_sts_old & ANR24_SPEED)    /* Display current media speed */
        print_msg(msg_100M);
    else
        print_msg(msg_10M);
#endif

    if (lanc_amd.lan_cfg.sel_loopback) {     /* If loopback mode selected */
        tmp = read_phy(LAN_ANR0);
        write_phy(LAN_ANR0, tmp | ANR0_LPBACK);
    }

    /* set the interrupt mask */
    mask = CSR3_MISSM|CSR3_MERRM|CSR3_DXSUFLO|CSR3_IDONM|CSR3_RINTM|CSR3_TINTM|CSR3_EMBA|CSR3_LAPPEN;
    write_csr(LAN_CSR3, mask);

    /* set PHYSELEN and clear INTLEVEL in BCR2 reg */
    write_bcr(LAN_BCR2, 0x2000);
    tmp = read_bcr(LAN_BCR2);

    write_bcr(LAN_BCR22, 0x1818);       /* update PCI latency period */

    tmp = read_bcr(LAN_BCR25);
    write_bcr(LAN_BCR25, 0x14);         /* 2K for Transmit buffer */
                                        /* 8K for receive buffer */
    tmp = read_bcr(LAN_BCR26);
    write_bcr(LAN_BCR26, 0x04);

    tmp = read_bcr(LAN_BCR27);
    write_bcr(LAN_BCR27, 0x4001);
    tmp = read_bcr(LAN_BCR27);

    tmp = read_bcr(LAN_BCR18);
    write_bcr(LAN_BCR18, tmp | 0x860);  /* Enable PCI Burst Rd/Wr control */
    tmp = read_bcr(LAN_BCR18);

    write_csr(LAN_CSR4, CSR4_RCVCCOM|CSR4_TXSTRTM|CSR4_ASTRP_RCV|CSR4_APAD_XMT|CSR4_MFCOM);

    tmp = CSR5_LTINTEN;                 /* enable transmit end interrupt */
    write_csr(LAN_CSR5, tmp);

    tmp = read_csr(LAN_CSR7);

    tmp = CSR80_XMTSP | CSR80_XMTFW | CSR80_RCVFW;
    write_csr(LAN_CSR80, tmp);
    tmp = read_csr(LAN_CSR80);          /* update DMA transfer control register */

    tmp = addr_cpu_to_pci((UW)(init_blk));
    write_csr(LAN_CSR1, tmp & 0xffff);
    write_csr(LAN_CSR2, tmp >> 16);

    tmp = read_csr(LAN_CSR0);
    write_csr(LAN_CSR0, CSR0_STOP);

    /* start transmitter and receiver operation */
    tmp = read_csr(LAN_CSR15);
    tmp &= (~(DIS_TRX | DIS_RCV | DIS_CRC));
    write_csr(LAN_CSR15, tmp & 0xffff);

    tmp = read_csr(LAN_CSR0);
    write_csr(LAN_CSR0, CSR0_INIT );

    tmp = 0;
    while (tmp++ < 400) {
        if (read_csr(LAN_CSR0) & CSR0_IDON)
            break;
    }

    tmp = read_csr(LAN_CSR0);
    if (tmp & CSR0_IDON) {
        write_csr(LAN_CSR0, tmp);
    }
    write_csr(LAN_CSR0, CSR0_START | CSR0_IENA | CSR0_RXON | CSR0_TXON);
    write_csr(LAN_CSR3, mask);

    tmp = read_csr(LAN_CSR4);   /* Clear Transmit Start status */
    write_csr(LAN_CSR4, tmp | CSR4_TXSTRT);

    lan_error_count = 0;
    lanc_amd.lnk_sts_old = 0;
    rx_ringptr = 0;             /* initialize receive ring pointer */
    tx_ringptr = 0;             /* initialize transmit ring pointer */
    lanc_amd.rx_sts.rx_overflow_flag = FALSE;    /* reset overflow status */
    lanc_amd.rx_sts.rx_flag = FALSE;
    lanc_amd.tx_sts.tx_flag = FALSE;
    lanc_amd.rx_sts.rx_int_flag = FALSE;
    lanc_amd.tx_sts.tx_int_flag = FALSE;
    lanc_amd.rx_sts.rx_on = TRUE;
    lanc_amd.tx_sts.tx_on = TRUE;

#ifndef MS7751
    ercd = lan_def_int();   /* Configure Interrupt handler for Ethernet Port */
#endif

#ifdef DEBUG
    print_msg(msg_init);    /* "Initialization over" display in debug mode */
#endif

    return E_OK;
}

/******************************************************************************
*      Routine to check the valid LAN driver configuration
*
******************************************************************************/
static BOOL lan_chk_cfg(void)
{
    if (((RX_RING_COUNT * RX_SIZE) < ETHMAXLEN)
        || ((TX_RING_COUNT * TX_SIZE) < ETHMAXLEN)) {
        return FALSE;
    }

    return TRUE;
}
/******************************************************************************
*      Collect newly received packet information
*
******************************************************************************/
static ER rcv_pkt(void)
{
    static UW tmp;
    static UW wai_tmr;
    static ER ercd;
    static UW rx_lastptr;

    ercd = E_OK;
    lanc_amd.rx_sts.rx_error_flag = FALSE;
    if((tmp = htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD1)))) & RMD_OWN) {
        return E_RXERR;
                     /* Error... Rx-Descriptor is not released by LAN Controller */
    }

    if (tmp & RMD_ERR) {
        lan_error_count++;
        lanc_amd.rx_sts.rx_error_flag = TRUE;

        if (tmp&(RMD_CRC|RMD_OFLO|RMD_FRAM))
            ercd = E_RXERR;
    }

    tmp = htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD0)));

    lanc_amd.rx_pkt_size = (tmp & 0x0fff)- 0x04;    /* get the message length */
    *((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD0)) = 0x00;

    tmp = htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD2)));
    tmp = addr_pci_to_cpu(tmp);
    lanc_amd.rx_start_ptr = tmp;
    lanc_amd.rx_read_ptr =0;

    if (rx_ringptr == 0)
        rx_lastptr = (RX_RING_COUNT-1);
    else
        rx_lastptr = rx_ringptr-1;

    if(!(htopci(*((UW*)(rx_dscp_addr + (rx_lastptr * 16) + RMD1))) & RMD_OWN)) {
        *((UW*)(rx_dscp_addr + (rx_lastptr * 16) + RMD0)) = 0x00;
        tmp = (-RX_SIZE) & 0xffff;
        tmp |= RMD_OWN|RMD_STP|RMD_ENP;
        *((UW*)(rx_dscp_addr + (rx_lastptr * 16) + RMD1)) = htopci(tmp);
    }

    rx_ringptr++;
    if (rx_ringptr >= RX_RING_COUNT) {
        rx_ringptr = 0;
    }

    lanc_amd.rx_sts.rx_flag = FALSE;

#ifdef DEBUG
    print_rxsts();
#endif

    return ercd;
}
/******************************************************************************
*   Check for Auto-Negotiation status.
*
*******************************************************************************/
static BOOL lan_chk_lnk(void)
{
    static UW tmp;

    if (!(read_phy(LAN_ANR24) & ANR24_LNKSTS)) {
#ifdef DEBUG
        print_msg(msg_lnkerr);          /* Link error display in debug mode */
#endif
        while (!(read_phy(LAN_ANR24) & ANR24_LNKSTS)) {
            dly_tsk(10);
        }
    }

    tmp = read_phy(LAN_ANR24) & 0x0f;
    if ((tmp != lanc_amd.lnk_sts_old) && (lanc_amd.lan_cfg.sel_autoneg)) {
                        /* Wait till Auto-Negotiation is completed */
        tmp = read_phy(LAN_ANR1);
        while(!((tmp = read_phy(LAN_ANR1)) & ANR1_ANGEND));

        lanc_amd.lnk_sts_old = read_phy(LAN_ANR24) & 0x0f;  /* Get new status */
#ifdef DEBUG
        print_msg(msg_angoff);
#endif
#ifdef DEBUG
        if(lanc_amd.lnk_sts_old & ANR24_DUPLEX)   /* Display current duplex mode */
            print_msg(msg_fdup);
        else
            print_msg(msg_hdup);
        if(lanc_amd.lnk_sts_old & ANR24_SPEED)    /* Display current media speed */
            print_msg(msg_100M);
        else
            print_msg(msg_10M);
#endif
        return TRUE;
    }
    return FALSE;
}
/******************************************************************************
*   Wait for a reception interrupt by the Ethernet controller
*
* Input -
*   tmout   - timeout data to hold the send-task waiting.
*******************************************************************************/
ER lan_wai_rcv(TMO tmout)
{
    ER ercd;
    UW tmp;
    TMO wai_tmr;


    /* check for the link present. If ethernet link is down then prompt error */
    rx_wtskid = FALSE;

    if (lan_chk_lnk()) {
        return E_LINK;
    }

    if((htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD0))))!= 0x0) {
        ercd = rcv_pkt();
        return ercd;
    }

    /* start transmitter and receiver operation */

    tmp = read_csr(LAN_CSR3);
    write_csr(LAN_CSR3, tmp & (~(CSR3_RINTM|CSR3_MISSM)));

    if (!lanc_amd.rx_sts.rx_int_flag) {  /* For polling mode LAN driver */
        for (;;) {
            if((htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD0))))!= 0x0)
                break;
            lan_rx_intr();
            if (lanc_amd.rx_sts.rx_flag)
                break;
            wai_tmr = tmout;
            if ((wai_tmr != TMO_FEVR) && (wai_tmr-- == 0))
                return E_TMOUT;
            dly_tsk(2);
        }
        ercd = rcv_pkt();
        return ercd;
    }

    for(;;) {
        lan_rx_intr();
        if(((htopci(*((UW*)(rx_dscp_addr + (rx_ringptr * 16) + RMD0))))!= 0x0)
            || (lanc_amd.rx_sts.rx_flag)) {
                    /* For interrupt mode Data Receive control */
            rx_wtskid = FALSE;
            ercd = rcv_pkt();
            return ercd;
        }
        rx_wtskid = vget_tid();         /* Collect Receive Task ID */
                                        /* Enable Rx interrupt for next data Reception */
        ercd = wai_tsk(tmout);
        rx_wtskid = FALSE;
        if (ercd == E_OK) {
            ercd = rcv_pkt();
            return ercd;
        }
        return ercd;
    }
 }

/******************************************************************************
*   Wait for a transmission interrupt by the Ethernet controller
*
* Input -
*   tmout   - timeout data to hold the send-task waiting.
*******************************************************************************/
/* tx_flag is set if Transmit interrupt has occured */

ER lan_wai_snd(TMO tmout)
{
    ER ercd;
    UW tmp;
    TMO wai_tmr;

    /* check for the link present. If ethernet link is down then prompt error */
    if (lan_chk_lnk()) {
        return E_LINK;
    }

    tmp = read_csr(LAN_CSR3);
    write_csr(LAN_CSR3, tmp & (~CSR3_TINTM));

    if(!((htopci(*((UW*)(tx_dscp_addr + (tx_ringptr * 16) + TMD1))))&TMD_OWN)) {
        return E_OK;        /* Continue if next transmitter ring is free */
    }

    if (!lanc_amd.tx_sts.tx_int_flag) {  /* polling mode  Tx control */
        for (;;) {
            lan_tx_intr();  /* get current transmission status */
            if (!(lanc_amd.tx_sts.tx_flag)) {
                while ((htopci(*((UW*)(tx_dscp_addr + (tx_ringptr * 16) + TMD1))))
                        & TMD_OWN) {
                    dly_tsk(1);
                                /* Wait till Tx Descriptor is released by LANC */
                }
                return E_OK;
            }
            dly_tsk(2);
            wai_tmr = tmout;
            if ((wai_tmr != TMO_FEVR) && (wai_tmr-- == 0))
                return E_TMOUT;
        }
    }

    for (;;) {
        lan_tx_intr();
        if (!lanc_amd.tx_sts.tx_flag) {  /* check if transmission in progress */
            tx_wtskid = FALSE;
            while ((htopci(*((UW*)(tx_dscp_addr + (tx_ringptr * 16) + TMD1))))
                    & TMD_OWN) {
                dly_tsk(1);
                                /* Wait till Tx Descriptor is released by LANC */
            }
            return E_OK;
        }

        tx_wtskid = vget_tid();

        ercd = wai_tsk(tmout);
        tx_wtskid = FALSE;
        if (lan_chk_lnk()) {
            return E_LINK;
        }
        return ercd;
    }
}

/******************************************************************************
*   Inform the byte size of the packet to be read
*
* Output -
*   len     - length of data in received packet.
*******************************************************************************/
ER lan_get_len(UH *len)
{
    if (lanc_amd.rx_pkt_size > ETHMAXLEN) {
        lan_reini();
        *len = 0;
        return E_RXERR;
    }

    *len = (UH)(lanc_amd.rx_pkt_size);
    return E_OK;
}

/******************************************************************************
* Read out packet data from the controller in specified size.
*
* buf       buffer to store packet data
* len       number of bytes to be read
*******************************************************************************/
ER lan_get_pkt(VP buf, int len)
{
    UH *p;
    UH *s;

    p = (UH *)buf;
    s = (UH *)(lanc_amd.rx_start_ptr + lanc_amd.rx_read_ptr);
    lanc_amd.rx_read_ptr += ((UW)(len) & 0xffff);

    while(len > 1) {
        *p++ = *s++;
        len -= 2;
    }
    if (len == 1) {
        *(UB *)p = *(UB *)s;
    }

    return E_OK;
}

/******************************************************************************
*   Finish reading packet data
*******************************************************************************/
ER lan_get_end(void)
{
        /* Nothing to Do */
    return E_OK;
}

/******************************************************************************
*   Skip to read packet data in specific size
*
* Input -
*   len     - length of the data to be skipped.
*******************************************************************************/
ER lan_skp_pkt(int len)
{
    lanc_amd.rx_read_ptr += (UW)(len) & 0xffff;

    if (lanc_amd.rx_read_ptr >= lanc_amd.rx_pkt_size) {
        lan_get_end();
    }

    return E_OK;
}

/******************************************************************************
*   Inform the byte size of the packet data to write
*
* Input -
*   len     - length of the data to be transmitted.
*******************************************************************************/
ER lan_set_len(int len)
{
    UW tmp;

    lanc_amd.tx_pkt_size = (UW)len;
    lanc_amd.tx_write_ptr = 0;

    tmp = htopci(*((UW*)(tx_dscp_addr + (tx_ringptr * 16) + TMD2)));

    lanc_amd.tx_start_ptr = addr_pci_to_cpu(tmp);    /* Get the Transmit buffer base address */

    return E_OK;
}

/******************************************************************************
*   Write packet data into controller buffer in specified size
*
* Input -
*   len     - number of bytes to write.
*   data    - start address of buffer containing packet data.
*******************************************************************************/
ER lan_put_pkt(const VP data, int len)
{
    UH *p;
    UH *s;

    p = (UH *)data;
    s = (UH *)(lanc_amd.tx_start_ptr + lanc_amd.tx_write_ptr);
    lanc_amd.tx_write_ptr += (UW)(len) & 0xffff;
    while(len > 1) {
        *s++ = *p++;
        len -= 2;
    }
    if (len == 1) {
        *(UB *)s = *(UB *)p;
    }

    if (lanc_amd.tx_write_ptr >= TX_SIZE) {
        lanc_amd.tx_write_ptr -= TX_SIZE;
    }
    return E_OK;
}

/******************************************************************************
*   Write null bytes into controller buffer in specified size.
*   (Called if a packet sending is shorter than 60 bytes.)
*
* Input -
*   len     - number of null bytes to write (> 0)
*******************************************************************************/
ER lan_put_dmy(int len)
{
    UW temp;
    /* Padding is done by controller automatically */
    if (lanc_amd.tx_pkt_size > len)
        lanc_amd.tx_pkt_size = lanc_amd.tx_pkt_size - len;

    return E_OK;
}

/******************************************************************************
*   Finish writing packet data.
*   (Start a packet transmit operation actually.)
*
*******************************************************************************/
ER lan_put_end(void)
{
    UW tmp;
    ER ercd;

    *((UW*)(tx_dscp_addr + (tx_ringptr *16) + TMD0)) = 0x0;

    lanc_amd.tx_sts.tx_flag = TRUE;      /* ready for transmission */
    tx_wtskid = FALSE;                  /* Collect Transmission Task ID */

    /* check for length less than TX_SIZE If not then divide across two descriptors */
    tmp = (-lanc_amd.tx_pkt_size) & 0xffff;    /* Get 2's complement form for count */
    tmp |= TMD_OWN | TMD_ADDFCS;
    if (lanc_amd.tx_pkt_size <= TX_SIZE) {
        tmp |= TMD_STP | TMD_ENP | TMD_LTINT; /* Indicate start of buffer */
    }
    *((UW*)(tx_dscp_addr + (tx_ringptr *16) + TMD1)) = htopci(tmp);

    tmp = read_csr(LAN_CSR5);

    /* Start the actual transmission */
    write_csr(LAN_CSR0, 0x0048);

#ifdef DEBUG
    print_txsts();          /* Debug mode transmission status display */
#endif

    tx_ringptr++;           /* Update Tx ring pointer for next transmission */
    if (tx_ringptr >= TX_RING_COUNT) {
        tx_ringptr = 0;
    }
    return E_OK;
}

#ifdef DEBUG
/*****************************************************************************
* Debug mode message display function.
* Message is send to serial console defined by DBG_CH channel number.
*
******************************************************************************/
static void print_msg(char* c)
{
    while (*c!=0) {
        put_sio(DBG_CH, (UB)(*c), 40/MSEC);
        c++;
    }
}
/*****************************************************************************
* Debug mode Receiver status display function.
* Message is send to serial console defined by DBG_CH channel number.
*
******************************************************************************/
static void print_rxsts(void)
{
    print_msg(msg_rcvok);
    print_msg("Start Page = ");
    print_H8(rx_ringptr);

    if(lanc_amd.rx_sts.rx_error_flag==TRUE) {
        print_msg(msg_rcverr);
    }
    else {
        print_msg("; Count = ");
        print_H16((UH)lanc_amd.rx_pkt_size);
    }
    print_msg("\r");

}
/*****************************************************************************
* Debug mode Transmitter status display function.
* Message is send to serial console defined by DBG_CH channel number.
*
******************************************************************************/
static void print_txsts(void)
{
    print_msg(msg_sndok);
    print_msg("From Page = ");
    print_H8(tx_ringptr);
    print_msg("\r");
}
/*****************************************************************************
* Routine to display 8-bit hex number on Debug screen.
* 8-bit data will be printed as 2 hex digits
*
******************************************************************************/
static void print_H8(UB value)
{
    UB tmp;

    tmp = (value & 0xf0)>>4;
    if((tmp>=0)&&(tmp<=9))
        put_sio(DBG_CH, (UB)('0'+tmp), 10/MSEC);
    else
        put_sio(DBG_CH, (UB)('A'+tmp-0x0A), 10/MSEC);
    tmp = (value & 0x0f);
    if((tmp>=0)&&(tmp<=9))
        put_sio(DBG_CH, (UB)('0'+tmp), 10/MSEC);
    else
        put_sio(DBG_CH, (UB)('A'+tmp-0x0A), 10/MSEC);
}

/*****************************************************************************
* Routine to display 16-bit hex number on Debug screen.
* 16-bit data will be printed as 4 hex digits.
*
******************************************************************************/
static void print_H16(UH value)
{
    UB tmp;
    print_H8((UB)(value>>8));
    print_H8((UB)value);
}

#endif


/************************* End of file LAN.C *********************************/
