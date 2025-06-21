/******************************************************************************
* LAN Controller Definition (SH7615 peripheral)                               *
*                                                                             *
*  File name : sh7615e.h                                                      *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 15/Apr/2000 作成                                                      MiSPO *
* 14/May/2002 Modified FIFO config for SH7616                        OK MiSPO *
* 15/May/2002 Modified bit-field on EDOCR                            OK MiSPO *
******************************************************************************/

#ifndef SH7615E_H
#define SH7615E_H
#ifdef __cplusplus
extern "C" {
#endif


/*------------------------------------------
 *--- E-DMAC Definition
 */

/* Buffer Descripter */
typedef struct BufferDescript {
    UW status;
    UH bufsize;
    UH rxsize;
    UB *buf_p;
    struct BufferDescript *next; /* user working area */
} ETHER_BD;

/* Buffer Descripter status */
#define DESC_ACT                0x80000000L
#define DESC_DL                 0x40000000L
#define DESC_FP1                0x20000000L
#define DESC_FP0                0x10000000L
#define DESC_FE                 0x08000000L

/* EDMR(E-DMACモードレジスタ) */
#define EDMR_DL16               0x00000000L
#define EDMR_DL32               0x00000010L
#define EDMR_DL64               0x00000020L
#define EDMR_SWR                0x00000001L

/* EDTRR(E-DMAC送信要求レジスタ) */
#define EDTRR_TR                0x00000001L

/* EDRRR(E-DMAC受信要求レジスタ) */
#define EDRRR_RR                0x00000001L

/* TDLAR(送信ディスクリプタ先頭アドレスレジスタ) */
#define TDLAR_16_MASK           0xFFFFFFF0L
#define TDLAR_32_MASK           0xFFFFFFE0L
#define TDLAR_64_MASK           0xFFFFFFC0L

/* RDLAR(受信ディスクリプタ先頭アドレスレジスタ) */
#define RDLAR_16_MASK           0xFFFFFFF0L
#define RDLAR_32_MASK           0xFFFFFFE0L
#define RDLAR_64_MASK           0xFFFFFFC0L

/* EESR(EtherC/E-DMACステータスレジスタ) */
#define EESR_RFCOF              0x01000000L
#define EESR_ECI                0x00400000L
#define EESR_TC                 0x00200000L
#define EESR_TDE                0x00100000L
#define EESR_TFUF               0x00080000L
#define EESR_FR                 0x00040000L
#define EESR_RDE                0x00020000L
#define EESR_RFOF               0x00010000L
#define EESR_ITF                0x00001000L
#define EESR_CND                0x00000800L
#define EESR_DLC                0x00000400L
#define EESR_CD                 0x00000200L
#define EESR_TRO                0x00000100L
#define EESR_RMAF               0x00000080L
#define EESR_RRF                0x00000010L
#define EESR_RTLF               0x00000008L
#define EESR_RTSF               0x00000004L
#define EESR_PRE                0x00000002L
#define EESR_CERF               0x00000001L

/* EESIPR(EtherC/E-DMACステータス割込み許可レジスタ) */
#define EESIPR_RFCOFIP          0x01000000L
#define EESIPR_ECIP             0x00400000L
#define EESIPR_TCIP             0x00200000L
#define EESIPR_TDEIP            0x00100000L
#define EESIPR_TFUFIP           0x00080000L
#define EESIPR_FRIP             0x00040000L
#define EESIPR_RDEIP            0x00020000L
#define EESIPR_RFOFIP           0x00010000L
#define EESIPR_ITFIP            0x00001000L
#define EESIPR_CNDIP            0x00000800L
#define EESIPR_DLCIP            0x00000400L
#define EESIPR_CDIP             0x00000200L
#define EESIPR_TROIP            0x00000100L
#define EESIPR_RMAFIP           0x00000080L
#define EESIPR_RRFIP            0x00000010L
#define EESIPR_RTLFIP           0x00000008L
#define EESIPR_RTSFIP           0x00000004L
#define EESIPR_PREIP            0x00000002L
#define EESIPR_CERFIP           0x00000001L

/* TRSCER(送受信ステータスコピー許可レジスタ) */
#define TRSCER_ITFCE            0x00001000L
#define TRSCER_CNDCE            0x00000800L
#define TRSCER_DLCCE            0x00000400L
#define TRSCER_CDCE             0x00000200L
#define TRSCER_TROCE            0x00000100L
#define TRSCER_RMAFCE           0x00000080L
#define TRSCER_RRFCE            0x00000010L
#define TRSCER_RTLFCE           0x00000008L
#define TRSCER_RTSFCE           0x00000004L
#define TRSCER_PRECE            0x00000002L
#define TRSCER_CERFCE           0x00000001L

/* RMFCR(ミスドフレームカウンタレジスタ) */
#define RMFCR_MASK              0x0000FFFFL

/* TFTR(送信FIFOしきい値指定レジスタ) */
#define TFTR_MASK               0x000007FFL

/* FDR(FIFO容量値設定レジスタ) */
#define FDR_TFD256              0x00000000L
#define FDR_TFD512              0x00000100L
#define FDR_RFD256              0x00000000L
#define FDR_RFD512              0x00000001L

#ifdef SH7616
/* SH7616 */
#define FDR_TFD768              0x00000200L
#define FDR_TFD1024             0x00000300L
#define FDR_TFD1280             0x00000400L
#define FDR_TFD1536             0x00000500L
#define FDR_TFD1792             0x00000600L
#define FDR_TFD2048             0x00000700L
#define FDR_RFD768              0x00000002L
#define FDR_RFD1024             0x00000003L
#define FDR_RFD1280             0x00000004L
#define FDR_RFD1536             0x00000005L
#define FDR_RFD1792             0x00000006L
#define FDR_RFD2048             0x00000007L

/* EDOCR(E-DMAC動作制御レジスタ) */
#define EDOCR_FEC               0x00000008L
#define EDOCR_AEC               0x00000004L
#define EDOCR_EDH               0x00000002L

#else
/* SH7615 */

/* EDOCR(E-DMAC動作制御レジスタ) */
#define EDOCR_FEC               0x00000004L
#define EDOCR_AEC               0x00000002L
#define EDOCR_EDH               0x00000001L

#endif

/* RCR(受信方式制御レジスタ) */
#define RCR_RNC                 0x00000001L

/*------------------------------------------
 *--- Ether-C Definition
 */

/* ECMR（EtherCモードレジスタ） */
#define ECMR_PRCEF              0x00001000L
#define ECMR_MPDE               0x00000200L
#define ECMR_RE                 0x00000040L
#define ECMR_TE                 0x00000020L
#define ECMR_ILB                0x00000008L
#define ECMR_ELB                0x00000004L
#define ECMR_DM                 0x00000002L
#define ECMR_PRM                0x00000001L

/* ECSR（EtherCステータスレジスタ） */
#define ECSR_LCHNG              0x00000004L
#define ECSR_MPR                0x00000002L
#define ECSR_ICD                0x00000001L

/* ECSIPR（EtherC割込み許可レジスタ） */
#define ECSIPR_LCHNGIP          0x00000004L
#define ECSIPR_MPRIP            0x00000002L
#define ECSIPR_ICDIP            0x00000001L

/* PIR（PHY部インタフェースレジスタ） */
#define PIR_MDI                 0x00000008L  /* DATA IN */
#define PIR_MDO                 0x00000004L  /* DATA OUT */
#define PIR_MMD                 0x00000002L  /* DIRECTION(read/write) */
#define PIR_MDC                 0x00000001L  /* DATA CLOCK */

/* PSR（PHY部ステータスレジスタ） */
#define PSR_LMON                0x00000001L

/* MAHR/MALR（MACアドレス上位/下位設定レジスタ） */
#define MAHR_MASK               0xFFFFFFFFL  /* Upper */
#define MALR_MASK               0x0000FFFFL  /* Lower */

/* TROCR（送信リトライオーバーカウンタレジスタ） */
#define TROCR_MASK              0x0000FFFFL

/* CDCR（衝突検出カウンタレジスタ） */
#define CDCR_MASK               0x0000FFFFL

/* LCCR（キャリア消失カウンタレジスタ） */
#define LCCR_MASK               0x0000FFFFL

/* CNDRC（キャリア未検出カウンタレジスタ） */
#define CNDRC_MASK              0x0000FFFFL

/* IFLCR（フレーム長異常カウンタレジスタ） */
#define IFLCR_MASK              0x0000FFFFL

/* CEFCR（CRCエラーフレーム受信カウンタレジスタ） */
#define CEFCR_MASK              0x0000FFFFL

/* FRECR（フレーム受信エラーカウンタレジスタ） */
#define FRECR_MASK              0x0000FFFFL

/* TSFRCR（64バイト未満フレーム受信カウンタレジスタ） */
#define TSFRCR_MASK             0x0000FFFFL

/* TLFRCR（指定バイト超フレーム受信カウンタレジスタ） */
#define TLFRCR_MASK             0x0000FFFFL

/* RFCR（端数ビットフレーム受信カウンタレジスタ） */
#define RFCR_MASK               0x0000FFFFL

/* MAFCR（マルチキャストアドレスフレーム受信カウンタレジスタ） */
#define MAFCR_MASK              0x0000FFFFL


#ifdef __cplusplus
}
#endif
#endif /* SH7615E_H */
