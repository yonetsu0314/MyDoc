/*----------------------------------------------------------------------*
 * File name	: rx116.h	/ RX116 header file			*
 *----------------------------------------------------------------------*
 *$Header: p:/presto2/shinc/rxif/RCS/rx116.h 1.5 1970/01/01 00:00:00 chimura Exp $
 *$Log: rx116.h $
 * リビジョン 1.5  1970/01/01  00:00:00  chimura
 * 2003/07/25 10:15
 * WAI_FLGのイベント待ち条件の定義をsignedからunsignedに変更する。
 * 
 * リビジョン 1.4  1970/01/01  00:00:00  chimura
 * 2003/01/27 11:45
 * intrpt.hをインクルードする。
 * 
 * リビジョン 1.3  1970/01/01  00:00:00  m-yamana
 * 2002/12/16 18:20
 * ・SND_MSG のプロトタイプを修正した。
 * ・一部コメント(m-yamana)を削除した。
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  syuji
 * 2002/12/16 16:15
 * ベースソースのＲＣＳ用ヘッダコメントを削除。
 * ｆａｒ，ｎｅａｒの再定義防止。
 * アクセス･アドレス･ポインターの構造を変更。ＲＸ１１６では、上位１６ビット
 * がセグメントで下位１６ビットがオフセットと使用していたので、オフセットの
 * 位置を使用しないように、すべてのポインタを配列に上位のみ使用するように、
 * 変更する。
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  syuji
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN			*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(RX116_H)
#define	RX116_H

/*#include	<dos.h>*/				/* Show xlib		*/
#include	<intrpt.h>				/* Show xlib		*/

#ifndef	far
#define	far
#endif
#ifndef	near
#define near
#endif

/*--------------------------------------------------------------*/
/*	タスク生成時に使用するパケットの構造			*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	cre_tsk (arg1, arg2, struct pktsk(far *))			*/
/*--------------------------------------------------------------*/

typedef struct pktsk {
	void		(far *tsk_adr)(int);		/* タスク･スタート･ADDR	*/
	unsigned int	stk_size;			/* 使用するstack size	*/
	unsigned int	priority;			/* タスクの優先度	*/
	unsigned int	option;				/* NDPを使用するか否か	*/
	unsigned int	data_seg;			/* タスクで使用するDS	*/
} PKTSK;


/*--------------------------------------------------------------*/
/*	タスクステータスのＲＤに使用するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	tsk_sts (struct pksts(far *), arg2)			*/
/*--------------------------------------------------------------*/

typedef struct pksts {
	unsigned int	id;			/* タスクのＩＤ		*/
	unsigned int	priority;		/* タスクの優先度	*/
/*---------------------------------------------*/
	unsigned int	status;			/* タスクのステータス	*/
#define		READY		(0x1 << 0)	/* ready bit assign	*/
#define		SLEEP		(0x1 << 1)	/* sleep	"	*/
#define		WAIT_TIMEOUT	(0x1 << 2)	/* wait timeout		*/
#define		WAIT_FLG	(0x1 << 3)	/* wait event flag	*/
#define		WAIT_SEM	(0x1 << 4)	/* wait semaphore	*/
#define		WAIT_MBX	(0x1 << 5)	/* wait mailbox		*/
#define		WAIT_MEM	(0x1 << 6)	/* wait memory		*/
#define		SUSPEND		(0x1 << 7)	/* suspend		*/
#define		DORMANT		(0x1 << 8)	/* dormant		*/
#define		EXIT_TSK	(0x1 << 13)	/* 終了時処理ﾙｰﾁﾝ内	*/
#define		SYSEXC_HDR	(0x1 << 14)	/* ﾏｼﾝ例外処理ﾊﾝﾄﾞﾗ	*/
#define		MCHEXC_HDR	(0x1 << 15)	/* ｼｽﾃﾑｺｰﾙ例外処理ﾊﾝﾄﾞﾗ	*/
/*---------------------------------------------*/
} PKSTS;


