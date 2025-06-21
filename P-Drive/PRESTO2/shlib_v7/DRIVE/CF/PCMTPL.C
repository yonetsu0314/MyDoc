/*======================================================================
 * File name    : pcmtpl.c
 * Original		: r:/c/astra-s/exp/rcs/pcmtpl.c 1.3
 *----------------------------------------------------------------------
 * Function 	:
 *----------------------------------------------------------------------
 * $Header: p:/presto2/shlib_v7/drive/cf/RCS/pcmtpl.c 1.3 1970/01/01 00:00:00 sakaguti Exp $
 * $Log: pcmtpl.c $
 * リビジョン 1.3  1970/01/01  00:00:00  sakaguti
 * PCMCIA_..._BASEｱﾄﾞﾚｽをcf_base...から取得
 * 
 * リビジョン 1.2  1970/01/01  00:00:00  chimura
 * 2003/11/13 ワーニング削除
 * 
 * リビジョン 1.1  1970/01/01  00:00:00  sakaguti
 * LOG MESSAGE
 * 
 *----------------------------------------------------------------------
 *		(C) Copyright 2002, ISHIDA CO., LTD.                            
 *		959-1 SHIMOMAGARI RITTO-CITY SHIGA JAPAN                        
 *		PHONE (077) 553-4141                                            
 *----------------------------------------------------------------------*/
/************************************************************************/
/*                                                                      */
/*      ファイル名      : PCMTPL.C                                      */
/* ==================================================================== */
/*      機能分類        : ＰＣＭＣＩＡタプル処理				        */
/*                      : (ＰＣＭＣＩＡ準拠ではない)                    */
/*      作成日付        : 1997.12. 5 < Fri >                            */
/*      作成者名        :                                               */
/*                                                                      */
/*  履歴管理                                                            */
/* ------------+----------+-------------------------------------------- */
/*  日 付      | 管理番号 | 担当者名及び変更内容                        */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*             |          |                                             */
/* ------------+----------+-------------------------------------------- */
/*                                                                      */
/************************************************************************/
#include <stddef.h>
#include <string.h>
#include "drive\align.h"
#include "drive\pcmsock.h"
#include "drive\pcmcard.h"
#include "drive\pcmtpl.h"
#include <biosif\cf.h>

	/*==========================================================
							定数定義
	==========================================================*/
typedef unsigned char u_char;
#define LINK_MFC_N	0x00ff
#define LINK_C		0x0100
#define LINK_A		0x0200
#define LINK_MFC	0x0400
#define IS_ATTR 	0x1000
#define MFC_IS_ATTR	0x2000

	/*==========================================================
							構造体定義
	==========================================================*/
	/*==========================================================
							静的変数定義
	==========================================================*/
static const unsigned char mantissa[] = {
	10, 12, 13, 15, 20, 25, 30, 35,
	40, 45, 50, 55, 60, 70, 80, 90
};

static const unsigned long exponent[] = {
	1, 10, 100, 1000, 10000, 100000, 1000000, 10000000
};

	/*==========================================================
							マクロ関数定義
	==========================================================*/
#define MIN(a, b) (long)(((a) < (b)) ? (a) : (b))

/* アクセス速度タプルバイトをナノ秒に変換する */
#define SPEED_CVT(v) \
	(mantissa[(((v)>>3)&15)-1] * exponent[(v)&7] / 10)

/* 電圧タプルバイトをミリアンペアに変換する */
#define POWER_CVT(v) \
	(mantissa[((v)>>3)&15] * exponent[(v)&7] / 10)

#define POWER_SCALE(v) (exponent[(v)&7])

	/*==========================================================
							大域変数定義
	==========================================================*/
	/*==========================================================
						プロトタイプ定義
	==========================================================*/


/*=====================================================================
						ＣＩＳデータのリード
======================================================================*/
void read_cis_mem( int sock, int attr, long addr, long len, u_char *ptr )
{
	unsigned long i;

	if( len > 0 ) {
		for( i=0; i<len; i++ ) {
//			*ptr = *(unsigned char *)((attr? PCMCIA_ATTR_BASE:PCMCIA_COMM_BASE) + addr);
			*ptr = *(unsigned char *)((attr? (volatile unsigned char*)cf_base.attr:(volatile unsigned char*)cf_base.comm) + addr);
			ptr++;
			addr += 2;
		}
	}
}
/*---------------------------------------------------------------------

 ---------------------------------------------------------------------*/
