/**************************************************************************
* NORTi TELNET Function Header                                            *
*                                                                         *
*  Copyright (c) 1998-2003, MiSPO Co., Ltd.                               *
*  All rights reserved.                                                   *
*                                                                         *
* 25/Feb/1999                                                             *
* 23/May/1999 SHELL_SSZ 512 -> 2048                                       *
* 25/Apr/1999 void terminal_putch -> BOOL terminal_putch                  *
* 01/May/1999 'time' is added to T_TELNETD                                *
* 12/Feb/2001 Change Task priority 4 -> 5  TELNETD_PRI,SHELL_PRI          *
* 18/Mar/2003 TELNETD_SSZ and SHELL_SSZ became configurable            YS *
**************************************************************************/

#ifndef NONTELN_H
#define NONTELN_H
#ifdef __cplusplus
extern "C" {
#endif

#define TELNETD_PRI     5      /* TELNET Daemon Task Priority */
#define SHELL_PRI       5       /* Command Shell Task Priority */
#ifndef TELNETD_SSZ
#define TELNETD_SSZ     512     /* TELNET Daemon Task Stack Size */
#endif
#ifndef SHELL_SSZ
#define SHELL_SSZ       2048    /* Command Shell Task Stack Size */
#endif

#define LOGIN_NAME_LEN  10      /* Login Name Max. Length */
#define PASSWORD_LEN    10      /* Password Max. Length */
#define COMMAND_LINE_LEN 80     /* Command Line Max. Length */

#define TELNETD_SBUFSZ  256     /* Sending Window Buffer Size */
#define TELNETD_RBUFSZ  256     /* Receiving Window Buffer Size */

/* TELNET Daemon Control Block */

typedef struct t_telnetd {
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
    ID cepid;
    ID repid;
    UW time;
    T_IPV4EP dstaddr;
    UH sbuf[TELNETD_SBUFSZ/2];  /* Sending Window Buffer */
    UH rbuf[TELNETD_RBUFSZ/2];  /* Receiving Window Buffer */
} T_TELNETD;

typedef T_TELNETD T_TERMINAL;

/* Callback Routine to Check Login Password */

typedef B *(*PASSWD_CHECK)(T_TERMINAL *, const B *, const B *);

/* TELNET Support Function Prototypes */

BOOL telnetd_callback(T_TERMINAL *t, B *s);
int telnet_command(int argc, char *argv[]);

/* TELNET Daemon Low Level Function Prototypes */

BOOL telnetd_print(T_TELNETD *t, const B *s);
INT telnetd_input(T_TELNETD *t, B *buf, INT size);
ER telnetd_ini(T_TELNETD *t, ID tskid, ID mbfid, ID cepid, ID repid);

/* Remote Terminal Function Prototypes */

BOOL terminal_puts(T_TERMINAL *t, const B *s);
BOOL terminal_rputs(T_TERMINAL *t, const B *s);
BOOL terminal_print(T_TERMINAL *t, const B *s);
INT terminal_input(T_TERMINAL *t, B *buf, INT size);
B *terminal_gets(T_TERMINAL *t, B *s, INT size);
BOOL terminal_putch(T_TERMINAL *t, INT c);
INT terminal_getch(T_TERMINAL *t);
BOOL terminal_kbhit(T_TERMINAL *t);

/* Command Shell Function Prototypes */

BOOL login_proc(T_TERMINAL *t, B *login_prompt, B *passwd_prompt, PASSWD_CHECK callback);
ER shell_ini(VP t, ID tskid, ID mbfid, PASSWD_CHECK callback);

/* stdio.h, conio.h */

#ifndef NONCONS_H
#define puts(b)         terminal_puts(NULL,b)
#define rputs(b)        terminal_rputs(NULL,b)
#define print(b)        terminal_print(NULL,b)
#define input(b,s)      terminal_input(NULL,b,s)
#define gets(b)         terminal_gets(NULL,b,sizeof(b))
#define putch(c)        terminal_putch(NULL,c)
#define getch()         terminal_getch(NULL)
#define kbhit()         terminal_kbhit(NULL)
#endif

/* signal.h */

typedef void (*SIGNAL_FUNC)(int);
SIGNAL_FUNC terminal_signal(T_TERMINAL *, int, SIGNAL_FUNC);
#define signal(s,f)     terminal_signal(NULL, s, f)

#define SIGINT          2           /* CTRL-C Signal */
#define SIGBREAK        21          /* Ctrl-Break Sequence */
#define SIG_DFL ((void(*)(int))0)   /* Default Signal Action */
#define SIG_ERR ((void(*)(int))(-1))/* Error */

/* string.h */

int stricmp(const char *d, const char *s);
char *strlwr(char *s);

#ifdef __cplusplus
}
#endif
#endif /* NONTELN_H */