/*--------------------------------------------------------------*/
/*	タスクの周期起床に使用するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	cyc_wup (arg1, srg2, struct pkcwup(far *))		*/
/*--------------------------------------------------------------*/

typedef struct pkcwup {
	long		ltime;			/* 周期起床時刻(下位)	*/
	unsigned int	utime;			/* 	"      (上位)	*/
	long		interval;		/* 周期起床時間間隔	*/
	unsigned int	count;			/* 起床回数		*/
} PKCWUP;


/*--------------------------------------------------------------*/
/*	割り込みハンドラ定義に使用するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	def_int (arg1, struct pkint(far *))			*/
/*--------------------------------------------------------------*/

typedef struct pkint {
	int		(far *hdr_adr)();	/* ハンドラ･アドレス	*/
	unsigned int	data_seg;		/* ハンドラで使用するDS	*/
} PKINT;


/*--------------------------------------------------------------*/
/*	メモリ･プール生成時に使用するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	cre_mpl (arg1 , arg2 , arg3, struct pkmpl(far *))	*/
/*--------------------------------------------------------------*/

typedef struct pkmpl {
	long		mpl_size;		/* メモリ･プールのサイズ*/
	long		blk_size;		/* 切り出すブロックsize	*/
} PKMPL;


/*--------------------------------------------------------------*/
/*	システム･クロック設定時に使用するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	set_tim (struct pktim(far *))				*/
/*--------------------------------------------------------------*/

typedef struct pktim {
	long		ltime;			/* 指定時刻(下位)	*/
	unsigned int	utime;			/* 	"  (上位)	*/
} PKTIM;


/*--------------------------------------------------------------*/
/*	拡張ＳＶＣハンドラの定義に使用するパケットの構造	*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	def_svc (arg1, struct pkesvc(far *))			*/
/*--------------------------------------------------------------*/

typedef struct pkesvc {
	int		(far *hdr_adr)();	/* ハンドラ･アドレス	*/
	unsigned int	data_seg;		/* ハンドラで使用するDS	*/
} PKESVC;


/*--------------------------------------------------------------*/
/*	ＯＳのバージョン情報を格納するパケットの構造		*/
/*--------------------------------------------------------------*/
/* 書式:							*/
/*	get_ver (struct pkver(far *))				*/
/*--------------------------------------------------------------*/

typedef struct pkver {
	unsigned char	maker;			/* メーカー：NEC	*/
	unsigned char	reserve;		/* 予約			*/
	unsigned char	tron_ver;		/* ITRONのバージョン	*/
	unsigned char	maker_ver;		/* RX-116のバージョン	*/
	unsigned char	serial_no[4];		/* シリアル･ナンバー	*/
	unsigned char	cpu;			/* 実行するCPU		*/
	unsigned char	variation;		/* バリエーション記述子	*/
} PKVER;


/*------------------------------------------------------*/
/*	アクセス･アドレス･ポインターの構造		*/
/*							*/
/*------------------------------------------------------*/

#if	!defined(acadr)
typedef union aap {
	void far		*vfpp[2];	/* void型farポインタ	*/
	char far		*cfpp[2];	/* char		"	*/
	int far			*ifpp[2];	/* int型	"	*/
	long far		*lfpp[2];	/* long型	"	*/
	unsigned int		ary[2];		/* 32bitアドレスの配列	*/
} AAP;
#define	acadr	ary[1]				/* [1]ｱｸｾｽ･ｱﾄﾞﾚｽ(para)	*/
#define	vfp	vfpp[1]				/* [1]void型farポインタ	*/
#define	cfp	cfpp[1]				/* [1]char	"	*/
#define	ifp	ifpp[1]				/* [1]int型	"	*/
#define	lfp	lfpp[1]				/* [1]long型	"	*/
#endif


/*------------------------------------------------------*/
/*	RX116で使用する割り込み処理制御情報		*/
/*							*/
/*------------------------------------------------------*/

