/*======================================================================
 * File name    : errcode.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	:
 *	エラーコードコンバート（ＩＴＲＯＮ１→μＩＴＲＯＮ４）
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/cnv/src/RCS/errcode.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: errcode.c $
 * リビジョン 1.1  1970/01/01  00:00:00  chimura
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 2001, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CITY
 *		SHIGA JAPAN
 *		(077) 553-4141
 *======================================================================
*/
#include	<rxif\rx116.h>			/* Show include		*/
#include	<kernel.h>

#define		STORAGE
#include	"errcode.h"			/* Show Current		*/
#undef		STORAGE

/*----------------------------------------------------------------------*/
/*	err_code							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	err	: μＩＴＲＯＮ４のエラーコード				*/
/*									*/
/* 戻り値								*/
/*	int	: ＩＴＲＯＮ１のエラーコード				*/
/*----------------------------------------------------------------------*/
int
err_code(int err) {
	int	iret = 0;

	switch (err) {
	case E_OK:				/*    0   normal completion				*/
		iret = SVCE_OK;			/*  00h   normal exit					*/
		break;
	case E_SYS:				/*  (-5)  system error					*/
		iret = SVCE_ERROR;		/*  1eh   others system error				*/
		break;
	case E_NOSPT:				/*  (-9)  feature not supported				*/
		iret = SVCE_OPTION;		/*  14h   can't use this option           		*/
		break;
	case E_RSFN:				/* (-10)  reserved function code number			*/
		iret = SVCE_SVC;		/*  40h   illegal system cal number			*/
		break;
	case E_RSATR:				/* (-11)  reserved attribute				*/
		iret = SVCE_PARAM;		/*  1fh   illegal parameter               		*/
		break;
	case E_PAR:				/* (-17)  parameter error				*/
		iret = SVCE_PARAM;		/*  1fh   illegal parameter               		*/
		break;
	case E_ID:				/* (-18)  invalid ID number				*/
		iret = SVCE_ID_BOUND;		/*  0fh   task id is greater than max_id  		*/
		break;
	case E_CTX:				/* (-25)  context error					*/
		iret = SVCE_CONTEXT;		/*  03h   context error					*/
		break;
	case E_MACV:				/* (-26)  memory access disabled or violation		*/
		iret = SVCE_ACCESS_ADR;		/*  04h   access address error				*/
		break;
	case E_OACV:				/* (-27)  object access violation			*/
		iret = SVCE_ACCESS_ADR;		/*  04h   access address error				*/
		break;
	case E_ILUSE:				/* (-28)  illegal use					*/
		iret = SVCE_UNDEF_SVC;		/*  02h   illegal system call				*/
		break;
	case E_NOMEM:				/* (-33)  insufficient memory				*/
		iret = SVCE_MEM;		/*  01h   can't obtain memory area			*/
		break;
	case E_NOID:				/* (-34)  no free ID number				*/
		iret = SVCE_ID0;		/*  0eh   task id equal '0'               		*/
		break;
	case E_OBJ:				/* (-41)  invalid object state				*/
		iret = SVCE_ALREADY_EXIST;	/*  09h   existent object                 		*/
		break;
	case E_NOEXS:				/* (-42)  object does not exist				*/
		iret = SVCE_NOT_EXIST;		/*  0ah   not exist object                		*/
		break;
	case E_QOVR:				/* (-43)  queuing or nesting overflow			*/
		iret = SVCE_COUNT_OVER;		/*  11h   queue count overflow            		*/
		break;
	case E_RLWAI:				/* (-49)  WAIT state was forcibly released		*/
		iret = SVCE_DELETE_OBJ;		/*  13h   delete obj on something_wait    		*/
		break;
	case E_TMOUT:				/* (-50)  polling failure or timeout exceeded		*/
		iret = SVCE_TIMEOUT;		/*  10h   timeout error                   		*/
		break;
	case E_DLT:				/* (-51)  object being waited for was deleted		*/
		iret = SVCE_DELETE_OBJ;		/*  13h   delete obj on something_wait    		*/
		break;
	/* case E_BOVR: */			/* (-58)  buffer overflow				*/
	/*	break; */
	case E_INOSPT:				/* (-97)  feature not supported by ITRON/FILE		*/
		iret = SVCE_EXCEPTION;		/*  80h   error in exception proc.        		*/
		break;
	default:
		iret = 0;
		break;
	}	/* End of switch */
	return (iret);
}




