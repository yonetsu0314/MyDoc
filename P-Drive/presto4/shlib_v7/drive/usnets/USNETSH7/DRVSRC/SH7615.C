/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/drvsrc/RCS/sh7615.c 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: sh7615.c $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/

/* NET構造体のフィールドのうち、このドライバ内で独自に使用しているものの一覧 */
/* irno[0]    使用する割込み番号                                             */
/* base[0]    送信ディスクリプタの先頭アドレス                               */
/* base[1]    受信ディスクリプタの先頭アドレス                               */
/* irno[1]    次のパケットの送信に使用する送信ディスクリプタのアドレス       */
/* irno[2]    次に送信が完了する（はず）の送信ディスクリプタのアドレス       */
/* irno[3]    次に使用する受信ディスクリプタのアドレス                       */
/* irno[0]    Interrupt number                                               */
/* base[0]    Base address for transmit descriptors                          */
/* base[1]    Base address for receive descriptors                           */
/* irno[1]    Address of descriptor which will be used to send next packet   */
/* irno[2]    Address of descriptor which complete transmit next             */
/* irno[3]    Address of descriptor which next packet will be received       */

#include <string.h>
#include "net.h"
#include "local.h"
#include "driver.h"
#include "support.h"
#include "ethernet.h"
#include "sh7615.h"
#include "itron.h"
#include "kernel.h"

#define NUM_TXDESC	8	/* number of Tx descriptors. Too much? */
#define NUM_RXDESC	8	/* number of Rx descriptors */

/* Area for Receive/Transmit Descriptor. Byte alignment will be adjusted */
/* at the initialization. */
unsigned char DescArea[(NUM_TXDESC+NUM_RXDESC+1)*sizeof (struct TxDesc_t)];

extern struct CONNECT connblo[];
extern struct NET nets[];

/* Link Status. Can be referenced from the application. */
/*  bit2(0x04) ... Duplex Mode.  1 - Full duplex, 0 - Half duplex */
/*  bit1(0x02) ... Link Speed.   1 - 10Mbps, 0 - 100Mbps */
/*  bit0(0x01) ... Link Status.  1 - Link Valid, 0 - Link not established */
unsigned short ReadLinkStatus(void);
unsigned short LinkStatus;
unsigned short LastLinkStatus;
unsigned short ResetLinkStatus;
static char EthrtC_init_flag = 0;

static unsigned char LinkFlagNo = 0;
static const T_CFLG cflg = { TA_WMUL, 0, "" };
#define LINK_UP		0x01
#define LINK_DOWN	0x02

int eth_flg[10];
void eth_chk( void )
{
	int i;
	for( i=0; i<10; i++ ) {
		if( eth_flg[i] == SIG_RN(i) ) {
			WAITNOMORE_IR(SIG_RN(i));
			eth_flg[i] = 0;
		}
		else if( eth_flg[i] == SIG_WN(i) ) {
			WAITNOMORE_IR(SIG_WN(i));
			eth_flg[i] = 0;
		}
	}
}

char UsnetLinkFlagID( void ) {
	return LinkFlagNo;
}

void WaitLinkUp( void )
{
	/* 回線接続されるまで待つ */
	for(;;) {
		LinkStatus = ReadLinkStatus();
		if (LinkStatus & PHYSTS_LNKSTS) break;
		dly_tsk( 25 );
    }
	/* forループによる時間計測があるためスケジュールをロックする*/
}

/*	デバッグ用レジスタ	*/
struct TxDesc_t  *Tww[9];	/* Tx Descriptor */
struct RxDesc_t  *Rww[9];	/* Tx Descriptor */

static void SH7615SendComplete(int netno,struct TxDesc_t *Tw);