static long follow_link( int sock, tuple_t *tuple )
{
	u_char link[5];
	long ofs;
	
	switch( tuple->Flags & (LINK_A | LINK_C | LINK_MFC) ) {
		case LINK_A:
			ofs = tuple->LinkOffset;
			tuple->Flags = IS_ATTR;
			break;
		case LINK_C:
			ofs = tuple->LinkOffset;
			tuple->Flags = 0;
			break;
		case LINK_MFC:
			/* Get indirect link from the MFC tuple */
			read_cis_mem( sock, (int)(tuple->Flags & MFC_IS_ATTR), (long)tuple->LinkOffset, 5, link);
			ofs = *(long *)(link+1);
			if (link[0] == CISTPL_MFC_ATTR)
				tuple->Flags |= IS_ATTR;
			else
				tuple->Flags &= ~IS_ATTR;
			/* Move to the next indirect link */
			tuple->Flags--;
			tuple->LinkOffset += (tuple->Flags & MFC_IS_ATTR) ? 10 : 5;
			if ((tuple->Flags & LINK_MFC_N) == 0)
				tuple->Flags &= ~LINK_MFC;
			break;
		default:
			return -1;
	}
	if( tuple->Flags & IS_ATTR ) {
		ofs = ofs << 1;
		read_cis_mem( sock, (int)(tuple->Flags & IS_ATTR), ofs, 5L, link);
		if ((link[0] == CISTPL_LINKTARGET) && (link[1] >= 3) &&
			(strncmp((char*)link+2, "CIS", 3) == 0))
			return ofs;
		ofs = ofs >> 1;
	}
	read_cis_mem( sock, (int)(tuple->Flags & IS_ATTR), ofs, 5, link);
	if ((link[0] != CISTPL_LINKTARGET) || (link[1] < 3) ||
		(strncmp((char*)link+2, "CIS", 3) != 0))
		return -1;
	return ofs;
}

/*=====================================================================
						カードサービス関数
	---------------------------------------------------------------
				現在の位置から次の指定タプル位置を検索
======================================================================*/
int GetNextTuple( int sock, tuple_t *tuple )
{
	u_char link[2];
	long ofs;
	int i, attr;

	/* 初回のタプル検索 */	
	if( tuple->CISOffset == -1 ) {
		ofs = 0;
		tuple->LinkOffset = 0;
		tuple->Flags = IS_ATTR | LINK_C;
	}
	/* ２回目以降のタプル検索 */
	else {
		if( tuple->TupleLink == 0xff ) {
			if ((ofs = follow_link( sock, tuple )) < 0)
				return CS_NO_MORE_ITEMS;
		}
		else {
			ofs = tuple->CISOffset +
				((tuple->Flags & IS_ATTR) ? 2 : 1) * (tuple->TupleLink+2);
		}
	}
	attr = tuple->Flags & IS_ATTR;
	
	for( i = 0; i < 1000; i++ ) {
		read_cis_mem( sock, attr, ofs, 2, link );

		if( link[0] == CISTPL_NULL ) {
			ofs += attr ? 2 : 1;
			continue;
		}
		
		if( link[0] == CISTPL_END ) {
			if( (ofs = follow_link( sock, tuple )) < 0 )
				return CS_NO_MORE_ITEMS;
			attr = tuple->Flags & IS_ATTR;
			read_cis_mem( sock, attr, ofs, 2, link );
		}

		if( (link[0] == CISTPL_LONGLINK_A) || (link[0] == CISTPL_LONGLINK_C) ||
		 (link[0] == CISTPL_LONGLINK_MFC) || (link[0] == CISTPL_LINKTARGET) ||
		 (link[0] == CISTPL_NO_LINK) ) {
			switch( link[0] ) {
				case CISTPL_LONGLINK_A:
					tuple->Flags = attr | LINK_A;
					read_cis_mem( sock, attr, ofs + (attr ? 4 : 2), 4,
								   (u_char *)&tuple->LinkOffset );
					break;
				case CISTPL_LONGLINK_C:
					tuple->Flags = attr | LINK_C;
					read_cis_mem( sock, attr, ofs + (attr ? 4 : 2), 4,
								   (u_char *)&tuple->LinkOffset );
					break;
				case CISTPL_LONGLINK_MFC:
					tuple->Flags = attr | LINK_MFC | 1;
					tuple->LinkOffset = ofs + (attr ? 2 : 1) * 8;
					if( attr )
						tuple->Flags |= MFC_IS_ATTR;
					break;
				case CISTPL_NO_LINK:
					tuple->Flags = attr;
					break;
			}
			if( (tuple->Attributes & TUPLE_RETURN_LINK) &&
				(tuple->DesiredTuple == RETURN_FIRST_TUPLE) )
				break;
		}
		else
			if( tuple->DesiredTuple == RETURN_FIRST_TUPLE )
				break;
		
		if( link[0] == tuple->DesiredTuple )
			break;
		ofs += (attr ? 2 : 1) * (link[1]+2);
	}
	if( i == 1000 ) {
		return CS_NO_MORE_ITEMS;
	}
	
	tuple->TupleCode = link[0];
	tuple->TupleLink = link[1];
	tuple->CISOffset = ofs;
	return CS_SUCCESS;
}