#define	MASTER		0x0008
#define	SLAVE		0x0000
#define	INT_DVN1	((unsigned)((1 <<4)|MASTER))
#define	INT_DVN2	((unsigned)((2 <<4)|MASTER))
#define	INT_DVN3	((unsigned)((3 <<4)|MASTER))
#define	INT_DVN4	((unsigned)((4 <<4)|MASTER))
#define	INT_DVN5	((unsigned)((5 <<4)|MASTER))
#define	INT_DVN6	((unsigned)((6 <<4)|MASTER))
#define	INT_DVN7	((unsigned)((7 <<4)|MASTER))

#define	VECT_MOD	0x8000			/* bit 15		*/


/*------------------------------------------------------*/
/*	オプション指定のシンボル定義			*/
/*							*/
/*------------------------------------------------------*/

/*#define	PRI		0*/		/* prioryty base	*/
#define		FIFO		(unsigned int)1	/* FIFO base		*/
#define		ABS_TIME	0		/* 絶対時間		*/
#define		REL_TIME	1		/* 相対時間		*/
#define		TIME_OUT	(unsigned int)1	/* ﾀｲﾑ･ｱｳﾄ･ｵﾌﾟｼｮﾝ指定	*/
/*---------------------------------------------*/
#define		ENDLESS		(unsigned int)0	/* ﾀｲﾑ･ｱｳﾄ無しｵﾌﾟｼｮﾝ指定*/
#define		VOID_TIME	((long far *)0)	/* 無効時間定数:long変数
						   へのfarﾎﾟｲﾝﾀの代わり	*/
/*---------------------------------------------*/


/*------------------------------------------------------*/
/*	タスク関係のＳＶＣプロトタイプ			*/
/*							*/
/*------------------------------------------------------*/

int far	CRE_TSK(unsigned int (far *), unsigned int, struct pktsk (far *));
int far	STA_TSK(unsigned int, unsigned int);
int far	DEL_TSK(unsigned int);
int far	DEF_EXT(int (far *)(), unsigned int);
int far	EXT_TSK(void);
int far	EXD_TSK(void);
int far	ABO_TSK(unsigned int);
int far	TER_TSK(unsigned int, unsigned int);
int far	CHG_PRI(unsigned int, unsigned int);
int far	ROT_RDQ(unsigned int);
int far	TCB_ADR(unsigned int (far *), unsigned int);
int far	TSK_STS(struct pksts (far *), unsigned int);
int far	SUS_TSK(unsigned int);
int far	RSM_TSK(unsigned int, unsigned int);
int far	SLP_TSK(void);
int far	WAI_TSK(long (far *));
int far	WUP_TSK(unsigned int);
int far	CAN_WUP(unsigned int (far *), unsigned int);
int far	CYC_WUP(unsigned int, unsigned int, struct pkcwup (far *));
int far	CAN_CYC(unsigned int);


/*------------------------------------------------------*/
/*	イベント･フラグ関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	CRE_FLG(unsigned int (far *), unsigned int);
int far	DEL_FLG(unsigned int);
/*---------------------------------------------*/
int far	SET_FLG(unsigned int, unsigned int (far *), unsigned int,
		unsigned int);
#define	EVF_SET_SET	(unsigned)0x0		/* ORする		*/
#define	EVF_SET_RES	(unsigned)0x1		/* ANDする		*/
#define	EVF_SET_REPL	(unsigned)0x2		/* 置き換える		*/
#define	EVF_SET_EXOR	(unsigned)0x3		/* EXORする		*/
/*---------------------------------------------*/
int far	WAI_FLG(unsigned int, unsigned int (far *), unsigned int,
		unsigned int, long (far *));
#define	EVF_AND		(unsigned)0		/* AND条件で待つ	*/
#define	EVF_OR		(unsigned)(0x1 << 1)	/* OR条件で待つ		*/
#define	EVF_RES		(unsigned)(0x1 << 2)	/* 待ち条件成立後ﾘｾｯﾄ	*/
/*---------------------------------------------*/
int far	FLG_ADR(unsigned int (far *), unsigned int);


