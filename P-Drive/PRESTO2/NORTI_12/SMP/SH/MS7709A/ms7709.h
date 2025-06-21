/******************************************************************************
* ‚l‚r‚V‚V‚O‚X‚q‚o‚O‚P ’è‹`                                                   *
*                                                                             *
*  File name : ms7709.h                                                       *
*  Copyright (c) 1997, MiSPO Co., Ltd.                                        *
*                                                                             *
* 30/Nov/1998                                                                 *
* 25/Apr/1999                                                           MiSPO *
* 18/May/1999 Changed the name of this file to ms7709.h.                [YT]  *
******************************************************************************/

#ifndef MS7709_H
#define MS7709_H

/* no3flash.c */
                    /* Flash memory address */
#define FLUSH 0x01000000

BOOL flash_id(UH *maker, UH *device);
BOOL flash_erase(UW addr, W size);
BOOL flash_program(UW addr, void *data, W size);
BOOL flash_clear(UW addr, W size);

#endif /* MS7709_H */