/*=====================================================================
						カードサービス関数
	---------------------------------------------------------------
			 	　    最初のタプル位置を検索
======================================================================*/
int GetFirstTuple( int sock, tuple_t *tuple )
{
	unsigned char req;

	tuple->CISOffset = -1;

	if( !(tuple->Attributes & TUPLE_RETURN_COMMON) ) {
		req = tuple->DesiredTuple;
		tuple->DesiredTuple = CISTPL_LONGLINK_MFC;

		if( GetNextTuple( sock, tuple ) != CS_SUCCESS )
			tuple->CISOffset = -1;
		tuple->DesiredTuple = req;
	}
	return GetNextTuple( sock, tuple );
}

/*=====================================================================
						カードサービス関数
	---------------------------------------------------------------
				 	検索したタプルの内容の取得
======================================================================*/
int GetTupleData( int sock, tuple_t *tuple )
{
	unsigned long len;
	
	if( tuple->TupleLink < tuple->TupleOffset )
		return CS_NO_MORE_ITEMS;
	len = tuple->TupleLink - tuple->TupleOffset;
	tuple->TupleDataLen = tuple->TupleLink;
	if( len == 0 )
		return CS_SUCCESS;

	if( tuple->Flags & IS_ATTR )
		read_cis_mem( sock, 1,(long)(tuple->CISOffset+((tuple->TupleOffset+2)<<1)),
					MIN(len, tuple->TupleDataMax), tuple->TupleData );
	else
		read_cis_mem( sock, 0, (long)(tuple->CISOffset+(tuple->TupleOffset+2)),
					MIN(len, tuple->TupleDataMax), tuple->TupleData );
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
			デバイス情報タプル（０１Ｈ，１７Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_device( tuple_t *tuple, cistpl_device_t *device )
{
	int i;
	u_char scale;
	u_char *p, *q;

	p = (u_char *)tuple->TupleData;
	q = p + tuple->TupleDataLen;

	device->ndev = 0;
	for (i = 0; i < CISTPL_MAX_DEVICES; i++) {
		
		if (*p == 0xff) break;
		device->dev[i].type = (u_char)(*p >> 4);
		device->dev[i].wp = (u_char)((*p & 0x08) ? 1 : 0);
		switch (*p & 0x07) {
		case 0: device->dev[i].speed = 0;	break;
		case 1: device->dev[i].speed = 250; break;
		case 2: device->dev[i].speed = 200; break;
		case 3: device->dev[i].speed = 150; break;
		case 4: device->dev[i].speed = 100; break;
		case 7:
			if (++p == q) return CS_BAD_TUPLE;
			if (p == q)
				return CS_BAD_TUPLE;
			device->dev[i].speed = SPEED_CVT(*p);
			while (*p & 0x80)
				if (++p == q) return CS_BAD_TUPLE;
			break;
		default:
			return CS_BAD_TUPLE;
		}

		if (++p == q) return CS_BAD_TUPLE;
		if (*p == 0xff) break;
		scale = (u_char)(*p & 7);
		if (scale == 7) return CS_BAD_TUPLE;
		device->dev[i].size = ((*p >> 3) + 1) * (512 << (scale*2));
		device->ndev++;
		if (++p == q) break;
	}
	
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
					チェックサムタプル（１０Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_checksum( tuple_t *tuple, cistpl_checksum_t *csum )
{
	char *p;
	if (tuple->TupleDataLen < 5)
		return CS_BAD_TUPLE;
	p = (char *)tuple->TupleData;
	csum->addr = *(unsigned short *)p;
	csum->len = *(unsigned short *)(p + 2);
	csum->sum = *(p+4);
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
			ロングリンクタプル（１１Ｈ，１２Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_longlink( tuple_t *tuple, cistpl_longlink_t *link )
{
	if (tuple->TupleDataLen < 4)
		return CS_BAD_TUPLE;
	link->addr = *(unsigned long *)tuple->TupleData;
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
		マルチファンクションカード用ロングリンクタプルの解析
 ---------------------------------------------------------------------*/
static int parse_longlink_mfc( tuple_t *tuple, cistpl_longlink_mfc_t *link )
{
	u_char *p;
	int i;
	
	p = tuple->TupleData;
	
	link->nfn = *p; p++;
	if (tuple->TupleDataLen <= link->nfn*5)
		return CS_BAD_TUPLE;
	for (i = 0; i < link->nfn; i++) {
		link->fn[i].space = *p; p++;
		link->fn[i].addr = *(unsigned long *)p; p += 4;
	}
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
					タプル文字列の取り出し
 ---------------------------------------------------------------------*/
static int parse_strings( u_char *p, u_char *q, int max, char *s, int *ofs, int *found )
{
	int i, j, ns;

	if (p == q) return CS_BAD_TUPLE;
	ns = 0; j = 0;
	for (i = 0; i < max; i++) {
		if (*p == 0xff) break;
		ofs[i] = j;
		ns++;
		for (;;) {
			s[j++] = (u_char)((*p == 0xff) ? '\0' : *p);
			if ((*p == '\0') || (*p == 0xff)) break;
			if (++p == q) return CS_BAD_TUPLE;
		}
		if ((*p == 0xff) || (++p == q)) break;
	}
	if (found) {
		*found = ns;
		return CS_SUCCESS;
	}
	else {
		if (ns == max)
			return CS_SUCCESS;
		else
			return CS_BAD_TUPLE;
	}
}

/*---------------------------------------------------------------------
				レベル１バージョンタプル（１５Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_vers_1( tuple_t *tuple, cistpl_vers_1_t *vers_1 )
{
	u_char *p, *q;
	
	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;
	
	vers_1->major = *p; p++;
	vers_1->minor = *p; p++;
	if (p >= q) return CS_BAD_TUPLE;

	return parse_strings(p, q, CISTPL_VERS_1_MAX_PROD_STRINGS,
						 vers_1->str, vers_1->ofs, &vers_1->ns);
}

/*---------------------------------------------------------------------
				各国語文字列タプル（１６Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_altstr( tuple_t *tuple, cistpl_altstr_t *altstr )
{
	u_char *p, *q;
	
	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;
	
	return parse_strings(p, q, CISTPL_MAX_ALTSTR_STRINGS,
						 altstr->str, altstr->ofs, &altstr->ns);
}

/*---------------------------------------------------------------------
			ＪＥＤＥＣデバイスＩＤタプル（１８Ｈ，１９Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_jedec( tuple_t *tuple, cistpl_jedec_t *jedec )
{
	u_char *p, *q;
	int nid;

	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;

	for (nid = 0; nid < CISTPL_MAX_DEVICES; nid++) {
		if (p > q-2) break;
		jedec->id[nid].mfr = p[0];
		jedec->id[nid].info = p[1];
		p += 2;
	}
	jedec->nid = nid;
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
				製造者ＩＤ（２０Ｈ）タプルの解析
 ---------------------------------------------------------------------*/
static int parse_manfid( tuple_t *tuple, cistpl_manfid_t *m )
{
	unsigned short *p;
	if (tuple->TupleDataLen < 4)
		return CS_BAD_TUPLE;
	p = (unsigned short *)tuple->TupleData;
	m->manf = p[0];
	m->card = p[1];
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
				ファンクションＩＤタプル（２１Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_funcid( tuple_t *tuple, cistpl_funcid_t *f )
{
	u_char *p;
	if (tuple->TupleDataLen < 2)
		return CS_BAD_TUPLE;
	p = (u_char *)tuple->TupleData;
	f->func = p[0];
	f->sysinit = p[1];
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
				ファンクション拡張タプル（２１Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_funce( tuple_t *tuple, cistpl_funce_t *f )
{
	u_char *p;
	int i;
	if (tuple->TupleDataLen < 1)
		return CS_BAD_TUPLE;
	p = (u_char *)tuple->TupleData;
	f->type = p[0];
	for (i = 1; i < tuple->TupleDataLen; i++)
		f->data[i-1] = p[i];
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
			コンフィギュレーションタプル(１ＡＨ）の解析
 ---------------------------------------------------------------------*/
static int parse_config( tuple_t *tuple, cistpl_config_t *config )
{
	int rasz, rmsz, i;
	u_char *p;

	p = tuple->TupleData;
	rasz = *p & 0x03;
	rmsz = (*p & 0x3c) >> 2;
	if (tuple->TupleDataLen < rasz+rmsz+4)
		return CS_BAD_TUPLE;
	config->last_idx = *(++p);
	p++;
	config->base = 0;
	for (i = 0; i <= rasz; i++)
		config->base += p[i] << (8*i);
	p += rasz+1;
	for (i = 0; i < 4; i++)
		config->rmask[i] = 0;
	for (i = 0; i <= rmsz; i++)
		config->rmask[i>>2] += p[i] << (8*(i%4));
	config->subtuples = (u_char)(tuple->TupleDataLen - (rasz+rmsz+4));
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
			コンフィギュレーションエントリタプルの解析
						（電源仕様の解析）
 ---------------------------------------------------------------------*/
static u_char *parse_power( u_char *p, u_char *q, cistpl_power_t *pwr )
{
	int i;
	unsigned long scale;

	if (p == q) return NULL;
	pwr->present = *p;
	pwr->flags = 0;
	p++;
	for (i = 0; i < 7; i++)
		if (pwr->present & (1<<i)) {
			if (p == q) return NULL;
			pwr->param[i] = POWER_CVT(*p);
			scale = POWER_SCALE(*p);
			while (*p & 0x80) {
				if (++p == q) return NULL;
				if ((*p & 0x7f) < 100)
					pwr->param[i] += (*p & 0x7f) * scale / 100;
				else if (*p == 0x7d)
					pwr->flags |= CISTPL_POWER_HIGHZ_OK;
				else if (*p == 0x7e)
					pwr->param[i] = 0;
				else if (*p == 0x7f)
					pwr->flags |= CISTPL_POWER_HIGHZ_REQ;
				else
					return NULL;
			}
			p++;
		}
	return p;
}

/*---------------------------------------------------------------------
			コンフィギュレーションエントリタプルの解析
					（タイミング仕様の解析）
 ---------------------------------------------------------------------*/
static u_char *parse_timing( u_char *p, u_char *q, cistpl_timing_t *timing )
{
	u_char scale;

	if (p == q) return NULL;
	scale = *p;
	if ((scale & 3) != 3) {
		if (++p == q) return NULL;
		timing->wait = SPEED_CVT(*p);
		timing->waitscale = exponent[scale & 3];
	}
	scale >>= 2;
	if ((scale & 7) != 7) {
		if (++p == q) return NULL;
		timing->ready = SPEED_CVT(*p);
		timing->rdyscale = exponent[scale & 7];
	}
	scale >>= 3;
	if (scale != 7) {
		if (++p == q) return NULL;
		timing->reserved = SPEED_CVT(*p);
		timing->rsvscale = exponent[scale];
	}
	p++;
	return p;
}

/*---------------------------------------------------------------------
			コンフィギュレーションエントリタプルの解析
					（ＩＯウィンドウ仕様の解析）
 ---------------------------------------------------------------------*/
static u_char *parse_io(u_char *p, u_char *q, cistpl_io_t *io)
{
	int i, j, bsz, lsz;

	if (p == q) return NULL;
	io->flags = *p;

	if (!(*p & 0x80)) {
		io->nwin = 1;
		io->win[0].base = 0;
		io->win[0].len = (1 << (io->flags & CISTPL_IO_LINES_MASK));
		return p+1;
	}
	
	if (++p == q) return NULL;
	io->nwin = (*p & 0x0f) + 1;
	bsz = (*p & 0x30) >> 4;
	if (bsz == 3) bsz++;
	lsz = (*p & 0xc0) >> 6;
	if (lsz == 3) lsz++;
	p++;
	
	for (i = 0; i < io->nwin; i++) {
		io->win[i].base = 0;
		io->win[i].len = 1;
		for (j = 0; j < bsz; j++, p++) {
			if (p == q) return NULL;
			io->win[i].base += *p << (j*8);
		}
		for (j = 0; j < lsz; j++, p++) {
			if (p == q) return NULL;
			io->win[i].len += *p << (j*8);
		}
	}
	return p;
}

/*---------------------------------------------------------------------
			コンフィギュレーションエントリタプルの解析
				（メモリウィンドウ仕様の解析）
 ---------------------------------------------------------------------*/
static u_char *parse_mem(u_char *p, u_char *q, cistpl_mem_t *mem)
{
	int i, j, asz, lsz, has_ha;
	unsigned long len, ca, ha;

	if (p == q) return NULL;

	mem->nwin = (u_char)((*p & 0x07) + 1);
	lsz = (*p & 0x18) >> 3;
	asz = (*p & 0x60) >> 5;
	has_ha = (*p & 0x80);
	if (++p == q) return NULL;
	
	for (i = 0; i < mem->nwin; i++) {
		len = ca = ha = 0;
		for (j = 0; j < lsz; j++, p++) {
			if (p == q) return NULL;
			len += *p << (j*8);
		}
		for (j = 0; j < asz; j++, p++) {
			if (p == q) return NULL;
			ca += *p << (j*8);
		}
		if (has_ha)
			for (j = 0; j < asz; j++, p++) {
				if (p == q) return NULL;
				ha += *p << (j*8);
			}
		mem->win[i].len = len << 8;
		mem->win[i].card_addr = ca << 8;
		mem->win[i].host_addr = (char*)(ha << 8);
	}
	return p;
} /* parse_mem */

/*---------------------------------------------------------------------
			コンフィギュレーションエントリタプルの解析
						（割り込みマスクの解析）
 ---------------------------------------------------------------------*/
static u_char *parse_irq(u_char *p, u_char *q, cistpl_irq_t *irq)
{
	if (p == q) return NULL;
	irq->IRQInfo1 = *p; p++;
	if (irq->IRQInfo1 & IRQ_INFO2_VALID) {
		if (p+2 > q) return NULL;
		irq->IRQInfo2 = (p[1]<<8) + p[0];
		p += 2;
	}
	return p;
}

/*---------------------------------------------------------------------
		コンフィギュレーションエントリタプル（１ＢＨ）の解析
 ---------------------------------------------------------------------*/
static int parse_cftable_entry( tuple_t *tuple, cistpl_cftable_entry_t *entry )
{
	u_char *p, *q, features;
	unsigned short ss;

	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;
	entry->index = (u_char)(*p & 0x3f);
	entry->flags = 0;
	if (*p & 0x40)
		entry->flags |= CISTPL_CFTABLE_DEFAULT;
	if (*p & 0x80) {
		if (++p == q) return CS_BAD_TUPLE;
		if (*p & 0x10)
			entry->flags |= CISTPL_CFTABLE_BVDS;
		if (*p & 0x20)
			entry->flags |= CISTPL_CFTABLE_WP;
		if (*p & 0x40)
			entry->flags |= CISTPL_CFTABLE_RDYBSY;
		if (*p & 0x80)
			entry->flags |= CISTPL_CFTABLE_MWAIT;
		entry->interface = (u_char)(*p & 0x0f);
	}
	else
		entry->interface = 0;

	/* Process optional features */
	if (++p == q) return CS_BAD_TUPLE;
	features = *p; p++;

	/* Power options */
	if ((features & 3) > 0) {
		p = parse_power(p, q, &entry->vcc);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else entry->vcc.present = 0;
	if ((features & 3) > 1) {
		p = parse_power(p, q, &entry->vpp1);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else entry->vpp1.present = 0;
	if ((features & 3) > 2) {
		p = parse_power(p, q, &entry->vpp2);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else entry->vpp2.present = 0;

	/* Timing options */
	if (features & 0x04) {
		p = parse_timing(p, q, &entry->timing);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else {
		entry->timing.wait = 0;
		entry->timing.ready = 0;
		entry->timing.reserved = 0;
	}
	
	/* I/O window options */
	if (features & 0x08) {
		p = parse_io(p, q, &entry->io);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else
		entry->io.nwin = 0;
	
	/* Interrupt options */
	if (features & 0x10) {
		p = parse_irq(p, q, &entry->irq);
		if (p == NULL) return CS_BAD_TUPLE;
	}
	else
		entry->irq.IRQInfo1 = 0;

	switch (features & 0x60) {
	case 0x00:
		entry->mem.nwin = 0;
		break;
	case 0x20:
		entry->mem.nwin = 1;
/*		entry->mem.win[0].len = (*(unsigned short *)p) << 8;*/
		ss = shortswap(p);
		entry->mem.win[0].len = ss << 8;

		entry->mem.win[0].card_addr = 0;
		entry->mem.win[0].host_addr = NULL;
		p += 2;
		if (p > q) return CS_BAD_TUPLE;
		break;
	case 0x40:
		entry->mem.nwin = 1;
/*		entry->mem.win[0].len = (*(unsigned short *)p) << 8;*/
		ss = shortswap(p);
		entry->mem.win[0].len = ss << 8;

/*		entry->mem.win[0].card_addr = (*(unsigned short *)(p+2)) << 8;*/
		ss = shortswap(p+2);
		entry->mem.win[0].card_addr = ss << 8;

		entry->mem.win[0].host_addr = (char*)entry->mem.win[0].host_addr;
		p += 4;
		if (p > q) return CS_BAD_TUPLE;
		break;
	case 0x60:
		p = parse_mem(p, q, &entry->mem);
		if (p == NULL) return CS_BAD_TUPLE;
		break;
	}

	/* Misc features */
	if (features & 0x80) {
		if (p == q) return CS_BAD_TUPLE;
		if (*p & 0x08)
			entry->flags |= CISTPL_CFTABLE_AUDIO;
		if (*p & 0x10)
			entry->flags |= CISTPL_CFTABLE_READONLY;
		if (*p & 0x20)
			entry->flags |= CISTPL_CFTABLE_PWRDOWN;
		while (*p & 0x80)
			if (++p == q) return CS_BAD_TUPLE;
		p++;
	}

	entry->subtuples = (u_char)(q-p);
	
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
			デバイスジオメトリタプル（１Ｅ，１ＦＨ）の解析
 ---------------------------------------------------------------------*/
static int parse_device_geo( tuple_t *tuple, cistpl_device_geo_t *geo )
{
	u_char *p, *q;
	int n;

	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;

	for (n = 0; n < CISTPL_MAX_DEVICES; n++) {
		if (p > q-6) break;
		geo->geo[n].buswidth = p[0];
		geo->geo[n].erase_block = 1 << (p[1]-1);
		geo->geo[n].read_block	= 1 << (p[2]-1);
		geo->geo[n].write_block = 1 << (p[3]-1);
		geo->geo[n].partition	= 1 << (p[4]-1);
		geo->geo[n].interleave	= 1 << (p[5]-1);
		p += 6;
	}
	geo->ngeo = n;
	return CS_SUCCESS;
}

/*---------------------------------------------------------------------
				レベル２バージョンタプル（４０Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_vers_2( tuple_t *tuple, cistpl_vers_2_t *v2 )
{
	u_char *p, *q;

	if (tuple->TupleDataLen < 10)
		return CS_BAD_TUPLE;
	
	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;

	v2->vers = p[0];
	v2->comply = p[1];
	v2->dindex = *(unsigned short *)(p+2);
	v2->vspec8 = p[6];
	v2->vspec9 = p[7];
	v2->nhdr = p[8];
	p += 9;
	return parse_strings(p, q, 2, v2->str, &v2->vendor, NULL);
}

/*---------------------------------------------------------------------
			パーティション内容情報タプル（４６Ｈ）の解析
 ---------------------------------------------------------------------*/
static int parse_org( tuple_t *tuple, cistpl_org_t *org )
{
	u_char *p, *q;
	int i;
	
	p = tuple->TupleData;
	q = p + tuple->TupleDataLen;
	if (p == q) return CS_BAD_TUPLE;
	org->data_org = *p;
	if (++p == q) return CS_BAD_TUPLE;
	for (i = 0; i < 30; i++) {
		org->desc[i] = *p;
		if (*p == '\0') break;
		if (++p == q) return CS_BAD_TUPLE;
	}
	return CS_SUCCESS;
}

/*=====================================================================
						カードサービス関数
	---------------------------------------------------------------
				 	検索したタプルの内容の解析
======================================================================*/
int ParseTuple( tuple_t *tuple, cisparse_t *parse )
{
	int ret = CS_SUCCESS;
	
	if( tuple->TupleDataLen > tuple->TupleDataMax )
		return CS_BAD_TUPLE;

	switch( tuple->TupleCode ) {
		case CISTPL_DEVICE:
		case CISTPL_DEVICE_A:
			ret = parse_device( tuple, &parse->device );
			break;
		case CISTPL_CHECKSUM:
			ret = parse_checksum( tuple, &parse->checksum );
			break;
		case CISTPL_LONGLINK_A:
		case CISTPL_LONGLINK_C:
			ret = parse_longlink( tuple, &parse->longlink );
			break;
		case CISTPL_LONGLINK_MFC:
			ret = parse_longlink_mfc( tuple, &parse->longlink_mfc );
			break;
		case CISTPL_VERS_1:
			ret = parse_vers_1( tuple, &parse->version_1 );
			break;
		case CISTPL_ALTSTR:
			ret = parse_altstr( tuple, &parse->altstr );
			break;
		case CISTPL_JEDEC_A:
		case CISTPL_JEDEC_C:
			ret = parse_jedec( tuple, &parse->jedec );
			break;
		case CISTPL_MANFID:
			ret = parse_manfid( tuple, &parse->manfid );
			break;
		case CISTPL_FUNCID:
			ret = parse_funcid( tuple, &parse->funcid );
			break;
		case CISTPL_FUNCE:
			ret = parse_funce( tuple, &parse->funce );
			break;
		case CISTPL_CONFIG:
			ret = parse_config( tuple, &parse->config );
			break;
		case CISTPL_CFTABLE_ENTRY:
			ret = parse_cftable_entry( tuple, &parse->cftable_entry );
			break;
		case CISTPL_DEVICE_GEO:
		case CISTPL_DEVICE_GEO_A:
			ret = parse_device_geo( tuple, &parse->device_geo );
			break;
		case CISTPL_VERS_2:
			ret = parse_vers_2( tuple, &parse->vers_2 );
			break;
		case CISTPL_ORG:
			ret = parse_org( tuple, &parse->org );
			break;
		default:
			ret = CS_UNSUPPORTED_FUNCTION;
			break;
	}
	return ret;
}


int ReadFirstTuple( int sock, tuple_t *tuple, cisparse_t *parse )
{
	if( GetFirstTuple( sock, tuple ) != CS_SUCCESS )
		return -1;
	if( GetTupleData( sock, tuple ) != CS_SUCCESS )
		return -1;
	if( ParseTuple( tuple, parse ) != CS_SUCCESS )
		return -1;
	return CS_SUCCESS;
}
