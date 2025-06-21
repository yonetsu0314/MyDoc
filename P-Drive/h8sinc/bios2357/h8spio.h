/*----------------------------------------------------------------------*
 * File name	: h8spio.h		/ H8S PIO context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8spio.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8spio.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SPIO_H)
#define		H8SPIO_H

struct st_p1 {											/* struct P1	*/
			 unsigned char		DDR;					/* P1DDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* P1PORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* P1DR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
}; 														/*				*/
														/*				*/
struct st_p3 {											/* struct P3	*/
			 unsigned char		DDR;					/* P3DDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* P3PORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:2;			/*	  Bit 7,6	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* P3DR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:2;			/*	  Bit 7,6	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
			 char				wk3[19];				/*				*/
			 union {									/* P3ODR		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:2;			/*	  Bit 7,6	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			ODR;					/*				*/
};														/*				*/
 														/*				*/
struct st_p4 {											/* struct P4	*/
			 union {									/* P4PORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
};														/*				*/
														/*				*/
struct st_p5 {											/* struct P5	*/
			 unsigned char		DDR;					/* P5DDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* P5PORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:4;			/*	  Bit 7-4	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* P5DR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:4;			/*	  Bit 7-4	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
};														/*				*/
														/*				*/
struct st_pa {											/* struct PA	*/
			 unsigned char		DDR;					/* PADDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* PAPORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* PADR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
			 char				wk3[6]; 				/*				*/
			 union {									/* PAPCR		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PCR;					/*				*/
			 char				wk4[6]; 				/*				*/
			 union {									/* PAODR		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			ODR;					/*				*/
};														/*				*/
														/*				*/
struct st_pb {											/* struct PB	*/
			 unsigned char		DDR;					/* PBDDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* PBPORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* PBDR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
			 char				wk3[6]; 				/*				*/
			 union {									/* PBPCR		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char B7:1;			/*	  Bit 7 	*/
						  unsigned char B6:1;			/*	  Bit 6 	*/
						  unsigned char B5:1;			/*	  Bit 5 	*/
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PCR;					/*				*/
};														/*				*/
														/*				*/
struct st_pg {											/* struct PG	*/
			 unsigned char		DDR;					/* PGDDR		*/
			 char				wk1[159];				/*				*/
			 union {									/* PGPORT		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:3;			/*	  Bit 7,6,5 */
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			PORT;					/*				*/
			 char				wk2[15];				/*				*/
			 union {									/* PGDR 		*/
				   unsigned char BYTE;					/*	Byte Access */
				   struct { 							/*	Bit  Access */
						  unsigned char wk:3;			/*	  Bit 7,6,5 */
						  unsigned char B4:1;			/*	  Bit 4 	*/
						  unsigned char B3:1;			/*	  Bit 3 	*/
						  unsigned char B2:1;			/*	  Bit 2 	*/
						  unsigned char B1:1;			/*	  Bit 1 	*/
						  unsigned char B0:1;			/*	  Bit 0 	*/
						  } 	 BIT;					/*				*/
				   }			DR; 					/*				*/
};														/*				*/

#define P1		(*(volatile struct st_p1	*)0xFFFEB0) /* P1	 Address*/
#define P2		(*(volatile struct st_p1	*)0xFFFEB1) /* P2	 Address*/
#define P3		(*(volatile struct st_p3	*)0xFFFEB2) /* P3	 Address*/
#define P4		(*(volatile struct st_p4	*)0xFFFF53) /* P4	 Address*/
#define P5		(*(volatile struct st_p5	*)0xFFFEB4) /* P5	 Address*/
#define P6		(*(volatile struct st_p1	*)0xFFFEB5) /* P6	 Address*/
#define PA		(*(volatile struct st_pa	*)0xFFFEB9) /* PA	 Address*/
#define PB		(*(volatile struct st_pb	*)0xFFFEBA) /* PB	 Address*/
#define PC		(*(volatile struct st_pb	*)0xFFFEBB) /* PC	 Address*/
#define PD		(*(volatile struct st_pb	*)0xFFFEBC) /* PD	 Address*/
#define PE		(*(volatile struct st_pb	*)0xFFFEBD) /* PE	 Address*/
#define PF		(*(volatile struct st_p1	*)0xFFFEBE) /* PF	 Address*/
#define PG		(*(volatile struct st_pg	*)0xFFFEBF) /* PG	 Address*/

#define st_p2	st_p1							/* Change Struct P2->P1 */
#define st_p6	st_p1							/* Change Struct P6->P1 */
#define st_pf	st_p1							/* Change Struct PF->P1 */
#define st_pc	st_pb							/* Change Struct PC->PB */
#define st_pd	st_pb							/* Change Struct PD->PB */
#define st_pe	st_pb							/* Change Struct PE->PB */

#endif
