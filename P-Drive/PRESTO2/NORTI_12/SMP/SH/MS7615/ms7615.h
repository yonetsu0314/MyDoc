/******************************************************************************
* MS7615SE01 (SH7615) ボード固有定義                                          *
*                                                                             *
*  File name : ms7615.h                                                       *
*  Copyright (c) 2000 MiSPO Co., Ltd.  All rights reserved.                   *
*                                                                             *
* 17/Apr/2000                                                           MiSPO *
******************************************************************************/

#ifndef MS7615_H
#define MS7615_H

#define LED_PORT        0x22200000      /* LED port address */

void led_out(UH);

/* LED set/clear functions defined here */
#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))

#endif /* MS7615_H */
