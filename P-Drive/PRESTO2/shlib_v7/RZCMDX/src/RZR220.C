/*======================================================================
 * File name    : rzr220.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ÇcÇeÅ@ÇnÇméûä‘Å^ÇnÇeÇeéûä‘Å@ÉeÉLÉXÉgçÏê¨
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr220.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr220.c $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  chimura
 * èâä˙ÉäÉrÉWÉáÉì
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr220.c 1.2 1995/07/20 18:52:30 hiroki
 *Log: rzr220.c
 * ÉäÉrÉWÉáÉì 1.2  1995/07/20  18:52:30  hiroki
 * itoc->ctohÇ…èCê≥
 *----------------------------------------------------------------------
 *		(c) Copyright 1993, ISHIDA CO., LTD.
 *		959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN
 *		SHIGA JAPAN
 *		(0775) 53-4141
 *======================================================================
 */

#include	<custom\typedefs.h>		/* Show xlib		*/
#include	<custom\custom.h>		/*	"		*/
#include	<rzcmd\rzcmd.h>			/* Show Current		*/
#include	<rzcmd\rzr219.h>		/* Show Current		*/


/*----------------------------------------------------------------------*/
/*	rz_print_df_time						*/
/*----------------------------------------------------------------------*/
/* ê‡ñæ					  				*/
/*	ÇcÇeÅ@ÇnÇmÅ^ÇnÇeÇeéûä‘Å@ÉfÅ[É^ÇÉeÉLÉXÉgÇ…ïœä∑Ç∑ÇÈÅB    	*/
/*----------------------------------------------------------------------*/
/* à¯Ç´êî	      							*/
/*	*txt	: ëóêM√∑Ωƒ• ﬁØÃßÇ÷ÇÃŒﬂ≤›¿(char *)	    		*/
/*	*pkt	:  ﬂ◊“∞¿• ﬂπØƒÇ÷ÇÃŒﬂ≤›¿		    			*/
/*									*/
/* ñﬂÇËíl								*/
/*	int	: txt+sizeof(fmt_cb_result)	••• ê≥èÌèIóπ		*/
/*		  NULL				••• ÉGÉâÅ[		*/
/*----------------------------------------------------------------------*/

void *
rz_print_df_time(
	struct fmt_df_time	*txt,
	struct rzr219t		*pkt
	)
{
	BYTE		*buf;
	int		i, j;

	txt->min_df_no = hex_ascii( pkt->min_df_no );
	txt->nof_df    = hex_ascii( pkt->nof_df );

	buf = (BYTE_PTR)txt->df;

	for(i = pkt->nof_df, j = pkt->min_df_no-1 ; i ; i--, j++) {
		ctoh(pkt->df[j].on_time, (char *)buf);
		buf += sizeof(txt->df[0].on_time);
		ctoh(pkt->df[j].off_time, (char *)buf);
		buf += sizeof(txt->df[0].off_time);
	}
	return(buf);
}
