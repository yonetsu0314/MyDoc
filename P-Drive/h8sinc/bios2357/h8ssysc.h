/*----------------------------------------------------------------------*
 * File name	: h8ssysc.h		/ H8S SYSC context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8ssysc.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8ssysc.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SSYSC_H)
#define		H8SSYSC_H

union un_mdcr { 										/* union MDCR	*/
			  unsigned char BYTE;						/*	Byte Access */
			  struct {									/*	Bit  Access */
					 unsigned char wk :5;				/*				*/
					 unsigned char MDS:3;				/*	  MDS		*/
					 }		BIT;						/*				*/
};														/*				*/
														/*				*/
union un_syscr {										/* union SYSCR	*/
			   unsigned char BYTE;						/*	Byte Access */
			   struct { 								/*	Bit  Access */
					  unsigned char wk	 :2;			/*				*/
					  unsigned char INTM :2;			/*	  INTM		*/
					  unsigned char NMIEG:1;			/*	  NMIEG 	*/
					  unsigned char 	 :2;			/*				*/
					  unsigned char RAME :1;			/*	  RAME		*/
					  } 	 BIT;						/*				*/
};														/*				*/
														/*				*/
union un_syscr2 {										/* union SYSCR2 */
				unsigned char BYTE; 					/*	Byte Access */
				struct {								/*	Bit  Access */
					   unsigned char wk   :4;			/*				*/
					   unsigned char FLSHE:1;			/*	  FLSHE 	*/
					   }	  BIT;						/*				*/
};														/*				*/
														/*				*/
#define MDCR	(*(volatile union  un_mdcr	*)0xFFFF3B) /* MDCR  Address*/
#define SYSCR	(*(volatile union  un_syscr *)0xFFFF39) /* SYSCR Address*/
#define SYSCR2	(*(volatile union  un_syscr2*)0xFFFF42) /* SYSCR2Address*/

#endif
