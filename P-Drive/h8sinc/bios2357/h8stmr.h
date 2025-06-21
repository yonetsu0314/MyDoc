/*----------------------------------------------------------------------*
 * File name	: h8stmr.h		/ H8S TMR context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8stmr.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8stmr.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8STMR_H)
#define		H8STMR_H

struct st_tmr { 										/* struct TMR	*/
			  union {									/* TCR0 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char CMIEB:1;		/*	  CMIEB 	*/
						   unsigned char CMIEA:1;		/*	  CMIEA 	*/
						   unsigned char OVIE :1;		/*	  OVIE		*/
						   unsigned char CCLR :2;		/*	  CCLR		*/
						   unsigned char CKS  :3;		/*	  CKS		*/
						   }	  BIT;					/*				*/
					}			TCR0;					/*				*/
			  union {									/* TCR1 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char CMIEB:1;		/*	  CMIEB 	*/
						   unsigned char CMIEA:1;		/*	  CMIEA 	*/
						   unsigned char OVIE :1;		/*	  OVIE		*/
						   unsigned char CCLR :2;		/*	  CCLR		*/
						   unsigned char CKS  :3;		/*	  CKS		*/
						   }	  BIT;					/*				*/
					}			TCR1;					/*				*/
			  union {									/* TCSR0		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char CMFB:1;		/*	  CMFB		*/
						   unsigned char CMFA:1;		/*	  CMFA		*/
						   unsigned char OVF :1;		/*	  OVF		*/
						   unsigned char ADTE:1;		/*	  ADTE		*/
						   unsigned char OS  :4;		/*	  OS		*/
						   }	  BIT;					/*				*/
					}			TCSR0;					/*				*/
			  union {									/* TCSR1		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char CMFB:1;		/*	  CMFB		*/
						   unsigned char CMFA:1;		/*	  CMFA		*/
						   unsigned char OVF :1;		/*	  OVF		*/
						   unsigned char	 :1;		/*				*/
						   unsigned char OS  :4;		/*	  OS		*/
						   }	  BIT;					/*				*/
					}			TCSR1;					/*				*/
			  unsigned int		TCORA;					/* TCORA		*/
			  unsigned int		TCORB;					/* TCORB		*/
			  unsigned int		TCNT;					/* TCNT 		*/
};														/*				*/
														/*				*/
struct st_tmr0 {										/* struct TMR0	*/
			   union {									/* TCR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char CMIEB:1;		/*	  CMIEB 	*/
							unsigned char CMIEA:1;		/*	  CMIEA 	*/
							unsigned char OVIE :1;		/*	  OVIE		*/
							unsigned char CCLR :2;		/*	  CCLR		*/
							unsigned char CKS  :3;		/*	  CKS		*/
							}	   BIT; 				/*				*/
					 }			TCR;					/*				*/
			   char 			wk1;					/*				*/
			   union {									/* TCSR 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char CMFB:1;		/*	  CMFB		*/
							unsigned char CMFA:1;		/*	  CMFA		*/
							unsigned char OVF :1;		/*	  OVF		*/
							unsigned char ADTE:1;		/*	  ADTE		*/
							unsigned char OS  :4;		/*	  OS		*/
							}	   BIT; 				/*				*/
					 }			TCSR;					/*				*/
			   char 			wk2;					/*				*/
			   unsigned char	TCORA;					/* TCORA		*/
			   char 			wk3;					/*				*/
			   unsigned char	TCORB;					/* TCORB		*/
			   char 			wk4;					/*				*/
			   unsigned char	TCNT;					/* TCNT 		*/
};														/*				*/
														/*				*/
struct st_tmr1 {										/* struct TMR1	*/
			   union {									/* TCR			*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char CMIEB:1;		/*	  CMIEB 	*/
							unsigned char CMIEA:1;		/*	  CMIEA 	*/
							unsigned char OVIE :1;		/*	  OVIE		*/
							unsigned char CCLR :2;		/*	  CCLR		*/
							unsigned char CKS  :3;		/*	  CKS		*/
							}	   BIT; 				/*				*/
					 }			TCR;					/*				*/
			   char 			wk1;					/*				*/
			   union {									/* TCSR 		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char CMFB:1;		/*	  CMFB		*/
							unsigned char CMFA:1;		/*	  CMFA		*/
							unsigned char OVF :1;		/*	  OVF		*/
							unsigned char	  :1;		/*				*/
							unsigned char OS  :4;		/*	  OS		*/
							}	   BIT; 				/*				*/
					 }			TCSR;					/*				*/
			   char 			wk2;					/*				*/
			   unsigned char	TCORA;					/* TCORA		*/
			   char 			wk3;					/*				*/
			   unsigned char	TCORB;					/* TCORB		*/
			   char 			wk4;					/*				*/
			   unsigned char	TCNT;					/* TCNT 		*/
};														/*				*/

#define TMR 	(*(volatile struct st_tmr	*)0xFFFFB0) /* TMR	 Address*/
#define TMR0	(*(volatile struct st_tmr0	*)0xFFFFB0) /* TMR0  Address*/
#define TMR1	(*(volatile struct st_tmr1	*)0xFFFFB1) /* TMR1  Address*/

#endif
