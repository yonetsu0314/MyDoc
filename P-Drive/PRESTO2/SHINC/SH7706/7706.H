/*----------------------------------------------------------------------
 * File name	: 7706.h	/ 					
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7706/RCS/7706.h 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: 7706.h $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2002, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *----------------------------------------------------------------------
 * Function								
 *----------------------------------------------------------------------
 */
/************************************************************************/
/*      SH7706 Include File                                Ver 2.0      */
/************************************************************************/
struct st_sci {                                         /* struct SCI   */
              union {                                   /* SCSMR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CA  :1;        /*    C/A       */
                           unsigned char CHR :1;        /*    CHR       */
                           unsigned char _PE :1;        /*    PE        */
                           unsigned char OE  :1;        /*    O/E       */
                           unsigned char STOP:1;        /*    STOP      */
                           unsigned char MP  :1;        /*    MP        */
                           unsigned char CKS :2;        /*    CKS       */
                           }      BIT;                  /*              */
                    }           SCSMR;                  /*              */
              char              wk1;                    /*              */
              unsigned char     SCBRR;                  /* SCBRR        */
              char              wk2;                    /*              */
              union {                                   /* SCSCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TIE :1;        /*    TIE       */
                           unsigned char RIE :1;        /*    RIE       */
                           unsigned char TE  :1;        /*    TE        */
                           unsigned char RE  :1;        /*    RE        */
                           unsigned char MPIE:1;        /*    MPIE      */
                           unsigned char TEIE:1;        /*    TEIE      */
                           unsigned char CKE :2;        /*    CKE       */
                           }      BIT;                  /*              */
                    }           SCSCR;                  /*              */
              char              wk3;                    /*              */
              unsigned char     SCTDR;                  /* SCTDR        */
              char              wk4;                    /*              */
              union {                                   /* SCSSR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TDRE:1;        /*    TDRE      */
                           unsigned char RDRF:1;        /*    RDRF      */
                           unsigned char ORER:1;        /*    ORER      */
                           unsigned char FER :1;        /*    FER       */
                           unsigned char PER :1;        /*    PER       */
                           unsigned char TEND:1;        /*    TEND      */
                           unsigned char MPB :1;        /*    MPB       */
                           unsigned char MPBT:1;        /*    MPBT      */
                           }      BIT;                  /*              */
                    }           SCSSR;                  /*              */
              char              wk5;                    /*              */
              unsigned char     SCRDR;                  /* SCRDR        */
              char              wk6;                    /*              */
              union {                                   /* SCSCMR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :4;        /*              */
                           unsigned char SDIR:1;        /*    SDIR      */
                           unsigned char SINV:1;        /*    SINV      */
                           unsigned char     :1;        /*              */
                           unsigned char SMIF:1;        /*    SMIF      */
                           }      BIT;                  /*              */
                    }           SCSCMR;                 /*              */
};                                                      /*              */
struct st_smci {                                        /* struct SMCI  */
               union {                                  /* SCSMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CA  :1;       /*    C/A       */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char _PE :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char MP  :1;       /*    MP        */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SCSMR;                  /*              */
               char             wk1;                    /*              */
               unsigned char    SCBRR;                  /* SCBRR        */
               char             wk2;                    /*              */
               union {                                  /* SCSCR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE :1;       /*    TIE       */
                            unsigned char RIE :1;       /*    RIE       */
                            unsigned char TE  :1;       /*    TE        */
                            unsigned char RE  :1;       /*    RE        */
                            unsigned char MPIE:1;       /*    MPIE      */
                            unsigned char TEIE:1;       /*    TEIE      */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3;                    /*              */
               unsigned char    SCTDR;                  /* SCTDR        */
               char             wk4;                    /*              */
               union {                                  /* SCSSR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TDRE:1;       /*    TDRE      */
                            unsigned char RDRF:1;       /*    RDRF      */
                            unsigned char ORER:1;       /*    ORER      */
                            unsigned char ERS :1;       /*    ERS       */
                            unsigned char PER :1;       /*    PER       */
                            unsigned char TEND:1;       /*    TEND      */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            }      BIT;                 /*              */
                     }          SCSSR;                  /*              */
               char             wk5;                    /*              */
               unsigned char    SCRDR;                  /* SCRDR        */
               char             wk6;                    /*              */
               union {                                  /* SCSCMR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :4;       /*              */
                            unsigned char SDIR:1;       /*    SDIR      */
                            unsigned char SINV:1;       /*    SINV      */
                            unsigned char     :1;       /*              */
                            unsigned char SMIF:1;       /*    SMIF      */
                            }      BIT;                 /*              */
                     }          SCSCMR;                 /*              */
};                                                      /*              */
struct st_scif {                                        /* struct SCIF  */
               union {                                  /* SCSMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :1;       /*              */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char _PE :1;       /*    PE        */
                            unsigned char OE  :1;       /*    O/E       */
                            unsigned char STOP:1;       /*    STOP      */
                            unsigned char     :1;       /*              */
                            unsigned char CKS :2;       /*    CKS       */
                            }      BIT;                 /*              */
                     }          SCSMR;                  /*              */
               char             wk1;                    /*              */
               unsigned char    SCBRR;                  /* SCBRR        */
               char             wk2;                    /*              */
               union {                                  /* SCSCR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TIE:1;        /*    TIE       */
                            unsigned char RIE:1;        /*    RIE       */
                            unsigned char TE :1;        /*    TE        */
                            unsigned char RE :1;        /*    RE        */
                            unsigned char    :2;        /*              */
                            unsigned char CKE:2;        /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3;                    /*              */
               unsigned char    SCFTDR;                 /* SCFTDR       */
               char             wk4;                    /*              */
               union {                                  /* SCSSR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short PERC:4;      /*    PER3-0    */
                            unsigned short FERC:4;      /*    FER3-0    */
                            unsigned short ER  :1;      /*    ER        */
                            unsigned short TEND:1;      /*    TEND      */
                            unsigned short TDFE:1;      /*    TDFE      */
                            unsigned short BRK :1;      /*    BRK       */
                            unsigned short FER :1;      /*    FER       */
                            unsigned short PER :1;      /*    PER       */
                            unsigned short RDF :1;      /*    RDF       */
                            unsigned short DR  :1;      /*    DR        */
                            }       BIT;                /*              */
                     }          SCSSR;                  /*              */
               unsigned char    SCFRDR;                 /* SCFRDR       */
               char             wk5;                    /*              */
               union {                                  /* SCFCR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char RTRG :2;      /*    RTRG      */
                            unsigned char TTRG :2;      /*    TTRG      */
                            unsigned char MCE  :1;      /*    MCE       */
                            unsigned char TFRST:1;      /*    TFRST     */
                            unsigned char RFRST:1;      /*    RFRST     */
                            unsigned char LOOP :1;      /*    LOOP      */
                            }      BIT;                 /*              */
                     }          SCFCR;                  /*              */
               char             wk6;                    /*              */
               union {                                  /* SCFDR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short  :3;         /*              */
                            unsigned short T:5;         /*    T         */
                            unsigned short  :3;         /*              */
                            unsigned short R:5;         /*    R         */
                            }       BIT;                /*              */
                     }          SCFDR;                  /*              */
};                                                      /*              */
struct st_tmu {                                         /* struct TMU   */
              union {                                   /* TOCR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :7;        /*              */
                           unsigned char TCOE:1;        /*    TCOE      */
                           }      BIT;                  /*              */
                    }           TOCR;                   /*              */
              char              wk;                     /*              */
              union {                                   /* TSTR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :5;        /*              */
                           unsigned char STR2:1;        /*    STR2      */
                           unsigned char STR1:1;        /*    STR1      */
                           unsigned char STR0:1;        /*    STR0      */
                           }      BIT;                  /*              */
                    }           TSTR;                   /*              */
};                                                      /*              */
struct st_tmu0 {                                        /* struct TMU0  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :7;      /*              */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short     :2;      /*              */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
};                                                      /*              */
struct st_tmu2 {                                        /* struct TMU2  */
               unsigned int     TCOR;                   /* TCOR         */
               unsigned int     TCNT;                   /* TCNT         */
               union {                                  /* TCR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short     :6;      /*              */
                            unsigned short ICPF:1;      /*    ICPF      */
                            unsigned short UNF :1;      /*    UNF       */
                            unsigned short ICPE:2;      /*    ICPE      */
                            unsigned short UNIE:1;      /*    UNIE      */
                            unsigned short CKEG:2;      /*    CKEG      */
                            unsigned short TPSC:3;      /*    TPSC      */
                            }       BIT;                /*              */
                     }          TCR;                    /*              */
               char             wk[2];                  /*              */
               unsigned int     TCPR;                   /* TCPR         */
};                                                      /*              */
struct st_rtc {                                         /* struct RTC   */
              unsigned char     R64CNT;                 /* R64CNT       */
              char              wk1;                    /*              */
              union {                                   /* RSECCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :1;         /*              */
                           unsigned char S10:3;         /*    10sec     */
                           unsigned char S1 :4;         /*     1sec     */
                           }      BIT;                  /*              */
                    }           RSECCNT;                /*              */
              char              wk2;                    /*              */
              union {                                   /* RMINCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :1;         /*              */
                           unsigned char M10:3;         /*    10min     */
                           unsigned char M1 :4;         /*     1min     */
                           }      BIT;                  /*              */
                    }           RMINCNT;                /*              */
              char              wk3;                    /*              */
              union {                                   /* RHRCNT       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :2;         /*              */
                           unsigned char H10:2;         /*    10sec     */
                           unsigned char H1 :4;         /*     1sec     */
                           }      BIT;                  /*              */
                    }           RHRCNT;                 /*              */
              char              wk4;                    /*              */
              union {                                   /* RWKCNT       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :5;          /*              */
                           unsigned char WK:3;          /*    week      */
                           }      BIT;                  /*              */
                    }           RWKCNT;                 /*              */
              char              wk5;                    /*              */
              union {                                   /* RDAYCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :2;         /*              */
                           unsigned char D10:2;         /*    10day     */
                           unsigned char D1 :4;         /*     1day     */
                           }      BIT;                  /*              */
                    }           RDAYCNT;                /*              */
              char              wk6;                    /*              */
              union {                                   /* RMONCNT      */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :3;         /*              */
                           unsigned char M10:1;         /*    10mon     */
                           unsigned char M1 :4;         /*     1mon     */
                           }      BIT;                  /*              */
                    }          RMONCNT;                 /*              */
              char             wk7;                     /*              */
              union {                                   /* RYRCNT       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char Y10:4;         /*    10year    */
                           unsigned char Y1 :4;         /*     1year    */
                           }      BIT;                  /*              */
                    }           RYRCNT;                 /*              */
              char              wk8;                    /*              */
              union {                                   /* RSECAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char S10:3;         /*    10sec     */
                           unsigned char S1 :4;         /*     1sec     */
                           }      BIT;                  /*              */
                    }           RSECAR;                 /*              */
              char              wk9;                    /*              */
              union {                                   /* RMINAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char M10:3;         /*    10min     */
                           unsigned char M1 :4;         /*     1min     */
                           }      BIT;                  /*              */
                    }           RMINAR;                 /*              */
              char              wk10;                   /*              */
              union {                                   /* RHRAR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char    :1;         /*              */
                           unsigned char H10:2;         /*    10sec     */
                           unsigned char H1 :4;         /*     1sec     */
                           }      BIT;                  /*              */
                    }           RHRAR;                  /*              */
              char              wk11;                   /*              */
              union {                                   /* RWKAR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char    :4;         /*              */
                           unsigned char WK :3;         /*    week      */
                           }      BIT;                  /*              */
                    }           RWKAR;                  /*              */
              char              wk12;                   /*              */
              union {                                   /* RDAYAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char    :1;         /*              */
                           unsigned char D10:2;         /*    10day     */
                           unsigned char D1 :4;         /*     1day     */
                           }      BIT;                  /*              */
                    }           RDAYAR;                 /*              */
              char              wk13;                   /*              */
              union {                                   /* RMONAR       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ENB:1;         /*    ENB       */
                           unsigned char    :2;         /*              */
                           unsigned char M10:1;         /*    10mon     */
                           unsigned char M1 :4;         /*     1mon     */
                           }      BIT;                  /*              */
                    }           RMONAR;                 /*              */
              char              wk14;                   /*              */
              union {                                   /* RCR1         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char CF :1;         /*    CF        */
                           unsigned char    :2;         /*              */
                           unsigned char CIE:1;         /*    CIE       */
                           unsigned char AIE:1;         /*    AIE       */
                           unsigned char    :2;         /*              */
                           unsigned char AF :1;         /*    AF        */
                           }      BIT;                  /*              */
                    }           RCR1;                   /*              */
              char              wk15;                   /*              */
              union {                                   /* RCR2         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char PEF  :1;       /*    PEF       */
                           unsigned char PES  :3;       /*    PES       */
                           unsigned char RTCEN:1;       /*    RTCEN     */
                           unsigned char ADJ  :1;       /*    ADJ       */
                           unsigned char RESET:1;       /*    RESET     */
                           unsigned char START:1;       /*    START     */
                           }      BIT;                  /*              */
                    }           RCR2;                   /*              */
};                                                      /*              */
struct st_intc {                                        /* struct INTC  */
               union {                                  /* ICR0         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short NMIL:1;      /*    NMIL      */
                            unsigned short     :6;      /*              */
                            unsigned short NMIE:1;      /*    NMIE      */
                            }       BIT;                /*              */
                     }          ICR0;                   /*              */
               union {                                  /* IPRA         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _TMU0:4;     /*    TMU0      */
                            unsigned short _TMU1:4;     /*    TMU1      */
                            unsigned short _TMU2:4;     /*    TMU2      */
                            unsigned short _RTC :4;     /*    RTC       */
                            }       BIT;                /*              */
                     }          IPRA;                   /*              */
               union {                                  /* IPRB         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _WDT:4;      /*    WDT       */
                            unsigned short _REF:4;      /*    REF       */
                            unsigned short _SCI:4;      /*    SCI       */
                            }       BIT;                /*              */
                     }          IPRB;                   /*              */
               char             wk[234];                /*              */
               unsigned int     TRA;                    /* TRA          */
               unsigned int     EXPEVT;                 /* EXPEVT       */
               unsigned int     INTEVT;                 /* INTEVT       */
};                                                      /*              */
struct st_intx {                                        /* struct INTX  */
               unsigned int     INTEVT2;                /* INTEVT2      */
               union {                                  /* IRR0         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :2;      /*              */
                            unsigned char IRQ5R:1;      /*    IRQ5R     */
                            unsigned char IRQ4R:1;      /*    IRQ4R     */
                            unsigned char IRQ3R:1;      /*    IRQ3R     */
                            unsigned char IRQ2R:1;      /*    IRQ2R     */
                            unsigned char IRQ1R:1;      /*    IRQ1R     */
                            unsigned char IRQ0R:1;      /*    IRQ0R     */
                            }      BIT;                 /*              */
                     }          IRR0;                   /*              */
               char             wk1;                    /*              */
               union {                                  /* IRR1         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :4;      /*              */
                            unsigned char DEI3R:1;      /*    DEI3R     */
                            unsigned char DEI2R:1;      /*    DEI2R     */
                            unsigned char DEI1R:1;      /*    DEI1R     */
                            unsigned char DEI0R:1;      /*    DEI0R     */
                            }      BIT;                 /*              */
                     }          IRR1;                   /*              */
               char             wk2;                    /*              */
               union {                                  /* IRR2         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :3;      /*              */
                            unsigned char ADIR :1;      /*    ADIR      */
                            unsigned char TXI2R:1;      /*    TXI2R     */
                            unsigned char BRI2R:1;      /*    BRI2R     */
                            unsigned char RXI2R:1;      /*    RXI2R     */
                            unsigned char ERI2R:1;      /*    ERI2R     */
                            }      BIT;                 /*              */
                     }          IRR2;                   /*              */
               char             wk3[7];                 /*              */
               union {                                  /* ICR1         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short MAI   :1;    /*    MAI       */
                            unsigned short IRQLVL:1;    /*    IRQLVL    */
                            unsigned short BLMSK :1;    /*    BLMSK     */
                            unsigned short       :1;    /*              */
                            unsigned short IRQ5S :2;    /*    IRQ5S     */
                            unsigned short IRQ4S :2;    /*    IRQ4S     */
                            unsigned short IRQ3S :2;    /*    IRQ3S     */
                            unsigned short IRQ2S :2;    /*    IRQ2S     */
                            unsigned short IRQ1S :2;    /*    IRQ1S     */
                            unsigned short IRQ0S :2;    /*    IRQ0S     */
                            }       BIT;                /*              */
                     }          ICR1;                   /*              */
               char             wk4[4];                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _IRQ3:4;     /*    IRQ3      */
                            unsigned short _IRQ2:4;     /*    IRQ2      */
                            unsigned short _IRQ1:4;     /*    IRQ1      */
                            unsigned short _IRQ0:4;     /*    IRQ0      */
                            }       BIT;                /*              */
                     }          IPRC;                   /*              */
               union {                                  /* IPRD         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short      :8;     /*              */
                            unsigned short _IRQ5:4;     /*    IRQ5      */
                            unsigned short _IRQ4:4;     /*    IRQ4      */
                            }       BIT;                /*              */
                     }          IPRD;                   /*              */
               union {                                  /* IPRE         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short _DMAC:4;     /*    DMAC      */
                            unsigned short      :4;     /*              */
                            unsigned short _SCIF:4;     /*    SCIF      */
                            unsigned short _ADC :4;     /*    ADC       */
                            }       BIT;                /*              */
                     }          IPRE;                   /*              */
};                                                      /*              */
struct st_bsc {                                         /* struct BSC   */
              union {                                   /* BCR1         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PULA  :1;     /*    PULA      */
                           unsigned short PULD  :1;     /*    PULD      */
                           unsigned short HIZMEM:1;     /*    HIZMEM    */
                           unsigned short HIZCNT:1;     /*    HIZCNT    */
                           unsigned short ENDIAN:1;     /*    ENDIAN    */
                           unsigned short A0BST :2;     /*    A0BST     */
                           unsigned short A5BST :2;     /*    A5BST     */
                           unsigned short A6BST :2;     /*    A6BST     */
                           unsigned short DRAMTP:3;     /*    DRAMTP    */
                           unsigned short A5PCM :1;     /*    A5PCM     */
                           unsigned short A6PCM :1;     /*    A6PCM     */
                           }       BIT;                 /*              */
                    }           BCR1;                   /*              */
              union {                                   /* BCR2         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :2;       /*              */
                           unsigned short A6SZ:2;       /*    A6SZ      */
                           unsigned short A5SZ:2;       /*    A5SZ      */
                           unsigned short A4SZ:2;       /*    A4SZ      */
                           unsigned short A3SZ:2;       /*    A3SZ      */
                           unsigned short A2SZ:2;       /*    A2SZ      */
                           }       BIT;                 /*              */
                    }           BCR2;                   /*              */
              union {                                   /* WCR1         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short WAITSEL:1;    /*    WAITSEL   */
                           unsigned short        :1;    /*              */
                           unsigned short A6IW   :2;    /*    A6IW      */
                           unsigned short A5IW   :2;    /*    A5IW      */
                           unsigned short A4IW   :2;    /*    A4IW      */
                           unsigned short A3IW   :2;    /*    A3IW      */
                           unsigned short A2IW   :2;    /*    A2IW      */
                           unsigned short        :2;    /*              */
                           unsigned short A0IW   :2;    /*    A0IW      */
                           }       BIT;                 /*              */
                    }           WCR1;                   /*              */
              union {                                   /* WCR2         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short A6W:3;        /*    A6W       */
                           unsigned short A5W:3;        /*    A5W       */
                           unsigned short A4W:3;        /*    A4W       */
                           unsigned short A3W:2;        /*    A3W       */
                           unsigned short A2W:2;        /*    A2W       */
                           unsigned short A0W:3;        /*    A0W       */
                           }       BIT;                 /*              */
                    }           WCR2;                   /*              */
              union {                                   /* MCR          */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short TPC    :2;    /*    TPC       */
                           unsigned short RCD    :2;    /*    RCD       */
                           unsigned short TRWL   :2;    /*    TRWL      */
                           unsigned short TRAS   :2;    /*    TRAS      */
                           unsigned short RASD   :1;    /*    RASD      */
                           unsigned short AMX    :4;    /*    AMX       */
                           unsigned short RFSH   :1;    /*    RFSH      */
                           unsigned short RMODE  :1;    /*    RMODE     */
                           unsigned short EDOMODE:1;    /*    EDOMODE   */
                           }       BIT;                 /*              */
                    }           MCR;                    /*              */
              char              wk[2];                  /*              */
              union {                                   /* PCR          */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short A6W3  :1;     /*    A6W3      */
                           unsigned short A5W3  :1;     /*    A5W3      */
                           unsigned short       :2;     /*              */
                           unsigned short A5TED2:1;     /*    A5TED2    */
                           unsigned short A6TED2:1;     /*    A6TED2    */
                           unsigned short A5TEH2:1;     /*    A5TEH2    */
                           unsigned short A6TEH2:1;     /*    A6TEH2    */
                           unsigned short A5TED1:1;     /*    A5TED1    */
                           unsigned short A5TED0:1;     /*    A5TED0    */
                           unsigned short A6TED1:1;     /*    A6TED1    */
                           unsigned short A6TED0:1;     /*    A6TED0    */
                           unsigned short A5TEH1:1;     /*    A5TEH1    */
                           unsigned short A5TEH0:1;     /*    A5TEH0    */
                           unsigned short A6TEH1:1;     /*    A6TEH1    */
                           unsigned short A6TEH0:1;     /*    A6TEH0    */
                           }       BIT;                 /*              */
                    }           PCR;                    /*              */
              union {                                   /* RTCSR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :8;       /*              */
                           unsigned short CMF :1;       /*    CMF       */
                           unsigned short CMIE:1;       /*    CMIE      */
                           unsigned short CKS :3;       /*    CKS       */
                           unsigned short OVF :1;       /*    OVF       */
                           unsigned short OVIE:1;       /*    OVIE      */
                           unsigned short LMTS:1;       /*    LMTS      */
                           }       BIT;                 /*              */
                    }           RTCSR;                  /*              */
              unsigned short    RTCNT;                  /* RTCNT        */
              unsigned short    RTCOR;                  /* RTCOR        */
              unsigned short    RFCR;                   /* RFCR         */
};                                                      /*              */
struct st_cpg {                                         /* struct CPG   */
              union {                                   /* FRQCR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short STC2:1;       /*    STC2      */
                           unsigned short IFC2:1;       /*    IFC2      */
                           unsigned short PFC2:1;       /*    PFC2      */
                           unsigned short     :7;       /*              */
                           unsigned short STC :2;       /*    STC       */
                           unsigned short IFC :2;       /*    IFC       */
                           unsigned short _PFC:2;       /*    PFC       */
                           }       BIT;                 /*              */
                    }           FRQCR;                  /*              */
              char              wk[2];                  /*              */
              union {                                   /* WTCNT        */
                    unsigned char  READ;                /*  Read  Access*/
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCNT;                  /*              */
              union {                                   /* WTCSR        */
                    union {                             /*  Read  Access*/
                          unsigned char BYTE;           /*   Byte Access*/
                          struct {                      /*   Bit  Access*/
                                 unsigned char TME :1;  /*    TME       */
                                 unsigned char WTIT:1;  /*    WT/IT     */
                                 unsigned char RSTS:1;  /*    RSTS      */
                                 unsigned char WOVF:1;  /*    WOVF      */
                                 unsigned char IOVF:1;  /*    IOVF      */
                                 unsigned char CKS :3;  /*    CKS       */
                                 }      BIT;            /*              */
                         }         READ;                /*              */
                    unsigned short WRITE;               /*  Write Access*/
                    }           WTCSR;                  /*              */
};                                                      /*              */
struct st_stb {                                         /* struct STB   */
              union {                                   /* STBCR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char STBY  :1;      /*    STBY      */
                           unsigned char       :2;      /*              */
                           unsigned char STBXTL:1;      /*    STBXTL    */
                           unsigned char       :1;      /*              */
                           unsigned char _TMU  :1;      /*    TMU       */
                           unsigned char _RTC  :1;      /*    RTC       */
                           unsigned char _SCI  :1;      /*    SCI       */
                           }      BIT;                  /*              */
                    }           CR;                     /*              */
              char              wk[5];                  /*              */
              union {                                   /* STBCR2       */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :1;       /*              */
                           unsigned char MDCHG:1;       /*    MDCHG     */
                           unsigned char _UBC :1;       /*    UBC       */
                           unsigned char _DMAC:1;       /*    DMAC      */
                           unsigned char _DAC :1;       /*    DAC       */
                           unsigned char _ADC :1;       /*    ADC       */
                           unsigned char _SCIF:1;       /*    SCIF      */
                           }      BIT;                  /*              */
                    }           CR2;                    /*              */
};                                                      /*              */
struct st_ubc {                                         /* struct UBC   */
              unsigned int      BDRB;                   /* BDRB         */
              unsigned int      BDMRB;                  /* BDMRB        */
              union {                                   /* BRCR         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short       :10;    /*              */
                           unsigned short BASMA : 1;    /*    BASMA     */
                           unsigned short BASMB : 1;    /*    BASMB     */
                           unsigned short       : 4;    /*              */
                           unsigned short SCMFCA: 1;    /*    SCMCA     */
                           unsigned short SCMFCB: 1;    /*    SCMCB     */
                           unsigned short SCMFDA: 1;    /*    SCMDA     */
                           unsigned short SCMFDB: 1;    /*    SCMDB     */
                           unsigned short PCTE  : 1;    /*    PCTE      */
                           unsigned short PCBA  : 1;    /*    PCTA      */
                           unsigned short       : 2;    /*              */
                           unsigned short DBEB  : 1;    /*    DBEB      */
                           unsigned short PCBB  : 1;    /*    PCBB      */
                           unsigned short       : 2;    /*              */
                           unsigned short SEQ   : 1;    /*    SEQ       */
                           unsigned short       : 2;    /*              */
                           unsigned short ETBE  : 1;    /*    ETBE      */
                           }     BIT;                   /*              */
                    }           BRCR;                   /*              */
              unsigned short    BETR;                   /* BETR         */
              char              wk1[2];                 /*              */
              void             *BARB;                   /* BARB         */
              unsigned int      BAMRB;                  /* BAMRB        */
              union {                                   /* BBRB         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short    :8;        /*              */
                           unsigned short CDB:2;        /*    CDB       */
                           unsigned short IDB:2;        /*    IDB       */
                           unsigned short RWB:2;        /*    RWB       */
                           unsigned short SZB:2;        /*    SZB       */
                           }       BIT;                 /*              */
                    }           BBRB;                   /*              */
              char              wk2[2];                 /*              */
              union {                                   /* BRSR         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int SVF: 1;         /*    SVF       */
                           unsigned int PID: 3;         /*    PID       */
                           unsigned int BSA:28;         /*    BSA       */
                           }     BIT;                   /*              */
                    }           BRSR;                   /*              */
              void             *BARA;                   /* BARA         */
              unsigned int      BAMRA;                  /* BAMRA        */
              union {                                   /* BBRA         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short    :8;        /*              */
                           unsigned short CDA:2;        /*    CDA       */
                           unsigned short IDA:2;        /*    IDA       */
                           unsigned short RWA:2;        /*    RWA       */
                           unsigned short SZA:2;        /*    SZA       */
                           }       BIT;                 /*              */
                    }           BBRA;                   /*              */
              char              wk3[2];                 /*              */
              union {                                   /* BRDR         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned int DVF: 1;         /*    DVF       */
                           unsigned int    : 3;         /*              */
                           unsigned int BDA:28;         /*    BDA       */
                           }     BIT;                   /*              */
                    }           BRDR;                   /*              */
              char              wk4[36];                /*              */
              unsigned char     BASRA;                  /* BASRA        */
              char              wk5[3];                 /*              */
              unsigned char     BASRB;                  /* BASRB        */
};                                                      /*              */
struct st_mmu {                                         /* struct MMU   */
              unsigned int      MMUCR;                  /* MMUCR        */
              char              wk[12];                 /*              */
              unsigned int      PTEH;                   /* PTEH         */
              unsigned int      PTEL;                   /* PTEL         */
              unsigned int      TTB;                    /* TTB          */
              unsigned int      TEA;                    /* TEA          */
};                                                      /*              */
struct st_dmac {                                        /* struct DMAC  */
               union {                                  /* DMAOR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :6;       /*              */
                            unsigned char PR  :2;       /*    PR        */
                            unsigned char     :5;       /*              */
                            unsigned char AE  :1;       /*    AE        */
                            unsigned char NMIF:1;       /*    NMIF      */
                            unsigned char DME :1;       /*    DME       */
                            }       BIT;                /*              */
                     }          DMAOR;                  /*              */
};                                                      /*              */
struct st_dmac0 {                                       /* struct DMAC0 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Word Access */
                             unsigned short H;          /*    High Byte */
                             unsigned short L;          /*    Low  Byte */
                             }     WORD;                /*              */
                      struct {                          /*  Bit  Access */
                             unsigned char   :8;        /*              */
                             unsigned char   :5;        /*              */
                             unsigned char RL:1;        /*    RL        */
                             unsigned char AM:1;        /*    AM        */
                             unsigned char AL:1;        /*    AL        */
                             unsigned char DM:2;        /*    DM        */
                             unsigned char SM:2;        /*    SM        */
                             unsigned char RS:4;        /*    RS        */
                             unsigned char   :1;        /*              */
                             unsigned char DS:1;        /*    DS        */
                             unsigned char TM:1;        /*    TM        */
                             unsigned char TS:2;        /*    TS        */
                             unsigned char IE:1;        /*    IE        */
                             unsigned char TE:1;        /*    TE        */
                             unsigned char DE:1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_dmac2 {                                       /* struct DMAC2 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Word Access */
                             unsigned short H;          /*    High Byte */
                             unsigned short L;          /*    Low  Byte */
                             }     WORD;                /*              */
                      struct {                          /*  Bit  Access */
                             unsigned char   :8;        /*              */
                             unsigned char   :4;        /*              */
                             unsigned char RO:1;        /*    RO        */
                             unsigned char   :3;        /*              */
                             unsigned char DM:2;        /*    DM        */
                             unsigned char SM:2;        /*    SM        */
                             unsigned char RS:4;        /*    RS        */
                             unsigned char   :2;        /*              */
                             unsigned char TM:1;        /*    TM        */
                             unsigned char TS:2;        /*    TS        */
                             unsigned char IE:1;        /*    IE        */
                             unsigned char TE:1;        /*    TE        */
                             unsigned char DE:1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_dmac3 {                                       /* struct DMAC3 */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    DMATCR;                 /* DMATCR       */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Word Access */
                             unsigned short H;          /*    High Byte */
                             unsigned short L;          /*    Low  Byte */
                             }     WORD;                /*              */
                      struct {                          /*  Bit  Access */
                             unsigned char   :8;        /*              */
                             unsigned char   :3;        /*              */
                             unsigned char DI:1;        /*    DI        */
                             unsigned char   :4;        /*              */
                             unsigned char DM:2;        /*    DM        */
                             unsigned char SM:2;        /*    SM        */
                             unsigned char RS:4;        /*    RS        */
                             unsigned char   :2;        /*              */
                             unsigned char TM:1;        /*    TM        */
                             unsigned char TS:2;        /*    TS        */
                             unsigned char IE:1;        /*    IE        */
                             unsigned char TE:1;        /*    TE        */
                             unsigned char DE:1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_cmt {                                         /* struct CMT   */
              union {                                   /* CMSTR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :8;        /*              */
                           unsigned char     :7;        /*              */
                           unsigned char STR0:1;        /*    STR0      */
                           }       BIT;                 /*              */
                    }           CMSTR;                  /*              */
              union {                                   /* CMCSR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :8;         /*              */
                           unsigned char CMF:1;         /*    CMF       */
                           unsigned char    :5;         /*              */
                           unsigned char CKS:2;         /*    CKS       */
                           }       BIT;                 /*              */
                    }           CMCSR;                  /*              */
              unsigned short    CMCNT;                  /* CMCNT        */
              unsigned short    CMCOR;                  /* CMCOR        */
};                                                      /*              */
struct st_adc {                                         /* struct ADC   */
              unsigned char     ADDRAH;                 /* ADDRAH       */
              char              wk1;                    /*              */
              unsigned char     ADDRAL;                 /* ADDRAL       */
              char              wk2;                    /*              */
              unsigned char     ADDRBH;                 /* ADDRBH       */
              char              wk3;                    /*              */
              unsigned char     ADDRBL;                 /* ADDRBL       */
              char              wk4;                    /*              */
              unsigned char     ADDRCH;                 /* ADDRCH       */
              char              wk5;                    /*              */
              unsigned char     ADDRCL;                 /* ADDRCL       */
              char              wk6;                    /*              */
              unsigned char     ADDRDH;                 /* ADDRDH       */
              char              wk7;                    /*              */
              unsigned char     ADDRDL;                 /* ADDRDL       */
              char              wk8;                    /*              */
              union {                                   /* ADCSR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ADF  :1;       /*    ADF       */
                           unsigned char ADIE :1;       /*    ADIE      */
                           unsigned char ADST :1;       /*    ADST      */
                           unsigned char MULTI:1;       /*    MULTI     */
                           unsigned char CKS  :1;       /*    CKS       */
                           unsigned char CH   :3;       /*    CH        */
                           }      BIT;                  /*              */
                    }           ADCSR;                  /*              */
              char              wk9;                    /*              */
              union {                                   /* ADCR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char TRGE:2;        /*    TRGE      */
                           unsigned char SCN :1;        /*    SCN       */
                           }      BIT;                  /*              */
                    }           ADCR;                   /*              */
};                                                      /*              */
struct st_dac {                                         /* struct DAC   */
              unsigned char     DADR0;                  /* DADR0        */
              char              wk1;                    /*              */
              unsigned char     DADR1;                  /* DADR1        */
              char              wk2;                    /*              */
              union {                                   /* DACR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char DAOE1:1;       /*    DAOE1     */
                           unsigned char DAOE0:1;       /*    DAOE0     */
                           unsigned char DAE  :1;       /*    DAE       */
                           }      BIT;                  /*              */
                    }           DACR;                   /*              */
};                                                      /*              */
struct st_pfc {                                         /* struct PFC   */
              union {                                   /* PACR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PA7MD:2;      /*    PA7MD     */
                           unsigned short PA6MD:2;      /*    PA6MD     */
                           unsigned short PA5MD:2;      /*    PA5MD     */
                           unsigned short PA4MD:2;      /*    PA4MD     */
                           unsigned short PA3MD:2;      /*    PA3MD     */
                           unsigned short PA2MD:2;      /*    PA2MD     */
                           unsigned short PA1MD:2;      /*    PA1MD     */
                           unsigned short PA0MD:2;      /*    PA0MD     */
                           }       BIT;                 /*              */
                    }           PACR;                   /*              */
              union {                                   /* PBCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PB7MD:2;      /*    PB7MD     */
                           unsigned short PB6MD:2;      /*    PB6MD     */
                           unsigned short PB5MD:2;      /*    PB5MD     */
                           unsigned short PB4MD:2;      /*    PB4MD     */
                           unsigned short PB3MD:2;      /*    PB3MD     */
                           unsigned short PB2MD:2;      /*    PB2MD     */
                           unsigned short PB1MD:2;      /*    PB1MD     */
                           unsigned short PB0MD:2;      /*    PB0MD     */
                           }       BIT;                 /*              */
                    }           PBCR;                   /*              */
              union {                                   /* PCCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PC7MD:2;      /*    PC7MD     */
                           unsigned short PC6MD:2;      /*    PC6MD     */
                           unsigned short PC5MD:2;      /*    PC5MD     */
                           unsigned short PC4MD:2;      /*    PC4MD     */
                           unsigned short PC3MD:2;      /*    PC3MD     */
                           unsigned short PC2MD:2;      /*    PC2MD     */
                           unsigned short PC1MD:2;      /*    PC1MD     */
                           unsigned short PC0MD:2;      /*    PC0MD     */
                           }       BIT;                 /*              */
                    }           PCCR;                   /*              */
              union {                                   /* PDCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PD7MD:2;      /*    PD7MD     */
                           unsigned short PD6MD:2;      /*    PD6MD     */
                           unsigned short PD5MD:2;      /*    PD5MD     */
                           unsigned short PD4MD:2;      /*    PD4MD     */
                           unsigned short PD3MD:2;      /*    PD3MD     */
                           unsigned short PD2MD:2;      /*    PD2MD     */
                           unsigned short PD1MD:2;      /*    PD1MD     */
                           unsigned short PD0MD:2;      /*    PD0MD     */
                           }       BIT;                 /*              */
                    }           PDCR;                   /*              */
              union {                                   /* PECR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PE7MD:2;      /*    PE7MD     */
                           unsigned short PE6MD:2;      /*    PE6MD     */
                           unsigned short PE5MD:2;      /*    PE5MD     */
                           unsigned short PE4MD:2;      /*    PE4MD     */
                           unsigned short PE3MD:2;      /*    PE3MD     */
                           unsigned short PE2MD:2;      /*    PE2MD     */
                           unsigned short PE1MD:2;      /*    PE1MD     */
                           unsigned short PE0MD:2;      /*    PE0MD     */
                           }       BIT;                 /*              */
                    }           PECR;                   /*              */
              union {                                   /* PFCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :2;      /*              */
                           unsigned short PF6MD:2;      /*    PF6MD     */
                           unsigned short PF5MD:2;      /*    PF5MD     */
                           unsigned short PF4MD:2;      /*    PF4MD     */
                           unsigned short PF3MD:2;      /*    PF3MD     */
                           unsigned short PF2MD:2;      /*    PF2MD     */
                           unsigned short PF1MD:2;      /*    PF1MD     */
                           unsigned short PF0MD:2;      /*    PF0MD     */
                           }       BIT;                 /*              */
                    }           PFCR;                   /*              */
              union {                                   /* PGCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :4;      /*              */
                           unsigned short PG5MD:2;      /*    PG5MD     */
                           unsigned short PG4MD:2;      /*    PG4MD     */
                           unsigned short PG3MD:2;      /*    PG3MD     */
                           unsigned short PG2MD:2;      /*    PG2MD     */
                           unsigned short PG1MD:2;      /*    PG1MD     */
                           unsigned short PG0MD:2;      /*    PG0MD     */
                           }       BIT;                 /*              */
                    }           PGCR;                   /*              */
              union {                                   /* PHCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :2;      /*              */
                           unsigned short PH6MD:2;      /*    PH6MD     */
                           unsigned short PH5MD:2;      /*    PH5MD     */
                           unsigned short PH4MD:2;      /*    PH4MD     */
                           unsigned short PH3MD:2;      /*    PH3MD     */
                           unsigned short PH2MD:2;      /*    PH2MD     */
                           unsigned short PH1MD:2;      /*    PH1MD     */
                           unsigned short PH0MD:2;      /*    PH0MD     */
                           }       BIT;                 /*              */
                    }           PHCR;                   /*              */
              union {                                   /* PJCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :8;      /*              */
                           unsigned short PJ3MD:2;      /*    PJ3MD     */
                           unsigned short PJ2MD:2;      /*    PJ2MD     */
                           unsigned short PJ1MD:2;      /*    PJ1MD     */
                           unsigned short PJ0MD:2;      /*    PJ0MD     */
                           }       BIT;                 /*              */
                    }           PJCR;                   /*              */
              char              wk[4];                  /*              */
              union {                                   /* SCPCR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short       :4;     /*              */
                           unsigned short SCP5MD:2;     /*    SCP5MD    */
                           unsigned short SCP4MD:2;     /*    SCP4MD    */
                           unsigned short SCP3MD:2;     /*    SCP3MD    */
                           unsigned short SCP2MD:2;     /*    SCP2MD    */
                           unsigned short SCP1MD:2;     /*    SCP1MD    */
                           unsigned short SCP0MD:2;     /*    SCP0MD    */
                           }       BIT;                 /*              */
                    }           SCPCR;                  /*              */
};                                                      /*              */
struct st_pa {                                          /* struct PA    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pb {                                          /* struct PB    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pc {                                          /* struct PC    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pd {                                          /* struct PD    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pe {                                          /* struct PE    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char B7:1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pf {                                          /* struct PF    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pg {                                          /* struct PG    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :2;           /*    Bit 7,6   */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_ph {                                          /* struct PH    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :1;           /*    Bit 7     */
                          unsigned char B6:1;           /*    Bit 6     */
                          unsigned char B5:1;           /*    Bit 5     */
                          unsigned char B4:1;           /*    Bit 4     */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pj {                                          /* struct PJ    */
             union {                                    /*              */
                   unsigned char BYTE;                  /*  Byte Access */
                   struct {                             /*  Bit  Access */
                          unsigned char   :4;           /*    Bit 7-4   */
                          unsigned char B3:1;           /*    Bit 3     */
                          unsigned char B2:1;           /*    Bit 2     */
                          unsigned char B1:1;           /*    Bit 1     */
                          unsigned char B0:1;           /*    Bit 0     */
                          }      BIT;                   /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_scp {                                         /* struct SCP   */
              union {                                   /*              */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char   :2;          /*    Bit 7,6   */
                           unsigned char B5:1;          /*    Bit 5     */
                           unsigned char B4:1;          /*    Bit 4     */
                           unsigned char B3:1;          /*    Bit 3     */
                           unsigned char B2:1;          /*    Bit 2     */
                           unsigned char B1:1;          /*    Bit 1     */
                           unsigned char B0:1;          /*    Bit 0     */
                           }      BIT;                  /*              */
                    }           DR;                     /*              */
};                                                      /*              */
struct st_hudi {                                        /* struct H-UDI */
               union {                                  /* SDIR         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit Access  */
                            unsigned char TI:4;         /*    TI        */
                            }       BIT;                /*              */
                     }          SDIR;                   /*              */
};                                                      /*              */
#define SCI   (*(volatile struct st_sci   *)0xFFFFFE80) /* SCI   Address*/
#define SMCI  (*(volatile struct st_smci  *)0xFFFFFE80) /* SMCI  Address*/
#define SCIF  (*(volatile struct st_scif  *)0xA4000150) /* SCIF  Address*/
#define TMU   (*(volatile struct st_tmu   *)0xFFFFFE90) /* TMU   Address*/
#define TMU0  (*(volatile struct st_tmu0  *)0xFFFFFE94) /* TMU0  Address*/
#define TMU1  (*(volatile struct st_tmu0  *)0xFFFFFEA0) /* TMU1  Address*/
#define TMU2  (*(volatile struct st_tmu2  *)0xFFFFFEAC) /* TMU2  Address*/
#define RTC   (*(volatile struct st_rtc   *)0xFFFFFEC0) /* RTC   Address*/
#define INTC  (*(volatile struct st_intc  *)0xFFFFFEE0) /* INTC  Address*/
#define INTX  (*(volatile struct st_intx  *)0xA4000000) /* INTX  Address*/
#define BSC   (*(volatile struct st_bsc   *)0xFFFFFF60) /* BSC   Address*/
#define CPG   (*(volatile struct st_cpg   *)0xFFFFFF80) /* CPG   Address*/
#define STB   (*(volatile struct st_stb   *)0xFFFFFF82) /* STB   Address*/
#define UBC   (*(volatile struct st_ubc   *)0xFFFFFF90) /* UBC   Address*/
#define MMU   (*(volatile struct st_mmu   *)0xFFFFFFE0) /* MMU   Address*/
#define CCR   (*(volatile unsigned int    *)0xFFFFFFEC) /* CCR   Address*/
#define CCR2  (*(volatile unsigned int    *)0xA40000B0) /* CCR2  Address*/
#define DMAC  (*(volatile struct st_dmac  *)0xA4000060) /* DMAC  Address*/
#define DMAC0 (*(volatile struct st_dmac0 *)0xA4000020) /* DMAC0 Address*/
#define DMAC1 (*(volatile struct st_dmac0 *)0xA4000030) /* DMAC1 Address*/
#define DMAC2 (*(volatile struct st_dmac2 *)0xA4000040) /* DMAC2 Address*/
#define DMAC3 (*(volatile struct st_dmac3 *)0xA4000050) /* DMAC3 Address*/
#define CMT   (*(volatile struct st_cmt   *)0xA4000070) /* CMT   Address*/
#define ADC   (*(volatile struct st_adc   *)0xA4000080) /* ADC   Address*/
#define DAC   (*(volatile struct st_dac   *)0xA40000A0) /* DAC   Address*/
#define PFC   (*(volatile struct st_pfc   *)0xA4000100) /* PFC   Address*/
#define PA    (*(volatile struct st_pa    *)0xA4000120) /* PA    Address*/
#define PB    (*(volatile struct st_pb    *)0xA4000122) /* PB    Address*/
#define PC    (*(volatile struct st_pc    *)0xA4000124) /* PC    Address*/
#define PD    (*(volatile struct st_pd    *)0xA4000126) /* PD    Address*/
#define PE    (*(volatile struct st_pe    *)0xA4000128) /* PE    Address*/
#define PF    (*(volatile struct st_pf    *)0xA400012A) /* PF    Address*/
#define PG    (*(volatile struct st_pg    *)0xA400012C) /* PG    Address*/
#define PH    (*(volatile struct st_ph    *)0xA400012E) /* PH    Address*/
#define PJ    (*(volatile struct st_pj    *)0xA4000130) /* PJ    Address*/
#define SCP   (*(volatile struct st_scp   *)0xA4000136) /* SCP   Address*/
#define HUDI  (*(volatile struct st_hudi  *)0xA4000200) /* H-UDI Address*/
