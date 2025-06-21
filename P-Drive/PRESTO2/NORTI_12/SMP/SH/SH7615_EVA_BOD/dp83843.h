/******************************************************************************
* PHY Controller Definitions (DP83843/National Semiconductor)                 *
*                                                                             *
*  File name : dp83843.h                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 13/Apr/2000 作成                                                      MiSPO *
******************************************************************************/

#ifndef DP83843_H
#define DP83843_H
#ifdef __cplusplus
extern "C" {
#endif


/* DP83843 register address */
#define PHY_BMCR       0x00
#define PHY_BMSR       0x01
#define PHY_ANAR       0x04
#define PHY_ANLPAR     0x05
#define PHY_ANER       0x06
#define PHY_ANNPTR     0x07
#define PHY_PHYSTS     0x10
#define PHY_MIPSCR     0x11
#define PHY_MIPGSR     0x12
#define PHY_DCR        0x13
#define PHY_FCSCR      0x14
#define PHY_RECR       0x15
#define PHY_PCSR       0x16
#define PHY_LBR        0x17
#define PHY_BTSCR      0x18
#define PHY_PHYCTRL    0x19

/* BMCR */
#define PHY_BMCR_RESET      0x8000  /* Initiate software Reset */
#define PHY_BMCR_LOOP       0x4000  /* Loopback */
#define PHY_BMCR_SPEED100   0x2000  /* Speed Selection 100/10Mbps */
#define PHY_BMCR_ANE        0x1000  /* Auto-Negotiation Enable */
#define PHY_BMCR_PDOWN      0x0800  /* Power Down */
#define PHY_BMCR_ISOL       0x0400  /* Isolation */
#define PHY_BMCR_REANE      0x0200  /* Restart Auto-Negotiation */
#define PHY_BMCR_DUP_FD     0x0100  /* Duplex FULL/HALF */
#define PHY_BMCR_COL_TEST   0x0080  /* Collision Test */

/* BMSR */
#define PHY_BMSR_T4         0x8000  /* 100BASE-T4 Capable */
#define PHY_BMSR_TX_FD      0x4000  /* 100BASE-TX Full Duplex Capable */
#define PHY_BMSR_TX_HD      0x2000  /* 100BASE-TX Half Duplex Capable */
#define PHY_BMSR_10_FD      0x1000  /*  10BASE-T  Full Duplex Capable */
#define PHY_BMSR_10_HD      0x0800  /*  10BASE-T  Half Duplex Capable */
#define PHY_BMSR_PREAMBLE   0x0040  /* Preamble suppression Capable */
#define PHY_BMSR_ANE_OK     0x0020  /* Auto-Negotiation Complete */
#define PHY_BMSR_RF         0x0010  /* Remote Fault */
#define PHY_BMSR_ANEGO_A    0x0008  /* Auto Configuration Ability */
#define PHY_BMSR_LINK       0x0004  /* Link Status */
#define PHY_BMSR_JABBER     0x0002  /* Jabber Detect */
#define PHY_BMSR_EXT        0x0001  /* Extended Capability */

/* ANAR */
#define PHY_ANAR_NP         0x8000  /* Next Page Indication */
#define PHY_ANAR_RF         0x2000  /* Remote Fault */
#define PHY_ANAR_FDFC       0x0400  /* Full Duplex Flow Control */
#define PHY_ANAR_T4         0x0200  /* 100BASE-T4 Support */
#define PHY_ANAR_TX_FD      0x0100  /* 100BASE-TX Full Duplex Support */
#define PHY_ANAR_TX_HD      0x0080  /* 100BASE-TX Half Duplex Support */
#define PHY_ANAR_10_FD      0x0040  /*  10BASE-T  Full Duplex Support */
#define PHY_ANAR_10_HD      0x0020  /*  10BASE-T  Half Duplex Support */

/* ANLPAR */
#define PHY_ANLPAR_NP       0x8000  /* Next Page Indication */
#define PHY_ANLPAR_ACK      0x4000  /* Acknowledge */
#define PHY_ANLPAR_RF       0x2000  /* Remote Fault */
#define PHY_ANLPAR_T4       0x0200  /* 100BASE-T4 Support */
#define PHY_ANLPAR_TX_FD    0x0100  /* 100BASE-TX Full Duplex Support */
#define PHY_ANLPAR_TX_HD    0x0080  /* 100BASE-TX Half Duplex Support */
#define PHY_ANLPAR_10_FD    0x0040  /*  10BASE-T  Full Duplex Support */
#define PHY_ANLPAR_10_HD    0x0020  /*  10BASE-T  Half Duplex Support */

/* ANER */
#define PHY_ANER_PDF        0x0010  /* Parallel Detection Fault */
#define PHY_ANER_LPNP       0x0008  /* Link Partner Next Page Able */
#define PHY_ANER_NP         0x0004  /* Next Page Able */
#define PHY_ANER_PAGE       0x0002  /* Link Code Word Page Received */
#define PHY_ANER_LPAN       0x0001  /* Link Partner Auto-Negotiation Able */

/* ANNPTR */
#define PHY_ANNPTR_NP       0x8000  /* Next Page Indication */
#define PHY_ANNPTR_MP       0x2000  /* Message Page */
#define PHY_ANNPTR_ACK2     0x1000  /* Acknowledge2 */
#define PHY_ANNPTR_TOG      0x0800  /* Toggle */

/* PHYSTS */
#define PHY_PHYSTS_RER      0x8000  /* Receive Error Latch */
#define PHY_PHYSTS_CIM      0x4000  /* Carrier Integrity Monitor Latch */
#define PHY_PHYSTS_FCS      0x2000  /* False Carrier Sense Latch */
#define PHY_PHYSTS_DR       0x0800  /* Device Ready */
#define PHY_PHYSTS_PR       0x0400  /* Link Code Word Page Received */
#define PHY_PHYSTS_ANE      0x0200  /* Auto-Negotiation Enabled */
#define PHY_PHYSTS_INT      0x0100  /* MII Interrupt Pending */
#define PHY_PHYSTS_RF       0x0080  /* Remote Fault */
#define PHY_PHYSTS_JABBER   0x0040  /* Jabber Detect */
#define PHY_PHYSTS_ANE_OK   0x0020  /* Auto-Negotiation Complete */
#define PHY_PHYSTS_RESET    0x0010  /* Reset Status */
#define PHY_PHYSTS_LOOP     0x0008  /* Loopback */
#define PHY_PHYSTS_DUP_FD   0x0004  /* Duplex FULL/HALF */
#define PHY_PHYSTS_SPEED10  0x0002  /* Speed 10/100Mbps */
#define PHY_PHYSTS_LINK     0x0001  /* Link Status */

/* MIPSCR */
#define PHY_MIPSCR_INTEN    0x0002  /* Interrupt Enable */
#define PHY_MIPSCR_TINT     0x0001  /* Test Interrupt */

/* MIPGSR */
#define PHY_MIPGSR_MINT     0x8000  /* MII Interrupt Pending */

/* PCSR */
#define PHY_PCSR_SD         0x8000  /* SIngle Ended Signal Detect Enable */
#define PHY_PCSR_FEFI       0x4000  /* Far End Fault Indication Mode */
#define PHY_PCSR_DESCR_RST  0x2000  /* Reset Descrambler Time-Out Counter */
#define PHY_PCSR_DESCR_SEL  0x1000  /* Descrambler Time-out Select */
#define PHY_PCSR_DESCR_DIS  0x0800  /* Descrambler Time-out Disable */
#define PHY_PCSR_LD_SCR     0x0400  /* Load Scrambler Seed */
#define PHY_PCSR_QUIET      0x0200  /* 100 Mbps Transmit True Quiet Mode */
#define PHY_PCSR_PATTERN    0x0180  /* 100 Mbps Transmit Test Pattern */
#define PHY_PCSR_LINK_100   0x0040  /* Force Good Link in 100Mbps */
#define PHY_PCSR_CIM_DIS    0x0020  /* Carrier Integrity Monitor Disable */
#define PHY_PCSR_CIM_STS    0x0010  /* Carrier Integrity Monitor Status */
#define PHY_PCSR_CODE_ERR   0x0008  /* Code Errors */
#define PHY_PCSR_PME_ERR    0x0004  /* Premature End Errors */
#define PHY_PCSR_LINK_ERR   0x0002  /* Link Errors */
#define PHY_PCSR_PKT_ERR    0x0001  /* Packet Errors */

/* LBR */
#define PHY_LBR_STRETCH     0x4000  /* Bypass LED Stretching */
#define PHY_LBR_4B5B        0x2000  /* Bypass 4B5B Encoding and 5B4B Decoding */
#define PHY_LBR_SCR         0x1000  /* Bypass Scrambler/Descrambler Function */
#define PHY_LBR_RX          0x0800  /* Bypass Receive Function */
#define PHY_LBR_TX          0x0400  /* Bypass Transmit Function */
#define PHY_LBR_LBEN        0x0020  /* TWISTER Loopback Enable */
#define PHY_LBR_10_ENDEC    0x0010  /* 10Mbps ENDEC Loopback */

/* BTSCR */
#define PHY_BTSCR_TPI       0x2000  /* TREX Operating Mode */
#define PHY_BTSCR_RX_SERIAL 0x1000  /* 10BASE-T RX Serial Mode */
#define PHY_BTSCR_TX_SERIAL 0x0800  /* 10BASE-T TX Serial Mode */
#define PHY_BTSCR_POL       0x0400  /* Polarity Detection & Correction Disable */
#define PHY_BTSCR_AUTOSW    0x0200  /* AUI/TPI Autoswitch */
#define PHY_BTSCR_LP_DS     0x0100  /* Link Pulse Disable */
#define PHY_BTSCR_HB_DS     0x0080  /* Heartbeat Disable */
#define PHY_BTSCR_LS_SEL    0x0040  /* Low Squelch Select */
#define PHY_BTSCR_AUI_SEL   0x0020  /* AUI Select */
#define PHY_BTSCR_JAB_DS    0x0010  /* Jabber Disable */
#define PHY_BTSCR_THIN_SEL  0x0008  /* Thin Ethernet Select */
#define PHY_BTSCR_FILT_DS   0x0004  /* TPI Receive Filter Disable */

/* PHYCTRL */
#define PHY_PHYCTRL_BLW_DS  0x0800  /* TWISTER Base Line Wander Disable */
#define PHY_PHYCTRL_REPEAT  0x0200  /* Repeater/Node Mode */
#define PHY_PHYCTRL_LED_DUP 0x0040  /* LED_DUP Mode Select */
#define PHY_PHYCTRL_FX_EN   0x0020  /* Fiber Mode Enable */

/* PHY BMCR設定データ */
#define HD10   0x0000                       /* 10M, Half,AutoNego=disable */
#define HD100  PHY_BMCR_SPEED100            /* 100M,Half,AutoNego=disable */
#define FD10   PHY_BMCR_DUP_FD              /* 10M, Full,AutoNego=disable */
#define FD100 (PHY_BMCR_SPEED100|PHY_BMCR_DUP_FD) /* 100M,Full,AutoNego=disable */

/* PHY ANAR用設定データ */
#define AHD10   PHY_ANAR_10_HD               /* 10M, Half,AutoNego=able */
#define AHD100  PHY_ANAR_TX_HD               /* 100M,Half,AutoNego=able */
#define AFD10   PHY_ANAR_10_FD               /* 10M, Full,AutoNego=able */
#define AFD100  PHY_ANAR_TX_FD               /* 100M,Full,AutoNego=able */

#define PHY_WRITE(reg,dat) PHY_write(PHY_get_adr(),reg,dat)
#define PHY_READ(reg) PHY_read(PHY_get_adr(),reg)

UB PHY_get_adr(void);
void PHY_write(UB adr, UB reg, UH dat);
UH PHY_read(UB adr, UB reg);
void PHY_init(BOOL auto_nego_able, UH modo);

#ifdef __cplusplus
};
#endif
#endif /* DP83843_H */
