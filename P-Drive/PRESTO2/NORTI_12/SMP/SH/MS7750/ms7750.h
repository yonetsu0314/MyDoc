/******************************************************************************
* MS7750SE01 (SH7750) �{�[�h�ŗL��`                                          *
*                                                                             *
*  File name : ms7750.h                                                       *
*  Copyright (c) 1999, MiSPO Co., Ltd.                                        *
*                                                                             *
* 29/Nov/1999 Created for MS7750SE01 board.                       [YT/Tygnus] *
******************************************************************************/

#ifndef MS7750_H
#define MS7750_H

/* flash.c */

/* �K�v�ɉ����āA�����Ƀt���b�V��ROM�֘A�̒�`��ǉ����Ă������� */

/* ????????.c */

/* �ȉ��́A�����̊֐����O������Q�Ƃ���ꍇ�ɕK�v�ł� */

BOOL read_ethernet_addr(void);
BOOL read_ip_addr(void);


/* LED set/clear functions defined here */
#define LED_PORT        0xB0C00000      /* LED port address */

void led_out(unsigned int);
#define led_out(n) (void)(*(volatile UH *)LED_PORT = (UH)(n))

#endif /* MS7750_H */