/*------------------------------------------------------*/
/*	セマフォ関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	CRE_SEM(unsigned int, unsigned int (far *), unsigned int, unsigned int);
int far	DEL_SEM(unsigned int);
int far	SIG_SEM(unsigned int, unsigned int);
int far	WAI_SEM(unsigned int, unsigned int, unsigned int, long (far *));
int far	SEM_ADR(unsigned int (far *), unsigned int);


/*------------------------------------------------------*/
/*	メール･ボックス関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	CRE_MBX(unsigned int, unsigned int (far *), unsigned int);
int far	DEL_MBX(unsigned int);
int far	SND_MSG(unsigned int, unsigned int);
int far	RCV_MSG(unsigned int, unsigned int (far *), unsigned int, long (far *));
int far	MBX_ADR(unsigned int (far *), unsigned int);


/*------------------------------------------------------*/
/*	割り込み処理関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	DEF_INT(unsigned int, struct pkint (far *));
int far	RET_INT(void);
int far	DIS_INT(unsigned int);
int far	ENA_INT(unsigned int);
int far	FET_DAT(unsigned int);
int far	GET_DVN(unsigned int (far *));
void far IRET_WUP(unsigned int);


/*------------------------------------------------------*/
/*	例外処理関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	DEF_EXC(unsigned int, int (far *)(), unsigned int);
int far	RET_EXC(void);


/*------------------------------------------------------*/
/*	メモリー･プール関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	CRE_MPL(unsigned int, unsigned int (far *), unsigned int,
		struct pkmpl (far *));
int far	DEL_MPL(unsigned int);
int far	GET_BLK(unsigned int, unsigned int (far *), unsigned int,
		unsigned int, long (far *));
int far	REL_BLK(unsigned int);
int far	MPL_ADR(unsigned int (far *), unsigned int);


/*------------------------------------------------------*/
/*	時間管理関係のＳＶＣプロトタイプ		*/
/*							*/
/*------------------------------------------------------*/

int far	SET_TIM(struct pktim (far *));
int far	GET_TIM(struct pktim (far *));


/*------------------------------------------------------*/
/*	その他のＳＶＣプロトタイプ			*/
/*							*/
/*------------------------------------------------------*/

int far	DEF_SVC(unsigned int, struct esvc (far *));
int far	GET_VER(struct pkver (far *));

/*------------------------------------------------------*/
/*	カスタムのＳＶＣプロトタイプ			*/
/*							*/
/*------------------------------------------------------*/

#define	HLT_TSK_SEMID	((unsigned int)60000)	/* システム予約		*/

int far	HLT_TSK(long far *);

/*------------------------------------------------------*/
/*	システムコール戻り値のシンボル定義		*/
/*							*/
/*------------------------------------------------------*/

