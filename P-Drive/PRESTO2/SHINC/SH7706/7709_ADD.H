/*----------------------------------------------------------------------
 * File name	: 7709_add.h	/ 					
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shinc/sh7706/RCS/7709_add.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: 7709_add.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2011, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN
 *		(077) 553-4141
 *----------------------------------------------------------------------
 * Function								
 *----------------------------------------------------------------------
 */
/************************************************************************/
/*      SH7709S Series Include File                         Ver 1.1      */
/************************************************************************/
struct st_irda {                                        /* struct IrDA  */
			  union {                                   /* SMR          */
					unsigned char  BYTE;                /*  Byte Access */
					struct {                            /*  Bit  Access */
						   unsigned char IRMOD:1;       /*    IRMOD     */
						   unsigned char ICK  :4;       /*    ICK       */
						   unsigned char PSEL :1;       /*    PSEL      */
						   unsigned char CKS  :2;       /*    CKS       */
						   }       BIT;                 /*              */
					}         SCSMR;                    /*              */
			  char               wk1;                   /*              */
			  unsigned char   SCBRR;                    /* BRR          */
			  char               wk2;                   /*              */
			  union {                                   /* SCR          */
					unsigned char  BYTE;                /*  Byte Access */
					struct {                            /*  Bit  Access */
						   unsigned char TIE :1;        /*    TIE       */
						   unsigned char RIE :1;        /*    RIE       */
						   unsigned char TE  :1;        /*    TE        */
						   unsigned char RE  :1;        /*    RE        */
						   unsigned char     :1;        /*              */
						   unsigned char     :1;        /*              */
						   unsigned char CKE :2;        /*    CKE       */
						   }       BIT;                 /*              */
					}         SCSCR;                    /*              */
			  char               wk3;                   /*              */
			  unsigned char   SCFTDR;                   /* TDR          */
			  char               wk4;                   /*              */
			  union {                                   /* SSR          */
					unsigned short  WORD;               /*  Word Access */
					struct {                            /*  Bit  Access */
						   unsigned short PER3:1;       /*    PER3      */
						   unsigned short PER2:1;       /*    PER2      */
						   unsigned short PER1:1;       /*    PER1      */
						   unsigned short PER0:1;       /*    PER0      */
						   unsigned short FER3:1;       /*    FER3      */
						   unsigned short FER2:1;       /*    FER2      */
						   unsigned short FER1:1;       /*    FER1      */
						   unsigned short FER0:1;       /*    FER0      */
						   unsigned short ER  :1;       /*    ER        */
						   unsigned short TEND:1;       /*    TEND      */
						   unsigned short TDFE:1;       /*    TDFE      */
						   unsigned short BRK :1;       /*    BRK       */
						   unsigned short FER :1;       /*    FER       */
						   unsigned short PER :1;       /*    PER       */
						   unsigned short RDF :1;       /*    RDF       */
						   unsigned short DR  :1;       /*    DR        */
						   }       BIT;                 /*              */
					}         SCSSR;                    /*              */
			  unsigned char   SCFRDR;                   /* RDR          */
			  char               wk5;                   /*              */
			  union {                                   /* SCFCR        */
					unsigned char  BYTE;                /*  Byte Access */
					struct {                            /*  Bit  Access */
						   unsigned char RTRG :2;       /*    RTRG      */
						   unsigned char TTGR :2;       /*    TTRG      */
						   unsigned char MCE  :1;       /*    MCE       */
						   unsigned char TFRST:1;       /*    TFRST     */
						   unsigned char RFRST:1;       /*    RFRST     */
						   unsigned char LOOP :1;       /*    LOOP      */
						   }       BIT;                 /*              */
					}         SCFCR;                    /*              */
			  char               wk6;                   /*              */
			  union {                                   /* SSR          */
					unsigned short WORD;                /*  Word Access */
					struct {                            /*  Byte Access */
						   unsigned short    :3 ;       /*              */
						   unsigned short R  :5 ;       /*    R         */
						   unsigned short    :3 ;       /*              */
						   unsigned short T  :5 ;       /*    T         */
						   }       BIT;                 /*              */
					}         SCFDR;                    /*              */
};                                                      /*              */

struct st_pfc_kl {                                      /* struct PFC   */
              union {                                   /* PKCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PK7MD:2;      /*    PK7MD     */
                           unsigned short PK6MD:2;      /*    PK6MD     */
                           unsigned short PK5MD:2;      /*    PK5MD     */
                           unsigned short PK4MD:2;      /*    PK4MD     */
                           unsigned short PK3MD:2;      /*    PK3MD     */
                           unsigned short PK2MD:2;      /*    PK2MD     */
                           unsigned short PK1MD:2;      /*    PK1MD     */
                           unsigned short PK0MD:2;      /*    PK0MD     */
                           }       BIT;                 /*              */
                    }           PKCR;                   /*              */
              union {                                   /* PLCR         */
                    unsigned short WORD;                /*  Word Access */
                    struct {                            /*  Bit  Access */
                           unsigned short PL7MD:2;      /*    PL7MD     */
                           unsigned short PL6MD:2;      /*    PL6MD     */
                           unsigned short PL5MD:2;      /*    PL5MD     */
                           unsigned short PL4MD:2;      /*    PL4MD     */
                           unsigned short PL3MD:2;      /*    PL3MD     */
                           unsigned short PL2MD:2;      /*    PL2MD     */
                           unsigned short PL1MD:2;      /*    PL1MD     */
                           unsigned short PL0MD:2;      /*    PL0MD     */
                           }       BIT;                 /*              */
                    }           PLCR;                   /*              */
};                                                      /*              */

struct st_pk {                                          /* struct PJ    */
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
struct st_pl {                                          /* struct PJ    */
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


#define IRDA  (*(volatile struct st_irda  *)0xA4000140) /* IrDA  Address*/

#define PFC_KL  (*(volatile struct st_pfc_kl *)0xA4000112) /* PFC_KL   Address*/
#define PK    	(*(volatile struct st_pk     *)0xA4000132) 	/* PK    Address*/
#define PL    	(*(volatile struct st_pl     *)0xA4000134) 	/* PL    Address*/
