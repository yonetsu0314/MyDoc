/*======================================================================
 * File name    : rzr219.c
 * Original	: 
 *----------------------------------------------------------------------
 * Function	:
 *	ÇcÇeÅ@ÇnÇméûä‘Å^ÇnÇeÇeéûä‘Å@ÉeÉLÉXÉgâì«
 *----------------------------------------------------------------------
 *$Header: p:/presto2/shlib_v7/rzcmdx/src/RCS/rzr219.c 1.1 1970/01/01 00:00:00 chimura Exp $
 *$Log: rzr219.c $
 * ÉäÉrÉWÉáÉì 1.1  1970/01/01  00:00:00  chimura
 * èâä˙ÉäÉrÉWÉáÉì
 * 
 *----------------------------------------------------------------------
 *Header: p:/lib/rzcmdx/src/RCS/rzr219.c 1.2 1995/07/20 18:51:49 hiroki
 *Log: rzr219.c
 * ÉäÉrÉWÉáÉì 1.2  1995/07/20  18:51:49  hiroki
 * htoi->htocÇ…èCê≥Ç∑ÇÈ
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
#define		STORAGE
#include	<rzcmd\rzr219.h>		/* Show Current		*/
#undef		STORAGE


/*----------------------------------------------------------------------*/
/*	rz_scan_df_time							*/
/*----------------------------------------------------------------------*/
/* ê‡ñæ					  				*/
/*	ÇcÇeÇÃÇnÇméûä‘ãyÇ—ÇnÇeÇeéûä‘ÉeÉLÉXÉgÇç\ë¢ëÃÇ…ÉZÅ[ÉuÇ∑ÇÈÅBÅ@Å@Å@*/
/*----------------------------------------------------------------------*/
/* à¯Ç´êî	      							*/
/*	*buf	: éÛêM√∑Ωƒ• ﬁØÃßÇ÷ÇÃŒﬂ≤›¿		    		*/
/*									*/
/* ñﬂÇËíl								*/
/*	int	: 0	••• ê≥èÌèIóπ					*/
/*		  -1	••• ÉGÉâÅ[					*/
/*----------------------------------------------------------------------*/

int
rz_scan_df_time(
	struct fmt_df_time	*buf
	)
{
	BYTE		*p;
	int		i, j;

	rzr219s.min_df_no = ascii_hex( buf->min_df_no );
	rzr219s.nof_df    = ascii_hex( buf->nof_df );
	
	p = (BYTE_PTR)&buf->df;
	
	for(i = rzr219s.nof_df, j = rzr219s.min_df_no-1; i ; i--, j++) {
		rzr219s.df[j].on_time = htoc( (char *)p );
		p += sizeof(buf->df[0].on_time);
		rzr219s.df[j].off_time = htoc( (char *)p );
		p += sizeof(buf->df[0].off_time);
	}
	return(0);
}
