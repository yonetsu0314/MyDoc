/*----------------------------------------------------------------------*
 * File name	: gets.c		/ romable gets							*
 *----------------------------------------------------------------------*
 *$Header: p:/h8lib/rom/src/rcs/gets.c 1.1 1996/01/19 09:45:08 kabumoto Exp $
 *$Log: gets.c $
 * リビジョン 1.1  1996/01/19  09:45:08  kabumoto
 * 初期リビジョン
 * 
 *----------------------------------------------------------------------*/
/*				(c) Copyright 1993, ISHIDA  CO., LTD.					*/
/*				959-1 SHIMOMAGARI RITTO-CHO KURITA-GUN					*/
/*				SHIGA JAPAN												*/
/*				(0775) 53-4141											*/
/*----------------------------------------------------------------------*/
/*	n = gets(char *s)													*/
/*		read a string of characters	from stdin input stream,			*/
/*		into the buffer pointed by s.									*/
/*		gets doesn't put <CR> in the caller's buffer.					*/
/*		return read character number if ESC key read,return -1			*/
/*----------------------------------------------------------------------*/

#define	_LF		0x0a
#define	_CR		0x0d
#define	_BS		0x08
#define	_ESC	0x1b
#define	_DEL	0x7f

gets(s)
	register	char	*s;
{
	register	int	cnt;
	register	char	c;
	cnt = 0;

	while(1)
	{
	c = getch();
	switch(c)
		{
		case _CR:	*s = 0;
				putch('\n');
				putch('\r');
				return(cnt);
				break;

		case _DEL:	c = _BS;
		case _BS:	if(cnt != 0)
				{
				putch(_BS); putch(' '); putch(_BS);
				--cnt;
				--s;
				}
				break;

		case _ESC:	*s = 0;
				return(-1);
				break;
	
		default:	if(c > ' ')
				{
				*s++ = c;
				putch(c);
				++cnt;
				}
				break;
		}
	}

}
