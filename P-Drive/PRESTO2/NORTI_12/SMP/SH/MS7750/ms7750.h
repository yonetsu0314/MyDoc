/******************************************************************************
* MS7750SE01 (SH7750) ボード固有定義                                          *
*                                                                             *
*  File name : ms7750.h                                                       *
*  Copyright (c) 1999, MiSPO Co., Ltd.                                        *
*                                                                             *
* 29/Nov/1999 Created for MS7750SE01 board.                       [YT/Tygnus] *
******************************************************************************/

#ifndef MS7750_H
#define MS7750_H

/* flash.c */

/* 必要に応じて、ここにフラッシュROM関連の定義を追加してください */

/* ????????.c */

/* 以下は、これらの関数を外部から参照する場合に必要です */

BOOL read_ethernet_addr(void);
BOOL read_ip_addr(void);


/* LED set/clear functions defined here */
#define LED_PORT        0xB0C00000      /* LED port address */

void led_out(unsigned int);
#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))

#endif /* MS7750_H */