static void ResetEtherC( void )
{
    int i;
	int netno;
    unsigned long ul1;
    struct NET *netp;
    struct TxDesc_t *Tw;	/* Tx Descriptor */
    struct RxDesc_t *Rw;	/* Rx Descriptor */

	netno = 0;
    netp = &nets[netno];

	/*	割り込み要因検出、またはLinkDouwnからLinkUpに状態が変化した場合
		は、シャットダウン及びEtherCのリセットを行なう。	*/

	/* Duplexモードが変化した場合、ECMRレジスタのDMビットの設定を変える */
	/* 必要はあるのか？ */
	/* DMビットの再設定は一旦EtherCをリセットする事になるので、バッファの */
	/* 扱いが面倒になる */
	if (LinkStatus & PHYSTS_SPEED10) netp->bps = 10000000;
	else                      netp->bps = 100000000;

	if ((ResetLinkStatus & PHYSTS_DUPLEX) != (LinkStatus & PHYSTS_DUPLEX)) {
		ResetLinkStatus = LinkStatus;
		/* Duplexモードが変化した場合、一旦EtherCをリセットする */
		/* 必要がある。 */
		/* この場合のバッファの扱いは、受信バッファに関しては未読 */
		/* データは存在しない筈なので全て空きにする。送信バッファに */
		/* 関しても未送信は存在しない筈なので、全て開放。 */
		_outl(EDMR,0x01);           /* reset EtherC and E-DMAC */
		_outl(RFLR,0x5ee);          /* Maximum receive length (include CRC) */
		_outl(ECSR,0x07);           /* clear EtherC status */
		_outl(ECSIPR,0);            /* LCNHG/MPRIP/ICDIP interrupt disable */
		_outl(MAHR,*(unsigned long *)&netp->id.c[0]);  /* high 32bit of MAC address */
		_outl(MALR,*(unsigned short *)&netp->id.c[4]); /* low 16bit of MAC address */
		_outl(EDMR,0x00);           /* set descriptor length to 16bytes */
		_outl(TDLAR,(unsigned long)netp->base[0]);     /* Tx descriptor address */
		_outl(RDLAR,(unsigned long)netp->base[1]);     /* Rx descriptor address */
		_outl(EESR,0x017f1f9f);     /* clear status */
		_outl(EESIPR,0);            /* disable interrupt */
		_outl(TRSCER,EESR_RMAF);    /* copy status bits to descriptor */
		_outl(TFTR,0);              /* FIFO store and forward mode */
	/************************************************************************/
	/* _outl(FDR,0x00);            /* FIFO depth = 256 */
	_outl(FDR,0x01);            /* FIFO depth = 512 */
	/************************************************************************/
		_outl(RCR,RCR_RNC);         /* do not stop receive */
		_outl(EDOCR,0);             /* coutinue E-DMAC in underflow or overrun */

		netp->irno[1] = netp->irno[2] = (int)netp->base[0];
		netp->irno[3] = (int)netp->base[1];

		Rw = (struct RxDesc_t *)netp->base[1];
		for (i=0;i<NUM_RXDESC;i++) {
		/**************************************/
		/*	デバッグ用レジスタ	*/
		Rww[i] = Rw;
		/***************************************/
		if (i >= NUM_RXDESC-1) {
			Rw->status = EDESC_RACT | EDESC_RDL;
		} else {
			Rw->status = EDESC_RACT;
		}
			Rw++;
		}
		Tw = (struct TxDesc_t *)netp->irno[2];
		while ((Tw->buffer != (void *)0)&&(!(Tw->status & EDESC_TACT))) {
			SH7615SendComplete(netno,Tw);
			if (Tw->status & EDESC_TDL) Tw = (struct TxDesc_t *)netp->base[0];
			else                        Tw++;
		}

		/* リンク状態からDuplex Modeの設定状態を決定する */
		if (LinkStatus & PHYSTS_DUPLEX) ul1 = ECMR_DM;    /* Full Duplex Mode */
		else                     ul1 = 0;          /* Half Duplex Mode */

		_outl(ECSIPR,ECSR_LCNHG | ECSR_ICD);       /* LCNHG/ICDIP interrupt enable */
		_outl(EESIPR,EESR_ERRBITS | EESR_TC | EESR_FR | EESR_ECI);  /* interrupt enable */
		_outl(EDRRR,EDRRR_RR);                     /* receive request */
		_outl(ECMR,ECMR_RE | ECMR_TE | ul1);       /* start controller */
	}
    /* This controller seems to stops when run outs receive descriptors. */
    /* Restart receive if stopped */
    if ((_inl(EDRRR) & EDRRR_RR) == 0) {
        _outl(EDRRR,EDRRR_RR);
    }
}

void UsnetCheckLinkUp( void )
{
	unsigned short states, last_states;

	if( !EthrtC_init_flag ) {
		/*	EtherCが未初期化の場合は処理しない。*/
		return;
	}
	LinkStatus = ReadLinkStatus();
	states = (LinkStatus&PHYSTS_LNKSTS);
	last_states = (LastLinkStatus&PHYSTS_LNKSTS);
	if (states) {
		if( last_states != states ) {
			/*	接続を検知	*/
			set_flg(LinkFlagNo, LINK_UP);
			ResetEtherC();
		}
	}
	else {
		if( last_states != states ) {
			/*	切断を検知	*/
			set_flg(LinkFlagNo, LINK_DOWN);
		}
	}
	LastLinkStatus = LinkStatus;
}

/* Delay loop count. 1 microsecond delay required to access PHYTER register. */
unsigned long delaycnt;

/* Delay loop to access PHYTER register. */
void delayloop(unsigned long cnt)
{
    while (cnt--);
}

