/*----------------------------------------------------------------------*
 * File name	: h8sdtc.h		/ H8S DTC context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sdtc.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sdtc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SDTC_H)
#define		H8SDTC_H

struct st_dtc { 										/* struct DTC	*/
			  union {									/* EA			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  IRQ0		*/
						   unsigned char B6:1;			/*	  IRQ1		*/
						   unsigned char B5:1;			/*	  IRQ2		*/
						   unsigned char B4:1;			/*	  IRQ3		*/
						   unsigned char B3:1;			/*	  IRQ4		*/
						   unsigned char B2:1;			/*	  IRQ5		*/
						   unsigned char B1:1;			/*	  IRQ6		*/
						   unsigned char B0:1;			/*	  IRQ7		*/
						   }	  BIT;					/*				*/
					}			EA; 					/*				*/
			  union {									/* EB			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk:1;			/*				*/
						   unsigned char B6:1;			/*	  ADI		*/
						   unsigned char B5:1;			/*	  TGI0A 	*/
						   unsigned char B4:1;			/*	  TGI0B 	*/
						   unsigned char B3:1;			/*	  TGI0C 	*/
						   unsigned char B2:1;			/*	  TGI0D 	*/
						   unsigned char B1:1;			/*	  TGI1A 	*/
						   unsigned char B0:1;			/*	  TGI1B 	*/
						   }	  BIT;					/*				*/
					}			EB; 					/*				*/
			  union {									/* EC			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  TGI2A 	*/
						   unsigned char B6:1;			/*	  TGI2B 	*/
						   unsigned char B5:1;			/*	  TGI3A 	*/
						   unsigned char B4:1;			/*	  TGI3B 	*/
						   unsigned char B3:1;			/*	  TGI3C 	*/
						   unsigned char B2:1;			/*	  TGI3D 	*/
						   unsigned char B1:1;			/*	  TGI4A 	*/
						   unsigned char B0:1;			/*	  TGI4B 	*/
						   }	  BIT;					/*				*/
					}			EC; 					/*				*/
			  union {									/* ED			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk:2;			/*				*/
						   unsigned char B5:1;			/*	  TGI5A 	*/
						   unsigned char B4:1;			/*	  TGI5B 	*/
						   unsigned char B3:1;			/*	  CMI0A 	*/
						   unsigned char B2:1;			/*	  CMI0B 	*/
						   unsigned char B1:1;			/*	  CMI1A 	*/
						   unsigned char B0:1;			/*	  CMI1B 	*/
						   }	  BIT;					/*				*/
					}			ED; 					/*				*/
			  union {									/* EE			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk:4;			/*				*/
						   unsigned char B3:1;			/*	  RXI0		*/
						   unsigned char B2:1;			/*	  TXI0		*/
						   unsigned char B1:1;			/*	  RXI1		*/
						   unsigned char B0:1;			/*	  TXI1		*/
						   }	  BIT;					/*				*/
					}			EE; 					/*				*/
			  union {									/* EF			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  RXI2		*/
						   unsigned char B6:1;			/*	  TXI2		*/
						   }	  BIT;					/*				*/
					}			EF; 					/*				*/
			  char				wk; 					/*				*/
			  union {									/* VECR 		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char SWDTE:1;		/*	  SWDTE 	*/
						   unsigned char DTVEC:7;		/*	  DTVEC 	*/
						   }	  BIT;					/*				*/
					}			VECR;					/*				*/
};														/*				*/

#define DTC 	(*(volatile struct st_dtc	*)0xFFFF30) /* DTC	 Address*/

#endif