#if 0	/* MEMO メモ書き */
		iret = SVCE_OK;			/*  00h   normal exit					*/
		iret = SVCE_MEM;		/*  01h   can't obtain memory area			*/
						/*
							CRE_TSK:TCB領域がとれない。
							CRE_FLG:イベント・フラグの領域が確保できない。
						*/
		iret = SVCE_UNDEF_SVC;		/*  02h   illegal system call				*/
		iret = SVCE_CONTEXT;		/*  03h   context error					*/
						/*
							ABO_TSK:終了時処理中に発行した。
							TER_TSK:終了時処理中に発行した。
							CHG_PRI:終了時処理中に発行した。
							SUS_TSK:終了時処理中に発行した。
						 	GET_DVN:環境エラー
						*/
		iret = SVCE_ACCESS_ADR;		/*  04h   access address error				*/
						/*
							STA_TSK:タスク・アクセス・アドレス・エラー（対象タスクが存在しない。）
							DEL_TSK:タスク・アクセス・エラー（対象タスクが存在しない。）
							TER_TSK:タスク・アクセス・アドレスのエラー
							CHG_PRI:タスク・アクセス・アドレスのエラー
							TSK_STS:存在しないタスクを指定した。
							SUS_TSK:タスク・アクセス・アドレス・エラー
							RSM_TSK:タスク・アクセス・アドレス・エラー
							WUP_TSK:タスク・アクセス・アドレス・エラー
							CAN_WUP:タスク・アクセス・アドレス・エラー
							CYC_WUP:タスク・アクセス・アドレス・エラー
							CAN_CYC:タスク・アクセス・アドレス・エラー
							DEL_FLG:アクセス・アドレス・エラー
							SET_FLG:アクセス・アドレス・エラー
							WAI_FLG:アクセス・アドレス・エラー
						*/
		iret = SVCE_ILLEGAL_ADR;	/*  05h   illegal address ( aa )			*/
						/*
							STA_TSK:タスク・アクセス・アドレスが「０」である。
							DEL_TSK:タスク・アクセス・アドレスが「０」である。
							TER_TSK:タスク・アクセス・アドレスが「０」である。
							TSK_STS:タスク・アクセス・アドレスが「０」である。
							SUS_TSK:タスク・アクセス・アドレスが「０」である。
							RSM_TSK:タスク・アクセス・アドレスが「０」である。
							WUP_TSK:タスク・アクセス・アドレスが「０」である。
							DEL_FLG:イベント・フラグ・アクセス・アドレスが「０」である。
							SET_FLG:フラグ・アクセス・アドレスが「０」である。
							WAI_FLG:フラグ・アクセス・アドレスが「０」である。
							SND_MSG:メールボックス・アクセス・アドレスが「０」である。
						*/
		iret = SVCE_PACKET_ADR;		/*  06h   illegal address ( pa )			*/
						/*
							CRE_TSK:タスク・アクセス・アドレス・エリア・アドレスまたはパラメータ・パケット・アドレスが「０：０」である。
							TCB_ADR:タスク・アクセス・アドレス・エリア・ポインタが「０：０」である。
							TSK_STS:ステータス・エリア・アドレスが「０：０」である。
							WAI_TSK:タイム・アウト・エリア・アドレスが「０：０」である。
							CAN_WUP:カウント・エリア・アドレスが「０」である。
							CYC_WUP:カウント・エリア・アドレスが「０：０」である。
							CRE_FLG:フラグ・アクセス・アドレス・エリア・アドレスが「０：０」である。
							SET_FLG:ビット・パターン・エリア・アドレスが「０：０」である。
						*/
		iret = SVCE_BLOCK_ADR;		/*  07h   illegal address ( ma, mba )			*/
						/*
							SND_MSG:メッセージ・アクセス・アドレスが「０」である。
						*/
		iret = SVCE_START_ADR;		/*  08h   illegal address ( psa )       		*/
						/*
							CRE_TSK:タスク・スタート・アドレスが「０：０」である。
						*/
		iret = SVCE_ALREADY_EXIST;	/*  09h   existent object                 		*/
						/*
							CRE_TSK:タスクＩＤエラー（対象タスクが存在する。）
							CRE_FLG:既に存在するＩＤを指定した。
							CRE_SEM:指定したＩＤ番号のセマフォが既に存在する。
						*/
		iret = SVCE_NOT_EXIST;		/*  0ah   not exist object                		*/
						/*
							TCB_ADR:対象タスクが存在しない。
							FLG_ADR:指定したＩＤ番号のイベント・フラグは存在しない。
							DEL_MPL:指定されたメモリ・プールは存在しない。
						*/
		iret = SVCE_NOT_IDLE;		/*  0bh   task status isn't idle          		*/
						/*
							STA_TSK:対象タスクが休止状態にない。
							DEL_TSK:対象タスクが休止状態にない。
						*/
		iret = SVCE_NOT_SUSPEND;	/*  0ch   task status isn't suspend			*/
						/*
							RSM_TSK:タスクがサスペンド状態でない。
						*/
		iret = SVCE_IDLE;		/*  0dh   task status is idle             		*/
						/*
							TER_TSK:対象タスクが休止状態(DORMANT)である。
							CHG_PRI:対象タスクが休止状態(DORMANT)である。
							SUS_TSK:対象タスクが休止状態(DORMANT)である。
							WUP_TSK:対象タスクが休止状態(DORMANT)である。
							CAN_WUP:対象タスクが休止状態(DORMANT)である。
						*/
		iret = SVCE_ID0;		/*  0eh   task id equal '0'               		*/
						/*
							CRE_TSK:ＩＤに０を指定した。
							CRE_FLG:ＩＤに０を指定した。
						*/
		iret = SVCE_ID_BOUND;		/*  0fh   task id is greater than max_id  		*/
						/*
							CRE_MPL:「ＩＤ」が「Ｆ」より大きい。
						*/
		iret = SVCE_TIMEOUT;		/*  10h   timeout error                   		*/
						/*
							WAI_TSK:タイム・アウト・エラー
						*/
		iret = SVCE_COUNT_OVER;		/*  11h   queue count overflow            		*/
						/*
							SUS_TSK:サスペンド要求回数オーバ
							WUP_TSK:起床要求の回数オーバ
							SIG_SEM:カウント値が返却により最大値を超えた。
						*/
		iret = SVCE_SELF_TASK;		/*  12h   self task no-good				*/
						/*
							STA_TSK:自タスクを指定した。
							DEL_TSK:自タスクを指定した。
							TER_TSK:自タスクを指定した。
							SUS_TSK:自タスクを指定した。
							RSM_TSK:自タスクを指定した。
							WUP_TSK:自タスクを指定した。
						*/
		iret = SVCE_DELETE_OBJ;		/*  13h   delete obj on something_wait    		*/
						/*
							WAI_FLG:待ち状態のときにイベント・フラグが削除された。
							WAI_SEM:待ち状態のときにセマフォが削除された。
						*/
		iret = SVCE_OPTION;		/*  14h   can't use this option           		*/
						/*
							CRE_TSK:数値演算プロセッサ・オプションが「１」より大きい。
							RSM_TSK:ＯＰＴＩＯＮ指定が１より大きい。
							CYC_WUP:ＯＰＴＩＯＮ指定が１より大きい。
							SET_FLG:ＯＰＴＩＯＮ指定が３より大きい。
							CRE_MPL:サポートされていないオプションを指定した。
						*/
		iret = SVCE_FLG_WAIT;		/*  15h   task already wait on this evf   		*/
						/*
							WAI_FLG:指定したイベント・フラグで待つことはできない。
						*/
		iret = SVCE_TIMER;		/*  16h   can't use timer                 		*/
						/*
							WAI_TSK:タイム・コントロール・テーブルを作成する分だけのメモリ・エリアがとれない。
							CYC_WUP:タイム・コントロール・テーブルを作成する分だけのメモリ・エリアがとれない。
						*/
		iret = SVCE_PRIORITY;		/*  17h   priority error                  		*/
						/*
							CRE_TSK:タスク優先度が「ＦＦ」より大きい。
						*/
		iret = SVCE_INTERRUPT_PRIORITY;	/*  18h   interrut priority error			*/
		iret = SVCE_NON_CYCLIC;		/*  19h   can_cwak to non-cyclic task     		*/
						/*
							CAN_CYC:周期起床要求を受けていないタスクを指定した。
						*/
		iret = SVCE_POOL_SIZE;		/*  1ah   memory pool size error          		*/
		iret = SVCE_MEMORY_BLOCK;	/*  1bh   rel_blk to not system memory    		*/
		iret = SVCE_DEVICE_NO;		/*  1ch   illegal device number           		*/
						/*
							DEF_INT:割り込みレベル番号エラー
						*/
		iret = SVCE_MES_OVER;		/*  1dh   message buffer overflow         		*/
		iret = SVCE_ERROR;		/*  1eh   others system error				*/
						/*
							WAI_SEM:要求カウント値が初期値より大きい。
						*/
		iret = SVCE_PARAM;		/*  1fh   illegal parameter               		*/
						/*
							CYC_WUP:インターバル・タイムが「０」である。
							WAI_FLG:マスク・ビット・パターンが「０」である。
							CRE_SEM:イニシャル・カウントが「０」である。
							WAI_SEM:要求カウントが「０」である。
							CRE_MPL:メモリ・プール・サイズまたはブロック・サイズが「０」である。
							CRE_MPL:メモリ・プール・サイズが「０ＦＦＦＦＨ」より大きい。
						*/
		iret = SVCE_SVC;		/*  40h   illegal system cal number			*/
						/*
							DEL_SVC:ファンクションNO.が不正である。
						*/
		iret = SVCE_EXCEPTION;		/*  80h   error in exception proc.        		*/
		iret = SVCE_SYS_OBJECT;		/*  81h   this object is system task			*/
						/*
							TER_TSK:システムのアイドル・タスクに対して操作を行おうとした。
							CHG_PRI:システムのアイドル・タスクに対して操作を行おうとした。
							SUS_TSK:システムのアイドル・タスクに対して操作を行おうとした。
							DEL_MPL:メモリ・プール＃０を指定した。
						*/