/* ※PHYTER関係の関数の動作確認はDP83843で行っている。PHYTERに依存するかもしれない。 */
/* Following MII functions (WriteMII1bit,WriteMIIBusRelease,ReadMII1bit,ReadMII, */
/* WriteMII,ReadLinkStatus) are tested with DP83843 PHYTER. May depend on the */
/* PHYTER lsi, but I'm not sure. */

/* Send 1 bit to PHYTER. */
/* Inputs  : unsigned short data - data will be sent. */
/*                          bit15(MSB) - data bit */
void WriteMII1bit(unsigned short data)
{
    if (data & 0x8000) {
        data = PIR_MDO;
    } else {
        data = 0;
    }

    _outl(PIR,data | PIR_MMD);
    delayloop(delaycnt);
    _outl(PIR,data | PIR_MMD | PIR_MMC);
    delayloop(delaycnt);
    _outl(PIR,data | PIR_MMD);
    delayloop(delaycnt);
}

/* Bus release for PHYTER */
void WriteMIIBusRelease(void)
{
    _outl(PIR,0);
    delayloop(delaycnt);
    _outl(PIR,PIR_MMC);
    delayloop(delaycnt);
    _outl(PIR,0);
    delayloop(delaycnt);
}

/* Single Bus Release for PHYTER */
void WriteMIIBusRelease1(void)
{
    _outl(PIR,0);
    delayloop(delaycnt);
}

/* Read 1 bit data from PHYTER */
/* Outputs : data. The bit will be set to LSB. */
unsigned short ReadMII1bit(void)
{
    unsigned long ul1;

    _outl(PIR,PIR_MMC);
    delayloop(delaycnt);
    ul1 = _inl(PIR);
    delayloop(delaycnt);
    _outl(PIR,0);
    delayloop(delaycnt);

    ul1 = (ul1 & PIR_MDI) >> 3;		/* move MDI bit to LSB */
    return (unsigned short)(ul1 & 0x0001);
}

/* Read Management data from PHY */
/* Inputs  : phyad - PHY address. 5 bits. */
/*           regad - Register address. 5 bits. */
/* Outputs : 16-bit register contents. */
unsigned short ReadMII(int phyad,int regad)
{
    union MIIframe frame;
    unsigned short us1,us2;
    int i;

    /* Create request frame to PHYTER. May depend on the endian. */
    frame.st.pre = 0xffffffff;	/* Synchronization */
    frame.st.st = 0x1;		/* Start */
    frame.st.op = 0x2;		/* Opcode (read) */
    frame.st.phyad = phyad;	/* PHY address */
    frame.st.regad = regad;	/* Register address */

    /* send pre,st,op,phyad,regad */
    us1 = frame.b[0];
    for (i=0;i<16;i++) {
        WriteMII1bit(us1);
        us1 <<= 1;
    }

    us1 = frame.b[1];
    for (i=0;i<16;i++) {
        WriteMII1bit(us1);
        us1 <<= 1;
    }

    us1 = frame.b[2];
    for (i=0;i<14;i++) {
        WriteMII1bit(us1);
        us1 <<= 1;
    }

    /* send TA (bus release) */
    WriteMIIBusRelease();

    /* read register data */
    for (us1=i=0;i<16;i++) {
        us1 <<= 1;
        us2 = ReadMII1bit();
        us1 |= us2;
    }

    return us1;
}

/* Write Management data to PHY */
/* Inputs  : phyad - PHY address. 5 bits. */
/*           regad - Register address. 5 bits. */
/*           data  - 16 bit register contents. */
void WriteMII(int phyad,int regad,unsigned short data)
{
    union MIIframe frame;
    unsigned short us1,us2;
    int i,j;

    /* Create request frame to PHYTER. May depend on the endian. */
    frame.st.pre = 0xffffffff;	/* Synchronization */
    frame.st.st = 0x1;		/* Start */
    frame.st.op = 0x1;		/* Opcode (write) */
    frame.st.phyad = phyad;	/* PHY address */
    frame.st.regad = regad;	/* Register address */
    frame.st.ta = 0x2;		/* TA (write) */
    frame.st.data = data;	/* data */

    /* send pre,st,op,phyad,regad,ta,data */
    for (j=0;j<4;j++) {
        us1 = frame.b[j];
        for (i=0;i<16;i++) {
            WriteMII1bit(us1);
            us1 <<= 1;
        }
    }
    /* bus release */
    WriteMIIBusRelease1();
}

