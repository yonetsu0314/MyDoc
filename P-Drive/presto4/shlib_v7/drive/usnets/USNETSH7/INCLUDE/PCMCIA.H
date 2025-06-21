/*---------------------------------------------------------------------
 * $Header: p:/presto/shlib_v7/drive/usnets/usnetsh7/include/RCS/pcmcia.h 1.1 1970/01/01 00:00:00 masahiro Exp $
 * $Log: pcmcia.h $
 * リビジョン 1.1  1970/01/01  00:00:00  masahiro
 * 初期リビジョン
 * 
 * 
*----------------------------------------------------------------------
*		(C) Copyright 2002, ISHIDA CO., LTD.                           
*		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                       
*		PHONE (077) 553-4141                                           
*----------------------------------------------------------------------*/


/* +++=>>> * %n  Edition: %v  Last changed: %f  By: %w */
/* PCMCIA.H  Edition: 2  Last changed: 17-Feb-98,15:03:42  By: BILLK */
/* +++=>>> '%l' */
/* Currently extracted for edit by: '***_NOBODY_***' */
/*
    PCMCIA.H -- PCMCIA Definitions for USNET

    Copyright (C) 1994 By
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


/* various codes=============================================================*/

#define CONFIGURATION_LOCKED 0x1d
#define IRQ_HIGH   0x40
#define IRQ_ENABLE 0x80
#define IF_IO      0x02
#define WS_IO      0x01         /* SS window parameters */
#define WS_ENABLED 0x02
#define WS_16BIT   0x04


/* socket service structure definitions======================================*/

struct SSINFO {                 /* socket services information */
    short           blen,
                    len;
    short           caps;
    long            ahi,
                    alo;
};

struct ADAPINFO {               /* adapter information */
    short           BufferLength,
                    DataLength,
                    AdpCaps;
    long            ActiveHighc,
                    ActiveLow;
    short           NumPwrEntries;
    struct {
        char            PowerLevel;
        char            ValidSignals;
    }               PwrEntry[12];
};


/* card service structure definitions========================================*/

struct CSINFO {                 /* card services information */
    short           len;
    char            sign[2];
    short           nsocks,
                    rev,
                    level,
                    voff,
                    vlen;
    char            vendor[80];
};

struct REGINFO {                /* registration information */
    short           Attributes,
                    EventMask;
    char            ClientData[8];
    short           Version;
};

struct CONFINFO {               /* configuration information */
    short           Socket,
                    Attributes;
    char            Vcc,
                    Vpp1,
                    Vpp2,
                    IntType;
    long            ConfigBase;
    char            Status,
                    Pin,
                    Copy,
                    Option,
                    Present,
                    FirstDevType,
                    FuncCode,
                    SysInitMask;
    short           ManufCode,
                    ManufInfo;
    char            CardValues,
                    AssignedIRQ;
    short           IRQAttributes,
                    BasePort1;
    char            NumPorts1,
                    Attributes1;
    short           BasePort2;
    char            NumPorts2,
                    Attributes2,
                    IOAddrLines;
};

struct IOINFO {                 /* request I/O information */
    short           Socket,
                    BasePort1;
    char            NumPorts1,
                    Attributes1;
    short           BasePort2;
    char            NumPorts2,
                    Attributes2,
                    IOAddrLines;
};

struct IRQINFO {                /* request IRQ information */
    short           Socket,
                    Attributes;
    char            AssignedIRQ,
                    IRQInfo1;
    short           IRQInfo2;
};

struct ADJRESINFO {             /* resource info structure */
    char            Action,
                    Resource;
    union {                     /* I/O port resource */
        struct {
            short           BasePort;
            char            NumPorts,
                            Attributes,
                            IOAddrLines;
        }               ports;
        struct {                /* IRQ resource */
            char            Attributes,
                            IRQ;
        }               irq;
    }               un;
};

struct REQINFO {                /* request configuration structure */
    short           Socket,
                    Attributes;
    char            Vcc,
                    Vpp1,
                    Vpp2,
                    IntType;
    long            ConfigBase;
    char            Status,
                    Pin,
                    Copy,
                    ConfigIndex,
                    Present;
};

struct GETTUPLE {               /* Get Tuple structure */
    short           Socket,
                    Attributes;
    char            DesiredTuple,
                    null;
    short           Flags;
    long            LinkOffset,
                    CISOffset;
    char            TupleCode,
                    TupleLink;
};

struct TUPLEDATA {              /* Tuple data structure */
    short           Socket,
                    Attributes;
    char            DesiredTuple,
                    TupleOffset;
    short           Flags;
    long            LinkOffset,
                    CISOffset;
    short           TupleDataMax,
                    TupleDataLen;
    char            TupleData[32];
};


/* function definitions======================================================*/

int             GetAdapterCount(char *noadpts, int *id);
int             GetSSInfo(int adno, char vers[], char *noadpts, char *firstad);
int             InquireAdapter(int adptr, struct ADAPINFO * adapinfo,
                                               char *f1, char *f2, int *f3);
int             InquireSocket(int adptr, int sock, struct SSINFO * ssinfo,
                                              char *f1, char *f2, char *f3);
int             SetSocket(int adptr, int sock, int mask, int vcc, int vpp, int state,
                                          int ctlind, int irq);
int             GetWindow(int adptr, int window, char *sock, int *size, char *state,
                                          char *speed, int *base);
int             SetWindow(int adptr, int window, int sock, int size, int state,
                                          int speed, int base);
int             CardServices(int opcode, int *handle, void *ptr, int arglen, void *arg);

#define GetConfigurationInfo(handle, alen, argp) \
	CardServices(0x04, handle, 0, alen, argp)
#define GetFirstTuple(alen, argp) \
	CardServices(0x07, 0, 0, alen, argp)
#define GetNextTuple(alen, argp) \
	CardServices(0x0a, 0, 0, alen, argp)
#define GetCardServicesInfo(alen, argp) \
	CardServices(0x0b, 0, 0, alen, argp)
#define GetTupleData(alen, argp) \
	CardServices(0x0d, 0, 0, alen, argp)
#define RegisterClient(handle, alen, argp) \
	CardServices(0x10, handle, 0, alen, argp)
#define ReleaseIO(handle, alen, argp) \
	CardServices(0x1b, handle, 0, alen, argp)
#define ReleaseIRQ(handle, alen, argp) \
	CardServices(0x1c, handle, 0, alen, argp)
#define ReleaseConfiguration(handle, alen, argp) \
	CardServices(0x1e, handle, 0, alen, argp)
#define RequestIO(handle, alen, argp) \
	CardServices(0x1f, handle, 0, alen, argp)
#define RequestIRQ(handle, alen, argp) \
	CardServices(0x20, handle, 0, alen, argp)
#define RequestConfiguration(handle, alen, argp) \
	CardServices(0x30, handle, 0, alen, argp)
#define AdjustResourceInfo(handle, alen, argp) \
	CardServices(0x35, handle, 0, alen, argp)
