/*----------------------------------------------------------------------*
 * File name	: h8ssci.h		/ H8S SCI context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8ssci.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8ssci.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SSCI_H)
#define		H8SSCI_H

struct st_sci { 										/* struct SCI	*/
			  union {									/* SMR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char CA  :1;		/*	  C/A		*/
						   unsigned char CHR :1;		/*	  CHR		*/
						   unsigned char PRE :1;		/*	  PE		*/
						   unsigned char OE  :1;		/*	  O/E		*/
						   unsigned char STOP:1;		/*	  STOP		*/
						   unsigned char MP  :1;		/*	  MP		*/
						   unsigned char CKS :2;		/*	  CKS		*/
						   }	  BIT;					/*				*/
					}			SMR;					/*				*/
			  unsigned char 	BRR;					/* BRR			*/
			  union {									/* SCR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char TIE :1;		/*	  TIE		*/
						   unsigned char RIE :1;		/*	  RIE		*/
						   unsigned char TE  :1;		/*	  TE		*/
						   unsigned char RE  :1;		/*	  RE		*/
						   unsigned char MPIE:1;		/*	  MPIE		*/
						   unsigned char TEIE:1;		/*	  TEIE		*/
						   unsigned char CKE :2;		/*	  CKE		*/
						   }	  BIT;					/*				*/
					}			SCR;					/*				*/
			  unsigned char 	TDR;					/* TDR			*/
			  union {									/* SSR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char TDRE:1;		/*	  TDRE		*/
						   unsigned char RDRF:1;		/*	  RDRF		*/
						   unsigned char ORER:1;		/*	  ORER		*/
						   unsigned char FER :1;		/*	  FER		*/
						   unsigned char PER :1;		/*	  PER		*/
						   unsigned char TEND:1;		/*	  TEND		*/
						   unsigned char MPB :1;		/*	  MPB		*/
						   unsigned char MPBT:1;		/*	  MPBT		*/
						   }	  BIT;					/*				*/
					}			SSR;					/*				*/
			  unsigned char 	RDR;					/* RDR			*/
			  union {									/* SCMR 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk  :4;		/*				*/
						   unsigned char SDIR:1;		/*	  SDIR		*/
						   unsigned char SINV:1;		/*	  SINV		*/
						   unsigned char	 :1;		/*				*/
						   unsigned char SMIF:1;		/*	  SMIF		*/
						   }	  BIT;					/*				*/
					}			SCMR;					/*				*/
			  char				wk; 					/*				*/
};														/*				*/

#define SCI0ADRS	(volatile struct st_sci *)0xFFFF78 /* SCI0  Address*/
#define SCI1ADRS	(volatile struct st_sci *)0xFFFF80 /* SCI1  Address*/
#define SCI2ADRS	(volatile struct st_sci *)0xFFFF88 /* SCI2  Address*/

#define SCI0	(*SCI0ADRS) /* SCI0 */
#define SCI1	(*SCI1ADRS) /* SCI1 */
#define SCI2	(*SCI2ADRS) /* SCI2 */

#endif
