/*----------------------------------------------------------------------*
 * File name	: h8stpu.h		/ H8S TPU context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8stpu.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8stpu.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8STPU_H)
#define		H8STPU_H

struct st_tpu { 										/* struct TPU	*/
			  union {									/* TSTR 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk  :2;		/*				*/
						   unsigned char CST5:1;		/*	  CST5		*/
						   unsigned char CST4:1;		/*	  CST4		*/
						   unsigned char CST3:1;		/*	  CST3		*/
						   unsigned char CST2:1;		/*	  CST2		*/
						   unsigned char CST1:1;		/*	  CST1		*/
						   unsigned char CST0:1;		/*	  CST0		*/
						   }	  BIT;					/*				*/
					}			TSTR;					/*				*/
			  union {									/* TSYR 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk   :2;		/*				*/
						   unsigned char SYNC5:1;		/*	  SYNC5 	*/
						   unsigned char SYNC4:1;		/*	  SYNC4 	*/
						   unsigned char SYNC3:1;		/*	  SYNC3 	*/
						   unsigned char SYNC2:1;		/*	  SYNC2 	*/
						   unsigned char SYNC1:1;		/*	  SYNC1 	*/
						   unsigned char SYNC0:1;		/*	  SYNC0 	*/
						   }	  BIT;					/*				*/
					}			TSYR;					/*				*/
};														/*				*/
														/*				*/
struct st_tpu0 {										/* struct TPU0	*/
			   union {									/* TCR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char CCLR:3;		/*	  CCLR		*/
							unsigned char CKEG:2;		/*	  CKEG		*/
							unsigned char TPSC:3;		/*	  TPSC		*/
							}	   BIT; 				/*				*/
					 }			TCR;					/*				*/
			   union {									/* TMDR 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk :2;		/*				*/
							unsigned char BFB:1;		/*	  BFB		*/
							unsigned char BFA:1;		/*	  BFA		*/
							unsigned char MD :4;		/*	  MD		*/
							}	   BIT; 				/*				*/
					 }			TMDR;					/*				*/
			   union {									/* TIOR 		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char H;			/*	  TIORH 	*/
							unsigned char L;			/*	  TIORL 	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char IOB:4;		/*	  IOB		*/
							unsigned char IOA:4;		/*	  IOA		*/
							unsigned char IOD:4;		/*	  IOD		*/
							unsigned char IOC:4;		/*	  IOC		*/
							}	  BIT;					/*				*/
					 }			TIOR;					/*				*/
			   union {									/* TIER 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char TTGE :1;		/*	  TTGE		*/
							unsigned char	   :2;		/*				*/
							unsigned char TCIEV:1;		/*	  TCIEV 	*/
							unsigned char TGIED:1;		/*	  TGIED 	*/
							unsigned char TGIEC:1;		/*	  TGIEC 	*/
							unsigned char TGIEB:1;		/*	  TGIEB 	*/
							unsigned char TGIEA:1;		/*	  TGIEA 	*/
							}	   BIT; 				/*				*/
					 }			TIER;					/*				*/
			   union {									/* TSR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :3;		/*				*/
							unsigned char TCFV:1;		/*	  TCFV		*/
							unsigned char TGFD:1;		/*	  TGFD		*/
							unsigned char TGFC:1;		/*	  TGFC		*/
							unsigned char TGFB:1;		/*	  TGFB		*/
							unsigned char TGFA:1;		/*	  TGFA		*/
							}	   BIT; 				/*				*/
					 }			TSR;					/*				*/
			   unsigned int 	TCNT;					/* TCNT 		*/
			   unsigned int 	TGRA;					/* TGRA 		*/
			   unsigned int 	TGRB;					/* TGRB 		*/
			   unsigned int 	TGRC;					/* TGRC 		*/
			   unsigned int 	TGRD;					/* TGRD 		*/
};														/*				*/
														/*				*/
struct st_tpu1 {										/* struct TPU1	*/
			   union {									/* TCR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char CCLR:2;		/*	  CCLR		*/
							unsigned char CKEG:2;		/*	  CKEG		*/
							unsigned char TPSC:3;		/*	  TPSC		*/
							}	   BIT; 				/*				*/
					 }			TCR;					/*				*/
			   union {									/* TMDR 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk :4;		/*				*/
							unsigned char MD :4;		/*	  MD		*/
							}	   BIT; 				/*				*/
					 }			TMDR;					/*				*/
			   union {									/* TIOR 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char IOB:4;		/*	  IOB		*/
							unsigned char IOA:4;		/*	  IOA		*/
							}	   BIT; 				/*				*/
					 }			TIOR;					/*				*/
			   char 			wk; 					/*				*/
			   union {									/* TIER 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char TTGE :1;		/*	  TTGE		*/
							unsigned char	   :1;		/*				*/
							unsigned char TCIEU:1;		/*	  TCIEU 	*/
							unsigned char TCIEV:1;		/*	  TCIEV 	*/
							unsigned char	   :2;		/*				*/
							unsigned char TGIEB:1;		/*	  TGIEB 	*/
							unsigned char TGIEA:1;		/*	  TGIEA 	*/
							}	   BIT; 				/*				*/
					 }			TIER;					/*				*/
			   union {									/* TSR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char TCFD:1;		/*	  TCFD		*/
							unsigned char	  :1;		/*				*/
							unsigned char TCFU:1;		/*	  TCFU		*/
							unsigned char TCFV:1;		/*	  TCFV		*/
							unsigned char	  :2;		/*				*/
							unsigned char TGFB:1;		/*	  TGFB		*/
							unsigned char TGFA:1;		/*	  TGFA		*/
							}	   BIT; 				/*				*/
					 }			TSR;					/*				*/
			   unsigned int 	TCNT;					/* TCNT 		*/
			   unsigned int 	TGRA;					/* TGRA 		*/
			   unsigned int 	TGRB;					/* TGRB 		*/
};														/*				*/


#define TPU_ADRS 	((volatile struct st_tpu  *)0xFFFFC0) /* TPU   Address*/
#define TPU0_ADRS	((volatile struct st_tpu0 *)0xFFFFD0) /* TPU0  Address*/
#define TPU1_ADRS	((volatile struct st_tpu1 *)0xFFFFE0) /* TPU1  Address*/
#define TPU2_ADRS	((volatile struct st_tpu1 *)0xFFFFF0) /* TPU2  Address*/
#define TPU3_ADRS	((volatile struct st_tpu0 *)0xFFFE80) /* TPU3  Address*/
#define TPU4_ADRS	((volatile struct st_tpu1 *)0xFFFE90) /* TPU4  Address*/
#define TPU5_ADRS	((volatile struct st_tpu1 *)0xFFFEA0) /* TPU5  Address*/


#define TPU 	(*TPU_ADRS)	 /* TPU	Data */
#define TPU0	(*TPU0_ADRS) /* TPU0 Data */
#define TPU1	(*TPU1_ADRS) /* TPU1 Data */
#define TPU2	(*TPU2_ADRS) /* TPU2 Data */
#define TPU3	(*TPU3_ADRS) /* TPU3 Data */
#define TPU4	(*TPU4_ADRS) /* TPU4 Data */
#define TPU5	(*TPU5_ADRS) /* TPU5 Data */

#define st_tpu3 st_tpu0 						/* Change Struct TPU3	*/
#define st_tpu2 st_tpu1 						/* Change Struct TPU2	*/
#define st_tpu4 st_tpu1 						/* Change Struct TPU4	*/
#define st_tpu5 st_tpu1 						/* Change Struct TPU5	*/

#endif