#define SVCE_OK          	(0x00)	/* 00h  normal exit			*/
#define	SVCE_MEM                (0x01)  /* 01h  can't obtain memory area	*/
#define	SVCE_UNDEF_SVC          (0x02)  /* 02h  illegal system call		*/
#define	SVCE_CONTEXT            (0x03)  /* 03h  context error			*/
#define	SVCE_ACCESS_ADR         (0x04)  /* 04h  access address error		*/
#define	SVCE_ILLEGAL_ADR        (0x05)  /* 05h  illegal address ( aa )		*/
#define	SVCE_PACKET_ADR         (0x06)  /* 06h  illegal address ( pa )		*/
#define	SVCE_BLOCK_ADR          (0x07)  /* 07h  illegal address ( ma, mba )	*/
#define	SVCE_START_ADR          (0x08)  /* 08h  illegal address ( psa )         */
#define	SVCE_ALREADY_EXIST      (0x09)  /* 09h  existent object                 */
#define	SVCE_NOT_EXIST          (0x0A)  /* 0ah  not exist object                */
#define	SVCE_NOT_IDLE           (0x0B)  /* 0bh  task status isn't idle          */
#define	SVCE_NOT_SUSPEND        (0x0C)  /* 0ch  task status isn't suspend	*/
#define	SVCE_IDLE               (0x0D)  /* 0dh  task status is idle             */
#define	SVCE_ID0                (0x0E)  /* 0eh  task id equal '0'               */
#define	SVCE_ID_BOUND           (0x0F)  /* 0fh  task id is greater than max_id  */
#define	SVCE_TIMEOUT            (0x10)  /* 10h  timeout error                   */
#define	SVCE_COUNT_OVER         (0x11)  /* 11h  queue count overflow            */
#define	SVCE_SELF_TASK          (0x12)  /* 12h  self task no-good		*/
#define	SVCE_DELETE_OBJ         (0x13)  /* 13h  delete obj on something_wait    */
#define	SVCE_OPTION             (0x14)  /* 14h  can't use this option           */
#define	SVCE_FLG_WAIT           (0x15)  /* 15h  task already wait on this evf   */
#define	SVCE_TIMER              (0x16)  /* 16h  can't use timer                 */
#define	SVCE_PRIORITY           (0x17)  /* 17h  priority error                  */
#define	SVCE_INTERRUPT_PRIORITY	(0x18)  /* 18h  interrut priority error		*/
#define	SVCE_NON_CYCLIC         (0x19)  /* 19h  can_cwak to non-cyclic task     */
#define	SVCE_POOL_SIZE          (0x1A)  /* 1ah  memory pool size error          */
#define	SVCE_MEMORY_BLOCK       (0x1B)  /* 1bh  rel_blk to not system memory    */
#define	SVCE_DEVICE_NO          (0x1C)  /* 1ch  illegal device number           */
#define	SVCE_MES_OVER           (0x1D)  /* 1dh  message buffer overflow         */
#define	SVCE_ERROR              (0x1E)  /* 1eh  others system error		*/
#define	SVCE_PARAM              (0x1F)  /* 1fh  illegal parameter               */
#define	SVCE_SVC	        (0x40) 	/* 40h  illegal system cal number	*/
#define	SVCE_EXCEPTION          (0x80)  /* 80h  error in exception proc.        */
#define	SVCE_SYS_OBJECT         (0x81)  /* 81h  this object is system task	*/


/*--------------------------------------------------------------*/
/*	ADV_WAI_SEM						*/
/*--------------------------------------------------------------*/
/* 説明								*/
/*	ネスティングをカウンターよりチェックし、競合が発生	*/
/*	した場合のみWAI_SEM()を発行し排他制御を行う。		*/
/*--------------------------------------------------------------*/

void	ADV_WAI_SEM( int err, unsigned char cnt, int option,
		     unsigned int sem_aa, long far *tmout);

#define	ADV_WAI_SEM(							\
		err,		/* WAI_SEMの戻り値			*/\
		cnt,		/* ネスティング･チェック･カウンタ	*/\
		option,		/* WAI_SEMのタイムアウト指定オプション	*/\
		sem_aa,		/* WAI_SEMに使用するアクセス･アドレス	*/\
		tmout   	/* WAI_SEMに使用するタイマーへのポインタ*/\
		)							\
	_disable();							\
	if(++cnt > 1)	err = WAI_SEM( (unsigned int)option, (unsigned int)sem_aa, (unsigned int)1, (long *)tmout);\
	_enable();


/*--------------------------------------------------------------*/
/*	ADV_SIG_SEM						*/
/*--------------------------------------------------------------*/
/* 説明								*/
/*	ネスティングをカウンターよりチェックし、競合が発生	*/
/*	している場合SIG_SEM()を発行しセマフォ待ちのタスクへ	*/
/*	制御を移す。						*/
/*--------------------------------------------------------------*/

void	ADV_SIG_SEM( int err, unsigned char cnt, unsigned int sem_aa);

#define	ADV_SIG_SEM(							\
		err,		/* SIG_SEMの戻り値			*/\
		cnt,		/* ネスティング･チェック･カウンタ	*/\
		sem_aa		/* SIG_SEMに使用するアクセス･アドレス	*/\
		)							\
	_disable();							\
	if(--cnt >= 1)	err = SIG_SEM( (unsigned int)sem_aa, (unsigned int)1);\
	_enable();

#endif	/* RX116_H */

