/*----------------------------------------------------------------------*
 * File name	: h8sppg.h		/ H8S PPG context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sppg.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sppg.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SPPG_H)
#define		H8SPPG_H

struct st_ppg { 										/* struct PPG	*/
			  union {									/* PCR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char G3CMS:2;		/*	  G3CMS 	*/
						   unsigned char G2CMS:2;		/*	  G2CMS 	*/
						   unsigned char G1CMS:2;		/*	  G1CMS 	*/
						   unsigned char G0CMS:2;		/*	  G0CMS 	*/
						   }	  BIT;					/*				*/
					}			PCR;					/*				*/
			  union {									/* PMR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char G3INV:1;		/*	  G3INV 	*/
						   unsigned char G2INV:1;		/*	  G2INV 	*/
						   unsigned char G1INV:1;		/*	  G1INV 	*/
						   unsigned char G0INV:1;		/*	  G0INV 	*/
						   unsigned char G3NOV:1;		/*	  G3NOV 	*/
						   unsigned char G2NOV:1;		/*	  G2NOV 	*/
						   unsigned char G1NOV:1;		/*	  G1NOV 	*/
						   unsigned char G0NOV:1;		/*	  G0NOV 	*/
						   }	  BIT;					/*				*/
					}			PMR;					/*				*/
			  union {									/* NDER 		*/
					unsigned int WORD;					/*	Word Access */
					struct {							/*	Bit  Access */
						   unsigned char H; 			/*	  NDERH 	*/
						   unsigned char L; 			/*	  NDERL 	*/
						   }	 BYTE;					/*				*/
					struct {							/*	Bit  Access */
						   unsigned char B15:1; 		/*	  NDER15	*/
						   unsigned char B14:1; 		/*	  NDER14	*/
						   unsigned char B13:1; 		/*	  NDER13	*/
						   unsigned char B12:1; 		/*	  NDER12	*/
						   unsigned char B11:1; 		/*	  NDER11	*/
						   unsigned char B10:1; 		/*	  NDER10	*/
						   unsigned char B9 :1; 		/*	  NDER9 	*/
						   unsigned char B8 :1; 		/*	  NDER8 	*/
						   unsigned char B7:1;			/*	  NDER7 	*/
						   unsigned char B6:1;			/*	  NDER6 	*/
						   unsigned char B5:1;			/*	  NDER5 	*/
						   unsigned char B4:1;			/*	  NDER4 	*/
						   unsigned char B3:1;			/*	  NDER3 	*/
						   unsigned char B2:1;			/*	  NDER2 	*/
						   unsigned char B1:1;			/*	  NDER1 	*/
						   unsigned char B0:1;			/*	  NDER0 	*/
						   }	  BIT;					/*				*/
					}			NDER;					/*				*/
			  union {									/* PODR 		*/
					unsigned int WORD;					/*	Word Access */
					struct {							/*	Bit  Access */
						   unsigned char H; 			/*	  PODRH 	*/
						   unsigned char L; 			/*	  PODRL 	*/
						   }	 BYTE;					/*				*/
					struct {							/*	Bit  Access */
						   unsigned char B15:1; 		/*	  NDER15	*/
						   unsigned char B14:1; 		/*	  NDER14	*/
						   unsigned char B13:1; 		/*	  NDER13	*/
						   unsigned char B12:1; 		/*	  NDER12	*/
						   unsigned char B11:1; 		/*	  NDER11	*/
						   unsigned char B10:1; 		/*	  NDER10	*/
						   unsigned char B9 :1; 		/*	  NDER9 	*/
						   unsigned char B8 :1; 		/*	  NDER8 	*/
						   unsigned char B7:1;			/*	  NDER7 	*/
						   unsigned char B6:1;			/*	  NDER6 	*/
						   unsigned char B5:1;			/*	  NDER5 	*/
						   unsigned char B4:1;			/*	  NDER4 	*/
						   unsigned char B3:1;			/*	  NDER3 	*/
						   unsigned char B2:1;			/*	  NDER2 	*/
						   unsigned char B1:1;			/*	  NDER1 	*/
						   unsigned char B0:1;			/*	  NDER0 	*/
						   }	 BIT;					/*				*/
					}			PODR;					/*				*/
			  union {									/* NDRH (H'4C)  */
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B15:1; 		/*	  NDR15 	*/
						   unsigned char B14:1; 		/*	  NDR14 	*/
						   unsigned char B13:1; 		/*	  NDR13 	*/
						   unsigned char B12:1; 		/*	  NDR12 	*/
						   unsigned char B11:1; 		/*	  NDR11 	*/
						   unsigned char B10:1; 		/*	  NDR10 	*/
						   unsigned char B9 :1; 		/*	  NDR9		*/
						   unsigned char B8 :1; 		/*	  NDR8		*/
						   }	  BIT;					/*				*/
					}			NDRH1;					/*				*/
			  union {									/* NDRL (H'4D)  */
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  NDR7		*/
						   unsigned char B6:1;			/*	  NDR6		*/
						   unsigned char B5:1;			/*	  NDR5		*/
						   unsigned char B4:1;			/*	  NDR4		*/
						   unsigned char B3:1;			/*	  NDR3		*/
						   unsigned char B2:1;			/*	  NDR2		*/
						   unsigned char B1:1;			/*	  NDR1		*/
						   unsigned char B0:1;			/*	  NDR0		*/
						   }	  BIT;					/*				*/
					}			NDRL1;					/*				*/
			  union {									/* NDRH (H'4E)  */
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk :4; 		/*				*/
						   unsigned char B11:1; 		/*	  NDR11 	*/
						   unsigned char B10:1; 		/*	  NDR10 	*/
						   unsigned char B9 :1; 		/*	  NDR9		*/
						   unsigned char B8 :1; 		/*	  NDR8		*/
						   }	  BIT;					/*				*/
					}			NDRH2;					/*				*/
			  union {									/* NDRL (H'4F)  */
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk:4;			/*				*/
						   unsigned char B3:1;			/*	  NDR3		*/
						   unsigned char B2:1;			/*	  NDR2		*/
						   unsigned char B1:1;			/*	  NDR1		*/
						   unsigned char B0:1;			/*	  NDR0		*/
						   }	  BIT;					/*				*/
					}			NDRL2;					/*				*/
};														/*				*/

#define PPG 	(*(volatile struct st_ppg	*)0xFFFF46) /* PPG	 Address*/

#endif
