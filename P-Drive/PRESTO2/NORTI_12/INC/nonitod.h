/*************************************************************************/
/* Integer-String Convertor Functions                                    */
/*                                                                       */
/*  Copyright (c) 1999-2003 MiSPO Co., Ltd.                              */
/*  All rights reserved.                                                 */
/*                                                                       */
/* 02/Feb/1999                                                           */
/* 22/Mar/1999 ?tolen -> ?todlen, ?toulen                                */
/* 12/Jun/1999 void -> char *                                            */
/* 31/Jan/2002 C++ style added                                           */
/*************************************************************************/

#ifndef NONITOD_H
#define NONITOD_H
#ifdef __cplusplus
extern "C" {
#endif

int itoulen(unsigned short);
int ltoulen(unsigned long);
int itodlen(short);
int ltodlen(long);
char *ito0x(char *, unsigned short, int);
char *ito0X(char *, unsigned short, int);
char *lto0x(char *, unsigned long, int);
char *lto0X(char *, unsigned long, int);
char *itod(char *, short, int);
char *itou(char *, unsigned short, int);
char *ito_d(char *, short, int);
char *ito_u(char *, unsigned short, int);
char *ito0u(char *, unsigned short, int);
char *ltod(char *, long, int);
char *ltou(char *, unsigned long, int);
char *lto_d(char *, long, int);
char *lto_u(char *, unsigned long, int);
char *lto0u(char *, unsigned long, int);
char *ito_f(char *, short, int, int);
char *ito_fu(char *, unsigned short, int, int);
char *lto_f(char *, long, int, int);
char *lto_fu(char *, unsigned long, int, int);
unsigned short xtoi(const char *);
unsigned long xtol(const char *);
long atofl(const char *, int);

#ifdef __cplusplus
}
#endif
#endif /* NONITODS_H */

