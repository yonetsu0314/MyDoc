/****************************************************************************
* MS7729RSE01 ’è‹`                                                          *
*                                                                           *
*  File name : ms7729R.h                                                    *
*  Copyright (c) 2003, MiSPO Co., Ltd.                                      *
*                                                                           *
* 26/Jun/2003 Changed the name of this file to ms7729R.h.                KA *
****************************************************************************/

#ifndef MS7729R_H
#define MS7729R_H

/* LED set/clear functions defined here */
#define LED_PORT        0xB0C00000      /* LED port address(Write Only) */

void led_out(unsigned int);
#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))

#endif /* MS7729R_H */
