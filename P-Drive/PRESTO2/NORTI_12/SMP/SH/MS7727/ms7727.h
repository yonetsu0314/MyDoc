/****************************************************************************
* MS7727SE01 ’è‹`                                                           *
*                                                                           *
*  File name : ms7727.h                                                     *
*  Copyright (c) 2003, MiSPO Co., Ltd.                                      *
*                                                                           *
* 26/Jun/2003 Changed the name of this file to ms7727.h.                 KA *
****************************************************************************/

#ifndef MS7727_H
#define MS7727_H

/* LED set/clear functions defined here */
#define LED_PORT        0xB0C00000      /* LED port address */

void led_out(unsigned int);
#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))

#endif /* MS7727_H */
