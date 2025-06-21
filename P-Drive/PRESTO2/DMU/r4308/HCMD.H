/*======================================================================
 * File name    : hcmd.h
 * Original	: 
 *----------------------------------------------------------------------
 * Function 	: ホスト連動に使用されるコマンド
 *----------------------------------------------------------------------
 *$Header: p:/nhic/main/ccw/rcs/hcmd.h 1.1 1998/08/26 18:29:31 yonetsu Exp $
 *$Log: hcmd.h $
 * リビジョン 1.1  1998/08/26  18:29:31  yonetsu
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------
 *		(c) Copyright 1995-1998, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */
#ifndef _HCMD_H_
#define	_HCMD_H_

#define	PRE_NUM_CMD	'0'
#define	PRE_REQ_CMD	'1'
#define	HCMD_MD_CHG	'2'
#define	HCMD_ROM_REQ	','
#define	HCMD_ROM_DATA	'Y'
#define	HCMD_STAT_REQ	'@'
#define	HCMD_STAT_DATA	'9'
#define	HCMD_CTTL_REQ	'#'
#define	HCMD_TTL_DATA	'U'
#define	HCMD_HST_REQ	')'
#define	HCMD_HST_DATA	'K'
#define	HCMD_EFFI_REQ	'-'
#define	HCMD_EFFI_DATA	'Q'
#define	HCMD_ATTL_REQ	'$'
#define	HCMD_ERR_REQ	'.'
#define	HCMD_ERR_DATA	'I'
#define	HCMD_CB_REQ	'('
#define	HCMD_CB_DATA	'E'
#define	HCMD_PRE_REQ	'%'
#define	HCMD_PRE_DATA	'P'
#define	HCMD_FD_REQ	'{'
#define	HCMD_FD_DATA	'f'
#define	HCMD_TMG_REQ	'}'
#define	HCMD_TMG_DATA	't'
#define	HCMD_CBP_REQ	'['
#define	HCMD_CBP_DATA	'c'
#define	HCMD_SDP_REQ	':'
#define	HCMD_SDP_DATA	'j'
#define	HCMD_IFD_REQ	';'
#define	HCMD_IFD_DATA	'k'
#define	HCMD_ILK_REQ	']'
#define	HCMD_ILK_DATA	'i'
#define	HCMD_TTL_CLR	'/'
#define	HCMD_TMR_SET	'!'
#define	HCMD_PW_ON	'W'
#define	HCMD_PW_OFF	'X'
#define	HCMD_PROD_STRT	'R'
#define	HCMD_PROD_STOP	'S'
#define	HCMD_DRAIN_STRT	'G'
#define	HCMD_DRAIN_STOP	'H'
#define	HCMD_ZERO_ADJ	'Z'
#define	HCMD_ERR_REJ	'D'
#define	HCMD_MCAL_REQ	'C'
#define	HCMD_MCAL_DATA	'B'
#define	HCMD_ACT_COMP	'>'
#define	HCMD_ILLEGAL	'?'
#define	HCMD_CURR_PROD	'm'	/*現在の予約で運転*/
#define	HCMD_CURR_DRAIN	'n'	/*現在の予約で排出*/
#define	HCMD_REQ_TXTFRM	'T'	/*テキストフォーマット変更*/
#define HCMD_NET_ERR	'<'	/* HIF-NET異常応答 */
#endif
/* end of file */
