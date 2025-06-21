/*----------------------------------------------------------------------*
 * File name	: h8sintc.h		/ H8S INTC context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sintc.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sintc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SINTC_H)
#define		H8SINTC_H

struct st_intc {										/* struct INTC	*/
			   union {									/* IPRA 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  IRQ0		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  IRQ1		*/
							}	   BIT; 				/*				*/
					 }			IPRA;					/*				*/
			   union {									/* IPRB 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  IRQ2,3	*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  IRQ4,5	*/
							}	   BIT; 				/*				*/
					 }			IPRB;					/*				*/
			   union {									/* IPRC 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  IRQ6,IRQ7 */
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  DTC		*/
							}	   BIT; 				/*				*/
					 }			IPRC;					/*				*/
			   union {									/* IPRD 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  WDT		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  RFSHC 	*/
							}	   BIT; 				/*				*/
					 }			IPRD;					/*				*/
			   union {									/* IPRE 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :5;		/*				*/
							unsigned char LOW :3;		/*	  A/D		*/
							}	   BIT; 				/*				*/
					 }			IPRE;					/*				*/
			   union {									/* IPRF 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  TPU0		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  TPU1		*/
							}	   BIT; 				/*				*/
					 }			IPRF;					/*				*/
			   union {									/* IPRG 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  TPU2		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  TPU3		*/
							}	   BIT; 				/*				*/
					 }			IPRG;					/*				*/
			   union {									/* IPRH 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  TPU4		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  TPU5		*/
							}	   BIT; 				/*				*/
					 }			IPRH;					/*				*/
			   union {									/* IPRI 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  TMR0		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  TMR1		*/
							}	   BIT; 				/*				*/
					 }			IPRI;					/*				*/
			   union {									/* IPRJ 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  DMAC		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  SCI0		*/
							}	   BIT; 				/*				*/
					 }			IPRJ;					/*				*/
			   union {									/* IPRK 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :1;		/*				*/
							unsigned char HIGH:3;		/*	  SCI1		*/
							unsigned char	  :1;		/*				*/
							unsigned char LOW :3;		/*	  SCI2		*/
							}	   BIT; 				/*				*/
					 }			IPRK;					/*				*/
			   char 			wk[93]; 				/*				*/
			   union {									/* ISCR 		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char H;			/*	  ISCRH 	*/
							unsigned char L;			/*	  ISCRL 	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char IRQ7SC:2; 	/*	  IRQ7SC	*/
							unsigned char IRQ6SC:2; 	/*	  IRQ6SC	*/
							unsigned char IRQ5SC:2; 	/*	  IRQ5SC	*/
							unsigned char IRQ4SC:2; 	/*	  IRQ4SC	*/
							unsigned char IRQ3SC:2; 	/*	  IRQ3SC	*/
							unsigned char IRQ2SC:2; 	/*	  IRQ2SC	*/
							unsigned char IRQ1SC:2; 	/*	  IRQ1SC	*/
							unsigned char IRQ0SC:2; 	/*	  IRQ0SC	*/
							}	  BIT;					/*				*/
					 }			ISCR;					/*				*/
			   union {									/* IER			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char IRQ7E:1;		/*	  IRQ7E 	*/
							unsigned char IRQ6E:1;		/*	  IRQ6E 	*/
							unsigned char IRQ5E:1;		/*	  IRQ5E 	*/
							unsigned char IRQ4E:1;		/*	  IRQ4E 	*/
							unsigned char IRQ3E:1;		/*	  IRQ3E 	*/
							unsigned char IRQ2E:1;		/*	  IRQ2E 	*/
							unsigned char IRQ1E:1;		/*	  IRQ1E 	*/
							unsigned char IRQ0E:1;		/*	  IRQ0E 	*/
							}	   BIT; 				/*				*/
					 }			IER;					/*				*/
			   union {									/* ISR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char IRQ7F:1;		/*	  IRQ7F 	*/
							unsigned char IRQ6F:1;		/*	  IRQ6F 	*/
							unsigned char IRQ5F:1;		/*	  IRQ5F 	*/
							unsigned char IRQ4F:1;		/*	  IRQ4F 	*/
							unsigned char IRQ3F:1;		/*	  IRQ3F 	*/
							unsigned char IRQ2F:1;		/*	  IRQ2F 	*/
							unsigned char IRQ1F:1;		/*	  IRQ1F 	*/
							unsigned char IRQ0F:1;		/*	  IRQ0F 	*/
							}	   BIT; 				/*				*/
					 }			ISR;					/*				*/
};														/*				*/

#define INTC	(*(volatile struct st_intc	*)0xFFFEC4) /* INTC  Address*/

#endif
