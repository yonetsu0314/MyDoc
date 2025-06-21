/*----------------------------------------------------------------------*
 * File name	:	7615.h												*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/sh7615/RCS/7615.h 1.2 1970/01/01 00:00:00 sakaguti Exp $
 *$Log: 7615.h $
 * リビジョン 1.2  1970/01/01  00:00:00  sakaguti
 * SCIF1,2をPSCIF1,2に変更
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * ＳＨ２用インクルード
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

/************************************************************************/
/*      SH7615 Include File                                Ver 1.1      */
/************************************************************************/
struct st_sio {                                         /* struct SIO   */
              unsigned short    SIRDR;                  /* SIRDR        */
              unsigned short    SITDR;                  /* SITDR        */
              char              wk1;                    /*              */
              union {                                   /* SICTR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :1;         /*              */
                           unsigned char TM :1;         /*    TM        */
                           unsigned char SE :1;         /*    SE        */
                           unsigned char DL :1;         /*    DL        */
                           unsigned char TIE:1;         /*    TIE       */
                           unsigned char RIE:1;         /*    RIE       */
                           unsigned char TE :1;         /*    TE        */
                           unsigned char RE :1;         /*    RE        */
                           }      BIT;                  /*              */
                    }           SICTR;                  /*              */
              char              wk2;                    /*              */
              union {                                   /* SISTR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :4;        /*              */
                           unsigned char TERR:1;        /*    TERR      */
                           unsigned char RERR:1;        /*    RERR      */
                           unsigned char TDRE:1;        /*    TDRE      */
                           unsigned char RDRF:1;        /*    RDRF      */
                           }      BIT;                  /*              */
                    }           SISTR;                  /*              */
};                                                      /*              */
struct st_tpu {                                         /* struct TPU   */
              union {                                   /* TSTR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :5;        /*              */
                           unsigned char CST2:1;        /*    CST2      */
                           unsigned char CST1:1;        /*    CST1      */
                           unsigned char CST0:1;        /*    CST0      */
                           }      BIT;                  /*              */
                    }           TSTR;                   /*              */
              union {                                   /* TSYR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :5;       /*              */
                           unsigned char SYNC2:1;       /*    SYNC2     */
                           unsigned char SYNC1:1;       /*    SYNC1     */
                           unsigned char SYNC0:1;       /*    SYNC0     */
                           }      BIT;                  /*              */
                    }           TSYR;                   /*              */
};                                                      /*              */
struct st_tpu0 {                                        /* struct TPU0  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CCLR:3;       /*    CCLR      */
                            unsigned char CKEG:2;       /*    CKEG      */
                            unsigned char TPSC:3;       /*    TPSC      */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               union {                                  /* TMDR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char    :2;        /*              */
                            unsigned char BFB:1;        /*    BFB       */
                            unsigned char BFA:1;        /*    BFA       */
                            unsigned char MD :4;        /*    MD        */
                            }      BIT;                 /*              */
                     }          TMDR;                   /*              */
               union {                                  /* TIOR         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char H;            /*    TIORH     */
                            unsigned char L;            /*    TIORL     */
                            }       BYTE;               /*              */
                     struct {                           /*  Bit  Access */
                            unsigned char IOB:4;        /*    IOB       */
                            unsigned char IOA:4;        /*    IOA       */
                            unsigned char IOD:4;        /*    IOD       */
                            unsigned char IOC:4;        /*    IOC       */
                            }       BIT;                /*              */
                     }          TIOR;                   /*              */
               union {                                  /* TIER         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :3;      /*              */
                            unsigned char TCIEV:1;      /*    TCIEV     */
                            unsigned char TGIED:1;      /*    TGIED     */
                            unsigned char TGIEC:1;      /*    TGIEC     */
                            unsigned char TGIEB:1;      /*    TGIEB     */
                            unsigned char TGIEA:1;      /*    TGIEA     */
                            }      BIT;                 /*              */
                     }          TIER;                   /*              */
               union {                                  /* TSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :3;       /*              */
                            unsigned char TCFV:1;       /*    TCFV      */
                            unsigned char TGFD:1;       /*    TGFD      */
                            unsigned char TGFC:1;       /*    TGFC      */
                            unsigned char TGFB:1;       /*    TGFB      */
                            unsigned char TGFA:1;       /*    TGFA      */
                            }      BIT;                 /*              */
                     }          TSR;                    /*              */
               unsigned short   TCNT;                   /* TCNT         */
               unsigned short   TGRA;                   /* TGRA         */
               unsigned short   TGRB;                   /* TGRB         */
               unsigned short   TGRC;                   /* TGRC         */
               unsigned short   TGRD;                   /* TGRD         */
};                                                      /*              */
struct st_tpu1 {                                        /* struct TPU1  */
               union {                                  /* TCR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char     :1;       /*              */
                            unsigned char CCLR:2;       /*    CCLR      */
                            unsigned char CKEG:2;       /*    CKEG      */
                            unsigned char TPSC:3;       /*    TPSC      */
                            }      BIT;                 /*              */
                     }          TCR;                    /*              */
               union {                                  /* TMDR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char    :4;        /*              */
                            unsigned char MD :4;        /*    MD        */
                            }      BIT;                 /*              */
                     }           TMDR;                  /*              */
               union {                                  /* TIOR         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IOB:4;        /*    IOB       */
                            unsigned char IOA:4;        /*    IOA       */
                            }      BIT;                 /*              */
                     }          TIOR;                   /*              */
               unsigned char    wk;                     /*              */
               union {                                  /* TIER         */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char      :2;      /*              */
                            unsigned char TCIEU:1;      /*    TCIEU     */
                            unsigned char TCIEV:1;      /*    TCIEV     */
                            unsigned char      :2;      /*              */
                            unsigned char TGIEB:1;      /*    TGIEB     */
                            unsigned char TGIEA:1;      /*    TGIEA     */
                            }      BIT;                 /*              */
                     }          TIER;                   /*              */
               union {                                  /* TSR          */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TCFD:1;       /*    TCFD      */
                            unsigned char     :1;       /*              */
                            unsigned char TCFU:1;       /*    TCFU      */
                            unsigned char TCFV:1;       /*    TCFV      */
                            unsigned char     :2;       /*              */
                            unsigned char TGFB:1;       /*    TGFB      */
                            unsigned char TGFA:1;       /*    TGFA      */
                            }      BIT;                 /*              */
                     }          TSR;                    /*              */
               unsigned short   TCNT;                   /* TCNT         */
               unsigned short   TGRA;                   /* TGRA         */
               unsigned short   TGRB;                   /* TGRB         */
};                                                      /*              */
struct st_pfc {                                         /* struct PFC   */
              union {                                   /* PACR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char     :2;        /*              */
                           unsigned char MD13:1;        /*    PA13MD    */
                           unsigned char MD12:1;        /*    PA12MD    */
                           unsigned char MD11:1;        /*    PA11MD    */
                           unsigned char MD10:1;        /*    PA10MD    */
                           unsigned char MD9 :1;        /*    PA9MD     */
                           unsigned char MD8 :1;        /*    PA8MD     */
                           unsigned char MD7 :1;        /*    PA7MD     */
                           unsigned char MD6 :1;        /*    PA6MD     */
                           unsigned char MD5 :1;        /*    PA5MD     */
                           unsigned char MD4 :1;        /*    PA4MD     */
                           unsigned char MD3 :1;        /*    PA3MD     */
                           unsigned char MD2 :1;        /*    PA2MD     */
                           unsigned char MD1 :1;        /*    PA1MD     */
                           unsigned char MD0 :1;        /*    PA0MD     */
                           }       BIT;                 /*              */
              }                 PACR;                   /*              */
              union {                                   /* PAIOR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char    :2;         /*              */
                           unsigned char B13:1;         /*    Bit 13    */
                           unsigned char B12:1;         /*    Bit 12    */
                           unsigned char B11:1;         /*    Bit 11    */
                           unsigned char B10:1;         /*    Bit 10    */
                           unsigned char B9 :1;         /*    Bit  9    */
                           unsigned char B8 :1;         /*    Bit  8    */
                           unsigned char B7 :1;         /*    Bit  7    */
                           unsigned char B6 :1;         /*    Bit  6    */
                           unsigned char B5 :1;         /*    Bit  5    */
                           unsigned char B4 :1;         /*    Bit  4    */
                           unsigned char    :1;         /*              */
                           unsigned char B2 :1;         /*    Bit  2    */
                           unsigned char B1 :1;         /*    Bit  1    */
                           unsigned char B0 :1;         /*    Bit  0    */
                           }       BIT;                 /*              */
              }                 PAIOR;                  /*              */
              unsigned char     wk1[4];                 /*              */
              union {                                   /* PBCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char MD15:2;        /*    PB15MD    */
                           unsigned char MD14:2;        /*    PB14MD    */
                           unsigned char MD13:2;        /*    PB13MD    */
                           unsigned char MD12:2;        /*    PB12MD    */
                           unsigned char MD11:2;        /*    PB11MD    */
                           unsigned char MD10:2;        /*    PB10MD    */
                           unsigned char MD9 :2;        /*    PB9MD     */
                           unsigned char MD8 :2;        /*    PB8MD     */
                           }       BIT;                 /*              */
              }                 PBCR;                   /*              */
              union {                                   /* PBIOR        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char B15:1;         /*    Bit 15    */
                           unsigned char B14:1;         /*    Bit 14    */
                           unsigned char B13:1;         /*    Bit 13    */
                           unsigned char B12:1;         /*    Bit 12    */
                           unsigned char B11:1;         /*    Bit 11    */
                           unsigned char B10:1;         /*    Bit 10    */
                           unsigned char B9 :1;         /*    Bit  9    */
                           unsigned char B8 :1;         /*    Bit  8    */
                           unsigned char B7 :1;         /*    Bit  7    */
                           unsigned char B6 :1;         /*    Bit  6    */
                           unsigned char B5 :1;         /*    Bit  5    */
                           unsigned char B4 :1;         /*    Bit  4    */
                           unsigned char B3 :1;         /*    Bit  3    */
                           unsigned char B2 :1;         /*    Bit  2    */
                           unsigned char B1 :1;         /*    Bit  1    */
                           unsigned char B0 :1;         /*    Bit  0    */
                           }       BIT;                 /*              */
              }                 PBIOR;                  /*              */
              unsigned char     wk2[2];                 /*              */
              union {                                   /* PBCR2        */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned char MD7:2;         /*    PB7MD     */
                           unsigned char MD6:2;         /*    PB6MD     */
                           unsigned char MD5:2;         /*    PB5MD     */
                           unsigned char MD4:2;         /*    PB4MD     */
                           unsigned char MD3:2;         /*    PB3MD     */
                           unsigned char MD2:2;         /*    PB2MD     */
                           unsigned char MD1:2;         /*    PB1MD     */
                           unsigned char MD0:2;         /*    PB0MD     */
                           }       BIT;                 /*              */
              }                 PBCR2;                  /*              */
};                                                      /*              */
struct st_pa {                                          /* struct PA    */
             union {                                    /* PADR         */
                   unsigned short WORD;                 /*  Word Access */
                   struct {                             /*  Byte Access */
                          unsigned char H;              /*    High      */
                          unsigned char L;              /*    Low       */
                          }       BYTE;                 /*              */
                   struct {                             /*  Bit  Access */
                          unsigned char    :2;          /*              */
                          unsigned char B13:1;          /*    Bit 13    */
                          unsigned char B12:1;          /*    Bit 12    */
                          unsigned char B11:1;          /*    Bit 11    */
                          unsigned char B10:1;          /*    Bit 10    */
                          unsigned char B9 :1;          /*    Bit  9    */
                          unsigned char B8 :1;          /*    Bit  8    */
                          unsigned char B7 :1;          /*    Bit  7    */
                          unsigned char B6 :1;          /*    Bit  6    */
                          unsigned char B5 :1;          /*    Bit  5    */
                          unsigned char B4 :1;          /*    Bit  4    */
                          unsigned char    :1;          /*              */
                          unsigned char B2 :1;          /*    Bit  2    */
                          unsigned char B1 :1;          /*    Bit  1    */
                          unsigned char B0 :1;          /*    Bit  0    */
                          }       BIT;                  /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_pb {                                          /* struct PB    */
             union {                                    /* PBDR         */
                   unsigned short WORD;                 /*  Word Access */
                   struct {                             /*  Byte Access */
                          unsigned char H;              /*    High      */
                          unsigned char L;              /*    Low       */
                          }       BYTE;                 /*              */
                   struct {                             /*  Bit  Access */
                          unsigned char B15:1;          /*    Bit 15    */
                          unsigned char B14:1;          /*    Bit 14    */
                          unsigned char B13:1;          /*    Bit 13    */
                          unsigned char B12:1;          /*    Bit 12    */
                          unsigned char B11:1;          /*    Bit 11    */
                          unsigned char B10:1;          /*    Bit 10    */
                          unsigned char B9 :1;          /*    Bit  9    */
                          unsigned char B8 :1;          /*    Bit  8    */
                          unsigned char B7 :1;          /*    Bit  7    */
                          unsigned char B6 :1;          /*    Bit  6    */
                          unsigned char B5 :1;          /*    Bit  5    */
                          unsigned char B4 :1;          /*    Bit  4    */
                          unsigned char B3 :1;          /*    Bit  3    */
                          unsigned char B2 :1;          /*    Bit  2    */
                          unsigned char B1 :1;          /*    Bit  1    */
                          unsigned char B0 :1;          /*    Bit  0    */
                          }       BIT;                  /*              */
                   }            DR;                     /*              */
};                                                      /*              */
struct st_scif {                                        /* struct SCIF  */
               union {                                  /* SCSMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char CA  :1;       /*    C/A       */
                            unsigned char CHR :1;       /*    CHR       */
                            unsigned char PERR:1;       /* PEはSCUA.Hと競合ためPERRとする */
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
                            unsigned char     :1;       /*              */
                            unsigned char CKE :2;       /*    CKE       */
                            }      BIT;                 /*              */
                     }          SCSCR;                  /*              */
               char             wk3;                    /*              */
               unsigned char    SCFTDR;                 /* SCFTDR       */
               char             wk4;                    /*              */
               union {                                  /* SC1SSR       */
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
                     }          SC1SSR;                 /*              */
               union {                                  /* SC2SSR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TLM :1;       /*    TLM       */
                            unsigned char RLM :1;       /*    RLM       */
                            unsigned char N   :2;       /*    N         */
                            unsigned char MPB :1;       /*    MPB       */
                            unsigned char MPBT:1;       /*    MPBT      */
                            unsigned char EI  :1;       /*    EI        */
                            unsigned char ORER:1;       /*    ORER      */
                            }      BIT;                 /*              */
                     }          SC2SSR;                 /*              */
               char             wk5;                    /*              */
               unsigned char    SCFRDR;                 /* SCFRDR       */
               char             wk6;                    /*              */
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
               char             wk7;                    /*              */
               union {                                  /* SCFDR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short  :3;         /*              */
                            unsigned short T:5;         /*    T         */
                            unsigned short  :3;         /*              */
                            unsigned short R:5;         /*    R         */
                            }       BIT;                /*              */
                     }          SCFDR;                  /*              */
               unsigned short   SCFER;                  /* SCFER        */
               union {                                  /* SCIMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IRMOD:1;      /*    IRMOD     */
                            }      BIT;                 /*              */
                     }          SCIMR;                  /*              */
};                                                      /*              */
struct st_irda {                                        /* struct IrDA  */
               union {                                  /* SCSMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char    :1;        /*              */
                            unsigned char ICK:4;        /*    ICK       */
                            unsigned char    :1;        /*              */
                            unsigned char CKS:2;        /*    CKS       */
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
               union {                                  /* SC1SSR       */
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
                     }          SC1SSR;                 /*              */
               union {                                  /* SC2SSR       */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char TLM :1;       /*    TLM       */
                            unsigned char RLM :1;       /*    RLM       */
                            unsigned char N   :2;       /*    N         */
                            unsigned char     :2;       /*              */
                            unsigned char EI  :1;       /*    EI        */
                            unsigned char ORER:1;       /*    ORER      */
                            }      BIT;                 /*              */
                     }          SC2SSR;                 /*              */
               char             wk5;                    /*              */
               unsigned char    SCFRDR;                 /* SCFRDR       */
               char             wk6;                    /*              */
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
               char             wk7;                    /*              */
               union {                                  /* SCFDR        */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned short  :3;         /*              */
                            unsigned short T:5;         /*    T         */
                            unsigned short  :3;         /*              */
                            unsigned short R:5;         /*    R         */
                            }       BIT;                /*              */
                     }          SCFDR;                  /*              */
               unsigned short   SCFER;                  /* SCFER        */
               union {                                  /* SCIMR        */
                     unsigned char BYTE;                /*  Byte Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IRMOD:1;      /*    IRMOD     */
                            unsigned char PSEL :1;      /*    PSEL      */
                            unsigned char RIVS :1;      /*    RIVS      */
                            }      BIT;                 /*              */
                     }          SCIMR;                  /*              */
};                                                      /*              */
struct st_edmac {                                       /* struct E-DMAC*/
                union {                                 /* EDMR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int    :26;       /*              */
                             unsigned int DL : 2;       /*    DL        */
                             unsigned int    : 3;       /*              */
                             unsigned int SWR: 1;       /*    SWR       */
                             }     BIT;                 /*              */
                      }         EDMR;                   /*              */
                union {                                 /* EDTRR        */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int   :31;        /*              */
                             unsigned int TR: 1;        /*    TR        */
                             }     BIT;                 /*              */
                      }         EDTRR;                  /*              */
                union {                                 /* EDRRR        */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int   :31;        /*              */
                             unsigned int RR: 1;        /*    RR        */
                             }     BIT;                 /*              */
                      }         EDRRR;                  /*              */
                void           *TDLAR;                  /* TDLAR        */
                void           *RDLAR;                  /* RDLAR        */
                union {                                 /* EESR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int      :7;      /*              */
                             unsigned int RFCOF:1;      /*    RFCOF     */
                             unsigned int      :1;      /*              */
                             unsigned int ECI  :1;      /*    ECI       */
                             unsigned int TC   :1;      /*    TC        */
                             unsigned int TDE  :1;      /*    TDE       */
                             unsigned int TFUF :1;      /*    TFUF      */
                             unsigned int FR   :1;      /*    FR        */
                             unsigned int RDE  :1;      /*    RDE       */
                             unsigned int RFOF :1;      /*    RFOF      */
                             unsigned int      :3;      /*              */
                             unsigned int ITF  :1;      /*    ITF       */
                             unsigned int CND  :1;      /*    CND       */
                             unsigned int DLC  :1;      /*    DLC       */
                             unsigned int CD   :1;      /*    CD        */
                             unsigned int TRO  :1;      /*    TRO       */
                             unsigned int RMAF :1;      /*    RMAF      */
                             unsigned int      :2;      /*              */
                             unsigned int RRF  :1;      /*    RRF       */
                             unsigned int RTLF :1;      /*    RTLF      */
                             unsigned int RTSF :1;      /*    RTSF      */
                             unsigned int PRE  :1;      /*    PRE       */
                             unsigned int CERF :1;      /*    CERF      */
                             }     BIT;                 /*              */
                      }         EESR;                   /*              */
                union {                                 /* EESIPR       */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int        :7;    /*              */
                             unsigned int RFCOFIP:1;    /*    RFCOFIP   */
                             unsigned int        :1;    /*              */
                             unsigned int ECIIP  :1;    /*    ECIIP     */
                             unsigned int TCIP   :1;    /*    TCIP      */
                             unsigned int TDEIP  :1;    /*    TDEIP     */
                             unsigned int TFUFIP :1;    /*    TFUFIP    */
                             unsigned int FRIP   :1;    /*    FRIP      */
                             unsigned int RDEIP  :1;    /*    RDEIP     */
                             unsigned int RFOFIP :1;    /*    RFOFIP    */
                             unsigned int        :3;    /*              */
                             unsigned int ITFIP  :1;    /*    ITFIP     */
                             unsigned int CNDIP  :1;    /*    CNDIP     */
                             unsigned int DLCIP  :1;    /*    DLCIP     */
                             unsigned int CDIP   :1;    /*    CDIP      */
                             unsigned int TROIP  :1;    /*    TROIP     */
                             unsigned int RMAFIP :1;    /*    RMAFIP    */
                             unsigned int        :2;    /*              */
                             unsigned int RRFIP  :1;    /*    RRFIP     */
                             unsigned int RTLFIP :1;    /*    RTLFIP    */
                             unsigned int RTSFIP :1;    /*    RTSFIP    */
                             unsigned int PREIP  :1;    /*    PREIP     */
                             unsigned int CERFIP :1;    /*    CERFIP    */
                             }     BIT;                 /*              */
                      }         EESIPR;                 /*              */
                union {                                 /* TRSCER       */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int       :19;    /*              */
                             unsigned int ITFCE : 1;    /*    ITFCE     */
                             unsigned int CNDCE : 1;    /*    CNDCE     */
                             unsigned int DLCCE : 1;    /*    DLCCE     */
                             unsigned int CDCE  : 1;    /*    CDCE      */
                             unsigned int TROCE : 1;    /*    TROCE     */
                             unsigned int RMAFCE: 1;    /*    RMAFCE    */
                             unsigned int       : 2;    /*              */
                             unsigned int RRFCE : 1;    /*    RRFCE     */
                             unsigned int RTLFCE: 1;    /*    RTLFCE    */
                             unsigned int RTSFCE: 1;    /*    RTSFCE    */
                             unsigned int PRECE : 1;    /*    PRECE     */
                             unsigned int CERFCE: 1;    /*    CERFCE    */
                             }     BIT;                 /*              */
                      }         TRSCER;                 /*              */
                unsigned int    RMFCR;                  /* RMFCR        */
                unsigned int    TFTR;                   /* TFTR         */
                union {                                 /* FDR          */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int    :23;       /*              */
                             unsigned int TFD: 1;       /*    TFD       */
                             unsigned int    : 7;       /*              */
                             unsigned int RFD: 1;       /*    RFD       */
                             }     BIT;                 /*              */
                      }         FDR;                    /*              */
                union {                                 /* RCR          */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int    :31;       /*              */
                             unsigned int RNC: 1;       /*    RNC       */
                             }     BIT;                 /*              */
                      }         RCR;                    /*              */
                union {                                 /* EDOCR        */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int    :29;       /*              */
                             unsigned int FEC: 1;       /*    FEC       */
                             unsigned int AEC: 1;       /*    AEC       */
                             unsigned int EDH: 1;       /*    EDH       */
                             }     BIT;                 /*              */
                      }         EDOCR;                  /*              */
                char            wk1[12];                /*              */
                void           *RBWAR;                  /* RBWAR        */
                void           *RDFAR;                  /* RDFAR        */
                char            wk2[4];                 /*              */
                void           *TBRAR;                  /* TBRAR        */
                void           *TDFAR;                  /* TDFAR        */
};                                                      /*              */
struct st_ec {                                          /* struct EtherC*/
             union {                                    /* ECMR         */
                   unsigned int LONG;                   /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int      :19;        /*              */
                          unsigned int PRCEF: 1;        /*    PRCEF     */
                          unsigned int      : 2;        /*              */
                          unsigned int MPDE : 1;        /*    MPDE      */
                          unsigned int      : 2;        /*              */
                          unsigned int RE   : 1;        /*    RE        */
                          unsigned int TE   : 1;        /*    TE        */
                          unsigned int      : 1;        /*              */
                          unsigned int ILB  : 1;        /*    ILB       */
                          unsigned int ELB  : 1;        /*    ELB       */
                          unsigned int DM   : 1;        /*    DM        */
                          unsigned int PRM  : 1;        /*    PRM       */
                          }       BIT;                  /*              */
                   }            ECMR;                   /*              */
             union {                                    /* ECSR         */
                   unsigned int LONG;                   /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int      :29;        /*              */
                          unsigned int LCHNG: 1;        /*    LCHNG     */
                          unsigned int MPR  : 1;        /*    MPR       */
                          unsigned int ICD  : 1;        /*    ICD       */
                          }       BIT;                  /*              */
                   }            ECSR;                   /*              */
             union {                                    /* ECSIPR       */
                   unsigned int LONG;                   /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int        :29;      /*              */
                          unsigned int LCHNGIP: 1;      /*    LCHNGIP   */
                          unsigned int MPRIP  : 1;      /*    MPRIP     */
                          unsigned int ICDIP  : 1;      /*    ICDIP     */
                          }       BIT;                  /*              */
                   }            ECSIPR;                 /*              */
             union {                                    /* PIR          */
                   unsigned int LONG;                   /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int    :28;          /*              */
                          unsigned int MDI: 1;          /*    MDI       */
                          unsigned int MDO: 1;          /*    MDO       */
                          unsigned int MMD: 1;          /*    MMD       */
                          unsigned int MDC: 1;          /*    MDC       */
                          }       BIT;                  /*              */
                   }            PIR;                    /*              */
             unsigned int       MAHR;                   /* MAHR         */
             unsigned int       MALR;                   /* MALR         */
             unsigned int       RFLR;                   /* RFLR         */
             union {                                    /* PSR          */
                   unsigned int LONG;                   /*  Long Access */
                   struct {                             /*  Bit  Access */
                          unsigned int     :31;         /*              */
                          unsigned int LMON: 1;         /*    LMON      */
                          }       BIT;                  /*              */
                   }            PSR;                    /*              */
             unsigned int       TROCR;                  /* TROCR        */
             unsigned int       CDCR;                   /* CDCR         */
             unsigned int       LCCR;                   /* LCCR         */
             unsigned int       CNDCR;                  /* CNDCR        */
             unsigned int       IFLCR;                  /* IFLCR        */
             unsigned int       CEFCR;                  /* CEFCR        */
             unsigned int       FRECR;                  /* FRECR        */
             unsigned int       TSFRCR;                 /* TSFRCR       */
             unsigned int       TLFRCR;                 /* TLFRCR       */
             unsigned int       RFCR;                   /* RFCR         */
             unsigned int       MAFCR;                  /* MAFCR        */
};                                                      /*              */
struct st_frt {                                         /* struct FRT   */
              union {                                   /* TIER         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ICIE :1;       /*    ICIE      */
                           unsigned char      :3;       /*              */
                           unsigned char OCIAE:1;       /*    OCIAE     */
                           unsigned char OCIBE:1;       /*    OCIBE     */
                           unsigned char OVIE :1;       /*    OVIE      */
                           }      BIT;                  /*              */
                    }           TIER;                   /*              */
              union {                                   /* FTCSR        */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char ICF  :1;       /*    ICF       */
                           unsigned char      :3;       /*              */
                           unsigned char OCFA :1;       /*    OCFA      */
                           unsigned char OCFB :1;       /*    OCFB      */
                           unsigned char OVF  :1;       /*    OVF       */
                           unsigned char CCLRA:1;       /*    CCLRA     */
                           }      BIT;                  /*              */
                    }           FTCSR;                  /*              */
              unsigned short    FRC;                    /* FRC          */
              unsigned short    OCRA;                   /* OCRA or OCRB */
              union {                                   /* TCR          */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char IEDG:1;        /*    IEDG      */
                           unsigned char     :5;        /*              */
                           unsigned char CKS :2;        /*    CKS       */
                           }      BIT;                  /*              */
                    }           TCR;                    /*              */
              union {                                   /* TOCR         */
                    unsigned char BYTE;                 /*  Byte Access */
                    struct {                            /*  Bit  Access */
                           unsigned char      :3;       /*              */
                           unsigned char OCRS :1;       /*    OCRS      */
                           unsigned char      :2;       /*              */
                           unsigned char OLVLA:1;       /*    OLVLA     */
                           unsigned char OLVLB:1;       /*    OLVLB     */
                           }      BIT;                  /*              */
                    }           TOCR;                   /*              */
              unsigned short    FICR;                   /* FICR         */
};                                                      /*              */
struct st_intc {                                        /* struct INTC  */
               union {                                  /* IPRD         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char UU:4;         /*    TPU0      */
                            unsigned char UL:4;         /*    TPU1      */
                            unsigned char LU:4;         /*    TPU2      */
                            unsigned char LL:4;         /*    SCIF1     */
                            }       BIT;                /*              */
                     }          IPRD;                   /*              */
               union {                                  /* VCRE         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TG0A;         /*    TG0A      */
                            unsigned char TG0B;         /*    TG0B      */
                            }       BYTE;               /*              */
                     }          VCRE;                   /*              */
               union {                                  /* VCRF         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TG0C;         /*    TG0C      */
                            unsigned char TG0D;         /*    TG0D      */
                            }       BYTE;               /*              */
                     }          VCRF;                   /*              */
               union {                                  /* VCRG         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TC0V;         /*    TC0V      */
                            }       BYTE;               /*              */
                     }          VCRG;                   /*              */
               union {                                  /* VCRH         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TG1A;         /*    TG1A      */
                            unsigned char TG1B;         /*    TG1B      */
                            }       BYTE;               /*              */
                     }          VCRH;                   /*              */
               union {                                  /* VCRI         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TC1V;         /*    TC1V      */
                            unsigned char TC1U;         /*    TC1U      */
                            }       BYTE;               /*              */
                     }          VCRI;                   /*              */
               union {                                  /* VCRJ         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TG2A;         /*    TG2A      */
                            unsigned char TG2B;         /*    TG2B      */
                            }       BYTE;               /*              */
                     }          VCRJ;                   /*              */
               union {                                  /* VCRK         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char TC2V;         /*    TC2V      */
                            unsigned char TC2U;         /*    TC2U      */
                            }       BYTE;               /*              */
                     }          VCRK;                   /*              */
               union {                                  /* VCRL         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char SER1;         /*    SER1      */
                            unsigned char SRX1;         /*    SRX1      */
                            }       BYTE;               /*              */
                     }          VCRL;                   /*              */
               union {                                  /* VCRM         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char SBR1;         /*    SBR1      */
                            unsigned char STX1;         /*    STX1      */
                            }       BYTE;               /*              */
                     }          VCRM;                   /*              */
               union {                                  /* VCRN         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char SER2;         /*    SER2      */
                            unsigned char SRX2;         /*    SRX2      */
                            }       BYTE;               /*              */
                     }          VCRN;                   /*              */
               union {                                  /* VCRO         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char SBR2;         /*    SBR2      */
                            unsigned char STX2;         /*    STX2      */
                            }       BYTE;               /*              */
                     }          VCRO;                   /*              */
               char             wk1[8];                 /*              */
               union {                                  /* IPRB         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned short UU:4;        /*    E-DMAC    */
                            unsigned short UL:4;        /*    FRT       */
                            }       BIT;                /*              */
                     }          IPRB;                   /*              */
               union {                                  /* VCRA         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char EIN;          /*    EIN       */
                            }       BYTE;               /*              */
                     }          VCRA;                   /*              */
               char             wk2[2];                 /*              */
               union {                                  /* VCRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char FIC;          /*    FIC       */
                            unsigned char FOC;          /*    FOC       */
                            }       BYTE;               /*              */
                     }          VCRC;                   /*              */
               union {                                  /* VCRD         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char FOV;          /*    FOV       */
                            }       BYTE;               /*              */
                     }          VCRD;                   /*              */
               char             wk3[86];                /*              */
               union {                                  /* IPRE         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char UU:4;         /*    SCF2      */
                            unsigned char UL:4;         /*    SIO0      */
                            unsigned char LU:4;         /*    SIO1      */
                            unsigned char LL:4;         /*    SIO2      */
                            }       BIT;                /*              */
                     }          IPRE;                   /*              */
               union {                                  /* VCRP         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RER0;         /*    RER0      */
                            unsigned char TER0;         /*    TER0      */
                            }       BYTE;               /*              */
                     }          VCRP;                   /*              */
               union {                                  /* VCRQ         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RDF0;         /*    RDF0      */
                            unsigned char TDE0;         /*    TDE0      */
                            }       BYTE;               /*              */
                     }          VCRQ;                   /*              */
               union {                                  /* VCRR         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RER1;         /*    RER1      */
                            unsigned char TER1;         /*    TER1      */
                            }       BYTE;               /*              */
                     }          VCRR;                   /*              */
               union {                                  /* VCRS         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RDF1;         /*    RDF1      */
                            unsigned char TDE1;         /*    TDE1      */
                            }       BYTE;               /*              */
                     }          VCRS;                   /*              */
               union {                                  /* VCRT         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RER2;         /*    RER2      */
                            unsigned char TER2;         /*    TER2      */
                            }       BYTE;               /*              */
                     }          VCRT;                   /*              */
               union {                                  /* VCRU         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char RDF2;         /*    RDF2      */
                            unsigned char TDE2;         /*    TDE2      */
                            }       BYTE;               /*              */
                     }          VCRU;                   /*              */
               char             wk4[18];                /*              */
               union {                                  /* ICR          */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char NMIL :1;      /*    NMIL      */
                            unsigned char      :6;      /*              */
                            unsigned char NMIE: 1;      /*    NMIE      */
                            unsigned char      :6;      /*              */
                            unsigned char EXIMD:1;      /*    EXIMD     */
                            unsigned char VECMD:1;      /*    VECMD     */
                            }       BIT;                /*              */
                     }          ICR;                    /*              */
               union {                                  /* IPRA         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char wk:4;         /*              */
                            unsigned char UL:4;         /*    DMAC0,1   */
                            unsigned char LU:4;         /*    WDT,REF   */
                            }       BIT;                /*              */
                     }          IPRA;                   /*              */
               union {                                  /* VCRWDT       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Byte Access */
                            unsigned char WIT;          /*    WIT       */
                            unsigned char BCM;          /*    BCM       */
                            }       BYTE;               /*              */
                     }          VCRWDT;                 /*              */
               union {                                  /* IPRC         */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char UU:4;         /*    IRQ0      */
                            unsigned char UL:4;         /*    IRQ1      */
                            unsigned char LU:4;         /*    IRQ2      */
                            unsigned char LL:4;         /*    IRQ3      */
                            }       BIT;                /*              */
                     }          IPRC;                   /*              */
               union {                                  /* IRQCSR       */
                     unsigned short WORD;               /*  Word Access */
                     struct {                           /*  Bit  Access */
                            unsigned char IRQ3S :2;     /*    IRQ3S     */
                            unsigned char IRQ2S :2;     /*    IRQ2S     */
                            unsigned char IRQ1S :2;     /*    IRQ1S     */
                            unsigned char IRQ0S :2;     /*    IRQ0S     */
                            unsigned char IRL3PS:1;     /*    IRQ3PS    */
                            unsigned char IRL2PS:1;     /*    IRQ2PS    */
                            unsigned char IRL1PS:1;     /*    IRQ1PS    */
                            unsigned char IRL0PS:1;     /*    IRQ0PS    */
                            unsigned char IRQ3F :1;     /*    IRQ3F     */
                            unsigned char IRQ2F :1;     /*    IRQ2F     */
                            unsigned char IRQ1F :1;     /*    IRQ1F     */
                            unsigned char IRQ0F :1;     /*    IRQ0F     */
                            }       BIT;                /*              */
                     }          IRQCSR;                 /*              */
               char             wk5[182];               /*              */
               unsigned int     VCRDMA0;                /* VCRDMA0      */
               char             wk6[4];                 /*              */
               unsigned int     VCRDMA1;                /* VCRDMA1      */
};                                                      /*              */
struct st_dmac {                                        /* struct DMAC  */
               union {                                  /* DMAOR        */
                     unsigned int LONG;                 /*  Long Access */
                     struct {                           /*  Bit  Access */
                            unsigned int     :28;       /*              */
                            unsigned int PR  :1 ;       /*    PR        */
                            unsigned int AE  :1 ;       /*    AE        */
                            unsigned int NMIF:1 ;       /*    NMIF      */
                            unsigned int DME :1 ;       /*    DME       */
                            }     BIT;                  /*              */
                     }          DMAOR;                  /*              */
};                                                      /*              */
struct st_dmac0 {                                       /* struct DMAC0 */
                unsigned char   wk1;                    /*              */
                union {                                 /* DRCR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char   :3;        /*              */
                             unsigned char RS:5;        /*    RS        */
                             }      BIT;                /*              */
                      }         DRCR;                   /*              */
                char            wk2[270];               /*              */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    TCR;                    /* TCR          */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int   :16;        /*              */
                             unsigned int DM: 2;        /*    DM        */
                             unsigned int SM: 2;        /*    SM        */
                             unsigned int TS: 2;        /*    TS        */
                             unsigned int AR: 1;        /*    AR        */
                             unsigned int AM: 1;        /*    AM        */
                             unsigned int AL: 1;        /*    AL        */
                             unsigned int DS: 1;        /*    DS        */
                             unsigned int DL: 1;        /*    DL        */
                             unsigned int TB: 1;        /*    TB        */
                             unsigned int TA: 1;        /*    TA        */
                             unsigned int IE: 1;        /*    IE        */
                             unsigned int TE: 1;        /*    TE        */
                             unsigned int DE: 1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_dmac1 {                                       /* struct DMAC1 */
                unsigned char   wk1[2];                 /*              */
                union {                                 /* DRCR         */
                      unsigned char BYTE;               /*  Byte Access */
                      struct {                          /*  Bit  Access */
                             unsigned char   :3;        /*              */
                             unsigned char RS:5;        /*    RS        */
                             }      BIT;                /*              */
                      }         DRCR;                   /*              */
                char            wk2[285];               /*              */
                void           *SAR;                    /* SAR          */
                void           *DAR;                    /* DAR          */
                unsigned int    TCR;                    /* TCR          */
                union {                                 /* CHCR         */
                      unsigned int LONG;                /*  Long Access */
                      struct {                          /*  Bit  Access */
                             unsigned int   :16;        /*              */
                             unsigned int DM: 2;        /*    DM        */
                             unsigned int SM: 2;        /*    SM        */
                             unsigned int TS: 2;        /*    TS        */
                             unsigned int AR: 1;        /*    AR        */
                             unsigned int AM: 1;        /*    AM        */
                             unsigned int AL: 1;        /*    AL        */
                             unsigned int DS: 1;        /*    DS        */
                             unsigned int DL: 1;        /*    DL        */
                             unsigned int TB: 1;        /*    TB        */
                             unsigned int TA: 1;        /*    TA        */
                             unsigned int IE: 1;        /*    IE        */
                             unsigned int TE: 1;        /*    TE        */
                             unsigned int DE: 1;        /*    DE        */
                             }     BIT;                 /*              */
                      }         CHCR;                   /*              */
};                                                      /*              */
struct st_ubc {                                         /* struct UBC   */
              void             *BARA;                   /* BARA         */
              unsigned int      BAMRA;                  /* BAMRA        */
              union {                                   /* BBRA         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short    :8;        /*              */
                           unsigned short CPA:2;        /*    CPA       */
                           unsigned short IDA:2;        /*    IDA       */
                           unsigned short RWA:2;        /*    RWA       */
                           unsigned short SZA:2;        /*    SZA       */
                           }       BIT;                 /*              */
                    }           BBRA;                   /*              */
              char              wk1[6];                 /*              */
              union {                                   /* BRFR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short SVF:1;        /*    SVF       */
                           unsigned short PID:3;        /*    PID       */
                           unsigned short    :4;        /*              */
                           unsigned short DVF:1;        /*    DVF       */
                           }       BIT;                 /*              */
                    }           BRFR;                   /*              */
              char              wk2[2];                 /*              */
              void             *BRSR;                   /* BRSR         */
              void             *BRDR;                   /* BRDR         */
              char              wk3[4];                 /*              */
              void             *BARB;                   /* BARB         */
              unsigned int      BAMRB;                  /* BAMRB        */
              union {                                   /* BBRB         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short    :8;        /*              */
                           unsigned short CPB:2;        /*    CPB       */
                           unsigned short IDB:2;        /*    IDB       */
                           unsigned short RWB:2;        /*    RWB       */
                           unsigned short SZB:2;        /*    SZB       */
                           }       BIT;                 /*              */
                    }           BBRB;                   /*              */
              char              wk4[6];                 /*              */
              union {                                   /* BRCR         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Word Access */
                           unsigned short H;            /*    High      */
                           unsigned short L;            /*    Low       */
                           }     WORD;                  /*              */
                    struct {                            /*  Bit  Access */
                           unsigned short CMFCA:1;      /*    CMFCA     */
                           unsigned short CMFPA:1;      /*    CMFPA     */
                           unsigned short      :2;      /*              */
                           unsigned short PCTE :1;      /*    PCTE      */
                           unsigned short PCBA :1;      /*    PCBA      */
                           unsigned short      :2;      /*              */
                           unsigned short CMFCB:1;      /*    CMFCB     */
                           unsigned short CMFPB:1;      /*    CMFPB     */
                           unsigned short      :1;      /*              */
                           unsigned short SEQ  :2;      /*    SEQ       */
                           unsigned short PCBB :1;      /*    PCBB      */
                           unsigned short      :2;      /*              */
                           unsigned short CMFCC:1;      /*    CMFCC     */
                           unsigned short CMFPC:1;      /*    CMFPC     */
                           unsigned short ETBEC:1;      /*    ETBEC     */
                           unsigned short      :1;      /*              */
                           unsigned short DBEC :1;      /*    DBEC      */
                           unsigned short PCBC :1;      /*    PCBC      */
                           unsigned short      :2;      /*              */
                           unsigned short CMFCD:1;      /*    CMFCD     */
                           unsigned short CMFPD:1;      /*    CMFPD     */
                           unsigned short ETBED:1;      /*    ETBED     */
                           unsigned short      :1;      /*              */
                           unsigned short DBED :1;      /*    DBED      */
                           unsigned short PCBD :1;      /*    PCBD      */
                           }     BIT;                   /*              */
                    }           BRCR;                   /*              */
              char              wk5[12];                /*              */
              void             *BARC;                   /* BARC         */
              unsigned int      BAMRC;                  /* BAMRC        */
              union {                                   /* BBRC         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :6;       /*              */
                           unsigned short XYEC:1;       /*    XYEC      */
                           unsigned short XYSC:1;       /*    XYSC      */
                           unsigned short CPC :2;       /*    CPC       */
                           unsigned short IDC :2;       /*    IDC       */
                           unsigned short RWC :2;       /*    RWC       */
                           unsigned short SZC :2;       /*    SZC       */
                           }       BIT;                 /*              */
                    }           BBRC;                   /*              */
              char              wk6[6];                 /*              */
              unsigned int      BDRC;                   /* BDRC         */
              unsigned int      BDMRC;                  /* BDMRC        */
              unsigned short    BETRC;                  /* BETRC        */
              char              wk7[6];                 /*              */
              void             *BARD;                   /* BARD         */
              unsigned int      BAMRD;                  /* BAMRD        */
              union {                                   /* BBRD         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :6;       /*              */
                           unsigned short XYED:1;       /*    XYED      */
                           unsigned short XYSD:1;       /*    XYSD      */
                           unsigned short CPD :2;       /*    CPD       */
                           unsigned short IDD :2;       /*    IDD       */
                           unsigned short RWD :2;       /*    RWD       */
                           unsigned short SZD :2;       /*    SZD       */
                           }       BIT;                 /*              */
                    }           BBRD;                   /*              */
              char              wk8[6];                 /*              */
              unsigned int      BDRD;                   /* BDRD         */
              unsigned int      BDMRD;                  /* BDMRD        */
              unsigned short    BETRD;                  /* BETRD        */
};                                                      /*              */
struct st_bsc {                                         /* struct BSC   */
              union {                                   /* WCR2         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :16;      /*              */
                           unsigned short A4WD: 2;      /*    A4WD      */
                           unsigned short     : 1;      /*              */
                           unsigned short A4WM: 1;      /*    A4WM      */
                           unsigned short A3WM: 1;      /*    A3WM      */
                           unsigned short A2WM: 1;      /*    A2WM      */
                           unsigned short A1WM: 1;      /*    A1WM      */
                           unsigned short A0WM: 1;      /*    A0WM      */
                           unsigned short     : 4;      /*              */
                           unsigned short IW4 : 2;      /*    IW4       */
                           unsigned short W4  : 2;      /*    W4        */
                           }     BIT;                   /*              */
                    }           WCR2;                   /*              */
              union {                                   /* WCR3         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :16;     /*              */
                           unsigned short      : 2;     /*              */
                           unsigned short A4SW : 3;     /*    A4SW      */
                           unsigned short      : 1;     /*              */
                           unsigned short A4HW : 2;     /*    A4HW      */
                           unsigned short A3SHW: 2;     /*    A3SHW     */
                           unsigned short A2SHW: 2;     /*    A2SHW     */
                           unsigned short A1SHW: 2;     /*    A1SHW     */
                           unsigned short A0SHW: 2;     /*    A0SHW     */
                           }     BIT;                   /*              */
                    }           WCR3;                   /*              */
              char              wk1[24];                /*              */
              union {                                   /* BCR1         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short         :16;  /*              */
                           unsigned short         : 1;  /*              */
                           unsigned short A4LW    : 2;  /*    A4LW      */
                           unsigned short A2ENDIAN: 1;  /*    A2ENDIAN  */
                           unsigned short BSTROM  : 1;  /*    BSTROM    */
                           unsigned short         : 1;  /*              */
                           unsigned short AHLW    : 2;  /*    AHLW      */
                           unsigned short A1LW    : 2;  /*    A1LW      */
                           unsigned short A0LW    : 2;  /*    A0LW      */
                           unsigned short A4ENDIAN: 1;  /*    A4ENDIAN  */
                           unsigned short DRAM    : 3;  /*    DRAM      */
                           }     BIT;                   /*              */
                    }           BCR1;                   /*              */
              union {                                   /* BCR2         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :16;      /*              */
                           unsigned short     : 6;      /*              */
                           unsigned short A4SZ: 2;      /*    A4SZ      */
                           unsigned short A3SZ: 2;      /*    A3SZ      */
                           unsigned short A2SZ: 2;      /*    A2SZ      */
                           unsigned short A1SZ: 2;      /*    A1SZ      */
                           }     BIT;                   /*              */
                    }           BCR2;                   /*              */
              union {                                   /* WCR1         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short    :16;       /*              */
                           unsigned short IW3: 2;       /*    IW3       */
                           unsigned short IW2: 2;       /*    IW2       */
                           unsigned short IW1: 2;       /*    IW1       */
                           unsigned short IW0: 2;       /*    IW0       */
                           unsigned short W3 : 2;       /*    W3        */
                           unsigned short W2 : 2;       /*    W2        */
                           unsigned short W1 : 2;       /*    W1        */
                           unsigned short W0 : 2;       /*    W0        */
                           }     BIT;                   /*              */
                    }           WCR1;                   /*              */
              union {                                   /* MCR          */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short      :16;     /*              */
                           unsigned short TRP0 : 1;     /*    TRP0      */
                           unsigned short RCD0 : 1;     /*    RCD0      */
                           unsigned short TRWL0: 1;     /*    TRWL0     */
                           unsigned short TRAS : 2;     /*    TRAS      */
                           unsigned short BE   : 1;     /*    BE        */
                           unsigned short RASD : 1;     /*    RASD      */
                           unsigned short TRWL1: 1;     /*    TRWL1     */
                           unsigned short AMX2 : 1;     /*    AMX2      */
                           unsigned short SZ   : 1;     /*    SZ        */
                           unsigned short AMX1 : 1;     /*    AMX1      */
                           unsigned short AMX0 : 1;     /*    AMX0      */
                           unsigned short RFSH : 1;     /*    RFSH      */
                           unsigned short RMODE: 1;     /*    RMODE     */
                           unsigned short TRP1 : 1;     /*    TRP1      */
                           unsigned short RCD1 : 1;     /*    RCD1      */
                           }     BIT;                   /*              */
                    }           MCR;                    /*              */
              union {                                   /* RTCSR        */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short     :16;      /*              */
                           unsigned short     : 8;      /*              */
                           unsigned short CMF : 1;      /*    CMF       */
                           unsigned short CMIE: 1;      /*    CMIE      */
                           unsigned short CKS : 3;      /*    CKS       */
                           unsigned short RRC : 3;      /*    RRC       */
                           }     BIT;                   /*              */
                    }           RTCSR;                  /*              */
              unsigned int      RTCNT;                  /* RTCNT        */
              unsigned int      RTCOR;                  /* RTCOR        */
              union {                                   /* BCR3         */
                    unsigned int LONG;                  /*  Long Access */
                    struct {                            /*  Bit  Access */
                           unsigned short        :16;   /*              */
                           unsigned short        : 4;   /*              */
                           unsigned short  A4LW2 : 1;   /*    A4LW2     */
                           unsigned short  AHLW2 : 1;   /*    AHLW2     */
                           unsigned short  A1LW2 : 1;   /*    A1LW2     */
                           unsigned short  A0LW2 : 1;   /*    A0LW2     */
                           unsigned short  DSWW  : 2;   /*    DSWW      */
                           unsigned short        : 3;   /*              */
                           unsigned short  BASEL : 1;   /*    BASEL     */
                           unsigned short  EDO   : 1;   /*    EDO       */
                           unsigned short  BWE   : 1;   /*    BWE       */
                           }     BIT;                   /*              */
                    }           BCR3;                   /*              */
};                                                      /*              */
union un_sbycr1 {                                       /* union SBYCR1 */
                unsigned char BYTE;                     /*  Byte Access */
                struct {                                /*  Bit  Access */
                       unsigned char SBY  :1;           /*    SBY       */
                       unsigned char HIZ  :1;           /*    HIZ       */
                       unsigned char MSTP5:1;           /*    UBC       */
                       unsigned char MSTP4:1;           /*    DMAC      */
                       unsigned char MSTP3:1;           /*    DSP       */
                       unsigned char      :1;           /*              */
                       unsigned char MSTP1:1;           /*    FRT       */
                       }      BIT;                      /*              */
};                                                      /*              */
union un_sbycr2 {                                       /* union SBYCR2 */
                unsigned char BYTE;                     /*  Byte Access */
                struct {                                /*  Bit  Access */
                       unsigned char       :2;          /*              */
                       unsigned char MSTP11:1;          /*    TPU       */
                       unsigned char MSTP10:1;          /*    SIO2      */
                       unsigned char MSTP9 :1;          /*    SIO1      */
                       unsigned char MSTP8 :1;          /*    SIO0      */
                       unsigned char MSTP7 :1;          /*    SCIF2     */
                       unsigned char MSTP6 :1;          /*    SCIF1     */
                       }      BIT;                      /*              */
};                                                      /*              */
union un_ccr {                                          /* union CCR    */
             unsigned char BYTE;                        /*  Byte Access */
             struct {                                   /*  Bit  Access */
                    unsigned char W :2;                 /*    W         */
                    unsigned char WB:1;                 /*    WB        */
                    unsigned char CP:1;                 /*    CP        */
                    unsigned char TW:1;                 /*    TW        */
                    unsigned char OD:1;                 /*    OD        */
                    unsigned char ID:1;                 /*    ID        */
                    unsigned char CE:1;                 /*    CE        */
                    }      BIT;                         /*              */
};                                                      /*              */
union un_fmr {                                          /* union FMR    */
             unsigned char BYTE;                        /*  Byte Access */
             struct {                                   /*  Bit  Access */
                    unsigned char PLL2ST:1;             /*    PLL2ST    */
                    unsigned char PLL1ST:1;             /*    PLL1ST    */
                    unsigned char CKIOST:1;             /*    CKIOST    */
                    unsigned char       :1;             /*              */
                    unsigned char FR    :4;             /*    FR        */
                    }      BIT;                         /*              */
};                                                      /*              */
#define SIO0   (*(volatile struct st_sio   *)0xFFFFFC00)/* SIO0  Address*/
#define SIO1   (*(volatile struct st_sio   *)0xFFFFFC10)/* SIO1  Address*/
#define SIO2   (*(volatile struct st_sio   *)0xFFFFFC20)/* SIO2  Address*/
#define TPU    (*(volatile struct st_tpu   *)0xFFFFFC40)/* TPU   Address*/
#define TPU0   (*(volatile struct st_tpu0  *)0xFFFFFC50)/* TPU0  Address*/
#define TPU1   (*(volatile struct st_tpu1  *)0xFFFFFC60)/* TPU1  Address*/
#define TPU2   (*(volatile struct st_tpu1  *)0xFFFFFC70)/* TPU2  Address*/
#define PFC    (*(volatile struct st_pfc   *)0xFFFFFC80)/* PFC   Address*/
#define PA     (*(volatile struct st_pa    *)0xFFFFFC84)/* PA    Address*/
#define PB     (*(volatile struct st_pb    *)0xFFFFFC8C)/* PB    Address*/

