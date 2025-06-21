/*----------------------------------------------------------------------*
 * File name	: h8sad.h		/ H8S A/D context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sad.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sad.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SAD_H)
#define		H8SAD_H

struct st_ad {											/* struct A/D	*/
			 unsigned int		DRA;					/* ADDRA		*/
			 unsigned int		DRB;					/* ADDRB		*/
			 unsigned int		DRC;					/* ADDRC		*/
			 unsigned int		DRD;					/* ADDRD		*/
			 union {									/* ADCSR		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char ADF :1; 		/*	  ADF		*/
						  unsigned char ADIE:1; 		/*	  ADIE		*/
						  unsigned char ADST:1; 		/*	  ADST		*/
						  unsigned char SCAN:1; 		/*	  SCAN		*/
						  unsigned char CKS :1; 		/*	  CKS		*/
						  unsigned char CH	:3; 		/*	  CH		*/
						  } 	 BIT;					/*				*/
				   }			CSR;					/*				*/
			 union {									/* ADCR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char TRGS:2; 		/*	  TRGS		*/
						  } 	 BIT;					/*				*/
				   }			CR; 					/*				*/
};														/*				*/

#define AD		(*(volatile struct st_ad	*)0xFFFF90) /* A/D	 Address*/

#endif