/* リンク状態をPHYTERから取得する */
/* Read Link status from PHYTER */
/* Inputs  : none */
/* Outputs : unsigned short - link status */
/*           bit0(0x01) ... 1 - link valid / 0 - link invalid */
/*           bit1(0x02) ... 1 - 10Mbps / 0 - 100Mbps */
/*           bit2(0x04) ... 1 - full duplex / 0 - half duplex */
unsigned short ReadLinkStatus(void)
{
    unsigned short us1;

    us1 = ReadMII(PHYAD,PHYREG_PHYSTS);
    return us1;
}

/* 受信済みの受信ディスクリプタからデータを取り出し、USNETの受信キューに格納する */
/* Inputs       : netno - Network number */
/*                Rw    - Receive Descriptor */
/* Return value : status */
/*                bit0 (0x01) - bit26 : Rx Frame status. See sh7615 manual. */
/*                bit31 (0x80000000) - Receive queue full. */
/*                bit30 (0x40000000) - Can't allocate altanative buffer. */
static unsigned long SH7615RecvPacket(int netno,struct RxDesc_t *Rw)
{
    unsigned long sts;
    unsigned short len;
    MESS *mess;
    struct NET *netp = &nets[netno];
    unsigned long ul1;
    unsigned long rc;

    sts = Rw->status;                   /* get status and length */
    len = Rw->datalen;
    mess = (MESS *)(((unsigned long)Rw->buffer & CADDR_MASK) - MESSH_SZ);	/* get buffer address */

    mess->mlen = len + MESSH_SZ;
    mess->offset = MESSH_SZ;
    mess->netno = netno;

    /* Discard packet if there are some errors */
    if (sts & EDESC_RFE) {
#if NTRACE >= 1
/*
        Nprintf("irhan : error status (0x%08lX)\n",sts);
*/
#endif
        netp->ifInErrors++;
        rc = sts & 0x03ffffff;
    } else {

        /* if receive queue is full, discard packet */
        if (QUEUE_FULL(netp, arrive)) {
#if NTRACE >= 1
/*
            Nprintf("irhan : queue full.\n");
*/
#endif
            netp->ifInDiscards++;
            rc = 0x80000000;
        } else {

            /* Buffer address of receive descriptor must not be a null. */
            /* So, received packet must be discarded if failed to allocate */
            /* new buffer which will be replaced to this. */
            ul1 = (unsigned long)NgetbufIR();
            if (ul1 == 0) {
#if NTRACE >= 1
/*
                Nprintf("irhan : Can't allocate buffer.\n");
*/
#endif
                netp->ifInDiscards++;
                rc = 0x40000000;
            } else {

#if NTRACE >= 5
/*                Nprintf("SH7615 irhan : %d bytes received, allocate 0x%08lX\n",len,ul1);
*/
#endif

                ul1 |= NCADDR_MASK;	/* non-cached address */

                /* Queue in packet and signal to NetTask. */
                QUEUE_IN(netp, arrive, mess);
/*****************************************************/
#if 1
                WAITNOMORE_IR(SIG_RN(netno));
#else
				eth_flg[netno]=SIG_RN(netno);
#endif
/*****************************************************/
                /* Assign new buffer to this descriptor */
                ul1 += MESSH_SZ;
                Rw->buffer = (void *)ul1;
                Rw->buflen = MAXBUF - MESSH_SZ;
                rc = 0;
            }
        }
    }

    /* Return descriptor to controller */
    Rw->status |= EDESC_RACT;

    return rc;
}

/* 送信済みの送信ディスクリプタに割り当てられたバッファを開放する
/* Inputs       : netno - Network number */
/*                Tw    - Transmit Descriptor */
static void SH7615SendComplete(int netno,struct TxDesc_t *Tw)
{
    MESS *mess = (MESS *)(((unsigned long)Tw->buffer & CADDR_MASK) - MESSH_SZ);

    /* Release buffer if neccesary. */
    mess->offset = boTXDONE;
    if (mess->id <= bWACK) {
        if (mess->id == bRELEASE) {
            mess->id = bALLOC;
#if NTRACE >= 5
/*            Nprintf("SH7615 irhan : release buffer 0x%08lX\n",(unsigned long)mess & 0x1fffffff);
*/
#endif
            NrelbufIR(mess);
        }
    } else {
/*****************************************************/
#if 1
        WAITNOMORE_IR(SIG_WN(netno));
#else
		eth_flg[netno]=SIG_WN(netno);
#endif
/*****************************************************/
    }
    Tw->buffer = (void *)0;
}


/* ==========================================================================
   C level interrupt handler for Ethernet.  Called from a stub, registers are
   saved.  Returns to the interrupt stub.  Queues the arrived message into
   the arrive queue of the network block.  */

static void irhan(int netno);

#pragma interrupt (irhan00)
void irhan00(void)
{
	ent_int();
	irhan(0);
	ret_int();
}