/* SCIF1,SCIF2はSH7615\SCIDRV.Hと競合するためPSCIF1,PSCIF2とする */
#define PSCIF1  (*(volatile struct st_scif  *)0xFFFFFCC0)/* SCIF1 Address*/
/*#define PSCIF2  (*(volatile struct st_scif  *)0xFFFFFCD4)*/ /* SCIF2 Address*/
#define PSCIF2  (*(volatile struct st_scif  *)0xFFFFFCE0)/* SCIF2 Address*/

#define IrDA1  (*(volatile struct st_irda  *)0xFFFFFCC0)/* IrDA1 Address*/
/*#define IrDA2  (*(volatile struct st_irda  *)0xFFFFFCD4)*/ /* IrDA2 Address*/
#define IrDA2  (*(volatile struct st_irda  *)0xFFFFFCE0)/* IrDA2 Address*/
#define EDMAC  (*(volatile struct st_edmac *)0xFFFFFD00)/* E-DMACAddress*/
#define EC     (*(volatile struct st_ec    *)0xFFFFFD60)/* EtherCAddress*/
#define FRT    (*(volatile struct st_frt   *)0xFFFFFE10)/* FRT   Address*/
#define OCRB    OCRA                                    /* OCRB  Change */
#define INTC   (*(volatile struct st_intc  *)0xFFFFFE40)/* INTC  Address*/
#define DMAC   (*(volatile struct st_dmac  *)0xFFFFFFB0)/* DMAC  Address*/
#define DMAC0  (*(volatile struct st_dmac0 *)0xFFFFFE70)/* DMAC0 Address*/
#define DMAC1  (*(volatile struct st_dmac1 *)0xFFFFFE70)/* DMAC1 Address*/
#define UBC    (*(volatile struct st_ubc   *)0xFFFFFF00)/* UBC   Address*/
#define BSC    (*(volatile struct st_bsc   *)0xFFFFFFC0)/* BSC   Address*/
#define SBYCR1 (*(volatile union  un_sbycr1*)0xFFFFFE91)/* SBYCR1Address*/
#define SBYCR2 (*(volatile union  un_sbycr2*)0xFFFFFE93)/* SBYCR2Address*/
#define CCR    (*(volatile union  un_ccr   *)0xFFFFFE92)/* CCR   Address*/
#define FMR    (*(volatile union  un_fmr   *)0xFFFFFE90)/* FMR   Address*/

#define RSTCSR  ((volatile unsigned short*)0xfffffe82)
#define WTCSR  ((volatile unsigned short*)0xfffffe80)
#define WTCNT WTCSR
