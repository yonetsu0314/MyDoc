/*======================================================================
 * File name    : prr3018.c
 * Original	: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr318.c 1.1
 *----------------------------------------------------------------------
 * Function	:
 *	 �^�C�~���O�e�L�X�g�쐬
 *----------------------------------------------------------------------
 *$Header: P:/PRESTO2/shlib_v7/PRCMDX/src/RCS/prr3018.c 1.1 2015-05-28 12:01:07+09 itami Exp $
 *$Log: prr3018.c $
 *Revision 1.1  2015-05-28 12:01:07+09  itami
 *Initial revision
 *
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<prcmd\prcmd.h>			/* Show Current		*/
#include	<prcmd\prr3017.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	pr_print_round_tmg						*/
/*----------------------------------------------------------------------*/
/* ����					  				*/
/*	���E���f�B���O�f�[�^���e�L�X�g�ɕϊ�����B			*/
/*----------------------------------------------------------------------*/
/* ������	      							*/
/*	*txt	: ���M÷�ĥ�ޯ̧�ւ��߲��(char *)	    		*/
/*	*pkt	: ���Ұ���߹�Ăւ��߲��		    			*/
/*									*/
/* �߂�l								*/
/*	int	: txt+sizeof(fmt_round_tmg)	��� ����I��		*/
/*		  NULL				��� �G���[		*/
/*----------------------------------------------------------------------*/

void *
pr_print_round_tmg(
	struct fmt_round_tmg	*txt,
	struct prr3017t		*pkt
	)
{
	BYTE		*buf;
	int		i, j;

	txt->nof_round  = hex_ascii( pkt->nof_round );
	buf = (BYTE_PTR)txt->round_base[0];
	for(i = pkt->nof_round, j = 0 ; i ; i--, j++) {
		ctoh( pkt->round_base[j], (char *)buf );
		buf += sizeof(txt->round_base[0]);
	}
	return(buf);
}
