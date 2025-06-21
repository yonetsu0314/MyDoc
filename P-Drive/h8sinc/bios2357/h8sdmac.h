/*----------------------------------------------------------------------*
 * File name	: h8sdmac.h		/ H8S DMAC context header				*
 *----------------------------------------------------------------------*
 *$Header: p:/h8sinc/bios2357/RCS/h8sdmac.h 1.1 1970/01/01 00:00:00 yonetsu Exp $
 *$Log: h8sdmac.h $
 * リビジョン 1.1  1970/01/01  00:00:00  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------* 
 *				(c) Copyright 2000, ISHIDA  CO., LTD.					* 
 *				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					* 
 *				SHIGA JAPAN												* 
 *				(077) 553-4141											* 
 *----------------------------------------------------------------------*/

#if	!defined(H8SDMAC_H)
#define		H8SDMAC_H

struct st_sam0a {										/* struct DMAC0A*/
#if __CPU__==4											/* Normal Mode	*/
				char			wk1[2]; 				/*				*/
#endif													/*				*/
				void		   *MAR;					/* MAR			*/
				unsigned int	IOAR;					/* IOAR 		*/
				unsigned int	ETCR;					/* ETCR 		*/
				char			wk2[26];				/*				*/
				union { 								/* DMACR		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char DTSZ :1; 	/*	  DTSZ		*/
							 unsigned char DTID :1; 	/*	  DTID		*/
							 unsigned char RPE	:1; 	/*	  RPE		*/
							 unsigned char DTDIR:1; 	/*	  DTDIR 	*/
							 unsigned char DTF	:4; 	/*	  DTF		*/
							 }		BIT;				/*				*/
					  } 		DMACR;					/*				*/
				char			wk3[3]; 				/*				*/
				union { 								/* DMABCR		*/
					  unsigned int WORD;				/*	Word Access */
					  struct {							/*	Byte Access */
							 unsigned char H;			/*	  DMABCRH	*/
							 unsigned char L;			/*	  DMABCRL	*/
							 }	   BYTE;				/*				*/
					  struct {							/*	Bit  Access */
							 unsigned char wk  :1;		/*				*/
							 unsigned char FAE :1;		/*	  FAE		*/
							 unsigned char	   :1;		/*				*/
							 unsigned char SAE :1;		/*	  SAE		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTA :1;		/*	  DTA		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTE :1;		/*	  DTE		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTIE:1;		/*	  DTIE		*/
							 }	   BIT; 				/*				*/
					  } 		DMABCR; 				/*				*/
};														/*				*/
struct st_sam0b {										/* struct DMAC0B*/
#if __CPU__==4											/* Normal Mode	*/
				char			wk1[2]; 				/*				*/
#endif													/*				*/
				void		   *MAR;					/* MAR			*/
				unsigned int	IOAR;					/* IOAR 		*/
				unsigned int	ETCR;					/* ETCR 		*/
				char			wk2[19];				/*				*/
				union { 								/* DMACR		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char DTSZ :1; 	/*	  DTSZ		*/
							 unsigned char DTID :1; 	/*	  DTID		*/
							 unsigned char RPE	:1; 	/*	  RPE		*/
							 unsigned char DTDIR:1; 	/*	  DTDIR 	*/
							 unsigned char DTF	:4; 	/*	  DTF		*/
							 }		BIT;				/*				*/
					  } 		DMACR;					/*				*/
				char			wk3[2]; 				/*				*/
				union { 								/* DMABCR		*/
					  unsigned int WORD;				/*	Word Access */
					  struct {							/*	Byte Access */
							 unsigned char H;			/*	  DMABCRH	*/
							 unsigned char L;			/*	  DMABCRL	*/
							 }	   BYTE;				/*				*/
					  struct {							/*	Bit  Access */
							 unsigned char wk  :1;		/*				*/
							 unsigned char FAE :1;		/*	  FAE		*/
							 unsigned char	   :1;		/*				*/
							 unsigned char SAE :1;		/*	  SAE		*/
							 unsigned char	   :2;		/*				*/
							 unsigned char DTA :1;		/*	  DTA		*/
							 unsigned char	   :0;		/*				*/
							 unsigned char	   :2;		/*				*/
							 unsigned char DTE :1;		/*	  DTE		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTIE:1;		/*	  DTIE		*/
							 }	   BIT; 				/*				*/
					  } 		DMABCR; 				/*				*/
};														/*				*/
struct st_sam1a {										/* struct DMAC1A*/
#if __CPU__==4											/* Normal Mode	*/
				char			wk1[2]; 				/*				*/
#endif													/*				*/
				void		   *MAR;					/* MAR			*/
				unsigned int	IOAR;					/* IOAR 		*/
				unsigned int	ETCR;					/* ETCR 		*/
				char			wk2[12];				/*				*/
				union { 								/* DMACR		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char DTSZ :1; 	/*	  DTSZ		*/
							 unsigned char DTID :1; 	/*	  DTID		*/
							 unsigned char RPE	:1; 	/*	  RPE		*/
							 unsigned char DTDIR:1; 	/*	  DTDIR 	*/
							 unsigned char DTF	:4; 	/*	  DTF		*/
							 }		BIT;				/*				*/
					  } 		DMACR;					/*				*/
				char			wk3;					/*				*/
				union { 								/* DMABCR		*/
					  unsigned int WORD;				/*	Word Access */
					  struct {							/*	Byte Access */
							 unsigned char H;			/*	  DMABCRH	*/
							 unsigned char L;			/*	  DMABCRL	*/
							 }	   BYTE;				/*				*/
					  struct {							/*	Bit  Access */
							 unsigned char FAE :1;		/*	  FAE		*/
							 unsigned char	   :1;		/*				*/
							 unsigned char SAE :1;		/*	  SAE		*/
							 unsigned char	   :2;		/*				*/
							 unsigned char DTA :1;		/*	  DTA		*/
							 unsigned char	   :0;		/*				*/
							 unsigned char	   :1;		/*				*/
							 unsigned char DTE :1;		/*	  DTE		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTIE:1;		/*	  DTIE		*/
							 }	   BIT; 				/*				*/
					  } 		DMABCR; 				/*				*/
};														/*				*/
struct st_sam1b {										/* struct DMAC1B*/
#if __CPU__==4											/* Normal Mode	*/
				char			wk1[2]; 				/*				*/
#endif													/*				*/
				void		   *MAR;					/* MAR			*/
				unsigned int	IOAR;					/* IOAR 		*/
				unsigned int	ETCR;					/* ETCR 		*/
				char			wk2[5]; 				/*				*/
				union { 								/* DMACR		*/
					  unsigned char BYTE;				/*	Byte Access */
					  struct {							/*	Bit  Access */
							 unsigned char DTSZ :1; 	/*	  DTSZ		*/
							 unsigned char DTID :1; 	/*	  DTID		*/
							 unsigned char RPE	:1; 	/*	  RPE		*/
							 unsigned char DTDIR:1; 	/*	  DTDIR 	*/
							 unsigned char DTF	:4; 	/*	  DTF		*/
							 }		BIT;				/*				*/
					  } 		DMACR;					/*				*/
				union { 								/* DMABCR		*/
					  unsigned int WORD;				/*	Word Access */
					  struct {							/*	Byte Access */
							 unsigned char H;			/*	  DMABCRH	*/
							 unsigned char L;			/*	  DMABCRL	*/
							 }	   BYTE;				/*				*/
					  struct {							/*	Bit  Access */
							 unsigned char FAE :1;		/*	  FAE		*/
							 unsigned char	   :1;		/*				*/
							 unsigned char SAE :1;		/*	  SAE		*/
							 unsigned char	   :1;		/*				*/
							 unsigned char DTA :1;		/*	  DTA		*/
							 unsigned char	   :0;		/*				*/
							 unsigned char DTE :1;		/*	  DTE		*/
							 unsigned char	   :3;		/*				*/
							 unsigned char DTIE:1;		/*	  DTIE		*/
							 }	   BIT; 				/*				*/
					  } 		DMABCR; 				/*				*/
};														/*				*/
struct st_fam0 {										/* struct DMAC0 */
#if __CPU__==4											/* Normal Mode	*/
			   char 			wk1[2]; 				/*				*/
#endif													/*				*/
			   void 		   *MARA;					/* MARA 		*/
			   char 			wk2[2]; 				/*				*/
			   unsigned int 	ETCRA;					/* ETCRA		*/
#if __CPU__==4											/* Normal Mode	*/
			   char 			wk3[2]; 				/*				*/
#endif													/*				*/
			   void 		   *MARB;					/* MARB 		*/
			   char 			wk4[2]; 				/*				*/
			   unsigned int 	ETCRB;					/* ETCRB		*/
			   char 			wk5[18];				/*				*/
			   union {									/* DMACR		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char A;			/*	  DMACRA	*/
							unsigned char B;			/*	  DMACRB	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char DTSZ	:1; 	/*	  DTSZ		*/
							unsigned char SAID	:1; 	/*	  SAID		*/
							unsigned char SAIDE :1; 	/*	  SAIDE 	*/
							unsigned char BLKDIR:1; 	/*	  BLKDIR	*/
							unsigned char BLKE	:1; 	/*	  BLKE		*/
							unsigned char		:0; 	/*				*/
							unsigned char		:1; 	/*				*/
							unsigned char DAID	:1; 	/*	  DAID		*/
							unsigned char DAIDE :1; 	/*	  DAIDE 	*/
							unsigned char		:1; 	/*				*/
							unsigned char DTF	:4; 	/*	  DTF		*/
							}	  BIT;					/*				*/
					 }			DMACR;					/*				*/
			   char 			wk6[2]; 				/*				*/
			   union {									/* DMABCR		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char H;			/*	  DMABCRH	*/
							unsigned char L;			/*	  DMABCRL	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char wk   :1;		/*				*/
							unsigned char FAE  :1;		/*	  FAE		*/
							unsigned char	   :4;		/*				*/
							unsigned char DTA  :1;		/*	  DTA		*/
							unsigned char	   :0;		/*				*/
							unsigned char	   :2;		/*				*/
							unsigned char DTME :1;		/*	  DTME		*/
							unsigned char DTE  :1;		/*	  DTE		*/
							unsigned char	   :2;		/*				*/
							unsigned char DTIEB:1;		/*	  DTIEB 	*/
							unsigned char DTIEA:1;		/*	  DTIEA 	*/
							}	  BIT;					/*				*/
					 }			DMABCR; 				/*				*/
};														/*				*/
struct st_fam1 {										/* struct DMAC1 */
#if __CPU__==4											/* Normal Mode	*/
			   char 			wk1[2]; 				/*				*/
#endif													/*				*/
			   void 		   *MARA;					/* MARA 		*/
			   char 			wk2[2]; 				/*				*/
			   unsigned int 	ETCRA;					/* ETCRA		*/
#if __CPU__==4											/* Normal Mode	*/
			   char 			wk3[2]; 				/*				*/
#endif													/*				*/
			   void 		   *MARB;					/* MARB 		*/
			   char 			wk4[2]; 				/*				*/
			   unsigned int 	ETCRB;					/* ETCRB		*/
			   char 			wk5[4]; 				/*				*/
			   union {									/* DMACR		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char A;			/*	  DMACRA	*/
							unsigned char B;			/*	  DMACRB	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char DTSZ	:1; 	/*	  DTSZ		*/
							unsigned char SAID	:1; 	/*	  SAID		*/
							unsigned char SAIDE :1; 	/*	  SAIDE 	*/
							unsigned char BLKDIR:1; 	/*	  BLKDIR	*/
							unsigned char BLKE	:1; 	/*	  BLKE		*/
							unsigned char		:0; 	/*				*/
							unsigned char		:1; 	/*				*/
							unsigned char DAID	:1; 	/*	  DAID		*/
							unsigned char DAIDE :1; 	/*	  DAIDE 	*/
							unsigned char		:1; 	/*				*/
							unsigned char DTF	:4; 	/*	  DTF		*/
							}	  BIT;					/*				*/
					 }		   DMACR;					/*				*/
			   union {									/* DMABCR		*/
					 unsigned int WORD; 				/*	Word Access */
					 struct {							/*	Byte Access */
							unsigned char H;			/*	  DMABCRH	*/
							unsigned char L;			/*	  DMABCRL	*/
							}	  BYTE; 				/*				*/
					 struct {							/*	Bit  Access */
							unsigned char FAE  :1;		/*	  FAE		*/
							unsigned char	   :3;		/*				*/
							unsigned char DTA  :1;		/*	  DTA		*/
							unsigned char	   :0;		/*				*/
							unsigned char DTME :1;		/*	  DTME		*/
							unsigned char DTE  :1;		/*	  DTE		*/
							unsigned char	   :2;		/*				*/
							unsigned char DTIEB:1;		/*	  DTIEB 	*/
							unsigned char DTIEA:1;		/*	  DTIEA 	*/
							}	  BIT;					/*				*/
					 }			DMABCR; 				/*				*/
};														/*				*/
														/*				*/