static void irhan(int netno)
{
    unsigned long ul1;
    MESS *mess;
    struct NET *netp;
    unsigned long ests;
    unsigned long sts;
    unsigned short len;
    int i;
    struct TxDesc_t *Tw;	/* Tx Descriptor */
    struct TxDesc_t *Tw0;	/* Tx Descriptor */
    struct RxDesc_t *Rw;	/* Rx Descriptor */
    unsigned short us1;

    netp = &nets[netno];

    /* get interrupt status */
    ests = _inl(EESR);

#if NTRACE >= 5
/*    Nprintf("irhan : EESR = 0x%08lX\n",ests);
*/
#endif

    /* receive interrupt handling */
    if (ests & EESR_FR) {			/* "Frame received" */
        _outl(EESR,EESR_FR);			/* clear status */
        Rw = (struct RxDesc_t *)netp->irno[3];
        while (!(Rw->status & EDESC_RACT)) {	/* loop for read all available data */
            sts = SH7615RecvPacket(netno,Rw);

            /* move to next descriptor */
            if (Rw->status & EDESC_RDL) Rw = (struct RxDesc_t *)netp->base[1];
            else                        Rw++;
        }
        netp->irno[3] = (int)Rw;
    }

    /* Transmit completed */
    if (ests & EESR_TC) {
        _outl(EESR,EESR_TC);		/* clear status */
#if NTRACE >= 5
/*
        Nprintf("SH7615 irhan : transmit completed.\n");
*/
#endif

        Tw = (struct TxDesc_t *)netp->irno[2];

        /* +-------------+-------------+------------------+ */
        /* | Tw->buffer  | EDESC_TACT  | descriptor status| */
        /* +=============+=============+==================+ */
        /* | zero        | zero        | Empty            | */
        /* +-------------+-------------+------------------+ */
        /* | non-zero    | non-zero    | Transmitting     | */
        /* +-------------+-------------+------------------+ */
        /* | non-zero    | zero        | Transmit done    | */
        /* +-------------+-------------+------------------+ */
        /* | zero        | non-zero    | Invalid          | */
        /* +-------------+-------------+------------------+ */

        while ((Tw->buffer != (void *)0)&&(!(Tw->status & EDESC_TACT))) {
            SH7615SendComplete(netno,Tw);

            /* Send next packet if queued */
            if (!(QUEUE_EMPTY(netp, depart))) {	/* send packet in queue */
                Tw0 = Tw;
#if NTRACE >= 5
/*
                Nprintf("SH7615 irhan : send queued packet.\n");
*/
#endif
                QUEUE_OUT(netp, depart, mess);
                ul1 = (unsigned long)mess + MESSH_SZ;
                len = mess->mlen - MESSH_SZ;
                if (len < ET_MINLEN) len = ET_MINLEN;

                Tw0->buffer = (void *)(ul1 | NCADDR_MASK); /* access through non-cached address */
                Tw0->datalen = len;
                Tw0->status |= EDESC_TACT;
                _outl(EDTRR,EDTRR_TR);		/* start transmit */
                if (Tw0->status & EDESC_TDL) Tw0 = (struct TxDesc_t *)netp->base[0];
                else                         Tw0++;
                netp->irno[1] = (int)Tw0;
                /* ネットワーク輻輳時にUDP送信が止まってしまう問題の場当たり的な対応 */
/*****************************************************/
#if 1
                WAITNOMORE_IR(SIG_WN(netno));
#else
				eth_flg[netno]=SIG_WN(netno);
#endif
/*****************************************************/
            } else {
                netp->hwflags = 0;
            }

            if (Tw->status & EDESC_TDL) Tw = (struct TxDesc_t *)netp->base[0];
            else                        Tw++;
        }
        netp->irno[2] = (int)Tw;
    }

    /* Receive Frame Counter Overflow */
    /* Tx Descriptor Exhausted */
    /* Tx FIFO Underflow */
    /* Rx Descriptor Exhausted */
    /* Rx FIFO Overflow */
    /* Illegal Tx Frame */
    /* Carrier Not Detect */
    /* Detect Loss of Carrier */
    /* Collision Detect */
    /* Tx Retry Over */
    /* Receive Residual-bit Frame */
    /* Receive Too-Long Frame */
    /* Receive Too-Short Frame */
    /* PHY-LSI Receive Error */
    /* CRC Error on Received Frame */
    if (ests & EESR_ERRBITS) {
        _outl(EESR,EESR_ERRBITS);		/* clear status */
    }

    /* clear receive-multicast-address flag */
    if (ests & EESR_RMAF) {
        _outl(EESR,EESR_RMAF);			/* clear status */
    }

    /* EtherC status register interrupt */
    if (ests & EESR_ECI) {
        _outl(EESR,EESR_ECI);			/* clear status */
        sts = _inl(ECSR);
        _outl(ECSR,ECSR_LCNHG | ECSR_MPR | ECSR_ICD);	/* clear status */
	}

    /* This controller seems to stops when run outs receive descriptors. */
    /* Restart receive if stopped */
    if ((_inl(EDRRR) & EDRRR_RR) == 0) {
        _outl(EDRRR,EDRRR_RR);
    }
}


