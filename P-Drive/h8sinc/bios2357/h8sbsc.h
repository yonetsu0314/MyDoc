/*----------------------------------------------------------------------*
 * File name	: h8sbsc.h		/ H8S BSC context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sbsc.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sbsc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SBSC_H)
#define		H8SBSC_H

struct st_bsc { 										/* struct BSC	*/
			  union {									/* ABWCR		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  Bit 7 	*/
						   unsigned char B6:1;			/*	  Bit 6 	*/
						   unsigned char B5:1;			/*	  Bit 5 	*/
						   unsigned char B4:1;			/*	  Bit 4 	*/
						   unsigned char B3:1;			/*	  Bit 3 	*/
						   unsigned char B2:1;			/*	  Bit 2 	*/
						   unsigned char B1:1;			/*	  Bit 1 	*/
						   unsigned char B0:1;			/*	  Bit 0 	*/
						   }	  BIT;					/*				*/
					}			ABWCR;					/*				*/
			  union {									/* ASTCR		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char B7:1;			/*	  Bit 7 	*/
						   unsigned char B6:1;			/*	  Bit 6 	*/
						   unsigned char B5:1;			/*	  Bit 5 	*/
						   unsigned char B4:1;			/*	  Bit 4 	*/
						   unsigned char B3:1;			/*	  Bit 3 	*/
						   unsigned char B2:1;			/*	  Bit 2 	*/
						   unsigned char B1:1;			/*	  Bit 1 	*/
						   unsigned char B0:1;			/*	  Bit 0 	*/
						   }	  BIT;					/*				*/
					}			ASTCR;					/*				*/
			  union {									/* WCR			*/
					unsigned int WORD;					/*	Word Access */
					struct {							/*	Byte Access */
						   unsigned char H; 			/*	  WCRH		*/
						   unsigned char L; 			/*	  WCRL		*/
						   }	 BYTE;					/*				*/
					struct {							/*	Bit  Access */
						   unsigned char W7:2;			/*	  W7		*/
						   unsigned char W6:2;			/*	  W6		*/
						   unsigned char W5:2;			/*	  W5		*/
						   unsigned char W4:2;			/*	  W4		*/
						   unsigned char W3:2;			/*	  W3		*/
						   unsigned char W2:2;			/*	  W2		*/
						   unsigned char W1:2;			/*	  W1		*/
						   unsigned char W0:2;			/*	  W0		*/
						   }	 BIT;					/*				*/
					}			WCR;					/*				*/
			  union {									/* BCR			*/
					unsigned int WORD;					/*	Word Access */
					struct {							/*	Byte Access */
						   unsigned char H; 			/*	  BCRH		*/
						   unsigned char L; 			/*	  BCRL		*/
						   }	 BYTE;					/*				*/
					struct {							/*	Bit  Access */
						   unsigned char ICIS1 :1;		/*	  ICIS1 	*/
						   unsigned char ICIS0 :1;		/*	  ICIS0 	*/
						   unsigned char BRSTRM:1;		/*	  BRSTRM	*/
						   unsigned char BRSTS1:1;		/*	  BRSTS1	*/
						   unsigned char BRSTS0:1;		/*	  BRSTS0	*/
						   unsigned char RMTS  :3;		/*	  RMTS		*/
						   unsigned char BRLE  :1;		/*	  BRLE		*/
						   unsigned char BREQOE:1;		/*	  BREQOE	*/
						   unsigned char EAE   :1;		/*				*/
						   unsigned char LCASS :1;		/*	  LCASS 	*/
						   unsigned char DDS   :1;		/*	  DDS		*/
						   unsigned char	   :1;		/*				*/
						   unsigned char WDBE  :1;		/*	  WDBE		*/
						   unsigned char WAITE :1;		/*	  WAITE 	*/
						   }	 BIT;					/*				*/
					}			BCR;					/*				*/
			  union {									/* MCR			*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char TPC :1;		/*	  TPC		*/
						   unsigned char BE  :1;		/*	  BE		*/
						   unsigned char RCDM:1;		/*	  RCDM		*/
						   unsigned char CW2 :1;		/*	  CW2		*/
						   unsigned char MXC :2;		/*	  MXC		*/
						   unsigned char RLW :2;		/*	  RLW		*/
						   }	  BIT;					/*				*/
					}			MCR;					/*				*/
			  union {									/* DRAMCR		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char RFSHE:1;		/*	  RFCHE 	*/
						   unsigned char RCW  :1;		/*	  RCW		*/
						   unsigned char RMODE:1;		/*	  RMODE 	*/
						   unsigned char CMF  :1;		/*	  CMF		*/
						   unsigned char CMIE :1;		/*	  CMIE		*/
						   unsigned char CKS  :3;		/*	  CKS		*/
						   }	  BIT;					/*				*/
					}			DRAMCR; 				/*				*/
			  unsigned char 	RTCNT;					/* RTCNT		*/
			  unsigned char 	RTCOR;					/* RTCOR		*/
			  char				wk; 					/*				*/
			  union {									/* RAMER		*/
					unsigned char BYTE; 				/*	Byte Access */
					struct {							/*	Bit  Access */
						   unsigned char wk  :5;		/*				*/
						   unsigned char RAMS:3;		/*	  RAMS		*/
						   }	  BIT;					/*				*/
					}			RAMER;					/*				*/
};														/*				*/

#define BSC 	(*(volatile struct st_bsc	*)0xFFFED0) /* BSC	 Address*/

#endif