#endif	/* MEMO メモ書き */


#if 0	/* MEMO メモ書き */
/*----------------------------------------------------------------------*/
/*	err_code_2							*/
/*----------------------------------------------------------------------*/
/* 説明                                                  		*/
/*----------------------------------------------------------------------*/
/* 引き数								*/
/*	err	: ＩＴＲＯＮ１のエラーコード				*/
/*									*/
/* 戻り値								*/
/*	int	: μＩＴＲＯＮ４のエラーコード				*/
/*----------------------------------------------------------------------*/
int
err_code_2(int err) {
	int	iret = 0;

	switch (err) {
	case	SVCE_OK:			/*  00h   normal exit					*/
		iret = E_OK;			/*    0   normal completion				*/
		break;
	case	SVCE_MEM:			/*  01h   can't obtain memory area			*/
		iret = E_NOMEM;			/* (-33)  insufficient memory				*/
		break;
	case	SVCE_UNDEF_SVC:			/*  02h   illegal system call				*/
		iret = E_ILUSE;			/* (-28)  illegal use					*/
		break;
	case	SVCE_CONTEXT:			/*  03h   context error					*/
		iret = E_CTX;			/* (-25)  context error					*/
		break;
	case	SVCE_ACCESS_ADR:		/*  04h   access address error				*/
		iret = E_OACV;			/* (-27)  object access violation			*/
		break;
	case	SVCE_ILLEGAL_ADR:		/*  05h   illegal address ( aa )			*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_PACKET_ADR:		/*  06h   illegal address ( pa )			*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_BLOCK_ADR:			/*  07h   illegal address ( ma, mba )			*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_START_ADR:			/*  08h   illegal address ( psa )       		*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_ALREADY_EXIST:		/*  09h   existent object                 		*/
		iret = E_OBJ;			/* (-41)  invalid object state				*/
		break;
	case	SVCE_NOT_EXIST:			/*  0ah   not exist object                		*/
		iret = E_NOEXS;			/* (-42)  object does not exist				*/
		break;
	case	SVCE_NOT_IDLE:			/*  0bh   task status isn't idle          		*/
		iret = E_OBJ;			/* (-41)  invalid object state				*/
		break;
	case	SVCE_NOT_SUSPEND:		/*  0ch   task status isn't suspend			*/
		iret = E_OBJ;			/* (-41)  invalid object state				*/
		break;
	case	SVCE_IDLE:			/*  0dh   task status is idle             		*/
		iret = E_OBJ;			/* (-41)  invalid object state				*/
		break;
	case	SVCE_ID0:			/*  0eh   task id equal '0'               		*/
		iret = E_NOID;			/* (-34)  no free ID number				*/
		break;
	case	SVCE_ID_BOUND:			/*  0fh   task id is greater than max_id  		*/
		iret = E_ID;			/* (-18)  invalid ID number				*/
		break;
	case	SVCE_TIMEOUT:			/*  10h   timeout error                   		*/
		iret = E_TMOUT;			/* (-50)  polling failure or timeout exceeded		*/
		break;
	case	SVCE_COUNT_OVER:		/*  11h   queue count overflow            		*/
		iret = E_QOVR;			/* (-43)  queuing or nesting overflow			*/
		break;
	case	SVCE_SELF_TASK:			/*  12h   self task no-good				*/
		iret = E_ID;			/* (-18)  invalid ID number				*/
		break;
	case	SVCE_DELETE_OBJ:		/*  13h   delete obj on something_wait    		*/
		iret = E_DLT;			/* (-51)  object being waited for was deleted		*/
		break;
	case	SVCE_OPTION:			/*  14h   can't use this option           		*/
		iret = E_NOSPT;			/*  (-9)  feature not supported				*/
		break;
	case	SVCE_FLG_WAIT:			/*  15h   task already wait on this evf   		*/
		iret = E_PAR;			/* (-17)  parameter error				*/
		break;
	case	SVCE_TIMER:			/*  16h   can't use timer                 		*/
		iret = E_NOMEM;			/* (-33)  insufficient memory				*/
		break;
	case	SVCE_PRIORITY:			/*  17h   priority error                  		*/
		iret = E_ILUSE;			/* (-28)  illegal use					*/
		break;
	case	SVCE_INTERRUPT_PRIORITY:	/*  18h   interrut priority error			*/
		iret = E_PAR;			/* (-17)  parameter error				*/
		break;
	case	SVCE_NON_CYCLIC:		/*  19h   can_cwak to non-cyclic task     		*/
		iret = E_CTX;			/* (-25)  context error					*/
		break;
	case	SVCE_POOL_SIZE:			/*  1ah   memory pool size error          		*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_MEMORY_BLOCK:		/*  1bh   rel_blk to not system memory    		*/
		iret = E_MACV;			/* (-26)  memory access disabled or violation		*/
		break;
	case	SVCE_DEVICE_NO:			/*  1ch   illegal device number           		*/
		iret = E_PAR;			/* (-17)  parameter error				*/
		break;
	case	SVCE_MES_OVER:			/*  1dh   message buffer overflow         		*/
		/* iret = E_BOVR; */			/* (-58)  buffer overflow				*/
		iret = E_NOMEM;			/* (-33)  insufficient memory				*/
		break;
	case	SVCE_ERROR:			/*  1eh   others system error				*/
		iret = E_INOSPT;		/* (-97)  feature not supported by ITRON/FILE		*/
		break;
	case	SVCE_PARAM:			/*  1fh   illegal parameter               		*/
		iret = E_PAR;			/* (-17)  parameter error				*/
		break;
	case	SVCE_SVC:			/*  40h   illegal system cal number			*/
		iret = E_RSATR;			/* (-11)  reserved attribute				*/
		break;
	case	SVCE_EXCEPTION:			/*  80h   error in exception proc.        		*/
		iret = E_SYS;			/*  (-5)  system error					*/
		break;
	case	SVCE_SYS_OBJECT:		/*  81h   this object is system task			*/
		iret = E_SYS;			/*  (-5)  system error					*/
		break;
	default:
		iret = 0;
		break;
	}	/* End of switch */
	return (iret);
}
#endif	/* MEMO メモ書き */
