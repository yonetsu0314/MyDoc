/*----------------------------------------------------------------------*
 * File name	: h8sflsh.h		/ H8S FLSH context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sflsh.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sflsh.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SFLSH_H)
#define		H8SFLSH_H

struct st_flash {										/* struct FLASH */
				union { 								/* FLMCR1		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char FWE:1;		/*	  FWE		*/
							 unsigned char SWE:1;		/*	  SWE		*/
							 unsigned char	  :2;		/*				*/
							 unsigned char EV :1;		/*	  EV		*/
							 unsigned char PV :1;		/*	  PV		*/
							 unsigned char E  :1;		/*	  E 		*/
							 unsigned char P  :1;		/*	  P 		*/
							 }		BIT;				/*				*/
					  } 		FLMCR1; 				/*				*/
				union { 								/* FLMCR2		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char FLER:1;		/*	  FLER		*/
							 unsigned char	   :5;		/*				*/
							 unsigned char ESU :1;		/*	  ESU		*/
							 unsigned char PSU :1;		/*	  PSU		*/
							 }		BIT;				/*				*/
					  } 		FLMCR2; 				/*				*/
				union { 								/* EBR1 		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char wk :6;		/*				*/
							 unsigned char EB9:1;		/*	  EB9		*/
							 unsigned char EB8:1;		/*	  EB8		*/
							 }		BIT;				/*				*/
					  } 		EBR1;					/*				*/
				union { 								/* EBR2 		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char EB7:1;		/*	  EB7		*/
							 unsigned char EB6:1;		/*	  EB6		*/
							 unsigned char EB5:1;		/*	  EB5		*/
							 unsigned char EB4:1;		/*	  EB4		*/
							 unsigned char EB3:1;		/*	  EB3		*/
							 unsigned char EB2:1;		/*	  EB2		*/
							 unsigned char EB1:1;		/*	  EB1		*/
							 unsigned char EB0:1;		/*	  EB0		*/
							 }		BIT;				/*				*/
					  } 		EBR2;					/*				*/
};														/*				*/


#define FLASH	(*(volatile struct st_flash *)0xFFFFC8) /* FLASH Address*/

#endif
