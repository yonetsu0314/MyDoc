/*----------------------------------------------------------------------*
 * File name	: m37702.h						*
 *----------------------------------------------------------------------*
 *$Header$
 *$Log$
 *----------------------------------------------------------------------*/
/*		(c) Copyright 1988, ISHIDA SCALES MFG. CO., LTD.	*/
/*		959-1 SHIMOMAGARI RITTO-CITY				*/
/*		SHIGA JAPAN						*/
/*		(0775) 53-4141						*/
/*----------------------------------------------------------------------*/

#if	!defined(M37702_H)
#define	M37702_H

void	alert_bell(int n);
void	back_light(int n);
void	bell(void );
void	bell_repeat(int period, int length, int count);
void	buzzer(int length);
void	error_bell(int n);
void	green_led(int n);
void	key_repeat(int period,int click,int saver);
int	lprintf(char *fmt, ...);
int	lputc(int c);
int	lputs(char *s);
void	print_buff_clear(void);
int	print_init(void);
int	print_buff_check(void);
int	print_buf_left(void);
void	print_reset(void);
int	print_send(void);
void	red_led(int n);
void	warning_bell(int n);
void	yellow_led(int n);

#endif	/* M37702_H */