struct st_dmac {										/* struct DMAC	*/
			   union {									/* DMAWER		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :4;		/*				*/
							unsigned char WE1B:1;		/*	  WE1B		*/
							unsigned char WE1A:1;		/*	  WE1A		*/
							unsigned char WE0B:1;		/*	  WE0B		*/
							unsigned char WE0A:1;		/*	  WE0A		*/
							}	   BIT; 				/*				*/
					 }			DMAWER; 				/*				*/
			   union {									/* DMATCR		*/
					 unsigned char BYTE;				/*	Byte Access */
					 struct {							/*	Bit  Access */
							unsigned char wk  :2;		/*				*/
							unsigned char TEE1:1;		/*	  TEE1		*/
							unsigned char TEE0:1;		/*	  TEE0		*/
							}	   BIT; 				/*				*/
					 }			DMATCR; 				/*				*/
};														/*				*/

#define DMAC0A	(*(volatile struct st_sam0a *)0xFFFEE0) /* DMAC 0A Addr */
#define DMAC0B	(*(volatile struct st_sam0b *)0xFFFEE8) /* DMAC 0B Addr */
#define DMAC1A	(*(volatile struct st_sam1a *)0xFFFEF0) /* DMAC 1A Addr */
#define DMAC1B	(*(volatile struct st_sam1b *)0xFFFEF8) /* DMAC 1B Addr */
#define DMAC0	(*(volatile struct st_fam0	*)0xFFFEE0) /* DMAC 0  Addr */
#define DMAC1	(*(volatile struct st_fam1	*)0xFFFEF0) /* DMAC 1  Addr */
#define DMAC	(*(volatile struct st_dmac	*)0xFFFF00) /* DMAC  Address*/

#endif
