/*----------------------------------------------------------------------*
 * File name	: h8sda.h		/ H8S D/A context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sda.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sda.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SDA_H)
#define		H8SDA_H

struct st_da {											/* struct D/A	*/
			 unsigned char		DR0;					/* DADR0		*/
			 unsigned char		DR1;					/* DADR1		*/
			 union {									/* DACR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char DAOE1:1;		/*	  DAOE1 	*/
						  unsigned char DAOE0:1;		/*	  DAOE0 	*/
						  unsigned char DAE  :1;		/*	  DAE		*/
						  } 	 BIT;					/*				*/
				   }			CR; 					/*				*/
};														/*				*/

#define DA		(*(volatile struct st_da	*)0xFFFFA4) /* D/A	 Address*/

#endif