/* ==========================================================================
   Transmit routine.  If the transmitter is idle, starts the transmission and
   returns.  Otherwise adds message to the departure queue; the interrupt
   handler will trasmit it.  Returns:
    error:    -1
    success:  0
 */

static int writE(int conno, MESS *mess)
{
    int len, stat, i;
    unsigned long ul1;
    struct NET *netp;
    struct TxDesc_t *Tw;	/* Tx Descriptor */

    netp = &nets[mess->netno];
    mess->offset = mess->netno;

    DISABLE();

    Tw = (struct TxDesc_t *)netp->irno[1];	/* next available Tx descriptor */
    if (Tw->buffer != (void *)0) {		/* no descriptors available - controller busy */
#if NTRACE >= 5
/*        Nprintf("SH7615 writE : transmitter busy. Queue packet\n");*/
#endif
        if (QUEUE_FULL(netp, depart)) {		/* if queue is full */
#if NTRACE >= 1
/*            Nprintf("SH7615 writE : Queue full, discard packet\n");*/
#endif
            netp->ifOutDiscards++;
            mess->offset = boTXDONE;
            stat = ENOBUFS;
        } else {
            QUEUE_IN(netp, depart, mess);
            stat = 0;
        }
    }
    else
    {
        netp->hwflags = 1;
        ul1 = (unsigned long)mess + MESSH_SZ;
        len = mess->mlen - MESSH_SZ;
        if (len < ET_MINLEN) len = ET_MINLEN;

#if NTRACE >= 5
/*        Nprintf("SH7615 writE : send %d bytes.\n",len);*/
#endif

        Tw->buffer = (void *)(ul1 | NCADDR_MASK); /* access through non-cached address */
		Tw->datalen = len;
		Tw->status |= EDESC_TACT;
		_outl(EDTRR,EDTRR_TR);
/************************************************************/
#if 1
		i = 0;
		while ((Tw->status & EDESC_TACT)&&(i<1000)) {	/* loop for read all available data */
/*		while ((Tw->status & EDESC_TACT)) {*/	/* loop for read all available data */
			_outl(EDTRR,EDTRR_TR);
		    Tw = (struct TxDesc_t *)netp->irno[1];	/* next available Tx descriptor */
			i++;
		}
#endif
/************************************************************/

        if (Tw->status & EDESC_TDL) Tw = (struct TxDesc_t *)netp->base[0];
        else                        Tw++;
        netp->irno[1] = (int)Tw;

        stat = 0;
    }

    ENABLE();
    return stat;
}

/* ==========================================================================
   Open a connection. */

static int opeN(int conno, int flag)
{
    (void)conno, (void)flag;
    return 0;
}


/* ==========================================================================
   Close the connection. */

static int closE(int conno)
{
    (void)conno;
    return 0;
}

/* ==========================================================================
   Configure and start up the Ethernet interface.  We process the user-level
   text parameters and store the values into the net table.  We take the
   Ethernet address, and set up the controller.  Then we store the interrupt
   address and enable the interrupt.  Returns:
    error:    -1
    success:   0
 */

