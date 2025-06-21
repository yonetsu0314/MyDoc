/**************************************************************************
* NORTi Console Function Header                                           *
*                                                                         *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                               *
*  All rights reserved.                                                   *
*                                                                         *
* 25/Feb/1999                                                             *
**************************************************************************/

#ifndef NONCONS_H
#define NONCONS_H
#ifdef __cplusplus
extern "C" {
#endif

/* UART Console Control Block */

typedef struct t_console {
    ID telnetd_tskid;
    ID shell_tskid;
    ID rx_mbfid;
    ID tx_mbfid;
    FP sigint;
    FP passwd_check;
    ER ercd;
    B logged_in;
    B echo_on;
    B *prompt;
    ID cepid; /* == 0 */
    INT ch;
} T_CONSOLE;

/* Console Low Level Function Prototypes */

BOOL console_print(T_CONSOLE *t, const B *s);
INT console_input(T_CONSOLE *t, B *buf, int size);
INT console_getch(T_CONSOLE *t);
BOOL console_kbhit(T_CONSOLE *t);
ER console_ini(T_CONSOLE *t, ID tskid, ID mbfid, INT ch, const B *param);

#ifndef NONTELN_H
#define puts(b)         console_puts(NULL,b)
#define rputs(b)        console_rputs(NULL,b)
#define print(b)        console_print(NULL,b)
#define input(b,s)      console_input(NULL,b,s)
#define gets(b)         console_gets(NULL,b,sizeof(b))
#define putch(c)        console_putch(NULL,c)
#define getch()         console_getch(NULL)
#define kbhit()         console_kbhit(NULL)
#endif

#ifdef __cplusplus
}
#endif
#endif /* NONCONS_H */
