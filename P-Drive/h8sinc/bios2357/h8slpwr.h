/*----------------------------------------------------------------------*
 * File name	: h8slpwr.h		/ H8S LPWR context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8slpwr.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8slpwr.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SLPWR_H)
#define		H8SLPWR_H

union un_sbycr {										/* union SBYCR	*/
			   unsigned char BYTE;						/*	Byte Access */
			   struct { 								/*	Bit  Access */
					  unsigned char SSBY:1; 			/*	  SSBY		*/
					  unsigned char STS :3; 			/*	  STS		*/
					  unsigned char OPE :1; 			/*	  OPE		*/
					  } 	 BIT;						/*				*/
};														/*				*/
														/*				*/
union un_sckcr {										/* union SCKCR	*/
			   unsigned char BYTE;						/*	Byte Access */
			   struct { 								/*	Bit  Access */
					  unsigned char PSTOP:1;			/*	  SSBY		*/
					  unsigned char 	 :4;			/*				*/
					  unsigned char SCK  :3;			/*	  SCK		*/
					  } 	 BIT;						/*				*/
};														/*				*/
														/*				*/
union un_mstpcr {										/* union MSTPCR */
				unsigned int WORD;						/*	Word Access */
				struct {								/*	Byte Access */
					   unsigned char H; 				/*	  MSTPCRH	*/
					   unsigned char L; 				/*	  MSTPCRL	*/
					   }	 BYTE;						/*				*/
				struct {								/*	Bit  Access */
					   unsigned char B15:1; 			/*	  DMAC		*/
					   unsigned char B14:1; 			/*	  DTC		*/
					   unsigned char B13:1; 			/*	  TPU		*/
					   unsigned char B12:1; 			/*	  TMR		*/
					   unsigned char B11:1; 			/*	  PPG		*/
					   unsigned char B10:1; 			/*	  D/A		*/
					   unsigned char B9 :1; 			/*	  A/D		*/
					   unsigned char	:0; 			/*				*/
					   unsigned char B7 :1; 			/*	  SCI2		*/
					   unsigned char B6 :1; 			/*	  SCI1		*/
					   unsigned char B5 :1; 			/*	  SCI0		*/
					   }	 BIT;						/*				*/
};														/*				*/

#define SBYCR	(*(volatile union  un_sbycr *)0xFFFF38) /* SBYCR Address*/
#define SCKCR	(*(volatile union  un_sckcr *)0xFFFF3A) /* SCKCR Address*/
#define MSTPCR	(*(volatile union  un_mstpcr*)0xFFFF3C) /* MSTPCRAddress*/

#endif