static int init(int netno, char *params)
{
    int i1;
    struct NET *netp;
    short *spt;
    char *cp1, par[16], val[16];
    unsigned long ul1,ul2;
    unsigned short us1;
    struct TxDesc_t *Tw;	/* Tx Descriptor */
    struct RxDesc_t *Rw;	/* Rx Descriptor */

    netp = &nets[netno];

#if NTRACE >= 5
    Nprintf("SH7615 init : start\n");
#endif
    if (params != 0) {
        for (cp1=params; *cp1; )
        {
            Nsscanf(cp1, "%[^=]=%s %n", par, val, &i1);
            cp1 += i1;
            if (strcmp(par, "ENA") == 0)
                Nsscanf(val, "%i", &netp->base[0]);
        }
    }

    i1 = netp->protoc[2]->init(netno, params);
    if (i1 < 0) {
        return i1;
    }
    netp->irno[0] = EDMAC_VECTNO;

    spt = (short *)&netp->id;			/* get Ethernet address */
    if ((spt[0] | spt[1] | spt[2]) == 0) {	/* unless configured */
        memcpy(netp->id.c, (char *)netp->base[0], Eid_SZ);
    }

    ul1 = (unsigned long)DescArea | NCADDR_MASK;	/* access through non-cached address */
    if (ul1 & 0x0f) ul1 = (ul1 & 0xfffffff0) + 0x10;
    netp->base[0] = (char *)ul1;
    netp->base[1] = (char *)(ul1+sizeof (struct TxDesc_t) * NUM_TXDESC);
    netp->irno[1] = netp->irno[2] = (int)netp->base[0];
    netp->irno[3] = (int)netp->base[1];

    _outl(EDMR,0x01);				/* reset EtherC and E-DMAC */

    _outl(RFLR,0x5ee);				/* Maximum receive length (include CRC) */
    _outl(ECSR,0x07);				/* clear EtherC status */
    _outl(ECSIPR,0);				/* LCNHG/MPRIP/ICDIP interrupt disable */
    _outl(MAHR,*(unsigned long *)&netp->id.c[0]);	/* high 32bit of MAC address */
    _outl(MALR,*(unsigned short *)&netp->id.c[4]);	/* low 16bit of MAC address */
    _outl(TROCR,0);				/* reset Tx Retry Over Counter */
    _outl(CDCR,0);				/* reset Collision Detect Counter */
    _outl(LCCR,0);				/* reset Lost Carrier Counter */
    _outl(CNDCR,0);				/* reset Carrier Not Detect Counter */
    _outl(IFLCR,0);				/* reset Illegal Frame Length Counter */
    _outl(CEFCR,0);				/* reset CRC Error Frame Counter */
    _outl(FRECR,0);				/* reset Frame Receive Error Counter */
    _outl(TSFRCR,0);				/* reset Too-short Frame Receive Counter */
    _outl(TLFRCR,0);				/* reset Too-long Frame Receive Counter */
    _outl(RFCR,0);				/* reset Residual-bit Frame Counter */
    _outl(MAFCR,0);				/* reset Multicast Address Frame Counter */

    _outl(EDMR,0x00);				/* set descriptor length to 16bytes */
    _outl(TDLAR,(unsigned long)netp->base[0]);	/* Tx descriptor address */
    _outl(RDLAR,(unsigned long)netp->base[1]);	/* Rx descriptor address */
    _outl(EESR,0x017f1f9f);			/* clear status */
    _outl(EESIPR,0);				/* disable interrupt */
    _outl(TRSCER,EESR_RMAF);			/* copy status bits to descriptor */
    ul1 = _inl(RMFCR);				/* reset Missed-Frame counter */
    _outl(TFTR,0);				/* FIFO store and forward mode */
/************************************************************************/
 /* _outl(FDR,0x00);            /* FIFO depth = 256 */
    _outl(FDR,0x01);            /* FIFO depth = 512 */
/************************************************************************/
    _outl(RCR,RCR_RNC);				/* do not stop receive */
    _outl(EDOCR,0);				/* coutinue E-DMAC in underflow or overrun */

    Tw = (struct TxDesc_t *)netp->base[0];
    for (i1=0;i1<NUM_TXDESC;i1++) {
/***************************************/
/*	デバッグ用レジスタ	*/
		Tww[i1] = Tw;
/***************************************/
        if (i1 >= NUM_TXDESC-1) {
            Tw->status = EDESC_TDL | EDESC_TFP;
        } else {
            Tw->status = EDESC_TFP;
        }
        Tw->buffer = (void *)0;
		Tw++;
    }

    if (ul1 & 0x0f) ul1 = (ul1 & 0xfffffff0) + 0x10;
    Rw = (struct RxDesc_t *)netp->base[1];
    for (i1=0;i1<NUM_RXDESC;i1++) {
/***************************************/
/*	デバッグ用レジスタ	*/
		Rww[i1] = Rw;
/***************************************/
        if (i1 >= NUM_RXDESC-1) {
            Rw->status = EDESC_RACT | EDESC_RDL;
        } else {
            Rw->status = EDESC_RACT;
        }

        ul1 = (unsigned long)Ngetbuf();

        if (ul1 == 0) {
#if NTRACE >= 1
            Nprintf("SH7615 init : Can't allocate buffer.\n");
#endif
            return ENOBUFS;
        }

#if NTRACE >= 5
        Nprintf("SH7615 init : allocate buffer 0x%08lX\n",ul1);
#endif

        ul1 |= NCADDR_MASK;     /* non-cached address */
        ul1 += MESSH_SZ;
        if (ul1 & 0xf) {
#if NTRACE >= 1
            Nprintf("SH7615 init : USSBUFALIGN must be set to 16.\n");
#endif
            return ENOBUFS;
        }
        Rw->buffer = (void *)ul1;
        Rw->buflen = MAXBUF - MESSH_SZ;
        Rw++;
    }

    /* DP83843(PHYTER)のレジスタアクセスに必要なクロック間隔を確保するための */
    /* ディレイタイマ用のループカウントを算出する。1μ秒以上が適当。 */
    /* TimeMS()を用いて100ミリ秒に相当するループカウント値を計算し、1μ秒に */
    /* 換算する */
    /* もしTimeMS()が動いていないとこのコードは永久ループに陥るが、これは */
    /* USNETにとって致命的なトラブルなのでドライバだけ救済しても意味がない */
    /* ....実は、ここでの１秒間のディレイはPHYTERのAuto Negotiationの完了 */
    /* 待ちも兼ねている。 */

    for (ul1=TimeMS();TimeMS()==ul1;);	/* 時刻の変わり目に同期させる */

    /* 仮にループカウントを1000回として、1000ミリ秒間でディレイループを何回実行 */
    /* できるかを測る */
    for (ul1=TimeMS(),delaycnt=0;(TimeMS()-ul1)<100;delaycnt++) {
        delayloop(1000);
    }
    delaycnt /= 1000;			/* 1μ秒に対するループカウント */
    if (delaycnt == 0) delaycnt = 1;	/* 最低限 */

    /* PHYTERのAuto-Negotiationに１秒程度必要なため、電源投入直後ではAuto-Negotiation */
    /* 完了前にリンク状態を取得してしまいDuplex Modeの決定を間違う事がある。 */
    /* そのため、Auto-Negotiation完了まで（最大５秒）待つ */
    ul1 = TimeMS();
    while ((TimeMS() - ul1) < 5000) {
        LinkStatus = ReadLinkStatus();
        if (LinkStatus & PHYSTS_LNKSTS) break;
    }

/*********************************************************/
#if 0
	WriteMII(PHYAD,PHYREG_BMCR,0x2000);
    ul1 = TimeMS();
    while ((TimeMS() - ul1) < 5000) {
        LinkStatus = ReadLinkStatus();
        if (LinkStatus & PHYSTS_LNKSTS) break;
    }
#endif
/**********************************************************/

    /* リンク状態からDuplex Modeの設定状態を決定する */
    if (LinkStatus & PHYSTS_DUPLEX) ul1 = ECMR_DM;	/* Full Duplex Mode */
    else                            ul1 = 0;		/* Half Duplex Mode */

    /* リンクの通信速度を調べる。SNMP統計用データ。 */
    if (LinkStatus & PHYSTS_SPEED10) netp->bps = 10000000;
    else                             netp->bps = 100000000;
//nh    IRinstall(netp->irno[0], netno, irhan);


    _outl(ECSIPR,ECSR_LCNHG | ECSR_ICD);	/* LCNHG/ICDIP interrupt enable */
    _outl(EESIPR,EESR_ERRBITS | EESR_TC | EESR_FR | EESR_ECI);	/* interrupt enable */

    _outl(EDRRR,EDRRR_RR);			/* receive request */
    _outl(ECMR,ECMR_RE | ECMR_TE | ul1);	/* start controller */

    if( LinkFlagNo == 0 ) {
		LinkFlagNo = acre_flg(&cflg);
	}
#if NTRACE >= 1
    Nprintf("SH7615 %02x%02x%02x%02x%02x%02x IR%d ", netp->id.c[0],
        netp->id.c[1], netp->id.c[2], netp->id.c[3], netp->id.c[4],
        netp->id.c[5], netp->irno[0]); 
    if (LinkStatus & PHYSTS_LNKSTS) {
        if (LinkStatus & PHYSTS_SPEED10) Nprintf("10M ");
        else                             Nprintf("100M ");
        if (LinkStatus & PHYSTS_DUPLEX)  Nprintf("Full Duplex\n");
        else                             Nprintf("Half Duplex\n");
    } else {
        Nprintf("Link down\n");
    	return -1;
	}
#endif
	LastLinkStatus = LinkStatus;
	ResetLinkStatus = LinkStatus;
	EthrtC_init_flag=1;
	return 0;
}


/* ==========================================================================
   Shut down the Ethernet interface.  Restores original IRQ, mask and vector.
   Turns off the controller. */

static void shut(int netno)
{
    struct NET *netp;

    netp = &nets[netno];
    _outl(EDMR,0x01);				/* reset EtherC and E-DMAC */
    IRrestore(netp->irno[0]);
    netp->protoc[2]->shut(netno);
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
PTABLE ussSH7615Table = {"SH7615", init, shut, 0, opeN, closE, 0, writE,  ioctl, 0, MESSH_SZ};
