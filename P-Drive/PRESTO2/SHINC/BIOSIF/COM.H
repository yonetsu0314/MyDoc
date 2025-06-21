/*----------------------------------------------------------------------*
 * File name	: com.h		/ com20020 bios prototype		*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/biosif/RCS/com.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: com.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*
 *Header: p:/include/biosif/rcs/com.h 1.2 1998/02/27 14:35:26 unknown
 *Log: com.h
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(COM_H)
#define	COM_H

#include	<custom\typedefs.h>		/* Show xlib		*/


#define	SPKT_MAX	256		/* short packet MAX		*/
#define	LPKT_MAX	512		/* long packet MAX		*/

/*------------------------------------------------------*/
/*	COM20020 I/O register number define		*/
/*							*/
/*------------------------------------------------------*/

enum comreg {
	STATR,				/* status			*/
 	INTMR	= STATR,		/* interrupt mask		*/
 	DIAGNR,				/* diagnostics			*/
 	COMMR	= DIAGNR,		/* command			*/
 	ADRHR,				/* address pointer high		*/
 	ADRLR,				/* address pointer low		*/
 	DATR,				/* data register		*/
	RESERVED,
 	CONFGR,				/* configuration register	*/
	R7,				/* extended register		*/
	TIDR	= R7,			/* tend-id			*/
	NIDR	= R7,			/* node-id			*/
	SETUPR	= R7,			/* setup			*/
	NXTIDR	= R7,			/* next-id			*/
};

/*------------------------------------------------------*/
/*	COM20020 control data define			*/
/*							*/
/*------------------------------------------------------*/

/*	<<< STATR/INTMR >>>		*/
#define	RI		(BYTE)0x80	/* Receiver Inhibit		*/
#define	POR		(BYTE)0x10	/* Power on reset		*/
#define	TEST		(BYTE)0x08	/* Test				*/
#define	EXCNAK		TEST		/* Excessive NAK		*/
#define	RECON		(BYTE)0x04	/* Reconfiguration		*/
#define	TMA		(BYTE)0x02	/* Tranmitter message ack	*/
#define	NEW_NXTID	TMA		/* New next ID			*/
#define	TA		(BYTE)0x01	/* Tranmitter available		*/

#define	ALL_MASK	(RI|EXCNAK|RECON|NEW_NXTID|TA)

/*	<<< DIAGNR >>>			*/
#define	MY_RECON	(BYTE)0x80	/* My Reconfiguration		*/
#define	DUPID		(BYTE)0x40	/* Duplicate ID			*/
#define	RCVACT		(BYTE)0x20	/* Receive activity		*/
#define	TOKEN		(BYTE)0x10	/* Token seen			*/
#define	TENTID		(BYTE)0x04	/* Tentative ID			*/

/*	<<< COMMR >>>			*/
#define	CLRTX_PEND	(BYTE)0x01	/* Disable transmitter		*/
#define	CLRRX_PEND	(BYTE)0x02	/* Disable receiver		*/
#define	SHORT_PKT	(BYTE)0x05	/* Short packet			*/
#define	LONG_PKT	(BYTE)0x0d	/* Long or short packet		*/
#define	CLR_POR		(BYTE)0x0e	/* Reset POR & EXCNAK flag	*/
#define	CLR_RECON	(BYTE)0x16	/* Clear RECON flag		*/
#define	ENTX		(BYTE)0x03	/* Enable transmit to page fnn	*/
#define	ENRX		(BYTE)0x84	/* Enable receive to page fnn	*/
#define	PFNN(pg)			/* page	=> fnn	| ENTX/ENRX	*/\
	((BYTE)(((pg<<2) & 0x18)|((pg<<5) & 0x20)))

/*	<<< ADRHR >>>			*/
#define	RDDATA		(BYTE)0x80	/* Read data			*/
#define	AUTOINC		(BYTE)0x40	/* Auto increment		*/
#define	ADRH_MAX	(BYTE)0x07	/* A10-A8 all bit 1		*/
#define	ADRL_MAX	(BYTE)0xff	/* A7-A0 all bit 1		*/

/*	<<< DATR >>>			*/
#define	CORE_STA	(BYTE)0xd1	/* Core start			*/

/*	<<< CONFGR >>>			*/
#define	RESET		(BYTE)0x80	/* Read data			*/
#define	TXEN		(BYTE)0x20	/* Tranmit enable		*/
#define	ET12		(BYTE)0x18	/* Extended timeout 1,2		*/
#define	BCKPLN		(BYTE)0x04	/* back plane mode		*/
#define	SUBAD		(BYTE)0x03	/* Sub address			*/

#define	TENTIDA		(BYTE)0x00	/* Tentative ID Sub address	*/
#define	NODEIDA		(BYTE)0x01	/* Node ID Sub address		*/
#define	SETUP		(BYTE)0x02	/* Setup Sub address		*/
#define	NXTID		(BYTE)0x03	/* Next ID Sub address		*/

/*----------------------------------------------------------------------*/
/*	<<< return parameter >>>					*/

typedef struct compinf {
	unsigned int	io_adr;		/* id i/o address		*/
} COMPINF;

typedef struct	com_ctxt {
	unsigned int	ipfw;		/* ICUのIPFWﾎﾟｰﾄ･ｱﾄﾞﾚｽ		*/
	unsigned int	iobase;		/* I/Oｱﾄﾞﾚｽ			*/
	unsigned char	reg[7];		/* I/Oﾚｼﾞｽﾀ			*/
	unsigned char	diagr;
} COM_CTXT;

const COMPINF *		_arcnet_init(int);
/*----------------------------------------------------------------------*/

int	_arcnet_outp(struct com_ctxt *, enum comreg, unsigned char);
int	_arcnet_outp2(struct com_ctxt *, enum comreg, unsigned short);
int	_arcnet_inp(struct com_ctxt *, enum comreg);
long	_arcnet_inp2(struct com_ctxt *, enum comreg);
int	_arcnet_outpb(struct com_ctxt *, enum comreg, unsigned char,
							unsigned char);

#endif	/* COM_H */

